#include "progressbar.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	ProgressBar::ProgressBar(ParentWindow* parent, const ConStruct< ProgressBar>& conStruct)
		: BaseWindow(parent)
	{
		m_rect = conStruct.rect;

		m_minValue = conStruct.range.min;
		m_maxValue = std::max(conStruct.range.max, m_minValue);

		m_position = conStruct.position;
		m_barState = conStruct.barState;
		m_barOrientation = conStruct.barOrientation;
		m_barDisplayStyle = conStruct.barDisplayStyle;
		m_step = conStruct.step;

		CreateWinapiWindow();
	}
	ProgressBar::~ProgressBar()
	{
		DestroyWinapiWindow();
	}

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
	bool ProgressBar::CreateWinapiWindow()
	{
		m_window_style |= WS_CHILD | WS_VISIBLE;

		// [>] Set ProgressBar styles
		// set bar orientation
		switch (m_barOrientation)
		{
			case WinapiFramework::ProgressBar::BarOrientation::Horizontal:	break;
			case WinapiFramework::ProgressBar::BarOrientation::Vertical: m_window_style |= PBS_VERTICAL;
		}

		// set bar display style
		switch (m_barDisplayStyle)
		{
			case WinapiFramework::ProgressBar::BarDisplayStyle::Default:	break;
			case WinapiFramework::ProgressBar::BarDisplayStyle::Marquee:		m_window_style |= PBS_MARQUEE;			break;
			case WinapiFramework::ProgressBar::BarDisplayStyle::Smooth:			m_window_style |= PBS_SMOOTH;			break;
			case WinapiFramework::ProgressBar::BarDisplayStyle::SmoothReversed:	m_window_style |= PBS_SMOOTHREVERSE;	break;
		}


		// create window
		m_hWindow = CreateWindow(PROGRESS_CLASS, NULL,
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

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
		SetDisplayStyle(m_barDisplayStyle);

		return true;
	}
	void ProgressBar::DestroyWinapiWindow()
	{
		DestroyWindow(m_hWindow);
	}

	void ProgressBar::SetMinValue(int value)
	{
		if (value > m_maxValue) value = m_maxValue;
		m_minValue = value;

		SendMessage(m_hWindow, PBM_SETRANGE, 0, MAKELPARAM(value, m_maxValue));
		RaiseEventByHandler<Events::EventSetMinValue>();
	}
	void ProgressBar::SetMaxValue(int value)
	{
		if (value < m_minValue) value = m_minValue;
		m_maxValue = value;

		SendMessage(m_hWindow, PBM_SETRANGE, 0, MAKELPARAM(m_maxValue, value));
		RaiseEventByHandler<Events::EventSetMaxValue>();
	}
	void ProgressBar::SetRange(int min, int max)
	{
		if (min >= max) max = min + 1;
		m_minValue = min;
		m_maxValue = max;
		RaiseEventByHandler<Events::EventSetRange>();

		SendMessage(m_hWindow, PBM_SETRANGE32, m_minValue, m_maxValue);
	}
	void ProgressBar::SetStep(int step)
	{
		this->m_step = step;
		SendMessage(m_hWindow, PBM_SETSTEP, step, 0);
		RaiseEventByHandler<Events::EventSetStep>();
	}
	void ProgressBar::SetPosition(int position)
	{
		if (position < m_minValue) position = m_minValue;
		if (position > m_maxValue) position = m_maxValue;
		this->m_position = position;

		if (m_barDisplayStyle == BarDisplayStyle::Marquee) return;
		SendMessage(m_hWindow, PBM_SETPOS, position, 0);
		RaiseEventByHandler<Events::EventSetPosition>();
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
		RaiseEventByHandler<Events::EventSetStep>();
	}
	void ProgressBar::SetOrientation(ProgressBar::BarOrientation barOrientation)
	{
		if (m_window_style & PBS_VERTICAL) m_window_style = m_window_style & (~PBS_VERTICAL);

		m_barOrientation = barOrientation;
		if (m_barOrientation == ProgressBar::BarOrientation::Vertical) m_window_style |= PBS_VERTICAL;

		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

		RaiseEventByHandler<Events::EventSetBarOriantation>();
	}
	void ProgressBar::SetDisplayStyle(ProgressBar::BarDisplayStyle barDisplayStyle)
	{
		m_window_style = m_window_style & (~(PBS_MARQUEE | PBS_SMOOTH | PBS_SMOOTHREVERSE));

		m_barDisplayStyle = barDisplayStyle;
		switch (m_barDisplayStyle)
		{
			case WinapiFramework::ProgressBar::BarDisplayStyle::Default:	break;
			case WinapiFramework::ProgressBar::BarDisplayStyle::Marquee:		m_window_style |= PBS_MARQUEE;			break;
			case WinapiFramework::ProgressBar::BarDisplayStyle::Smooth:			m_window_style |= PBS_SMOOTH;			break;
			case WinapiFramework::ProgressBar::BarDisplayStyle::SmoothReversed:	m_window_style |= PBS_SMOOTHREVERSE;	break;
		}
		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

		if (m_barDisplayStyle == BarDisplayStyle::Marquee)
			SendMessage(m_hWindow, PBM_SETMARQUEE, TRUE, 30);
		else
			SendMessage(m_hWindow, PBM_SETMARQUEE, FALSE, 30);

		RaiseEventByHandler<Events::EventSetDisplayStyle>();
	}
	void ProgressBar::StepIt()
	{
		m_position += m_step;
		SetPosition(m_position);
		RaiseEventByHandler<Events::EventStep>();
	}
	void ProgressBar::StepIt(int step)
	{
		m_position += step;
		SetPosition(m_position);
		RaiseEventByHandler<Events::EventStep>();
	}

	int ProgressBar::GetMinValue()
	{
		return m_minValue;
	}
	int ProgressBar::GetMaxValue()
	{
		return m_maxValue;
	}
	Range ProgressBar::GetRange()
	{
		return Range(m_minValue, m_maxValue);
	}
	int ProgressBar::GetStep()
	{
		return m_step;
	}
	int ProgressBar::GetPosition()
	{
		return m_position;
	}
	ProgressBar::BarState ProgressBar::GetState()
	{
		return m_barState;
	}
	ProgressBar::BarOrientation ProgressBar::GetBarOrientation()
	{
		return m_barOrientation;
	}
	ProgressBar::BarDisplayStyle ProgressBar::GetBarDisplayStyle()
	{
		return m_barDisplayStyle;
	}
}