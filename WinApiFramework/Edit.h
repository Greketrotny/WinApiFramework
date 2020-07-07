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
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4,
				TextChanged,
				TextLimitReached,
				TextAlignmentSet,
				LettersModeSet,
				PasswordModeSet,
				NumberModeSet,
				FocusSet,
				FocusKilled,
				HorizontalScroll
			};
			Type type;

			Event(Edit::Event::Type type = Edit::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		std::wstring m_text = L"";
		TextAlignment m_textAlignment = TextAlignment::Left;
		LettersMode m_lettersMode = LettersMode::All;
		bool m_passwordMode = false;
		bool m_numberOnly = false;
		unsigned int m_textLengthLimit = 0xFFFF;

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
		int ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(Edit::Event((Edit::Event::Type)event.type));
		}
	public:
		void SetText(std::wstring newText);
		void SetTextAlignment(Edit::TextAlignment newAlignment);
		void SetLettersMode(Edit::LettersMode newLettersMode);
		void SetPasswordMode(bool passwordMode);
		void SetNumberOnlyMode(bool numberOnlyMode);
		void SetTextLengthLimit(unsigned int newLimit);
		std::wstring GetText();

		// -- property fields -- //
	public:
		const bool& PasswordMode;
		const bool& NumberOnlyMode;
		const TextAlignment& Alignment;
		const LettersMode& LetterMode;
		ChildControl::EventsManager<Edit::Event>& Events;

		// -- friends -- //
	public:
		friend class ControlCreator;
	};

	template <> struct ConStruct<Edit> : ConStruct<ChildControl>
	{
		std::wstring text = L"";
		Edit::TextAlignment textAlignment = Edit::TextAlignment::Left;
		Edit::LettersMode lettersMode = Edit::LettersMode::All;
		bool passwordMode = false;
		bool numberOnly = false;
		unsigned int textLengthLimit = 0xFFFFFFFF;

		ConStruct(ConStruct<ChildControl> windowControlConStruct = ConStruct<ChildControl>(),
				  const std::wstring& text = L"",
				  Edit::TextAlignment textAlignment = Edit::TextAlignment::Left,
				  Edit::LettersMode lettersMode = Edit::LettersMode::All,
				  bool passwordMode = false,
				  bool numberOnly = false,
				  unsigned int textLengthLimit = 0xFFFFFFFF)
			: ConStruct<ChildControl>::ConStruct(windowControlConStruct)
			, text(text)
			, textAlignment(textAlignment)
			, lettersMode(lettersMode)
			, passwordMode(passwordMode)
			, numberOnly(numberOnly)
			, textLengthLimit(textLengthLimit)
		{}
	};
}

#endif // !EDIT_H