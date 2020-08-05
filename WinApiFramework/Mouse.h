#ifndef MOUSE_H
#define MOUSE_H

#include "window_include.h"
#include "data_types.h"
#include "event.h"

#include <vector>
#include <queue>
#include <functional>

namespace WinapiFramework
{
	class Mouse : public EventHandler
	{
	private:
		int x = 0, y = 0;
		bool isLeftPressed = false;
		bool isRightPressed = false;
		bool isMiddlePressed = false;
		bool isVisible = true;

	public:
		enum class Cursor
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
		enum class MouseButton
		{
			Left,
			Right,
			Middle
		};

		struct Events
		{
			struct EventLeftPress : public BaseEvent {};
			struct EventLeftRelease : public BaseEvent {};
			struct EventLeftDPress: public BaseEvent {};
			struct EventRightPress : public BaseEvent {};
			struct EventRightRelease : public BaseEvent {};
			struct EventRightDPress : public BaseEvent {};
			struct EventMiddlePress : public BaseEvent {};
			struct EventMiddleRelease : public BaseEvent {};
			struct EventMiddleDPress : public BaseEvent {};
			struct EventWheel : public BaseEvent
			{
				const int delta;

				EventWheel(int d)
					: delta(d)
				{}
			};
			struct EventMove : public BaseEvent
			{
				const Point position;

				EventMove(const Point& pos)
					: position(pos)
				{}
			};
			struct EventCursorChange : public BaseEvent {};
		};


	public:
		Mouse();
		Mouse(const Mouse& mouse) = delete;
		Mouse(const Mouse&& mouse) = delete;
	private:
		~Mouse();


	public:
		Mouse& operator=(const Mouse& mouse) = delete;
		Mouse& operator=(const Mouse&& mouse) = delete;


	private:
		LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		void SetCursorPosition(int x, int y);
		void ShowCursor();
		void HideCursor();
		void SetCursorType(Mouse::Cursor cursorType);


	public:
		const int& X;
		const int& Y;
		const bool& LeftPressed;
		const bool& RightPressed;
		const bool& MiddlePressed;


		friend class Window;
		friend class Framework;
	};
}
#endif // !MOUSE_H