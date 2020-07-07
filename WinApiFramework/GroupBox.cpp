#include "Precompiled.h"
#include "GroupBox.h"
#include "WinApiFramework.h"

namespace WinApiFramework
{
	// ~~~~~~~~ [CLASS] GroupBox ~~~~~~~~ //
	// ~~ GroupBox::constructor ~~ //
	GroupBox::GroupBox(ParentControl* parentControl, const ConStruct<GroupBox>& conStruct)
		: ChildControl(parentControl, conStruct)
	{
		m_caption = conStruct.caption;

		CreateControlWindow();
	}
	GroupBox::~GroupBox()
	{
		DestroyControlWindow();
	}

	// ~~ GroupBox::methods ~~ //
	int GroupBox::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		UINT event = HIWORD(wParam);
		switch (event)
		{
			default:	return 1;	// if did't handle
		}
		return 0;	// if did
	}
	bool GroupBox::CreateControlWindow()
	{
		m_controlStyle |= (BS_NOTIFY | BS_GROUPBOX);
		m_controlStyle |= BS_HOLLOW;

		// create window
		m_hWindow = CreateWindow(L"BUTTON", m_caption.c_str(),
			m_controlStyle,
			m_rect.position.x, m_rect.position.y, m_rect.size.width, m_rect.size.height,
			m_pParentControl->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"GroupBox creation failed.", L"GroupBox create error", MB_OK | MB_ICONERROR);
			return false;
		}

		HFONT hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hNormalFont, 0);

		return true;
	}
	void GroupBox::DestroyControlWindow()
	{
		::DestroyWindow(m_hWindow);
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
}