#include "label.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	Label::Label(ParentControl* parentControl, const ConStruct<Label>& conStruct)
		: ChildControl(parentControl, conStruct)
		, Caption(m_caption)
		, Alignment(m_textAlignment)
		, Events(m_events)
	{
		m_rect = conStruct.rect;
		m_caption = conStruct.caption;
		m_textAlignment = conStruct.textAlignment;

		CreateWinapiWindow();
	}
	Label::~Label()
	{
		DestroyWinapiWindow();
	}

	LRESULT Label::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		UINT event = HIWORD(wParam);
		switch (event)
		{
			case STN_CLICKED:
				m_events.PushEvent(Event(Event::Type::Clicked));
				break;

			case STN_DBLCLK:
				m_events.PushEvent(Event(Event::Type::DoubleClicked));
				break;

			case STN_ENABLE:
				// Handled by base class ChildControl
				break;

			case STN_DISABLE:
				// Handled by base class ChildControl
				break;

			default:
				return 1;
		}
		return 0;
	}
	bool Label::CreateWinapiWindow()
	{
		// set text alignment
		if (m_textAlignment == Label::Left)
			m_controlStyle |= SS_LEFT;
		if (m_textAlignment == Label::Center)
			m_controlStyle |= SS_CENTER;
		if (m_textAlignment == Label::Right)
			m_controlStyle |= SS_RIGHT;

		// for notifications from parent control
		m_controlStyle |= SS_NOTIFY;

		// create window
		m_hWindow = CreateWindow(L"STATIC", m_caption.c_str(),
			m_controlStyle,
			m_rect.position.x - m_pParentControl->GetCanvasPosition().x,
			m_rect.position.y - m_pParentControl->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			m_pParentControl->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

		// check control creation
		if (!m_hWindow)
		{
			MessageBox(nullptr, L"Label window creation failed.", L"Label create error", MB_OK | MB_ICONERROR);
			return false;
		}

		// set visual font
		HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);

		return true;
	}
	void Label::DestroyWinapiWindow()
	{
		DestroyWindow(m_hWindow);
	}

	void Label::SetCaption(std::wstring newCaption)
	{
		m_caption = newCaption;
		SetWindowText(m_hWindow, m_caption.c_str());

		m_events.PushEvent(Label::Event(Label::Event::Type::CaptionChanged));
	}
	void Label::SetTextAligment(Label::TextAlignment textAlignment)
	{
		unsigned int newStyle = 0u;
		if (textAlignment == Label::Left)
			newStyle = SS_LEFT;
		if (textAlignment == Label::Center)
			newStyle = SS_CENTER;
		if (textAlignment == Label::Right)
			newStyle = SS_RIGHT;

		m_controlStyle = (m_controlStyle & ~(SS_LEFT | SS_CENTER | SS_RIGHT | SS_LEFTNOWORDWRAP)) | newStyle;
		SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);
		InvalidateRect(m_hWindow, NULL, TRUE);

		m_events.PushEvent(Label::Event(Label::Event::Type::TextAlignmentChanged));
	}
}