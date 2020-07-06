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
	HWND BaseControl::GetWindowHandle() const
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

	Point ChildControl::GetMousePosition()
	{
		return m_pParentControl->GetCanvasMousePosition() - this->m_rect.position;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentControl ~~~~~~~~
	// ~~ ParentControl::constructor ~~
	ParentControl::ParentControl()
	{
	}
	ParentControl::~ParentControl()
	{
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



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