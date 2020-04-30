#ifndef BUTTON_H
#define BUTTON_H

#include "BaseControl.h"
#include "WindowControl.h"

namespace WinApiFramework
{
	class Button : public WindowControl, public ChildControl
	{
		// -- fields -- //
	private:
		std::wstring m_caption;
	public:
		struct ConStruct : public WindowControl::ConStruct
		{
			std::wstring caption;

			ConStruct(WindowControl::ConStruct conStruct = WindowControl::ConStruct(),
					  std::wstring caption = L"text")
				: WindowControl::ConStruct(conStruct)
				, caption(caption)
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
	public:
		Button(const Button &otherButton) = delete;
		Button(const Button &&otherButton) = delete;
		Button(ParentControl* parentcontrol, const ConStruct &conStruct);
		~Button();


		// -- operators -- //
		Button& operator=(const Button &otherButton) = delete;
		Button& operator=(const Button &&otherButton) = delete;


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
		void SetCaption(std::wstring newCaption);


		// -- property fields -- //
		const std::wstring& Caption;
		WindowControl::EventsManager<Button::Event>& Events;
	};
}
#endif // !BUTTON_H_H
