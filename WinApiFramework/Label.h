#ifndef LABEL_H
#define LABEL_H

#include "WindowControl.h"
#include <string>

namespace WinApiFramework
{
	class Label : public WindowControl
	{
		// -- fields -- //
	private:
		std::wstring caption;
	public:
		enum TextAlignment
		{
			Left,
			Center,
			Right
		};
		struct Config : public WindowControl::Config
		{
			std::wstring caption = L"default";
			TextAlignment textAlignment = TextAlignment::Left;
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
				CaptionChange,
				TextAlignmentChange
			};
			Type type;

			Event()
			{
				type = Invalid;
			}
			Event(Label::Event::Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler<Label::Event>
		{
			virtual void HandleEvent(Label::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case Label::Event::Type::CaptionChange:			CaptionChange();		break;
				case Label::Event::Type::TextAlignmentChange:	TextAlignmentChange();	break;
				}
			}
			virtual void CaptionChange() {};
			virtual void TextAlignmentChange() {};
		};
	private:
		WindowControl::Events<Label::Event> events;
		TextAlignment textAlignment;


		// -- constructors -- //
	public:
		Label(const Label& label) = delete;
		Label(const Label&& label) = delete;
		Label(const Config &config);
		Label(const Config &config, EventHandler *eventHandler);
		~Label();


		// -- operators -- //
	public:
		Label operator=(const Label& label) = delete;
		Label operator=(const Label&& label) = delete;

		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			events.PushEvent(Label::Event((Label::Event::Type)event.type));
		}
	public:
		void SetCaption(std::wstring newCaption);
		void SetTextAligment(TextAlignment textAlignment);


		// -- property fields -- //
	public:
		const std::wstring& Caption;
		const TextAlignment& Alignment;
		WindowControl::Events<Label::Event>& Events;
	};
}

#endif // !LABEL_H