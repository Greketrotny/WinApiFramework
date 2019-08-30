#include "WindowControl.h"
#include "WinApiWindow.h"

using namespace WinApiFramework;

// [CLASS] WindowConrtol -----------------------|
// -- constructors -- //
WindowControl::WindowControl(const WindowControl::Config& config)
	:Rectangle(rect),
	X(rect.x),
	Y(rect.y),
	Width(rect.width),
	Height(rect.height)
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
	rect.x = x;
	rect.y = y;

	SetWindowPos(hControl, nullptr,
		rect.x, rect.y,
		rect.width, rect.height,
		SWP_NOSIZE);

	PushBaseEvent(WindowControl::Event::Type::Move);
}
void WindowControl::Resize(unsigned int width, unsigned int height)
{
	rect.width = width;
	rect.height = height;

	SetWindowPos(hControl, nullptr,
		rect.x, rect.y,
		rect.width, rect.height,
		SWP_NOMOVE);

	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Resize));
}
int WindowControl::GetMouseX()
{
	return parentWindow->GetClientMouseX() - rect.x;
}
int WindowControl::GetMouseY()
{
	return parentWindow->GetClientMouseY() - rect.y;
}
// [CLASS] WindowConrtol -----------------------|