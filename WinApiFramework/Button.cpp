#include "Precompiled.h"
#include "Button.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;


// [CLASS] Button ------------------------------|
// -- constructors -- //
Button::Button(const Button::ConStruct& conStruct)
	: WindowControl(conStruct),
	Rect(rect),
	Caption(caption),
	Events(events)
{

	caption = conStruct.caption;
}
Button::~Button()
{

}

// -- methods -- //
// private:
int Button::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	case BN_CLICKED:
		events.PushEvent(Button::Event(Button::Event::Type::Click));
		break;

	case BN_DBLCLK:
		events.PushEvent(Button::Event(Button::Event::Type::DoubleClick));
		break;

	case BN_SETFOCUS:
		events.PushEvent(Button::Event(Button::Event::Type::Focus));
		break;

	case BN_KILLFOCUS:
		events.PushEvent(Button::Event(Button::Event::Type::Unfocus));
		break;

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool Button::CreateControlWindow()
{
	controlStyle |= BS_NOTIFY | BS_PUSHBUTTON | BS_MULTILINE;

	// create window
	hControl = CreateWindow(L"BUTTON", caption.c_str(),
		controlStyle,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!hControl)
	{
		MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
		return false;
	}
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	return true;
}

// public:
void Button::SetCaption(std::wstring newCaption)
{
	caption = newCaption;
	SetWindowText(hControl, caption.c_str());
	events.PushEvent(Button::Event(Button::Event::Type::CaptionChanged));
}
// [CLASS] Button ------------------------------|