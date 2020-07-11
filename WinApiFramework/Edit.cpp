#include "Precompiled.h"
#include "Edit.h"
#include "WinApiFramework.h"

namespace WinApiFramework
{
	// [CLASS] Edit --------------------------------|
	// -- constructors -- //
	Edit::Edit(ParentControl* parentControl, const ConStruct<Edit>& conStruct)
		: ChildControl(parentControl, conStruct)
		, Events(m_events)
	{
		m_text = conStruct.text;
		m_cueText = conStruct.cueText;
		m_textAlignment = conStruct.textAlignment;
		m_lettersMode = conStruct.lettersMode;
		m_passwordMode = conStruct.passwordMode;
		m_numberOnly = conStruct.numberOnly;
		m_readOnly = conStruct.readOnly;
		m_multiline = conStruct.multiline;
		m_wordWrap = conStruct.wordWrap;
		m_textLengthLimit = conStruct.textLengthLimit;
		m_passwordChar = conStruct.passwordChar;
		m_scrollingStyle = conStruct.scrollingStyle;

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
				m_events.PushEvent(Event(Event::Type::TextChanged));
				break;

			case EN_MAXTEXT:
				m_events.PushEvent(Event(Event::Type::TextLimitReached));
				break;

			case EN_HSCROLL:
				m_events.PushEvent(Event(Event::Type::HorizontalScroll));
				break;

			case EN_VSCROLL:
				m_events.PushEvent(Event(Event::Type::VerticalScroll));
				break;

			case EN_SETFOCUS:
				m_events.PushEvent(Event(Event::Type::FocusSet));
				break;

			case EN_KILLFOCUS:
				m_events.PushEvent(Event(Event::Type::FocusKilled));
				break;

			default:
				return ProcedureResult::Unhandled;
		}
		return ProcedureResult::Handled;
	}
	bool Edit::CreateControlWindow()
	{
		// [>] Set edit styles
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

		// set read only mode
		if (m_readOnly)
			m_controlStyle |= ES_READONLY;

		// set multiline mode
		if (m_multiline)
			m_controlStyle |= ES_MULTILINE | ES_AUTOVSCROLL;

		// set word wrap mode
		if (!m_wordWrap)
			m_controlStyle |= ES_AUTOHSCROLL;

		// set scrolling style
		switch (m_scrollingStyle)
		{
			case WinApiFramework::Edit::NoScrolling:	break;
			case WinApiFramework::Edit::Horizontal:			m_controlStyle |= WS_HSCROLL;	break;
			case WinApiFramework::Edit::Vertical:			m_controlStyle |= WS_VSCROLL;	break;
			case WinApiFramework::Edit::HorizontalVertical:	m_controlStyle |= WS_HSCROLL | WS_VSCROLL;	break;
		}

		m_controlStyle |= WS_BORDER | ES_NOHIDESEL;


		// [>] Create window
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
		SetCueText(m_cueText);

		return true;
	}
	void Edit::DestroyControlWindow()
	{
		DestroyWindow(m_hWindow);
	}
	// public:
	void Edit::SetText(const std::wstring& newText)
	{
		m_text = newText;
		SetWindowText(m_hWindow, m_text.c_str());

		m_events.PushEvent(Edit::Event(Edit::Event::Type::TextChanged));
	}
	void Edit::SetCueText(const std::wstring& cueText)
	{	// must be wide string ^^^^^^^
		m_cueText = cueText;
		BOOL success = SendMessage(m_hWindow, EM_SETCUEBANNER, TRUE, (LPARAM)m_cueText.c_str());
		m_events.PushEvent(Event(Event::Type::CueTextChanged));
	}
	void Edit::SetTextAlignment(Edit::TextAlignment newTextAlignment)
	{
		m_textAlignment = newTextAlignment;

		m_controlStyle = (m_controlStyle & (~(ES_LEFT | ES_CENTER | ES_RIGHT)));
		switch (m_textAlignment)
		{
			case WinApiFramework::Edit::Left:	m_controlStyle |= ES_LEFT;		break;
			case WinApiFramework::Edit::Center:	m_controlStyle |= ES_CENTER;	break;
			case WinApiFramework::Edit::Right:	m_controlStyle |= ES_RIGHT;		break;
		}
		SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);
		InvalidateRect(m_hWindow, NULL, TRUE);

		m_events.PushEvent(Event(Event::Type::TextAlignmentSet));
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

		m_events.PushEvent(Event(Event::Type::LettersModeSet));
	}
	void Edit::SetPasswordMode(bool newPasswordMode)
	{
		m_passwordMode = newPasswordMode;

		m_controlStyle = (m_controlStyle & (~ES_PASSWORD));
		if (m_passwordMode) m_controlStyle |= ES_PASSWORD;

		SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);
		if (m_passwordMode) SendMessage(m_hWindow, EM_SETPASSWORDCHAR, m_passwordChar, 0);
		else				SendMessage(m_hWindow, EM_SETPASSWORDCHAR, 0, 0);

		InvalidateRect(m_hWindow, NULL, TRUE);

		m_events.PushEvent(Event(Event::Type::PasswordModeSet));

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
	void Edit::SetReadOnlyMode(bool readOnlyMode)
	{
		m_readOnly = readOnlyMode;

		if (m_readOnly)	m_controlStyle |= ES_READONLY;
		else			m_controlStyle = (m_controlStyle & (~ES_READONLY));

		SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);
		SendMessage(m_hWindow, EM_SETREADONLY, m_readOnly, 0);

		m_events.PushEvent(Event(Event::Type::ReadOnlyModeSet));
	}
	/*void Edit::SetMultilineMode(bool multilineMode)
	{
		if (m_multiline != multilineMode)
		{
			m_multiline = multilineMode;

			if (m_multiline)	m_controlStyle |= ES_MULTILINE;
			else				m_controlStyle = (m_controlStyle & (~ES_MULTILINE));

			SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

			m_events.PushEvent(Event(Event::Type::MultilineModeSet));
		}
	}*/
	/*void Edit::SetWordWrapMode(bool wordWrapMode)
	{
		if (m_wordWrap != wordWrapMode)
		{
			m_wordWrap = wordWrapMode;

			if (!m_wordWrap)	m_controlStyle |= ES_AUTOHSCROLL;
			else				m_controlStyle = (m_controlStyle & (~ES_AUTOHSCROLL));

			SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);


			m_events.PushEvent(Event(Event::Type::WordWrapModeSet));
		}
	}*/
	void Edit::SetTextLengthLimit(unsigned int lengthLimit)
	{
		m_textLengthLimit = lengthLimit;
		SendMessage(m_hWindow, EM_LIMITTEXT, m_textLengthLimit, 0u);

		m_events.PushEvent(Event(Event::Type::TextLengthLimitSet));
	}
	void Edit::SetPasswordChar(wchar_t passwordChar)
	{
		if (passwordChar == 0) return;
		m_passwordChar = passwordChar;

		if (m_passwordMode)
		{
			SendMessage(m_hWindow, EM_SETPASSWORDCHAR, m_passwordChar, 0);
			InvalidateRect(m_hWindow, NULL, TRUE);
		}

		m_events.PushEvent(Event(Event::Type::PasswordCharSet));
	}

	const std::wstring& Edit::GetText()
	{
		wchar_t* buffer = new wchar_t[m_textLengthLimit];
		GetWindowText(m_hWindow, buffer, m_textLengthLimit);
		m_text = buffer;
		delete[] buffer;
		return m_text;
	}
	const std::wstring& Edit::GetCueText() const
	{
		return m_cueText;
	}
	Edit::TextAlignment Edit::GetTextAlignment() const
	{
		return m_textAlignment;
	}
	Edit::LettersMode Edit::GetLettersMode() const
	{
		return m_lettersMode;
	}
	bool Edit::GetPasswordMode() const
	{
		return m_passwordMode;
	}
	bool Edit::GetNumberOnlyMode() const
	{
		return m_numberOnly;
	}
	bool Edit::GetReadOnlyMode() const
	{
		return m_readOnly;
	}
	bool Edit::GetMultilineMode() const
	{
		return m_multiline;
	}
	bool Edit::GetWordWrapMode() const
	{
		return m_wordWrap;
	}
	unsigned int Edit::GetTextLengthLimit() const
	{
		return m_textLengthLimit;
	}
	wchar_t Edit::GetPasswordChar() const
	{
		return m_passwordChar;
	}

	void Edit::SetSelection(int startIndex, int endIndex)
	{
		startIndex = std::max(startIndex, 0);
		endIndex = std::max(endIndex, 0);

		SendMessage(m_hWindow, EM_SETSEL, startIndex, endIndex);

		m_events.PushEvent(Event(Event::Type::SelectionSet));
	}
	void Edit::SetSelection(Range selectionRange)
	{
		SetSelection(selectionRange.min, selectionRange.max);
	}
	void Edit::SelectAll()
	{
		SendMessage(m_hWindow, EM_SETSEL, 0, -1);
		m_events.PushEvent(Event(Event::Type::AllSelected));
	}
	void Edit::RemoveSelection()
	{
		SendMessage(m_hWindow, EM_SETSEL, -1, -1);
		m_events.PushEvent(Event(Event::Type::SelectionRemoved));
	}
	void Edit::ReplaceSelection(const std::wstring& text)
	{
		SendMessage(m_hWindow, EM_REPLACESEL, TRUE, (LPARAM)text.c_str());
		m_events.PushEvent(Event(Event::Type::SelectionReplaced));
	}

	int Edit::GetLineCount() const
	{
		return SendMessage(m_hWindow, EM_GETLINECOUNT, 0, 0);
	}
	int Edit::GetLineLength(int lineIndex) const
	{
		return SendMessage(m_hWindow, EM_LINELENGTH, GetCharIndexFromLine(lineIndex), 0);
	}
	int Edit::GetCharIndexFromPosition(int x, int y) const
	{
		return LOWORD(SendMessage(m_hWindow, EM_CHARFROMPOS, 0, MAKELPARAM(x, y)));
	}
	int Edit::GetCharIndexFromPosition(const Point& position) const
	{
		return GetCharIndexFromPosition(position.x, position.y);
	}
	int Edit::GetLineIndexFromPosition(int x, int y) const
	{
		return HIWORD(SendMessage(m_hWindow, EM_CHARFROMPOS, 0, MAKELPARAM(x, y)));
	}
	int Edit::GetLineIndexFromPosition(const Point& position) const
	{
		return GetLineIndexFromPosition(position.x, position.y);
	}
	Rect Edit::GetEditableRect() const
	{
		RECT r;
		SendMessage(m_hWindow, EM_GETRECT, 0, (LPARAM)&r);
		return Rect(BoundRect(r.left, r.top, r.right, r.bottom));
	}

	bool Edit::IsMouseInEditableRect() const
	{
		Point mouse = GetMousePosition();
		BoundRect rect = GetEditableRect();
		return (mouse.x >= rect.left && mouse.y >= rect.top && mouse.x < rect.right && mouse.y < rect.bottom);
	}
	int Edit::GetCharIndexFromLine(int lineIndex) const
	{
		return SendMessage(m_hWindow, EM_LINEINDEX, lineIndex, 0);
	}
	int Edit::GetLineIndexFromChar(int charIndex) const
	{
		return SendMessage(m_hWindow, EM_LINEFROMCHAR, charIndex, 0);
	}
	// [CLASS] Edit --------------------------------|
}