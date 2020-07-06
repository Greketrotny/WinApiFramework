#ifndef TRACK_BAR_H
#define TRACK_BAR_H

#include "BaseControl.h"


namespace WinApiFramework
{
	class TrackBar;
	template <> struct ConStruct<TrackBar>;

	class TrackBar : public ChildControl
	{
		// -- fields -- //
	public:
		enum Orientation
		{
			Horizontal,
			Vertical
		};
		enum TickStyle
		{
			Default,
			Top,
			Bottom,
			Both,
			NoTicks
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

				MinTrackValueChanged,
				MaxTrackValueChanged,
				TrackRangeChanged,
				MinSelectValueChanged,
				MaxSelectValueChanged,
				SelectRangeChanged,
				SelectRangeEnabled,
				SelectRangeDisabled,
				SmallStepChanged,
				LargeStepChanged,

				LinedUp,
				LinedDown,
				PagedUp,
				PagedDown,
				MovedToTop,
				MovedToBottom,
				ThumbDragged,
				DraggingFinnished,
				ThumbPositionChanged
			};
			Type type;

			Event(TrackBar::Event::Type type = TrackBar::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		int m_ThumbPosition;
		Range m_trackRange;
		Range m_selectRange;
		Orientation m_orientation;
		TickStyle m_tickStyle;
		unsigned int m_smallStep, m_largeStep;
		bool m_selectRangeEnabled;

		ChildControl::EventsManager<TrackBar::Event> m_events;


		// -- constructors -- //
	private:
		TrackBar(const TrackBar &TrackBar) = delete;
		TrackBar(const TrackBar &&TrackBar) = delete;
		TrackBar(ParentControl* parentControl, const ConStruct<TrackBar> &conStruct);
		~TrackBar();


		// -- operators -- //
	public:
		TrackBar& operator=(const TrackBar& TrackBar) = delete;
		TrackBar& operator-(const TrackBar &&TrackBar) = delete;


		// -- methods -- //
	private:
		int ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(TrackBar::Event((TrackBar::Event::Type)event.type));
		}
	public:
		//void SetPosition(int x, int y);
		
		void EnableSelectRange();
		void DisableSelectRange();
		bool IsThumbInSelectRange();


		// ~~ TrackBar::setters ~~ //
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
		void SetSmallStep(unsigned int smallStep);
		void SetLargeStep(unsigned int largeStep);

		// ~~ TrackBar::getters ~~ //
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


		// -- property fields -- //
	public:
		ChildControl::EventsManager<TrackBar::Event>& Events;

		// -- friends -- //
	public:
		friend class ParentControl;
	};

	template <> struct ConStruct<TrackBar> : ConStruct<ChildControl>
	{
		Range trackRange;
		int startPosition;
		unsigned int smallStep, largeStep;
		TrackBar::Orientation orientation;
		TrackBar::TickStyle tickStyle;
		bool enableSelectRange;
		Range selectRange;

		ConStruct(ConStruct<ChildControl> winCtrlConStruct = ConStruct<ChildControl>(),
			Range trackRange = Range(0, 100),
			int startPosition = 0,
			unsigned int smallStep = 1u, unsigned int largeStep = 5u,
			TrackBar::Orientation orientation = TrackBar::Orientation::Horizontal,
			TrackBar::TickStyle tickStyle = TrackBar::TickStyle::Default,
			bool enableSelectRange = false,
			Range selectRange = Range(0, 100))
			: ConStruct<ChildControl>(winCtrlConStruct)
			, trackRange(trackRange)
			, startPosition(startPosition)
			, smallStep(smallStep), largeStep(largeStep)
			, orientation(orientation)
			, tickStyle(tickStyle)
			, enableSelectRange(enableSelectRange)
			, selectRange(selectRange)
		{}
	};
}

#endif // !TRACK_BAR_H
