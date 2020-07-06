#ifndef LABEL_H
#define LABEL_H

#include "BaseControl.h"

namespace WinApiFramework
{
	class Label;
	template <> struct ConStruct<Label>;

	class Label : public ChildControl
	{
		// -- fields -- //
	private:
		std::wstring m_caption;
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
				CaptionChanged,
				TextAlignmentChanged,
				Clicked,
				DoubleClicked
			};
			Type type;

			Event(Label::Event::Type type = Label::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		ChildControl::EventsManager<Label::Event> m_events;
		TextAlignment m_textAlignment;


		// -- constructors -- //
	private:
		Label(const Label& label) = delete;
		Label(const Label&& label) = delete;
		Label(ParentControl* parentControl, const ConStruct<Label> &conStruct);
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
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(Label::Event((Label::Event::Type)event.type));
		}
	public:
		void SetCaption(std::wstring newCaption);
		void SetTextAligment(TextAlignment textAlignment);


		// -- property fields -- //
	public:
		const std::wstring& Caption;
		const TextAlignment& Alignment;
		ChildControl::EventsManager<Label::Event>& Events;

		// -- friends -- //
	public:
		friend class ParentControl;
	};

	template <> struct ConStruct<Label> : ConStruct<ChildControl>
	{
		std::wstring caption;
		Label::TextAlignment textAlignment;

		ConStruct(ConStruct<ChildControl> windowControlConStruct = ConStruct<ChildControl>(),
				  const std::wstring& caption = L"caption",
				  Label::TextAlignment textAlignment = Label::TextAlignment::Left)
			: ConStruct<ChildControl>::ConStruct(windowControlConStruct)
			, caption(caption)
			, textAlignment(textAlignment)
		{}
	};
}

#endif // !LABEL_H