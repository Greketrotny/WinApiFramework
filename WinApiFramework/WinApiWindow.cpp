#include "Precompiled.h"
#include "WinApiWindow.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;

#undef IsMinimized

// ~~~~~~~~ [CLASS] Window ~~~~~~~~
// -- constructor -- //
Window::Window()
	: WndHandle(m_hWindow)
	, IsMainWindow(isMainWindow)
	, IsEnabled(isEnabled)
	, IsActivated(isActivated)
	, IsMinimized(isMinimized)
	, Id(window_id)
	, WindowRect(windowRect)
	, ClientRect(clientRect)
	, WindowSizeRect(sizeRect)
	, Caption(caption)
	, Events(events)
{
	// register self in framework
	Framework::AddWindow(this);
}
Window::Window(const ConStruct<Window> &conStruct)
	:Window()
{
	windowRect = conStruct.rect;
	SetSizeRect(conStruct.sizeRect);
	position = conStruct.position;
	caption = conStruct.caption;

	// create window class
	CreateAndRegisterWindowClass();

	// create window
	CreateWinApiWindow(conStruct);
}
Window::~Window()
{
	// destroy all controls
	for (size_t i = 0; i < controls.size(); i++)
	{
		DestroyControlAsParent(controls[i]);
	}
	controls.clear();

	// destroy window
	SendMessage(m_hWindow, WM_CLOSE, 0, 0);
	UnregisterClass(window_class_name.c_str(), Framework::hProgramInstance);
	Framework::RemoveWindow(this);
}


// -- methods -- //
// private:
LRESULT Window::WndProcedure(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)	
	{
		// on window controls events //
	case WM_COMMAND:
	case WM_NOTIFY:
	case WM_HSCROLL:
	case WM_VSCROLL:
		for (ChildControl*& control : controls)
		{
			if (control->GetWindowHandle() == (HWND)lParam)
			{
				if (!control->ControlProcedure(wParam, lParam))
					return 0;
			}
		}
		return 1;
		break;

		// base events //
	case WM_CLOSE:
		events.PushEvent(Window::Event(Event::Type::Close));
		DestroyWindow(m_hWindow);
		m_hWindow = NULL;
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
		if (GetWindowRect(m_hWindow, &r))
		{
			windowRect.position.x = r.left;
			windowRect.position.y = r.top;
			windowRect.size.width = r.right - r.left;
			windowRect.size.height = r.bottom - r.top;
		}
		if (GetClientRect(m_hWindow, &r))
		{
			POINT p{ 0, 0 };
			ClientToScreen(m_hWindow, &p);
			clientRect.position.x = p.x;
			clientRect.position.y = p.y;
			clientRect.size.width = r.right - r.left;
			clientRect.size.height = r.bottom - r.top;
		}
		events.PushEvent(Window::Event(Window::Event::Type::Move));
		return 0;
	}
	break;

	case WM_SIZE:
	{
		RECT r;
		if (GetWindowRect(m_hWindow, &r))
		{
			windowRect.position.x = r.left;
			windowRect.position.y = r.top;
			windowRect.size.width = r.right - r.left;
			windowRect.size.height = r.bottom - r.top;
		}
		if (GetClientRect(m_hWindow, &r))
		{
			POINT p{ 0, 0 };
			ClientToScreen(m_hWindow, &p);
			clientRect.position.x = p.x;
			clientRect.position.y = p.y;
			clientRect.size.width = r.right - r.left;
			clientRect.size.height = r.bottom - r.top;
		}
		events.PushEvent(Window::Event(Window::Event::Type::Resize));
		return 0;
	}
	break;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
		mmi->ptMinTrackSize.x = sizeRect.minSize.width;
		mmi->ptMinTrackSize.y = sizeRect.minSize.height;
		mmi->ptMaxTrackSize.x = sizeRect.maxSize.width;
		mmi->ptMaxTrackSize.y = sizeRect.maxSize.height;
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
	//			SetCapture(m_hWindow);
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
bool Window::CreateWinApiWindow(ConStruct<Window> conStruct)
{
	// set start window style
	if (conStruct.startStyle == Window::StartStyle::Maximized)
		windowStyle |= WS_MAXIMIZE;
	else if (conStruct.startStyle == Window::StartStyle::Minimized)
		windowStyle |= WS_MINIMIZE;

	// setup window rect
	RECT r = { (LONG)windowRect.position.x, (LONG)windowRect.position.y, (LONG)(windowRect.position.x + windowRect.size.width), (LONG)(windowRect.position.y + windowRect.size.height) };
	AdjustWindowRect(&r, windowStyle, FALSE);
	windowRect.position.x = r.left;
	windowRect.position.y = r.top;
	windowRect.size.width = r.right - r.left;
	windowRect.size.height = r.bottom - r.top;

	if (position == Position::Center)
	{
		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		windowRect.position.x = (w - std::min(windowRect.size.width, w)) / 2;
		windowRect.position.y = (h - std::min(windowRect.size.height, h)) / 2;
	}
	else if (position == Position::Default)
	{
		windowRect.position.x = 100;
		windowRect.position.y = 100;
	}

	// create window
	m_hWindow = CreateWindow((LPCWSTR)window_class_name.c_str(), (LPCWSTR)caption.c_str(),
		windowStyle,
		windowRect.position.x, windowRect.position.y, windowRect.size.width, windowRect.size.height,
		nullptr, nullptr, Framework::hProgramInstance, nullptr);

	if (!m_hWindow)
	{
		MessageBox(nullptr, L"Window creation failed.", L"Window create error", MB_OK | MB_ICONERROR);
		return false;
	}

	SetCaption(GetCaption());

	ShowWindow(m_hWindow, SW_SHOW);
	UpdateWindow(m_hWindow);

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
void Window::EnableEventHandlers()
{
	events.EnableEventHandlers();
}
void Window::DisableEventHandlers()
{
	events.DisableEventHandlers();
}

void Window::SetCaption(std::wstring new_caption)
{
	caption = new_caption;
	SetWindowText(m_hWindow, (LPCWSTR)(((isMainWindow) ? L"[Main Window] " : L"") + caption).c_str());
	events.PushEvent(Window::Event(Event::Type::CaptionChange));
}
void Window::SetPosition(unsigned int x, unsigned int y)
{
	windowRect.position.x = x;
	windowRect.position.y = y;

	SetWindowPos(m_hWindow, nullptr,
		windowRect.position.x, windowRect.position.y,
		windowRect.size.width, windowRect.size.height,
		0);

	events.PushEvent(Window::Event(Event::Type::Move));
}
void Window::SetDimensions(unsigned int width, unsigned int height)
{
	windowRect.size.width = width;
	windowRect.size.height = height;

	SetWindowPos(m_hWindow, nullptr,
		windowRect.position.x, windowRect.position.y,
		windowRect.size.width, windowRect.size.height,
		0);

	events.PushEvent(Window::Event(Event::Type::Resize));
}
void Window::SetMinSize(unsigned int minWidth, unsigned int minHeight)
{
	sizeRect.minSize.width = minWidth;
	sizeRect.minSize.height = minHeight;

	if (sizeRect.minSize.width > sizeRect.maxSize.width) sizeRect.minSize.width = sizeRect.maxSize.width;
	if (sizeRect.minSize.height > sizeRect.maxSize.height) sizeRect.minSize.height = sizeRect.maxSize.height;
}
void Window::SetMaxSize(unsigned int maxWidth, unsigned int maxHeight)
{
	sizeRect.maxSize.width = maxWidth;
	sizeRect.maxSize.height = maxHeight;

	if (sizeRect.maxSize.width < sizeRect.minSize.width)  sizeRect.maxSize.width = sizeRect.minSize.width;
	if (sizeRect.maxSize.height < sizeRect.minSize.height)  sizeRect.maxSize.height = sizeRect.minSize.height;
}
void Window::SetSizeRect(SizeRect newSizeRect)
{
	sizeRect = newSizeRect;

	if (sizeRect.maxSize.width < sizeRect.minSize.width) sizeRect.maxSize.width = sizeRect.minSize.width;
	if (sizeRect.maxSize.height < sizeRect.minSize.height) sizeRect.maxSize.height = sizeRect.minSize.height;
}
void Window::SetAsMainWindow()
{
	Framework::SetAsMainWindow(this);
}

void Window::Enable()
{
	if (isMainWindow) return;
	if (isEnabled) return;

	::EnableWindow(m_hWindow, 1);
	isEnabled = true;

	events.PushEvent(Window::Event(Event::Type::Enable));
}
void Window::Disable()
{
	if (isMainWindow) return;
	if (!isEnabled) return;

	::EnableWindow(m_hWindow, 0);
	isEnabled = false;

	events.PushEvent(Window::Event(Event::Type::Disable));
}
void Window::EnableResize()
{
	windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
	windowStyle = windowStyle | WS_SIZEBOX;
	SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::EnableResize);
}
void Window::DisableResize()
{
	windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
	windowStyle = windowStyle & (~WS_SIZEBOX);
	SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::DisableResize);
}
void Window::EnableMaximizeBox()
{
	windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
	windowStyle = windowStyle | WS_MAXIMIZEBOX;
	SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::EnableMaximizeBox);
}
void Window::DisableMaximizeBox()
{
	windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
	windowStyle = windowStyle & (~WS_MAXIMIZEBOX);
	SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::DisableMaximizeBox);
}
void Window::EnableMinimizeBox()
{
	windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
	windowStyle = windowStyle | WS_MINIMIZEBOX;
	SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::EnableMinimizeBox);
}
void Window::DisableMinimizeBox()
{
	windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
	windowStyle = windowStyle & (~WS_MINIMIZEBOX);
	SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

	events.PushEvent(Window::Event::Type::DisableMinimizeBox);
}
void Window::Activate()
{
	SetActiveWindow(m_hWindow);
}
void Window::Maximize()
{
	ShowWindow(m_hWindow, SW_MAXIMIZE);
}
void Window::Minimize()
{
	ShowWindow(m_hWindow, SW_MINIMIZE);
}
void Window::Show()
{
	ShowWindow(m_hWindow, SW_SHOW);
}
void Window::Hide()
{
	ShowWindow(m_hWindow, SW_HIDE);
}
int Window::ShowMessageBox(std::wstring text, std::wstring caption, UINT message_box_style)
{
	return MessageBoxW(m_hWindow, text.c_str(), caption.c_str(), message_box_style);
}

const HWND& Window::GetWindowHandle() const
{
	return m_hWindow;
}
const std::wstring& Window::GetCaption() const
{
	return caption;
}

bool Window::AddControl(ChildControl* newControl)
{
	if (newControl == nullptr) return false;

	controls.push_back(newControl);
	events.PushEvent(Window::Event(Event::Type::ControlAdded));

	return true;
}
bool Window::RemoveControl(ChildControl* oldControl)
{
	if (oldControl == nullptr) return false;


	for (unsigned int i = 0; i < controls.size(); i++)
	{
		if (controls[i] == oldControl)
		{
			controls.erase(controls.begin() + i);
			events.PushEvent(Window::Event(Event::Type::ControlRemoved));
			return true;
		}
	}

	return false;
}

Point Window::GetMousePosition() const
{
	return Point(Framework::Mouse.X - ClientRect.position.x, Framework::Mouse.Y - ClientRect.position.y);
}
// [CLASS] Window ------------------------------|