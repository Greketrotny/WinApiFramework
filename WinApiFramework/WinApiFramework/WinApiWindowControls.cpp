#include "WinApiWindowControls.h"
#include "WinApiFramework.h"
#include "WinApiWindow.h"

using namespace WinApiFramework;

#include <chrono>

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
		SWP_NOSIZE);

	PushBaseEvent(WindowControl::Event::Type::Move);
}
void WindowControl::SetDimensions(unsigned int width, unsigned int height)
{
	rect.width = width;
	rect.height = height;

	SetWindowPos(hControl, nullptr,
		rect.x, rect.y,
		rect.width, rect.height,
		SWP_NOMOVE);

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
	controlStyle |= BS_NOTIFY | BS_PUSHBUTTON | BS_MULTILINE;

	// create window
	hControl = CreateWindow(L"BUTTON", caption.c_str(),
		controlStyle,
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
	controlStyle |= BS_NOTIFY | BS_CHECKBOX | BS_MULTILINE | BS_AUTO3STATE;

	// create window
	hControl = CreateWindow(L"BUTTON", caption.c_str(),
		controlStyle,
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
	if (boxState == Check)
	{
		SendMessage(hControl, BM_SETCHECK, BST_CHECKED, 0);
		events.PushEvent(CheckBox::Event(CheckBox::Event::Type::Check));
		return;
	}

	if (boxState == MiddleState && isTripleState)
	{
		SendMessage(hControl, BM_SETCHECK, BST_INDETERMINATE, 0);
		events.PushEvent(CheckBox::Event(CheckBox::Event::Type::MiddleState));
		return;
	}

	if (boxState == UnCheck)
	{
		SendMessage(hControl, BM_SETCHECK, BST_UNCHECKED, 0);
		events.PushEvent(CheckBox::Event(CheckBox::Event::Type::UnCheck));
		return;
	}
}
void CheckBox::SetBoxState(unsigned int newState)
{
	if (newState == 0) { SetBoxState(CheckBox::BoxState::UnCheck); return; }
	if (newState == 1) { SetBoxState(CheckBox::BoxState::Check); return; }
	if (newState == 2 && isTripleState) { SetBoxState(CheckBox::BoxState::MiddleState); return; }
}
void CheckBox::EnableTripleState()
{
	isTripleState = true;
}
void CheckBox::DisableTripleState()
{
	isTripleState = false;

	if (boxState == CheckBox::BoxState::MiddleState)
		SetBoxState(CheckBox::BoxState::UnCheck);
}
// [CLASS] CheckBox ----------------------------|



// [CLASS] TrackBar ------------------------------|
// -- constructor -- //
TrackBar::TrackBar(const TrackBar::Config& config)
	:WindowControl(config),
	Position(thumbPosition),
	TrackMin(trackRange.min),
	TrackMax(trackRange.max),
	Orientation(orientation),
	SmallStep(smallStep),
	LargeStep(largeStep)
{
	thumbPosition = config.startPosition;
	trackRange = config.trackRange;
	selectRange = config.selectRange;
	labels = config.labels;
	orientation = config.orientation;
	smallStep = config.smallStep;
	largeStep = config.largeStep;
	selectRangeEnabled = config.EnableSelectRange;
}
TrackBar::TrackBar(const TrackBar::Config& config, TrackBar::EventHandler *eventHandler)
	: TrackBar(config)
{
	events.SetEventHandler(eventHandler);
}
TrackBar::~TrackBar()
{
	DestroyWindow(hLabel1);
	DestroyWindow(hLabel2);
}

// -- methods -- //
// private:
int TrackBar::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = LOWORD(wParam);
	switch (event)
	{
	case TB_TOP:			// End
	case TB_BOTTOM:			// Home
	case TB_LINEUP:			// Left/Up arrow
	case TB_LINEDOWN:		// Right/Down arrow
	case TB_PAGEUP:			// PageUp
	case TB_PAGEDOWN:		// PageDown
	case TB_THUMBTRACK:		// Mouse drag
		thumbPosition = SendMessage(hControl, TBM_GETPOS, 0, 0);
		events.PushEvent(TrackBar::Event::Type::TrackPosChange);
		break;

	default:
		return 1;	// if did't handle message
	}
	return 0;		// if did
}
bool TrackBar::CreateControlWindow()
{
	if (orientation == TrackBar::Orientation::Horizontal)
		controlStyle |= TBS_HORZ;
	else if (orientation == TrackBar::Orientation::Vertical)
		controlStyle |= TBS_VERT;

	if (selectRangeEnabled)
		controlStyle |= TBS_ENABLESELRANGE;

	// create window
	hControl = CreateWindow(TRACKBAR_CLASS, L"TrackBar",
		controlStyle,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	if (!hControl)
	{
		MessageBox(nullptr, L"TrackBar window creation failed.", L"TrackBar create error", MB_OK | MB_ICONERROR);
		return false;
	}

	SetTrackRange(trackRange);
	SetSelectRange(selectRange);
	SetThumbPosition(thumbPosition);
	SetSmallStep(smallStep);
	SetLargeStep(largeStep);



	// create and set side labels
	hLabel1 = CreateWindow(L"STATIC", labels.label1.c_str(), SS_CENTER | WS_CHILD | WS_VISIBLE,
		0, 0, 50, 10, parentWindow->WndHandle, NULL, Framework::ProgramInstance, NULL);
	hLabel2 = CreateWindow(L"STATIC", labels.label2.c_str(), SS_CENTER | WS_CHILD | WS_VISIBLE,
		0, 0, 50, 10, parentWindow->WndHandle, NULL, Framework::ProgramInstance, NULL);

	HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hLabel1, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
	SendMessage(hLabel2, WM_SETFONT, (WPARAM)hNormalFont, TRUE);

	SendMessage(hControl, TBM_SETBUDDY, TRUE, (LPARAM)hLabel1);
	SendMessage(hControl, TBM_SETBUDDY, FALSE, (LPARAM)hLabel2);

	return true;
}
// public:
void TrackBar::SetPosition(int x, int y)
{
	RECT label1Rect, label2Rect;
	GetWindowRect(hLabel1, &label1Rect);
	GetWindowRect(hLabel2, &label2Rect);
	//SetWindowPos(hLabel1, NULL, label1Rect.left - parentWindow->X - (rect.x - x), label1Rect.top - parentWindow->Y - (rect.y - y), 0, 0, SWP_NOSIZE);
	//SetWindowPos(hLabel2, NULL, label2Rect.left - parentWindow->X - (rect.x - x), label2Rect.top - parentWindow->Y - (rect.y - y), 0, 0, SWP_NOSIZE);

	WindowControl::SetPosition(x, y);

	//SendMessage(hControl, TBM_SETBUDDY, TRUE, (LPARAM)hLabel1);
	//SendMessage(hControl, TBM_SETBUDDY, FALSE, (LPARAM)hLabel2);
}
void TrackBar::SetMinTrackValue(int value)
{
	if (value < trackRange.max)
	{
		trackRange.min = value;
		SendMessage(hControl, TBM_SETRANGEMIN, TRUE, value);
		SendMessage(hControl, TBM_SETTICFREQ, 1, 0);
	}
}
void TrackBar::SetMaxTrackValue(int value)
{
	if (value > trackRange.min)
	{
		trackRange.max = value;
		SendMessage(hControl, TBM_SETRANGEMAX, TRUE, value);
		SendMessage(hControl, TBM_SETTICFREQ, 1, 0);
	}
}
void TrackBar::SetMinSelectValue(int value)
{
	if (value > trackRange.min && value < selectRange.max)
	{
		selectRange.min = value;
		SendMessage(hControl, TBM_SETSELSTART, TRUE, value);
	}
}
void TrackBar::SetMaxSelectValue(int value)
{
	if (value < trackRange.max && value > selectRange.min)
	{
		SendMessage(hControl, TBM_SETSELEND, TRUE, value);
	}
}
void TrackBar::SetTrackRange(TrackBar::Range newRange)
{
	SetTrackRange(newRange.min, newRange.max);
}
void TrackBar::SetTrackRange(int minValue, int maxValue)
{
	if (minValue < maxValue)
	{
		if (minValue >= trackRange.max)
		{
			SendMessage(hControl, TBM_SETRANGEMAX, TRUE, maxValue);
			SendMessage(hControl, TBM_SETRANGEMIN, TRUE, minValue);
		}
		else
		{
			SendMessage(hControl, TBM_SETRANGEMIN, TRUE, minValue);
			SendMessage(hControl, TBM_SETRANGEMAX, TRUE, maxValue);
		}

		trackRange.min = minValue;
		trackRange.max = maxValue;
	}
}
void TrackBar::SetSelectRange(TrackBar::Range newRange)
{
	SetSelectRange(newRange.min, newRange.max);
}
void TrackBar::SetSelectRange(int minValue, int maxValue)
{
	if (selectRangeEnabled)
	{
		SendMessage(hControl, TBM_SETSEL, TRUE, minValue | (maxValue << 16));
	}
}
void TrackBar::SetThumbPosition(int newThumbPosition)
{
	if (newThumbPosition < trackRange.min) newThumbPosition = trackRange.min;
	if (newThumbPosition > trackRange.max) newThumbPosition = trackRange.max;

	thumbPosition = newThumbPosition;
	SendMessage(hControl, TBM_SETPOS, TRUE, thumbPosition);
}
void TrackBar::SetSmallStep(unsigned int smallStep)
{
	this->smallStep = smallStep;
	SendMessage(hControl, TBM_SETLINESIZE, 0, smallStep);
}
void TrackBar::SetLargeStep(unsigned int largeStep)
{
	this->largeStep = largeStep;
	SendMessage(hControl, TBM_SETPAGESIZE, 0, largeStep);
}
void TrackBar::EnableSelectRange()
{
	if (!selectRangeEnabled)
	{
		selectRangeEnabled = true;
		controlStyle |= TBS_ENABLESELRANGE;
		SetWindowLong(hControl, GWL_STYLE, controlStyle);
	}
}
void TrackBar::DisableSelectRange()
{
	if (selectRangeEnabled)
	{
		selectRangeEnabled = false;
		controlStyle = controlStyle & (~TBS_ENABLESELRANGE);
		SetWindowLong(hControl, GWL_STYLE, controlStyle);
	}
}
void TrackBar::SetLabel1(const std::wstring& label1)
{
	labels.label1 = label1;
	SetWindowText(hLabel1, labels.label1.c_str());
}
void TrackBar::SetLabel2(const std::wstring& label2)
{
	labels.label2 = label2;
	SetWindowText(hLabel2, labels.label2.c_str());
}
void TrackBar::SetLabels(const std::wstring& label1, const std::wstring& label2)
{
	SetLabel1(label1);
	SetLabel2(label2);
}
void TrackBar::SetLabels(const TrackBar::Labels& newLabels)
{
	SetLabel1(newLabels.label1);
	SetLabel2(newLabels.label2);
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

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool Label::CreateControlWindow()
{
	// set text alignment
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

	controlStyle = (controlStyle & ~(SS_LEFT | SS_CENTER | SS_RIGHT | SS_LEFTNOWORDWRAP)) | newStyle;
	SetWindowLong(hControl, GWL_STYLE, controlStyle);
	InvalidateRect(hControl, NULL, TRUE);

	events.PushEvent(Label::Event(Label::Event::Type::TextAlignmentChange));
}
// [CLASS] Label -------------------------------|



// [CLASS] Edit --------------------------------|
// -- constructors -- //
Edit::Edit(const Edit::Config& config)
	:WindowControl(config),
	PasswordMode(passwordMode),
	NumberOnlyMode(numberOnly),
	Alignment(textAlignment),
	LetterMode(lettersMode)
{
	text = config.text;
	passwordMode = config.passwordMode;
	numberOnly = config.numberOnly;
	textAlignment = config.textAlignment;
	lettersMode = config.lettersMode;
	textLengthLimit = config.textLengthLimit;

	controlStyle |= WS_BORDER;
}
Edit::Edit(const Edit::Config& config, Edit::EventHandler *eh)
	:Edit(config)
{
	events.SetEventHandler(eh);
}
Edit::~Edit()
{

}

// -- methods -- //
// private:
int Edit::ControlProc(WPARAM wParam, LPARAM lParam)
{
	UINT event = HIWORD(wParam);
	switch (event)
	{
	case EN_MAXTEXT:
		events.PushEvent(Edit::Event(Edit::Event::Type::TextLimitReach));
		break;

	case EN_UPDATE:
		events.PushEvent(Edit::Event(Edit::Event::Type::Text));
		break;

	default:
		return 1;	// if did't handle
	}
	return 0;		// if did
}
bool Edit::CreateControlWindow()
{
	// set text alignment
	if (textAlignment == Edit::TextAlignment::Left)
		controlStyle |= ES_LEFT;
	if (textAlignment == Edit::TextAlignment::Center)
		controlStyle |= ES_CENTER;
	if (textAlignment == Edit::TextAlignment::Right)
		controlStyle |= ES_RIGHT;

	// set letters mode
	if (lettersMode == Edit::LettersMode::LowerCase)
		controlStyle |= ES_LOWERCASE;
	if (lettersMode == Edit::LettersMode::UpperCase)
		controlStyle |= ES_UPPERCASE;

	// set password mode
	if (passwordMode)
		controlStyle |= ES_PASSWORD;

	// set number only mode
	if (numberOnly)
		controlStyle |= ES_NUMBER;


	// create window
	hControl = CreateWindow(L"EDIT", text.c_str(),
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

	SetTextLengthLimit(textLengthLimit);
	return true;
}
// public:
void Edit::SetText(std::wstring newText)
{
	text = newText;
	SetWindowText(hControl, text.c_str());

	events.PushEvent(Edit::Event(Edit::Event::Type::Text));
}
void Edit::SetTextAlignment(Edit::TextAlignment newTextAlignment)
{
	// set text alignment
	unsigned int newAlignment = 0u;
	if (newTextAlignment == Edit::TextAlignment::Left)
		newAlignment |= ES_LEFT;
	if (newTextAlignment == Edit::TextAlignment::Center)
		newAlignment |= ES_CENTER;
	if (newTextAlignment == Edit::TextAlignment::Right)
		newAlignment |= ES_RIGHT;

	controlStyle = (controlStyle & ~(ES_LEFT | ES_CENTER | ES_RIGHT) | newAlignment);
	SetWindowLong(hControl, GWL_STYLE, controlStyle);

	events.PushEvent(Edit::Event(Edit::Event::Type::TextAlignment));
}
void Edit::SetLettersMode(Edit::LettersMode newLettersMode)
{
	lettersMode = newLettersMode;

	unsigned int newMode = 0u;
	if (newLettersMode == Edit::LettersMode::LowerCase)
		newMode |= ES_LOWERCASE;
	if (newLettersMode == Edit::LettersMode::UpperCase)
		newMode |= ES_UPPERCASE;

	controlStyle = (controlStyle & ~(ES_LOWERCASE | ES_UPPERCASE) | newMode);
	SetWindowLong(hControl, GWL_STYLE, controlStyle);

	events.PushEvent(Edit::Event(Edit::Event::Type::LettersMode));
}
void Edit::SetPasswordMode(bool newPasswordMode)
{
	if (passwordMode != newPasswordMode)
	{
		passwordMode = newPasswordMode;

		controlStyle ^= ES_PASSWORD;
		SetWindowLong(hControl, GWL_STYLE, controlStyle);

		events.PushEvent(Edit::Event(Edit::Event::Type::PasswordMode));
	}
}
void Edit::SetNumberOnlyMode(bool numberOnlyMode)
{
	if (numberOnly != numberOnlyMode)
	{
		numberOnly = numberOnlyMode;

		if (numberOnly)
			controlStyle |= ES_NUMBER;
		else
			controlStyle = (controlStyle & (~ES_NUMBER));
		SetWindowLong(hControl, GWL_STYLE, controlStyle);

		events.PushEvent(Edit::Event(Edit::Event::Type::NumberMode));
	}
}
void Edit::SetTextLengthLimit(unsigned int lengthLimit)
{
	textLengthLimit = lengthLimit;
	SendMessage(hControl, EM_LIMITTEXT, textLengthLimit, 0u);
}
std::wstring Edit::GetText()
{
	wchar_t* buffer = new wchar_t[textLengthLimit];
	GetWindowText(hControl, buffer, textLengthLimit);
	text = buffer;
	return text;
}
// [CLASS] Edit --------------------------------|


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



// [CLASS] GraphicsBox -------------------------|
// -- constructors -- //
GraphicsBox::GraphicsBox(const GraphicsBox::Config &config)
	:WindowControl(config)
{
	pixelMap = new PixelMap(rect.width, rect.height);
}
GraphicsBox::~GraphicsBox()
{
	delete pixelMap;

	delete cBitmap;
	delete bitmap;
	delete graphics;
	ReleaseDC(hControl, hdc);

	Gdiplus::GdiplusShutdown(gdiplusToken);
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
	controlStyle |= WS_BORDER;

	// create window
	hControl = CreateWindow(L"STATIC", NULL,
		controlStyle,
		rect.x, rect.y, rect.width, rect.height,
		parentWindow->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

	// check control creation
	if (!hControl)
	{
		MessageBox(nullptr, L"ProgressBar creation failed.", L"ProgressBar create error", MB_OK | MB_ICONERROR);
		return false;
	}


	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	hdc = GetDC(hControl);
	graphics = new Gdiplus::Graphics(hdc);
	bitmap = new Gdiplus::Bitmap(rect.width, rect.height, PixelFormat32bppARGB);

	return true;
}
// public:
/*void GraphicsBox::InitGraphics2D()
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

}*/
void GraphicsBox::Render()
{
	// lock bitmap
	Gdiplus::Rect lockRect(0, 0, rect.width, rect.height);
	Gdiplus::BitmapData bitmapData;

	bitmap->LockBits
	(
		&lockRect,
		Gdiplus::ImageLockMode::ImageLockModeWrite,
		PixelFormat32bppARGB, &bitmapData
	);

	// copy data to bitmap ------
	unsigned int *pixelIn = (unsigned int*)bitmapData.Scan0;
	unsigned int *pixelOut = pixelMap->GetFirstAddress();
	unsigned int *lastPixel = pixelOut + rect.width * rect.height;
	while (pixelOut != lastPixel)
		*pixelIn++ = *pixelOut++;
	// ---------------------------

	// unlock bitmap
	bitmap->UnlockBits(&bitmapData);

	// draw bitmap on control
	cBitmap = new Gdiplus::CachedBitmap(bitmap, graphics);
	graphics->DrawCachedBitmap(cBitmap, 0, 0);
	delete cBitmap;
}
void GraphicsBox::SetPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b)
{
	pixelMap->SetPixel(x, y, r, g, b);
}
// [CLASS] GraphicsBox -------------------------|
