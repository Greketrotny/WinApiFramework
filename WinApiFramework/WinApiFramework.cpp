#include "Precompiled.h"
#include "WinApiFramework.h"
#include "WinApiWindow.h"


namespace WinApiFramework
{
	// ~~~~~~~~ [CLASS] Framework ~~~~~~~~
	// ~~ Framework::fields ~~
	HINSTANCE Framework::hProgramInstance = GetModuleHandle(NULL);
	const HINSTANCE& Framework::ProgramInstance(hProgramInstance);
	std::vector<Window*> Framework::m_windows;
	Window* Framework::m_pRootWindow(nullptr);
	std::function<void()> Framework::callBack = nullptr;
	HHOOK Framework::InputHook = SetWindowsHookEx(WH_GETMESSAGE, Framework::InputProcedure, NULL, GetThreadId(GetCurrentThread()));
	Mouse Framework::mouse;
	Keyboard Framework::keyboard;
	Mouse& Framework::Mouse(Framework::mouse);
	Keyboard& Framework::Keyboard(Framework::keyboard);


	// ~~ Framework::methods ~~
	LRESULT WINAPI Framework::WinApiProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// find destination window for the event
		for (Window *w : m_windows)
		{
			if (w->GetWindowHandle() == hWnd)
			{
				switch (w->WndProcedure(msg, wParam, lParam))
				{
					case ProcedureResult::Handled: return 0;
					case ProcedureResult::Unhandled: return DefWindowProc(hWnd, msg, wParam, lParam);
				}
			}
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	LRESULT WINAPI Framework::InputProcedure(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code >= 0 && code == HC_ACTION)
		{
			MSG *msg = (MSG*)lParam;

			switch (msg->message)
			{
				// Keyboard events //
				case WM_KEYDOWN:
				case WM_SYSKEYDOWN:
					if (!(msg->lParam & 0x40000000) || keyboard.autorepeat)
					{
						keyboard.KeyPress((Keyboard::Key)msg->wParam);
					}
					break;

				case WM_KEYUP:
				case WM_SYSKEYUP:
					keyboard.KeyRelase((Keyboard::Key)(msg->wParam));
					break;

				case WM_CHAR:
					if (!(msg->lParam & 0x40000000) || keyboard.autorepeat)
					{
						keyboard.CharInput((wchar_t)msg->wParam);
					}
					break;


					// Mouse events //
				case WM_MOUSEMOVE:
				{
					//const POINTS pt = MAKEPOINTS(msg->lParam);
					POINT pt;
					GetCursorPos(&pt);
					mouse.Move(pt.x, pt.y);
					break;
				}
				case WM_LBUTTONDOWN:
					mouse.isLeftPressed = true;
					mouse.Events.PushEvent(Mouse::Event(Mouse::Event::Type::LeftPress));
					break;
				case WM_RBUTTONDOWN:
					mouse.isRightPressed = true;
					mouse.Events.PushEvent(Mouse::Event(Mouse::Event::Type::RightPress));
					break;
				case WM_LBUTTONUP:
					mouse.isLeftPressed = false;
					mouse.Events.PushEvent(Mouse::Event(Mouse::Event::Type::LeftRelase));
					break;
				case WM_RBUTTONUP:
					mouse.isRightPressed = false;
					mouse.Events.PushEvent(Mouse::Event(Mouse::Event::Type::RightRelase));
					break;
				case WM_MBUTTONDOWN:
					mouse.isMiddlePressed = true;
					mouse.Events.PushEvent(Mouse::Event(Mouse::Event::Type::MiddlePress));
					break;
				case WM_MBUTTONUP:
					mouse.isMiddlePressed = false;
					mouse.Events.PushEvent(Mouse::Event(Mouse::Event::Type::MiddleRelase));
					break;
				case WM_MOUSEWHEEL:
					if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
					{
						mouse.Events.PushEvent(Mouse::Event(Mouse::Event::Type::WheelUp));
					}
					else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
					{
						mouse.Events.PushEvent(Mouse::Event(Mouse::Event::Type::WheelDown));
					}
					break;
			}
		}
		return CallNextHookEx(InputHook, code, wParam, lParam);
	}
	
	Window* Framework::CreateNewWindow(const ConStruct<Window>& conStruct)
	{
		// find next unused window id
		unsigned int next_id = 0;
		for (Window *w : m_windows)
		{
			if (w->Id == next_id)
				next_id++;
		}

	// create window
		Window* window = new Window(next_id, conStruct);
		m_windows.push_back(window);
		window->CreateWinApiWindow(conStruct);

		// first window is main automaticly
		if (m_pRootWindow == nullptr)
		{
			m_pRootWindow = window;
			window->isMainWindow = true;
		}

			// return new window
		return window;
	}
	bool Framework::DestroyWindow(Window* const window)
	{
		for (size_t i = 0; i < m_windows.size(); ++i)
		{
			if (m_windows[i] == window)
			{
				delete window;
				m_windows.erase(m_windows.begin() + i);

				if (window == m_pRootWindow)
					Framework::Exit(0);

				return true;
			}
		}

		return false;
	}
	void Framework::DestroyAllWindows()
	{
		for (size_t i = 0; i < m_windows.size(); ++i)
		{
			delete m_windows[i];
		}
		m_windows.clear();
	}
	size_t Framework::GetWindowCount()
	{
		return m_windows.size();
	}

	void Framework::SetAsMainWindow(Window *window)
	{
		if (window == m_pRootWindow) return;

		for (Window *w : m_windows)
		{
			if (w->isMainWindow)
			{
				w->isMainWindow = false;
				w->SetCaption(w->GetCaption());
			}
		}

		m_pRootWindow = window;
		window->isMainWindow = true;
		window->SetCaption(window->GetCaption());
	}

	void Framework::Exit(int return_value)
	{
		DestroyAllWindows();
		PostQuitMessage(return_value);
	}

	int Framework::ShowGlobalMessageBox(std::wstring text, std::wstring caption, UINT message_box_style)
	{
		return MessageBoxW(NULL, text.c_str(), caption.c_str(), message_box_style);
	}
	Framework::MessBoxButtonPressed Framework::ShowGlobalMessageBox(std::wstring text, std::wstring caption,
		Framework::MessBoxButtonLayout buttons, Framework::MessBoxIcon icon)
	{
		return (Framework::MessBoxButtonPressed)MessageBox(NULL, text.c_str(), caption.c_str(),
			buttons | icon);
	}

	UINT Framework::ProcessMessages()
	{
		//InitCommonControls();

		MSG msg;
		unsigned int eventCounter = 0u;
		while (true)
		{
			// process all untouched messages
			eventCounter = 0u;
			while ((PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) && (eventCounter < 16u))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT)
					return WM_QUIT;

				eventCounter++;
			}
			/*while (GetMessage(&msg, NULL, 0, 0))
			{
					TranslateMessage(&msg);
					DispatchMessage(&msg);

					if (msg.message == WM_QUIT)
							return WM_QUIT;
			}

			return 0; // temporary for get message...*/

			// call callBackFunction between messages
			if (callBack != nullptr) callBack();
		}
		return 0;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}