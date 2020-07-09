#include "Precompiled.h"
#include "Edit.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;

// [CLASS] Edit --------------------------------|
// -- constructors -- //
Edit::Edit(ParentControl* parentControl, const ConStruct<Edit>& conStruct)
	: ChildControl(parentControl, conStruct)
	, PasswordMode(m_passwordMode)
	, NumberOnlyMode(m_numberOnly)
	, Alignment(m_textAlignment)
	, LetterMode(m_lettersMode)
	, Events(m_events)
{
	m_text = conStruct.text;
	m_passwordMode = conStruct.passwordMode;
	m_numberOnly = conStruct.numberOnly;
	m_textAlignment = conStruct.textAlignment;
	m_lettersMode = conStruct.lettersMode;
	m_textLengthLimit = conStruct.textLengthLimit;

	m_controlStyle |= WS_BORDER;

	CreateControlWindow();
}
Edit::~Edit()
{
	DestroyControlWindow();
}

// -- methods -- //
// private:
ProcedureResult Edit::ControlProcedure(WPARAM wParam, LPARAM lParam)
{
	if ((HWND)lParam != m_hWindow) return ProcedureResult::TargetNotFound;

	UINT event = HIWORD(wParam);
	switch (event)
	{
		case EN_CHANGE:
			m_events.PushEvent(Edit::Event(Edit::Event::Type::TextChanged));
			break;

		case EN_MAXTEXT:
			m_events.PushEvent(Edit::Event(Edit::Event::Type::TextLimitReached));
			break;

		case EN_ERRSPACE:

			break;

		case EN_HSCROLL:
			m_events.PushEvent(Edit::Event(Edit::Event::Type::HorizontalScroll));
			break;

		case EN_VSCROLL:

			break;

		case EN_SETFOCUS:
			m_events.PushEvent(Edit::Event(Edit::Event::Type::FocusSet));
			break;

		case EN_KILLFOCUS:
			m_events.PushEvent(Edit::Event(Edit::Event::Type::FocusKilled));
			break;

		default:
			return ProcedureResult::Unhandled;
	}
	return ProcedureResult::Handled;
}
bool Edit::CreateControlWindow()
{
	// set text alignment
	if (m_textAlignment == Edit::TextAlignment::Left)
		m_controlStyle |= ES_LEFT;
	if (m_textAlignment == Edit::TextAlignment::Center)
		m_controlStyle |= ES_CENTER;
	if (m_textAlignment == Edit::TextAlignment::Right)
		m_controlStyle |= ES_RIGHT;

	// set letters mode
	if (m_lettersMode == Edit::LettersMode::LowerCase)
		m_controlStyle |= ES_LOWERCASE;
	if (m_lettersMode == Edit::LettersMode::UpperCase)
		m_controlStyle |= ES_UPPERCASE;

	// set password mode
	if (m_passwordMode)
		m_controlStyle |= ES_PASSWORD;

	// set number only mode
	if (m_numberOnly)
		m_controlStyle |= ES_NUMBER;

	m_controlStyle |= ES_MULTILINE;
	m_controlStyle |= ES_AUTOVSCROLL;

	// create window
	m_hWindow = CreateWindow(L"EDIT", m_text.c_str(),
		m_controlStyle,
		m_rect.position.x, m_rect.position.y, m_rect.size.width, m_rect.size.height,
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

	SetTextLengthLimit(m_textLengthLimit);
	return true;
}
void Edit::DestroyControlWindow()
{
	DestroyWindow(m_hWindow);
}
// public:
void Edit::SetText(std::wstring newText)
{
	m_text = newText;
	SetWindowText(m_hWindow, m_text.c_str());

	m_events.PushEvent(Edit::Event(Edit::Event::Type::TextChanged));
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

	m_controlStyle = (m_controlStyle & ~(ES_LEFT | ES_CENTER | ES_RIGHT) | newAlignment);
	SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

	m_events.PushEvent(Edit::Event(Edit::Event::Type::TextAlignmentSet));
}
void Edit::SetLettersMode(Edit::LettersMode newLettersMode)
{
	m_lettersMode = newLettersMode;

	unsigned int newMode = 0u;
	if (newLettersMode == Edit::LettersMode::LowerCase)
		newMode |= ES_LOWERCASE;
	if (newLettersMode == Edit::LettersMode::UpperCase)
		newMode |= ES_UPPERCASE;

	m_controlStyle = (m_controlStyle & ~(ES_LOWERCASE | ES_UPPERCASE) | newMode);
	SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

	m_events.PushEvent(Edit::Event(Edit::Event::Type::LettersModeSet));
}
void Edit::SetPasswordMode(bool newPasswordMode)
{
	if (m_passwordMode != newPasswordMode)
	{
		m_passwordMode = newPasswordMode;

		m_controlStyle ^= ES_PASSWORD;
		SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

		m_events.PushEvent(Edit::Event(Edit::Event::Type::PasswordModeSet));
	}
}
void Edit::SetNumberOnlyMode(bool numberOnlyMode)
{
	if (m_numberOnly != numberOnlyMode)
	{
		m_numberOnly = numberOnlyMode;

		if (m_numberOnly)
			m_controlStyle |= ES_NUMBER;
		else
			m_controlStyle = (m_controlStyle & (~ES_NUMBER));
		SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

		m_events.PushEvent(Edit::Event(Edit::Event::Type::NumberModeSet));
	}
}
void Edit::SetTextLengthLimit(unsigned int lengthLimit)
{
	m_textLengthLimit = lengthLimit;
	SendMessage(m_hWindow, EM_LIMITTEXT, m_textLengthLimit, 0u);
}
std::wstring Edit::GetText()
{
	wchar_t* buffer = new wchar_t[m_textLengthLimit];
	GetWindowText(m_hWindow, buffer, m_textLengthLimit);
	m_text = buffer;
	return m_text;
}
// [CLASS] Edit --------------------------------|