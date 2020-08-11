#include "checkbox.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	CheckBox::CheckBox(ParentWindow* parent, const ConStruct<CheckBox>& conStruct)
		: BaseWindow(parent)
	{
		m_rect = conStruct.rect;
		m_caption = conStruct.caption;
		m_isTripleState = conStruct.isTripleState;
		m_boxState = conStruct.boxState;

		if (this->m_boxState == BoxState::Middle && !this->m_isTripleState)
		{
			this->m_boxState = BoxState::Uncheck;
		}

		CreateWinapiWindow();
	}
	CheckBox::~CheckBox()
	{
		DestroyWinapiWindow();
	}

	LRESULT CheckBox::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		UINT event = HIWORD(wParam);
		switch (event)
		{
			case BN_CLICKED:
			case BN_DBLCLK:
				if (m_boxState == BoxState::Check)
				{
					if (m_isTripleState)
					{
						SendMessage(m_hWindow, BM_SETCHECK, BST_INDETERMINATE, 0);
						m_boxState = BoxState::Middle;
					}
					else
					{
						SendMessage(m_hWindow, BM_SETCHECK, BST_UNCHECKED, 0);
						m_boxState = BoxState::Uncheck;
					}
				}
				else if (m_boxState == BoxState::Middle)
				{
					SendMessage(m_hWindow, BM_SETCHECK, BST_UNCHECKED, 0);
					m_boxState = BoxState::Uncheck;
				}
				else if (m_boxState == BoxState::Uncheck)
				{
					SendMessage(m_hWindow, BM_SETCHECK, BST_CHECKED, 0);
					m_boxState = BoxState::Check;
				}
				RaiseEventByHandler<Events::EventSetState>(m_boxState);
				break;

			default:
				return 1;
		}
		return 0;
	}
	bool CheckBox::CreateWinapiWindow()
	{
		m_window_style |= WS_CHILD | WS_VISIBLE;
		m_window_style |= BS_NOTIFY | BS_CHECKBOX | BS_MULTILINE | BS_AUTO3STATE;

		// create window
		m_hWindow = CreateWindow(L"BUTTON", m_caption.c_str(),
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::GetInstance().GetProgramInstance(), nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"CheckBox window creation failed.", L"CheckBox create error", MB_OK | MB_ICONERROR);
			return false;
		}

		HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);
		SetState(m_boxState);

		return true;
	}
	void CheckBox::DestroyWinapiWindow()
	{
		DestroyWindow(m_hWindow);
	}

	void CheckBox::SetCaption(const std::wstring& newCaption)
	{
		m_caption = newCaption;
		SetWindowText(m_hWindow, m_caption.c_str());

		RaiseEventByHandler<Events::EventSetCaption>();
	}
	void CheckBox::SetState(CheckBox::BoxState newState)
	{
		m_boxState = newState;
		if (m_boxState == BoxState::Check)
		{
			SendMessage(m_hWindow, BM_SETCHECK, BST_CHECKED, 0);
			RaiseEventByHandler<Events::EventSetState>(m_boxState);
			return;
		}

		if (m_boxState == BoxState::Middle && m_isTripleState)
		{
			SendMessage(m_hWindow, BM_SETCHECK, BST_INDETERMINATE, 0);
			RaiseEventByHandler<Events::EventSetState>(m_boxState);
			return;
		}

		if (m_boxState == BoxState::Uncheck)
		{
			SendMessage(m_hWindow, BM_SETCHECK, BST_UNCHECKED, 0);
			RaiseEventByHandler<Events::EventSetState>(m_boxState);
			return;
		}
	}
	void CheckBox::EnableTripleState()
	{
		m_isTripleState = true;
		RaiseEventByHandler<Events::EventEnableTripleState>();
	}
	void CheckBox::DisableTripleState()
	{
		m_isTripleState = false;

		if (m_boxState == CheckBox::BoxState::Middle)
			SetState(CheckBox::BoxState::Uncheck);

		RaiseEventByHandler<Events::EventDisableTripleState>();
	}

	const std::wstring& CheckBox::GetCaption()
	{
		return m_caption;
	}
	CheckBox::BoxState CheckBox::GetState()
	{
		return m_boxState;
	}
	bool CheckBox::TripleStateEnabled()
	{
		return m_isTripleState;
	}
}