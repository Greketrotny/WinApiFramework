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
	enum MessBoxButtonLayout
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
	enum MessBoxIcon
	{
		IconWarning = 0x00000030L,
		IconInformation = 0x00000040L,
		IconQuestion = 0x00000020L,
		IconError = 0x00000010L
	};
	enum MessBoxButtonPressed
	{
		ButtonOk = 1,
		ButtonCancel = 2,
		ButtonAbort = 3,
		ButtonRetry = 4,
		ButtonIgnore = 5,
		ButtonYes = 6,
		ButtonNo = 7,
		ButtonTryAgain = 10,
		ButtonContinue = 11
	};
}

#endif // !DATA_TYPES_H