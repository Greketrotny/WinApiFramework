#ifndef BUTTON_H
#define BUTTON_H

#include "WindowControl.h"

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
				Unfocus,
				CaptionChanged
			};
			Type type;

			Event(Type type = Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		WindowControl::EventsManager<Button::Event> events;


		// -- constructors -- //
	public:
		Button(const Button &otherButton) = delete;
		Button(const Button &&otherButton) = delete;
		Button(const Config &config);
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
		WindowControl::EventsManager<Button::Event>& Events;
	};
}
#endif // !BUTTON_H_H
