#ifndef BUTTON_H
#define BUTTON_H

#include "BaseControl.h"
#include "WindowControl.h"

namespace WinApiFramework
{
	// ConStruct specialization for Button
	class Button;
	template <> struct ConStruct<Button>;

	// ~~~~~~~~ [CLASS] Button ~~~~~~~~
	class Button : public WindowControl, public ChildControl
	{
		// ~~ Button::fields ~~
	private:
		std::wstring m_caption;
	public:
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
				Hilite,
				Push,
				Unpush
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
		WindowControl::EventsManager<Button::Event> m_events;


		// -- constructors -- //
	private:
		Button(const Button &otherButton) = delete;
		Button(const Button &&otherButton) = delete;
		Button(ParentControl* parentcontrol, const ConStruct<Button>& conStruct);
		~Button();


		// -- operators -- //
	private:
		Button& operator=(const Button &otherButton) = delete;
		Button& operator=(const Button &&otherButton) = delete;


		// -- static methods -- //
	public:
		static Button* Create(ParentControl* parentControl, const ConStruct<Button>& conStruct);
		static void Destroy(Button* button);

		// -- methods -- //
	private:
		int ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			m_events.PushEvent(Button::Event((Button::Event::Type)event.type, this));
		}
	public:
		void Destroy();
		void SetCaption(std::wstring newCaption);


		// -- property fields -- //
		const std::wstring& Caption;
		WindowControl::EventsManager<Button::Event>& Events;

		// -- friends -- //
	public:
		friend class ParentControl;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	template <> struct ConStruct<Button> : ConStruct<WindowControl>
	{
		std::wstring caption;

		ConStruct(ConStruct<WindowControl> conStruct = ConStruct<WindowControl>(),
				  std::wstring caption = L"text")
			: ConStruct<WindowControl>::ConStruct(conStruct)
			, caption(caption)
		{}
	};
}
#endif // !BUTTON_H_H
