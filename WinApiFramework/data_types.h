#ifndef DATA_TYPES_H
#define DATA_TYPES_H

namespace WinapiFramework
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


	// ~~~~~~~~ MessageBox enums ~~~~~~~~
	enum class MessBoxButtonLayout
	{
		AbortRetryIgnore = 0x00000002L,
		CancelTryContinue = 0x00000006L,
		Help = 0x00004000L,
		Ok = 0x00000000L,
		OkCancel = 0x00000001L,
		RetryCancel = 0x00000005L,
		YesNo = 0x00000004L,
		YesNoCancel = 0x00000003L
	};
	enum class MessBoxIcon
	{
		Warning = 0x00000030L,
		Information = 0x00000040L,
		Question = 0x00000020L,
		Error = 0x00000010L
	};
	enum class MessBoxButtonPressed
	{
		Ok = 1,
		Cancel = 2,
		Abort = 3,
		Retry = 4,
		Ignore = 5,
		Yes = 6,
		No = 7,
		TryAgain = 10,
		Continue = 11
	};
}

#endif // !DATA_TYPES_H