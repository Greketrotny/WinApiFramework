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
		bool isVisible = true;

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

		// -- friend statements -- //
		friend class Window;
		friend class Framework;
	};
}
#endif // !MOUSE_H