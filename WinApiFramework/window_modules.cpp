#include "window_modules.h"

#include "base_window.h"

namespace WinapiFramework
{
	// ~~~~~~~~ [CLASS] Scrollable ~~~~~~~~
	Scrollable::Scrollable()
		: m_canvasRect(0, 0, 800, 600)
	{
	}
	Scrollable::~Scrollable()
	{
	}

	BoundRect Scrollable::GetCanvasRect() const
	{
		return m_canvasRect;
	}
	Point Scrollable::GetCanvasPosition() const
	{
		return m_canvasDrift;
	}

	void Scrollable::AdjustCanvasRect(const std::vector<BaseWindow*> children)
	{
		BoundRect boundRect(std::numeric_limits<int>::max(), std::numeric_limits<int>::max(),
			std::numeric_limits<int>::min(), std::numeric_limits<int>::min());

		for (BaseWindow* child : children)
		{
			if (child == nullptr) continue;

			// left
			if (child->GetWindowRect().position.x < boundRect.left)
				boundRect.left = child->GetWindowRect().position.x;

			// top
			if (child->GetWindowRect().position.y < boundRect.top)
				boundRect.top = child->GetWindowRect().position.y;

			// right
			if (child->GetWindowRect().position.x + child->GetWindowRect().size.width > boundRect.right)
				boundRect.right = child->GetWindowRect().position.x + child->GetWindowRect().size.width;

			// bottom
			if (child->GetWindowRect().position.y + child->GetWindowRect().size.height > boundRect.bottom)
				boundRect.bottom = child->GetWindowRect().position.y + child->GetWindowRect().size.height;
		}

		m_canvasRect = boundRect;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}