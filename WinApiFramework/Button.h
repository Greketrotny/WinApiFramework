#ifndef BUTTON_H
#define BUTTON_H

#include "WindowControl.h"
#include <string>

namespace WinApiFramework
{
	class Button : public WindowControl
	{
		// -- fields -- //
	private:
		std::wstring caption;
	public:
		struct Config : public WindowControl::Config
		{
			std::wstring caption = L"Default";
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
				Unfocus
			};
			Type type;

			Event()
			{
				this->type = Invalid;
			}
			Event(Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler<Button::Event>
		{
			virtual void HandleEvent(Button::Event event) override
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case Button::Event::Type::Click:		Click();		break;
				case Button::Event::Type::DoubleClick:	DoubleClick();	break;
				case Button::Event::Type::Focus:		Focus();		break;
				case Button::Event::Type::Unfocus:		Unfocus();		break;
				}
			}
			virtual void Click() {};
			virtual void DoubleClick() {};
			virtual void Focus() {};
			virtual void Unfocus() {};
		};
	private:
		WindowControl::Events<Button::Event> events;


		// -- constructors -- //
	public:
		Button(const Button &otherButton) = delete;
		Button(const Button &&otherButton) = delete;
		Button(const Config &config);
		Button(const Config &config, EventHandler *eventHandler);
		~Button();


		// -- operators -- //
		Button& operator=(const Button &otherButton) = delete;
		Button& operator=(const Button &&otherButton) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			events.PushEvent(Button::Event((Button::Event::Type)event.type));
		}
	public:
		void SetCaption(std::wstring newCaption);


		// -- property fields -- //
		const Rect& Rect;
		const std::wstring& Caption;
		WindowControl::Events<Button::Event>& Events;
	};
}
#endif // !BUTTON_H_H
