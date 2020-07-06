#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "BaseControl.h"

namespace WinApiFramework
{
	class ProgressBar;
	template <> struct ConStruct<ProgressBar>;

	class ProgressBar : public ChildControl
	{
		// -- fields -- //
	public:
		enum BarState
		{
			Normal,
			Pause,
			Error
		};
		enum BarOrientation
		{
			Horizontal,
			Vertical
		};
		enum BarDisplayStyle
		{
			Default,
			Marquee,
			Smooth,
			SmoothReversed
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
				MinValueChanged,
				MaxValueChanged,
				RangeChanged,
				PositionChanged,
				StepChanged,
				BarStateChanged,
				BarOrientationChanged,
				BarDisplayStyleChanged
			};
			Type type;

			Event(ProgressBar::Event::Type type = ProgressBar::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		unsigned int m_minValue, m_maxValue, m_position, m_step;
		BarState m_barState;
		BarOrientation m_barOrientation;
		BarDisplayStyle m_barDisplayStyle;
		ChildControl::EventsManager<ProgressBar::Event> m_events;
		Graphics::Color m_barColor;


		// -- constrctor -- //
	private:
		ProgressBar(const ProgressBar &progressBar) = delete;
		ProgressBar(const ProgressBar &&progressBar) = delete;
		ProgressBar(ParentControl* parentControl, const ConStruct<ProgressBar>& conStruct);
		~ProgressBar();


		// -- operators -- //
	public:
		ProgressBar& operator=(const ProgressBar &progressBar) = delete;
		ProgressBar& operator=(const ProgressBar &&progressBar) = delete;


		// -- methods -- //
	private:
		int ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void DestroyControlWindow() override;
		void PushBaseEvent(ChildControl::Event event) override
		{
			m_events.PushEvent(ProgressBar::Event((ProgressBar::Event::Type)event.type));
		}
	public:
		void SetMinValue(unsigned int value);
		void SetMaxValue(unsigned int value);
		void SetRange(unsigned int min, unsigned int max);
		void SetStep(unsigned int step);
		void SetPosition(unsigned int position);
		void SetState(BarState state);
		void SetOrientation(BarOrientation barOrientation);
		void SetDisplayStyle(BarDisplayStyle barDisplayStyle);
		void StepIt();
		void StepIt(int step);


		// -- property fields -- //
	public:
		const unsigned int& MinValue;
		const unsigned int& MaxValue;
		const unsigned int& Position;
		const unsigned int& Step;
		const BarState& State;
		ChildControl::EventsManager<ProgressBar::Event>& Events;

		// -- friends -- //
	public:
		friend class ParentControl;
	};

	template <> struct ConStruct<ProgressBar> : ConStruct<ChildControl>
	{
		Range range;
		int position = 0;
		ProgressBar::BarState barState;
		ProgressBar::BarOrientation barOrientation;
		ProgressBar::BarDisplayStyle barDisplayStyle;
		unsigned int step = 1;


		ConStruct(ConStruct<ChildControl> winCtrlConStrut = ConStruct<ChildControl>(),
			Range range = Range(0, 100),
			int position = 0,
			ProgressBar::BarState barState = ProgressBar::BarState::Normal,
			ProgressBar::BarOrientation barOrientation = ProgressBar::BarOrientation::Horizontal,
			ProgressBar::BarDisplayStyle barDisplayStyle = ProgressBar::BarDisplayStyle::Default,
			unsigned int step = 1u)
			: ConStruct<ChildControl>::ConStruct(winCtrlConStrut)
			, range(range)
			, position(position)
			, barState(barState)
			, barOrientation(barOrientation)
			, barDisplayStyle(barDisplayStyle)
			, step(step)
		{}
	};
}

#endif // !PROGRESS_BAR_H