#include "groupbox.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	GroupBox::GroupBox(ParentWindow* parent, const ConStruct<GroupBox>& conStruct)
		: BaseWindow(parent)
	{
		m_rect = conStruct.rect;
		m_caption = conStruct.caption;
		m_caption_position = conStruct.caption_position;

		CreateWinapiWindow();
	}
	GroupBox::~GroupBox()
	{
		DestroyWinapiWindow();
	}

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
	bool GroupBox::CreateWinapiWindow()
	{
		m_window_style |= WS_CHILD | WS_VISIBLE;

		// [>] Set groupbox style
		m_window_style |= (BS_NOTIFY | BS_GROUPBOX);
		//m_window_style |= BS_HOLLOW;

		// set caption position
		switch (m_caption_position)
		{
			case WinapiFramework::GroupBox::CaptionPosition::Left:		m_window_style |= BS_LEFT;		break;
			case WinapiFramework::GroupBox::CaptionPosition::Center:	m_window_style |= BS_CENTER;	break;
			case WinapiFramework::GroupBox::CaptionPosition::Right:		m_window_style |= BS_RIGHT;		break;
		}


		// [>] Create window
		m_hWindow = CreateWindow(L"BUTTON", m_caption.c_str(),
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"GroupBox creation failed.", L"GroupBox create error", MB_OK | MB_ICONERROR);
			return false;
		}

		HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);

		return true;
	}
	void GroupBox::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
	}
	
	void GroupBox::SetCaption(const std::wstring& newCaption)
	{
		m_caption = newCaption;
		SetWindowText(m_hWindow, m_caption.c_str());
		RaiseEventByHandler<Events::EventSetCaption>();
	}
	void GroupBox::SetCaptionPosition(GroupBox::CaptionPosition captionPosition)
	{
		m_caption_position = captionPosition;

		m_window_style = GetWindowStyle(m_hWindow);
		m_window_style = m_window_style & (~(BS_LEFT | BS_CENTER | BS_RIGHT));

		switch (m_caption_position)
		{
			case WinapiFramework::GroupBox::CaptionPosition::Left:	m_window_style |= BS_LEFT;		break;
			case WinapiFramework::GroupBox::CaptionPosition::Center:m_window_style |= BS_CENTER;	break;
			case WinapiFramework::GroupBox::CaptionPosition::Right:	m_window_style |= BS_RIGHT;		break;
		}

		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

		RECT r;
		r.left = m_rect.position.x;
		r.top = m_rect.position.y;
		r.right = m_rect.position.x + m_rect.size.width;
		r.bottom = m_rect.position.y + m_rect.size.height;
		InvalidateRect(mp_parent->GetWindowHandle(), &r, TRUE);

		RaiseEventByHandler<Events::EventSetCaptionPosition>();
	}

	const std::wstring& GroupBox::GetCaption()
	{
		return m_caption;
	}
	GroupBox::CaptionPosition GroupBox::GetCaptionPosition()
	{
		return m_caption_position;
	}
}