#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "WindowControl.h"

namespace WinApiFramework
{
	class CheckBox : public WindowControl
	{
		// -- fields -- //
	public:
		enum BoxState
		{
			UnCheck,
			Check,
			MiddleState
		};
		struct Config : public WindowControl::Config
		{
			std::wstring caption = L"Default";
			bool isTripleState = 0;
			BoxState boxState = UnCheck;
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
				Check,
				UnCheck,
				MiddleState
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
		struct EventHandler : public WindowControl::EventHandler<CheckBox::Event>
		{
			virtual void HandleEvent(CheckBox::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case CheckBox::Event::Type::Check:			Check();		break;
				case CheckBox::Event::Type::UnCheck:		UnCheck();		break;
				case CheckBox::Event::Type::MiddleState:	MiddleState();	break;
				}
			}
			virtual void Check() {};
			virtual void UnCheck() {};
			virtual void MiddleState() {};
		};
	private:
		std::wstring caption = L"Default";
		bool isTripleState = 0;
		BoxState boxState;
		WindowControl::Events<CheckBox::Event> events;


		// -- constructors -- //
	public:
		CheckBox(const CheckBox& checkBox) = delete;
		CheckBox(const CheckBox&& checkBox) = delete;
		CheckBox(const Config& config);
		CheckBox(const Config& config, CheckBox::EventHandler *eventHandler);
		~CheckBox();


		// -- operators -- //
	public:
		CheckBox& operator=(const CheckBox& checkBox) = delete;
		CheckBox& operator=(const CheckBox&& checkBox) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			events.PushEvent(CheckBox::Event((CheckBox::Event::Type)event.type));
		}
	public:
		void SetCaption(std::wstring newCaption);
		void SetBoxState(BoxState newState);
		void SetBoxState(unsigned int newState);
		void EnableTripleState();
		void DisableTripleState();


		// -- property fields -- //
		const BoxState& State;
		const std::wstring& Caption;
		const bool& IsTripleState;
	};
}

#endif // !CHECK_BOX_H