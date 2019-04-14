#include "WinApiWindowControls.h"
#include "WinApiFramework.h"
#include "WinApiWindow.h"

using namespace WinApiFramework;

// [CLASS] WindowConrtol -----------------------|
// -- constructors -- //
WindowControl::WindowControl(const WindowControl::Config& config)
	:Rectangle(rect)
{
	rect = config.rect;
}
WindowControl::~WindowControl()
{
	if (parentWindow)
	{
		parentWindow->RemoveControl(this);
		parentWindow = nullptr;
	}
}

// -- methods -- //
// protected:
void WindowControl::DestroyControlWindow()
{
	if (hControl != NULL)
	{
		DestroyWindow(hControl);
		hControl = NULL;
		parentWindow = nullptr;
	}
}
void WindowControl::EnableControl()
{
	::EnableWindow(hControl, TRUE);
	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Enable));
}
void WindowControl::DisableControl()
{
	::EnableWindow(hControl, FALSE);
	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Disable));
}
// public:
void WindowControl::SetPosition(int x, int y)
{
	rect.x = x;
	rect.y = y;

	SetWindowPos(hControl, nullptr,
		rect.x, rect.y,
		rect.width, rect.height,
		0);

	PushBaseEvent(WindowControl::Event::Type::Move);
}
void WindowControl::SetDimensions(unsigned int width, unsigned int height)
{
	rect.width = width;
	rect.height = height;

	SetWindowPos(hControl, nullptr,
		rect.x, rect.y,
		rect.width, rect.height,
		0);

	PushBaseEvent(WindowControl::Event(WindowControl::Event::Type::Resize));
}
int WindowControl::GetMouseX()
{
	return parentWindow->GetMouseX() - rect.x;
}
int WindowControl::GetMouseY()
{
	return parentWindow->GetMouseY() - rect.y;
}
// [CLASS] WindowConrtol -----------------------|


// [CLASS] Button ------------------------------|
// -- constructors -- //
Button::Button(const Button::Config& config)
	: WindowControl(config),
	Rect(rect),
	Caption(caption),
	Events(events)
{
	caption = config.caption;
}
Button::Button(const Button::Config &config, Button::EventHandler *eventHandler)
	: Button(config)
{
	events.SetEventHandler(eventHandler);
}
Button::~Button()
{

}

// -- methods -- //
// private:
int Button::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	case BN_CLICKED:
		events.PushEvent(Button::Event(Button::Event::Type::Click));
		break;

	case BN_DBLCLK:
		events.PushEvent(Button::Event(Button::Event::Type::DoubleClick));
		break;

	case BN_SETFOCUS:
		events.PushEvent(Button::Event(Button::Event::Type::Focus));
		break;

	case BN_KILLFOCUS:
		events.PushEvent(Button::Event(Button::Event::Type::Unfocus));
		break;

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool Button::CreateControlWindow()
{
	// create window
	hControl = CreateWindow(L"BUTTON", caption.c_str(),
		WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_PUSHBUTTON | BS_MULTILINE,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!hControl)
	{
		MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
		return false;
	}
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	return true;
}

// public:
void Button::SetCaption(std::wstring newCaption)
{
	caption = newCaption;
	SetWindowText(hControl, caption.c_str());
}
void Button::SetPosition(int x, int y)
{
	WindowControl::SetPosition(x, y);
	events.PushEvent(Button::Event::Type::Move);
}
void Button::SetDimensions(unsigned int width, unsigned int height)
{
	WindowControl::SetDimensions(width, height);
	events.PushEvent(Button::Event::Type::Resize);
}
// [CLASS] Button ------------------------------|


// [CLASS] CheckBox ----------------------------|
// -- constructors -- //
CheckBox::CheckBox(const CheckBox::Config& config)
	: WindowControl(config),
	State(boxState),
	Caption(caption),
	IsTripleState(isTripleState)
{
	caption = config.caption;
	isTripleState = config.isTripleState;
	boxState = config.boxState;

	if (!this->isTripleState && this->boxState == MiddleState)
	{
		this->boxState = UnCheck;
	}
}
CheckBox::CheckBox(const CheckBox::Config& config, CheckBox::EventHandler *eh)
	: CheckBox(config)
{
	events.SetEventHandler(eh);
}
CheckBox::~CheckBox()
{

}

// -- methods -- //
// private:
int CheckBox::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	case BN_CLICKED:
	case BN_DBLCLK:
		if (boxState == Check)
		{
			if (isTripleState)
			{
				SendMessage(hControl, BM_SETCHECK, BST_INDETERMINATE, 0);
				boxState = MiddleState;
				events.PushEvent(CheckBox::Event(CheckBox::Event::Type::MiddleState));
			}
			else
			{
				SendMessage(hControl, BM_SETCHECK, BST_UNCHECKED, 0);
				boxState = UnCheck;
				events.PushEvent(CheckBox::Event(CheckBox::Event::Type::UnCheck));
			}
		}
		else if (boxState == MiddleState)
		{
			SendMessage(hControl, BM_SETCHECK, BST_UNCHECKED, 0);
			boxState = UnCheck;
			events.PushEvent(CheckBox::Event(CheckBox::Event::Type::UnCheck));
		}
		else if (boxState == UnCheck)
		{
			SendMessage(hControl, BM_SETCHECK, BST_CHECKED, 0);
			boxState = Check;
			events.PushEvent(CheckBox::Event(CheckBox::Event::Type::Check));
		}
		break;

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool CheckBox::CreateControlWindow()
{
	// create window
	hControl = CreateWindow(L"BUTTON", caption.c_str(),
		WS_CHILD | WS_VISIBLE | BS_NOTIFY | BS_CHECKBOX | BS_MULTILINE | BS_AUTO3STATE,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!hControl)
	{
		MessageBox(nullptr, L"CheckBox window creation failed.", L"CheckBox create error", MB_OK | MB_ICONERROR);
		return false;
	}

	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SetBoxState(boxState);

	return true;
}
// public:
void CheckBox::SetCaption(std::wstring newCaption)
{
	caption = newCaption;
	SetWindowText(hControl, caption.c_str());
}
void CheckBox::SetBoxState(CheckBox::BoxState newState)
{
	boxState = newState;
	if (boxState == Check) SendMessage(hControl, BM_SETCHECK, BST_CHECKED, 0);
	if (boxState == MiddleState) SendMessage(hControl, BM_SETCHECK, BST_INDETERMINATE, 0);
	if (boxState == UnCheck) SendMessage(hControl, BM_SETCHECK, BST_UNCHECKED, 0);
}
void CheckBox::SetBoxState(unsigned int newState)
{
	if (newState == 0) SetBoxState(CheckBox::BoxState::UnCheck);
	if (newState == 1) SetBoxState(CheckBox::BoxState::Check);
	if (newState == 2) SetBoxState(CheckBox::BoxState::MiddleState);
}
void CheckBox::SetPosition(int x, int y)
{
	WindowControl::SetPosition(x, y);
	events.PushEvent(CheckBox::Event::Type::Move);
}
void CheckBox::SetDimensions(unsigned int width, unsigned int height)
{
	WindowControl::SetDimensions(width, height);
	events.PushEvent(CheckBox::Event::Type::Resize);
}
// [CLASS] CheckBox ----------------------------|


// [CLASS] TrackBar ------------------------------|
// -- constructor -- //
TrackBar::TrackBar(const TrackBar::Config& config)
	:WindowControl(config)
{

}
TrackBar::TrackBar(const TrackBar::Config& config, TrackBar::EventHandler *eventHandler)
	: TrackBar(config)
{
	events.SetEventHandler(eventHandler);
}
TrackBar::~TrackBar()
{

}

// -- methods -- //
// private:
int TrackBar::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = LOWORD(wParam);
	switch (event)
	{
	case TB_THUMBTRACK:
		events.PushEvent(TrackBar::Event::Type::TrackPosChange);
		break;


	default:
		return 1;	// if did't handle message
	}
	return 0;		// if did
}
bool TrackBar::CreateControlWindow()
{
	// create window
	hControl = CreateWindow(TRACKBAR_CLASS, L"TrackBar",
		WS_CHILD | WS_VISIBLE | TBS_ENABLESELRANGE,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!hControl)
	{
		MessageBox(nullptr, L"TrackBar window creation failed.", L"TrackBar create error", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
// [CLASS] TrackBar ------------------------------|



// [CLASS] Label -------------------------------|
// -- constructors -- //
Label::Label(const Label::Config& config)
	:WindowControl(config),
	Caption(caption),
	Alignment(textAlignment),
	Events(events)
{
	rect = config.rect;
	caption = config.caption;
	textAlignment = config.textAlignment;
}
Label::Label(const Label::Config& config, EventHandler *eh)
	:Label(config)
{
	events.SetEventHandler(eh);
}
Label::~Label()
{

}

// -- methods -- //
// private:
int Label::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	case 0u:
		break;

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool Label::CreateControlWindow()
{
	// set label styles
	unsigned int controlStyle = 0u;
	controlStyle = (WS_CHILD | WS_VISIBLE);

	if (textAlignment == Label::Left)
		controlStyle |= SS_LEFT;
	if (textAlignment == Label::Center)
		controlStyle |= SS_CENTER;
	if (textAlignment == Label::Right)
		controlStyle |= SS_RIGHT;

	// create window
	hControl = CreateWindow(L"STATIC", caption.c_str(),
		controlStyle,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!hControl)
	{
		MessageBox(nullptr, L"Label window creation failed.", L"Label create error", MB_OK | MB_ICONERROR);
		return false;
	}

	// set visual font
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	return true;
}
// public:
void Label::SetCaption(std::wstring newCaption)
{
	caption = newCaption;
	SetWindowText(hControl, caption.c_str());

	events.PushEvent(Label::Event(Label::Event::Type::CaptionChange));
}
void Label::SetTextAligment(Label::TextAlignment textAlignment)
{
	unsigned int newStyle = 0u;
	if (textAlignment == Label::Left)
		newStyle = SS_LEFT;
	if (textAlignment == Label::Center)
		newStyle = SS_CENTER;
	if (textAlignment == Label::Right)
		newStyle = SS_RIGHT;

	unsigned int s = GetWindowLongPtr(hControl, GWL_STYLE);
	s = (s & ~(SS_LEFT | SS_CENTER | SS_RIGHT | SS_LEFTNOWORDWRAP)) | newStyle;
	SetWindowLongPtr(hControl, GWL_STYLE, s);
	InvalidateRect(hControl, NULL, TRUE);

	events.PushEvent(Label::Event(Label::Event::Type::TextAlignmentChange));
}
// [CLASS] Label -------------------------------|

/*
// [CLASS] RadioButton -------------------------|
// -- constructors -- //
RadioButton::RadioButton(const RadioButton::Config& config)
	: WindowControl(config)
{

}
RadioButton::~RadioButton()
{

}

// -- methods -- //
// private:
int RadioButton::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT DefaultMessage = 0u;
	switch (DefaultMessage)
	{
	case 0u:
		break;

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool RadioButton::CreateControlWindow()
{
	// create window
	hControl = CreateWindow(L"BUTTON", L"something",
		WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!hControl)
	{
		MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
		return false;
	}
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	SendMessage(hControl, BM_SETCHECK, 1, 0);

	return true;
}
// [CLASS] RadioButton -------------------------|
*/

/*
// [CLASS] GroupBox ----------------------------|
// -- constructor -- //
GroupBox::GroupBox(const GroupBox::Config& config)
	: WindowControl(config),
	Caption(caption)
{
	caption = config.caption;
}
GroupBox::~GroupBox()
{

}

// -- methods -- //
// private:
int GroupBox::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT DefaultMessage = 0u;
	switch (DefaultMessage)
	{
	case 0u:
		break;

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool GroupBox::CreateControlWindow()
{
	// create window
	hControl = CreateWindow(L"BUTTON", caption.c_str(),
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX | WS_GROUP,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!hControl)
	{
		MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
		return false;
	}

	// set better font
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	return true;
}
// public:
void GroupBox::SetCaption(std::wstring newCaption)
{
	caption = newCaption;
	SetWindowText(hControl, caption.c_str());
}
void GroupBox::SetPosition(int x, int y)
{
	int deltaX = x - rect.x;
	int deltaY = y - rect.y;
	WindowControl::SetPosition(x, y);

	for (WindowControl* wc : controls)
	{
		wc->SetPosition(wc->rect.x + deltaX, wc->rect.y + deltaY);
	}
}
void GroupBox::AddControl(WindowControl *control)
{
	controls.push_back(control);
}
void GroupBox::RemoveControl(WindowControl *control)
{
	for (unsigned int i = 0; i < controls.size(); i++)
	{
		if (controls[i] == control)
		{
			controls.erase(controls.begin() + i);
			return;
		}
	}
}
// [CLASS] GroupBox ----------------------------|
*/

// [CLASS] ProgressBar -------------------------|
// -- constructors -- //
ProgressBar::ProgressBar(const ProgressBar::Config& config)
	: WindowControl(config),
	MinValue(minValue),
	MaxValue(maxValue),
	Position(position),
	Step(step),
	State(barState),
	Events(events)
{
	minValue = config.minValue;
	if (config.maxValue > config.minValue)
		maxValue = config.maxValue;
	else
		maxValue = minValue + 10;

	position = config.position;
	step = config.step;
}
ProgressBar::ProgressBar(const ProgressBar::Config& config, ProgressBar::EventHandler *eh)
	:ProgressBar(config)
{
	events.SetEventHandler(eh);
}
ProgressBar::~ProgressBar()
{

}

// -- methods -- //
// private:
int ProgressBar::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool ProgressBar::CreateControlWindow()
{
	// create window
	hControl = CreateWindow(PROGRESS_CLASS, NULL,
		WS_CHILD | WS_VISIBLE,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!hControl)
	{
		MessageBox(nullptr, L"ProgressBar creation failed.", L"ProgressBar create error", MB_OK | MB_ICONERROR);
		return false;
	}

	// set better font
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	SetRange(minValue, maxValue);
	SetPosition(position);
	SetStep(step);

	return true;
}
// public:
void ProgressBar::SetMinValue(unsigned int value)
{
	minValue = value;
	SendMessage(hControl, PBM_SETRANGE, 0, MAKELPARAM(value, maxValue));
}
void ProgressBar::SetMaxValue(unsigned int value)
{
	maxValue = value;
	SendMessage(hControl, PBM_SETRANGE, 0, MAKELPARAM(maxValue, value));
}
void ProgressBar::SetRange(unsigned int min, unsigned int max)
{
	if (min >= max) max = min + 1;
	minValue = min;
	maxValue = max;

	SendMessage(hControl, PBM_SETRANGE, 0, MAKELPARAM(minValue, maxValue));
}
void ProgressBar::SetStep(unsigned int step)
{
	this->step = step;
	SendMessage(hControl, PBM_SETSTEP, step, 0);
}
void ProgressBar::SetPosition(unsigned int position)
{
	this->position = position;
	SendMessage(hControl, PBM_SETPOS, position, 0);
}
void ProgressBar::SetState(ProgressBar::BarState state)
{
	if (state == BarState::Normal && barState != state)
		SendMessage(hControl, PBM_SETSTATE, 0, PBST_NORMAL);
	else if (state == BarState::Pause && barState != state)
		SendMessage(hControl, PBM_SETSTATE, 0, PBST_PAUSED);
	else if (state == BarState::Error && barState != state)
		SendMessage(hControl, PBM_SETSTATE, 0, PBST_ERROR);

	barState = state;
}
void ProgressBar::StepIt()
{
	SetPosition(position + step);
}
// [CLASS] ProgressBar -------------------------|

/*
// [CLASS] GraphicsBox -------------------------|
// -- constructors -- //
GraphicsBox::GraphicsBox(const GraphicsBox::Config &config)
	:WindowControl(config)
{

}
GraphicsBox::~GraphicsBox()
{
	//// release
	//if (RT)
	//{
	//	RT->Release();
	//	RT = NULL;
	//}
	//if (D2DFactory)
	//{
	//	D2DFactory->Release();
	//	D2DFactory = NULL;
	//}
	//if (D2DBitmap)
	//{
	//	D2DBitmap->Release();
	//	D2DBitmap = NULL;
	//}
}

// -- methods -- //
// private:
int GraphicsBox::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT msg = HIWORD(wParam);
	switch (msg)
	{
	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool GraphicsBox::CreateControlWindow()
{
	// create window
	hControl = CreateWindow(L"BUTTON", NULL,
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!hControl)
	{
		MessageBox(nullptr, L"ProgressBar creation failed.", L"ProgressBar create error", MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}
// public:
void GraphicsBox::InitGraphics2D()
{
	//initialized = true;

	//HRESULT hr;
	//hr = D2D1CreateFactory(
	//	D2D1_FACTORY_TYPE_SINGLE_THREADED,
	//	&D2DFactory
	//);

	//// Obtain the size of the drawing area.
	//GetClientRect(hControl, &rc);

	//// create a Direct2D render target
	//hr = D2DFactory->CreateHwndRenderTarget(
	//	D2D1::RenderTargetProperties(),
	//	D2D1::HwndRenderTargetProperties(
	//		hControl,
	//		D2D1::SizeU(
	//			rc.right - rc.left,
	//			rc.bottom - rc.top)
	//	),
	//	&RT
	//);

	//IWICImagingFactory * pFactory = NULL;
	//IWICBitmap *pBitmap = NULL;

	//UINT uiWidth = 640;
	//UINT uiHeight = 480;
	//WICPixelFormatGUID formatGUID;

	//WICRect rcLock = { 0, 0, uiWidth, uiHeight };
	//IWICBitmapLock *pLock = NULL;

	//HRESULT hr = CoCreateInstance(
	//	CLSID_WICImagingFactory,
	//	NULL,
	//	CLSCTX_INPROC_SERVER,
	//	IID_IWICImagingFactory,
	//	(LPVOID*)&pFactory
	//);

	//if (SUCCEEDED(hr))
	//{
	//	renderTarget.
	//	hr = pFactory->CreateBitmap(uiWidth, uiHeight, formatGUID, WICBitmapCacheOnDemand, &pBitmap);
	//	ID2D1RenderTarget renderTarget;
	//	ID2D1Factory::CreateWic
	//}

	//if (SUCCEEDED(hr))
	//{
	//	hr = pBitmap->Lock(&rcLock, WICBitmapLockWrite, &pLock);

	//	if (SUCCEEDED(hr))
	//	{
	//		UINT cbBufferSize = 0;
	//		UINT cbStride = 0;
	//		BYTE *pv = NULL;

	//		// Retrieve the stride.
	//		hr = pLock->GetStride(&cbStride);

	//		if (SUCCEEDED(hr))
	//		{
	//			hr = pLock->GetDataPointer(&cbBufferSize, &pv);
	//		}
	//		if (SUCCEEDED(hr))
	//		{
	//			for (unsigned int i = 0; i < 2000; i++)
	//			{
	//				*pv = i % 200;
	//				pv += 1;
	//			}
	//			// Access the bitmap memory starting at pv, where
	//			// each row begins cbStride bytes after the start
	//			// of the preceding row.


	//		}

	//		// Release the bitmap lock.
	//		pLock->Release();
	//	}
	//}

	//if (pBitmap)
	//{
	//	pBitmap->Release();
	//}

	//if (pFactory)
	//{
	//	pFactory->Release();
	//}

}
void GraphicsBox::DrawLine(int x, int y)
{
	/*if (!initialized) return;
	RT->BeginDraw();
	RT->DrawBitmap(D2DBitmap);
	RT->EndDraw();*/
	//}*/
	// [CLASS] GraphicsBox -------------------------|
