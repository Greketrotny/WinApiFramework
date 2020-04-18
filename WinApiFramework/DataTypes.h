#ifndef DATA_TYPES_H
#define DATA_TYPES_H

namespace WinApiFramework
{
	struct Rect
	{
	public:
		int x, y, width, height;

	public:
		Rect(int x = 0, int y = 0, int width = 100, int height = 50)
			: x(x)
			, y(y)
			, width(width)
			, height(height)
		{
		}
	};
	struct SizeRect
	{
	public:
		int minWidth, minHeight;
		int maxWidth, maxHeight;

	public:
		SizeRect(int minWidth = 0, int minHeight = 0, int maxWidth = 0x0FFFFFFF, int maxHeight = 0x0FFFFFFF)
			: minWidth(minWidth)
			, minHeight(minHeight)
			, maxWidth(maxWidth)
			, maxHeight(maxHeight)
		{
		}
	};
	struct Range
	{
		int min, max;

		Range(int min = 0, int max = 100)
			: min(min)
			, max(max)
		{
		}
	};
}

#endif // !DATA_TYPES_H