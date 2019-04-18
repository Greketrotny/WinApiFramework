#include "WinApiFramework.h"
#include <vector>
#include <windowsx.h>

//#include <CommCtrl.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace WinApiFramework;

// [CLASS] Framework ---------------------------|
// -- fields -- //
HINSTANCE Framework::hProgramInstance = GetModuleHandle(NULL);
const HINSTANCE& Framework::ProgramInstance(hProgramInstance);
std::vector<Window*> Framework::windows;
Window* Framework::mainWindow = nullptr;
void(*Framework::FreeTimeProcess)() = nullptr;
HHOOK Framework::InputHook = SetWindowsHookEx(WH_GETMESSAGE, Framework::InputProcedure, NULL, GetThreadId(GetCurrentThread()));
Mouse Framework::mouse;
Keyboard Framework::keyboard;
Mouse& Framework::Mouse(Framework::mouse);
Keyboard& Framework::Keyboard(Framework::keyboard);


// -- methods -- //
LRESULT WINAPI Framework::WinApiProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// find destination window for the event
	for (Window *w : windows)
	{
		if (w->hWindow == hWnd)
		{
			if (!w->WndProc(msg, wParam, lParam)) return 0;
			else return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI Framework::InputProcedure(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0 && code == HC_ACTION)
	{
		MSG *msg;
		msg = (MSG*)lParam;

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
			mouse.PushEvent(Mouse::Event(Mouse::Event::Type::LeftPress));
			break;
		case WM_RBUTTONDOWN:
			mouse.isRightPressed = true;
			mouse.PushEvent(Mouse::Event(Mouse::Event::Type::RightPress));
			break;
		case WM_LBUTTONUP:
			mouse.isLeftPressed = false;
			mouse.PushEvent(Mouse::Event(Mouse::Event::Type::LeftRelase));
			break;
		case WM_RBUTTONUP:
			mouse.isRightPressed = false;
			mouse.PushEvent(Mouse::Event(Mouse::Event::Type::RightRelase));
			break;
		case WM_MBUTTONDOWN:
			mouse.isMiddlePressed = true;
			mouse.PushEvent(Mouse::Event(Mouse::Event::Type::MiddlePress));
			break;
		case WM_MBUTTONUP:
			mouse.isMiddlePressed = false;
			mouse.PushEvent(Mouse::Event(Mouse::Event::Type::MiddleRelase));
			break;
		case WM_MOUSEWHEEL:
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse.PushEvent(Mouse::Event(Mouse::Event::Type::WheelUp));
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				mouse.PushEvent(Mouse::Event(Mouse::Event::Type::WheelDown));
			}
			break;
		}
	}
	return CallNextHookEx(InputHook, code, wParam, lParam);
}

void Framework::AddWindow(Window *newWindow)
{
	if (newWindow == nullptr) return;

	// find next unused window id
	unsigned int next_id = 0;
	for (Window *w : windows)
	{
		if (w->window_id == next_id)
			next_id++;
	}

	// first window is main automaticly
	if (mainWindow == nullptr)
	{
		mainWindow = newWindow;
		newWindow->isMainWindow = true;
	}

	// set id and class name for new window
	newWindow->window_id = next_id;
	newWindow->window_class_name = L"WindowClass" + std::to_wstring((next_id));

	// add new window to framework (program)
	windows.push_back(newWindow);
}
void Framework::RemoveWindow(Window *oldWindow)
{
	for (unsigned int i = 0; i < windows.size(); i++)
	{
		if (windows[i] == oldWindow)
			windows.erase(windows.begin() + i);
	}
}
void Framework::SetAsMainWindow(Window *window)
{
	if (window == mainWindow) return;

	for (Window *w : windows)
	{
		if (w->isMainWindow)
		{
			w->isMainWindow = false;
			w->SetCaption(w->GetCaption());
		}
	}

	mainWindow = window;
	window->isMainWindow = true;
	window->SetCaption(window->GetCaption());
}
void Framework::Exit(int return_value)
{
	PostQuitMessage(return_value);
}
int Framework::ShowGlobalMessageBox(std::wstring text, std::wstring caption, UINT message_box_style)
{
	return MessageBoxW(NULL, text.c_str(), caption.c_str(), message_box_style);
}
void Framework::SetFreeTimeFunction(void(*freeTimeFunction)())
{
	if (freeTimeFunction == nullptr) return;
	FreeTimeProcess = freeTimeFunction;
}
void Framework::UnsetFreeTimeFunction()
{
	FreeTimeProcess = nullptr;
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
		while ((PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) && (eventCounter < 2048u))
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

		// call function between messages
		if (FreeTimeProcess) (*FreeTimeProcess)();
	}
	return 0;
}
// [CLASS] Framework ---------------------------|