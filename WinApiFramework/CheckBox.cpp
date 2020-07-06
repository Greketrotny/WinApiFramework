#include "Precompiled.h"
#include "CheckBox.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;


// [CLASS] CheckBox ----------------------------|
// -- constructors -- //
CheckBox::CheckBox(ParentControl* parentControl, const ConStruct<CheckBox>& conStruct)
	: ChildControl(parentControl, conStruct)
	, State(m_boxState)
	, Caption(m_caption)
	, IsTripleState(m_isTripleState)
	, Events(m_events)
{
	m_caption = conStruct.caption;
	m_isTripleState = conStruct.isTripleState;
	m_boxState = conStruct.boxState;

	if (!this->m_isTripleState && this->m_boxState == MiddleState)
	{
		this->m_boxState = UnCheck;
	}

	CreateControlWindow();
}
CheckBox::~CheckBox()
{
	DestroyControlWindow();
}

// -- methods -- //
// private:
int CheckBox::ControlProcedure(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	case BN_CLICKED:
	case BN_DBLCLK:
		if (m_boxState == Check)
		{
			if (m_isTripleState)
			{
				SendMessage(m_hWindow, BM_SETCHECK, BST_INDETERMINATE, 0);
				m_boxState = MiddleState;
				m_events.PushEvent(CheckBox::Event(CheckBox::Event::Type::MiddleState));
			}
			else
			{
				SendMessage(m_hWindow, BM_SETCHECK, BST_UNCHECKED, 0);
				m_boxState = UnCheck;
				m_events.PushEvent(CheckBox::Event(CheckBox::Event::Type::UnCheck));
			}
		}
		else if (m_boxState == MiddleState)
		{
			SendMessage(m_hWindow, BM_SETCHECK, BST_UNCHECKED, 0);
			m_boxState = UnCheck;
			m_events.PushEvent(CheckBox::Event(CheckBox::Event::Type::UnCheck));
		}
		else if (m_boxState == UnCheck)
		{
			SendMessage(m_hWindow, BM_SETCHECK, BST_CHECKED, 0);
			m_boxState = Check;
			m_events.PushEvent(CheckBox::Event(CheckBox::Event::Type::Check));
		}
		break;

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool CheckBox::CreateControlWindow()
{
	m_controlStyle |= BS_NOTIFY | BS_CHECKBOX | BS_MULTILINE | BS_AUTO3STATE;

	// create window
	m_hWindow = CreateWindow(L"BUTTON", m_caption.c_str(),
		m_controlStyle,
		m_rect.position.x, m_rect.position.y, m_rect.size.width, m_rect.size.height,
		m_pParentControl->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!m_hWindow)
	{
		MessageBox(nullptr, L"CheckBox window creation failed.", L"CheckBox create error", MB_OK | MB_ICONERROR);
		return false;
	}

	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SetBoxState(m_boxState);

	return true;
}
void CheckBox::DestroyControlWindow()
{
	DestroyWindow(m_hWindow);
}

// public:
void CheckBox::SetCaption(std::wstring newCaption)
{
	m_caption = newCaption;
	SetWindowText(m_hWindow, m_caption.c_str());
}
void CheckBox::SetBoxState(CheckBox::BoxState newState)
{
	m_boxState = newState;
	if (m_boxState == Check)
	{
		SendMessage(m_hWindow, BM_SETCHECK, BST_CHECKED, 0);
		m_events.PushEvent(CheckBox::Event(CheckBox::Event::Type::Check));
		return;
	}

	if (m_boxState == MiddleState && m_isTripleState)
	{
		SendMessage(m_hWindow, BM_SETCHECK, BST_INDETERMINATE, 0);
		m_events.PushEvent(CheckBox::Event(CheckBox::Event::Type::MiddleState));
		return;
	}

	if (m_boxState == UnCheck)
	{
		SendMessage(m_hWindow, BM_SETCHECK, BST_UNCHECKED, 0);
		m_events.PushEvent(CheckBox::Event(CheckBox::Event::Type::UnCheck));
		return;
	}
}
void CheckBox::SetBoxState(unsigned int newState)
{
	if (newState == 0) { SetBoxState(CheckBox::BoxState::UnCheck); return; }
	if (newState == 1) { SetBoxState(CheckBox::BoxState::Check); return; }
	if (newState == 2 && m_isTripleState) { SetBoxState(CheckBox::BoxState::MiddleState); return; }
}
void CheckBox::EnableTripleState()
{
	m_isTripleState = true;
}
void CheckBox::DisableTripleState()
{
	m_isTripleState = false;

	if (m_boxState == CheckBox::BoxState::MiddleState)
		SetBoxState(CheckBox::BoxState::UnCheck);
}
// [CLASS] CheckBox ----------------------------|