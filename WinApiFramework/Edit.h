#ifndef EDIT_H
#define EDIT_H

#include "WindowControl.h"
#include "BaseControl.h"

namespace WinApiFramework
{
	class Edit;
	template <> struct ConStruct<Edit>;

	class Edit : public WindowControl, public ChildControl
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
			Either
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
				Text,
				TextAlignment,
				LettersMode,
				PasswordMode,
				NumberMode,
				TextLimitReach
			};
			Type type;

			Event(Edit::Event::Type type = Edit::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		std::wstring text = L"";
		TextAlignment textAlignment = TextAlignment::Left;
		LettersMode lettersMode = LettersMode::Either;
		bool passwordMode = false;
		bool numberOnly = false;
		unsigned int textLengthLimit = 0xFFFF;

		WindowControl::EventsManager<Edit::Event> events;


		// -- contstructors -- //
	public:
		Edit(const Edit& edit) = delete;
		Edit(const Edit&& edit) = delete;
		Edit(const ConStruct<Edit>& config);
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
		void PushBaseEvent(WindowControl::Event event) override
		{
			events.PushEvent(Edit::Event((Edit::Event::Type)event.type));
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
		WindowControl::EventsManager<Edit::Event>& Events;
	};

	template <> struct ConStruct<Edit> : ConStruct<WindowControl>
	{
		std::wstring text = L"";
		Edit::TextAlignment textAlignment = Edit::TextAlignment::Left;
		Edit::LettersMode lettersMode = Edit::LettersMode::Either;
		bool passwordMode = false;
		bool numberOnly = false;
		unsigned int textLengthLimit = 0xFFFF;

		ConStruct(ConStruct<WindowControl> windowControlConStruct = ConStruct<WindowControl>(),
				  const std::wstring& text = L"",
				  Edit::TextAlignment textAlignment = Edit::TextAlignment::Left,
				  Edit::LettersMode lettersMode = Edit::LettersMode::Either,
				  bool passwordMode = false,
				  bool numberOnly = false,
				  unsigned int textLengthLimit = 0xFFFF)
			: ConStruct<WindowControl>::ConStruct(windowControlConStruct)
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