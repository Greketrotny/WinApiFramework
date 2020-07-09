#include "Precompiled.h"
#include "TrackBar.h"
#include "WinApiFramework.h"

#include <algorithm>	// std::clamp()


namespace WinApiFramework
{
	// ~~~~~~~~ [CLASS] TrackBar ~~~~~~~~
	// ~~ constructor ~~ //
	TrackBar::TrackBar(ParentControl* parentControl, const ConStruct< TrackBar>& conStruct)
		: ChildControl(parentControl, conStruct)
		, Events(m_events)
	{
		m_trackRange = conStruct.trackRange;
		m_ThumbPosition = conStruct.startPosition;
		m_smallStep = conStruct.smallStep;
		m_largeStep = conStruct.largeStep;
		m_orientation = conStruct.orientation;
		m_tickStyle = conStruct.tickStyle;
		m_selectRange = conStruct.selectRange;
		m_selectRangeEnabled = conStruct.enableSelectRange;
		m_toolTipsStyle = conStruct.toolTipsStyle;

		CreateControlWindow();
	}
	TrackBar::~TrackBar()
	{
		DestroyControlWindow();
	}

	// ~~ methods ~~ //
	// private:
	ProcedureResult TrackBar::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		if ((HWND)lParam != m_hWindow) return ProcedureResult::TargetNotFound;

		UINT event = LOWORD(wParam);
		switch (event)
		{
			case TB_LINEUP:			// Left/Up arrow
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				m_events.PushEvent(Event(Event::Type::LinedUp));
				break;

			case TB_LINEDOWN:		// Right/Down arrow
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				m_events.PushEvent(Event(Event::Type::LinedDown));
				break;

			case TB_PAGEUP:			// PageUp
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				m_events.PushEvent(Event(Event::Type::PagedUp));
				break;

			case TB_PAGEDOWN:		// PageDown
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				m_events.PushEvent(Event(Event::Type::PagedDown));
				break;

			case TB_TOP:			// End
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				m_events.PushEvent(Event(Event::Type::MovedToTop));
				break;

			case TB_BOTTOM:			// Home
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				m_events.PushEvent(Event(Event::Type::MovedToBottom));
				break;

			case TB_THUMBTRACK:		// Mouse drag
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				m_events.PushEvent(TrackBar::Event::Type::ThumbDragged);
				break;

			//case TB_THUMBPOSITION:
			case TB_ENDTRACK:	// dragged and released thumb
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				m_events.PushEvent(Event(Event::Type::DraggingFinnished));
				break;

			default:
				return ProcedureResult::Unhandled;
		}
		return ProcedureResult::Handled;
	}
	bool TrackBar::CreateControlWindow()
	{
		// [>] Set TrackBar styles
		// set orienttaion
		if (m_orientation == TrackBar::Orientation::Horizontal)		m_controlStyle |= TBS_HORZ;
		else if (m_orientation == TrackBar::Orientation::Vertical)	m_controlStyle |= TBS_VERT;

		// set selection style
		if (m_selectRangeEnabled)	m_controlStyle |= TBS_ENABLESELRANGE;

		// set tick style
		switch (m_tickStyle)
		{
			
			case WinApiFramework::TrackBar::Default:	break;
			case WinApiFramework::TrackBar::Top:	m_controlStyle |= TBS_TOP;		break;
			case WinApiFramework::TrackBar::Bottom:	m_controlStyle |= TBS_BOTTOM;	break;
			case WinApiFramework::TrackBar::Both:	m_controlStyle |= TBS_BOTH;		break;
			case WinApiFramework::TrackBar::NoTicks:m_controlStyle |= TBS_NOTICKS;	break;
		}
		m_controlStyle |= TBS_AUTOTICKS;

		// set tool tips style
		if (m_toolTipsStyle != ToolTipsStyle::ToolTipsStyleNone) m_controlStyle |= TBS_TOOLTIPS;


		// [>] create window
		m_hWindow = CreateWindow(TRACKBAR_CLASS, L"TrackBar",
			m_controlStyle,
			m_rect.position.x, m_rect.position.y, m_rect.size.width, m_rect.size.height,
			m_pParentControl->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"TrackBar window creation failed.", L"TrackBar create error", MB_OK | MB_ICONERROR);
			return false;
		}

		SetTrackRange(m_trackRange);
		SetSelectRange(m_selectRange);
		SetToolTipsStyle(m_toolTipsStyle);
		SetThumbPosition(m_ThumbPosition);
		SetSmallStep(m_smallStep);
		SetLargeStep(m_largeStep);

		//// create and set side labels
		//hLabel1 = CreateWindow(L"STATIC", labels.label1.c_str(), SS_CENTER | WS_CHILD | WS_VISIBLE,
		//	0, 0, 50, 10, parentWindow->WndHandle, NULL, Framework::ProgramInstance, NULL);
		//hLabel2 = CreateWindow(L"STATIC", labels.label2.c_str(), SS_CENTER | WS_CHILD | WS_VISIBLE,
		//	0, 0, 50, 10, parentWindow->WndHandle, NULL, Framework::ProgramInstance, NULL);

		//HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		//SendMessage(hLabel1, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
		//SendMessage(hLabel2, WM_SETFONT, (WPARAM)hNormalFont, TRUE);

		//SendMessage(hControl, TBM_SETBUDDY, TRUE, (LPARAM)hLabel1);
		//SendMessage(hControl, TBM_SETBUDDY, FALSE, (LPARAM)hLabel2);

		return true;
	}
	void TrackBar::DestroyControlWindow()
	{
		::DestroyWindow(m_hWindow);
	}
	void TrackBar::EnableSelectRange()
	{
		if (!m_selectRangeEnabled)
		{
			m_selectRangeEnabled = true;
			m_controlStyle |= TBS_ENABLESELRANGE;
			SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);
			m_events.PushEvent(Event(Event::Type::SelectRangeEnabled));
		}
	}
	void TrackBar::DisableSelectRange()
	{
		if (m_selectRangeEnabled)
		{
			m_selectRangeEnabled = false;
			m_controlStyle = m_controlStyle & (~TBS_ENABLESELRANGE);
			SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);
			m_events.PushEvent(Event(Event::Type::SelectRangeDisabled));
		}
	}
	bool TrackBar::IsThumbInSelectRange()
	{
		return (m_ThumbPosition >= m_selectRange.min && m_ThumbPosition <= m_selectRange.max) ? true : false;
	}

	// ~~ TrackBar::setters ~~ //
	/*void TrackBar::SetPosition(int x, int y)
	{
		//RECT label1Rect, label2Rect;
		//GetWindowRect(hLabel1, &label1Rect);
		//GetWindowRect(hLabel2, &label2Rect);
		//SetWindowPos(hLabel1, NULL, label1Rect.left - parentWindow->X - (rect.x - x), label1Rect.top - parentWindow->Y - (rect.y - y), 0, 0, SWP_NOSIZE);
		//SetWindowPos(hLabel2, NULL, label2Rect.left - parentWindow->X - (rect.x - x), label2Rect.top - parentWindow->Y - (rect.y - y), 0, 0, SWP_NOSIZE);

		ChildControl::Move(x, y);

		//SendMessage(hControl, TBM_SETBUDDY, TRUE, (LPARAM)hLabel1);
		//SendMessage(hControl, TBM_SETBUDDY, FALSE, (LPARAM)hLabel2);
	}*/
	void TrackBar::SetThumbPosition(int newThumbPosition)
	{
		m_ThumbPosition = std::min(std::max(newThumbPosition, m_trackRange.min), m_trackRange.max);
		SendMessage(m_hWindow, TBM_SETPOS, TRUE, m_ThumbPosition);
		m_events.PushEvent(Event(Event::Type::ThumbPositionChanged));
	}
	void TrackBar::SetMinTrackValue(int value)
	{
		m_trackRange.min = std::min(value, m_trackRange.max);

		SendMessage(m_hWindow, TBM_SETRANGEMIN, TRUE, value);

		m_events.PushEvent(Event(Event::Type::MinTrackValueChanged));
	}
	void TrackBar::SetMaxTrackValue(int value)
	{
		m_trackRange.max = std::max(value, m_trackRange.min);

		SendMessage(m_hWindow, TBM_SETRANGEMAX, TRUE, value);

		m_events.PushEvent(Event(Event::Type::MinTrackValueChanged));
	}
	void TrackBar::SetTrackRange(Range newRange)
	{
		SetTrackRange(newRange.min, newRange.max);
	}
	void TrackBar::SetTrackRange(int minValue, int maxValue)
	{
		maxValue = std::max(maxValue, minValue);

		m_trackRange.min = minValue;
		m_trackRange.max = maxValue;

		SendMessage(m_hWindow, TBM_SETRANGE, TRUE, MAKELPARAM(minValue, maxValue));

		m_events.PushEvent(Event(Event::Type::TrackRangeChanged));
	}
	void TrackBar::SetMinSelectValue(int value)
	{
		std::clamp<int>(value, m_trackRange.min, m_selectRange.max);
		m_selectRange.min = value;

		SendMessage(m_hWindow, TBM_SETSELSTART, TRUE, m_selectRange.min);

		m_events.PushEvent(Event(Event::Type::MinSelectValueChanged));
	}
	void TrackBar::SetMaxSelectValue(int value)
	{
		std::clamp<int>(value, m_selectRange.min, m_trackRange.max);
		m_selectRange.max = value;

		SendMessage(m_hWindow, TBM_SETSELEND, TRUE, m_selectRange.max);

		m_events.PushEvent(Event(Event::Type::MaxSelectValueChanged));
	}
	void TrackBar::SetSelectRange(Range newRange)
	{
		SetSelectRange(newRange.min, newRange.max);
	}
	void TrackBar::SetSelectRange(int minValue, int maxValue)
	{
		std::clamp<int>(minValue, m_trackRange.min, m_trackRange.max);
		std::clamp<int>(maxValue, m_trackRange.min, m_trackRange.max);
		if (maxValue < minValue) maxValue = minValue;

		m_selectRange.min = minValue;
		m_selectRange.max = maxValue;

		if (m_selectRangeEnabled)
		{
			SendMessage(m_hWindow, TBM_SETSEL, TRUE, MAKELPARAM(minValue, maxValue));
			m_events.PushEvent(Event(Event::Type::SelectRangeChanged));
		}
	}
	void TrackBar::SetToolTipsStyle(ToolTipsStyle toolTipsStyle)
	{
		m_toolTipsStyle = toolTipsStyle;

		if (m_toolTipsStyle == ToolTipsStyle::ToolTipsStyleNone)
		{
			m_controlStyle = m_controlStyle & (~TBS_TOOLTIPS);
			SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);
		}
		else
		{
			m_controlStyle |= TBS_TOOLTIPS;
			SetWindowLong(m_hWindow, GWL_STYLE, m_controlStyle);

			switch (m_toolTipsStyle)
			{
				case WinApiFramework::TrackBar::ToolTipsStyleTop:	
					SendMessage(m_hWindow, TBM_SETTIPSIDE, TBTS_TOP, 0);
					break;
				case WinApiFramework::TrackBar::ToolTipsStyleBottom:
					SendMessage(m_hWindow, TBM_SETTIPSIDE, TBTS_BOTTOM, 0);
					break;
			}
		}

		m_events.PushEvent(Event(Event::Type::ToolTipsStyleChanged));
	}
	void TrackBar::SetSmallStep(unsigned int smallStep)
	{
		m_smallStep = smallStep;
		SendMessage(m_hWindow, TBM_SETLINESIZE, 0, smallStep);
		m_events.PushEvent(Event(Event::Type::SmallStepChanged));
	}
	void TrackBar::SetLargeStep(unsigned int largeStep)
	{
		m_largeStep = largeStep;
		SendMessage(m_hWindow, TBM_SETPAGESIZE, 0, largeStep);
		m_events.PushEvent(Event(Event::Type::LargeStepChanged));
	}
	void TrackBar::SetTicksFrequency(unsigned int frequency)
	{
		m_ticksFrequency = std::max(frequency, 1u);
		SendMessage(m_hWindow, TBM_SETTICFREQ, m_ticksFrequency, 0);

		m_events.PushEvent(Event(Event::Type::TicksFrequencyChanged));
	}

	// ~~ TrackBar::getters ~~ //
	int TrackBar::GetPosition()
	{
		return m_ThumbPosition;
	}
	int TrackBar::GetMinTrackValue()
	{
		return m_trackRange.min;
	}
	int TrackBar::GetMaxTrackValue()
	{
		return m_trackRange.max;
	}
	Range TrackBar::GetTrackRange()
	{
		return m_trackRange;
	}
	int TrackBar::GetMinSelectValue()
	{
		return m_selectRange.min;
	}
	int TrackBar::GetMaxSelectValue()
	{
		return m_selectRange.max;
	}
	Range TrackBar::GetSelectRange()
	{
		return m_selectRange;
	}
	unsigned int TrackBar::GetSmallStep()
	{
		return m_smallStep;
	}
	unsigned int TrackBar::GetLargeStep()
	{
		return m_largeStep;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}
