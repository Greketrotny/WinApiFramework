#include "base_window.h"
#include "winapi_framework.h"

#include <assert.h>

namespace WinapiFramework
{
	// ~~~~~~~~ [CLASS] BaseWindow ~~~~~~~~
	BaseWindow::BaseWindow(ParentWindow* parent)
		: mp_parent(parent)
		, m_window_style(0)
	{
	}
	BaseWindow::~BaseWindow()
	{
	}

	LRESULT BaseWindow::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		return LRESULT(TRUE);
	}
	
	void BaseWindow::Destroy()
	{
		DoDestroy();
	}
	void BaseWindow::Enable()
	{
		DoEnable();
		RaiseEventByHandler<BaseWindowEvents::EventEnable>();
	}
	void BaseWindow::Disable()
	{
		DoDisable();
		RaiseEventByHandler<BaseWindowEvents::EventDisable>();
	}
	void BaseWindow::Move(int x, int y)
	{
		assert(mp_parent);

		m_rect.position.x = x;
		m_rect.position.y = y;

		DoMove(
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y);

		mp_parent->AdjustCanvasRect();
		RaiseEventByHandler<BaseWindowEvents::EventMove>();
	}
	void BaseWindow::Move(const Point& position)
	{
		Move(position.x, position.y);
	}
	void BaseWindow::Resize(int width, int height)
	{
		m_rect.size.width = width;
		m_rect.size.height = height;

		if (m_rect.size.width < 0) m_rect.size.width = 0;
		if (m_rect.size.height < 0) m_rect.size.height = 0;

		DoResize(m_rect.size.width, m_rect.size.height);

		mp_parent->AdjustCanvasRect();
		RaiseEventByHandler<BaseWindowEvents::EventResize>();
	}
	void BaseWindow::Resize(const Size& size)
	{
		Resize(size.width, size.height);
	}
	void BaseWindow::SetRect(const Rect& rect)
	{
		Move(rect.position);
		Resize(rect.size);
	}

	void BaseWindow::DoDestroy()
	{
		assert(mp_parent);
		mp_parent->DestroyChild(this);
	}	
	void BaseWindow::DoEnable()
	{
		::EnableWindow(m_hWindow, TRUE);
	}
	void BaseWindow::DoDisable()
	{
		::EnableWindow(m_hWindow, FALSE);
	}	
	void BaseWindow::DoMove(int x, int y)
	{
		SetWindowPos(m_hWindow, nullptr,
			x, y,
			0, 0,
			SWP_NOSIZE);
	}
	void BaseWindow::DoResize(int width, int height)
	{
		SetWindowPos(m_hWindow, nullptr,
			0, 0,
			width, height,
			SWP_NOMOVE);
	}
	
	LRESULT BaseWindow::HandleMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Point pos(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		switch (msg)
		{
			case WM_LBUTTONDOWN:
			//case WM_NCLBUTTONDOWN:
			{
				m_mouse_captor.StartMouseCapture(Mouse::MouseButton::Left);
				RaiseEventByHandler<BaseWindowEvents::EventMouseLButtonPress>();
				break;
			}
			case WM_LBUTTONUP:
			//case WM_NCLBUTTONUP:
			{
				m_mouse_captor.StopTrackingMouse();
				m_mouse_captor.StopMouseCapture(Mouse::MouseButton::Left);
				RaiseEventByHandler<BaseWindowEvents::EventMouseLButtonRelease>();
				break;
			}
			case WM_LBUTTONDBLCLK:
			//case WM_NCLBUTTONDBLCLK:
			{
				RaiseEventByHandler<BaseWindowEvents::EventMouseLButtonDPress>();
				break;
			}

			case WM_RBUTTONDOWN:
			//case WM_NCRBUTTONDOWN:
			{
				m_mouse_captor.StartMouseCapture(Mouse::MouseButton::Right);
				RaiseEventByHandler<BaseWindowEvents::EventMouseRButtonPress>();
				break;
			}
			case WM_RBUTTONUP:
			//case WM_NCRBUTTONUP:
			{
				m_mouse_captor.StopTrackingMouse();
				m_mouse_captor.StopMouseCapture(Mouse::MouseButton::Right);
				RaiseEventByHandler<BaseWindowEvents::EventMouseRButtonRelease>();
				break;
			}
			case WM_RBUTTONDBLCLK:
			//case WM_NCRBUTTONDBLCLK:
			{
				RaiseEventByHandler<BaseWindowEvents::EventMouseRButtonDPress>();
				break;
			}

			case WM_MBUTTONDOWN:
			//case WM_NCMBUTTONDOWN:
			{
				m_mouse_captor.StartMouseCapture(Mouse::MouseButton::Middle);
				RaiseEventByHandler<BaseWindowEvents::EventMouseMButtonPress>();
				break;
			}
			case WM_MBUTTONUP:
			//case WM_NCMBUTTONUP:
			{
				m_mouse_captor.StopTrackingMouse();
				m_mouse_captor.StopMouseCapture(Mouse::MouseButton::Middle);
				RaiseEventByHandler<BaseWindowEvents::EventMouseMButtonRelease>();
				break;
			}
			case WM_MBUTTONDBLCLK:
			//case WM_NCMBUTTONDBLCLK:
			{
				RaiseEventByHandler<BaseWindowEvents::EventMouseMButtonDPress>();
				break;
			}

			case WM_MOUSEMOVE:
			//case WM_NCMOUSEMOVE:
			{
				if (!m_mouse_captor.IsTrackingMouse())
				{
					m_mouse_captor.StartTrackingMouse(m_hWindow);
					RaiseEventByHandler<BaseWindowEvents::EventMouseHover>();
				}

				RaiseEventByHandler<BaseWindowEvents::EventMouseMove>(pos);
				break;
			}

			case WM_MOUSELEAVE:
			{
				m_mouse_captor.CheckCaptureOnLeave(m_hWindow);
				m_mouse_captor.StopTrackingMouse();
				RaiseEventByHandler<BaseWindowEvents::EventMouseLeave>();
				break;
			}

			default: return 1;
		}

		return 0;
	}

	Point BaseWindow::GetMousePosition() const
	{
		assert(mp_parent);

		return mp_parent->GetMousePosition() - this->m_rect.position;
	}
	bool BaseWindow::IsMouseInside() const
	{
		Point mouse = GetMousePosition();
		return (mouse.x >= 0
			&& mouse.y >= 0
			&& mouse.x < m_rect.size.width
			&& mouse.y < m_rect.size.height);
	}

	const HWND& BaseWindow::GetWindowHandle() const
	{
		return m_hWindow;
	}
	const std::wstring& BaseWindow::GetWindowClassName() const
	{
		return m_window_class_name;
	}
	const Rect& BaseWindow::GetWindowRect() const
	{
		return m_rect;
	}
	
	ParentWindow* const BaseWindow::GetParent() const
	{
		return mp_parent;
	}
	WNDPROC BaseWindow::GetFrameworkProcedure() const
	{
		return (WNDPROC)Framework::WinApiProcedure;
	}
	SUBCLASSPROC BaseWindow::GetSubclassProcedure() const
	{
		return (SUBCLASSPROC)Framework::SubclassProcedure;
	}
	void BaseWindow::AppendAction(BaseAction* action) const
	{
		Framework::AppendAction(action);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentWindow ~~~~~~~~
	ParentWindow::ParentWindow(ParentWindow* parent)
		: BaseWindow(parent)
	{
	}
	ParentWindow::~ParentWindow()
	{
		DestroyAllChildren();
	}

	bool ParentWindow::DestroyChild(BaseWindow* child)
	{
		if (child == nullptr) return false;

		for (size_t i = 0; i < m_children.size(); ++i)
		{
			if (m_children[i] == child)
			{
				ObjectCreator::DestroyObject(child);
				m_children.erase(m_children.begin() + i);
				AdjustCanvasRect();
				return true;
			}
		}

		return false;
	}
	void ParentWindow::DestroyAllChildren()
	{
		for (size_t i = 0; i < m_children.size(); ++i)
		{
			ObjectCreator::DestroyObject(m_children[i]);
		}
		m_children.clear();
	}
	BaseWindow* ParentWindow::GetChild(size_t index)
	{
		if (index < m_children.size())	return m_children[index];
		else							return nullptr;
	}
	size_t ParentWindow::GetChildrenCount()
	{
		return m_children.size();
	}

	void ParentWindow::AdjustCanvasRect()
	{
		return;
	}

	Point ParentWindow::GetCanvasPosition() const
	{
		return Point(0, 0);
	}
	const Rect& ParentWindow::GetClientRect() const
	{
		return m_client_rect;
	}

	LRESULT ParentWindow::ProcessChildMessage(WPARAM wParam, LPARAM lParam)
	{
		for (BaseWindow*& child : m_children)
		{
			if (child == nullptr) continue;
			if (child->GetWindowHandle() == (HWND)lParam)
			{
				child->ControlProcedure(wParam, lParam);
				return 0;
			}
		}
		return 1;
	}
	const std::vector<BaseWindow*>& ParentWindow::GetChildren()
	{
		return m_children;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //



	// ~~~~~~~~ [CLASS] ScrollableWindow ~~~~~~~~
	ScrollableWindow::ScrollableWindow(ParentWindow* parent)
		: ParentWindow(parent)
		, m_canvas_rect(0, 0, 800, 600)
	{
	}
	ScrollableWindow::~ScrollableWindow()
	{
	}

	BoundRect ScrollableWindow::GetCanvasRect() const
	{
		return m_canvas_rect;
	}
	Point ScrollableWindow::GetCanvasDrift() const
	{
		return m_canvas_drift;
	}
	Point ScrollableWindow::GetCanvasPosition() const
	{
		return m_canvas_drift;
	}

	void ScrollableWindow::AdjustCanvasRect()
	{
		BoundRect boundRect(0, 0, 10, 10);

		auto& children = GetChildren();
		for (BaseWindow* child : children)
		{
			if (child == nullptr) continue;

			// right
			if (child->GetWindowRect().position.x + child->GetWindowRect().size.width > boundRect.right)
				boundRect.right = child->GetWindowRect().position.x + child->GetWindowRect().size.width;

			// bottom
			if (child->GetWindowRect().position.y + child->GetWindowRect().size.height > boundRect.bottom)
				boundRect.bottom = child->GetWindowRect().position.y + child->GetWindowRect().size.height;
		}

		m_canvas_rect = boundRect;
		UpdateScrollingInfo();
		AdjustCanvasDrift();
	}

	void ScrollableWindow::HandleVScroll(WPARAM wParam, LPARAM lParam)
	{
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS | SIF_PAGE | SIF_TRACKPOS;
		GetScrollInfo(m_hWindow, SB_VERT, &si);

		int pos = si.nPos;

		switch (LOWORD(wParam))
		{
			case SB_TOP:
				pos = m_canvas_rect.top;
				break;
			case SB_BOTTOM:
				pos = m_canvas_rect.bottom;
				break;
			case SB_LINEUP:
				if (pos > m_canvas_rect.top) pos--;
				break;
			case SB_LINEDOWN:
				if (pos < (m_canvas_rect.bottom) - m_client_rect.size.height) pos++;
				break;
			case SB_PAGEUP:
				pos -= si.nPage;
				if (pos < m_canvas_rect.top) pos = m_canvas_rect.top;
				break;
			case SB_PAGEDOWN:
				pos += si.nPage;
				if (pos > m_canvas_rect.bottom) pos = m_canvas_rect.bottom;
				break;
			case SB_THUMBPOSITION:
				pos = si.nTrackPos;
				break;
			case SB_THUMBTRACK:
				pos = si.nTrackPos;
				break;
		}

		int dy = (pos - si.nPos);
		m_canvas_drift.y += dy;
		ScrollWindowEx(m_hWindow, 0, -dy,
			(CONST RECT*)NULL,
			(CONST RECT*)NULL,
			(HRGN)NULL, (LPRECT)NULL,
			SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
		UpdateWindow(m_hWindow);

		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		si.nPos = pos;

		SetScrollInfo(m_hWindow, SB_VERT, &si, TRUE);
	}
	void ScrollableWindow::HandleHScroll(WPARAM wParam, LPARAM lParam)
	{
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS | SIF_PAGE | SIF_TRACKPOS;
		GetScrollInfo(m_hWindow, SB_HORZ, &si);

		int pos = si.nPos;

		switch (LOWORD(wParam))
		{
			case SB_LEFT:
				pos = m_canvas_rect.left;
				break;
			case SB_RIGHT:
				pos = m_canvas_rect.right;
				break;
			case SB_LINELEFT:
				if (pos > m_canvas_rect.left) pos--;
				break;
			case SB_LINERIGHT:
				if (pos < (m_canvas_rect.right) - m_client_rect.size.width) pos++;
				break;
			case SB_PAGELEFT:
				pos -= si.nPage;
				if (pos < m_canvas_rect.left) pos = m_canvas_rect.left;
				break;
			case SB_PAGERIGHT:
				pos += si.nPage;
				if (pos > m_canvas_rect.right) pos = m_canvas_rect.right;
				break;
			case SB_THUMBPOSITION:
				pos = si.nTrackPos;
				break;
			case SB_THUMBTRACK:
				pos = si.nTrackPos;
				break;
		}

		int dx = (pos - si.nPos);
		m_canvas_drift.x += dx;
		ScrollWindowEx(m_hWindow, -dx, 0,
			(CONST RECT*)NULL,
			(CONST RECT*)NULL,
			(HRGN)NULL, (LPRECT)NULL,
			SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
		UpdateWindow(m_hWindow);

		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		si.nPos = pos;

		SetScrollInfo(m_hWindow, SB_HORZ, &si, TRUE);
	}
	void ScrollableWindow::AdjustCanvasDrift()
	{
		Point deltaXY(0, 0);
		if (m_canvas_rect.right - m_canvas_drift.x < m_client_rect.size.width)
		{
			deltaXY.x = std::min(m_canvas_drift.x - m_canvas_rect.left, m_client_rect.size.width - (m_canvas_rect.right - m_canvas_drift.x));
		}
		if (m_canvas_rect.bottom - m_canvas_drift.y < m_client_rect.size.height)
		{
			deltaXY.y = std::min(m_canvas_drift.y - m_canvas_rect.top, m_client_rect.size.height - (m_canvas_rect.bottom - m_canvas_drift.y));
		}

		if (deltaXY.y != 0 || deltaXY.x != 0)
		{
			m_canvas_drift -= deltaXY;
			ScrollWindowEx(m_hWindow, deltaXY.x, deltaXY.y,
				(CONST RECT*)NULL,
				(CONST RECT*)NULL,
				(HRGN)NULL, (LPRECT)NULL,
				SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE);
			UpdateWindow(m_hWindow);
		}
	}
	void ScrollableWindow::UpdateScrollingInfo()
	{
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(SCROLLINFO);

		// vertical scroll
		si.nMin = m_canvas_rect.top;
		si.nMax = m_canvas_rect.bottom;
		si.nPage = m_client_rect.size.height;
		si.nPos = m_canvas_drift.y;
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		SetScrollInfo(m_hWindow, SB_VERT, &si, TRUE);

		// horizontal scroll
		si.nMin = m_canvas_rect.left;
		si.nMax = m_canvas_rect.right;
		si.nPage = m_client_rect.size.width;
		si.nPos = m_canvas_drift.x;
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		SetScrollInfo(m_hWindow, SB_HORZ, &si, TRUE);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}