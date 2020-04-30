#include "Precompiled.h"

#include "BaseControl.h"

namespace WinApiFramework
{
	// ~~~~~~~~ [CLASS] BaseControl ~~~~~~~~
	// ~~ BaseControl::constructor
	BaseControl::BaseControl()
	{
	}
	BaseControl::~BaseControl()
	{
	}

	// ~~ BaseControl::methods
	HWND BaseControl::GetWindowHandle() const
	{
		return m_hWindow;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ChildControl ~~~~~~~~
	// ~~ ChildControl::constructor
	ChildControl::ChildControl(ParentControl* const parentControl)
		: m_pParentControl(parentControl)
	{
		m_pParentControl->AddControl(this);
	}
	ChildControl::~ChildControl()
	{
		m_pParentControl->RemoveControl(this);
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