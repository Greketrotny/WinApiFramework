#include "WinApiWindow.h"
#include "WinApiFramework.h"

#include <algorithm>		// for min()/max()
#include <string>

using namespace WinApiFramework;

// [CLASS] Window ------------------------------|
// -- fields -- //
// [STRUCT] ControlsStorage----------------|
// -- constructors -- //
Window::ControlsStorage::ControlsStorage()
{

}
Window::ControlsStorage::~ControlsStorage()
{

}

// -- methods -- //
void Window::ControlsStorage::AddControl(WindowControl* newControl)
{
	this->controls.push_back(newControl);
}
void Window::ControlsStorage::RemoveControl(WindowControl* oldControl)
{
	for (unsigned int i = 0; i < controls.size(); i++)
	{
		if (controls[i] == oldControl)
		{
			controls.erase(controls.begin() + i);
			return;
		}
	}
}
// [STRUCT] ControlsStorage----------------|


// -- constructor -- //
Window::Window()
	:Mouse(mouse),
	Keyboard(keyboard),
	WndHandle(hWindow),
	IsMainWindow(isMainWindow),
	IsEnabled(isEnabled),
	IsFocused(isFocused),
	IsMinimized(isMinimized),
	Id(window_id),
	X(rect.x), Y(rect.y),
	Width(rect.width), Height(rect.height),
	Caption(caption),
	MinWidth(sizeRect.minWidth), MinHeight(sizeRect.minHeight),
	MaxWidth(sizeRect.maxWidth), MaxHeight(sizeRect.maxHeight)
{
	// register self in framework
	Framework::AddWindow(this);
}
Window::Window(Window::Config config)
	:Window()
{
	rect = config.rect;
	position = config.position;
	sizeRect = config.sizeRect;
	caption = config.caption;

	// create window class
	CreateAndRegisterWindowClass();

	// create window
	CreateWinApiWindow();
}
Window::~Window()
{
	for (WindowControl *C : controls.controls)
	{
		if (C != nullptr)
			C->DestroyControlWindow();
	}

	SendMessage(hWindow, WM_CLOSE, 0, 0);
	UnregisterClass(window_class_name.c_str(), Framework::hProgramInstance);
	Framework::RemoveWindow(this);
}


// -- methods -- //
// private:
LRESULT Window::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// base events //
	case WM_CLOSE:
		events.PushEvent(Window::Event(Event::Type::Close));
		DestroyWindow(hWindow);
		hWindow = NULL;
		break;
	case WM_DESTROY:
		if (this == Framework::mainWindow)
			PostQuitMessage(0);
		break;

		// on window controls events //
	case WM_COMMAND:
	case WM_NOTIFY:
		for (WindowControl *control : controls.controls)
		{
			if (control->hControl == (HWND)lParam)
			{
				if (!control->ControlProc(wParam, lParam))
					return 0;
			}
		}
		break;

		// window properties events //
	case WM_MOVE:
	{
		RECT r;
		if (GetWindowRect(hWindow, &r))
		{
			rect.x = r.left;
			rect.y = r.top;
			rect.width = r.right - r.left;
			rect.height = r.bottom - r.top;
		}
		events.PushEvent(Window::Event(Window::Event::Type::Move));
	}
	break;

	case WM_SIZE:
	{
		RECT r;
		if (GetWindowRect(hWindow, &r))
		{
			rect.x = r.left;
			rect.y = r.top;
			rect.width = r.right - r.left;
			rect.height = r.bottom - r.top;
		}
		events.PushEvent(Window::Event(Window::Event::Type::Resize));
	}
	break;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
		mmi->ptMinTrackSize.x = sizeRect.minWidth;
		mmi->ptMinTrackSize.y = sizeRect.minHeight;
		mmi->ptMaxTrackSize.x = sizeRect.maxWidth;
		mmi->ptMaxTrackSize.y = sizeRect.maxHeight;
	}
	break;


	// Keyboard events //
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || keyboard.autorepeat)
		{
			keyboard.KeyPress((unsigned char)wParam);
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		keyboard.KeyRelase((unsigned char)(wParam));
		break;

	case WM_CHAR:
		keyboard.CharInput((wchar_t)wParam);
		break;


		// Mouse events //
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		if (pt.x > (int)0 && pt.x < (int)rect.width && pt.y >(int)0 && pt.y < (int)rect.height)
		{
			mouse.Move(pt.x, pt.y);
			if (!mouse.onWindow)
			{
				//SetCapture(hWindow);
				mouse.onWindow = true;
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				/*pt.x = (0 > pt.x) ? 0 : pt.x;
				pt.x = ((int)windowRect.width < pt.x) ? (int)windowRect.width : pt.x;
				pt.y = (0 > pt.y) ? 0 : pt.y;
				pt.y = ((int)windowRect.height < y) ? (int)windowRect.height : y;*/
				mouse.Move(pt.x, pt.y);
			}
			else
			{
				//ReleaseCapture();
				mouse.onWindow = false;
				mouse.isLeftPressed = false;
				mouse.isRightPressed = false;
				mouse.isMiddlePressed = false;
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
		mouse.LeftPressedEvent();
		break;
	case WM_RBUTTONDOWN:
		mouse.RightPressedEvent();
		break;
	case WM_LBUTTONUP:
		mouse.LeftRelasedEvent();
		break;
	case WM_RBUTTONUP:
		mouse.RightRelasedEvent();
		break;
	case WM_MBUTTONDOWN:
		mouse.MiddlePressedEvent();
		break;
	case WM_MBUTTONUP:
		mouse.MiddleRelasedEvent();
		break;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse.WeelUpEvent();
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			mouse.WeelDownEvent();
		}


		// default event process //
	default:
		return 1;	// if the window did't handle message
	}
	return 0;		// if did
}
bool Window::CreateAndRegisterWindowClass()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.hInstance = Framework::hProgramInstance;
	wc.lpfnWndProc = (WNDPROC)Framework::WinApiProcedure;
	wc.lpszClassName = window_class_name.c_str();
	wc.lpszMenuName = nullptr;
	wc.cbSize = (sizeof(WNDCLASSEX));
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);

	// register class
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Register window class failed.", L"Register error", MB_OK | MB_ICONERROR);
		false;
	}
	return true;
}
bool Window::CreateWinApiWindow()
{
	// setup rect like user specified
	RECT r;
	if (position == Position::Custom)
	{
		r = { (LONG)rect.x, (LONG)rect.y,
			(LONG)(rect.x + rect.width), (LONG)(rect.y + rect.height) };

		AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
	}
	else if (position == Position::Center)
	{
		unsigned int w = GetSystemMetrics(SM_CXSCREEN);
		unsigned int h = GetSystemMetrics(SM_CYSCREEN);

		r.left = (w - std::min(rect.width, w)) / 2;
		r.top = (h - std::min(rect.height, h)) / 2;
		r.right = r.left + rect.width;
		r.bottom = r.top + rect.height;

		AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

		rect.x = r.left;
		rect.y = r.top;
	}
	else if (position == Position::Default)
	{
		r.left = 100;
		r.top = 100;
		r.right = r.left + rect.width;
		r.bottom = r.top + rect.height;
	}

	// create window
	hWindow = CreateWindow(window_class_name.c_str(), caption.c_str(),
		WS_OVERLAPPEDWINDOW/*WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/,
		r.left, r.top, r.right - r.left, r.bottom - r.top,
		nullptr, nullptr, Framework::hProgramInstance, nullptr);

	if (!hWindow)
	{
		MessageBox(nullptr, L"Window creation failed.", L"Window create error", MB_OK | MB_ICONERROR);
		return false;
	}

	SetCaption(GetCaption());

	ShowWindow(hWindow, SW_SHOWDEFAULT);
	UpdateWindow(hWindow);
	return true;
}

// public:
void Window::PushEvent(Window::Event newEvent)
{
	events.PushEvent(newEvent);
}
Window::Event Window::GetEvent()
{
	return events.GetEvent();
}
void Window::ClearEventBuffer()
{
	events.ClearBuffer();
}
void Window::SetCaption(std::wstring new_caption)
{
	caption = new_caption;
	SetWindowText(hWindow, (((isMainWindow) ? L"[Main Window] " : L"") + caption).c_str());
	events.PushEvent(Window::Event(Event::Type::CaptionChange));
}
void Window::SetPosition(unsigned int x, unsigned int y)
{
	rect.x = x;
	rect.y = y;

	SetWindowPos(hWindow, nullptr,
		rect.x, rect.y,
		rect.width, rect.height,
		0);

	events.PushEvent(Window::Event(Event::Type::Move));
}
void Window::SetDimensions(unsigned int width, unsigned int height)
{
	rect.width = width;
	rect.height = height;

	SetWindowPos(hWindow, nullptr,
		rect.x, rect.y,
		rect.width, rect.height,
		0);

	events.PushEvent(Window::Event(Event::Type::Resize));
}
void Window::SetRect(Window::Config newWindowRect)
{
	// reset window rect properties
	if (newWindowRect.position == Position::Custom)
	{
		rect.x = newWindowRect.rect.x;
		rect.y = newWindowRect.rect.y;
		rect.width = newWindowRect.rect.width;
		rect.height = newWindowRect.rect.height;
	}
	else if (newWindowRect.position == Position::Center)
	{
		unsigned int w = GetSystemMetrics(SM_CXSCREEN);
		unsigned int h = GetSystemMetrics(SM_CYSCREEN);

		newWindowRect.rect.width = std::min(newWindowRect.rect.width, w);
		newWindowRect.rect.height = std::min(newWindowRect.rect.height, h);

		rect.x = (w - newWindowRect.rect.width) / 2;
		rect.y = (h - newWindowRect.rect.height) / 2;
		rect.width = newWindowRect.rect.width;
		rect.height = newWindowRect.rect.height;
	}
	else if (newWindowRect.position == Position::Default)
	{
		rect.x = 100;
		rect.y = 100;
		rect.width = newWindowRect.rect.width;
		rect.height = newWindowRect.rect.height;
	}

	SetPosition(rect.x, rect.y);
	SetDimensions(rect.width, rect.height);
}
void Window::SetMinSize(unsigned int minWidth, unsigned int minHeight)
{
	sizeRect.minWidth = minWidth;
	sizeRect.minHeight = minHeight;
}
void Window::SetMaxSize(unsigned int maxWidth, unsigned int maxHeight)
{
	sizeRect.maxWidth = maxWidth;
	sizeRect.maxHeight = maxHeight;
}
void Window::SetSizeRect(Window::SizeRect newSizeRect)
{
	sizeRect = newSizeRect;
}
void Window::SetAsMainWindow()
{
	Framework::SetAsMainWindow(this);
}
void Window::EnableWindow()
{
	if (isMainWindow) return;
	if (isEnabled) return;

	::EnableWindow(hWindow, 1);
	isEnabled = true;

	events.PushEvent(Window::Event(Event::Type::Enable));
}
void Window::DisableWindow()
{
	if (isMainWindow) return;
	if (!isEnabled) return;

	::EnableWindow(hWindow, 0);
	isEnabled = false;

	events.PushEvent(Window::Event(Event::Type::Disable));
}
void Window::SetEventHandler(Window::EventHandler *eh)
{
	this->events.eventHandler = eh;
}
int Window::ShowMessageBox(std::wstring text, std::wstring caption, UINT message_box_style)
{
	return MessageBoxW(hWindow, text.c_str(), caption.c_str(), message_box_style);
}

unsigned int Window::GetXPos() const
{
	return rect.x;
}
unsigned int Window::GetYPos() const
{
	return rect.y;
}
unsigned int Window::GetWidth() const
{
	return rect.width;
}
unsigned int Window::GetHeight() const
{
	return rect.height;
}
const HWND& Window::GetWindowHandle() const
{
	return hWindow;
}
const std::wstring& Window::GetCaption() const
{
	return caption;
}

void Window::AddControl(WindowControl* newControl)
{
	if (newControl == nullptr) return;
	if (newControl->parentWindow != nullptr) return;

	controls.AddControl(newControl);
	newControl->parentWindow = this;
	newControl->CreateControlWindow();

	events.PushEvent(Window::Event(Event::Type::ControlAdd));
}
void Window::RemoveControl(WindowControl* oldControl)
{
	if (oldControl == nullptr) return;

	controls.RemoveControl(oldControl);
	oldControl->DestroyControlWindow();

	events.PushEvent(Window::Event(Event::Type::ControlRemove));
}
// [CLASS] Window ------------------------------|