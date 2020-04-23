#include "Precompiled.h"
#include "CheckBox.h"
#include "WinApiFramework.h"

using namespace WinApiFramework;


// [CLASS] CheckBox ----------------------------|
// -- constructors -- //
CheckBox::CheckBox(const CheckBox::ConStruct& conStruct)
	: WindowControl(conStruct),
	State(boxState),
	Caption(caption),
	IsTripleState(isTripleState),
	Events(events)
{
	caption = conStruct.caption;
	isTripleState = conStruct.isTripleState;
	boxState = conStruct.boxState;

	if (!this->isTripleState && this->boxState == MiddleState)
	{
		this->boxState = UnCheck;
	}
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
		rect.position.x, rect.position.y, rect.size.width, rect.size.height,
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