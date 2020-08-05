#include "panel.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	Panel::Panel(ParentWindow* parent, const ConStruct<Panel>& conStruct)
		: ParentWindow(parent)
		, HasWindowProcedure(this, &Panel::WindowProcedure)
	{
		m_rect = conStruct.rect;
		m_window_class_name = mp_parent->GetWindowClassName() + L"PanelClass" + std::to_wstring(int(this));

		CreateWinapiWindow();
	}
	Panel::~Panel()
	{
		DestroyWinapiWindow();
	}


	LRESULT Panel::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (HandleMouseEvent(msg, wParam, lParam) == 0) return 0;

		switch (msg)
		{
			case WM_COMMAND:
			case WM_NOTIFY:
				// try to find and process message by child controls
				return ProcessChildMessage(wParam, lParam);

			default: return 1;
		}
		return 0;
	}
	LRESULT Panel::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}
	bool Panel::CreateWinapiWindow()
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
		wc.hbrBackground = (HBRUSH)(COLOR_DESKTOP);

		// register class
		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, L"Register panel class failed.", L"Register error", MB_OK | MB_ICONERROR);
			return false;
		}

		// [>] Create window
		m_hWindow = CreateWindow((LPCWSTR)m_window_class_name.c_str(), L"caption",
			//m_controlStyle, 
			WS_VISIBLE | WS_BORDER | WS_CHILD | WS_CLIPCHILDREN,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::GetProgramInstance(), nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"Window creation failed.", L"Window create error", MB_OK | MB_ICONERROR);
			return false;
		}

		// set pointer to Window class to receive messages
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_window_procedure);

		return true;
	}
	void Panel::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
		UnregisterClass(m_window_class_name.c_str(), Framework::GetProgramInstance());
	}
}