#include "Precompiled.h"
#include "Panel.h"
#include "WinApiFramework.h"

namespace WinApiFramework
{
	// ~~~~~~~~ [CLASS] Panel ~~~~~~~~ //
	Panel::Panel(ParentControl* parentControl, const ConStruct<Panel>& conStruct)
		: ChildControl(parentControl, conStruct)
		, HasWindowProcedure(this, &Panel::WindowProcedure)
		, Events(m_events)
	{
		CreateControlWindow();
	}
	Panel::~Panel()
	{
		DestroyControlWindow();
	}


	LRESULT Panel::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			// on window controls events //
			case WM_COMMAND:
			case WM_NOTIFY:
				return ProcessChildMessage(wParam, lParam);

			case WM_SIZE:
				//m_events.PushEvent(Event(Event::Type::Resize));
				return 1;

			default: return 1;
		}
		return 0;
	}
	LRESULT Panel::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}
	bool Panel::CreateControlWindow()
	{
		// [>] Create WindowClassEx
		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(WNDCLASSEX));


		wc.hInstance = Framework::GetProgramInstance();
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
		wc.hbrBackground = (HBRUSH)(COLOR_DESKTOP);

		// register class
		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, L"Register panel class failed.", L"Register error", MB_OK | MB_ICONERROR);
			return false;
		}

		// [>] Create window
		m_hWindow = CreateWindow((LPCWSTR)window_class_name.c_str(), L"caption",
			//m_controlStyle, 
			WS_VISIBLE | WS_BORDER | WS_CHILD | WS_CLIPCHILDREN,
			m_rect.position.x - m_pParentControl->GetCanvasPosition().x,
			m_rect.position.y - m_pParentControl->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			m_pParentControl->GetWindowHandle(), nullptr, Framework::hProgramInstance, nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"Window creation failed.", L"Window create error", MB_OK | MB_ICONERROR);
			return false;
		}

		// set pointer to Window class to receive messages
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_windowProcedure);

		return true;
	}
	void Panel::DestroyControlWindow()
	{
		DestroyWindow(m_hWindow);
	}

	Point Panel::GetMousePosition() const
	{
		return m_pParentControl->GetMousePosition() - m_rect.position;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
}