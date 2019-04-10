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
	:WndHandle(hWindow),
	IsMainWindow(isMainWindow),
	IsEnabled(isEnabled),
	IsActivated(isActivated),
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
	CreateWinApiWindow(config);
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
		return 0;
		break;

		// base events //
	case WM_CLOSE:
		events.PushEvent(Window::Event(Event::Type::Close));
		DestroyWindow(hWindow);
		hWindow = NULL;
		return 0;
		break;
	case WM_DESTROY:
		if (this == Framework::mainWindow)
			PostQuitMessage(0);
		return 0;
		break;


		// window properties events //
	case WM_ACTIVATE:
		if (wParam & WA_INACTIVE)
		{
			isActivated = false;
			PushEvent(Window::Event::Type::Deactivate);
		}
		else
		{
			isActivated = true;
			PushEvent(Window::Event::Type::Activate);
		}
		return 0;
		break;

	case WM_SHOWWINDOW:
		if (wParam == TRUE)
		{
			PushEvent(Window::Event::Type::Show);
		}
		else
		{
			PushEvent(Window::Event::Type::Hide);
		}
		break;

	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_MAXIMIZE:
			PushEvent(Window::Event::Type::Maximize);
			isMinimized = false;
			break;
		case SC_MINIMIZE:
			PushEvent(Window::Event::Type::Minimize);
			isMinimized = true;
			break;
		}
		break;

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
		return 0;
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
		return 0;
	}
	break;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
		mmi->ptMinTrackSize.x = sizeRect.minWidth;
		mmi->ptMinTrackSize.y = sizeRect.minHeight;
		mmi->ptMaxTrackSize.x = sizeRect.maxWidth;
		mmi->ptMaxTrackSize.y = sizeRect.maxHeight;
		return 0;
	}
	break;


	//	// Mouse events //
	//case WM_MOUSEMOVE:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	if (pt.x > 2 && pt.x < (int)rect.width - 2 && pt.y > 2 && pt.y < (int)rect.height - 2)
	//	{
	//		if (!mouseOnWindow)
	//		{
	//			SetCapture(hWindow);
	//			mouseOnWindow = true;
	//		}
	//	}
	//	else
	//	{
	//		if (wParam & (MK_LBUTTON | MK_RBUTTON))
	//		{
	//			/*pt.x = (0 > pt.x) ? 0 : pt.x;
	//			pt.x = ((int)windowRect.width < pt.x) ? (int)windowRect.width : pt.x;
	//			pt.y = (0 > pt.y) ? 0 : pt.y;
	//			pt.y = ((int)windowRect.height < y) ? (int)windowRect.height : y;*/
	//		}
	//		else
	//		{
	//			ReleaseCapture();
	//			mouseOnWindow = false;
	//			Framework::Mouse.isLeftPressed = false;
	//			Framework::Mouse.isRightPressed = false;
	//			Framework::Mouse.isMiddlePressed = false;
	//		}
	//	}
	//	break;
	//}

	}
	return 1;	// if the window did't handle message
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
	wc.style = 0;
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
bool Window::CreateWinApiWindow(Window::Config config)
{
	// set start window style
	if (config.startStyle == Window::StartStyle::Maximized)
		windowStyle |= WS_MAXIMIZE;
	else if (config.startStyle == Window::StartStyle::Minimized)
		windowStyle |= WS_MINIMIZE;

	// setup window rect
	RECT r;
	if (position == Position::Custom)
	{
		r = { (LONG)rect.x, (LONG)rect.y,
			(LONG)(rect.x + rect.width), (LONG)(rect.y + rect.height) };

		AdjustWindowRect(&r, windowStyle, FALSE);
	}
	else if (position == Position::Center)
	{
		unsigned int w = GetSystemMetrics(SM_CXSCREEN);
		unsigned int h = GetSystemMetrics(SM_CYSCREEN);

		r.left = (w - std::min(rect.width, w)) / 2;
		r.top = (h - std::min(rect.height, h)) / 2;
		r.right = r.left + rect.width;
		r.bottom = r.top + rect.height;

		AdjustWindowRect(&r, windowStyle, FALSE);

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
		windowStyle,
		r.left, r.top, r.right - r.left, r.bottom - r.top,
		nullptr, nullptr, Framework::hProgramInstance, nullptr);

	if (!hWindow)
	{
		MessageBox(nullptr, L"Window creation failed.", L"Window create error", MB_OK | MB_ICONERROR);
		return false;
	}

	SetCaption(GetCaption());

	ShowWindow(hWindow, SW_SHOW);
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
void Window::Enable()
{
	if (isMainWindow) return;
	if (isEnabled) return;

	::EnableWindow(hWindow, 1);
	isEnabled = true;

	events.PushEvent(Window::Event(Event::Type::Enable));
}
void Window::Disable()
{
	if (isMainWindow) return;
	if (!isEnabled) return;

	::EnableWindow(hWindow, 0);
	isEnabled = false;

	events.PushEvent(Window::Event(Event::Type::Disable));
}
void Window::EnableResize()
{
	LONG windowStyle = GetWindowLong(hWindow, GWL_STYLE);
	windowStyle = windowStyle | WS_SIZEBOX;
	SetWindowLong(hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::EnableResize);
}
void Window::DisableResize()
{
	LONG windowStyle = GetWindowLong(hWindow, GWL_STYLE);
	windowStyle = windowStyle & (~WS_SIZEBOX);
	SetWindowLong(hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::DisableResize);
}
void Window::EnableMaximizeBox()
{
	LONG windowStyle = GetWindowLong(hWindow, GWL_STYLE);
	windowStyle = windowStyle | WS_MAXIMIZEBOX;
	SetWindowLong(hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::EnableMaximizeBox);
}
void Window::DisableMaximizeBox()
{
	LONG windowStyle = GetWindowLong(hWindow, GWL_STYLE);
	windowStyle = windowStyle & (~WS_MAXIMIZEBOX);
	SetWindowLong(hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::DisableMaximizeBox);
}
void Window::EnableMinimizeBox()
{
	LONG windowStyle = GetWindowLong(hWindow, GWL_STYLE);
	windowStyle = windowStyle | WS_MINIMIZEBOX;
	SetWindowLong(hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::EnableMinimizeBox);
}
void Window::DisableMinimizeBox()
{
	LONG windowStyle = GetWindowLong(hWindow, GWL_STYLE);
	windowStyle = windowStyle & (~WS_MINIMIZEBOX);
	SetWindowLong(hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::DisableMinimizeBox);
}
void Window::Activate()
{
	SetActiveWindow(hWindow);
}
void Window::Maximize()
{
	ShowWindow(hWindow, SW_MAXIMIZE);
}
void Window::Minimize()
{
	ShowWindow(hWindow, SW_MINIMIZE);
}
void Window::Show()
{
	ShowWindow(hWindow, SW_SHOW);
}
void Window::Hide()
{
	ShowWindow(hWindow, SW_HIDE);
}
void Window::SetEventHandler(Window::EventHandler *eh)
{
	this->events.eventHandler = eh;
}
int Window::ShowMessageBox(std::wstring text, std::wstring caption, UINT message_box_style)
{
	return MessageBoxW(hWindow, text.c_str(), caption.c_str(), message_box_style);
}

unsigned int Window::GetX() const
{
	return rect.x;
}
unsigned int Window::GetY() const
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
int Window::GetMouseX() const
{
	return Framework::Mouse.X - this->X;
}
int Window::GetMouseY() const
{
	return Framework::Mouse.Y - this->Y;
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