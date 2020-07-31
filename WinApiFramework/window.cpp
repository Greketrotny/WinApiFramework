#include "window.h"
#include "winapi_framework.h"

#undef IsMinimized

namespace WinapiFramework
{
	// ~~~~~~~~ [CLASS] Window ~~~~~~~~ //
	Window::Window()
		: ParentWindow(nullptr)
		, HasWindowProcedure(this, &Window::WindowProcedure)
	{}
	Window::Window(unsigned int id, const ConStruct<Window> &conStruct)
		: Window()
	{
		window_id = id;
		m_window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
		m_window_class_name = L"WindowClass" + std::to_wstring((window_id));

		m_window_rect = conStruct.rect;
		m_caption = conStruct.caption;
		m_min_size = conStruct.min_size;
		m_max_size = conStruct.max_size;

		m_canvasRect = BoundRect(0, 0, 100, 100);	// TODO: take user input into consideration
		m_canvasDrift = Point(0, 0);

		// create window class
		//CreateAndRegisterWindowClass();

		// create window
		//CreateWinApiWindow(conStruct);
	}
	Window::~Window()
	{
		// destroy all child controls
		DestroyAllChildren();

		// destroy window
		DestroyWinapiWindow();

		// unregister the window class
		UnregisterClass(m_window_class_name.c_str(), Framework::GetProgramInstance());
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
						if (pos < (m_canvasRect.bottom) - m_client_rect.size.height) pos++;
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
						if (pos < (m_canvasRect.right) - m_client_rect.size.width) pos++;
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
				return 0;

			case WM_DESTROY:
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
				if (::GetWindowRect(m_hWindow, &r))
				{
					m_window_rect.position.x = r.left;
					m_window_rect.position.y = r.top;
					m_window_rect.size.width = r.right - r.left;
					m_window_rect.size.height = r.bottom - r.top;
				}
				if (GetClientRect(m_hWindow, &r))
				{
					POINT p{ 0, 0 };
					ClientToScreen(m_hWindow, &p);
					m_client_rect.position.x = p.x;
					m_client_rect.position.y = p.y;
					m_client_rect.size.width = r.right - r.left;
					m_client_rect.size.height = r.bottom - r.top;
				}

				RaiseEvent<EventTypeMoved>();
				return 0;
			}

			case WM_SIZE:
			{
				RECT r;
				// set window rect
				if (::GetWindowRect(m_hWindow, &r))
				{
					m_window_rect.position.x = r.left;
					m_window_rect.position.y = r.top;
					m_window_rect.size.width = r.right - r.left;
					m_window_rect.size.height = r.bottom - r.top;
				}

				// set client rect
				if (GetClientRect(m_hWindow, &r))
				{
					POINT p{ 0, 0 };
					ClientToScreen(m_hWindow, &p);
					m_client_rect.position.x = p.x;
					m_client_rect.position.y = p.y;
					m_client_rect.size.width = r.right - r.left;
					m_client_rect.size.height = r.bottom - r.top;
				}


				// [>] Adjust canvas position
				Point deltaXY(0, 0);
				if (m_canvasRect.right - m_canvasDrift.x < m_client_rect.size.width)
				{
					deltaXY.x = std::min(m_canvasDrift.x - m_canvasRect.left, m_client_rect.size.width - (m_canvasRect.right - m_canvasDrift.x));
				}
				if (m_canvasRect.bottom - m_canvasDrift.y < m_client_rect.size.height)
				{
					deltaXY.y = std::min(m_canvasDrift.y - m_canvasRect.top, m_client_rect.size.height - (m_canvasRect.bottom - m_canvasDrift.y));
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
				si.nPage = m_client_rect.size.height;
				si.nPos = m_canvasDrift.y;
				si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
				SetScrollInfo(m_hWindow, SB_VERT, &si, TRUE);

				// horizontal scroll
				si.nMin = m_canvasRect.left;
				si.nMax = m_canvasRect.right;
				si.nPage = m_client_rect.size.width;
				si.nPos = m_canvasDrift.x;
				si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
				SetScrollInfo(m_hWindow, SB_HORZ, &si, TRUE);

				RaiseEvent<EventTypeResized>();
				return 0;
			}

			case WM_GETMINMAXINFO:
			{
				LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
				mmi->ptMinTrackSize.x = m_min_size.width;
				mmi->ptMinTrackSize.y = m_min_size.height;
				mmi->ptMaxTrackSize.x = m_max_size.width;
				mmi->ptMaxTrackSize.y = m_max_size.height;

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
			//			pt.x = ((int)m_window_rect.width < pt.x) ? (int)m_window_rect.width : pt.x;
			//			pt.y = (0 > pt.y) ? 0 : pt.y;
			//			pt.y = ((int)m_window_rect.height < y) ? (int)m_window_rect.height : y;*/
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
	bool Window::CreateWinapiWindow()
	{
		// [>] Create WindowClassEx
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.hInstance = Framework::GetProgramInstance();
		wc.lpfnWndProc = GetFrameworkProcedure();
		wc.lpszClassName = m_window_class_name.c_str();
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

		// setup window rect
		RECT r = { (LONG)m_window_rect.position.x, (LONG)m_window_rect.position.y, (LONG)(m_window_rect.position.x + m_window_rect.size.width), (LONG)(m_window_rect.position.y + m_window_rect.size.height) };
		AdjustWindowRect(&r, m_window_style, FALSE);
		m_window_rect.position.x = r.left;
		m_window_rect.position.y = r.top;
		m_window_rect.size.width = r.right - r.left;
		m_window_rect.size.height = r.bottom - r.top;

		/*if (conStruct.position == Position::Center)
		{
			int w = GetSystemMetrics(SM_CXSCREEN);
			int h = GetSystemMetrics(SM_CYSCREEN);

			m_window_rect.position.x = (w - std::min(m_window_rect.size.width, w)) / 2;
			m_window_rect.position.y = (h - std::min(m_window_rect.size.height, h)) / 2;
		}
		else if (conStruct.position == Position::Default)
		{
			m_window_rect.position.x = 100;
			m_window_rect.position.y = 100;
		}*/


		// [>] Create window
		m_hWindow = CreateWindow((LPCWSTR)m_window_class_name.c_str(), (LPCWSTR)m_caption.c_str(),
			m_window_style,
			m_window_rect.position.x, m_window_rect.position.y, m_window_rect.size.width, m_window_rect.size.height,
			nullptr, nullptr, Framework::GetProgramInstance(), nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"Window creation failed.", L"Window create error", MB_OK | MB_ICONERROR);
			return false;
		}

		// set pointer to Window class to receive messages
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_window_procedure);

		// set window scrolling
		SetWindowLongPtr(m_hWindow, GWL_STYLE, m_window_style | WS_HSCROLL | WS_VSCROLL);
		SetWindowPos(m_hWindow, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);


		// [>] Setup window scrolling
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);

		// vertical
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = m_canvasRect.top;
		si.nMax = m_canvasRect.bottom;
		si.nPage = m_client_rect.size.height;
		si.nPos = m_canvasDrift.y;
		SetScrollInfo(m_hWindow, SB_VERT, &si, TRUE);

		// horizontal
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMin = m_canvasRect.left;
		si.nMax = m_canvasRect.right;
		si.nPage = m_client_rect.size.width;
		si.nPos = m_canvasDrift.x;
		SetScrollInfo(m_hWindow, SB_HORZ, &si, TRUE);


		SetCaption(GetCaption());

		ShowWindow(m_hWindow, SW_SHOW);
		UpdateWindow(m_hWindow);

		return true;
	}
	void Window::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
		m_hWindow = NULL;
	}

	void Window::Destroy()
	{
		AppendAction(new DestroyAction(this));
	}
	void Window::Close()
	{
		SendMessage(m_hWindow, WM_CLOSE, 0, 0);
	}

	void Window::SetCaption(const std::wstring& new_caption)
	{
		m_caption = new_caption;
		SetWindowText(m_hWindow, (LPCWSTR)(((isMainWindow) ? L"[Main Window] " : L"") + m_caption).c_str());
		RaiseEvent<EventTypeCaptionChanged>();
	}
	void Window::Move(int x, int y)
	{
		m_window_rect.position.x = x;
		m_window_rect.position.y = y;

		SetWindowPos(m_hWindow, nullptr,
			m_window_rect.position.x, m_window_rect.position.y,
			m_window_rect.size.width, m_window_rect.size.height,
			0);

		RaiseEvent<EventTypeMoved>();
	}
	void Window::Resize(int width, int height)
	{
		m_window_rect.size.width = std::max(width, 0);
		m_window_rect.size.height = std::max(height, 0);

		SetWindowPos(m_hWindow, nullptr,
			m_window_rect.position.x, m_window_rect.position.y,
			m_window_rect.size.width, m_window_rect.size.height,
			0);

		RaiseEvent<EventTypeResized>();
	}
	void Window::SetMinSize(unsigned int minWidth, unsigned int minHeight)
	{
		m_min_size.width = minWidth;
		m_min_size.height = minHeight;

		if (m_min_size.width > m_max_size.width) m_min_size.width = m_max_size.width;
		if (m_min_size.height > m_max_size.height) m_min_size.height = m_max_size.height;

		RaiseEvent<EventTypeMinSizeChanged>();
	}
	void Window::SetMinSize(const Size& size)
	{
		SetMinSize(size.width, size.height);
	}
	void Window::SetMaxSize(unsigned int maxWidth, unsigned int maxHeight)
	{
		m_max_size.width = maxWidth;
		m_max_size.height = maxHeight;

		if (m_max_size.width < m_min_size.width)  m_max_size.width = m_min_size.width;
		if (m_max_size.height < m_min_size.height)  m_max_size.height = m_min_size.height;

		RaiseEvent<EventTypeMaxSizeChanged>();
	}
	void Window::SetMaxSize(const Size& size)
	{
		SetMaxSize(size.width, size.height);
	}
	void Window::SetAsMainWindow()
	{
		Framework::SetAsMainWindow(this);
	}

	void Window::EnableResize()
	{
		m_window_style = GetWindowLong(m_hWindow, GWL_STYLE);
		m_window_style = m_window_style | WS_SIZEBOX;
		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

		RaiseEvent<EventTypeResizeEnabled>();
	}
	void Window::DisableResize()
	{
		m_window_style = GetWindowLong(m_hWindow, GWL_STYLE);
		m_window_style = m_window_style & (~WS_SIZEBOX);
		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

		RaiseEvent<EventTypeResizeDisabled>();
	}
	void Window::EnableMaximizeBox()
	{
		m_window_style = GetWindowLong(m_hWindow, GWL_STYLE);
		m_window_style = m_window_style | WS_MAXIMIZEBOX;
		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

		RaiseEvent<EventTypeMaximizeBoxEnabled>();
	}
	void Window::DisableMaximizeBox()
	{
		m_window_style = GetWindowLong(m_hWindow, GWL_STYLE);
		m_window_style = m_window_style & (~WS_MAXIMIZEBOX);
		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

		RaiseEvent<EventTypeMaximizeBoxDisabled>();
	}
	void Window::EnableMinimizeBox()
	{
		m_window_style = GetWindowLong(m_hWindow, GWL_STYLE);
		m_window_style = m_window_style | WS_MINIMIZEBOX;
		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

		RaiseEvent<EventTypeMinimizeBoxEnabled>();
	}
	void Window::DisableMinimizeBox()
	{
		m_window_style = GetWindowLong(m_hWindow, GWL_STYLE);
		m_window_style = m_window_style & (~WS_MINIMIZEBOX);
		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

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
	unsigned int Window::GetId() const
	{
		return window_id;
	}

	MessBoxButtonPressed Window::ShowMessageBox(const std::wstring& caption, const std::wstring& text, 
		MessBoxButtonLayout buttons,
		MessBoxIcon icon)
	{
		return (MessBoxButtonPressed)MessageBoxW(m_hWindow, text.c_str(), caption.c_str(), buttons | icon);
	}

	const std::wstring& Window::GetCaption() const
	{
		return m_caption;
	}


	Point Window::GetMousePosition() const
	{
		return GetCanvasMousePosition();
	}
	Point Window::GetWindowMousePosition() const
	{
		return Point(Framework::Mouse.X - m_window_rect.position.x, Framework::Mouse.Y - m_window_rect.position.y);
	}
	Point Window::GetClientMousePosition() const
	{
		return Point(Framework::Mouse.X - m_client_rect.position.x, Framework::Mouse.Y - m_client_rect.position.y);
	}
	Point Window::GetCanvasMousePosition() const
	{
		return GetClientMousePosition() + GetCanvasPosition();
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	// ~~~~~~~~ [STRUCT] Window::DestroyAction ~~~~~~~~
	Window::DestroyAction::DestroyAction(Window* window)
		: m_pWindow(window)
	{}
	Window::DestroyAction::~DestroyAction()
	{}
	
	void Window::DestroyAction::Invoke()
	{
		if (m_pWindow) Framework::DestroyWindow(m_pWindow);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}