#ifndef BUTTON_H
#define BUTTON_H

#include "base_window.h"

namespace WinapiFramework
{
	// ConStruct specialization for Button
	class Button;
	template <> struct ConStruct<Button>;

	// ~~~~~~~~ [CLASS] Button ~~~~~~~~ //
	class Button : public BaseWindow
	{
	public:
		enum CaptionPosition
		{
			Center,
			TopLeft,
			TopCenter,
			TopRight,
			MiddleLeft,
			MiddleCenter,
			MiddleRight,
			BottomLeft,
			BottomCenter,
			BottomRight
		};
	private:
		std::wstring m_caption;
		CaptionPosition m_caption_position;

	public:
		struct Events : public BaseWindowEvents
		{
			struct EventClick : public BaseEvent {};
			struct EventDoubleClick : public BaseEvent {};
			struct EventFocus : public BaseEvent {};
			struct EventUnfocus : public BaseEvent {};
			struct EventSetCaption : public BaseEvent {};
			struct EventSetCaptionPosition : public BaseEvent {};
		};


	private:
		Button(const Button &other) = delete;
		Button(Button &&other) = delete;
		Button(ParentWindow* parent, const ConStruct<Button>& conStruct);
		~Button();


	private:
		Button& operator=(const Button &other) = delete;
		Button& operator=(Button &&other) = delete;


	private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		void SetCaption(std::wstring newCaption);
		void SetCaptionPosition(CaptionPosition captionPosition);

		const std::wstring& GetCaption() const;
		CaptionPosition GetCaptionPosition() const;


	public:
		friend class ObjectCreator;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	template <> struct ConStruct<Button>
	{
		Rect rect;
		std::wstring caption;
		Button::CaptionPosition caption_position;

		ConStruct(const Rect& rect = Rect(50, 50, 100, 50),
			std::wstring caption = L"text",
			Button::CaptionPosition caption_position = Button::CaptionPosition::Center)
			: rect(rect)
			, caption(caption)
			, caption_position(caption_position)
		{}
	};
}
#endif // !BUTTON_H_H
