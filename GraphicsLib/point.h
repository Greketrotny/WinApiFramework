#ifndef POINT2D_H
#define POINT2D_H

#include <math.h>

namespace Graphics
{
	template<typename T> struct Point
	{
	public:
		T x, y;


	public:
		Point(T x, T y)
			: x(x)
			, y(y)
		{
		}
		Point(const Point& point)
			: x(point.x)
			, y(point.y)
		{
		}
		Point(Point&& point)
			: x(point.x)
			, y(point.y)
		{
		}
		~Point()
		{
		}


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


	public:
		T Magnitude()
		{
			return sqrt((x * x) + (y * y));
		}
	};
}

#endif // !POINT2D_H