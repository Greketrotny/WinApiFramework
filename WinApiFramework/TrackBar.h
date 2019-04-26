#ifndef TRACK_BAR_H
#define TRACK_BAR_H

#include "WindowControl.h"


namespace WinApiFramework
{
	class TrackBar : public WindowControl
	{
		// -- fields -- //
	private:
		bool selectRangeEnabled = false;
		unsigned int smallStep = 1u, largeStep = 10u;
		int thumbPosition;
		HWND hLabel1, hLabel2;

	public:
		struct Range
		{
			int min = 0, max = 100;
		};
		struct Labels
		{
			std::wstring label1 = L"";
			std::wstring label2 = L"";
		};
		enum Orientation
		{
			Horizontal,
			Vertical
		};
		struct Config : public WindowControl::Config
		{
			Range trackRange;
			Range selectRange;
			Labels labels;
			Orientation orientation;
			int startPosition;
			unsigned int smallStep, largeStep;
			bool EnableSelectRange = false;
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
				TrackPosChange
			};
			Type type;

			Event()
			{
				this->type = Invalid;
			}
			Event(Type type)
			{
				this->type = type;
			}
		};
		struct EventHandler : public WindowControl::EventHandler<TrackBar::Event>
		{
			virtual void HandleEvent(TrackBar::Event event)
			{
				HandleBaseEvent((WindowControl::Event::Type)event.type);

				switch (event.type)
				{
				case TrackBar::Event::Type::MaxValueChange: MaxValueChange(); break;
				case TrackBar::Event::Type::MinValueChange: MinValueChange(); break;
				case TrackBar::Event::Type::TrackPosChange: TrackPosChange(); break;
				}
			}
			virtual void MinValueChange() {};
			virtual void MaxValueChange() {};
			virtual void TrackPosChange() {};
		};
	private:
		WindowControl::Events<TrackBar::Event> events;
		Range trackRange;
		Range selectRange;
		Labels labels;
		Orientation orientation;


		// -- constructors -- //
	public:
		TrackBar(const TrackBar &TrackBar) = delete;
		TrackBar(const TrackBar &&TrackBar) = delete;
		TrackBar(const Config &config);
		TrackBar(const Config &config, EventHandler *eventHandler);
		~TrackBar();


		// -- operators -- //
	public:
		TrackBar& operator=(const TrackBar& TrackBar) = delete;
		TrackBar& operator-(const TrackBar &&TrackBar) = delete;


		// -- methods -- //
	private:
		int ControlProc(WPARAM wParam, LPARAM lParam) override;
		bool CreateControlWindow() override;
		void PushBaseEvent(WindowControl::Event event) override
		{
			events.PushEvent(TrackBar::Event((TrackBar::Event::Type)event.type));
		}
	public:
		void SetPosition(int x, int y);
		void SetMinTrackValue(int value);
		void SetMaxTrackValue(int value);
		void SetMinSelectValue(int value);
		void SetMaxSelectValue(int value);
		void SetTrackRange(Range newRAnge);
		void SetTrackRange(int minValue, int maxValue);
		void SetSelectRange(Range newSelectRange);
		void SetSelectRange(int minValue, int maxValue);
		void SetThumbPosition(int newPosition);
		void SetSmallStep(unsigned int smallStep);
		void SetLargeStep(unsigned int largeStep);
		void EnableSelectRange();
		void DisableSelectRange();
		void SetLabel1(const std::wstring& label1);
		void SetLabel2(const std::wstring& label2);
		void SetLabels(const std::wstring& label1, const std::wstring& label2);
		void SetLabels(const Labels& newLabels);

		// -- property fields -- //
	public:
		const int& Position;
		const int& TrackMin;
		const int& TrackMax;
		const Orientation& Orientation;
		const unsigned int& SmallStep;
		const unsigned int& LargeStep;
	};
}

#endif // !TRACK_BAR_H
