#include "Precompiled.h"
#include "WindowControl.h"
#include "WinApiWindow.h"

using namespace WinApiFramework;

// ~~~~~~~~ [CLASS] WindowConrtol ~~~~~~~~
// ~~ WindowControl::constructors ~~
WindowControl::WindowControl(const WindowControl::ConStruct& config)
	: Rectangle(m_rect)
{
	m_rect = config.rect;

	//SetRect(config.rect);
}
WindowControl::~WindowControl()
{
}

// ~~ WindowControl::methods ~~
void WindowControl::Enable()
{
	::EnableWindow(m_hWindow, TRUE);
	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Enable));
}
void WindowControl::Disable()
{
	::EnableWindow(m_hWindow, FALSE);
	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Disable));
}
// public:
void WindowControl::Move(int x, int y)
{
	m_rect.position.x = x;
	m_rect.position.y = y;

	SetWindowPos(this->m_hWindow, nullptr,
				 m_rect.position.x, m_rect.position.y,
				 m_rect.size.width, m_rect.size.height,
				 SWP_NOSIZE);

	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Move));
}
void WindowControl::Resize(int width, int height)
{
	m_rect.size.width = width;
	m_rect.size.height = height;

	if (m_rect.size.width < 0) m_rect.size.width = 0;
	if (m_rect.size.height < 0) m_rect.size.height = 0;

	SetWindowPos(m_hWindow, nullptr,
				 m_rect.position.x, m_rect.position.y,
				 m_rect.size.width, m_rect.size.height,
				 SWP_NOMOVE);

	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Resize));
}
void WindowControl::SetRect(Rect rect)
{
	m_rect = rect;

	if (m_rect.size.width < 0) m_rect.size.width = 0;
	if (m_rect.size.height < 0) m_rect.size.height = 0;

	SetWindowPos(m_hWindow, nullptr,
				 m_rect.position.x, m_rect.position.y,
				 m_rect.size.width, m_rect.size.height, 0);

	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Move));
	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Resize));
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~