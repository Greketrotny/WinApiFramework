#ifndef POINT_H
#define POINT_H

#include <math.h>

namespace Graphics
{
	template<class T> class Point
	{
		// -- fields -- //
	public:
		T x, y;

		// -- constructor -- //
	public:
		Point(T x, T y)
			:x(x), y(y)
		{}


		// -- operators -- //
	public:
		Point& operator=(const Point& point)
		{
			this->x = point->x;
			this->y = point->y;

			return *this;
		}
		Point& operator=(const Point&& point)
		{
			this->x = point->x;
			this->y = point->y;

			return *this;
		}
		Point operator+(const Point& point)
		{
			return Point(this->x + point.x, this->y + point.y);
		}
		Point operator-(const Point& point)
		{
			return Point(this->x - point.x, this->y - point.y);
		}
		Point& operator+=(const Point& point)
		{
			this->x += point.x;
			this->y += point.y;
			return *this;
		}
		Point& operator-=(const Point& point)
		{
			this->x -= point.x;
			this->y -= point.y;
			return *this;
		}

		// -- methods -- //
	public:
		T Magnitude()
		{
			return sqrt((x * x) + (y * y));
		}
	};
}

#endif // !POINT_H

