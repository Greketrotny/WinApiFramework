#ifndef RECT_H
#define RECT_H

#include "Point.h"

namespace Graphics
{
	template <typename T> struct Rect
	{
		// -- Rect::fields -- //
	public:
		T left, top, right, bottom;


		// -- Rect::constructors -- //
	public:
		Rect(const Rect& rect)
			: left(rect.left)
			, top(rect.top)
			, right(rect.right)
			, bottom(rect.bottom)
		{
		}
		Rect(Rect&& rect)
			: left(rect.left)
			, top(rect.top)
			, right(rect.right)
			, bottom(rect.bottom)
		{
		}
		Rect(const T& left, const T& top, const T& right, const T& bottom)
			: left(left)
			, top(top)
			, right(right)
			, bottom(bottom)
		{
		}
		Rect(const Point<T>& topLeft, const Point<T>& bottomRight)
			: left(topLeft.x)
			, top(topLeft.y)
			, right(bottomRight.x)
			, bottom(bottomRight.y)
		{
		}
		~Rect()
		{
		}
	};
}

#endif // !RECT_H