#ifndef GROUPBOX_H
#define GROUPBOX_H

#include "base_window.h"

namespace WinapiFramework
{
	// ConStruct specialization for GroupBox
	class GroupBox;
	template <> struct ConStruct<GroupBox>;

	// ~~~~~~~~ [CLASS] GroupBox ~~~~~~~~
	class GroupBox : public BaseWindow
	{
	public:
		enum class CaptionPosition
		{
			Left,
			Center,
			Right
		};
	private:
		std::wstring m_caption;
		CaptionPosition m_caption_position;
	public:
		struct Events : public BaseWindowEvents
		{
			struct EventSetCaption : public BaseEvent {};
			struct EventSetCaptionPosition : public BaseEvent {};
		};


	private:
		GroupBox(const GroupBox& groupBox) = delete;
		GroupBox(GroupBox&& groupBox) = delete;
		GroupBox(ParentWindow* parent, const ConStruct<GroupBox>& conStruct);
		~GroupBox();


	private:
		GroupBox& operator=(const GroupBox &groupBox) = delete;
		GroupBox& operator=(GroupBox &&groupBox) = delete;


	private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		void SetCaption(const std::wstring& caption);
		void SetCaptionPosition(CaptionPosition captionPosition);

		const std::wstring& GetCaption();
		CaptionPosition GetCaptionPosition();
		

		friend class ObjectCreator;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	template <> struct ConStruct<GroupBox>
	{
		Rect rect;
		std::wstring caption;
		GroupBox::CaptionPosition caption_position;

		ConStruct(
			const Rect& rect = Rect(50, 50, 100, 50),
			const std::wstring& caption = L"caption",
			GroupBox::CaptionPosition caption_position = GroupBox::CaptionPosition::Left)
			: rect(rect)
			, caption(caption)
			, caption_position(caption_position)
		{}
	};
}

#endif // !GROUPBOX_H