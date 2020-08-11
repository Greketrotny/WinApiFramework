#include "button.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	Button::Button(ParentWindow* parent, const ConStruct<Button>& conStruct)
		: BaseWindow(parent)
		, HasSubclassProcedure(this, &Button::SubclassProcedure)
	{
		m_rect = conStruct.rect;
		m_caption = conStruct.caption;
		m_caption_position = conStruct.caption_position;

		CreateWinapiWindow();
	}
	Button::~Button()
	{
		DestroyWinapiWindow();
	}

	LRESULT Button::SubclassProcedure(
		HWND hWnd,
		UINT msg,
		WPARAM wParam, LPARAM lParam,
		UINT_PTR uIDSubClass, DWORD_PTR dwRefData)
	{
		// We do not return after succesful mouse event handling becuase DefSubclassProc 
		// must be called from Framework SubclassProcedure
		HandleMouseEvent(msg, wParam, lParam);

		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}
	LRESULT Button::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		UINT event = HIWORD(wParam);
		switch (event)
		{
			case BN_CLICKED:
				RaiseEventByHandler<Events::EventClick>();
				break;

			case BN_DBLCLK:
				RaiseEventByHandler<Events::EventDoubleClick>();
				break;

			case BN_SETFOCUS:
				RaiseEventByHandler<Events::EventFocus>();
				break;

			case BN_KILLFOCUS:
				RaiseEventByHandler<Events::EventUnfocus>();
				break;

			default:
				return 1;
		}
		return 0;
	}
	bool Button::CreateWinapiWindow()
	{
		// [>] Set button styles
		// set button appearance
		m_window_style |= WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_PUSHBUTTON | BS_MULTILINE;

		// set caption position
		switch (m_caption_position)
		{
			case WinapiFramework::Button::CaptionPosition::Center:		m_window_style |= BS_CENTER;			break;
			case WinapiFramework::Button::CaptionPosition::TopLeft:		m_window_style |= BS_TOP | BS_LEFT;		break;
			case WinapiFramework::Button::CaptionPosition::TopCenter:	m_window_style |= BS_TOP | BS_CENTER;	break;
			case WinapiFramework::Button::CaptionPosition::TopRight:	m_window_style |= BS_TOP | BS_RIGHT;	break;
			case WinapiFramework::Button::CaptionPosition::MiddleLeft:	m_window_style |= BS_LEFT;				break;
			case WinapiFramework::Button::CaptionPosition::MiddleCenter:m_window_style |= BS_CENTER;			break;
			case WinapiFramework::Button::CaptionPosition::MiddleRight:	m_window_style |= BS_RIGHT;				break;
			case WinapiFramework::Button::CaptionPosition::BottomLeft:	m_window_style |= BS_BOTTOM | BS_LEFT;	break;
			case WinapiFramework::Button::CaptionPosition::BottomCenter:m_window_style |= BS_BOTTOM;			break;
			case WinapiFramework::Button::CaptionPosition::BottomRight:	m_window_style |= BS_BOTTOM | BS_RIGHT;	break;
		}

		// [>] Create window
		m_hWindow = CreateWindow(L"BUTTON", m_caption.c_str(),
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::GetInstance().GetProgramInstance(), nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
			return false;
		}

		SetWindowSubclass(m_hWindow, GetSubclassProcedure(), 0, 0);
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_subclass_procedure);

		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hFont, 0);

		UpdateWindow(m_hWindow);
		return true;
	}
	void Button::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
		RemoveWindowSubclass(m_hWindow, GetSubclassProcedure(), 0);
	}

	void Button::SetCaption(std::wstring newCaption)
	{
		m_caption = newCaption;
		SetWindowText(m_hWindow, m_caption.c_str());

		RaiseEventByHandler<Events::EventSetCaption>();
	}
	void Button::SetCaptionPosition(Button::CaptionPosition captionPosition)
	{
		m_caption_position = captionPosition;

		m_window_style = GetWindowStyle(m_hWindow);
		m_window_style = m_window_style & (~(BS_TOP | BS_BOTTOM | BS_LEFT | BS_CENTER | BS_RIGHT));
		switch (m_caption_position)
		{
			case WinapiFramework::Button::CaptionPosition::Center:
			case WinapiFramework::Button::CaptionPosition::MiddleCenter:m_window_style |= BS_CENTER;			break;
			case WinapiFramework::Button::CaptionPosition::TopLeft:		m_window_style |= BS_TOP | BS_LEFT;		break;
			case WinapiFramework::Button::CaptionPosition::TopCenter:	m_window_style |= BS_TOP | BS_CENTER;	break;
			case WinapiFramework::Button::CaptionPosition::TopRight:	m_window_style |= BS_TOP | BS_RIGHT;	break;
			case WinapiFramework::Button::CaptionPosition::MiddleLeft:	m_window_style |= BS_LEFT;				break;
			case WinapiFramework::Button::CaptionPosition::MiddleRight:	m_window_style |= BS_RIGHT;				break;
			case WinapiFramework::Button::CaptionPosition::BottomLeft:	m_window_style |= BS_BOTTOM | BS_LEFT;	break;
			case WinapiFramework::Button::CaptionPosition::BottomCenter:m_window_style |= BS_BOTTOM;			break;
			case WinapiFramework::Button::CaptionPosition::BottomRight:	m_window_style |= BS_BOTTOM | BS_RIGHT;	break;
		}

		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);
		RedrawWindow(m_hWindow, NULL, NULL, RDW_INVALIDATE);

		RaiseEventByHandler<Events::EventSetCaptionPosition>();
	}

	const std::wstring& Button::GetCaption() const
	{
		return m_caption;
	}
	Button::CaptionPosition Button::GetCaptionPosition() const
	{
		return m_caption_position;
	}
}