#ifndef MOUSE_H
#define MOUSE_H

#include <vector>
#include <queue>
#include <functional>

namespace WinapiFramework
{
	class Mouse
	{
		// -- fields -- //
	private:
		int x = 0, y = 0;
		bool isLeftPressed = false;
		bool isRightPressed = false;
		bool isMiddlePressed = false;
		bool isVisible = true;

	public:
		enum Cursor
		{
			Arrow,
			IBeam,
			Wait,
			Cross,
			UpArrow,
			Size,
			Icon,
			SizeNWSE,
			SizeNESW,
			SizeWE,
			SizeNS,
			SizeALL,
			Hand,
			Help
		};
		struct Event
		{
		public:
			enum Type
			{
				LeftPress,
				LeftRelase,
				RightPress,
				RightRelase,
				MiddlePress,
				MiddleRelase,
				WheelUp,
				WheelDown,
				Move,
				CursorChanged,
				Invalid
			};
			Type type;
			int x = 0, y = 0;

			Event()
			{
				type = Invalid;
				x = 0;
				y = 0;
			}
			Event(Type type)
			{
				this->type = type;
			}
			Event(Type type, int x, int y)
			{
				this->type = type;
				this->x = x;
				this->y = y;
			}
		};
		struct Events
		{
			// -- fields -- //
		private:
			std::queue<Mouse::Event> events;
			std::vector<std::function<void(Mouse::Event)>> eventHandlers;
		public:
			const unsigned short buffLength = 16u;
			bool eventHandlersEnabled = true;


			// -- constructor -- //
		public:
			Events() {}
			~Events() {}


			// -- methods -- //
		public:
			void PushEvent(Mouse::Event newEvent)
			{
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
			Mouse::Event GetEvent()
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
				events = std::queue<Mouse::Event>();
			}
			template <class EventReceiver> void AddEventHandler(EventReceiver* receiverObject, void(EventReceiver::*eventFunction)(Mouse::Event))
			{
				using std::placeholders::_1;
				std::function<void(Mouse::Event)> f;
				f = std::bind(eventFunction, receiverObject, _1);
				eventHandlers.push_back(f);
			}
			void RemoveAllEventHandlers()
			{
				eventHandlers.clear();
			}
		};
	private:
		Events events;


		// -- constructor -- //
	public:
		Mouse();
		Mouse(const Mouse& mouse) = delete;
		Mouse(const Mouse&& mouse) = delete;
	private:
		~Mouse();


		// -- operators -- //
	public:
		Mouse& operator=(const Mouse& mouse) = delete;
		Mouse& operator=(const Mouse&& mouse) = delete;


		// -- methods -- //
	private:
		void Move(int x, int y);
	public:
		void SetCursorPosition(int x, int y);
		void ShowCursor();
		void HideCursor();
		void SetCursorType(Mouse::Cursor cursorType);


		// -- property fields -- //
	public:
		const int& X;
		const int& Y;
		const bool& LeftPressed;
		const bool& RightPressed;
		const bool& MiddlePressed;
		Mouse::Events& Events;

		// -- friend statements -- //
		friend class Window;
		friend class Framework;
	};
}
#endif // !MOUSE_H