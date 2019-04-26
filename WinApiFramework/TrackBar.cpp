#include "TrackBar.h"
#include "WinApiWindow.h"

using namespace WinApiFramework;

// [CLASS] TrackBar ------------------------------|
// -- constructor -- //
TrackBar::TrackBar(const TrackBar::Config& config)
	:WindowControl(config),
	Position(thumbPosition),
	TrackMin(trackRange.min),
	TrackMax(trackRange.max),
	Orientation(orientation),
	SmallStep(smallStep),
	LargeStep(largeStep)
{
	thumbPosition = config.startPosition;
	trackRange = config.trackRange;
	selectRange = config.selectRange;
	labels = config.labels;
	orientation = config.orientation;
	smallStep = config.smallStep;
	largeStep = config.largeStep;
	selectRangeEnabled = config.EnableSelectRange;
}
TrackBar::TrackBar(const TrackBar::Config& config, TrackBar::EventHandler *eventHandler)
	: TrackBar(config)
{
	events.SetEventHandler(eventHandler);
}
TrackBar::~TrackBar()
{
	DestroyWindow(hLabel1);
	DestroyWindow(hLabel2);
}

// -- methods -- //
// private:
int TrackBar::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = LOWORD(wParam);
	switch (event)
	{
	case TB_TOP:			// End
	case TB_BOTTOM:			// Home
	case TB_LINEUP:			// Left/Up arrow
	case TB_LINEDOWN:		// Right/Down arrow
	case TB_PAGEUP:			// PageUp
	case TB_PAGEDOWN:		// PageDown
	case TB_THUMBTRACK:		// Mouse drag
		thumbPosition = SendMessage(hControl, TBM_GETPOS, 0, 0);
		events.PushEvent(TrackBar::Event::Type::TrackPosChange);
		break;

	default:
		return 1;	// if did't handle message
	}
	return 0;		// if did
}
bool TrackBar::CreateControlWindow()
{
	if (orientation == TrackBar::Orientation::Horizontal)
		controlStyle |= TBS_HORZ;
	else if (orientation == TrackBar::Orientation::Vertical)
		controlStyle |= TBS_VERT;

	if (selectRangeEnabled)
		controlStyle |= TBS_ENABLESELRANGE;

	// create window
	hControl = CreateWindow(TRACKBAR_CLASS, L"TrackBar",
		controlStyle,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!hControl)
	{
		MessageBox(nullptr, L"TrackBar window creation failed.", L"TrackBar create error", MB_OK | MB_ICONERROR);
		return false;
	}

	SetTrackRange(trackRange);
	SetSelectRange(selectRange);
	SetThumbPosition(thumbPosition);
	SetSmallStep(smallStep);
	SetLargeStep(largeStep);



	// create and set side labels
	hLabel1 = CreateWindow(L"STATIC", labels.label1.c_str(), SS_CENTER | WS_CHILD | WS_VISIBLE,
		0, 0, 50, 10, parentWindow->WndHandle, NULL, Framework::ProgramInstance, NULL);
	hLabel2 = CreateWindow(L"STATIC", labels.label2.c_str(), SS_CENTER | WS_CHILD | WS_VISIBLE,
		0, 0, 50, 10, parentWindow->WndHandle, NULL, Framework::ProgramInstance, NULL);

	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hLabel1, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
	SendMessage(hLabel2, WM_SETFONT, (WPARAM)hNormalFont, TRUE);

	SendMessage(hControl, TBM_SETBUDDY, TRUE, (LPARAM)hLabel1);
	SendMessage(hControl, TBM_SETBUDDY, FALSE, (LPARAM)hLabel2);

	return true;
}
// public:
void TrackBar::SetPosition(int x, int y)
{
	RECT label1Rect, label2Rect;
	GetWindowRect(hLabel1, &label1Rect);
	GetWindowRect(hLabel2, &label2Rect);
	//SetWindowPos(hLabel1, NULL, label1Rect.left - parentWindow->X - (rect.x - x), label1Rect.top - parentWindow->Y - (rect.y - y), 0, 0, SWP_NOSIZE);
	//SetWindowPos(hLabel2, NULL, label2Rect.left - parentWindow->X - (rect.x - x), label2Rect.top - parentWindow->Y - (rect.y - y), 0, 0, SWP_NOSIZE);

	WindowControl::Move(x, y);

	//SendMessage(hControl, TBM_SETBUDDY, TRUE, (LPARAM)hLabel1);
	//SendMessage(hControl, TBM_SETBUDDY, FALSE, (LPARAM)hLabel2);
}
void TrackBar::SetMinTrackValue(int value)
{
	if (value < trackRange.max)
	{
		trackRange.min = value;
		SendMessage(hControl, TBM_SETRANGEMIN, TRUE, value);
		SendMessage(hControl, TBM_SETTICFREQ, 1, 0);
	}
}
void TrackBar::SetMaxTrackValue(int value)
{
	if (value > trackRange.min)
	{
		trackRange.max = value;
		SendMessage(hControl, TBM_SETRANGEMAX, TRUE, value);
		SendMessage(hControl, TBM_SETTICFREQ, 1, 0);
	}
}
void TrackBar::SetMinSelectValue(int value)
{
	if (value > trackRange.min && value < selectRange.max)
	{
		selectRange.min = value;
		SendMessage(hControl, TBM_SETSELSTART, TRUE, value);
	}
}
void TrackBar::SetMaxSelectValue(int value)
{
	if (value < trackRange.max && value > selectRange.min)
	{
		SendMessage(hControl, TBM_SETSELEND, TRUE, value);
	}
}
void TrackBar::SetTrackRange(TrackBar::Range newRange)
{
	SetTrackRange(newRange.min, newRange.max);
}
void TrackBar::SetTrackRange(int minValue, int maxValue)
{
	if (minValue < maxValue)
	{
		if (minValue >= trackRange.max)
		{
			SendMessage(hControl, TBM_SETRANGEMAX, TRUE, maxValue);
			SendMessage(hControl, TBM_SETRANGEMIN, TRUE, minValue);
		}
		else
		{
			SendMessage(hControl, TBM_SETRANGEMIN, TRUE, minValue);
			SendMessage(hControl, TBM_SETRANGEMAX, TRUE, maxValue);
		}

		trackRange.min = minValue;
		trackRange.max = maxValue;
	}
}
void TrackBar::SetSelectRange(TrackBar::Range newRange)
{
	SetSelectRange(newRange.min, newRange.max);
}
void TrackBar::SetSelectRange(int minValue, int maxValue)
{
	if (selectRangeEnabled)
	{
		SendMessage(hControl, TBM_SETSEL, TRUE, minValue | (maxValue << 16));
	}
}
void TrackBar::SetThumbPosition(int newThumbPosition)
{
	if (newThumbPosition < trackRange.min) newThumbPosition = trackRange.min;
	if (newThumbPosition > trackRange.max) newThumbPosition = trackRange.max;

	thumbPosition = newThumbPosition;
	SendMessage(hControl, TBM_SETPOS, TRUE, thumbPosition);
}
void TrackBar::SetSmallStep(unsigned int smallStep)
{
	this->smallStep = smallStep;
	SendMessage(hControl, TBM_SETLINESIZE, 0, smallStep);
}
void TrackBar::SetLargeStep(unsigned int largeStep)
{
	this->largeStep = largeStep;
	SendMessage(hControl, TBM_SETPAGESIZE, 0, largeStep);
}
void TrackBar::EnableSelectRange()
{
	if (!selectRangeEnabled)
	{
		selectRangeEnabled = true;
		controlStyle |= TBS_ENABLESELRANGE;
		SetWindowLong(hControl, GWL_STYLE, controlStyle);
	}
}
void TrackBar::DisableSelectRange()
{
	if (selectRangeEnabled)
	{
		selectRangeEnabled = false;
		controlStyle = controlStyle & (~TBS_ENABLESELRANGE);
		SetWindowLong(hControl, GWL_STYLE, controlStyle);
	}
}
void TrackBar::SetLabel1(const std::wstring& label1)
{
	labels.label1 = label1;
	SetWindowText(hLabel1, labels.label1.c_str());
}
void TrackBar::SetLabel2(const std::wstring& label2)
{
	labels.label2 = label2;
	SetWindowText(hLabel2, labels.label2.c_str());
}
void TrackBar::SetLabels(const std::wstring& label1, const std::wstring& label2)
{
	SetLabel1(label1);
	SetLabel2(label2);
}
void TrackBar::SetLabels(const TrackBar::Labels& newLabels)
{
	SetLabel1(newLabels.label1);
	SetLabel2(newLabels.label2);
}
// [CLASS] TrackBar ------------------------------|