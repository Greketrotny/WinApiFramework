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
		DoDestroy();
	}
	void BaseWindow::DoDestroy()
	{
		assert(mp_parent);
		mp_parent->DestroyChild(this);
	}

	void BaseWindow::Enable()
	{
		DoEnable();
		RaiseEventByHandler<BaseWindowEvents::EventEnable>();
	}
	void BaseWindow::DoEnable()
	{
		::EnableWindow(m_hWindow, TRUE);
	}

	void BaseWindow::Disable()
	{
		DoDisable();
		RaiseEventByHandler<BaseWindowEvents::EventDisable>();
	}
	void BaseWindow::DoDisable()
	{
		::EnableWindow(m_hWindow, FALSE);
	}

	void BaseWindow::Move(int x, int y)
	{
		assert(mp_parent);

		m_rect.position.x = x;
		m_rect.position.y = y;

		DoMove(
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y);


		RaiseEventByHandler<BaseWindowEvents::EventMove>();
	}
	void BaseWindow::Move(const Point& position)
	{
		Move(position.x, position.y);
	}
	void BaseWindow::DoMove(int x, int y)
	{
		SetWindowPos(m_hWindow, nullptr,
			x, y,
			0, 0,
			SWP_NOSIZE);
	}

	void BaseWindow::Resize(int width, int height)
	{
		m_rect.size.width = width;
		m_rect.size.height = height;

		if (m_rect.size.width < 0) m_rect.size.width = 0;
		if (m_rect.size.height < 0) m_rect.size.height = 0;

		DoResize(m_rect.size.width, m_rect.size.height);

		RaiseEventByHandler<BaseWindowEvents::EventResize>();
	}
	void BaseWindow::Resize(const Size& size)
	{
		Resize(size.width, size.height);
	}
	void BaseWindow::DoResize(int width, int height)
	{
		SetWindowPos(m_hWindow, nullptr,
			0, 0,
			width, height,
			SWP_NOMOVE);
	}

	void BaseWindow::SetRect(const Rect& rect)
	{
		Move(rect.position);
		Resize(rect.size);
	}

	Point BaseWindow::GetMousePosition() const
	{
		assert(mp_parent);

		return mp_parent->GetMousePosition() - this->m_rect.position;
	}
	bool BaseWindow::IsMouseInside() const
	{
		Point mouse = GetMousePosition();
		return (mouse.x >= 0
			&& mouse.y >= 0
			&& mouse.x < m_rect.size.width
			&& mouse.y < m_rect.size.height);
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
		return m_rect;
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

	Point ParentWindow::GetMousePosition() const
	{
		assert(mp_parent);
		return mp_parent->GetMousePosition() - m_rect.position;
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