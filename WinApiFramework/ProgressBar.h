#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "WindowControl.h"

namespace WinApiFramework
{
	class ProgressBar : public WindowControl
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
		struct Config : WindowControl::Config
		{
			Range range;
			unsigned int position = 0;
			unsigned int step = 1;
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
		WindowControl::EventsManager<ProgressBar::Event> events;


		// -- constrctor -- //
	public:
		ProgressBar(const ProgressBar &progressBar) = delete;
		ProgressBar(const ProgressBar &&progressBar) = delete;
		ProgressBar(const Config& config);
		~ProgressBar();


		// -- operators -- //
	public:
		ProgressBar& operator=(const ProgressBar &progressBar) = delete;
		ProgressBar& operator=(const ProgressBar &&progressBar) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
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
		WindowControl::EventsManager<ProgressBar::Event>& Events;
	};
}

#endif // !PROGRESS_BAR_H