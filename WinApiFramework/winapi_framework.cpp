#include "winapi_framework.h"


namespace WinapiFramework
{
	// ~~~~~~~~ [STRUCT] EventFunctor ~~~~~~~~
	Framework::PendingActionList::PendingActionList()
		: m_invocation_state(false)
	{
	}
	Framework::PendingActionList::~PendingActionList()
	{
		while (!m_actions.empty())
		{
			delete m_actions.front();
			m_actions.pop_front();
		}
	}

	void Framework::PendingActionList::AppendAction(BaseAction* action)
	{
		m_actions.push_back(action);
	}
	void Framework::PendingActionList::InvokeActions()
	{
		if (m_invocation_state == true) return;
		else m_invocation_state = true;

		for (int i = 0; i < m_invocations_limit && !m_actions.empty(); ++i)
		{
			m_actions.front()->Invoke();
			delete m_actions.front();
			m_actions.pop_front();
		}
		m_invocation_state = false;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] Framework ~~~~~~~~
	Framework::Framework()
		: m_hProgram_instance(GetModuleHandle(NULL))
		, m_input_hook(SetWindowsHookEx(WH_GETMESSAGE, Framework::InputProcedure, NULL, GetThreadId(GetCurrentThread())))
		, Mouse(m_mouse)
		, Keyboard(m_keyboard)
		, mp_root_window(nullptr)
		, callBack(nullptr)
	{

	}
	Framework::~Framework()
	{
		DestroyAllWindows();
	}

	LRESULT WINAPI Framework::WinApiProcedure(
		HWND hWnd, 
		UINT msg, 
		WPARAM wParam, LPARAM lParam)
	{
		std::function<LRESULT(
				HWND hWnd, 
				UINT msg, 
				WPARAM wParam, LPARAM lParam)>* windProc =
		(std::function<LRESULT(
				HWND hWnd, 
				UINT msg, 
				WPARAM wParam, LPARAM lParam)>*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

		if (windProc)
		{
			LRESULT result = (*windProc)(hWnd, msg, wParam, lParam);
			GetInstance().m_pending_actions.InvokeActions();
			return result;
		}
		else
		{
			GetInstance().m_pending_actions.InvokeActions();
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	LRESULT WINAPI Framework::SubclassProcedure(
		HWND hWnd, 
		UINT msg, WPARAM wParam, 
		LPARAM lParam, UINT_PTR idSubClass, DWORD_PTR refData)
	{
		std::function<LRESULT(
			HWND hWnd, 
			UINT msg, 
			WPARAM wParam, LPARAM lParam, 
			UINT_PTR idSubClass, DWORD_PTR refData)>* sub_proc = 
		reinterpret_cast<std::function<LRESULT(
			HWND hWnd, 
			UINT msg, 
			WPARAM wParam, LPARAM lParam, 
			UINT_PTR idSubClass, DWORD_PTR refData)>*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (sub_proc)
		{
			if (!(*sub_proc)(hWnd, msg, wParam, lParam, idSubClass, refData))
			{
				GetInstance().m_pending_actions.InvokeActions();
				return 0;
			}
		}
		GetInstance().m_pending_actions.InvokeActions();

		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}
	LRESULT WINAPI Framework::InputProcedure(int code, WPARAM wParam, LPARAM lParam)
	{
		if (code >= 0 && code == HC_ACTION)
		{
			MSG *msg = (MSG*)lParam;

			if (GetInstance().Mouse.HandleMessage(msg->message, wParam, lParam) == 0)
				return CallNextHookEx(GetInstance().m_input_hook, code, wParam, lParam);

			if (GetInstance().Keyboard.HandleMessage(wParam, lParam) == 0)
				return CallNextHookEx(GetInstance().m_input_hook, code, wParam, lParam);

		}
		return CallNextHookEx(GetInstance().m_input_hook, code, wParam, lParam);
	}
	
	Framework& Framework::GetInstance()
	{
		static Framework instance;
		return instance;
	}
	HINSTANCE Framework::GetProgramInstance()
	{
		return m_hProgram_instance;
	}
	Window* Framework::CreateNewWindow(const ConStruct<Window>& conStruct)
	{
		// find next unused window id
		unsigned int next_id = 0;
		for (Window *w : m_windows)
		{
			if (w->GetId() == next_id)
				next_id++;
		}

		// create window
		Window* window = new Window(next_id, conStruct);
		m_windows.push_back(window);
		window->CreateWinapiWindow();

		// first window is main automaticly
		if (mp_root_window == nullptr)
		{
			mp_root_window = window;
			window->isMainWindow = true;
		}

		// return new window
		return window;
	}
	bool Framework::DestroyWindow(Window* const window)
	{
		if (window == nullptr) return false;
		if (window == mp_root_window)
		{
			Framework::Exit(0);
		}

		for (size_t i = 0; i < m_windows.size(); ++i)
		{
			if (m_windows[i] == window)
			{
				delete window;
				m_windows.erase(m_windows.begin() + i);
				return true;
			}
		}

		return false;
	}
	void Framework::DestroyAllWindows()
	{
		for (auto* wnd : m_windows) delete wnd;
		m_windows.clear();
	}
	size_t Framework::GetWindowCount()
	{
		return m_windows.size();
	}
	const Window* Framework::GetRootWindow()
	{
		return mp_root_window;
	}
	void Framework::SetAsMainWindow(Window *window)
	{
		if (window == mp_root_window) return;

		for (Window *w : m_windows)
		{
			if (w->isMainWindow)
			{
				w->isMainWindow = false;
				w->SetCaption(w->GetCaption());
			}
		}

		mp_root_window = window;
		window->isMainWindow = true;
		window->SetCaption(window->GetCaption());
	}

	void Framework::AppendAction(BaseAction* action)
	{
		m_pending_actions.AppendAction(action);
	}

	UINT Framework::ProcessMessages()
	{
		//InitCommonControls();

		MSG msg;
		while (true)
		{
			// process all untouched messages
			unsigned int eventCounter = 0u;
			while ((PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) && (eventCounter < 16u))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				/*	see IsDialogMessage() function for
				*	implementing tabstoping between
				*	controls on window	*/

				if (msg.message == WM_QUIT)
					return WM_QUIT;

				eventCounter++;
			}

			// invoke handlers of occured events
			m_pending_actions.InvokeActions();


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
	void Framework::Exit(int return_value)
	{
		PostQuitMessage(return_value);
	}

	MessBoxButtonPressed Framework::ShowGlobalMessageBox(const std::wstring& text, const std::wstring& caption,
		MessBoxButtonLayout buttons, MessBoxIcon icon)
	{
		return (MessBoxButtonPressed)MessageBox(NULL, caption.c_str(), text.c_str(), 
			static_cast<UINT>(buttons) | static_cast<UINT>(icon));
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}