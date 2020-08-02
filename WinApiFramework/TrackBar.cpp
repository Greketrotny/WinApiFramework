#include "trackbar.h"
#include "winapi_framework.h"

#include <algorithm>	// std::clamp()


namespace WinapiFramework
{
	TrackBar::TrackBar(ParentWindow* parent, const ConStruct< TrackBar>& conStruct)
		: BaseWindow(parent)
	{
		m_rect = conStruct.rect;

		m_trackRange = conStruct.trackRange;
		m_ThumbPosition = conStruct.startPosition;
		m_smallStep = conStruct.smallStep;
		m_largeStep = conStruct.largeStep;
		m_orientation = conStruct.orientation;
		m_tickStyle = conStruct.tickStyle;
		m_ticksFrequency = conStruct.tick_frequency;
		m_selectRange = conStruct.selectRange;
		m_selectRangeEnabled = conStruct.enableSelectRange;
		m_toolTipsStyle = conStruct.toolTipsStyle;

		CreateWinapiWindow();
	}
	TrackBar::~TrackBar()
	{
		DestroyWinapiWindow();
	}

	LRESULT TrackBar::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		UINT event = LOWORD(wParam);
		switch (event)
		{
			case TB_LINEUP:			// Left/Up arrow
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				RaiseEventByHandler<Events::EventLineUp>();
				break;

			case TB_LINEDOWN:		// Right/Down arrow
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				RaiseEventByHandler<Events::EventLineDown>();
				break;

			case TB_PAGEUP:			// PageUp
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				RaiseEventByHandler<Events::EventPageUp>();
				break;

			case TB_PAGEDOWN:		// PageDown
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				RaiseEventByHandler<Events::EventPageDown>();
				break;

			case TB_TOP:			// End
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				RaiseEventByHandler<Events::EventMoveToMax>();
				break;

			case TB_BOTTOM:			// Home
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				RaiseEventByHandler<Events::EventMoveToMin>();
				break;

			case TB_THUMBTRACK:		// Mouse drag
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				RaiseEventByHandler<Events::EventDragThumb>();
				break;

			//case TB_THUMBPOSITION:
			case TB_ENDTRACK:	// dragged and released thumb
				m_ThumbPosition = SendMessage(m_hWindow, TBM_GETPOS, 0, 0);
				RaiseEventByHandler<Events::EventFinishDrag>();
				break;

			default:
				return 1;
		}
		return 0;
	}
	bool TrackBar::CreateWinapiWindow()
	{
		m_window_style |= WS_CHILD | WS_VISIBLE;

		// [>] Set TrackBar styles
		// set orienttaion
		if (m_orientation == TrackBar::Orientation::Horizontal)		m_window_style |= TBS_HORZ;
		else if (m_orientation == TrackBar::Orientation::Vertical)	m_window_style |= TBS_VERT;

		// set selection style
		if (m_selectRangeEnabled)	m_window_style |= TBS_ENABLESELRANGE;

		// set tick style
		switch (m_tickStyle)
		{
			
			case WinapiFramework::TrackBar::Default:	break;
			case WinapiFramework::TrackBar::Top:	m_window_style |= TBS_TOP;		break;
			case WinapiFramework::TrackBar::Bottom:	m_window_style |= TBS_BOTTOM;	break;
			case WinapiFramework::TrackBar::Both:	m_window_style |= TBS_BOTH;		break;
			case WinapiFramework::TrackBar::NoTicks:m_window_style |= TBS_NOTICKS;	break;
		}
		m_window_style |= TBS_AUTOTICKS;

		// set tool tips style
		if (m_toolTipsStyle != ToolTipsStyle::ToolTipsStyleNone) m_window_style |= TBS_TOOLTIPS;


		// [>] create window
		m_hWindow = CreateWindow(TRACKBAR_CLASS, L"TrackBar",
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

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
		SetTicksFrequency(m_ticksFrequency);

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
	void TrackBar::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
	}
	void TrackBar::EnableSelectRange()
	{
		if (!m_selectRangeEnabled)
		{
			m_selectRangeEnabled = true;
			m_window_style |= TBS_ENABLESELRANGE;
			SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);
			RaiseEventByHandler<Events::EventEnableSelectRange>();
		}
	}
	void TrackBar::DisableSelectRange()
	{
		if (m_selectRangeEnabled)
		{
			m_selectRangeEnabled = false;
			m_window_style = m_window_style & (~TBS_ENABLESELRANGE);
			SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);
			RaiseEventByHandler<Events::EventDisableSelectRange>();
		}
	}
	bool TrackBar::IsThumbInSelectRange()
	{
		return (m_ThumbPosition >= m_selectRange.min && m_ThumbPosition <= m_selectRange.max) ? true : false;
	}

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
		RaiseEventByHandler<Events::EventSetThumbPosition>();
	}
	void TrackBar::SetMinTrackValue(int value)
	{
		m_trackRange.min = std::min(value, m_trackRange.max);

		SendMessage(m_hWindow, TBM_SETRANGEMIN, TRUE, value);
		RaiseEventByHandler<Events::EventSetMinTrackValue>();
	}
	void TrackBar::SetMaxTrackValue(int value)
	{
		m_trackRange.max = std::max(value, m_trackRange.min);

		SendMessage(m_hWindow, TBM_SETRANGEMAX, TRUE, value);
		RaiseEventByHandler<Events::EventSetMaxTrackValue>();
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
		RaiseEventByHandler<Events::EventSetTrackRange>();
	}
	void TrackBar::SetMinSelectValue(int value)
	{
		m_selectRange.min = std::clamp<int>(value, m_trackRange.min, m_selectRange.max);

		SendMessage(m_hWindow, TBM_SETSELSTART, TRUE, m_selectRange.min);
		RaiseEventByHandler<Events::EventSetMinSelectValue>();
	}
	void TrackBar::SetMaxSelectValue(int value)
	{
		m_selectRange.max = std::clamp<int>(value, m_selectRange.min, m_trackRange.max);

		SendMessage(m_hWindow, TBM_SETSELEND, TRUE, m_selectRange.max);
		RaiseEventByHandler<Events::EventSetMaxSelectValue>();
	}
	void TrackBar::SetSelectRange(Range newRange)
	{
		SetSelectRange(newRange.min, newRange.max);
	}
	void TrackBar::SetSelectRange(int minValue, int maxValue)
	{
		minValue = std::clamp<int>(minValue, m_trackRange.min, m_trackRange.max);
		maxValue = std::clamp<int>(maxValue, m_trackRange.min, m_trackRange.max);
		if (maxValue < minValue) maxValue = minValue;

		m_selectRange.min = minValue;
		m_selectRange.max = maxValue;

		if (m_selectRangeEnabled)
		{
			SendMessage(m_hWindow, TBM_SETSEL, TRUE, MAKELPARAM(minValue, maxValue));
			RaiseEventByHandler<Events::EventSetSelectRange>();
		}
	}
	void TrackBar::SetToolTipsStyle(ToolTipsStyle toolTipsStyle)
	{
		m_toolTipsStyle = toolTipsStyle;

		if (m_toolTipsStyle == ToolTipsStyle::ToolTipsStyleNone)
		{
			m_window_style = m_window_style & (~TBS_TOOLTIPS);
			SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);
		}
		else
		{
			m_window_style |= TBS_TOOLTIPS;
			SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);

			switch (m_toolTipsStyle)
			{
				case WinapiFramework::TrackBar::ToolTipsStyleTop:	
					SendMessage(m_hWindow, TBM_SETTIPSIDE, TBTS_TOP, 0);
					break;
				case WinapiFramework::TrackBar::ToolTipsStyleBottom:
					SendMessage(m_hWindow, TBM_SETTIPSIDE, TBTS_BOTTOM, 0);
					break;
			}
		}

		RaiseEventByHandler<Events::EventToolTipStyle>();
	}
	void TrackBar::SetSmallStep(unsigned int smallStep)
	{
		m_smallStep = smallStep;
		SendMessage(m_hWindow, TBM_SETLINESIZE, 0, smallStep);
		RaiseEventByHandler<Events::EventSetSmallStep>();
	}
	void TrackBar::SetLargeStep(unsigned int largeStep)
	{
		m_largeStep = largeStep;
		SendMessage(m_hWindow, TBM_SETPAGESIZE, 0, largeStep);
		RaiseEventByHandler<Events::EventSetLargeStep>();
	}
	void TrackBar::SetTicksFrequency(unsigned int frequency)
	{
		m_ticksFrequency = std::max(frequency, 1u);
		SendMessage(m_hWindow, TBM_SETTICFREQ, m_ticksFrequency, 0);
		RaiseEventByHandler<Events::EventSetTicksFrequency>();
	}

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
}
