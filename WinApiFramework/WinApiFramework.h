#ifndef WIN_API_FRAMEWORK
#define WIN_API_FRAMEWORK

#include "WindowInclude.h"
#include "ExternIncludes.h"

#include "WinApiWindow.h"
#include "WinApiWindowControls.h"

#include "Mouse.h"
#include "Keyboard.h"

#include <queue>

namespace WinApiFramework
{
	class Framework
	{
		// ~~ Framework::fields ~~ //
	private:
		static HINSTANCE hProgramInstance;

		static std::vector<Window*> m_windows;
		static Window* m_pRootWindow;

		struct PendingActionList
		{
		private:
			std::list<BaseAction*> m_actions;
			bool m_invocation_state;
			int m_invocations_limit = 32;

		public:
			PendingActionList();
			~PendingActionList();

		public:
			void AppendAction(BaseAction* action);
			void InvokeActions();
		};
		static PendingActionList m_pending_actions;

		static HHOOK InputHook;
		static Mouse mouse;
		static Keyboard keyboard;

		static std::function<void()> callBack;
	public:
		Framework(const Framework &framework) = delete;
		Framework(const Framework &&framework) = delete;


	public:
		Framework& operator=(const Framework &framework) = delete;
		Framework& operator=(const Framework &&framework) = delete;


	private:
		static LRESULT WINAPI WinApiProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			std::function<LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)>* windProc =
				(std::function<LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)>*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (windProc)
			{
				if (!(*windProc)(hWnd, msg, wParam, lParam))
				{
					m_pending_actions.InvokeActions();
					return 0;
				}
			}
			m_pending_actions.InvokeActions();
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		static LRESULT WINAPI InputProcedure(int code, WPARAM wParam, LPARAM lParam);
	public:
		static HINSTANCE GetProgramInstance();
		static Window* CreateNewWindow(const ConStruct<Window>& conStruct);
		static bool DestroyWindow(Window* const window);
		static void DestroyAllWindows();
		static size_t GetWindowCount();
		static const Window* GetRootWindow();
		static void SetAsMainWindow(Window *window);
	private:
		static void AppendAction(BaseAction* action);
	public:
		static UINT ProcessMessages();
		static void Exit(int return_value);
		static MessBoxButtonPressed ShowGlobalMessageBox(
			std::wstring caption = L"default caption",
			std::wstring text = L"default text",
			MessBoxButtonLayout buttons = MessBoxButtonLayout::Ok,
			MessBoxIcon icon = MessBoxIcon::IconInformation);

		static void SetCallBackFunction(void(*callBackFunction)())
		{
			if (callBackFunction == nullptr)
			{
				Framework::callBack = nullptr;
				return;
			}
			else
			{
				std::function<void()> f(callBackFunction);
				Framework::callBack = f;
			}
		}
		template <class ReceivingObject>
		static void SetCallBackFunction(ReceivingObject* receivingObject, void(ReceivingObject::*callBackFunction)())
		{
			if (receivingObject == nullptr || callBackFunction == nullptr)
			{
				Framework::callBack = nullptr;
				return;
			}
			else
			{
				Framework::callBack = std::bind(callBackFunction, receivingObject);
			}
		}


	public:
		static const HINSTANCE &ProgramInstance;
		static Mouse &Mouse;
		static Keyboard &Keyboard;


		friend class BaseWindow;
	};
}

#endif // WIN_API_FRAMEWORK