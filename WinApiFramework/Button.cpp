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

	CreateControlWindow();
}
Button::~Button()
{
	DestroyControlWindow();
}


// -- methods -- //
// private:
int Button::ControlProcedure(WPARAM wParam, LPARAM lParam)
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
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool Button::CreateControlWindow()
{
	m_controlStyle |= BS_NOTIFY | BS_PUSHBUTTON | BS_MULTILINE | BS_CENTER;

	// create window
	m_hWindow = CreateWindow(L"BUTTON", m_caption.c_str(),
		m_controlStyle,
		m_rect.position.x, m_rect.position.y, m_rect.size.width, m_rect.size.height,
		m_pParentControl->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!m_hWindow)
	{
		MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
		return false;
	}
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);

	return true;
}
void Button::DestroyControlWindow()
{
	DestroyWindow(m_hWindow);
}

// public:
void Button::SetCaption(std::wstring newCaption)
{
	m_caption = newCaption;
	SetWindowText(m_hWindow, m_caption.c_str());
	m_events.PushEvent(Button::Event(Button::Event::Type::CaptionChanged, this));
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~