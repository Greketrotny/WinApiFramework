#ifndef LABEL_H
#define LABEL_H

#include "WindowControl.h"

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
		struct ConStruct : public WindowControl::ConStruct
		{
			std::wstring caption;
			TextAlignment textAlignment;

			ConStruct(WindowControl::ConStruct windowControlConStruct = WindowControl::ConStruct(),
					  const std::wstring& caption = L"caption",
					  TextAlignment textAlignment = TextAlignment::Left)
				: WindowControl::ConStruct(windowControlConStruct)
				, caption(caption)
				, textAlignment(textAlignment)
			{
			}
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

			Event(Label::Event::Type type = Label::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		WindowControl::EventsManager<Label::Event> events;
		TextAlignment textAlignment;


		// -- constructors -- //
	public:
		Label(const Label& label) = delete;
		Label(const Label&& label) = delete;
		Label(const ConStruct &conStruct);
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
		WindowControl::EventsManager<Label::Event>& Events;
	};
}

#endif // !LABEL_H