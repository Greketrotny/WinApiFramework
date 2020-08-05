#ifndef LABEL_H
#define LABEL_H

#include "base_window.h"

namespace WinapiFramework
{
	class Label;
	template <> struct ConStruct<Label>;

	class Label 
		: public BaseWindow
		, public HasSubclassProcedure<Label>
	{
	private:
		std::wstring m_caption;
	public:
		enum class TextAlignment
		{
			Left,
			Center,
			Right
		};
	private:
		TextAlignment m_textAlignment;

	public:
		struct Events : public BaseWindowEvents
		{
			struct EventSetCaption : public BaseEvent {};
			struct EventSetTextAlignment : public BaseEvent {};
		};


	private:
		Label(const Label& label) = delete;
		Label(const Label&& label) = delete;
		Label(ParentWindow* parent, const ConStruct<Label> &conStruct);
		~Label();


	public:
		Label operator=(const Label& label) = delete;
		Label operator=(const Label&& label) = delete;


	private:
		LRESULT SubclassProcedure(
			HWND hWnd,
			UINT msg, 
			WPARAM wParam, LPARAM lParam, 
			UINT_PTR uIDSubClass, DWORD_PTR dwRefData) override;
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;

		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		void SetCaption(const std::wstring& newCaption);
		void SetTextAligment(TextAlignment textAlignment);

		const std::wstring& GetCaption();
		TextAlignment GetTextAlignment();


	public:
		friend class ObjectCreator;
	};

	template <> struct ConStruct<Label>
	{
		Rect rect;
		std::wstring caption;
		Label::TextAlignment textAlignment;

		ConStruct(const Rect& rect,
			const std::wstring& caption = L"caption",
			Label::TextAlignment textAlignment = Label::TextAlignment::Left)
			: rect(rect)
			, caption(caption)
			, textAlignment(textAlignment)
		{}
	};
}

#endif // !LABEL_H