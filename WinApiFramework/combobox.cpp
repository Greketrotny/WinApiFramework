#include "combobox.h"
#include "winapi_framework.h"

namespace WinapiFramework
{
	// ~~~~~~~~ [CLASS] ComboBoxBase ~~~~~~~~
	ComboBoxBase::ComboBoxBase(ParentWindow* parent)
		: BaseWindow(parent)
		, m_item_count(0u)
		, m_drop_width(0u)
	{
	}
	ComboBoxBase::~ComboBoxBase()
	{
	}

	LRESULT ComboBoxBase::ComboBoxBaseProcedure(WPARAM wParam, LPARAM lParam)
	{
		switch (HIWORD(wParam))
		{
			case CBN_DROPDOWN:
				RaiseEventByHandler<ComboBoxBaseEvents::EventDropDown>();
				break;
			case CBN_CLOSEUP:
				RaiseEventByHandler<ComboBoxBaseEvents::EventCloseUp>();
				break;

			case CBN_SETFOCUS:
				RaiseEventByHandler<ComboBoxBaseEvents::EventSetFocus>();
				break;
			case CBN_KILLFOCUS:
				RaiseEventByHandler<ComboBoxBaseEvents::EventKillFocus>();
				break;

			case CBN_DBLCLK:
				RaiseEventByHandler<ComboBoxBaseEvents::EventDoubleClick>();
				break;

				// Sent when the user changes the current selection 
				// in the list box of a combo box. The user can change
				// the selection by clicking in the list box or by using the arrow keys.
			case CBN_SELCHANGE:
				RaiseEventByHandler<ComboBoxBaseEvents::EventSelectionChange>();
				break;

				// Sent when the user selects an item, but then selects
				// another control or closes the dialog box.
				// It indicates the user's initial selection is to be ingored.
			case CBN_SELENDCANCEL:
				RaiseEventByHandler<ComboBoxBaseEvents::EventSelectionCancel>();
				break;

				// Sent when the user selects a list item, or selects an item and then
				// closes the list. It indicates that the user's selection
				// is to be processed.
			case CBN_SELENDOK:
				RaiseEventByHandler<ComboBoxBaseEvents::EventSelectionAccept>();
				break;

			default: return 1;
		}
		return 0;
	}

	void ComboBoxBase::AddItem(const std::wstring& str)
	{
		LRESULT result = SendMessage(m_hWindow, CB_ADDSTRING, WPARAM(0), LPARAM(str.c_str()));
		if (result == CB_ERR)
		{
			Framework::ShowGlobalMessageBox(L"ComboBox error", L"Failed to add item",
				MessBoxButtonLayout::Ok, MessBoxIcon::Error);
		}
		else if (result == CB_ERRSPACE)
		{
			Framework::ShowGlobalMessageBox(L"ComboBox error", L"Failed to add item (insufficient space)",
				MessBoxButtonLayout::Ok, MessBoxIcon::Error);
		}

		m_item_count++;
		RaiseEventByHandler<ComboBoxBaseEvents::EventAddItem>();
	}
	void ComboBoxBase::InsertItem(const std::wstring& str, const unsigned int index)
	{
		if (index < m_item_count)
		{
			LRESULT result = SendMessage(m_hWindow, CB_INSERTSTRING, WPARAM(index), LPARAM(str.c_str()));
			if (result == CB_ERR)
			{
				Framework::ShowGlobalMessageBox(L"ComboBox error", L"Failed to insert item",
					MessBoxButtonLayout::Ok, MessBoxIcon::Error);
			}
			else if (result == CB_ERRSPACE)
			{
				Framework::ShowGlobalMessageBox(L"ComboBox error", L"Failed to insert item (insufficient space)",
					MessBoxButtonLayout::Ok, MessBoxIcon::Error);
			}

			m_item_count++;
			RaiseEventByHandler<ComboBoxBaseEvents::EventInsertItem>();
		}
	}
	void ComboBoxBase::RemoveItem(const unsigned int index)
	{
		if (index < m_item_count)
		{
			LRESULT result = SendMessage(m_hWindow, CB_DELETESTRING, WPARAM(index), 0);
			if (result != CB_ERR)
			{
				m_item_count = result;
				RaiseEventByHandler<ComboBoxBaseEvents::EventRemoveItem>();
			}
		}
	}
	void ComboBoxBase::SetItem(const unsigned int index, const std::wstring& new_value)
	{
		if (index < m_item_count)
		{
			SendMessage(m_hWindow, CB_SETITEMDATA, WPARAM(index), LPARAM(new_value.c_str()));
			RaiseEventByHandler<ComboBoxBaseEvents::EventSetItem>();
		}
	}
	std::wstring ComboBoxBase::GetItem(const unsigned int index)
	{
		if (index < m_item_count)
		{
			const unsigned int buff_size = SendMessage(m_hWindow, CB_GETLBTEXTLEN, index, 0);
			wchar_t* const buff = new wchar_t[buff_size + 1];

			SendMessage(m_hWindow, CB_GETLBTEXT, index, LPARAM(buff));
			buff[buff_size] = '\0';
			std::wstring item(buff);

			delete[] buff;
			return item;
		}
		else
		{
			return L"";
		}
	}
	void ComboBoxBase::DropDownList()
	{
		SendMessage(m_hWindow, CB_SHOWDROPDOWN, WPARAM(TRUE), 0);
	}
	void ComboBoxBase::CloseUpList()
	{
		SendMessage(m_hWindow, CB_SHOWDROPDOWN, WPARAM(FALSE), 0);
	}
	void ComboBoxBase::Clear()
	{
		SendMessage(m_hWindow, CB_RESETCONTENT, 0, 0);
		m_item_count = 0u;
		RaiseEventByHandler<ComboBoxBaseEvents::EventClear>();
	}
	void ComboBoxBase::SelectItem(const unsigned int index)
	{
		if (index < m_item_count)
		{
			SendMessage(m_hWindow, CB_SETCURSEL, index, 0);
			RaiseEventByHandler<ComboBoxBaseEvents::EventSelectItem>();
		}
	}

	void ComboBoxBase::SetMinDropWidth(int drop_width)
	{
		m_drop_width = std::max(drop_width, 5);
		SendMessage(m_hWindow, CB_SETDROPPEDWIDTH, m_drop_width, 0);
		RaiseEventByHandler<ComboBoxBaseEvents::EventSetMinDropWidth>();
	}
	void ComboBoxBase::SetVisibleItemsCount(const unsigned int count)
	{
		SendMessage(m_hWindow, CB_SETMINVISIBLE, count, 0);
		RaiseEventByHandler<ComboBoxBaseEvents::EventSetVisibleItemsCount>();
	}

	int ComboBoxBase::GetMinDropWidth()
	{
		return m_drop_width;
	}
	unsigned int ComboBoxBase::GetItemCount()
	{
		m_item_count = SendMessage(m_hWindow, CB_GETCOUNT, 0, 0);
		return m_item_count;
	}
	unsigned int ComboBoxBase::GetSelectedItemIndex()
	{
		return SendMessage(m_hWindow, CB_GETCURSEL, 0, 0);
	}
	bool ComboBoxBase::IsAnyItemSelected()
	{
		return (SendMessage(m_hWindow, CB_GETCURSEL, 0, 0) == -1) ? false : true;
	}
	bool ComboBoxBase::IsDroppedDown()
	{
		return SendMessage(m_hWindow, CB_GETDROPPEDSTATE, 0, 0);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] ComboBox ~~~~~~~~
	ComboBox::ComboBox(ParentWindow* parent, const ConStruct<ComboBox>& conStruct)
		: ComboBoxBase(parent)
		, HasSubclassProcedure(this, &ComboBox::SubclassProcedure)
	{
		m_rect = conStruct.rect;

		CreateWinapiWindow();
	}
	ComboBox::~ComboBox()
	{
		DestroyWinapiWindow();
	}

	LRESULT ComboBox::SubclassProcedure(
		HWND hWnd,
		UINT msg,
		WPARAM wParam, LPARAM lParam,
		UINT_PTR uIDSubClass, DWORD_PTR dwRefData)
	{
		// We do not return after succesful mouse event handling
		// becuase DefSubclassProc must be called for ComboBox
		// from Framework SubclassProcedure
		HandleMouseEvent(msg, wParam, lParam);

		return 1;
	}
	LRESULT ComboBox::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		if (ComboBoxBase::ComboBoxBaseProcedure(wParam, lParam) == 0) return 0;

		return 1;
	}
	bool ComboBox::CreateWinapiWindow()
	{
		// [>] Set ComboBox styles
		// set ComboBox appearance
		m_window_style |= WS_CHILD | WS_VISIBLE | WS_VSCROLL;
		m_window_style |= CBS_HASSTRINGS | CBS_DISABLENOSCROLL | CBS_DROPDOWNLIST;


		// [>] Create window
		m_hWindow = CreateWindow(WC_COMBOBOX, TEXT(""),
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"ComboBox creation failed.", L"ComboBox create error", MB_OK | MB_ICONERROR);
			return false;
		}

		SetWindowSubclass(m_hWindow, GetSubclassProcedure(), 0, 0);

		// set pointer to non-static std::function to receive WM_ messages
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_subclass_procedure);

		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hFont, 0);

		return true;
	}
	void ComboBox::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
		RemoveWindowSubclass(m_hWindow, GetSubclassProcedure(), 0);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] EditComboBox ~~~~~~~~
	EditComboBox::EditComboBox(ParentWindow* parent, const ConStruct<EditComboBox>& conStruct)
		: ComboBoxBase(parent)
		, HasSubclassProcedure(this, &EditComboBox::SubclassProcedure)
		, m_cue_banner(L"")
	{
		m_rect = conStruct.rect;

		CreateWinapiWindow();
	}
	EditComboBox::~EditComboBox()
	{
		DestroyWinapiWindow();
	}

	LRESULT EditComboBox::SubclassProcedure(
		HWND hWnd,
		UINT msg,
		WPARAM wParam, LPARAM lParam,
		UINT_PTR uIDSubClass, DWORD_PTR dwRefData)
	{
		// We do not return after succesful mouse event handling
		// becuase DefSubclassProc must be called for EditComboBox
		// from Framework SubclassProcedure
		HandleMouseEvent(msg, wParam, lParam);

		return 1;
	}
	LRESULT EditComboBox::ControlProcedure(WPARAM wParam, LPARAM lParam)
	{
		if (ComboBoxBase::ComboBoxBaseProcedure(wParam, lParam) == 0) return 0;

		switch (HIWORD(wParam))
		{
				// Sent after the user has taken an action that may have altered the text
				// in the edit control portion of a combo box.
				// Sent after the system updates the screen
			case CBN_EDITCHANGE:
				RaiseEventByHandler<Events::EditChange>();
				break;

				// Sent when the edit control portion of a combo box 
				// is about to display altered text.
			case CBN_EDITUPDATE:
				RaiseEventByHandler<Events::EditUpdate>();
				break;


			default: return 1;
		}
		return 0;
	}
	bool EditComboBox::CreateWinapiWindow()
	{
		// [>] Set EditComboBox styles
		// set EditComboBox appearance
		m_window_style |= WS_CHILD | WS_VISIBLE | WS_VSCROLL;
		m_window_style |= CBS_HASSTRINGS | CBS_DISABLENOSCROLL | CBS_DROPDOWN | CBS_AUTOHSCROLL;


		// [>] Create window
		m_hWindow = CreateWindow(WC_COMBOBOX, TEXT(""),
			m_window_style,
			m_rect.position.x - mp_parent->GetCanvasPosition().x,
			m_rect.position.y - mp_parent->GetCanvasPosition().y,
			m_rect.size.width, m_rect.size.height,
			mp_parent->GetWindowHandle(), nullptr, Framework::ProgramInstance, nullptr);

		if (!m_hWindow)
		{
			MessageBox(nullptr, L"EditComboBox creation failed.", L"EditComboBox create error", MB_OK | MB_ICONERROR);
			return false;
		}

		SetWindowSubclass(m_hWindow, GetSubclassProcedure(), 0, 0);

		// set pointer to non-static std::function to receive WM_ messages
		SetWindowLongPtr(m_hWindow, GWLP_USERDATA, (LONG_PTR)&m_subclass_procedure);

		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(m_hWindow, WM_SETFONT, (WPARAM)hFont, 0);

		return true;
	}
	void EditComboBox::DestroyWinapiWindow()
	{
		::DestroyWindow(m_hWindow);
		RemoveWindowSubclass(m_hWindow, GetSubclassProcedure(), 0);
	}

	void EditComboBox::SetEditTextLimit(int length)
	{
		length = std::max(length, 0);
		SendMessage(m_hWindow, CB_LIMITTEXT, length, 0);
		RaiseEventByHandler<Events::SetEditTextLimit>();
	}
	void EditComboBox::SetCueBanner(const std::wstring& cue_banner)
	{
		m_cue_banner = cue_banner;
		SendMessage(m_hWindow, CB_SETCUEBANNER, 0, (LPARAM)m_cue_banner.c_str());
		RaiseEventByHandler<Events::SetCueBanner>();
	}

	void EditComboBox::SetSelection(int start_index, int end_index)
	{
		start_index = std::max(start_index, 0);
		end_index = std::max(end_index, start_index);

		SendMessage(m_hWindow, CB_SETEDITSEL, 0, MAKELPARAM(start_index, end_index));
		RaiseEventByHandler<Events::SetSelection>();
	}
	void EditComboBox::SetSelection(const Range& range)
	{
		SetSelection(range.min, range.max);
	}
	void EditComboBox::SelectAll()
	{
		SendMessage(m_hWindow, CB_SETEDITSEL, 0, MAKELPARAM(0, -1));
		RaiseEventByHandler<Events::SelectAll>();
	}
	void EditComboBox::RemoveSelection()
	{
		SendMessage(m_hWindow, CB_SETEDITSEL, 0, MAKELPARAM(-1, 0));
		RaiseEventByHandler<Events::RemoveSelection>();
	}

	const std::wstring& EditComboBox::GetCueBanner()
	{
		return m_cue_banner;
	}
	Range EditComboBox::GetSelection()
	{
		Range range;
		SendMessage(m_hWindow, CB_GETEDITSEL, (WPARAM)&range.min, (LPARAM)&range.max);
		return range;
	}
}