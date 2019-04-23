#ifndef WIN_API_FRAMEWORK
#define WIN_API_FRAMEWORK

#include "WindowInclude.h"
#include "WinApiWindow.h"
#include "WinApiWindowControls.h"

#include "Mouse.h"
#include "Keyboard.h"


#include <CommCtrl.h>
#include <vector>

namespace WinApiFramework
{
	class Framework
	{
		// -- fields -- //
	private:
		static HINSTANCE hProgramInstance;
		static std::vector<Window*> windows;
		static Window* mainWindow;
		static void(*FreeTimeProcess)();
		static HHOOK InputHook;
		static Mouse mouse;
		static Keyboard keyboard;


		// -- constructor -- //
	public:
		Framework(const Framework &framework) = delete;
		Framework(const Framework &&framework) = delete;


		// -- operators -- //
	public:
		Framework& operator=(const Framework &framework) = delete;
		Framework& operator=(const Framework &&framework) = delete;


		// -- methods -- //
	private:
		static LRESULT WINAPI WinApiProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI InputProcedure(int code, WPARAM wParam, LPARAM lParam);

		static void AddWindow(Window *addWindow);
		static void RemoveWindow(Window *oldWindow);
		static void SetAsMainWindow(Window *window);
	public:
		static UINT ProcessMessages();
		static void Exit(int return_value);
		static int ShowGlobalMessageBox
		(
			std::wstring text = L"default text",
			std::wstring caption = L"Default caption",
			UINT message_box_style = 0
		);
		static void SetFreeTimeFunction(void(*freeTimeFunction)());
		static void UnsetFreeTimeFunction();


		// -- property fields -- //
	public:
		static const HINSTANCE &ProgramInstance;
		static Mouse &Mouse;
		static Keyboard &Keyboard;


		// -- friend relationship -- //
		friend class Window;
	};
}

#endif // WIN_API_FRAMEWORK