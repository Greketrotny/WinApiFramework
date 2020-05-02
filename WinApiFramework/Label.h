#ifndef LABEL_H
#define LABEL_H

#include "WindowControl.h"
#include "BaseControl.h"

namespace WinApiFramework
{
	class Label;
	template <> struct ConStruct<Label>;

	class Label : public WindowControl, public ChildControl
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
		Label(const ConStruct<Label> &conStruct);
		~Label();


		// -- operators -- //
	public:
		Label operator=(const Label& label) = delete;
		Label operator=(const Label&& label) = delete;

		// -- methods -- //
	private:
		int ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
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

	template <> struct ConStruct<Label> : ConStruct<WindowControl>
	{
		std::wstring caption;
		Label::TextAlignment textAlignment;

		ConStruct(ConStruct<WindowControl> windowControlConStruct = ConStruct<WindowControl>(),
				  const std::wstring& caption = L"caption",
				  Label::TextAlignment textAlignment = Label::TextAlignment::Left)
			: ConStruct<WindowControl>::ConStruct(windowControlConStruct)
			, caption(caption)
			, textAlignment(textAlignment)
		{}
	};
}

#endif // !LABEL_H