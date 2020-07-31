#include "data_types.h"

namespace WinapiFramework
{
	// ~~~~~~~~ [STRUCT] Point ~~~~~~~~ //
	Point::Point(int x, int y)
		: x(x)
		, y(y)
	{}
	Point::Point(const Point& p)
		: x(p.x)
		, y(p.y)
	{}

	Point& Point::operator=(const Point& p)
	{
		this->x = p.x;
		this->y = p.y;
		return *this;
	}
	Point& Point::operator+=(const Point& p)
	{
		this->x += p.x;
		this->y += p.y;
		return *this;
	}
	Point& Point::operator-=(const Point& p)
	{
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}
	Point Point::operator-(const Point& p) const
	{
		return Point(this->x - p.x, this->y - p.y);
	}
	Point Point::operator+(const Point& p) const
	{
		return Point(this->x + p.x, this->y + p.y);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //



	// ~~~~~~~~ [STRUCT] Size ~~~~~~~~ //
	Size::Size(int width, int height)
		:width(width)
		, height(height)
	{}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //



	// ~~~~~~~~ [STRUCT] Rect ~~~~~~~~ //
	Rect::Rect(int x, int y, int width, int height)
		: position(x, y)
		, size(width, height)
	{}
	Rect::Rect(const Point& position, const Size& size)
		: position(position)
		, size(size)
	{}
	Rect::Rect(const BoundRect& boundRect)
	{
		*this = boundRect;
	}

	Rect& Rect::operator=(const Rect& rect)
	{
		this->position = rect.position;
		this->size = rect.size;
		return *this;
	}
	Rect& Rect::operator=(const BoundRect& boundRect)
	{
		position.x = boundRect.left;
		position.y = boundRect.top;
		size.width = boundRect.right - boundRect.left;
		size.height = boundRect.bottom - boundRect.top;
		return *this;
	}

	bool Rect::IsPointInside(const Point& point) const
	{
		return (point.x >= position.x
			&& point.y >= position.y
			&& point.x <= position.x + size.width
			&& point.y <= position.y + size.height);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //



	// ~~~~~~~~ [STRUCT] BoundRect ~~~~~~~~ //
	BoundRect::BoundRect(int left, int top, int right, int bottom)
		: left(left)
		, top(top)
		, right(right)
		, bottom(bottom)
	{}
	BoundRect::BoundRect(const Rect& rect)
		: left(rect.position.x)
		, top(rect.position.y)
		, right(rect.position.x + rect.size.width)
		, bottom(rect.position.y + rect.size.height)
	{}

	BoundRect& BoundRect::operator=(const BoundRect& boundRect)
	{
		this->left = boundRect.left;
		this->top = boundRect.top;
		this->right = boundRect.right;
		this->bottom = boundRect.bottom;
		return *this;
	}
	BoundRect& BoundRect::operator=(const Rect& rect)
	{
		this->left = rect.position.x;
		this->top = rect.position.y;
		this->right = rect.position.x + rect.size.width;
		this->bottom = rect.position.y + rect.size.height;
		return *this;
	}

	bool BoundRect::IsPointInside(const Point& point) const
	{
		return (point.x >= left && point.y >= top && point.x <= right && point.y <= bottom);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //



	// ~~~~~~~~ [STRUCT] Range ~~~~~~~~ //
	Range::Range(int min, int max)
		: min(min)
		, max(max)
	{
		if (this->max < this->min) this->max = this->min;
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
}