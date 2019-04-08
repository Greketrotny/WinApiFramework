#ifndef WIN_API_WINDOW
#define WIN_API_WINDOW

#include "WindowInclude.h"

//#include "Mouse.h"
//#include "Keyboard.h"
#include "WinApiWindowControls.h"

#include <string>
#include <vector>


namespace WinApiFramework
{
	class Framework;

	class Window
	{
		// -- fields -- //
	private:
		HWND hWindow = NULL;
		std::wstring window_class_name = L"WindowClassName";
		std::wstring caption = L"Default caption";
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
		struct Rect
		{
			int x = 50, y = 50;
			unsigned int width = 800, height = 600;
		};
		struct SizeRect
		{
			unsigned int minWidth = 0, minHeight = 0;
			unsigned int maxWidth = 0xFFFFFFFF, maxHeight = 0xFFFFFFFF;
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
				EnableResize,
				DisableResize,
				EnableMaximizeBox,
				DisableMaximizeBox,
				EnableMinimizeBox,
				DisableMinimizeBox,
				ControlAdd,
				ControlRemove,
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
		struct EventHandler
		{
			virtual void HandleEvent(Window::Event event)
			{
				if (event.type == Window::Event::Type::Move)				Move();
				if (event.type == Window::Event::Type::Resize)				Resize();
				if (event.type == Window::Event::Type::Enable)				Enable();
				if (event.type == Window::Event::Type::Disable)				Disable();
				if (event.type == Window::Event::Type::Activate)			Activate();
				if (event.type == Window::Event::Type::Deactivate)			Deactivate();
				if (event.type == Window::Event::Type::EnableResize)		EnableResize();
				if (event.type == Window::Event::Type::DisableResize)		DisableResize();
				if (event.type == Window::Event::Type::EnableMaximizeBox)	EnableMaximizeBox();
				if (event.type == Window::Event::Type::DisableMaximizeBox)	DisableMaximizeBox();
				if (event.type == Window::Event::Type::EnableMinimizeBox)	EnableMinimizeBox();
				if (event.type == Window::Event::Type::DisableMinimizeBox)	DisableMinimizeBox();
				if (event.type == Window::Event::Type::ControlAdd)			ControlAdd();
				if (event.type == Window::Event::Type::ControlRemove)		ControlRemove();
				if (event.type == Window::Event::Type::CaptionChange)		CaptionChange();
				if (event.type == Window::Event::Type::Close)				Close();
			}
			virtual void Move() {};
			virtual void Resize() {};
			virtual void Enable() {};
			virtual void Disable() {};
			virtual void Activate() {};
			virtual void Deactivate() {};
			virtual void EnableResize() {};
			virtual void DisableResize() {};
			virtual void EnableMaximizeBox() {};
			virtual void DisableMaximizeBox() {};
			virtual void EnableMinimizeBox() {};
			virtual void DisableMinimizeBox() {};
			virtual void ControlAdd() {};
			virtual void ControlRemove() {};
			virtual void CaptionChange() {};
			virtual void Close() {};
		};
		struct Config
		{
			Position position = Position::Center;
			StartStyle startStyle = StartStyle::Normal;
			Rect rect;
			std::wstring caption = L"default";
			SizeRect sizeRect;
		};
	private:
		struct Events
		{
			// -- fields -- //
			std::queue<Event> events;
			const unsigned short buffLength = 32u;
			EventHandler* eventHandler = nullptr;

			// -- constructor -- //
			Events()
			{

			}
			~Events()
			{
				delete eventHandler;
				eventHandler = nullptr;
			}

			// -- methods -- //
			void PushEvent(Event newEvent)
			{
				// push event to buffer
				events.push(newEvent);
				if (events.size() > buffLength)
					events.pop();

				// call handler function
				if (eventHandler) eventHandler->HandleEvent(newEvent);
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
			void SetEventHandler(Window::EventHandler *eh)
			{
				eventHandler = eh;
			}
		};
		struct ControlsStorage
		{
			// -- fields -- //
		private:
			std::vector<WindowControl*> controls;

			// -- constructor -- //
		public:
			ControlsStorage();
			~ControlsStorage();

			// -- methods -- //
		public:
			void AddControl(WindowControl* newControl);
			void RemoveControl(WindowControl* oldControl);

			friend Window;
		};

	private:
		Position position;
		Rect rect;
		SizeRect sizeRect;
		Events events;
		ControlsStorage controls;


		// -- constructors -- //
	private:
		Window();
	public:
		Window(const Window &wnd) = delete;
		Window(const Window &&wnd) = delete;
		Window(Config config);
		~Window();


		// -- operators -- //
	public:
		Window& operator=(const Window &wnd) = delete;
		Window& operator=(const Window &&wnd) = delete;


		// -- methods -- // 
	private:
		LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
		bool CreateAndRegisterWindowClass();
		bool CreateWinApiWindow(Config config);
	public:
		void PushEvent(Event newEvent);
		Event GetEvent();
		void ClearEventBuffer();
		void SetCaption(std::wstring new_caption);
		void SetPosition(unsigned int x, unsigned int y);
		void SetDimensions(unsigned int width, unsigned int height);
		void SetRect(Config newWindowRect);
		void SetMinSize(unsigned int minWidth, unsigned int minHeight);
		void SetMaxSize(unsigned int maxWidth, unsigned int maxHeight);
		void SetSizeRect(SizeRect newSizeRect);
		void SetAsMainWindow();
		void EnableWindow();
		void DisableWindow();
		void EnableResize();
		void DisableResize();
		void EnableMaximizeBox();
		void DisableMaximizeBox();
		void EnableMinimizeBox();
		void DisableMinimizeBox();
		void ActivateWindow();
		void Maximize();
		void Minimize();
		void SetEventHandler(Window::EventHandler *eventHandler);
		int ShowMessageBox
		(
			std::wstring text = L"default text",
			std::wstring caption = L"Default caption",
			UINT message_box_style = 0
		);

		unsigned int GetX() const;
		unsigned int GetY() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		const HWND& GetWindowHandle() const;
		const std::wstring& GetCaption() const;
		int GetMouseX() const;
		int GetMouseY() const;

		void AddControl(WindowControl* newControl);
		void RemoveControl(WindowControl* oldControl);


		// -- property fields -- //
	public:
		const HWND& WndHandle;
		const bool& IsMainWindow;
		const bool& IsEnabled;
		const bool& IsActivated;
		const bool& IsMinimized;
		const unsigned int& Id;
		const int& X;
		const int& Y;
		const unsigned int& Width;
		const unsigned int& Height;
		const std::wstring& Caption;
		const unsigned int& MinWidth;
		const unsigned int& MinHeight;
		const unsigned int& MaxWidth;
		const unsigned int& MaxHeight;


		// -- friends -- //
		friend Framework;
	};
}

#endif // !WIN_API_WINDOW