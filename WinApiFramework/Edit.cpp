#include "Precompiled.h"
#include "Edit.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;

// [CLASS] Edit --------------------------------|
// -- constructors -- //
Edit::Edit(const Edit::ConStruct& conStruct)
	:ChildControl(conStruct),
	PasswordMode(passwordMode),
	NumberOnlyMode(numberOnly),
	Alignment(textAlignment),
	LetterMode(lettersMode),
	Events(events)
{
	text = conStruct.text;
	passwordMode = conStruct.passwordMode;
	numberOnly = conStruct.numberOnly;
	textAlignment = conStruct.textAlignment;
	lettersMode = conStruct.lettersMode;
	textLengthLimit = conStruct.textLengthLimit;

	controlStyle |= WS_BORDER;
}
Edit::~Edit()
{

}

// -- methods -- //
// private:
int Edit::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	case EN_MAXTEXT:
		events.PushEvent(Edit::Event(Edit::Event::Type::TextLimitReach));
		break;

	case EN_UPDATE:
		events.PushEvent(Edit::Event(Edit::Event::Type::Text));
		break;


	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool Edit::CreateControlWindow()
{
	// set text alignment
	if (textAlignment == Edit::TextAlignment::Left)
		controlStyle |= ES_LEFT;
	if (textAlignment == Edit::TextAlignment::Center)
		controlStyle |= ES_CENTER;
	if (textAlignment == Edit::TextAlignment::Right)
		controlStyle |= ES_RIGHT;

	// set letters mode
	if (lettersMode == Edit::LettersMode::LowerCase)
		controlStyle |= ES_LOWERCASE;
	if (lettersMode == Edit::LettersMode::UpperCase)
		controlStyle |= ES_UPPERCASE;

	// set password mode
	if (passwordMode)
		controlStyle |= ES_PASSWORD;

	// set number only mode
	if (numberOnly)
		controlStyle |= ES_NUMBER;

	controlStyle |= ES_MULTILINE;

	// create window
	hControl = CreateWindow(L"EDIT", text.c_str(),
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

	SetTextLengthLimit(textLengthLimit);
	return true;
}
// public:
void Edit::SetText(std::wstring newText)
{
	text = newText;
	SetWindowText(hControl, text.c_str());

	events.PushEvent(Edit::Event(Edit::Event::Type::Text));
}
void Edit::SetTextAlignment(Edit::TextAlignment newTextAlignment)
{
	// set text alignment
	unsigned int newAlignment = 0u;
	if (newTextAlignment == Edit::TextAlignment::Left)
		newAlignment |= ES_LEFT;
	if (newTextAlignment == Edit::TextAlignment::Center)
		newAlignment |= ES_CENTER;
	if (newTextAlignment == Edit::TextAlignment::Right)
		newAlignment |= ES_RIGHT;

	controlStyle = (controlStyle & ~(ES_LEFT | ES_CENTER | ES_RIGHT) | newAlignment);
	SetWindowLong(hControl, GWL_STYLE, controlStyle);

	events.PushEvent(Edit::Event(Edit::Event::Type::TextAlignment));
}
void Edit::SetLettersMode(Edit::LettersMode newLettersMode)
{
	lettersMode = newLettersMode;

	unsigned int newMode = 0u;
	if (newLettersMode == Edit::LettersMode::LowerCase)
		newMode |= ES_LOWERCASE;
	if (newLettersMode == Edit::LettersMode::UpperCase)
		newMode |= ES_UPPERCASE;

	controlStyle = (controlStyle & ~(ES_LOWERCASE | ES_UPPERCASE) | newMode);
	SetWindowLong(hControl, GWL_STYLE, controlStyle);

	events.PushEvent(Edit::Event(Edit::Event::Type::LettersMode));
}
void Edit::SetPasswordMode(bool newPasswordMode)
{
	if (passwordMode != newPasswordMode)
	{
		passwordMode = newPasswordMode;

		controlStyle ^= ES_PASSWORD;
		SetWindowLong(hControl, GWL_STYLE, controlStyle);

		events.PushEvent(Edit::Event(Edit::Event::Type::PasswordMode));
	}
}
void Edit::SetNumberOnlyMode(bool numberOnlyMode)
{
	if (numberOnly != numberOnlyMode)
	{
		numberOnly = numberOnlyMode;

		if (numberOnly)
			controlStyle |= ES_NUMBER;
		else
			controlStyle = (controlStyle & (~ES_NUMBER));
		SetWindowLong(hControl, GWL_STYLE, controlStyle);

		events.PushEvent(Edit::Event(Edit::Event::Type::NumberMode));
	}
}
void Edit::SetTextLengthLimit(unsigned int lengthLimit)
{
	textLengthLimit = lengthLimit;
	SendMessage(hControl, EM_LIMITTEXT, textLengthLimit, 0u);
}
std::wstring Edit::GetText()
{
	wchar_t* buffer = new wchar_t[textLengthLimit];
	GetWindowText(hControl, buffer, textLengthLimit);
	text = buffer;
	return text;
}
// [CLASS] Edit --------------------------------|