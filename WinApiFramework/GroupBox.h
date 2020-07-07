#ifndef GROUP_BOX_H
#define GROUP_BOX_H

#include "BaseControl.h"

namespace WinApiFramework
{
	// ConStruct specialization for GroupBox
	class GroupBox;
	template <> struct ConStruct<GroupBox>;

	// ~~~~~~~~ [CLASS] GroupBox ~~~~~~~~
	class GroupBox : public ChildControl
	{
		// ~~ Button::fields ~~ //
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
				Disable = 4
			};
			Type type;
			GroupBox* button;

			Event(Type type = Type::Invalid)
			{
				this->type = type;
				this->button = button;
			}
		};

	private:
		ChildControl::EventsManager<GroupBox::Event> m_events;

		// ~~ GroupBox::constructors ~~ //
	private:
		GroupBox(const GroupBox& groupBox) = delete;
		GroupBox(GroupBox&& groupBox) = delete;
		GroupBox(ParentControl* parentControl, const ConStruct<GroupBox>& conStruct);
		~GroupBox();


		// ~~ GroupBox::operators ~~ //
	private:
		GroupBox& operator=(const GroupBox &groupBox) = delete;
		GroupBox& operator=(GroupBox &&groupBox) = delete;


		// ~~ GroupBox::methods -~~ //
	private:
		int ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(GroupBox::Event((GroupBox::Event::Type)event.type));
		}
		

		// ~~ GroupBox::friends ~~ //
	public:
		friend class ControlCreator;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	template <> struct ConStruct<GroupBox> : ConStruct<ChildControl>
	{
		std::wstring caption;

		ConStruct(
			ConStruct<ChildControl> conStruct = ConStruct<ChildControl>(),
			std::wstring caption = L"caption")
			: ConStruct<ChildControl>::ConStruct(conStruct)
			, caption(caption)
		{}
	};
}

#endif // !GROUP_BOX_H