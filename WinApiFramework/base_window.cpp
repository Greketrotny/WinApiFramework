#include "base_window.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	// ~~~~~~~~ [CLASS] BaseWindow ~~~~~~~~
	BaseWindow::BaseWindow()
	{
	}
	BaseWindow::~BaseWindow()
	{
	}
		
	const HWND& BaseWindow::GetWindowHandle() const
	{
		return m_hWindow;
	}
	const std::wstring& BaseWindow::GetWindowClassName() const
	{
		return m_WindowClassName;
	}

	WNDPROC BaseWindow::GetWinApiProcedure() const
	{
		return (WNDPROC)Framework::WinApiProcedure;
	}
	void BaseWindow::AppendAction(BaseAction* action) const
	{
		Framework::AppendAction(action);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ChildControl ~~~~~~~~
	ChildControl::ChildControl(ParentControl* const parentControl, const ConStruct<ChildControl>& conStruct)
		: Rectangle(m_rect)
		, m_pParentControl(parentControl)
	{
		m_rect = conStruct.rect;
	}
	ChildControl::~ChildControl()
	{
	}
	
	void ChildControl::Destroy()
	{
		m_pParentControl->DestroyControl(this);
	}
	void ChildControl::Enable()
	{
		::EnableWindow(m_hWindow, TRUE);
		PushBaseEvent(ChildControl::Event(ChildControl::Event::Type::Enable));
	}
	void ChildControl::Disable()
	{
		::EnableWindow(m_hWindow, FALSE);
		PushBaseEvent(ChildControl::Event(ChildControl::Event::Type::Disable));
	}

	void ChildControl::Move(int x, int y)
	{
		m_rect.position.x = x;
		m_rect.position.y = y;

		SetWindowPos(this->m_hWindow, nullptr,
			m_rect.position.x - m_pParentControl->GetCanvasPosition().x, 
			m_rect.position.y - m_pParentControl->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			SWP_NOSIZE);

		PushBaseEvent(ChildControl::Event(ChildControl::Event::Type::Move));
	}
	void ChildControl::Move(const Point& position)
	{
		Move(position.x, position.y);
	}
	void ChildControl::Resize(int width, int height)
	{
		m_rect.size.width = width;
		m_rect.size.height = height;

		if (m_rect.size.width < 0) m_rect.size.width = 0;
		if (m_rect.size.height < 0) m_rect.size.height = 0;

		SetWindowPos(m_hWindow, nullptr,
			m_rect.position.x - m_pParentControl->GetCanvasPosition().x,
			m_rect.position.y - m_pParentControl->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			SWP_NOMOVE);

		PushBaseEvent(ChildControl::Event(ChildControl::Event::Type::Resize));
	}
	void ChildControl::Resize(const Size& size)
	{
		Resize(size.width, size.height);
	}
	void ChildControl::SetRect(const Rect& rect)
	{
		m_rect = rect;

		if (m_rect.size.width < 0) m_rect.size.width = 0;
		if (m_rect.size.height < 0) m_rect.size.height = 0;

		SetWindowPos(m_hWindow, nullptr,
			m_rect.position.x - m_pParentControl->GetCanvasPosition().x,
			m_rect.position.y - m_pParentControl->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height, 0);

		PushBaseEvent(ChildControl::Event(ChildControl::Event::Type::Move));
		PushBaseEvent(ChildControl::Event(ChildControl::Event::Type::Resize));
	}

	const Rect& ChildControl::GetRect()
	{
		return m_rect;
	}
	Point ChildControl::GetMousePosition() const
	{
		return m_pParentControl->GetMousePosition() - this->m_rect.position;
	}
	bool ChildControl::IsMouseInside() const
	{
		Point mouse = GetMousePosition();
		return (mouse.x >= 0 
			&& mouse.y >= 0 
			&& mouse.x < m_rect.size.width 
			&& mouse.y < m_rect.size.height);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentControl ~~~~~~~~ //
	ParentControl::ParentControl()
	{
	}
	ParentControl::~ParentControl()
	{
		DestroyAllChildControls();
	}
	
	bool ParentControl::DestroyControl(ChildControl* control)
	{
		if (control == nullptr) return false;

		for (size_t i = 0; i < m_controls.size(); ++i)
		{
			if (m_controls[i] == control)
			{
				ControlCreator::DestroyControl(control);
				m_controls.erase(m_controls.begin() + i);
				return true;
			}
		}

		return false;
	}
	void ParentControl::DestroyAllChildControls()
	{
		for (size_t i = 0; i < m_controls.size(); ++i)
		{
			ControlCreator::DestroyControl(m_controls[i]);
		}
		m_controls.clear();
	}
	LRESULT ParentControl::ProcessChildMessage(WPARAM wParam, LPARAM lParam)
	{
		for (ChildControl*& control : m_controls)
		{
			if (control->GetWindowHandle() == (HWND)lParam)
			{
				control->ControlProcedure(wParam, lParam);
				return 0;
			}
		}
		return 1;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //



	// ~~~~~~~~ [CLASS] Scrollable ~~~~~~~~
	Scrollable::Scrollable()
		: m_canvasRect(0, 0, 800, 600)
	{
	}
	Scrollable::~Scrollable()
	{
	}

	BoundRect Scrollable::GetCanvasRect() const
	{
		return m_canvasRect;
	}
	Point Scrollable::GetCanvasPosition() const
	{
		return m_canvasDrift;
	}

	void Scrollable::AdjustCanvasRect(const std::vector<ChildControl*> controls)
	{
		BoundRect boundRect(std::numeric_limits<int>::max(), std::numeric_limits<int>::max(),
			std::numeric_limits<int>::min(), std::numeric_limits<int>::min());

		for (ChildControl* control : controls)
		{
			if (control == nullptr) continue;

			// left
			if (control->Rectangle.position.x < boundRect.left)
				boundRect.left = control->Rectangle.position.x;

			// top
			if (control->Rectangle.position.y < boundRect.top)
				boundRect.top = control->Rectangle.position.y;

			// right
			if (control->Rectangle.position.x + control->Rectangle.size.width > boundRect.right)
				boundRect.right = control->Rectangle.position.x + control->Rectangle.size.width;

			// bottom
			if (control->Rectangle.position.y + control->Rectangle.size.height > boundRect.bottom)
				boundRect.bottom = control->Rectangle.position.y + control->Rectangle.size.height;
		}

		m_canvasRect = boundRect;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}