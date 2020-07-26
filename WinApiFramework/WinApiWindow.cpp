#include "Precompiled.h"
#include "WinApiWindow.h"
#include "WinApiFramework.h"

#undef IsMinimized

namespace WinApiFramework
{
	// ~~~~~~~~ [CLASS] Window ~~~~~~~~ //
	Window::Window()
		: HasWindowProcedure(this, &Window::WindowProcedure)
		, WndHandle(m_hWindow)
		, IsMainWindow(isMainWindow)
		, IsEnabled(isEnabled)
		, IsActivated(isActivated)
		, IsMinimized(isMinimized)
		, Id(window_id)
		, WindowRect(windowRect)
		, ClientRect(clientRect)
		, WindowSizeRect(sizeRect)
		, Caption(caption)
		//, Events(events)
	{
	}
	Window::Window(unsigned int id, const ConStruct<Window> &conStruct)
		: Window()
	{
		window_id = id;
		m_WindowClassName = L"WindowClass" + std::to_wstring((window_id));

		caption = conStruct.caption;
		windowRect = conStruct.rect;
		SetSizeRect(conStruct.sizeRect);
		m_canvasRect = BoundRect(0, 0, 100, 100);
		m_canvasDrift = Point(0, 0);

		// create window class
		//CreateAndRegisterWindowClass();

		// create window
		//CreateWinApiWindow(conStruct);
	}
	Window::~Window()
	{
		// destroy all child controls
		DestroyAllChildControls();

		// destroy window
		::DestroyWindow(m_hWindow);
		m_hWindow = NULL;

		// unregister the window class
		UnregisterClass(m_WindowClassName.c_str(), Framework::GetProgramInstance());
	}


	// private:
	LRESULT Window::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			// on window controls events //
			case WM_COMMAND:
			case WM_NOTIFY:
				return ProcessChildMessage(wParam, lParam);

			case WM_VSCROLL:
			{
				// try to find and process message on child controls
				if (!ProcessChildMessage(wParam, lParam)) return 0;


				SCROLLINFO si;
				ZeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS | SIF_PAGE | SIF_TRACKPOS;
				GetScrollInfo(m_hWindow, SB_VERT, &si);

				int pos = si.nPos;

				switch (LOWORD(wParam))
				{
					case SB_TOP:
						pos = m_canvasRect.top;
						break;
					case SB_BOTTOM:
						pos = m_canvasRect.bottom;
						break;
					case SB_LINEUP:
						if (pos > m_canvasRect.top) pos--;
						break;
					case SB_LINEDOWN:
						if (pos < (m_canvasRect.bottom) - clientRect.size.height) pos++;
						break;
					case SB_PAGEUP:
						pos -= si.nPage;
						if (pos < m_canvasRect.top) pos = m_canvasRect.top;
						break;
					case SB_PAGEDOWN:
						pos += si.nPage;
						if (pos > m_canvasRect.bottom) pos = m_canvasRect.bottom;
						break;
					case SB_THUMBPOSITION:
						pos = si.nTrackPos;
						break;
					case SB_THUMBTRACK:
						pos = si.nTrackPos;
						break;
				}

				int dy = (pos - si.nPos);
				m_canvasDrift.y += dy;
				ScrollWindowEx(m_hWindow, 0, -dy,
					(CONST RECT*)NULL,
					(CONST RECT*)NULL,
					(HRGN)NULL, (LPRECT)NULL,
					SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
				UpdateWindow(m_hWindow);

				ZeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;
				si.nPos = pos;

				SetScrollInfo(m_hWindow, SB_VERT, &si, TRUE);

				RaiseEvent<EventTypeVScrolled>();
				return 0;
			}

			case WM_HSCROLL:
			{
				// try to find and process message on child controls
				if (!ProcessChildMessage(wParam, lParam)) return 0;


				SCROLLINFO si;
				ZeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS | SIF_PAGE | SIF_TRACKPOS;
				GetScrollInfo(m_hWindow, SB_HORZ, &si);

				int pos = si.nPos;

				switch (LOWORD(wParam))
				{
					case SB_LEFT:
						pos = m_canvasRect.left;
						break;
					case SB_RIGHT:
						pos = m_canvasRect.right;
						break;
					case SB_LINELEFT:
						if (pos > m_canvasRect.left) pos--;
						break;
					case SB_LINERIGHT:
						if (pos < (m_canvasRect.right) - clientRect.size.width) pos++;
						break;
					case SB_PAGELEFT:
						pos -= si.nPage;
						if (pos < m_canvasRect.left) pos = m_canvasRect.left;
						break;
					case SB_PAGERIGHT:
						pos += si.nPage;
						if (pos > m_canvasRect.right) pos = m_canvasRect.right;
						break;
					case SB_THUMBPOSITION:
						pos = si.nTrackPos;
						break;
					case SB_THUMBTRACK:
						pos = si.nTrackPos;
						break;
				}

				int dx = (pos - si.nPos);
				m_canvasDrift.x += dx;
				ScrollWindowEx(m_hWindow, -dx, 0,
					(CONST RECT*)NULL,
					(CONST RECT*)NULL,
					(HRGN)NULL, (LPRECT)NULL,
					SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
				UpdateWindow(m_hWindow);

				ZeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;
				si.nPos = pos;

				SetScrollInfo(m_hWindow, SB_HORZ, &si, TRUE);

				RaiseEvent<EventTypeHScrolled>();
				return 0;
			}

				// base events //
			case WM_CLOSE:
				RaiseEvent<EventTypeClose>();
				if (m_hWindow) ::DestroyWindow(m_hWindow);
				m_hWindow = NULL;
				return 0;

			case WM_DESTROY:
				if (this == Framework::GetRootWindow())
					PostQuitMessage(0);
				return 0;



				// window properties events //
			case WM_ACTIVATE:
			{
				if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
				{
					isActivated = true;
					RaiseEvent<EventTypeActivated>();
				}
				else
				{
					isActivated = false;
					RaiseEvent<EventTypeDeactivated>();
				}
				break;
			}

			case WM_SHOWWINDOW:
			{
				if (wParam == TRUE)
				{
					RaiseEvent<EventTypeShowed>();
				}
				else
				{
					RaiseEvent<EventTypeHid>();
				}
				return 0;
			}

			case WM_SYSCOMMAND:
				switch (wParam)
				{
					case SC_MAXIMIZE:
						isMinimized = false;
						RaiseEvent<EventTypeMaximized>();
						break;
					case SC_MINIMIZE:
						isMinimized = true;
						RaiseEvent<EventTypeMinimized>();
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

				RaiseEvent<EventTypeMoved>();
				return 0;
			}

			case WM_SIZE:
			{
				RECT r;
				// set window rect
				if (GetWindowRect(m_hWindow, &r))
				{
					windowRect.position.x = r.left;
					windowRect.position.y = r.top;
					windowRect.size.width = r.right - r.left;
					windowRect.size.height = r.bottom - r.top;
				}

				// set client rect
				if (GetClientRect(m_hWindow, &r))
				{
					POINT p{ 0, 0 };
					ClientToScreen(m_hWindow, &p);
					clientRect.position.x = p.x;
					clientRect.position.y = p.y;
					clientRect.size.width = r.right - r.left;
					clientRect.size.height = r.bottom - r.top;
				}


				// [>] Adjust canvas position
				Point deltaXY(0, 0);
				if (m_canvasRect.right - m_canvasDrift.x < clientRect.size.width)
				{
					deltaXY.x = std::min(m_canvasDrift.x - m_canvasRect.left, clientRect.size.width - (m_canvasRect.right - m_canvasDrift.x));
				}
				if (m_canvasRect.bottom - m_canvasDrift.y < clientRect.size.height)
				{
					deltaXY.y = std::min(m_canvasDrift.y - m_canvasRect.top, clientRect.size.height - (m_canvasRect.bottom - m_canvasDrift.y));
				}

				if (deltaXY.y != 0 || deltaXY.x != 0)
				{
					m_canvasDrift -= deltaXY;
					ScrollWindowEx(m_hWindow, deltaXY.x, deltaXY.y,
						(CONST RECT*)NULL,
						(CONST RECT*)NULL,
						(HRGN)NULL, (LPRECT)NULL,
						SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
					UpdateWindow(m_hWindow);
				}



				// [>] Set scroing info
				SCROLLINFO si;
				ZeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(SCROLLINFO);

				// vertical scroll
				si.nMin = m_canvasRect.top;
				si.nMax = m_canvasRect.bottom;
				si.nPage = clientRect.size.height;
				si.nPos = m_canvasDrift.y;
				si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
				SetScrollInfo(m_hWindow, SB_VERT, &si, TRUE);

				// horizontal scroll
				si.nMin = m_canvasRect.left;
				si.nMax = m_canvasRect.right;
				si.nPage = clientRect.size.width;
				si.nPos = m_canvasDrift.x;
				si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
				SetScrollInfo(m_hWindow, SB_HORZ, &si, TRUE);

				RaiseEvent<EventTypeResized>();
				return 0;
			}

			case WM_GETMINMAXINFO:
			{
				LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
				mmi->ptMinTrackSize.x = sizeRect.minSize.width;
				mmi->ptMinTrackSize.y = sizeRect.minSize.height;
				mmi->ptMaxTrackSize.x = sizeRect.maxSize.width;
				mmi->ptMaxTrackSize.y = sizeRect.maxSize.height;

				return 0;
			}


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
		return 1;
	}
	//LRESULT Window::ProcessChildMessage(WPARAM wParam, LPARAM lParam)
	//{
	//	/*for (ChildControl*& control : controls)
	//	{
	//		if (control->GetWindowHandle() == (HWND)lParam)
	//		{
	//			if (!control->ControlProcedure(wParam, lParam))
	//				return 0;
	//		}
	//	}
	//	return 1;*/
	//	/*for (ChildControl*& control : controls)
	//	{
	//		if (control->GetWindowHandle() == (HWND)lParam)
	//		{
	//			control->ControlProcedure(wParam, lParam);
	//			return 0;
	//		}
	//	}
	//	return 1;*/
	//
	//	/*for (size_t i = 0; i < GetControlCount(); ++i)
	//	{
	//		switch (GetChildControl(i)->ControlProcedure(wParam, lParam))
	//		{
	//			case ProcedureResult::Handled: return 0;
	//			case ProcedureResult::Unhandled: return 1;
	//			case ProcedureResult::TargetNotFound: continue;
	//		}
	//	}
	//	return 1;*/
	//}
	bool Window::CreateWinApiWindow(const ConStruct<Window>& conStruct)
	{
		// [>] Create WindowClassEx
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.hInstance = Framework::GetProgramInstance();
		wc.lpfnWndProc = GetWinApiProcedure();
		wc.lpszClassName = m_WindowClassName.c_str();
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
			return false;
		}


		// [>] Set window styles
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

		if (conStruct.position == Position::Center)
		{
			int w = GetSystemMetrics(SM_CXSCREEN);
			int h = GetSystemMetrics(SM_CYSCREEN);

			windowRect.position.x = (w - std::min(windowRect.size.width, w)) / 2;
			windowRect.position.y = (h - std::min(windowRect.size.height, h)) / 2;
		}
		else if (conStruct.position == Position::Default)
		{
			windowRect.position.x = 100;
			windowRect.position.y = 100;
		}


		// [>] Create window
		m_hWindow = CreateWindow((LPCWSTR)m_WindowClassName.c_str(), (LPCWSTR)caption.c_str(),
			windowStyle,
			windowRect.position.x, windowRect.position.y, windowRect.size.width, windowRect.size.height,
			nullptr, nullptr, Framework::GetProgramInstance(), nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"Window creation failed.", L"Window create error", MB_OK | MB_ICONERROR);
			return false;
		}

		// set pointer to Window class to receive messages
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_windowProcedure);

		// set window scrolling
		SetWindowLongPtr(m_hWindow, GWL_STYLE, windowStyle | WS_HSCROLL | WS_VSCROLL);
		SetWindowPos(m_hWindow, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);


		// [>] Setup window scrolling
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);

		// vertical
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = m_canvasRect.top;
		si.nMax = m_canvasRect.bottom;
		si.nPage = clientRect.size.height;
		si.nPos = m_canvasDrift.y;
		SetScrollInfo(m_hWindow, SB_VERT, &si, TRUE);

		// horizontal
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = m_canvasRect.left;
		si.nMax = m_canvasRect.right;
		si.nPage = clientRect.size.width;
		si.nPos = m_canvasDrift.x;
		SetScrollInfo(m_hWindow, SB_HORZ, &si, TRUE);


		SetCaption(GetCaption());

		ShowWindow(m_hWindow, SW_SHOW);
		UpdateWindow(m_hWindow);

		return true;
	}
	void Window::Destroy()
	{
		Framework::DestroyWindow(this);
	}

	void Window::SetCaption(std::wstring new_caption)
	{
		caption = new_caption;
		SetWindowText(m_hWindow, (LPCWSTR)(((isMainWindow) ? L"[Main Window] " : L"") + caption).c_str());
		RaiseEvent<EventTypeCaptionChanged>();
	}
	void Window::SetPosition(unsigned int x, unsigned int y)
	{
		windowRect.position.x = x;
		windowRect.position.y = y;

		SetWindowPos(m_hWindow, nullptr,
			windowRect.position.x, windowRect.position.y,
			windowRect.size.width, windowRect.size.height,
			0);

		RaiseEvent<EventTypeMoved>();
	}
	void Window::SetPosition(const Point& position)
	{
		SetPosition(position.x, position.y);
	}
	void Window::Resize(unsigned int width, unsigned int height)
	{
		windowRect.size.width = width;
		windowRect.size.height = height;

		SetWindowPos(m_hWindow, nullptr,
			windowRect.position.x, windowRect.position.y,
			windowRect.size.width, windowRect.size.height,
			0);

		RaiseEvent<EventTypeResized>();
	}
	void Window::Resize(const Size& size)
	{
		Resize(size.width, size.height);
	}
	void Window::SetMinSize(unsigned int minWidth, unsigned int minHeight)
	{
		sizeRect.minSize.width = minWidth;
		sizeRect.minSize.height = minHeight;

		if (sizeRect.minSize.width > sizeRect.maxSize.width) sizeRect.minSize.width = sizeRect.maxSize.width;
		if (sizeRect.minSize.height > sizeRect.maxSize.height) sizeRect.minSize.height = sizeRect.maxSize.height;

		RaiseEvent<EventTypeMinSizeChanged>();
	}
	void Window::SetMinSize(const Size& size)
	{
		SetMinSize(size.width, size.height);
	}
	void Window::SetMaxSize(unsigned int maxWidth, unsigned int maxHeight)
	{
		sizeRect.maxSize.width = maxWidth;
		sizeRect.maxSize.height = maxHeight;

		if (sizeRect.maxSize.width < sizeRect.minSize.width)  sizeRect.maxSize.width = sizeRect.minSize.width;
		if (sizeRect.maxSize.height < sizeRect.minSize.height)  sizeRect.maxSize.height = sizeRect.minSize.height;

		RaiseEvent<EventTypeMaxSizeChanged>();
	}
	void Window::SetMaxSize(const Size& size)
	{
		SetMaxSize(size.width, size.height);
	}
	void Window::SetSizeRect(SizeRect newSizeRect)
	{
		sizeRect = newSizeRect;

		if (sizeRect.maxSize.width < sizeRect.minSize.width) sizeRect.maxSize.width = sizeRect.minSize.width;
		if (sizeRect.maxSize.height < sizeRect.minSize.height) sizeRect.maxSize.height = sizeRect.minSize.height;

		RaiseEvent<EventTypeMinSizeChanged>();
		RaiseEvent<EventTypeMaxSizeChanged>();
	}
	void Window::SetAsMainWindow()
	{
		Framework::SetAsMainWindow(this);
	}

	void Window::Enable()
	{
		::EnableWindow(m_hWindow, TRUE);
		isEnabled = true;

		RaiseEvent<EventTypeEnabled>();
	}
	void Window::Disable()
	{
		if (isMainWindow) return;

		::EnableWindow(m_hWindow, FALSE);
		isEnabled = false;

		RaiseEvent<EventTypeDisabled>();
	}
	void Window::EnableResize()
	{
		windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
		windowStyle = windowStyle | WS_SIZEBOX;
		SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

		RaiseEvent<EventTypeResizeEnabled>();
	}
	void Window::DisableResize()
	{
		windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
		windowStyle = windowStyle & (~WS_SIZEBOX);
		SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

		RaiseEvent<EventTypeResizeDisabled>();
	}
	void Window::EnableMaximizeBox()
	{
		windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
		windowStyle = windowStyle | WS_MAXIMIZEBOX;
		SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

		RaiseEvent<EventTypeMaximizeBoxEnabled>();
	}
	void Window::DisableMaximizeBox()
	{
		windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
		windowStyle = windowStyle & (~WS_MAXIMIZEBOX);
		SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

		RaiseEvent<EventTypeMaximizeBoxDisabled>();
	}
	void Window::EnableMinimizeBox()
	{
		windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
		windowStyle = windowStyle | WS_MINIMIZEBOX;
		SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

		RaiseEvent<EventTypeMinimizeBoxEnabled>();
	}
	void Window::DisableMinimizeBox()
	{
		windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
		windowStyle = windowStyle & (~WS_MINIMIZEBOX);
		SetWindowLong(m_hWindow, GWL_STYLE, windowStyle);

		RaiseEvent<EventTypeMinimizeBoxDisabled>();
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
		::ShowWindow(m_hWindow, SW_MINIMIZE);
	}
	void Window::Show()
	{
		::ShowWindow(m_hWindow, SW_SHOW);
	}
	void Window::Hide()
	{
		::ShowWindow(m_hWindow, SW_HIDE);
	}
	int Window::ShowMessageBox(std::wstring text, std::wstring caption, UINT message_box_style)
	{
		return MessageBoxW(m_hWindow, text.c_str(), caption.c_str(), message_box_style);
	}

	const std::wstring& Window::GetCaption() const
	{
		return caption;
	}


	Point Window::GetMousePosition() const
	{
		return GetCanvasMousePosition();
	}
	Point Window::GetWindowMousePosition() const
	{
		return Point(Framework::Mouse.X - windowRect.position.x, Framework::Mouse.Y - windowRect.position.y);
	}
	Point Window::GetClientMousePosition() const
	{
		return Point(Framework::Mouse.X - clientRect.position.x, Framework::Mouse.Y - clientRect.position.y);
	}
	Point Window::GetCanvasMousePosition() const
	{
		return GetClientMousePosition() + GetCanvasPosition();
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
}