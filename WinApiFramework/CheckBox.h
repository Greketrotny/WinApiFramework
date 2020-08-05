#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "base_window.h"

namespace WinapiFramework
{
	class CheckBox;
	template <> struct ConStruct<CheckBox>;

	class CheckBox : public BaseWindow
	{
	public:
		enum class BoxState
		{
			Uncheck,
			Check,
			Middle
		};
	private:
		std::wstring m_caption;
		bool m_isTripleState;
		BoxState m_boxState;

	public:
		struct Events : public BaseWindowEvents
		{
			struct EventSetCaption : public BaseEvent {};
			struct EventSetState : public BaseEvent 
			{
			public:
				BoxState new_state;

				EventSetState(BoxState state)
					: new_state(state)
				{}
			};
			struct EventEnableTripleState : public BaseEvent {};
			struct EventDisableTripleState : public BaseEvent {};
		};


	private:
		CheckBox(const CheckBox& checkBox) = delete;
		CheckBox(const CheckBox&& checkBox) = delete;
		CheckBox(ParentWindow* parent, const ConStruct<CheckBox>& conStruct);
		~CheckBox();


	public:
		CheckBox& operator=(const CheckBox& checkBox) = delete;
		CheckBox& operator=(const CheckBox&& checkBox) = delete;


	private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		void SetCaption(const std::wstring& newCaption);
		void SetState(BoxState newState);
		void EnableTripleState();
		void DisableTripleState();

		const std::wstring& GetCaption();
		BoxState GetState();
		bool TripleStateEnabled();


		friend class ObjectCreator;
	};

	template <> struct ConStruct<CheckBox>
	{
		Rect rect;
		std::wstring caption;
		bool isTripleState;
		CheckBox::BoxState boxState;

		ConStruct(const Rect& rect,
				  const std::wstring& caption = L"caption",
				  bool isTripleState = false,
				  CheckBox::BoxState boxState = CheckBox::BoxState::Uncheck)
			: rect(rect)
			, caption(caption)
			, isTripleState(isTripleState)
			, boxState(boxState)
		{}
	};
}

#endif // !CHECKBOX_H