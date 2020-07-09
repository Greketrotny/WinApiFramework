#include "Precompiled.h"

#include "BaseControl.h"

namespace WinApiFramework
{
	// ~~~~~~~~ [CLASS] BaseControl ~~~~~~~~
	// ~~ BaseControl::constructor ~~
	BaseControl::BaseControl()
	{
	}
	BaseControl::~BaseControl()
	{
	}

	// ~~ BaseControl::methods ~~
	const HWND& BaseControl::GetWindowHandle() const
	{
		return m_hWindow;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ChildControl ~~~~~~~~
	// ~~ ChildControl::constructor ~~ //
	ChildControl::ChildControl(ParentControl* const parentControl, const ConStruct<ChildControl>& conStruct)
		: Rectangle(m_rect)
		, m_pParentControl(parentControl)
	{
		m_rect = conStruct.rect;
	}
	ChildControl::~ChildControl()
	{
	}

	// ~~ Childcontrol::methods ~~ //
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
			m_rect.position.x + m_pParentControl->GetCanvasRect().position.x, 
			m_rect.position.y + m_pParentControl->GetCanvasRect().position.y,
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
			m_rect.position.x + m_pParentControl->GetCanvasRect().position.x, 
			m_rect.position.y + m_pParentControl->GetCanvasRect().position.y,
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
			m_rect.position.x + m_pParentControl->GetCanvasRect().position.x, 
			m_rect.position.y + m_pParentControl->GetCanvasRect().position.y,
			m_rect.size.width, m_rect.size.height, 0);

		PushBaseEvent(ChildControl::Event(ChildControl::Event::Type::Move));
		PushBaseEvent(ChildControl::Event(ChildControl::Event::Type::Resize));
	}

	Point ChildControl::GetMousePosition() const
	{
		return m_pParentControl->GetMousePosition() - this->m_rect.position;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentControl ~~~~~~~~ //
	// ~~ ParentControl::constructor ~~ //
	ParentControl::ParentControl()
	{
	}
	ParentControl::~ParentControl()
	{
		DestroyAllChildControls();
	}

	// ~~ ParentControl::methods ~~ //
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
	ProcedureResult ParentControl::ProcessChildMessage(WPARAM wParam, LPARAM lParam)
	{
		for (ChildControl* control : m_controls)
		{
			ProcedureResult pr = control->ControlProcedure(wParam, lParam);
			if (pr != ProcedureResult::TargetNotFound) return pr;
		}
		return ProcedureResult::TargetNotFound;
	}
	/*ChildControl* const ParentControl::GetChildControl(size_t index)
	{
		return m_controls[index];
	}*/
	/*size_t ParentControl::GetControlCount()
	{
		return m_controls.size();
	}*/
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //



	// ~~~~~~~~ [CLASS] Scrollable ~~~~~~~~
	// ~~ Scrollable::constructor ~~ //
	Scrollable::Scrollable()
		: m_canvasRect(0, 0, 800, 600)
	{
	}
	Scrollable::~Scrollable()
	{
	}

	// ~~ Scrollable::methods ~~ //
	Rect Scrollable::GetCanvasRect() const
	{
		return m_canvasRect;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}