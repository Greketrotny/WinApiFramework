#ifndef TRACK_BAR_H
#define TRACK_BAR_H

#include "BaseControl.h"


namespace WinApiFramework
{
	class TrackBar;
	template <> struct ConStruct<Button>;

	class TrackBar : public ChildControl
	{
		// -- fields -- //
	public:
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
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4,
				MinTrackValueChange,
				MaxTrackValueChange,
				MinSelectValueChange,
				MaxSelectValueChange,
				PositionChange,
				SmallStepChange,
				LargeStepChange
			};
			Type type;

			Event(TrackBar::Event::Type type = TrackBar::Event::Type::Invalid)
			{
				this->type = type;
			}
		};
	private:
		int position;
		Range trackRange;
		Range selectRange;
		Orientation orientation;
		unsigned int smallStep, largeStep;
		bool selectRangeEnabled;
		HWND hLabel1, hLabel2;
		Labels labels;

		ChildControl::EventsManager<TrackBar::Event> events;


		// -- constructors -- //
	public:
		TrackBar(const TrackBar &TrackBar) = delete;
		TrackBar(const TrackBar &&TrackBar) = delete;
		TrackBar(const ConStruct<TrackBar> &conStruct);
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
		const Orientation& TrackOrientation;
		const unsigned int& SmallStep;
		const unsigned int& LargeStep;
		ChildControl::EventsManager<TrackBar::Event>& Events;
	};

	template <> struct ConStruct<TrackBar> : ConStruct<ChildControl>
	{
		int startPosition;
		Range trackRange;
		Range selectRange;
		TrackBar::Orientation orientation;
		unsigned int smallStep, largeStep;
		bool EnableSelectRange;

		ConStruct(ConStruct<ChildControl> winCtrlConStruct = ConStruct<ChildControl>(),
				  int startPosition = 0,
				  Range trackRange = Range(0, 100),
				  Range selectRange = Range(0, 100),
				  TrackBar::Orientation orientation = TrackBar::Orientation::Horizontal,
				  unsigned int smallStep = 1u, unsigned int largeStep = 5u,
				  bool enagleSelectRange = false)
			: ConStruct<ChildControl>::ConStruct(winCtrlConStruct)
			, startPosition(startPosition)
			, trackRange(trackRange)
			, selectRange(selectRange)
			, orientation(orientation)
			, smallStep(smallStep), largeStep(largeStep)
			, EnableSelectRange(enagleSelectRange)
		{}
	};
}

#endif // !TRACK_BAR_H
