#ifndef BUTTON_H
#define BUTTON_H

#include "base_window.h"

namespace WinapiFramework
{
	// ConStruct specialization for Button
	class Button;
	template <> struct ConStruct<Button>;

	// ~~~~~~~~ [CLASS] Button ~~~~~~~~ //
	class Button : public ChildControl
	{
		// ~~ Button::fields ~~ //
	public:
		enum CaptionPosition
		{
			Center,
			TopLeft,
			TopCenter,
			TopRight,
			MiddleLeft,
			MiddleCenter,
			MiddleRight,
			BottomLeft,
			BottomCenter,
			BottomRight
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
				Click,
				DoubleClick,
				Focus,
				Unfocus,
				CaptionChanged,
				CaptionPositionChanged
			};
			Type type;
			Button* button;

			Event(Type type = Type::Invalid, Button* button = nullptr)
			{
				this->type = type;
				this->button = button;
			}
		};
	private:
		std::wstring m_caption;
		CaptionPosition m_caption_position;
		ChildControl::EventsManager<Button::Event> m_events;


		// -- constructors -- //
	private:
		Button(const Button &otherButton) = delete;
		Button(Button &&otherButton) = delete;
		Button(ParentControl* parentcontrol, const ConStruct<Button>& conStruct);
		~Button();


		// -- operators -- //
	private:
		Button& operator=(const Button &otherButton) = delete;
		Button& operator=(Button &&otherButton) = delete;


		// -- methods -- //
	private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(Button::Event((Button::Event::Type)event.type, this));
		}
	public:
		void SetCaption(std::wstring newCaption);
		void SetCaptionPosition(CaptionPosition captionPosition);
		CaptionPosition GetCaptionPosition();


		// -- property fields -- //
		const std::wstring& Caption;
		ChildControl::EventsManager<Button::Event>& Events;

		// -- friends -- //
	public:
		friend class ControlCreator;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	template <> struct ConStruct<Button> : ConStruct<ChildControl>
	{
		std::wstring caption;
		Button::CaptionPosition caption_position;

		ConStruct(ConStruct<ChildControl> conStruct = ConStruct<ChildControl>(),
				  std::wstring caption = L"text",
			Button::CaptionPosition caption_position = Button::CaptionPosition::Center)
			: ConStruct<ChildControl>::ConStruct(conStruct)
			, caption(caption)
			, caption_position(caption_position)
		{}
	};
}
#endif // !BUTTON_H_H
