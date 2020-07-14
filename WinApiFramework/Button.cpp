#include "Precompiled.h"
#include "Button.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;


// ~~~~~~~~ [CLASS] Button ~~~~~~~~
// ~~ Button::constructors ~~
Button::Button(ParentControl* parentControl, const ConStruct<Button>& conStruct)
	: ChildControl(parentControl, conStruct)
	, Caption(m_caption)
	, Events(m_events)
{
	m_caption = conStruct.caption;
	m_caption_position = conStruct.caption_position;

	CreateControlWindow();
}
Button::~Button()
{
	DestroyControlWindow();
}


// -- methods -- //
// private:
LRESULT Button::ControlProcedure(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
		case BN_CLICKED:
			m_events.PushEvent(Button::Event(Button::Event::Type::Click, this));
			break;

		case BN_DBLCLK:
			m_events.PushEvent(Button::Event(Button::Event::Type::DoubleClick, this));
			break;

		case BN_SETFOCUS:
			m_events.PushEvent(Button::Event(Button::Event::Type::Focus, this));
			break;

		case BN_KILLFOCUS:
			m_events.PushEvent(Button::Event(Button::Event::Type::Unfocus, this));
			break;

		default:
			return 1;
	}
	return 0;
}
bool Button::CreateControlWindow()
{
	// [>] Set button styles
	// set button appearance
	m_controlStyle |= BS_NOTIFY | BS_PUSHBUTTON | BS_MULTILINE;

	// set caption position
	switch (m_caption_position)
	{
		case WinApiFramework::Button::Center:		m_controlStyle |= BS_CENTER;			break;
		case WinApiFramework::Button::TopLeft:		m_controlStyle |= BS_TOP | BS_LEFT;		break;
		case WinApiFramework::Button::TopCenter:	m_controlStyle |= BS_TOP | BS_CENTER;	break;
		case WinApiFramework::Button::TopRight:		m_controlStyle |= BS_TOP | BS_RIGHT;	break;
		case WinApiFramework::Button::MiddleLeft:	m_controlStyle |= BS_LEFT;				break;
		case WinApiFramework::Button::MiddleCenter:	m_controlStyle |= BS_CENTER;			break;
		case WinApiFramework::Button::MiddleRight:	m_controlStyle |= BS_RIGHT;				break;
		case WinApiFramework::Button::BottomLeft:	m_controlStyle |= BS_BOTTOM | BS_LEFT;	break;
		case WinApiFramework::Button::BottomCenter:	m_controlStyle |= BS_BOTTOM;			break;
		case WinApiFramework::Button::BottomRight:	m_controlStyle |= BS_BOTTOM | BS_RIGHT;	break;
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
		MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
		return false;
	}

	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hFont, 0);

	return true;
}
void Button::DestroyControlWindow()
{
	::DestroyWindow(m_hWindow);
}

// public:
void Button::SetCaption(std::wstring newCaption)
{
	m_caption = newCaption;
	SetWindowText(m_hWindow, m_caption.c_str());
	m_events.PushEvent(Button::Event(Button::Event::Type::CaptionChanged, this));
}
void Button::SetCaptionPosition(Button::CaptionPosition captionPosition)
{
	m_caption_position = captionPosition;

	m_controlStyle = GetWindowStyle(m_hWindow);
	m_controlStyle = m_controlStyle & (~(BS_TOP | BS_BOTTOM | BS_LEFT | BS_CENTER | BS_RIGHT));
	switch (m_caption_position)
	{
		case WinApiFramework::Button::Center:
		case WinApiFramework::Button::MiddleCenter:	m_controlStyle |= BS_CENTER;			break;
		case WinApiFramework::Button::TopLeft:		m_controlStyle |= BS_TOP | BS_LEFT;		break;
		case WinApiFramework::Button::TopCenter:	m_controlStyle |= BS_TOP | BS_CENTER;	break;
		case WinApiFramework::Button::TopRight:		m_controlStyle |= BS_TOP | BS_RIGHT;	break;
		case WinApiFramework::Button::MiddleLeft:	m_controlStyle |= BS_LEFT;				break;
		case WinApiFramework::Button::MiddleRight:	m_controlStyle |= BS_RIGHT;				break;
		case WinApiFramework::Button::BottomLeft:	m_controlStyle |= BS_BOTTOM | BS_LEFT;	break;
		case WinApiFramework::Button::BottomCenter:	m_controlStyle |= BS_BOTTOM;			break;
		case WinApiFramework::Button::BottomRight:	m_controlStyle |= BS_BOTTOM | BS_RIGHT;	break;
	}

	SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);
	RedrawWindow(m_hWindow, NULL, NULL, RDW_INVALIDATE);

	m_events.PushEvent(Event(Event::Type::CaptionPositionChanged, this));
}
Button::CaptionPosition Button::GetCaptionPosition()
{
	return m_caption_position;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~