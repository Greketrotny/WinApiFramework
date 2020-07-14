#include "Precompiled.h"
#include "ProgressBar.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;

// [CLASS] ProgressBar -------------------------|
// -- constructors -- //
ProgressBar::ProgressBar(ParentControl* parentControl, const ConStruct< ProgressBar>& conStruct)
	: ChildControl(parentControl, conStruct)
	, MinValue(m_minValue)
	, MaxValue(m_maxValue)
	, Position(m_position)
	, Step(m_step)
	, State(m_barState)
	, Events(m_events)
{
	m_minValue = std::max(0, conStruct.range.min);
	m_maxValue = std::max(unsigned int(conStruct.range.max), m_minValue);

	m_position = conStruct.position;
	m_barState = conStruct.barState;
	m_barOrientation = conStruct.barOrientation;
	m_barDisplayStyle = conStruct.barDisplayStyle;
	m_step = conStruct.step;

	CreateControlWindow();
}
ProgressBar::~ProgressBar()
{
	DestroyControlWindow();
}

// -- methods -- //
// private:
LRESULT ProgressBar::ControlProcedure(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	default:
		return 1;
	}
	return 0;
}
bool ProgressBar::CreateControlWindow()
{
	// [>] Set ProgressBar styles
	// set bar orientation
	switch (m_barOrientation)
	{
		case WinApiFramework::ProgressBar::Horizontal:	break;
		case WinApiFramework::ProgressBar::Vertical: m_controlStyle |= PBS_VERTICAL;
	}

	// set bar display style
	switch (m_barDisplayStyle)
	{
		case WinApiFramework::ProgressBar::Default:	break;
		case WinApiFramework::ProgressBar::Marquee:	m_controlStyle |= PBS_MARQUEE;					break;
		case WinApiFramework::ProgressBar::Smooth:	m_controlStyle |= PBS_SMOOTH;					break;
		case WinApiFramework::ProgressBar::SmoothReversed:	m_controlStyle |= PBS_SMOOTHREVERSE;	break;
	}


	// create window
	m_hWindow = CreateWindow(PROGRESS_CLASS, NULL,
		m_controlStyle,
		m_rect.position.x - m_pParentControl->GetCanvasPosition().x,
		m_rect.position.y - m_pParentControl->GetCanvasPosition().y,
		m_rect.size.width, m_rect.size.height,
		m_pParentControl->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!m_hWindow)
	{
		MessageBox(nullptr, L"ProgressBar creation failed.", L"ProgressBar create error", MB_OK | MB_ICONERROR);
		return false;
	}

	// set better font
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);

	//SendMessage(m_hWindow, PBM_SETMARQUEE, TRUE, 30);

	SetRange(m_minValue, m_maxValue);
	SetPosition(m_position);
	SetStep(m_step);
	SetState(m_barState);

	return true;
}
void ProgressBar::DestroyControlWindow()
{
	DestroyWindow(m_hWindow);
}

// public:
void ProgressBar::SetMinValue(unsigned int value)
{
	if (value > m_maxValue) value = m_maxValue;
	m_minValue = value;

	SendMessage(m_hWindow, PBM_SETRANGE, 0, MAKELPARAM(value, m_maxValue));
	m_events.PushEvent(ProgressBar::Event(ProgressBar::Event::Type::MinValueChanged));
}
void ProgressBar::SetMaxValue(unsigned int value)
{
	if (value < m_minValue) value = m_minValue;
	m_maxValue = value;

	SendMessage(m_hWindow, PBM_SETRANGE, 0, MAKELPARAM(m_maxValue, value));
	m_events.PushEvent(ProgressBar::Event(ProgressBar::Event::Type::MaxValueChanged));
}
void ProgressBar::SetRange(unsigned int min, unsigned int max)
{
	if (min >= max) max = min + 1;
	m_minValue = min;
	m_maxValue = max;
	m_events.PushEvent(Event(Event::Type::RangeChanged));

	SendMessage(m_hWindow, PBM_SETRANGE, 0, MAKELPARAM(m_minValue, m_maxValue));
}
void ProgressBar::SetStep(unsigned int step)
{
	this->m_step = step;
	SendMessage(m_hWindow, PBM_SETSTEP, step, 0);
	m_events.PushEvent(ProgressBar::Event(ProgressBar::Event::Type::StepChanged));
}
void ProgressBar::SetPosition(unsigned int position)
{
	if (position < m_minValue) position = m_minValue;
	if (position > m_maxValue) position = m_maxValue;
	this->m_position = position;

	SendMessage(m_hWindow, PBM_SETPOS, position, 0);
	m_events.PushEvent(ProgressBar::Event(ProgressBar::Event::Type::PositionChanged));
}
void ProgressBar::SetState(ProgressBar::BarState state)
{
	if (state == BarState::Normal && m_barState != state)
		SendMessage(m_hWindow, PBM_SETSTATE, 0, PBST_NORMAL);
	else if (state == BarState::Pause && m_barState != state)
		SendMessage(m_hWindow, PBM_SETSTATE, 0, PBST_PAUSED);
	else if (state == BarState::Error && m_barState != state)
		SendMessage(m_hWindow, PBM_SETSTATE, 0, PBST_ERROR);

	m_barState = state;
	m_events.PushEvent(ProgressBar::Event(ProgressBar::Event::Type::BarStateChanged));
}
void ProgressBar::SetOrientation(ProgressBar::BarOrientation barOrientation)
{
	if (m_controlStyle & PBS_VERTICAL) m_controlStyle = m_controlStyle & (~PBS_VERTICAL);

	m_barOrientation = barOrientation;
	if (m_barOrientation == ProgressBar::BarOrientation::Vertical) m_controlStyle |= PBS_VERTICAL;

	SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

	m_events.PushEvent(Event(Event::Type::BarOrientationChanged));
}
void ProgressBar::SetDisplayStyle(ProgressBar::BarDisplayStyle barDisplayStyle)
{
	m_controlStyle = m_controlStyle & (~(PBS_MARQUEE | PBS_SMOOTH | PBS_SMOOTHREVERSE));

	m_barDisplayStyle = barDisplayStyle;
	switch (m_barDisplayStyle)
	{
		case WinApiFramework::ProgressBar::Default:	break;
		case WinApiFramework::ProgressBar::Marquee:			m_controlStyle |= PBS_MARQUEE;			break;
		case WinApiFramework::ProgressBar::Smooth:			m_controlStyle |= PBS_SMOOTH;			break;
		case WinApiFramework::ProgressBar::SmoothReversed:	m_controlStyle |= PBS_SMOOTHREVERSE;	break;
	}
	SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

	m_events.PushEvent(Event(Event::Type::BarDisplayStyleChanged));
}
void ProgressBar::StepIt()
{
	m_position += m_step;
	SetPosition(m_position);
}
void ProgressBar::StepIt(int step)
{
	m_position += step;
	SetPosition(m_position);
}
// [CLASS] ProgressBar -------------------------|