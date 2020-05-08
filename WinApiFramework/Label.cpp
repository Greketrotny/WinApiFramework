#include "Precompiled.h"
#include "Label.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;

// [CLASS] Label -------------------------------|
// -- constructors -- //
Label::Label(const Label::ConStruct& conStruct)
	:ChildControl(conStruct),
	Caption(caption),
	Alignment(textAlignment),
	Events(events)
{
	rect = conStruct.rect;
	caption = conStruct.caption;
	textAlignment = conStruct.textAlignment;
}
Label::~Label()
{

}

// -- methods -- //
// private:
int Label::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool Label::CreateControlWindow()
{
	// set text alignment
	if (textAlignment == Label::Left)
		controlStyle |= SS_LEFT;
	if (textAlignment == Label::Center)
		controlStyle |= SS_CENTER;
	if (textAlignment == Label::Right)
		controlStyle |= SS_RIGHT;

	// create window
	hControl = CreateWindow(L"STATIC", caption.c_str(),
		controlStyle,
		rect.position.x, rect.position.y, rect.size.width, rect.size.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!hControl)
	{
		MessageBox(nullptr, L"Label window creation failed.", L"Label create error", MB_OK | MB_ICONERROR);
		return false;
	}

	// set visual font
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	return true;
}
// public:
void Label::SetCaption(std::wstring newCaption)
{
	caption = newCaption;
	SetWindowText(hControl, caption.c_str());

	events.PushEvent(Label::Event(Label::Event::Type::CaptionChange));
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

	controlStyle = (controlStyle & ~(SS_LEFT | SS_CENTER | SS_RIGHT | SS_LEFTNOWORDWRAP)) | newStyle;
	SetWindowLong(hControl, GWL_STYLE, controlStyle);
	InvalidateRect(hControl, NULL, TRUE);

	events.PushEvent(Label::Event(Label::Event::Type::TextAlignmentChange));
}
// [CLASS] Label -------------------------------|