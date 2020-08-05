#include "label.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	Label::Label(ParentWindow* parent, const ConStruct<Label>& conStruct)
		: BaseWindow(parent)
		, HasSubclassProcedure(this, &Label::SubclassProcedure)
	{
		m_window_style |= WS_CHILD | WS_VISIBLE;

		m_rect = conStruct.rect;
		m_caption = conStruct.caption;
		m_textAlignment = conStruct.textAlignment;

		CreateWinapiWindow();
	}
	Label::~Label()
	{
		DestroyWinapiWindow();
	}

	LRESULT Label::SubclassProcedure(
		HWND hWnd,
		UINT msg,
		WPARAM wParam, LPARAM lParam,
		UINT_PTR uIDSubClass, DWORD_PTR dwRefData)
	{
		if (HandleMouseEvent(msg, wParam, lParam) == 0) return 0;

		return 1;
	}
	LRESULT Label::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		return 1;
	}
	bool Label::CreateWinapiWindow()
	{
		// set text alignment
		if (m_textAlignment == Label::TextAlignment::Left)
			m_window_style |= SS_LEFT;
		if (m_textAlignment == Label::TextAlignment::Center)
			m_window_style |= SS_CENTER;
		if (m_textAlignment == Label::TextAlignment::Right)
			m_window_style |= SS_RIGHT;

		// for notifications from parent control
		m_window_style |= SS_NOTIFY;

		// create window
		m_hWindow = CreateWindow(L"STATIC", m_caption.c_str(),
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);


		// check control creation
		if (!m_hWindow)
		{
			MessageBox(nullptr, L"Label window creation failed.", L"Label create error", MB_OK | MB_ICONERROR);
			return false;
		}

		SetWindowSubclass(m_hWindow, GetSubclassProcedure(), 0, 0);

		// set pointer to non-static std::function to receive WM_ messages
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_subclass_procedure);

		// set visual font
		HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);

		return true;
	}
	void Label::DestroyWinapiWindow()
	{
		RemoveWindowSubclass(m_hWindow, GetSubclassProcedure(), 0);
		DestroyWindow(m_hWindow);
	}

	void Label::SetCaption(const std::wstring& newCaption)
	{
		m_caption = newCaption;
		SetWindowText(m_hWindow, m_caption.c_str());

		RaiseEventByHandler<Events::EventSetCaption>();
	}
	void Label::SetTextAligment(Label::TextAlignment textAlignment)
	{
		unsigned int newStyle = 0u;
		if (textAlignment == Label::TextAlignment::Left)
			newStyle = SS_LEFT;
		if (textAlignment == Label::TextAlignment::Center)
			newStyle = SS_CENTER;
		if (textAlignment == Label::TextAlignment::Right)
			newStyle = SS_RIGHT;

		m_window_style = (m_window_style & ~(SS_LEFT | SS_CENTER | SS_RIGHT | SS_LEFTNOWORDWRAP)) | newStyle;
		SetWindowLong(m_hWindow, GWL_STYLE, m_window_style);
		InvalidateRect(m_hWindow, NULL, TRUE);

		RaiseEventByHandler<Events::EventSetTextAlignment>();
	}

	const std::wstring& Label::GetCaption()
	{
		return m_caption;
	}
	Label::TextAlignment Label::GetTextAlignment()
	{
		return m_textAlignment;
	}
}