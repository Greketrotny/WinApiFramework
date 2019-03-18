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
		bool onWindow = false;

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
			int x, y;

			Event()
			{
				type = Invalid;
				x = 0;
				y = 0;
			}
			Event(Type type, int x, int y)
			{
				this->type = type;
				this->x = x;
				this->y = y;
			}
		};
	private:
		std::queue<Event> mouseEvents;
		const unsigned short buffLength = 64u;


		// -- constructor -- //
	public:
		Mouse();
		Mouse(const Mouse &mouse) = delete;
	private:
		~Mouse();


		// -- operators -- //
	public:
		Mouse& operator=(const Mouse &mouse) = delete;


		// -- methods -- //
	private:
		void Move(int x, int y);
		void LeftPressedEvent();
		void LeftRelasedEvent();
		void RightPressedEvent();
		void RightRelasedEvent();
		void MiddlePressedEvent();
		void MiddleRelasedEvent();
		void WeelUpEvent();
		void WeelDownEvent();
		void TrimEventBuffer();
	public:
		Event GetEvent();
		void ClearEventBuffer();


		// -- property fields -- //
	public:
		const int& X;
		const int& Y;
		const bool& LeftPressed;
		const bool& RightPressed;
		const bool& MiddlePressed;
		const bool& IsOnWindow;

		// -- friend statements -- //
		friend class Window;
	};
}
#endif // !MOUSE_H