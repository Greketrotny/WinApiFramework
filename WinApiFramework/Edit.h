#ifndef EDIT_H
#define EDIT_H

#include "WindowControl.h"

namespace WinApiFramework
{
	class Edit : public WindowControl
	{
		// -- fields -- //
	private:
		std::wstring text = L"";
		bool passwordMode = false;
		bool numberOnly = false;
		unsigned int textLengthLimit = 0xFFFF;

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
		struct Config : public WindowControl::Config
		{
			TextAlignment textAlignment = TextAlignment::Left;
			LettersMode lettersMode = LettersMode::Either;
			std::wstring text = L"";
			bool passwordMode = false;
			bool numberOnly = false;
			unsigned int textLengthLimit = 0xFFFF;
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
		WindowControl::EventsManager<Edit::Event> events;
		TextAlignment textAlignment = TextAlignment::Left;
		LettersMode lettersMode = LettersMode::Either;


		// -- contstructors -- //
	public:
		Edit(const Edit& edit) = delete;
		Edit(const Edit&& edit) = delete;
		Edit(const Config& config);
		~Edit();


		// -- operators -- //
	public:
		Edit operator=(const Edit& edit) = delete;
		Edit operator=(const Edit&& edit) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
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
}

#endif // !EDIT_H