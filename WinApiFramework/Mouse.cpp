#include "mouse.h"
#include "window_include.h"

namespace WinapiFramework
{
	Mouse::Mouse()
		:X(x),
		Y(y),
		LeftPressed(isLeftPressed),
		RightPressed(isRightPressed),
		MiddlePressed(isMiddlePressed),
		Events(events)
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

	void Mouse::Move(int x, int y)
	{
		this->x = x;
		this->y = y;
		events.PushEvent(Mouse::Event(Mouse::Event::Type::Move, x, y));
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
			case WinapiFramework::Mouse::Arrow:		hCursor = LoadCursor(NULL, IDC_ARROW);		break;
			case WinapiFramework::Mouse::IBeam:		hCursor = LoadCursor(NULL, IDC_IBEAM);		break;
			case WinapiFramework::Mouse::Wait:		hCursor = LoadCursor(NULL, IDC_WAIT);		break;
			case WinapiFramework::Mouse::Cross:		hCursor = LoadCursor(NULL, IDC_CROSS);		break;
			case WinapiFramework::Mouse::UpArrow:	hCursor = LoadCursor(NULL, IDC_UPARROW);	break;
			case WinapiFramework::Mouse::Size:		hCursor = LoadCursor(NULL, IDC_SIZE);		break;
			case WinapiFramework::Mouse::Icon:		hCursor = LoadCursor(NULL, IDC_ICON);		break;
			case WinapiFramework::Mouse::SizeNWSE:	hCursor = LoadCursor(NULL, IDC_SIZENWSE);	break;
			case WinapiFramework::Mouse::SizeNESW:	hCursor = LoadCursor(NULL, IDC_SIZENESW);	break;
			case WinapiFramework::Mouse::SizeWE:	hCursor = LoadCursor(NULL, IDC_SIZEWE);		break;
			case WinapiFramework::Mouse::SizeNS:	hCursor = LoadCursor(NULL, IDC_SIZENS);		break;
			case WinapiFramework::Mouse::SizeALL:	hCursor = LoadCursor(NULL, IDC_SIZEALL);	break;
			case WinapiFramework::Mouse::Hand:		hCursor = LoadCursor(NULL, IDC_HAND);		break;
			case WinapiFramework::Mouse::Help:		hCursor = LoadCursor(NULL, IDC_HELP);		break;
		}
		SetCursor(hCursor);
		events.PushEvent(Mouse::Event(Mouse::Event::Type::CursorChanged, X, Y));
	}
}