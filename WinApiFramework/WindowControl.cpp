#include "Precompiled.h"
#include "WindowControl.h"
#include "WinApiWindow.h"

using namespace WinApiFramework;

// [CLASS] WindowConrtol -----------------------|
// -- constructors -- //
WindowControl::WindowControl(const WindowControl::ConStruct& config)
	:Rectangle(rect),
	X(rect.position.x),
	Y(rect.position.y),
	Width(rect.size.width),
	Height(rect.size.height)
{
	rect = config.rect;
}
WindowControl::~WindowControl()
{
	if (parentWindow)
	{
		parentWindow->RemoveControl(this);
		parentWindow = nullptr;
	}
}

// -- methods -- //
// protected:
void WindowControl::DestroyControlWindow()
{
	if (hControl != NULL)
	{
		DestroyWindow(hControl);
		hControl = NULL;
		parentWindow = nullptr;
	}
}
void WindowControl::EnableControl()
{
	::EnableWindow(hControl, TRUE);
	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Enable));
}
void WindowControl::DisableControl()
{
	::EnableWindow(hControl, FALSE);
	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Disable));
}
// public:
void WindowControl::Move(int x, int y)
{
	rect.position.x = x;
	rect.position.y = y;

	SetWindowPos(hControl, nullptr,
		rect.position.x, rect.position.y,
		rect.size.width, rect.size.height,
		SWP_NOSIZE);

	PushBaseEvent(WindowControl::Event::Type::Move);
}
void WindowControl::Resize(int width, int height)
{
	rect.size.width = width;
	rect.size.height = height;

	if (rect.size.width < 0) rect.size.width = 0;
	if (rect.size.height < 0) rect.size.height = 0;

	SetWindowPos(hControl, nullptr,
		rect.position.x, rect.position.y,
		rect.size.width, rect.size.height,
		SWP_NOMOVE);

	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Resize));
}
int WindowControl::GetMouseX()
{
	return parentWindow->GetClientMouseX() - rect.position.x;
}
int WindowControl::GetMouseY()
{
	return parentWindow->GetClientMouseY() - rect.position.y;
}
// [CLASS] WindowConrtol -----------------------|