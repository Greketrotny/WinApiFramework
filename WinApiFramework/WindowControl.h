#ifndef WINDOW_CONTROL_H
#define WINDOW_CONTROL_H

#include "WindowInclude.h"
#include "ExternIncludes.h"
#include "DataTypes.h"

#include "BaseControl.h"

namespace WinApiFramework
{
	class Window;

	class WindowControl : virtual public BaseControl
	{
		// ~~ WindowControl::fields ~~ //
	protected:
		DWORD m_controlStyle = WS_CHILD | WS_VISIBLE;
		Rect m_rect;
	public:
		struct ConStruct
		{
			Rect rect;

			ConStruct(const ConStruct& conStruct)
				: rect(conStruct.rect)
			{}
			ConStruct(Rect rect = Rect())
				: rect(rect)
			{}
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


		// ~~ WindowControl::constructors ~~ //
	protected:
		WindowControl(const WindowControl::ConStruct& conStruct);
		virtual ~WindowControl();


		// ~~ WindowControl::methods ~~ //
	protected:
		virtual bool CreateControlWindow() = 0;
		virtual void DestroyControlWindow() = 0;

		virtual void PushBaseEvent(WindowControl::Event event) = 0;
	public:
		void Enable();
		void Disable();

		void Move(int x, int y);
		void Resize(int width, int height);
		void SetRect(Rect rect);

		// ~~ property fields ~~ //
	public:
		const Rect& Rectangle;
	};
}
#endif // !WINDOW_CONTROL_H