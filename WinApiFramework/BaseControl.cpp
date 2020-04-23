#include "BaseControl.h"

namespace WinApiFramework
{
	// ~~~~~~~~ [STRUCT] BaseControl ~~~~~~~~
	// ~~ BaseControl::constructor
	BaseControl::BaseControl(const BaseControl::ConStruct& conStruct, ParentControl* parentControl)
		: m_pParentControl(parentControl)
	{
		SetRect(conStruct.rect);
	}
	BaseControl::~BaseControl()
	{
	}

	// ~~ BaseControl::methods
	HWND BaseControl::GetControlHandle() const
	{
		return m_hControl;
	}
	void BaseControl::Enable()
	{
		::EnableWindow(m_hWindow, TRUE);
		PushBaseEvent(BaseControl::Event(BaseControl::Event::Type::Enable));
	}
	void BaseControl::Disable()
	{
		::EnableWindow(m_hWindow, FALSE);
		PushBaseEvent(BaseControl::Event(BaseControl::Event::Type::Disable));
	}

	void BaseControl::Move(int x, int y)
	{
		m_rect.x = x;
		m_rect.y = y;

		SetWindowPos(this->m_hWindow, nullptr,
					 m_rect.x, m_rect.y,
					 m_rect.width, m_rect.height,
					 SWP_NOSIZE);

		PushBaseEvent(BaseControl::Event(BaseControl::Event::Type::Move));
	}
	void BaseControl::Resize(int width, int height)
	{
		m_rect.width = width;
		m_rect.height = height;

		if (m_rect.width < 0) m_rect.width = 0;
		if (m_rect.height < 0) m_rect.height = 0;

		SetWindowPos(m_hWindow, nullptr,
					 m_rect.x, m_rect.y,
					 m_rect.width, m_rect.height,
					 SWP_NOMOVE);

		PushBaseEvent(BaseControl::Event(BaseControl::Event::Type::Resize));
	}
	void BaseControl::SetRect(Rect rect)
	{
		m_rect = rect;

		if (m_rect.width < 0) m_rect.width = 0;
		if (m_rect.height < 0) m_rect.height = 0;

		SetWindowPos(m_hWindow, nullptr,
					 m_rect.x, m_rect.y,
					 m_rect.width, m_rect.height, 0);

		PushBaseEvent(BaseControl::Event(BaseControl::Event::Type::Move));
		PushBaseEvent(BaseControl::Event(BaseControl::Event::Type::Resize));
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	


	// ~~~~~~~~ [CLASS] ParentControl ~~~~~~~~
	// ~~ ParentControl::constructor
	ParentControl::ParentControl()
	{
	}
	ParentControl::~ParentControl()
	{
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}