#ifndef WIN_API_FRAMEWORK
#define WIN_API_FRAMEWORK

#include "WindowInclude.h"
#include "ExternIncludes.h"

#include "WinApiWindow.h"
#include "WinApiWindowControls.h"

#include "Mouse.h"
#include "Keyboard.h"

namespace WinApiFramework
{
	class Framework
	{
		// ~~ Framework::fields ~~ //
	private:
		static HINSTANCE hProgramInstance;

		static std::vector<Window*> m_windows;
		static Window* m_pRootWindow;

		static HHOOK InputHook;
		static Mouse mouse;
		static Keyboard keyboard;

		static std::function<void()> callBack;
	public:
		enum MessBoxButtonLayout
		{
			AbortRetryIgnore = 0x00000002L,
			CancelTryContinue = 0x00000006L,
			Help = 0x00004000L,
			Ok = 0x00000000L,
			OkCancel = 0x00000001L,
			RetryCancel = 0x00000005L,
			YesNo = 0x00000004L,
			YesNoCancel = 0x00000003L
		};
		enum MessBoxIcon
		{
			IconWarning = 0x00000030L,
			IconInformation = 0x00000040L,
			IconQuestion = 0x00000020L,
			IconError = 0x00000010L
		};
		enum MessBoxButtonPressed
		{
			ButtonOk = 1,
			ButtonCancel = 2,
			ButtonAbort = 3,
			ButtonRetry = 4,
			ButtonIgnore = 5,
			ButtonYes = 6,
			ButtonNo = 7,
			ButtonTryAgain = 10,
			ButtonContinue = 11
		};


		// ~~ Framework::constructor ~~ //
	public:
		Framework(const Framework &framework) = delete;
		Framework(const Framework &&framework) = delete;


		// ~~ Framework::operators ~~ //
	public:
		Framework& operator=(const Framework &framework) = delete;
		Framework& operator=(const Framework &&framework) = delete;


		// ~~ Framework::methods ~~ //
	private:
		static LRESULT WINAPI WinApiProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI InputProcedure(int code, WPARAM wParam, LPARAM lParam);

	public:
		static Window* CreateNewWindow(const ConStruct<Window>& conStruct);
		static bool DestroyWindow(Window* const window);
		static void DestroyAllWindows();
		static size_t GetWindowCount();

		static void SetAsMainWindow(Window *window);
	public:
		static UINT ProcessMessages();
		static void Exit(int return_value);

		static int ShowGlobalMessageBox(
			std::wstring text = L"default text",
			std::wstring caption = L"Default caption",
			UINT message_box_style = 0);
		static MessBoxButtonPressed ShowGlobalMessageBox(
			std::wstring text = L"default text",
			std::wstring caption = L"Default caption",
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
		template <class ReceivingObject> static void SetCallBackFunction(ReceivingObject* receivingObject, void(ReceivingObject::*callBackFunction)())
		{
			if (receivingObject == nullptr || callBackFunction == nullptr)
			{
				Framework::callBack = nullptr;
				return;
			}
			else
			{
				Framework::callBack = std::bind(callBackFunction, receivingObject);;
			}
		}


		// ~~ Framework::property fields ~~ //
	public:
		static const HINSTANCE &ProgramInstance;
		static Mouse &Mouse;
		static Keyboard &Keyboard;


		// ~~ Framework::friends ~~ //
		friend class Window;
	};
}

#endif // WIN_API_FRAMEWORK