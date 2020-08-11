#ifndef WIN_API_FRAMEWORK
#define WIN_API_FRAMEWORK

#include "window_include.h"

#include "window.h"
#include "all_controls.h"

#include "mouse.h"
#include "keyboard.h"

#include <list>			// std::list
#include <functional>	// std::function

namespace WinapiFramework
{
	class Framework
	{
	private:
		HINSTANCE m_hProgram_instance;

		std::vector<Window*> m_windows;
		Window* mp_root_window;

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
		PendingActionList m_pending_actions;

		HHOOK m_input_hook;
		Mouse m_mouse;
		Keyboard m_keyboard;

		std::function<void()> callBack;
	private:
		Framework();
	public:
		Framework(const Framework &framework) = delete;
		Framework(const Framework &&framework) = delete;
		~Framework();


	public:
		Framework& operator=(const Framework &framework) = delete;
		Framework& operator=(Framework &&framework) = delete;


	private:
		static LRESULT WINAPI WinApiProcedure(
			HWND hWnd, 
			UINT msg, 
			WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI SubclassProcedure(
			HWND hWnd, 
			UINT msg, WPARAM wParam, LPARAM lParam, 
			UINT_PTR idSubClass, DWORD_PTR refData);
		static LRESULT WINAPI InputProcedure(
			int code, 
			WPARAM wParam, LPARAM lParam);
	public:
		static Framework& GetInstance();
		HINSTANCE GetProgramInstance();
		Window* CreateNewWindow(const ConStruct<Window>& conStruct);
		bool DestroyWindow(Window* const window);
		void DestroyAllWindows();
		size_t GetWindowCount();
		const Window* GetRootWindow();
		void SetAsMainWindow(Window *window);
	private:
		void AppendAction(BaseAction* action);
	public:
		UINT ProcessMessages();
		void Exit(int return_value);
		MessBoxButtonPressed ShowGlobalMessageBox(
			const std::wstring& caption = L"default caption",
			const std::wstring& text = L"default text",
			MessBoxButtonLayout buttons = MessBoxButtonLayout::Ok,
			MessBoxIcon icon = MessBoxIcon::Information);

		void SetCallBackFunction(void(*callBackFunction)())
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
		void SetCallBackFunction(ReceivingObject* receivingObject, void(ReceivingObject::*callBackFunction)())
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
		Mouse &Mouse;
		Keyboard &Keyboard;


		friend class BaseWindow;
	};
}

#endif // WIN_API_FRAMEWORK