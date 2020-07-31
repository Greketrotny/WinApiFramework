#ifndef WINDOW_H
#define WINDOW_H

#include "window_include.h"

#include "base_window.h"
#include "event.h"
#include "data_types.h"

#include <string>
#include <algorithm>

namespace WinapiFramework
{
	class Framework;

	class Window;
	template <> struct ConStruct<Window>;

	// ~~~~~~~~ [CLASS] Window ~~~~~~~~
	class Window
		: public ParentWindow
		, public HasWindowProcedure<Window>
	{
		// -- fields -- //
	private:
		std::wstring m_caption;
		unsigned int window_id;
		bool isMainWindow = false;
		bool isEnabled = true;
		bool isActivated = true;
		bool isMinimized = false;
		bool mouseOnWindow = false;

	public:		
		enum EventType
		{
			EventTypeInvalid = 0,

			EventTypeMoved = 1000,
			EventTypeResized,
			EventTypeEnabled,
			EventTypeDisabled,
			EventTypeActivated,
			EventTypeDeactivated,
			EventTypeShowed,
			EventTypeHid,
			EventTypeMaximized,
			EventTypeMinimized,
			EventTypeHScrolled,
			EventTypeVScrolled,
			EventTypeClose,
			EventTypeDestroying,

			EventTypeCaptionChanged,
			EventTypeMinSizeChanged,
			EventTypeMaxSizeChanged,
			EventTypeResizeEnabled,
			EventTypeResizeDisabled,
			EventTypeMaximizeBoxEnabled,
			EventTypeMaximizeBoxDisabled,
			EventTypeMinimizeBoxEnabled,
			EventTypeMinimizeBoxDisabled,
		};
		template <EventType T> struct Event : public BaseEvent
		{
		private:
			Window* m_pWindow;
		public:
			Event(const Event& event) = delete;
			Event( Window* window)
				: m_pWindow(window)
			{}

			Window* GetWindow() const
			{
				return m_pWindow;
			}
		};
		template<> struct Event<EventTypeClose> : public BaseEvent
		{
		private:
			Window* m_pWindow;
			bool closing_aborted;
		public:
			Event(const Event* event) = delete;
			Event(Window* window)
				: m_pWindow(window)
				, closing_aborted(false)
			{}
			~Event() {}
		public:
			void AfterHandling() override
			{
				if (!closing_aborted)
				{
					m_pWindow->Destroy();
				}
			}
			Window* GetWindow() const
			{
				return m_pWindow;
			}
			void AbortClosing()
			{
				closing_aborted = true;
			}
		};

		struct DestroyAction : public BaseAction
		{
		private:
			Window* m_pWindow;
		public:
			DestroyAction(Window* window);
			~DestroyAction();
		public:
			void Invoke() override;
		};

	private:
		Rect m_client_rect;
		Size m_min_size, m_max_size;


	private:
		Window();
		Window(const Window &wnd) = delete;
		Window(const Window &&wnd) = delete;
		Window(unsigned int id, const ConStruct<Window> &conStruct);
		~Window();


	public:
		Window& operator=(const Window &other) = delete;
		Window& operator=(const Window &&other) = delete;


	private:
		LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		//LRESULT ProcessChildMessage(WPARAM wParam, LPARAM lParam);
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	public:
		void Destroy() override;
		void Close();
	public:
		template <EventType ET> void RaiseEvent()
		{
			Event<ET> e(this);
			InvokeEvent(e);
		}
		template <EventType ET, class ER> void AddEventHandler(void(ER::*function)(Event<ET>&), ER* object)
		{
			BindEventFunc<Event<ET>, ER>(function, object);
		}
		template <EventType ET> void AddEventHandler(void(*function)(Event<ET>&))
		{
			BindEventFunc<Event<ET>>(function);
		}
		template <EventType ET, class ER> bool RemoveEventHandler(void(ER::*function)(Event<ET>&))
		{
			return UnbindEventFunc(function);
		}
		template <EventType ET> bool RemoveEventHandler(void(*function)(Event<ET>&))
		{
			return UnbindEventFunc(function);
		}

		void SetCaption(const std::wstring& new_caption);
		void Move(int x, int y) override;
		void Resize(int width, int height) override;
		void SetMinSize(unsigned int minWidth, unsigned int minHeight);
		void SetMinSize(const Size& size);
		void SetMaxSize(unsigned int maxWidth, unsigned int maxHeight);
		void SetMaxSize(const Size& size);
		void SetAsMainWindow();

		void EnableResize();
		void DisableResize();
		void EnableMaximizeBox();
		void DisableMaximizeBox();
		void EnableMinimizeBox();
		void DisableMinimizeBox();
		void Activate();
		void Maximize();
		void Minimize();
		void Show();
		void Hide();

		unsigned int GetId() const;

		MessBoxButtonPressed ShowMessageBox(
			const std::wstring& caption = L"default caption",
			const std::wstring& text = L"default text",
			MessBoxButtonLayout buttons = MessBoxButtonLayout::Ok,
			MessBoxIcon icon = MessBoxIcon::IconInformation);

		const std::wstring& GetCaption() const;
	private:
		Point GetMousePosition() const override;
	public:
		Point GetWindowMousePosition() const;
		Point GetClientMousePosition() const;
		Point GetCanvasMousePosition() const;

		
		friend Framework;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	template <> struct ConStruct<Window>
	{
	public:
		std::wstring caption;
		Rect rect;
		Size min_size;
		Size max_size;
		Size canvas_size;


	public:
		ConStruct(
			const std::wstring& caption = L"Window",
			const Rect rect = Rect(),
			const Size& min_size = Size(100, 100),
			const Size& max_size = Size(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()),
			const Size& canvasSize = Size(800, 600))
			: caption(caption)
			, rect(rect)
			, min_size(min_size)
			, max_size(max_size)
			, canvas_size(canvasSize)
		{}
		~ConStruct() {}
	};
}

#endif // !WIN_API_WINDOW