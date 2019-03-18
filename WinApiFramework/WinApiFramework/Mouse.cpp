#include "Mouse.h"

using namespace WinApiFramework;

// constructors
Mouse::Mouse()
	:X(x),
	Y(y),
	LeftPressed(isLeftPressed),
	RightPressed(isRightPressed),
	MiddlePressed(isMiddlePressed),
	IsOnWindow(onWindow)
{
	x = 0;
	y = 0;
	isLeftPressed = false;
	isRightPressed = false;
	isMiddlePressed = false;
}
Mouse::~Mouse()
{

}

// methods
// private:
void Mouse::Move(int x, int y)
{
	this->x = x;
	this->y = y;
	mouseEvents.push(Event(Event::Move, x, y));
	TrimEventBuffer();
}
void Mouse::LeftPressedEvent()
{
	isLeftPressed = true;
	mouseEvents.push(Event(Event::LeftPress, x, y));
	TrimEventBuffer();
}
void Mouse::LeftRelasedEvent()
{
	isLeftPressed = false;
	mouseEvents.push(Event(Event::LeftRelase, x, y));
	TrimEventBuffer();
}
void Mouse::RightPressedEvent()
{
	isRightPressed = true;
	mouseEvents.push(Event(Event::RightPress, x, y));
	TrimEventBuffer();
}
void Mouse::RightRelasedEvent()
{
	isRightPressed = false;
	mouseEvents.push(Event(Event::RightRelase, x, y));
	TrimEventBuffer();
}
void Mouse::MiddlePressedEvent()
{
	isMiddlePressed = true;
	mouseEvents.push(Event(Event::MiddlePress, x, y));
	TrimEventBuffer();
}
void Mouse::MiddleRelasedEvent()
{
	isMiddlePressed = false;
	mouseEvents.push(Event(Event::MiddleRelase, x, y));
	TrimEventBuffer();
}
void Mouse::WeelUpEvent()
{
	mouseEvents.push(Event(Event::WeelUp, x, y));
	TrimEventBuffer();
}
void Mouse::WeelDownEvent()
{
	mouseEvents.push(Event(Event::WeelDown, x, y));
	TrimEventBuffer();
}
void Mouse::TrimEventBuffer()
{
	while (mouseEvents.size() > buffLength)
	{
		mouseEvents.pop();
	}
}

// public:
Mouse::Event Mouse::GetEvent()
{
	if (mouseEvents.size() > 0u)
	{
		Event e = mouseEvents.front();
		mouseEvents.pop();
		return e;
	}
	else
	{
		return Event();
	}
}
void Mouse::ClearEventBuffer()
{
	mouseEvents = std::queue<Event>();
}