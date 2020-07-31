#include "button.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	Button::Button(ParentWindow* parent, const ConStruct<Button>& conStruct)
		: BaseWindow(parent)
	{
		m_window_rect = conStruct.rect;
		m_caption = conStruct.caption;
		m_caption_position = conStruct.caption_position;

		CreateWinapiWindow();
	}
	Button::~Button()
	{
		DestroyWinapiWindow();
	}

	LRESULT Button::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		UINT event = HIWORD(wParam);
		switch (event)
		{
			case BN_CLICKED:
				//m_events.PushEvent(Button::Event(Button::Event::Type::Click, this));
				break;

			case BN_DBLCLK:
				//m_events.PushEvent(Button::Event(Button::Event::Type::DoubleClick, this));
				break;

			case BN_SETFOCUS:
				//m_events.PushEvent(Button::Event(Button::Event::Type::Focus, this));
				break;

			case BN_KILLFOCUS:
				//m_events.PushEvent(Button::Event(Button::Event::Type::Unfocus, this));
				break;

			default:
				return 1;
		}
		return 0;
	}
	bool Button::CreateWinapiWindow()
	{
		// [>] Set button styles
		// set button appearance
		m_window_style |= WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_PUSHBUTTON | BS_MULTILINE;

		// set caption position
		switch (m_caption_position)
		{
			case WinapiFramework::Button::Center:		m_window_style |= BS_CENTER;			break;
			case WinapiFramework::Button::TopLeft:		m_window_style |= BS_TOP | BS_LEFT;		break;
			case WinapiFramework::Button::TopCenter:	m_window_style |= BS_TOP | BS_CENTER;	break;
			case WinapiFramework::Button::TopRight:		m_window_style |= BS_TOP | BS_RIGHT;	break;
			case WinapiFramework::Button::MiddleLeft:	m_window_style |= BS_LEFT;				break;
			case WinapiFramework::Button::MiddleCenter:	m_window_style |= BS_CENTER;			break;
			case WinapiFramework::Button::MiddleRight:	m_window_style |= BS_RIGHT;				break;
			case WinapiFramework::Button::BottomLeft:	m_window_style |= BS_BOTTOM | BS_LEFT;	break;
			case WinapiFramework::Button::BottomCenter:	m_window_style |= BS_BOTTOM;			break;
			case WinapiFramework::Button::BottomRight:	m_window_style |= BS_BOTTOM | BS_RIGHT;	break;
		}

		// [>] Create window
		m_hWindow = CreateWindow(L"BUTTON", m_caption.c_str(),
			m_window_style,
			m_window_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_window_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_window_rect.size.width, m_window_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
			return false;
		}

		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hFont, 0);

		return true;
	}
	void Button::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
	}

	void Button::SetCaption(std::wstring newCaption)
	{
		m_caption = newCaption;
		SetWindowText(m_hWindow, m_caption.c_str());

		//m_events.PushEvent(Button::Event(Button::Event::Type::CaptionChanged, this));
	}
	void Button::SetCaptionPosition(Button::CaptionPosition captionPosition)
	{
		m_caption_position = captionPosition;

		m_window_style = GetWindowStyle(m_hWindow);
		m_window_style = m_window_style & (~(BS_TOP | BS_BOTTOM | BS_LEFT | BS_CENTER | BS_RIGHT));
		switch (m_caption_position)
		{
			case WinapiFramework::Button::Center:
			case WinapiFramework::Button::MiddleCenter:	m_window_style |= BS_CENTER;			break;
			case WinapiFramework::Button::TopLeft:		m_window_style |= BS_TOP | BS_LEFT;		break;
			case WinapiFramework::Button::TopCenter:	m_window_style |= BS_TOP | BS_CENTER;	break;
			case WinapiFramework::Button::TopRight:		m_window_style |= BS_TOP | BS_RIGHT;	break;
			case WinapiFramework::Button::MiddleLeft:	m_window_style |= BS_LEFT;				break;
			case WinapiFramework::Button::MiddleRight:	m_window_style |= BS_RIGHT;				break;
			case WinapiFramework::Button::BottomLeft:	m_window_style |= BS_BOTTOM | BS_LEFT;	break;
			case WinapiFramework::Button::BottomCenter:	m_window_style |= BS_BOTTOM;			break;
			case WinapiFramework::Button::BottomRight:	m_window_style |= BS_BOTTOM | BS_RIGHT;	break;
		}

		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);
		RedrawWindow(m_hWindow, NULL, NULL, RDW_INVALIDATE);

		//m_events.PushEvent(Event(Event::Type::CaptionPositionChanged, this));
	}

	const std::wstring& Button::GetCaption() const
	{
		return m_caption;
	}
	Button::CaptionPosition Button::GetCaptionPosition() const
	{
		return m_caption_position;
	}
}