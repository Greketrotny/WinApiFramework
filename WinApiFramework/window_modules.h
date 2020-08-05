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
		std::function<LRESULT(
			HWND hWnd, 
			UINT msg, 
			WPARAM wParam, LPARAM lParam)> m_window_procedure;

	protected:
		HasWindowProcedure(
			Derived* object, 
			LRESULT(Derived::*WndProc)(
				HWND hWnd, 
				UINT msg, 
				WPARAM wParam, LPARAM lParam))
		{
			m_window_procedure = std::bind(WndProc, object, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		}
		~HasWindowProcedure() {}

	protected:
		virtual LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] HasSubclassProcedure ~~~~~~~~
	template <class Derived> class HasSubclassProcedure
	{
	protected:
		std::function<LRESULT(
			HWND hWnd,
			UINT msg,
			WPARAM wParam, LPARAM lParam,
			UINT_PTR idSubClass, DWORD_PTR refData)> m_subclass_procedure;

	protected:
		HasSubclassProcedure(
			Derived* object, 
			LRESULT(Derived::*SubProc)(
				HWND hWnd,
				UINT msg,
				WPARAM wParam, LPARAM lParam,
				UINT_PTR idSubClass, DWORD_PTR refData))
		{
			m_subclass_procedure = std::bind(SubProc, object,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
				std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
		}
		~HasSubclassProcedure() {}

	protected:
		virtual LRESULT SubclassProcedure(
			HWND hWnd, 
			UINT msg, 
			WPARAM wParam, LPARAM lParam, 
			UINT_PTR idSubClass, DWORD_PTR refData) = 0;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !WINDOW_MODULES_H