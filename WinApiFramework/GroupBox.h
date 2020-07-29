#ifndef GROUPBOX_H
#define GROUPBOX_H

#include "base_window.h"

namespace WinapiFramework
{
	// ConStruct specialization for GroupBox
	class GroupBox;
	template <> struct ConStruct<GroupBox>;

	// ~~~~~~~~ [CLASS] GroupBox ~~~~~~~~
	class GroupBox 
		: public ChildControl
	{
		// ~~ Button::fields ~~ //
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
				CaptionChanged,
				CaptionPositionChanged
			};
			Type type;
			GroupBox* button;

			Event(Type type = Type::Invalid)
			{
				this->type = type;
				this->button = button;
			}
		};
		enum CaptionPosition
		{
			Left,
			Center,
			Right
		};
	private:
		std::wstring m_caption;
		CaptionPosition m_caption_position;
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
	public: // private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(GroupBox::Event((GroupBox::Event::Type)event.type));
		}
	public:
		void SetCaption(std::wstring caption);
		void SetCaptionPosition(CaptionPosition captionPosition);
		CaptionPosition GetCaptionPosition();
		

		// ~~ GroupBox::friends ~~ //
	public:
		friend class ControlCreator;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	template <> struct ConStruct<GroupBox> : ConStruct<ChildControl>
	{
		std::wstring caption;
		GroupBox::CaptionPosition caption_position;

		ConStruct(
			ConStruct<ChildControl> conStruct = ConStruct<ChildControl>(),
			std::wstring caption = L"caption",
			GroupBox::CaptionPosition caption_position = GroupBox::CaptionPosition::Left)
			: ConStruct<ChildControl>::ConStruct(conStruct)
			, caption(caption)
			, caption_position(caption_position)
		{}
	};
}

#endif // !GROUPBOX_H