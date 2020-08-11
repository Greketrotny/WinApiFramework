#ifndef TRACKBAR_H
#define TRACKBAR_H

#include "base_window.h"

namespace WinapiFramework
{
	class TrackBar;
	template <> struct ConStruct<TrackBar>;

	class TrackBar 
		: public BaseWindow
		, public HasSubclassProcedure<TrackBar>
	{
	public:
		enum class Orientation
		{
			Horizontal,
			Vertical
		};
		enum class TickStyle
		{
			Default,
			Top,
			Bottom,
			Both,
			NoTicks
		};
		enum class ToolTipsStyle
		{
			Top,
			Bottom,
			None
		};
	private:
		int m_ThumbPosition;
		Range m_trackRange;
		Range m_selectRange;
		Orientation m_orientation;
		TickStyle m_tickStyle;
		unsigned int m_ticksFrequency;
		ToolTipsStyle m_toolTipsStyle;
		unsigned int m_smallStep, m_largeStep;
		bool m_selectRangeEnabled;
	public:
		struct Events : public BaseWindowEvents
		{

			struct EventLineUp : public BaseEvent {};
			struct EventLineDown : public BaseEvent {};
			struct EventPageUp : public BaseEvent {};
			struct EventPageDown : public BaseEvent {};
			struct EventMoveToMax : public BaseEvent {};
			struct EventMoveToMin : public BaseEvent {};
			struct EventDragThumb : public BaseEvent {};
			struct EventFinishDrag : public BaseEvent {};
			struct EventSetThumbPosition : public BaseEvent {};

			struct EventSetMinTrackValue : public BaseEvent {};
			struct EventSetMaxTrackValue : public BaseEvent {};
			struct EventSetTrackRange : public BaseEvent {};
			struct EventSetMinSelectValue : public BaseEvent {};
			struct EventSetMaxSelectValue : public BaseEvent {};
			struct EventSetSelectRange : public BaseEvent {};
			struct EventEnableSelectRange : public BaseEvent {};
			struct EventDisableSelectRange : public BaseEvent {};
			struct EventToolTipStyle : public BaseEvent {};
			struct EventSetSmallStep : public BaseEvent {};
			struct EventSetLargeStep : public BaseEvent {};
			struct EventSetTicksFrequency : public BaseEvent {};
		};



	private:
		TrackBar(const TrackBar &TrackBar) = delete;
		TrackBar(const TrackBar &&TrackBar) = delete;
		TrackBar(ParentWindow* parent, const ConStruct<TrackBar> &conStruct);
		~TrackBar();


	public:
		TrackBar& operator=(const TrackBar& TrackBar) = delete;
		TrackBar& operator-(const TrackBar &&TrackBar) = delete;


	private:
		LRESULT SubclassProcedure(
			HWND hWnd,
			UINT msg,
			WPARAM wParam, LPARAM lParam,
			UINT_PTR uIDSubClass, DWORD_PTR dwRefData) override;
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		LRESULT NotifyProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		//void SetPosition(int x, int y);
		
		void EnableSelectRange();
		void DisableSelectRange();
		bool IsThumbInSelectRange();


	public:
		void SetThumbPosition(int newPosition);
		void SetMinTrackValue(int value);
		void SetMaxTrackValue(int value);
		void SetTrackRange(Range newRAnge);
		void SetTrackRange(int minValue, int maxValue);
		void SetMinSelectValue(int value);
		void SetMaxSelectValue(int value);
		void SetSelectRange(Range newSelectRange);
		void SetSelectRange(int minValue, int maxValue);
		void SetToolTipsStyle(ToolTipsStyle toolTipsStyle);
		void SetSmallStep(unsigned int smallStep);
		void SetLargeStep(unsigned int largeStep);
		void SetTicksFrequency(unsigned int frequency);
	public:
		int GetPosition();
		int GetMinTrackValue();
		int GetMaxTrackValue();
		Range GetTrackRange();
		int GetMinSelectValue();
		int GetMaxSelectValue();
		Range GetSelectRange();
		unsigned int GetSmallStep();
		unsigned int GetLargeStep();


	public:
		friend class ObjectCreator;
	};

	template <> struct ConStruct<TrackBar>
	{
		Rect rect;
		Range trackRange;
		int startPosition;
		unsigned int smallStep, largeStep;
		TrackBar::Orientation orientation;
		TrackBar::TickStyle tickStyle;
		unsigned int tick_frequency;
		bool enableSelectRange;
		Range selectRange;
		TrackBar::ToolTipsStyle toolTipsStyle;

		ConStruct(
			const Rect& rect = Rect(50, 50, 100, 50),
			const Range& trackRange = Range(0, 100),
			int startPosition = 0,
			unsigned int smallStep = 1u, unsigned int largeStep = 5u,
			TrackBar::Orientation orientation = TrackBar::Orientation::Horizontal,
			TrackBar::TickStyle tickStyle = TrackBar::TickStyle::Default,
			unsigned int tick_frequency = 1u,
			bool enableSelectRange = false,
			const Range& selectRange = Range(0, 100),
			TrackBar::ToolTipsStyle toolTipsStyle = TrackBar::ToolTipsStyle::None)
			: rect(rect)
			, trackRange(trackRange)
			, startPosition(startPosition)
			, smallStep(smallStep), largeStep(largeStep)
			, orientation(orientation)
			, tickStyle(tickStyle)
			, tick_frequency(tick_frequency)
			, enableSelectRange(enableSelectRange)
			, selectRange(selectRange)
			, toolTipsStyle(toolTipsStyle)
		{}
	};
}

#endif // !TRACKBAR_H
