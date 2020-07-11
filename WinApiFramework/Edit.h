#ifndef EDIT_H
#define EDIT_H

#include "BaseControl.h"

namespace WinApiFramework
{
	class Edit;
	template <> struct ConStruct<Edit>;

	class Edit : public ChildControl
	{
		// -- fields -- //
	public:
		enum TextAlignment
		{
			Left,
			Center,
			Right
		};
		enum LettersMode
		{
			UpperCase,
			LowerCase,
			All
		};
		enum ScrollingStyle
		{
			NoScrolling,
			Horizontal,
			Vertical,
			HorizontalVertical
		};
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4,
				FocusSet,
				FocusKilled,
				TextChanged,
				CueTextChanged,
				TextLimitReached,
				PasswordCharSet,
				TextAlignmentSet,
				LettersModeSet,
				PasswordModeSet,
				NumberModeSet,
				ReadOnlyModeSet,
				//MultilineModeSet,
				//WordWrapModeSet,
				TextLengthLimitSet,
				SelectionSet,
				AllSelected,
				SelectionRemoved,
				SelectionReplaced,
				HorizontalScroll,
				VerticalScroll
			};
			Type type;

			Event(Edit::Event::Type type = Edit::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		std::wstring m_text;
		std::wstring m_cueText;
		TextAlignment m_textAlignment;
		LettersMode m_lettersMode;
		bool m_passwordMode;
		bool m_numberOnly;
		bool m_readOnly;
		bool m_multiline;
		bool m_wordWrap;
		unsigned int m_textLengthLimit;
		wchar_t m_passwordChar;
		ScrollingStyle m_scrollingStyle;

		ChildControl::EventsManager<Edit::Event> m_events;


		// -- contstructors -- //
	private:
		Edit(const Edit& edit) = delete;
		Edit(const Edit&& edit) = delete;
		Edit(ParentControl* parentControl, const ConStruct<Edit>& config);
		~Edit();


		// -- operators -- //
	public:
		Edit operator=(const Edit& edit) = delete;
		Edit operator=(const Edit&& edit) = delete;


		// -- methods -- //
	private:
		ProcedureResult ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(Edit::Event((Edit::Event::Type)event.type));
		}
	public:
		void SetText(const std::wstring& newText);
		void SetCueText(const std::wstring& cueText);
		void SetTextAlignment(Edit::TextAlignment newAlignment);
		void SetLettersMode(Edit::LettersMode newLettersMode);
		void SetPasswordMode(bool passwordMode);
		void SetNumberOnlyMode(bool numberOnlyMode);
		void SetReadOnlyMode(bool readOnlyMode);
		// void SetMultilineMode(bool multilineMode);	
		// void SetWordWrapMode(bool wordWrapMode);
		void SetTextLengthLimit(unsigned int newLimit);
		void SetPasswordChar(wchar_t passwordChar);

		const std::wstring& GetText();
		const std::wstring& GetCueText() const;
		TextAlignment GetTextAlignment() const;
		LettersMode GetLettersMode() const;
		bool GetPasswordMode() const;
		bool GetNumberOnlyMode() const;
		bool GetReadOnlyMode() const;
		bool GetMultilineMode() const;
		bool GetWordWrapMode() const;
		unsigned int GetTextLengthLimit() const;
		wchar_t GetPasswordChar() const;

		void SetSelection(int startIndex, int endIndex);
		void SetSelection(Range selectionRange);
		void SelectAll();
		void RemoveSelection();
		void ReplaceSelection(const std::wstring& text);

		int GetLineCount() const;
		int GetLineLength(int lineIndex) const;
		int GetCharIndexFromPosition(int x, int y) const;
		int GetCharIndexFromPosition(const Point& position) const;
		int GetLineIndexFromPosition(int x, int y) const;
		int GetLineIndexFromPosition(const Point& position) const;
		Rect GetEditableRect() const;

		bool IsMouseInEditableRect() const;
	private:
		int GetCharIndexFromLine(int lineIndex) const;
		int GetLineIndexFromChar(int charIndex) const;



	// -- property fields -- //
	public:
		ChildControl::EventsManager<Edit::Event>& Events;

		// -- friends -- //
	public:
		friend class ControlCreator;
	};

	template <> struct ConStruct<Edit> : ConStruct<ChildControl>
	{
		std::wstring text = L"";
		std::wstring cueText = L"";
		Edit::TextAlignment textAlignment = Edit::TextAlignment::Left;
		Edit::LettersMode lettersMode = Edit::LettersMode::All;
		bool passwordMode = false;
		bool numberOnly = false;
		bool readOnly = false;
		bool multiline = false;
		bool wordWrap = false;
		unsigned int textLengthLimit = 0xFFFFFFFF;
		wchar_t passwordChar = L'\x25CF';
		Edit::ScrollingStyle scrollingStyle;

		ConStruct(ConStruct<ChildControl> windowControlConStruct = ConStruct<ChildControl>(),
			const std::wstring& text = L"",
			const std::wstring& cueText = L"",
			Edit::TextAlignment textAlignment = Edit::TextAlignment::Left,
			Edit::LettersMode lettersMode = Edit::LettersMode::All,
			bool passwordMode = false,
			bool numberOnly = false,
			bool readOnly = false,
			bool multiline = false,
			bool wordWrap = false,
			unsigned int textLengthLimit = 0x7FFFFFFE,
			wchar_t passwordChar = L'\x25CF',
			Edit::ScrollingStyle scrollingStyle = Edit::ScrollingStyle::NoScrolling)
			: ConStruct<ChildControl>::ConStruct(windowControlConStruct)
			, text(text)
			, cueText(cueText)
			, textAlignment(textAlignment)
			, lettersMode(lettersMode)
			, passwordMode(passwordMode)
			, numberOnly(numberOnly)
			, readOnly(readOnly)
			, multiline(multiline)
			, wordWrap(wordWrap)
			, textLengthLimit(textLengthLimit)
			, passwordChar(passwordChar)
			, scrollingStyle(scrollingStyle)
		{}
	};
}

#endif // !EDIT_H