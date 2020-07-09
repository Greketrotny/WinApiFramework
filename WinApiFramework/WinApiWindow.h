#ifndef WIN_API_WINDOW_H
#define WIN_API_WINDOW_H

#include "WindowInclude.h"
#include "ExternIncludes.h"

#include "BaseControl.h"

namespace WinApiFramework
{
	class Framework;

	class Window;
	template <> struct ConStruct<Window>;

	// ~~~~~~~~ [CLASS] Window ~~~~~~~~
	class Window : public ParentControl
	{
		// -- fields -- //
	private:
		std::wstring window_class_name;
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
		struct Event
		{
			enum Type
			{
				Move,
				Resize,
				Enable,
				Disable,
				Activate,
				Deactivate,
				Minimize,
				Maximize,
				Show,
				Hide,
				EnableResize,
				DisableResize,
				EnableMaximizeBox,
				DisableMaximizeBox,
				EnableMinimizeBox,
				DisableMinimizeBox,
				CaptionChange,
				Close,
				Invalid
			};
			Type type;

			Event()
			{
				type = Invalid;
			}
			Event(Type type)
			{
				this->type = type;
			}
		};
	private:
		struct EventsManager
		{
			// -- fields -- //
		private:
			const unsigned short buffLength = 32u;
			bool eventHandlersEnabled = true;
			std::queue<Event> events;
			std::vector<std::function<void(Window::Event)>> eventHandlers;


			// -- constructor -- //
		public:
			EventsManager()
			{
			}
			~EventsManager()
			{
			}


			// -- methods -- //
		public:
			void PushEvent(Event newEvent)
			{
				// push event to buffer
				events.push(newEvent);
				if (events.size() > buffLength)
					events.pop();

				// handle event
				if (eventHandlersEnabled)
				{
					for (unsigned int i = 0; i < eventHandlers.size(); ++i)
					{
						eventHandlers[i](newEvent);
					}
				}
			}
			Event GetEvent()
			{
				if (events.size() > 0u)
				{
					Event e = events.front();
					events.pop();
					return e;
				}
				else
				{
					return Event();
				}
			}
			void ClearBuffer()
			{
				events = std::queue<Event>();
			}
			template <class EventReceiver> void AddEventHandler(EventReceiver* receiverObject, void(EventReceiver::*eventFunction)(Window::Event))
			{
				using std::placeholders::_1;
				std::function<void(Window::Event)> f;
				f = std::bind(eventFunction, receiverObject, _1);
				eventHandlers.push_back(f);
			}
			void AddEventHandler(void(*eventFunction)(Window::Event))
			{
				if (eventFunction == nullptr) return;
				std::function<void(Window::Event)> f(eventFunction);
				eventHandlers.push_back(f);
			}
			void RemoveAllEventHandlers()
			{
				eventHandlers.clear();
			}
			void EnableEventHandlers()
			{
				eventHandlersEnabled = true;
			}
			void DisableEventHandlers()
			{
				eventHandlersEnabled = false;
			}
		};

	private:
		Rect windowRect;
		Rect clientRect;
		SizeRect sizeRect;
		EventsManager events;


		// -- constructors -- //
	private:
		Window();
		Window(const Window &wnd) = delete;
		Window(const Window &&wnd) = delete;
		Window(unsigned int id, const ConStruct<Window> &conStruct);
		~Window();


		// -- operators -- //
	public:
		Window& operator=(const Window &wnd) = delete;
		Window& operator=(const Window &&wnd) = delete;


		// -- methods -- // 
	private:
		ProcedureResult WndProcedure(UINT msg, WPARAM wParam, LPARAM lParam);
		//LRESULT ProcessChildMessage(WPARAM wParam, LPARAM lParam);
		bool CreateWinApiWindow(const ConStruct<Window>& config);
	public:
		void Destroy();
	public:
		void PushEvent(Window::Event newEvent);
		Window::Event GetEvent();
		void ClearEventBuffer();
		void EnableEventHandlers();
		void DisableEventHandlers();

		void SetCaption(std::wstring new_caption);
		void SetPosition(unsigned int x, unsigned int y);
		void SetDimensions(unsigned int width, unsigned int height);
		void SetMinSize(unsigned int minWidth, unsigned int minHeight);
		void SetMaxSize(unsigned int maxWidth, unsigned int maxHeight);
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
		template <class EventReceiver> void AddEventHandler(EventReceiver* receivingObject, void(EventReceiver::*eventFunction)(Window::Event))
		{
			events.AddEventHandler<EventReceiver>(receivingObject, eventFunction);
		}
		int ShowMessageBox(
			std::wstring text = L"default text",
			std::wstring caption = L"Default caption",
			UINT message_box_style = 0);

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
		EventsManager& Events;


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