#include "ProgressBar.h"
#include "WinApiWindow.h"

using namespace WinApiFramework;

// [CLASS] ProgressBar -------------------------|
// -- constructors -- //
ProgressBar::ProgressBar(const ProgressBar::Config& config)
	: WindowControl(config),
	MinValue(minValue),
	MaxValue(maxValue),
	Position(position),
	Step(step),
	State(barState),
	Events(events)
{
	minValue = config.minValue;
	if (config.maxValue > config.minValue)
		maxValue = config.maxValue;
	else
		maxValue = minValue + 10;

	position = config.position;
	step = config.step;
}
ProgressBar::ProgressBar(const ProgressBar::Config& config, ProgressBar::EventHandler *eh)
	:ProgressBar(config)
{
	events.SetEventHandler(eh);
}
ProgressBar::~ProgressBar()
{

}

// -- methods -- //
// private:
int ProgressBar::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool ProgressBar::CreateControlWindow()
{
	// create window
	hControl = CreateWindow(PROGRESS_CLASS, NULL,
		WS_CHILD | WS_VISIBLE,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!hControl)
	{
		MessageBox(nullptr, L"ProgressBar creation failed.", L"ProgressBar create error", MB_OK | MB_ICONERROR);
		return false;
	}

	// set better font
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	SetRange(minValue, maxValue);
	SetPosition(position);
	SetStep(step);

	return true;
}
// public:
void ProgressBar::SetMinValue(unsigned int value)
{
	minValue = value;
	SendMessage(hControl, PBM_SETRANGE, 0, MAKELPARAM(value, maxValue));
}
void ProgressBar::SetMaxValue(unsigned int value)
{
	maxValue = value;
	SendMessage(hControl, PBM_SETRANGE, 0, MAKELPARAM(maxValue, value));
}
void ProgressBar::SetRange(unsigned int min, unsigned int max)
{
	if (min >= max) max = min + 1;
	minValue = min;
	maxValue = max;

	SendMessage(hControl, PBM_SETRANGE, 0, MAKELPARAM(minValue, maxValue));
}
void ProgressBar::SetStep(unsigned int step)
{
	this->step = step;
	SendMessage(hControl, PBM_SETSTEP, step, 0);
}
void ProgressBar::SetPosition(unsigned int position)
{
	this->position = position;
	SendMessage(hControl, PBM_SETPOS, position, 0);
}
void ProgressBar::SetState(ProgressBar::BarState state)
{
	if (state == BarState::Normal && barState != state)
		SendMessage(hControl, PBM_SETSTATE, 0, PBST_NORMAL);
	else if (state == BarState::Pause && barState != state)
		SendMessage(hControl, PBM_SETSTATE, 0, PBST_PAUSED);
	else if (state == BarState::Error && barState != state)
		SendMessage(hControl, PBM_SETSTATE, 0, PBST_ERROR);

	barState = state;
}
void ProgressBar::StepIt()
{
	SetPosition(position + step);
}
// [CLASS] ProgressBar -------------------------|