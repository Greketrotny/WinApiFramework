#ifndef WIN_API_WINDOW
#define WIN_API_WINDOW

#include "WinApiFramework.h"
#include "WindowControl.h"

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
			unsigned int maxWidth = 0x0FFFFFFF, maxHeight = 0x0FFFFFFF;
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
				switch (event.type)
				{
				case Window::Event::Type::Move:					Move();					break;
				case Window::Event::Type::Resize:				Resize();				break;
				case Window::Event::Type::Enable:				Enable();				break;
				case Window::Event::Type::Disable:				Disable();				break;
				case Window::Event::Type::Activate:				Activate();				break;
				case Window::Event::Type::Deactivate:			Deactivate();			break;
				case Window::Event::Type::Minimize:				Minimize();				break;
				case Window::Event::Type::Maximize:				Maximize();				break;
				case Window::Event::Type::Show:					Show();					break;
				case Window::Event::Type::Hide:					Hide();					break;
				case Window::Event::Type::EnableResize:			EnableResize();			break;
				case Window::Event::Type::DisableResize:		DisableResize();		break;
				case Window::Event::Type::EnableMaximizeBox:	EnableMaximizeBox();	break;
				case Window::Event::Type::DisableMaximizeBox:	DisableMaximizeBox();	break;
				case Window::Event::Type::EnableMinimizeBox:	EnableMinimizeBox();	break;
				case Window::Event::Type::DisableMinimizeBox:	DisableMinimizeBox();	break;
				case Window::Event::Type::ControlAdd:			ControlAdd();			break;
				case Window::Event::Type::ControlRemove:		ControlRemove();		break;
				case Window::Event::Type::CaptionChange:		CaptionChange();		break;
				case Window::Event::Type::Close:				Close();				break;
				}
			}
			virtual void Move() {};
			virtual void Resize() {};
			virtual void Enable() {};
			virtual void Disable() {};
			virtual void Activate() {};
			virtual void Deactivate() {};
			virtual void Minimize() {};
			virtual void Maximize() {};
			virtual void Show() {};
			virtual void Hide() {};
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
			std::wstring caption = L"default title";
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
		Rect windowRect;
		Rect clientRect;
		SizeRect sizeRect;
		Events events;
		ControlsStorage controls;


		// -- constructors -- //
	private:
		Window();
	public:
		Window(const Window &wnd) = delete;
		Window(const Window &&wnd) = delete;
		Window(const Config &config);
		Window(const Config &config, EventHandler *eventHandler);
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
		void SetEventHandler(Window::EventHandler *eventHandler);
		int ShowMessageBox
		(
			std::wstring text = L"default text",
			std::wstring caption = L"Default caption",
			UINT message_box_style = 0
		);

		int GetWindowX() const;
		int GetWindowY() const;
		int GetClientX() const;
		int GetClientY() const;
		unsigned int GetWindowWidth() const;
		unsigned int GetWindowHeight() const;
		unsigned int GetClientWidth() const;
		unsigned int GetClientHeight() const;
		const HWND& GetWindowHandle() const;
		const std::wstring& GetCaption() const;
		int GetMouseX() const;
		int GetMouseY() const;
		int GetClientMouseX() const;
		int GetClientMouseY() const;

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
		const int& WindowX;
		const int& WindowY;
		const int& ClientX;
		const int& ClientY;
		const unsigned int& WindowWidth;
		const unsigned int& WindowHeight;
		const unsigned int& ClientWidth;
		const unsigned int& ClientHeight;
		const Rect& WindowRect;
		const Rect& ClientRect;
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