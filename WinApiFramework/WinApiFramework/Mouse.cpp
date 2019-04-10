#include "Mouse.h"
#include "WindowInclude.h"

using namespace WinApiFramework;

// [CLASS] Mouse -------------------------------|
// constructors
Mouse::Mouse()
	:X(x),
	Y(y),
	LeftPressed(isLeftPressed),
	RightPressed(isRightPressed),
	MiddlePressed(isMiddlePressed)
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

// -- methods -- //
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
void Mouse::SetEventHandler(Mouse::EventHandler *eh)
{
	this->eventHandler = eh;
}
void Mouse::SetCursorPosition(int x, int y)
{
	SetCursorPos(x, y);
	Move(x, y);
}
void Mouse::ShowCursor()
{
	if (!isVisible)
	{
		::ShowCursor(TRUE);
		isVisible = true;
	}	
}
void Mouse::HideCursor()
{
	if (isVisible)
	{
		::ShowCursor(FALSE);
		isVisible = false;
	}	
}
void Mouse::SetCursorType(Mouse::Cursor cursorType)
{
	HCURSOR hCursor = NULL;

	switch (cursorType)
	{
	case WinApiFramework::Mouse::Arrow:		hCursor = LoadCursor(NULL, IDC_ARROW);		break;
	case WinApiFramework::Mouse::IBeam:		hCursor = LoadCursor(NULL, IDC_IBEAM);		break;
	case WinApiFramework::Mouse::Wait:		hCursor = LoadCursor(NULL, IDC_WAIT);		break;
	case WinApiFramework::Mouse::Cross:		hCursor = LoadCursor(NULL, IDC_CROSS);		break;
	case WinApiFramework::Mouse::UpArrow:	hCursor = LoadCursor(NULL, IDC_UPARROW);	break;
	case WinApiFramework::Mouse::Size:		hCursor = LoadCursor(NULL, IDC_SIZE);		break;
	case WinApiFramework::Mouse::Icon:		hCursor = LoadCursor(NULL, IDC_ICON);		break;
	case WinApiFramework::Mouse::SizeNWSE:	hCursor = LoadCursor(NULL, IDC_SIZENWSE);	break;
	case WinApiFramework::Mouse::SizeNESW:	hCursor = LoadCursor(NULL, IDC_SIZENESW);	break;
	case WinApiFramework::Mouse::SizeWE:	hCursor = LoadCursor(NULL, IDC_SIZEWE);		break;
	case WinApiFramework::Mouse::SizeNS:	hCursor = LoadCursor(NULL, IDC_SIZENS);		break;
	case WinApiFramework::Mouse::SizeALL:	hCursor = LoadCursor(NULL, IDC_SIZEALL);	break;
	case WinApiFramework::Mouse::Hand:		hCursor = LoadCursor(NULL, IDC_HAND);		break;
	case WinApiFramework::Mouse::Help:		hCursor = LoadCursor(NULL, IDC_HELP);		break;
	}
	SetCursor(hCursor);
}
// [CLASS] Mouse -------------------------------|