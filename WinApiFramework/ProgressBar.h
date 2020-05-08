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
	private:
		unsigned int minValue, maxValue, position, step;

	public:
		enum BarState
		{
			Normal,
			Pause,
			Error
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
				MinValueChange,
				MaxValueChange,
				PositionChange,
				StepChange,
				BarStateChange
			};
			Type type;

			Event(ProgressBar::Event::Type type = ProgressBar::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		BarState barState;
		ChildControl::EventsManager<ProgressBar::Event> events;


		// -- constrctor -- //
	public:
		ProgressBar(const ProgressBar &progressBar) = delete;
		ProgressBar(const ProgressBar &&progressBar) = delete;
		ProgressBar(const ConStruct<ProgressBar>& conStruct);
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
			events.PushEvent(ProgressBar::Event((ProgressBar::Event::Type)event.type));
		}
	public:
		void SetMinValue(unsigned int value);
		void SetMaxValue(unsigned int value);
		void SetRange(unsigned int min, unsigned int max);
		void SetStep(unsigned int step);
		void SetPosition(unsigned int position);
		void SetState(BarState state);
		void StepIt();


		// -- property fields -- //
	public:
		const unsigned int& MinValue;
		const unsigned int& MaxValue;
		const unsigned int& Position;
		const unsigned int& Step;
		const BarState& State;
		ChildControl::EventsManager<ProgressBar::Event>& Events;
	};

	template <> struct ConStruct<ProgressBar> : ConStruct<ChildControl>
	{
		Range range;
		int position = 0;
		unsigned int step = 1;

		ConStruct(ConStruct<ChildControl> winCtrlConStrut = ConStruct<ChildControl>(),
				  Range range = Range(0, 100),
				  int position = 0,
				  unsigned int step = 1u)
			: ConStruct<ChildControl>::ConStruct(winCtrlConStrut)
			, range(range)
			, position(position)
			, step(step)
		{}
	};
}

#endif // !PROGRESS_BAR_H