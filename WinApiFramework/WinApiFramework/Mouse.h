#ifndef MOUSE_H
#define MOUSE_H

#include <queue>


namespace WinApiFramework
{
	class Mouse
	{
		// -- fields -- //
	private:
		int x = 0, y = 0;
		bool isLeftPressed = false;
		bool isRightPressed = false;
		bool isMiddlePressed = false;

	public:
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
				WeelUp,
				WeelDown,
				Move,
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
	private:
		std::queue<Event> events;
		const unsigned short buffLength = 64u;


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
		void PushEvent(Mouse::Event newEvent);
		Event GetEvent();
		void ClearEventBuffer();


		// -- property fields -- //
	public:
		const int& X;
		const int& Y;
		const bool& LeftPressed;
		const bool& RightPressed;
		const bool& MiddlePressed;

		// -- friend statements -- //
		friend class Window;
		friend class Framework;
	};
}
#endif // !MOUSE_H