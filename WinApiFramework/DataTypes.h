#ifndef DATA_TYPES_H
#define DATA_TYPES_H

namespace WinApiFramework
{
	struct BoundRect;

	struct Point
	{
		int x, y;

		Point(int x = 0, int y = 0);
		Point(const Point& p);

		Point& operator=(const Point& p);
		Point& operator+=(const Point& p);
		Point& operator-=(const Point& p);
		Point operator-(const Point& p) const;
		Point operator+(const Point& p) const;
	};
	struct Size
	{
		int width, height;

		Size(int width = 0, int height = 0);
	};

	struct Rect
	{
		Point position;
		Size size;

		Rect(int x = 0, int y = 0, int width = 100, int height = 50);
		Rect(const Point& position, const Size& size);
		Rect(const BoundRect& boundRect);

		Rect& operator=(const Rect& rect);
		Rect& operator=(const BoundRect& rect);

		bool IsPointInside(const Point& point) const;
	};
	struct SizeRect
	{
		Size minSize, maxSize;

		SizeRect(int minWidth = 0, int minHeight = 0, int maxWidth = 0x7FFFFFFF, int maxHeight = 0x7FFFFFFF);
		SizeRect(const Size& minSize, const Size& maxSize);
	};
	struct BoundRect
	{
		int left, top, right, bottom;

		BoundRect(int left, int top, int right, int bottom);
		BoundRect(const Rect& rect);

		BoundRect& operator=(const BoundRect& boundRect);
		BoundRect& operator=(const Rect& rect);

		bool IsPointInside(const Point& point) const;
	};

	struct Range
	{
		int min, max;

		Range(int min = 0, int max = 100);
	};


	// ~~~~~~~~ [STRUCT] ConStruct ~~~~~~~~
	/*
	*  generic ConStruct structure for all controls
	*/
	template <class T> struct ConStruct;
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !DATA_TYPES_H