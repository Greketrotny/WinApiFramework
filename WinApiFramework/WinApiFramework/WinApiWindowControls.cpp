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
	Caption(caption)
{
	caption = config.caption;
}
Button::Button(const Button::Config &config, Button::EventHandler *eventHandler)
	: Button(config)
{
	this->eventHandler = eventHandler;
}
Button::~Button()
{
	delete eventHandler;
	eventHandler = nullptr;
}

// -- methods -- //
// private:
int Button::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT bn = HIWORD(wParam);
	switch (bn)
	{
	case BN_CLICKED:
		PushEvent(Button::Event(Button::Event::Type::Click));
		break;

	case BN_DBLCLK:
		PushEvent(Button::Event(Button::Event::Type::DoubleClick));
		break;

	case BN_SETFOCUS:
		PushEvent(Button::Event(Button::Event::Type::Focus));
		break;

	case BN_KILLFOCUS:
		PushEvent(Button::Event(Button::Event::Type::Unfocus));
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
void Button::PushEvent(Button::Event newEvent)
{
	// push event to buffer
	events.push(newEvent);
	if (events.size() > eventBuffSize)
		events.pop();

	// call eventHandler
	if (eventHandler) eventHandler->HandleEvent(newEvent);
}
Button::Event Button::GetEvent()
{
	if (events.size() > 0u)
	{
		Button::Event e = events.front();
		events.pop();
		return e;
	}
	else
	{
		return Button::Event();
	}
}
void Button::ClearEventBuffer()
{
	events = std::queue<Button::Event>();
}
void Button::SetEventHandler(Button::EventHandler *eh)
{
	eventHandler = eh;
}
void Button::SetCaption(std::wstring newCaption)
{
	caption = newCaption;
	SetWindowText(hControl, caption.c_str());
}
void Button::SetPosition(int x, int y)
{
	WindowControl::SetPosition(x, y);
	PushEvent(Button::Event::Type::Move);
}
void Button::SetDimensions(unsigned int width, unsigned int height)
{
	WindowControl::SetDimensions(width, height);
	PushEvent(Button::Event::Type::Resize);
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
	this->eventHandler = eh;
}
CheckBox::~CheckBox()
{
	eventHandler = nullptr;
}

// -- methods -- //
// private:
int CheckBox::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT bn = HIWORD(wParam);
	switch (bn)
	{
	case BN_CLICKED:
	case BN_DBLCLK:
		if (boxState == Check)
		{
			if (isTripleState)
			{
				SendMessage(hControl, BM_SETCHECK, BST_INDETERMINATE, 0);
				boxState = MiddleState;
				PushEvent(CheckBox::Event(CheckBox::Event::Type::MiddleState));
			}
			else
			{
				SendMessage(hControl, BM_SETCHECK, BST_UNCHECKED, 0);
				boxState = UnCheck;
				PushEvent(CheckBox::Event(CheckBox::Event::Type::UnCheck));
			}
		}
		else if (boxState == MiddleState)
		{
			SendMessage(hControl, BM_SETCHECK, BST_UNCHECKED, 0);
			boxState = UnCheck;
			PushEvent(CheckBox::Event(CheckBox::Event::Type::UnCheck));
		}
		else if (boxState == UnCheck)
		{
			SendMessage(hControl, BM_SETCHECK, BST_CHECKED, 0);
			boxState = Check;
			PushEvent(CheckBox::Event(CheckBox::Event::Type::Check));
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
		MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
		return false;
	}

	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SetBoxState(boxState);

	return true;
}
// public:
void CheckBox::PushEvent(Event newEvent)
{
	// push event to buffer
	events.push(newEvent);
	if (events.size() > eventBuffSize)
		events.pop();

	// call eventHandler
	if (eventHandler) eventHandler->HandleEvent(newEvent);
}
CheckBox::Event CheckBox::GetEvent()
{
	if (events.size() > 0u)
	{
		CheckBox::Event e = events.front();
		events.pop();
		return e;
	}
	else
	{
		return CheckBox::Event();
	}
}
void CheckBox::ClearEventBuffer()
{
	events = std::queue<CheckBox::Event>();
	CheckBox::Event e;
}
void CheckBox::EnableControl()
{
	::EnableWindow(hControl, TRUE);
	PushEvent(CheckBox::Event::Type::Enable);
}
void CheckBox::DisableControl()
{
	::EnableWindow(hControl, FALSE);
	PushEvent(CheckBox::Event::Type::Disable);
}
void CheckBox::SetEventHandler(CheckBox::EventHandler *eh)
{
	eventHandler = eh;
}
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
	PushEvent(CheckBox::Event::Type::Move);
}
void CheckBox::SetDimensions(unsigned int width, unsigned int height)
{
	WindowControl::SetDimensions(width, height);
	PushEvent(CheckBox::Event::Type::Resize);
}
// [CLASS] CheckBox ----------------------------|


// [CLASS] Slider ------------------------------|
// TODO: make slider class after control event handling revolution
// [CLASS] Slider ------------------------------|



// [CLASS] Label -------------------------------|
// -- constructors -- //
Label::Label(const Label::Config& config)
	:WindowControl(config),
	Caption(caption),
	Alignment(textAlignment)
{
	rect = config.rect;
	caption = config.caption;
	textAlignment = config.textAlignment;
}
Label::Label(const Label::Config& config, EventHandler *eh)
	:Label(config)
{
	eventHandler = eh;
}
Label::~Label()
{

}

// -- methods -- //
// private:
int Label::ControlProc(WPARAM wParam, LPARAM lParam)
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
		MessageBox(nullptr, L"Button creation failed.", L"Button create error", MB_OK | MB_ICONERROR);
		return false;
	}

	// set better font
	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hNormalFont, 0);

	return true;
}
// public:
void Label::PushEvent(Label::Event newEvent)
{
	// push event to buffer
	events.push(newEvent);
	if (events.size() > eventBuffSize)
		events.pop();

	// call eventHandler
	if (eventHandler) eventHandler->HandleEvent(newEvent);
}
Label::Event Label::GetEvent()
{
	if (events.size() > 0u)
	{
		Label::Event e = events.front();
		events.pop();
		return e;
	}
	else
	{
		return Label::Event();
	}
}
void Label::ClearEventBuffer()
{
	events = std::queue<Label::Event>();
	Label::Event e;
}
void Label::SetEventHandler(Label::EventHandler *eh)
{
	this->eventHandler = eh;
}
void Label::SetCaption(std::wstring newCaption)
{
	caption = newCaption;
	SetWindowText(hControl, caption.c_str());
}
void Label::SetDimensions(unsigned int width, unsigned int height)
{
	WindowControl::SetDimensions(width, height);
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
}
// [CLASS] Label -------------------------------|


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


// [CLASS] ProgressBar -------------------------|
// -- constructors -- //
ProgressBar::ProgressBar(const ProgressBar::Config& config)
	: WindowControl(config),
	MinValue(minValue),
	MaxValue(maxValue),
	Position(position),
	Step(step),
	State(barState)
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
	eventHandler = eh;
}
ProgressBar::~ProgressBar()
{

}

// -- methods -- //
// private:
int ProgressBar::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT msg = HIWORD(wParam);
	switch (msg)
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
void ProgressBar::PushEvent(ProgressBar::Event newEvent)
{
	// push event to buffer
	events.push(newEvent);
	if (events.size() > eventBuffSize)
		events.pop();

	// call eventHandler
	if (eventHandler) eventHandler->HandleEvent(newEvent);
}
ProgressBar::Event ProgressBar::GetEvent()
{
	if (events.size() > 0u)
	{
		ProgressBar::Event e = events.front();
		events.pop();
		return e;
	}
	else
	{
		return ProgressBar::Event();
	}
}
void ProgressBar::ClearEventBuffer()
{
	events = std::queue<ProgressBar::Event>();
}
void ProgressBar::SetEventHandler(ProgressBar::EventHandler *eh)
{
	eventHandler = eh;
}
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
}
// [CLASS] GraphicsBox -------------------------|