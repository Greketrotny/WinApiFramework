#ifndef CHECK_BOX_H
#define CHECK_BOX_H

#include "WindowControl.h"
#include "BaseControl.h"

namespace WinApiFramework
{
	class CheckBox;
	template <> struct ConStruct<CheckBox>;

	class CheckBox : public WindowControl, public ChildControl
	{
		// -- fields -- //
	public:
		enum BoxState
		{
			UnCheck,
			Check,
			MiddleState
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

			Event(Type type = Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		std::wstring caption;
		bool isTripleState;
		BoxState boxState;
		WindowControl::EventsManager<CheckBox::Event> events;


		// -- constructors -- //
	public:
		CheckBox(const CheckBox& checkBox) = delete;
		CheckBox(const CheckBox&& checkBox) = delete;
		CheckBox(const ConStruct<CheckBox>& conStruct);
		~CheckBox();


		// -- operators -- //
	public:
		CheckBox& operator=(const CheckBox& checkBox) = delete;
		CheckBox& operator=(const CheckBox&& checkBox) = delete;


		// -- methods -- //
	private:
		int ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
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
		WindowControl::EventsManager<CheckBox::Event>& Events;
	};

	template <> struct ConStruct<CheckBox> : ConStruct<WindowControl>
	{
		std::wstring caption;
		bool isTripleState;
		CheckBox::BoxState boxState;

		ConStruct(ConStruct<WindowControl> windowControlConStruct = ConStruct<WindowControl>(),
				  const std::wstring& caption = L"caption",
				  bool isTripleState = false,
				  CheckBox::BoxState boxState = CheckBox::UnCheck)
			: ConStruct<WindowControl>::ConStruct(windowControlConStruct)
			, caption(caption)
			, isTripleState(isTripleState)
			, boxState(boxState)
		{}
	};
}

#endif // !CHECK_BOX_H