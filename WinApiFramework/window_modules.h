#ifndef WINDOW_MODULES_H
#define WINDOW_MODULES_H

#include "window_include.h"
#include "data_types.h"

#include <vector>
#include <functional>

namespace WinapiFramework
{
	class BaseWindow;
	class ParentWindow;

	// ~~~~~~~~ [CLASS] Scrollable ~~~~~~~~
	class Scrollable
	{
	protected:
		BoundRect m_canvasRect;
		Point m_canvasDrift;


	public:
		Scrollable();
		~Scrollable();


	public:
		BoundRect GetCanvasRect() const;
		Point GetCanvasPosition() const;
	protected:
		void AdjustCanvasRect(const std::vector<BaseWindow*> children);
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ObjectCreator ~~~~~~~~
	/*
	*	Purpose:
	*	1. For parent window to create and destroy children.
	*	2. For any other window or control which want to create and destroy
	*	   object beyond standard child container (for personal use).
	*/
	class ObjectCreator
	{
	protected:
		template <class T> static T* CreateObject(ParentWindow* const parent, const ConStruct<T>& conStruct)
		{
			return new T(parent, conStruct);
		}
		template <class T> static bool DestroyObject(T*& object)
		{
			if (object == nullptr) return false;

			delete object;
			object = nullptr;
			return true;
		}
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] HasWindowProcedure ~~~~~~~~
	/*
	*	Purpose:
	*	For every window or subclassed control to gain ability 
	*	to receive window-kind messages (WM_...) from main loop.
	*/
	template <class Derived> class HasWindowProcedure
	{
	protected:
		std::function<LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)> m_window_procedure;

	protected:
		HasWindowProcedure(Derived* object, LRESULT(Derived::*WndProc)(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam))
		{
			m_window_procedure = std::bind(WndProc, object, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		}
		~HasWindowProcedure()
		{
		}

	protected:
		virtual LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !WINDOW_MODULES_H