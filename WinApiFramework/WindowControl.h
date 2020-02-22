#ifndef WINDOW_CONTROL_H
#define WINDOW_CONTROL_H

#include "WindowInclude.h"
#include "ExternIncludes.h"

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

			Rect() {}

			Rect(int x, int y, unsigned int width, unsigned int height)
				:x(x),
				y(y),
				width(width),
				height(height)
			{}
		};
		struct Range
		{
			int min = 0, max = 100;

			Range() {};
			Range(int min, int max)
			{
				this->min = min;
				this->max = max;
			}
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
	protected:
		template <class T> struct EventsManager
		{
		private:
			std::queue<T> events;
			const unsigned short buffSize = 32u;
			std::vector<std::function<void(T)>> eventHandlers;
			bool eventHandlersEnabled = true;

		public:
			// -- constructor -- //
			EventsManager() {}
			~EventsManager() {}

		public:
			// -- methods -- //
			void PushEvent(T newEvent)
			{
				// push event to buffer
				events.push(newEvent);
				if (events.size() > buffSize)
					events.pop();

				// call handler function
				if (eventHandlersEnabled)
				{
					for (unsigned int i = 0u; i < eventHandlers.size(); ++i)
					{
						eventHandlers[i](newEvent);
					}
				}
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
			template <class EventReceiver> void AddEventHandler(EventReceiver* receiverObject, void(EventReceiver::*eventFunction)(T))
			{
				using std::placeholders::_1;
				std::function<void(T)> f;
				f = std::bind(eventFunction, receiverObject, _1);
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