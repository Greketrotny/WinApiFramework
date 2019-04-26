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
			unsigned int minValue = 0;
			unsigned int maxValue = 100;
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

			Event()
			{
				type = Invalid;
			}
			Event(ProgressBar::Event::Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler<ProgressBar::Event>
		{
			virtual void HandleEvent(ProgressBar::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case ProgressBar::Event::Type::MinValueChange:	MinValueChange();	break;
				case ProgressBar::Event::Type::MaxValueChange:	MaxValueChange();	break;
				case ProgressBar::Event::Type::PositionChange:	PositionChange();	break;
				case ProgressBar::Event::Type::StepChange:		StepChange();		break;
				case ProgressBar::Event::Type::BarStateChange:	BarStateChange();	break;
				}
			}
			virtual void MinValueChange() {};
			virtual void MaxValueChange() {};
			virtual void PositionChange() {};
			virtual void StepChange() {};
			virtual void BarStateChange() {};
		};
	private:
		BarState barState;
		WindowControl::Events<ProgressBar::Event> events;


		// -- constrctor -- //
	public:
		ProgressBar(const ProgressBar &progressBar) = delete;
		ProgressBar(const ProgressBar &&progressBar) = delete;
		ProgressBar(const Config& config);
		ProgressBar(const Config& config, EventHandler *eventHandler);
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
		WindowControl::Events<ProgressBar::Event>& Events;
	};
}

#endif // !PROGRESS_BAR_H