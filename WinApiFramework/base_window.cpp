#include "base_window.h"
#include "winapi_framework.h"

#include <assert.h>

namespace WinapiFramework
{
	// ~~~~~~~~ [CLASS] BaseWindow ~~~~~~~~
	BaseWindow::BaseWindow(ParentWindow* parent)
		: mp_parent(parent)
		, m_window_style(0)
	{
	}
	BaseWindow::~BaseWindow()
	{
	}


	LRESULT BaseWindow::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		return LRESULT(TRUE);
	}
	void BaseWindow::Destroy()
	{
		assert(mp_parent);
		mp_parent->DestroyChild(this);
	}
	void BaseWindow::Enable()
	{
		::EnableWindow(m_hWindow, TRUE);
	}
	void BaseWindow::Disable()
	{
		::EnableWindow(m_hWindow, FALSE);
	}
	void BaseWindow::Move(int x, int y)
	{
		assert(mp_parent);

		m_window_rect.position.x = x;
		m_window_rect.position.y = y;

		SetWindowPos(this->m_hWindow, nullptr,
			m_window_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_window_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_window_rect.size.width, m_window_rect.size.height,
			SWP_NOSIZE);

		// TODO: raise event
	}
	void BaseWindow::Move(const Point& position)
	{
		Move(position.x, position.y);
	}
	void BaseWindow::Resize(int width, int height)
	{
		assert(mp_parent);

		m_window_rect.size.width = width;
		m_window_rect.size.height = height;

		if (m_window_rect.size.width < 0) m_window_rect.size.width = 0;
		if (m_window_rect.size.height < 0) m_window_rect.size.height = 0;

		SetWindowPos(m_hWindow, nullptr,
			m_window_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_window_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_window_rect.size.width, m_window_rect.size.height,
			SWP_NOMOVE);
	}
	void BaseWindow::Resize(const Size& size)
	{
		Resize(size.width, size.height);
	}
	void BaseWindow::SetRect(const Rect& rect)
	{
		Move(rect.position);
		Resize(rect.size);
	}
	Point BaseWindow::GetMousePosition() const
	{
		assert(mp_parent);

		return mp_parent->GetMousePosition() - this->m_window_rect.position;
	}
	bool BaseWindow::IsMouseInside() const
	{
		Point mouse = GetMousePosition();
		return (mouse.x >= 0
			&& mouse.y >= 0
			&& mouse.x < m_window_rect.size.width
			&& mouse.y < m_window_rect.size.height);
	}

	const HWND& BaseWindow::GetWindowHandle() const
	{
		return m_hWindow;
	}
	const std::wstring& BaseWindow::GetWindowClassName() const
	{
		return m_window_class_name;
	}
	const Rect& BaseWindow::GetWindowRect() const
	{
		return m_window_rect;
	}
	
	ParentWindow* const BaseWindow::GetParent() const
	{
		return mp_parent;
	}
	WNDPROC BaseWindow::GetFrameworkProcedure() const
	{
		return (WNDPROC)Framework::WinApiProcedure;
	}
	void BaseWindow::AppendAction(BaseAction* action) const
	{
		Framework::AppendAction(action);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ParentWindow ~~~~~~~~
	ParentWindow::ParentWindow(ParentWindow* parent)
		: BaseWindow(parent)
	{
	}
	ParentWindow::~ParentWindow()
	{
		DestroyAllChildren();
	}

	bool ParentWindow::DestroyChild(BaseWindow* child)
	{
		if (child == nullptr) return false;

		for (size_t i = 0; i < m_children.size(); ++i)
		{
			if (m_children[i] == child)
			{
				ObjectCreator::DestroyObject(child);
				m_children.erase(m_children.begin() + i);
				return true;
			}
		}

		return false;
	}
	void ParentWindow::DestroyAllChildren()
	{
		for (size_t i = 0; i < m_children.size(); ++i)
		{
			ObjectCreator::DestroyObject(m_children[i]);
		}
		m_children.clear();
	}
	LRESULT ParentWindow::ProcessChildMessage(WPARAM wParam, LPARAM lParam)
	{
		for (BaseWindow*& child : m_children)
		{
			if (child->GetWindowHandle() == (HWND)lParam)
			{
				child->ControlProcedure(wParam, lParam);
				return 0;
			}
		}
		return 1;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
}