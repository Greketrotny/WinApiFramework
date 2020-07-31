#ifndef BASE_WINDOW_H
#define BASE_WINDOW_H

#include "window_include.h"
#include "window_modules.h"
#include "data_types.h"
#include "event.h"

#include <queue>
#include <vector>
#include <functional>

namespace WinapiFramework
{
	class BaseWindow;
	class ParentWindow;

	// ~~~~~~~~ [CLASS] BaseWindow ~~~~~~~~
	class BaseWindow : public EventHandler
	{
	protected:
		ParentWindow* mp_parent;
		HWND m_hWindow;
		DWORD m_window_style;
		std::wstring m_window_class_name;
		Rect m_window_rect;


	protected:
		BaseWindow(const BaseWindow& other) = delete;
		BaseWindow(BaseWindow&& other) = delete;
		explicit BaseWindow(ParentWindow* parent);
	public:
		virtual ~BaseWindow();


	protected:
		virtual LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam);
		virtual bool CreateWinapiWindow() = 0;
		virtual void DestroyWinapiWindow() = 0;
	public:
		virtual void Destroy();
		virtual void Enable();
		virtual void Disable();

		virtual void Move(int x, int y);
		void Move(const Point& position);
		virtual void Resize(int width, int height);
		void Resize(const Size& size);
		void SetRect(const Rect& rect);

		virtual Point GetMousePosition() const;
		bool IsMouseInside() const;

	public:
		const HWND& GetWindowHandle() const;
		const std::wstring& GetWindowClassName() const;
		const Rect& GetWindowRect() const;
	protected:
		ParentWindow* const GetParent() const;
		WNDPROC GetFrameworkProcedure() const;
		void AppendAction(BaseAction* action) const;


		friend class ParentWindow;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentWindow ~~~~~~~~
	class ParentWindow
		: public BaseWindow
		, public Scrollable
		, public ObjectCreator
	{
	private:
		std::vector<BaseWindow*> m_children;


	protected:
		ParentWindow(const ParentWindow& other) = delete;
		ParentWindow(ParentWindow&& other) = delete;
		ParentWindow(ParentWindow* parent);
		virtual ~ParentWindow();


	public:
		template <class T> T* CreateChild(const ConStruct<T>& conStruct)
		{
			// create child
			T* new_child = ObjectCreator::CreateObject<T>(this, conStruct);

			// add child to m_children
			m_children.push_back(new_child);

			// adjust canvas rect
			//AdjustCanvasRect(m_children);
			//SendMessage(m_hWindow, WM_SIZE, 0, 0);

			return new_child;
		}
		bool DestroyChild(BaseWindow* child);
		void DestroyAllChildren();
	public:
		virtual Point GetMousePosition() const = 0;
	protected:
		LRESULT ProcessChildMessage(WPARAM wParam, LPARAM lParam);
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !BASE_WINDOW_H