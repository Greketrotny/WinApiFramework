#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "base_window.h"

#include "Color.h"

namespace WinapiFramework
{
	class ProgressBar;
	template <> struct ConStruct<ProgressBar>;

	class ProgressBar : public BaseWindow
	{
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
	private:
		int m_minValue, m_maxValue, m_position, m_step;
		BarState m_barState;
		BarOrientation m_barOrientation;
		BarDisplayStyle m_barDisplayStyle;
		Graphics::Color m_barColor;

	public:
		struct Events : public BaseWindowEvents
		{
			struct EventSetMinValue : public BaseEvent {};
			struct EventSetMaxValue : public BaseEvent {};
			struct EventSetRange : public BaseEvent {};
			struct EventSetPosition : public BaseEvent {};
			struct EventSetStep : public BaseEvent {};
			struct EventSetBarState : public BaseEvent {};
			struct EventSetBarOriantation : public BaseEvent {};
			struct EventSetDisplayStyle : public BaseEvent {};
			struct EventStep : public BaseEvent {};
		};


	private:
		ProgressBar(const ProgressBar &other) = delete;
		ProgressBar(const ProgressBar &&other) = delete;
		ProgressBar(ParentWindow* parent, const ConStruct<ProgressBar>& conStruct);
		~ProgressBar();


	public:
		ProgressBar& operator=(const ProgressBar &other) = delete;
		ProgressBar& operator=(const ProgressBar &&other) = delete;


	private:
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		void SetMinValue(int value);
		void SetMaxValue(int value);
		void SetRange(int min, int max);
		void SetStep(int step);
		void SetPosition(int position);
		void SetState(BarState state);
		void SetOrientation(BarOrientation barOrientation);
		void SetDisplayStyle(BarDisplayStyle barDisplayStyle);
		void StepIt();
		void StepIt(int step);

		int GetMinValue();
		int GetMaxValue();
		Range GetRange();
		int GetStep();
		int GetPosition();
		BarState GetState();
		BarOrientation GetBarOrientation();
		BarDisplayStyle GetBarDisplayStyle();


		friend class ObjectCreator;
	};

	template <> struct ConStruct<ProgressBar>
	{
		Rect rect;
		Range range;
		int position;
		ProgressBar::BarState barState;
		ProgressBar::BarOrientation barOrientation;
		ProgressBar::BarDisplayStyle barDisplayStyle;
		int step;


		ConStruct(
			const Rect& rect,
			Range range = Range(0, 100),
			int position = 0,
			ProgressBar::BarState barState = ProgressBar::BarState::Normal,
			ProgressBar::BarOrientation barOrientation = ProgressBar::BarOrientation::Horizontal,
			ProgressBar::BarDisplayStyle barDisplayStyle = ProgressBar::BarDisplayStyle::Default,
			int step = 1u)
			: rect(rect)
			, range(range)
			, position(position)
			, barState(barState)
			, barOrientation(barOrientation)
			, barDisplayStyle(barDisplayStyle)
			, step(step)
		{}
	};
}

#endif // !PROGRESSBAR_H