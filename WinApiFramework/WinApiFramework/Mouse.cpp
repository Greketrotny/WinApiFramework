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
	PushEvent(Mouse::Event(Mouse::Event::Type::Move, x, y));
}

// public:
void Mouse::PushEvent(Mouse::Event newEvent)
{
	events.push(newEvent);

	while (events.size() > buffLength)
		events.pop();
}
Mouse::Event Mouse::GetEvent()
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
void Mouse::ClearEventBuffer()
{
	events = std::queue<Event>();
}
