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
		: public ParentControl
		, public HasWindowProcedure<Window>
		, public EventHandler
	{
		// -- fields -- //
	private:
		std::wstring caption;
		LONG windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_BORDER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
		unsigned int window_id;
		bool isMainWindow = false;
		bool isEnabled = true;
		bool isActivated = true;
		bool isMinimized = false;
		bool mouseOnWindow = false;

	public:
		enum Position
		{
			Center,
			Custom,
			Default
		};
		enum StartStyle
		{
			Minimized,
			Maximized,
			Normal
		};

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
		Rect windowRect;
		Rect clientRect;
		SizeRect sizeRect;


	private:
		Window();
		Window(const Window &wnd) = delete;
		Window(const Window &&wnd) = delete;
		Window(unsigned int id, const ConStruct<Window> &conStruct);
		~Window();


	public:
		Window& operator=(const Window &wnd) = delete;
		Window& operator=(const Window &&wnd) = delete;


	private:
		LRESULT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		//LRESULT ProcessChildMessage(WPARAM wParam, LPARAM lParam);
		bool CreateWinApiWindow(const ConStruct<Window>& config);
	public:
		void Destroy();
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

		void SetCaption(std::wstring new_caption);
		void SetPosition(unsigned int x, unsigned int y);
		void SetPosition(const Point& position);
		void Resize(unsigned int width, unsigned int height);
		void Resize(const Size& size);
		void SetMinSize(unsigned int minWidth, unsigned int minHeight);
		void SetMinSize(const Size& size);
		void SetMaxSize(unsigned int maxWidth, unsigned int maxHeight);
		void SetMaxSize(const Size& size);
		void SetSizeRect(SizeRect newSizeRect);
		void SetAsMainWindow();

		void Enable();
		void Disable();
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


	// -- property fields -- //
	public:
		const HWND& WndHandle;
		const bool& IsMainWindow;
		const bool& IsEnabled;
		const bool& IsActivated;
		const bool& IsMinimized;
		const unsigned int& Id;
		const Rect& WindowRect;
		const Rect& ClientRect;
		const SizeRect& WindowSizeRect;
		const std::wstring& Caption;
		//EventsManager& Events;


		// -- friends -- //
		friend Framework;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	template <> struct ConStruct<Window>
	{
		// ~~ ConStruct<Window>::fields ~~
	public:
		std::wstring caption;
		Rect rect;
		Window::Position position;
		Window::StartStyle startStyle;
		SizeRect sizeRect;
		Size canvasSize;


		// ~~ ConStruct<Window>::constructor ~~
	public:
		ConStruct(
			const std::wstring& caption = L"default title",
			const Rect rect = Rect(),
			Window::Position position = Window::Position::Center,
			Window::StartStyle startStyle = Window::StartStyle::Normal,
			SizeRect sizeRect = SizeRect(),
			Size canvasSize = Size(800, 600))
			: caption(caption)
			, rect(rect)
			, position(position)
			, startStyle(startStyle)
			, sizeRect(sizeRect)
			, canvasSize(canvasSize)
		{}
		~ConStruct()
		{}
	};
}

#endif // !WIN_API_WINDOW