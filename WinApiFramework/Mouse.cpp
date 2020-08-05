#include "mouse.h"
#include "window_include.h"

namespace WinapiFramework
{
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
		
	LRESULT Mouse::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			// left button
			case WM_LBUTTONDOWN:
			case WM_NCLBUTTONDOWN:
				isLeftPressed = true;
				RaiseEventByHandler<Events::EventLeftPress>();
				break;

			case WM_LBUTTONUP:
			case WM_NCLBUTTONUP:
				isLeftPressed = false;
				RaiseEventByHandler<Events::EventLeftRelease>();
				break;

			case WM_LBUTTONDBLCLK:
			case WM_NCLBUTTONDBLCLK:
				RaiseEventByHandler<Events::EventLeftDPress>();
				break;

			// right button
			case WM_RBUTTONDOWN:
			case WM_NCRBUTTONDOWN:
				isLeftPressed = true;
				RaiseEventByHandler<Events::EventRightPress>();
				break;

			case WM_RBUTTONUP:
			case WM_NCRBUTTONUP:
				isLeftPressed = false;
				RaiseEventByHandler<Events::EventRightRelease>();
				break;

			case WM_RBUTTONDBLCLK:
			case WM_NCRBUTTONDBLCLK:
				RaiseEventByHandler<Events::EventRightDPress>();
				break;

			// middle button
			case WM_MBUTTONDOWN:
			case WM_NCMBUTTONDOWN:
				isLeftPressed = true;
				RaiseEventByHandler<Events::EventMiddlePress>();
				break;

			case WM_MBUTTONUP:
			case WM_NCMBUTTONUP:
				isLeftPressed = false;
				RaiseEventByHandler<Events::EventMiddleRelease>();
				break;

			case WM_MBUTTONDBLCLK:
			case WM_NCMBUTTONDBLCLK:
				RaiseEventByHandler<Events::EventMiddleDPress>();
				break;

			// move
			case WM_MOUSEMOVE:
			case WM_NCMOUSEMOVE:
			{
				POINT pt;
				GetCursorPos(&pt);
				this->x = pt.x;
				this->y = pt.y;
				RaiseEventByHandler<Events::EventMove>(Point(this->x, this->y));
				break;
			}
			
			// wheel
			case WM_MOUSEWHEEL:
			{
				int z_delta = GET_WHEEL_DELTA_WPARAM(wParam);
				RaiseEventByHandler<Events::EventWheel>(z_delta);
				break;
			}

			default: return 1;
		}
		return 0;
	}

	void Mouse::SetCursorPosition(int x, int y)
	{
		SetCursorPos(x, y);
		POINT pt;
		GetCursorPos(&pt);
		this->x = pt.x;
		this->y = pt.y;
		RaiseEventByHandler<Events::EventMove>(Point(this->x, this->y));
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
			case WinapiFramework::Mouse::Cursor::Arrow:		hCursor = LoadCursor(NULL, IDC_ARROW);		break;
			case WinapiFramework::Mouse::Cursor::IBeam:		hCursor = LoadCursor(NULL, IDC_IBEAM);		break;
			case WinapiFramework::Mouse::Cursor::Wait:		hCursor = LoadCursor(NULL, IDC_WAIT);		break;
			case WinapiFramework::Mouse::Cursor::Cross:		hCursor = LoadCursor(NULL, IDC_CROSS);		break;
			case WinapiFramework::Mouse::Cursor::UpArrow:	hCursor = LoadCursor(NULL, IDC_UPARROW);	break;
			case WinapiFramework::Mouse::Cursor::Size:		hCursor = LoadCursor(NULL, IDC_SIZE);		break;
			case WinapiFramework::Mouse::Cursor::Icon:		hCursor = LoadCursor(NULL, IDC_ICON);		break;
			case WinapiFramework::Mouse::Cursor::SizeNWSE:	hCursor = LoadCursor(NULL, IDC_SIZENWSE);	break;
			case WinapiFramework::Mouse::Cursor::SizeNESW:	hCursor = LoadCursor(NULL, IDC_SIZENESW);	break;
			case WinapiFramework::Mouse::Cursor::SizeWE:	hCursor = LoadCursor(NULL, IDC_SIZEWE);		break;
			case WinapiFramework::Mouse::Cursor::SizeNS:	hCursor = LoadCursor(NULL, IDC_SIZENS);		break;
			case WinapiFramework::Mouse::Cursor::SizeALL:	hCursor = LoadCursor(NULL, IDC_SIZEALL);	break;
			case WinapiFramework::Mouse::Cursor::Hand:		hCursor = LoadCursor(NULL, IDC_HAND);		break;
			case WinapiFramework::Mouse::Cursor::Help:		hCursor = LoadCursor(NULL, IDC_HELP);		break;
		}
		SetCursor(hCursor);
		RaiseEventByHandler<Events::EventCursorChange>();
	}
}