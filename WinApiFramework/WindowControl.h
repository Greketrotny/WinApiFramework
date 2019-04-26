#ifndef WINDOW_CONTROL_H
#define WINDOW_CONTROL_H

#include "WindowInclude.h"
#include <queue>

namespace WinApiFramework
{
	class Window;

	class WindowControl
	{
		// -- fields -- //
	protected:
		HWND hControl;
		Window* parentWindow = nullptr;
		DWORD controlStyle = WS_CHILD | WS_VISIBLE;
		unsigned int id;
	public:
		struct Rect
		{
			int x = 50, y = 50;
			unsigned int width = 100, height = 100;
		};
		struct Config
		{
			Rect rect;
		};
		struct Event
		{
			enum Type
			{
				Invalid = 0,
				Move = 1,
				Resize = 2,
				Enable = 3,
				Disable = 4
			};
			Type type;

			Event()
			{
				type = Event::Type::Invalid;
			}
			Event(Event::Type type)
			{
				this->type = type;
			}
		};
		template <class T> struct EventHandler
		{
		protected:
			void HandleBaseEvent(WindowControl::Event::Type eventType)
			{
				switch (eventType)
				{
				case WindowControl::Event::Move:	Move();		break;
				case WindowControl::Event::Resize:	Resize();	break;
				case WindowControl::Event::Enable:	Enable();	break;
				case WindowControl::Event::Disable:	Disable();	break;
				}
			}
		public:
			virtual void HandleEvent(T event) = 0;
			virtual void Move() {};
			virtual void Resize() {};
			virtual void Enable() {};
			virtual void Disable() {};
		};
		template <class T> struct Events
		{
		private:
			std::queue<T> events;
			const unsigned short buffSize = 32u;
			WindowControl::EventHandler<T> *eventHandler = nullptr;

		public:
			// -- constructor -- //
			Events()
			{

			}
			~Events()
			{
				if (eventHandler)
				{
					delete eventHandler;
					eventHandler = nullptr;
				}
			}

		public:
			// -- methods -- //
			void PushEvent(T newEvent)
			{
				// push event to buffer
				events.push(newEvent);
				if (events.size() > buffSize)
					events.pop();

				// call handler function
				if (eventHandler) eventHandler->HandleEvent(newEvent);
			}
			T GetEvent()
			{
				if (events.size() > 0u)
				{
					T e = events.front();
					events.pop();
					return e;
				}
				else
				{
					return T();
				}
			}
			void ClearBuffer()
			{
				events = std::queue<T>();
			}
			void SetEventHandler(WindowControl::EventHandler<T> *eventHandler)
			{
				this->eventHandler = eventHandler;
			}
		};
	protected:
		Rect rect;


		// -- constructors -- //
	protected:
		WindowControl(const WindowControl::Config& config);
		virtual ~WindowControl();


		// -- methods -- //
	protected:
		virtual int ControlProc(WPARAM wParam, LPARAM lParam) = 0;
		virtual bool CreateControlWindow() = 0;
		void DestroyControlWindow();
		virtual void PushBaseEvent(WindowControl::Event event) = 0;
	public:
		void EnableControl();
		void DisableControl();
		virtual void Move(int x, int y);
		virtual void Resize(unsigned int width, unsigned int height);
		int GetMouseX();
		int GetMouseY();


		// -- property fields -- //
	public:
		const int& X;
		const int& Y;
		const unsigned int& Width;
		const unsigned int& Height;
		const Rect& Rectangle;


		// -- friends -- //
		friend Window;
	};
}
#endif // !WINDOW_CONTROL_H