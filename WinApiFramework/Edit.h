#ifndef EDIT_H
#define EDIT_H

#include "base_window.h"

namespace WinapiFramework
{
	class Edit;
	template <> struct ConStruct<Edit>;

	class Edit : public BaseWindow
	{
	public:
		enum class TextAlignment
		{
			Left,
			Center,
			Right
		};
		enum class LettersMode
		{
			UpperCase,
			LowerCase,
			All
		};
		enum class ScrollingStyle
		{
			NoScrolling,
			Horizontal,
			Vertical,
			HorizontalVertical
		};
	private:
		mutable std::wstring m_text;
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

	public:
		struct Events : public BaseWindowEvents
		{
			struct EventSetFocus : public BaseEvent {};
			struct EventKillFocus : public BaseEvent {};
			struct EventSetText : public BaseEvent {};
			struct EventSetCueText : public BaseEvent {};
			struct EventReachTextLimit : public BaseEvent {};
			struct EventSetPasswordChar : public BaseEvent {};
			struct EventSetTextAlignment : public BaseEvent {};
			struct EventSetLetterMode : public BaseEvent {};
			struct EventSetPasswordMode : public BaseEvent {};
			struct EventSetNumberMode : public BaseEvent {};
			struct EventSetReadOnlyMode : public BaseEvent {};
			struct EventSetTextLengthLimit : public BaseEvent {};
			struct SetSelection : public BaseEvent {};
			struct EventSelectAll : public BaseEvent {};
			struct EventRemoveSelection : public BaseEvent {};
			struct EventReplaceSelection : public BaseEvent {};
			struct EventHScroll : public BaseEvent {};
			struct EventVScroll : public BaseEvent {};
		};

	private:
		Edit(const Edit& other) = delete;
		Edit(const Edit&& other) = delete;
		Edit(ParentWindow* parent, const ConStruct<Edit>& config);
		~Edit();


	public:
		Edit operator=(const Edit& other) = delete;
		Edit operator=(const Edit&& other) = delete;


		// -- methods -- //
	private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
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

		const std::wstring& GetText() const;
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


		friend class ObjectCreator;
	};

	template <> struct ConStruct<Edit>
	{
		Rect rect;
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

		ConStruct(const Rect& rect = Rect(50, 50, 100, 50),
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
			: rect(rect)
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