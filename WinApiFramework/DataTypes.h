#ifndef DATA_TYPES_H
#define DATA_TYPES_H

namespace WinApiFramework
{
	struct Point
	{
		int x, y;

		Point(int x = 0, int y = 0)
			: x(x)
			, y(y)
		{}
	};
	struct Size
	{
		int width, height;

		Size(int width = 0, int height = 0)
			: width(width)
			, height(height)
		{}
	};

	struct Rect
	{
		Point position;
		Size size;

		Rect(int x = 0, int y = 0, int width = 100, int height = 50)
			: position(x, y)
			, size(width, height)
		{}
		Rect(const Point& position, const Size& size)
			: position(position)
			, size(size)
		{}
	};
	struct SizeRect
	{
		Size minSize, maxSize;

		SizeRect(int minWidth = 0, int minHeight = 0, int maxWidth = 0x7FFFFFFF, int maxHeight = 0x7FFFFFFF)
			: minSize(minWidth, minHeight)
			, maxSize(maxWidth, maxHeight)
		{}
		SizeRect(const Size& minSize, const Size& maxSize)
			: minSize(minSize)
			, maxSize(maxSize)
		{}
	};

	struct Range
	{
		int min, max;

		Range(int min = 0, int max = 100)
			: min(min)
			, max(max)
		{
			if (this->max < this->min) this->max = this->min;
		}
	};


	// ~~~~~~~~ [STRUCT] ConStruct ~~~~~~~~
	/*
	*  generic ConStruct structure for all controls
	*/
	template <class T> struct ConStruct;
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

#endif // !DATA_TYPES_H