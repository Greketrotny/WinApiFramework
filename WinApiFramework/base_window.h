#ifndef BASE_WINDOW_H
#define BASE_WINDOW_H

#include "window_include.h"
#include "window_modules.h"
#include "data_types.h"
#include "event.h"
#include "mouse.h"

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
		Rect m_rect;
	protected:
		struct BaseWindowEvents
		{
			struct EventEnable : public BaseEvent {};
			struct EventDisable : public BaseEvent {};
			struct EventMove : public BaseEvent {};
			struct EventResize : public BaseEvent {};

			struct EventMouseLButtonPress : public BaseEvent {};
			struct EventMouseLButtonRelease : public BaseEvent {};
			struct EventMouseLButtonDPress : public BaseEvent {};
			struct EventMouseRButtonPress : public BaseEvent {};
			struct EventMouseRButtonRelease : public BaseEvent {};
			struct EventMouseRButtonDPress : public BaseEvent {};
			struct EventMouseMButtonPress : public BaseEvent {};
			struct EventMouseMButtonRelease : public BaseEvent {};
			struct EventMouseMButtonDPress : public BaseEvent {};
			struct EventMouseMove : public BaseEvent 
			{
				Point mouse_pos;

				EventMouseMove(const Point& pos)
					: mouse_pos(pos)
				{}
			};
			struct EventMouseHover : public BaseEvent {};
			struct EventMouseLeave : public BaseEvent {};

		};

		struct MouseCaptor
		{
		private:
			bool is_tracking_mouse;
			bool is_capturing_mouse;
			bool want_capturing;
			Mouse::MouseButton capturing_button;

		public:
			MouseCaptor()
				: is_tracking_mouse(false)
				, is_capturing_mouse(false)
				, want_capturing(false)
				, capturing_button(Mouse::MouseButton::Left)
			{}
			~MouseCaptor()
			{
				StopMouseCapture(capturing_button);
			}

		public:
			void StartMouseCapture(Mouse::MouseButton mbutton)
			{
				if (!is_capturing_mouse)
				{
					want_capturing = true;
					capturing_button = mbutton;
				}
			}
			void StopMouseCapture(Mouse::MouseButton mbutton)
			{
				want_capturing = false;

				if (is_capturing_mouse && mbutton == capturing_button)
				{
					ReleaseCapture();
					is_capturing_mouse = false;
				}
			}
			void CheckCaptureOnLeave(HWND hwnd)
			{
				if (want_capturing)
				{
					SetCapture(hwnd);
					is_capturing_mouse = true;
				}
			}

			void StartTrackingMouse(HWND hwnd)
			{
				if (!is_tracking_mouse)
				{
					TRACKMOUSEEVENT tme;
					tme.cbSize = sizeof(tme);
					tme.hwndTrack = hwnd;
					tme.dwHoverTime = 1000;
					tme.dwFlags = TME_LEAVE;
					TrackMouseEvent(&tme);

					is_tracking_mouse = true;
				}
			}
			void StopTrackingMouse()
			{
				is_tracking_mouse = false;
			}
			bool IsTrackingMouse()
			{
				return is_tracking_mouse;
			}
		} m_mouse_captor;


	protected:
		BaseWindow(const BaseWindow& other) = delete;
		BaseWindow(BaseWindow&& other) = delete;
		explicit BaseWindow(ParentWindow* parent);
	public:
		virtual ~BaseWindow();


	protected:
		virtual LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam);
		virtual LRESULT NotifyProcedure(WPARAM wParam, LPARAM lParam);
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
	protected:
		void DoDestroy();
		void DoEnable();
		void DoDisable();
		void DoMove(int x, int y);
		void DoResize(int width, int height);

		LRESULT	HandleMouseEvent(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		virtual Point GetMousePosition() const;
		bool IsMouseInside() const;

	public:
		const HWND& GetWindowHandle() const;
		const std::wstring& GetWindowClassName() const;
		const Rect& GetWindowRect() const;
	protected:
		ParentWindow* const GetParent() const;
		WNDPROC GetFrameworkProcedure() const;
		SUBCLASSPROC GetSubclassProcedure() const;
		void AppendAction(BaseAction* action) const;


		friend class ParentWindow;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentWindow ~~~~~~~~
	class ParentWindow
		: public BaseWindow
		, public ObjectCreator
	{
	protected:
		Rect m_client_rect;
	private:
		std::vector<BaseWindow*> m_children;

	protected:
		struct ParentWindowEvents : public BaseWindowEvents
		{
			struct EventCreateChild : public BaseEvent {};
			struct EventDestroyChild : public BaseEvent {};
			struct EventDestroyAllChildren : public BaseEvent {};
		};


	protected:
		ParentWindow(const ParentWindow& other) = delete;
		ParentWindow(ParentWindow&& other) = delete;
		ParentWindow(ParentWindow* parent);
		virtual ~ParentWindow();


	public:
		virtual LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
		template <class T> T* CreateChild(const ConStruct<T>& conStruct)
		{
			// create child
			T* new_child = ObjectCreator::CreateObject<T>(this, conStruct);

			// add child to m_children
			m_children.push_back(new_child);

			// adjust canvas rect
			AdjustCanvasRect();
			//SendMessage(m_hWindow, WM_SIZE, 0, 0);

			return new_child;
		}
		bool DestroyChild(BaseWindow* child);
		void DestroyAllChildren();
		BaseWindow* GetChild(size_t index);
		size_t GetChildrenCount();

		virtual void AdjustCanvasRect();

		virtual Point GetCanvasPosition() const;
		const Rect& GetClientRect() const;
	protected:
		LRESULT ProcessChildMessage(WPARAM wParam, LPARAM lParam);
		LRESULT ProcessChildNotify(WPARAM wParam, LPARAM lParam);
		const std::vector<BaseWindow*>& GetChildren();
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] Scrollable ~~~~~~~~
	class ScrollableWindow : public ParentWindow
	{
	private:
		BoundRect m_canvas_rect;
		Point m_canvas_drift;


	protected:
		ScrollableWindow(const ScrollableWindow& other) = delete;
		ScrollableWindow(ScrollableWindow&& other) = delete;
		ScrollableWindow(ParentWindow* parent);
		~ScrollableWindow();


	public:
		BoundRect GetCanvasRect() const;
		Point GetCanvasDrift() const;
		Point GetCanvasPosition() const override;
	protected:
		void AdjustCanvasRect() override;

		void HandleVScroll(WPARAM wParam, LPARAM lParam);
		void HandleHScroll(WPARAM wParam, LPARAM lParam);
		void AdjustCanvasDrift();
		void UpdateScrollingInfo();
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !BASE_WINDOW_H