#include "Precompiled.h"
#include "GroupBox.h"
#include "WinApiFramework.h"

namespace WinApiFramework
{
	// ~~~~~~~~ [CLASS] GroupBox ~~~~~~~~ //
	// ~~ GroupBox::constructor ~~ //
	GroupBox::GroupBox(ParentControl* parentControl, const ConStruct<GroupBox>& conStruct)
		: ChildControl(parentControl, conStruct)
	{
		m_caption = conStruct.caption;
		m_caption_position = conStruct.caption_position;

		CreateControlWindow();
	}
	GroupBox::~GroupBox()
	{
		DestroyControlWindow();
	}

	// ~~ GroupBox::methods ~~ //
	LRESULT GroupBox::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		UINT event = HIWORD(wParam);
		switch (event)
		{
			default:
				return 1;
		}
		return 0;
	}
	bool GroupBox::CreateControlWindow()
	{
		// [>] Set groupbox style
		m_controlStyle |= (BS_NOTIFY | BS_GROUPBOX);
		//m_controlStyle |= BS_HOLLOW;

		// set caption position
		switch (m_caption_position)
		{
			case WinApiFramework::GroupBox::Left:	m_controlStyle |= BS_LEFT;		break;
			case WinApiFramework::GroupBox::Center:	m_controlStyle |= BS_CENTER;	break;
			case WinApiFramework::GroupBox::Right:	m_controlStyle |= BS_RIGHT;		break;
		}


		// [>] Create window
		m_hWindow = CreateWindow(L"BUTTON", m_caption.c_str(),
			m_controlStyle,
			m_rect.position.x - m_pParentControl->GetCanvasPosition().x,
			m_rect.position.y - m_pParentControl->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			m_pParentControl->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"GroupBox creation failed.", L"GroupBox create error", MB_OK | MB_ICONERROR);
			return false;
		}

		HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);

		return true;
	}
	void GroupBox::DestroyControlWindow()
	{
		::DestroyWindow(m_hWindow);
	}
	
	void GroupBox::SetCaption(std::wstring newCaption)
	{
		m_caption = newCaption;
		SetWindowText(m_hWindow, m_caption.c_str());
		m_events.PushEvent(Event(Event::Type::CaptionChanged));
	}
	void GroupBox::SetCaptionPosition(GroupBox::CaptionPosition captionPosition)
	{
		m_caption_position = captionPosition;

		m_controlStyle = GetWindowStyle(m_hWindow);
		m_controlStyle = m_controlStyle & (~(BS_LEFT | BS_CENTER | BS_RIGHT));

		switch (m_caption_position)
		{
			case WinApiFramework::GroupBox::Left:	m_controlStyle |= BS_LEFT;		break;
			case WinApiFramework::GroupBox::Center:	m_controlStyle |= BS_CENTER;	break;
			case WinApiFramework::GroupBox::Right:	m_controlStyle |= BS_RIGHT;		break;
		}

		SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

		RECT r;
		r.left = m_rect.position.x;
		r.top = m_rect.position.y;
		r.right = m_rect.position.x + m_rect.size.width;
		r.bottom = m_rect.position.y + m_rect.size.height;
		InvalidateRect(m_pParentControl->GetWindowHandle(), &r, TRUE);

		m_events.PushEvent(Event(Event::Type::CaptionPositionChanged));
	}
	GroupBox::CaptionPosition GroupBox::GetCaptionPosition()
	{
		return m_caption_position;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
}