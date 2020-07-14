#ifndef CHECK_BOX_H
#define CHECK_BOX_H


#include "BaseControl.h"

namespace WinApiFramework
{
	class CheckBox;
	template <> struct ConStruct<CheckBox>;

	class CheckBox : public ChildControl
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
		std::wstring m_caption;
		bool m_isTripleState;
		BoxState m_boxState;
		ChildControl::EventsManager<CheckBox::Event> m_events;


		// -- constructors -- //
	private:
		CheckBox(const CheckBox& checkBox) = delete;
		CheckBox(const CheckBox&& checkBox) = delete;
		CheckBox(ParentControl* parentControl, const ConStruct<CheckBox>& conStruct);
		~CheckBox();


		// -- operators -- //
	public:
		CheckBox& operator=(const CheckBox& checkBox) = delete;
		CheckBox& operator=(const CheckBox&& checkBox) = delete;


		// -- methods -- //
	private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(CheckBox::Event((CheckBox::Event::Type)event.type));
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
		ChildControl::EventsManager<CheckBox::Event>& Events;

		// -- friends -- //
	public:
		friend class ControlCreator;
	};

	template <> struct ConStruct<CheckBox> : ConStruct<ChildControl>
	{
		std::wstring caption;
		bool isTripleState;
		CheckBox::BoxState boxState;

		ConStruct(ConStruct<ChildControl> windowControlConStruct = ConStruct<ChildControl>(),
				  const std::wstring& caption = L"caption",
				  bool isTripleState = false,
				  CheckBox::BoxState boxState = CheckBox::UnCheck)
			: ConStruct<ChildControl>::ConStruct(windowControlConStruct)
			, caption(caption)
			, isTripleState(isTripleState)
			, boxState(boxState)
		{}
	};
}

#endif // !CHECK_BOX_H