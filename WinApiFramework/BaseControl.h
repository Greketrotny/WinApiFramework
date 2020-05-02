#ifndef BASE_CONTROL_H
#define BASE_CONTROL_H

#include "WindowInclude.h"
#include "ExternIncludes.h"
#include "DataTypes.h"

namespace WinApiFramework
{
	class ParentControl;

	// ~~~~~~~~ [CLASS] BaseControl ~~~~~~~~
	class BaseControl
	{
		// ~~ Basecontrol::fields ~~ //
	protected:
		HWND m_hWindow;


		// ~~ BaseControl::constructor ~~ //
	public:
		explicit BaseControl();
		virtual ~BaseControl();


		// ~~ BaseControl::methods -- //
	public:
		HWND GetWindowHandle() const;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ChildControl ~~~~~~~~
	class ChildControl : virtual public BaseControl
	{
		// ~~ ChildControl::fields ~~ //
	protected:
		ParentControl* const m_pParentControl;

		// ~~ ChildControl::constructor ~~ //
	public:
		ChildControl(ParentControl* const parentControl);
		virtual ~ChildControl();


		// ~~ ChildControl::methods ~~ //
	public:
		virtual int ControlProcedure(WPARAM wParam, LPARAM lParam) = 0;


		// ~~ ChildControl::friends ~~ //
	public:
		friend class ParentControl;
		friend class ControlsStorage;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentControl ~~~~~~~~
	class ParentControl : virtual public BaseControl
	{
		// ~~ ParentControl::constructor ~~ //
	public:
		ParentControl();
		virtual ~ParentControl();


		// ~~ ParentControl::methods ~~ //
	protected:
		virtual void AddControl(ChildControl* childControl) = 0;
		virtual void RemoveControl(ChildControl* childControl) = 0;
	public:
		virtual Point GetMousePosition() const = 0;

		
		// ~~ ParentControl::friends ~~ //
	public:
		friend class ChildControl;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !STORAGE_CONTROL_H