#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "base_window.h"

namespace WinapiFramework
{
	// ~~~~~~~~ [CLASS] ComboBoxBase ~~~~~~~~
	class ComboBoxBase
		: public BaseWindow
	{
	private:
		unsigned int m_item_count;
		unsigned int m_drop_width;
	public:
		struct ComboBoxBaseEvents : public BaseWindowEvents
		{
			// Sent when the list box of a combo box is about 
			// to be made visible.
			struct EventDropDown : public BaseEvent {};

			// Sent when the list box of a combo box has been closed.
			struct EventCloseUp : public BaseEvent {};

			// Sent when a combo box receivs the keyboard focus.
			struct EventSetFocus : public BaseEvent {};

			// Sent when a combo box loses the keyboard focus.
			struct EventKillFocus : public BaseEvent {};

			// Sent when the user double-clicks a string in the list box
			// of a combo box.
			struct EventDoubleClick : public BaseEvent {};

			// Sent when the user changes the current selection 
			// in the list box of a combo box.
			struct EventSelectionChange : public BaseEvent {};

			// Sent when the user selects an item, but then clicks away.
			struct EventSelectionCancel : public BaseEvent {};

			// Sent when the user selects a list item, and closes the list.
			struct EventSelectionAccept : public BaseEvent {};


			struct EventAddItem : public BaseEvent {};
			struct EventInsertItem : public BaseEvent {};
			struct EventRemoveItem : public BaseEvent {};
			struct EventSetItem : public BaseEvent {};
			struct EventClear : public BaseEvent {};
			struct EventSelectItem : public BaseEvent {};

			struct EventSetMinDropWidth : public BaseEvent {};
			struct EventSetVisibleItemsCount : public BaseEvent {};
		};


	protected:
		ComboBoxBase(const ComboBoxBase& other) = delete;
		ComboBoxBase(ComboBoxBase&& other) = delete;
		ComboBoxBase(ParentWindow* parent);
		~ComboBoxBase();


	protected:
		LRESULT ComboBoxBaseProcedure(WPARAM wParam, LPARAM lParam);
	public:
		void AddItem(const std::wstring& str);
		void InsertItem(const std::wstring& str, const unsigned int index);
		void RemoveItem(const unsigned int index);
		void SetItem(const unsigned int index, const std::wstring& new_value);
		std::wstring GetItem(const unsigned int index);
		void DropDownList();
		void CloseUpList();
		void Clear();
		void SelectItem(const unsigned int index);

		void SetMinDropWidth(int width);
		void SetVisibleItemsCount(const unsigned int count);

		int GetMinDropWidth();
		unsigned int GetItemCount();
		unsigned int GetSelectedItemIndex();
		bool IsAnyItemSelected();
		bool IsDroppedDown();
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	class ComboBox;
	template <> struct ConStruct<ComboBox>;

	// ~~~~~~~~ [CLASS] ComboBox ~~~~~~~~
	class ComboBox
		: public ComboBoxBase
		, public HasSubclassProcedure<ComboBox>
	{
	public:
		struct Events : public ComboBoxBaseEvents
		{

		};


	private:
		ComboBox(const ComboBox& other) = delete;
		ComboBox(ComboBox&& other) = delete;
		ComboBox(ParentWindow* parent, const ConStruct<ComboBox>& conStruct);
		~ComboBox();


	private:
		ComboBox& operator=(const ComboBox& other) = delete;
		ComboBox& operator=(ComboBox&& other) = delete;


	private:
		LRESULT SubclassProcedure(
			HWND hWnd,
			UINT msg,
			WPARAM wParam, LPARAM lParam,
			UINT_PTR uIDSubClass, DWORD_PTR dwRefData) override;
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;
	


	public:
		friend class ObjectCreator;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	template <> struct ConStruct<ComboBox>
	{
		Rect rect;

		ConStruct(const Rect& rect = Rect(50, 50, 100, 50))
			: rect(rect)
		{}
	};



	class EditComboBox;
	template <> struct ConStruct<EditComboBox>;

	// ~~~~~~~~ [CLASS] EditComboBox ~~~~~~~~
	class EditComboBox
		: public ComboBoxBase
		, public HasSubclassProcedure<EditComboBox>
	{
	private:
		std::wstring m_cue_banner;

	public:
		struct Events : public ComboBoxBaseEvents
		{
			// Sent after the user has taken an action that may have altered
			// the text in the edit control portion of a combo box.
			// Sent after the system updates the screen.
			struct EditChange : public BaseEvent {};

			// Sent when the edit control portion of a combo box is about
			// to display altered text.
			// Sent after the control has formatted the text,
			// but before it displays the text.
			struct EditUpdate : public BaseEvent {};

			struct SetEditTextLimit : public BaseEvent {};
			struct SetCueBanner : public BaseEvent {};

			struct SetSelection : public BaseEvent {};
			struct SelectAll : public BaseEvent {};
			struct RemoveSelection : public BaseEvent {};
		};


	private:
		EditComboBox(const EditComboBox& other) = delete;
		EditComboBox(EditComboBox&& other) = delete;
		EditComboBox(ParentWindow* parent, const ConStruct<EditComboBox>& conStruct);
		~EditComboBox();


	private:
		EditComboBox& operator=(const EditComboBox& other) = delete;
		EditComboBox& operator=(EditComboBox&& other) = delete;


	private:
		LRESULT SubclassProcedure(
			HWND hWnd,
			UINT msg,
			WPARAM wParam, LPARAM lParam,
			UINT_PTR uIDSubClass, DWORD_PTR dwRefData) override;
		LRESULT ControlProcedure(WPARAM wParam, LPARAM lParam) override;
		bool CreateWinapiWindow() override;
		void DestroyWinapiWindow() override;

	public:
		void SetEditTextLimit(int length = 0x7FFFFFFE);
		void SetCueBanner(const std::wstring& cue_banner);

		void SetSelection(int start_index, int end_index);
		void SetSelection(const Range& range);
		void SelectAll();
		void RemoveSelection();

		const std::wstring& GetCueBanner();
		Range GetSelection();


	public:
		friend class ObjectCreator;
	};
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	template <> struct ConStruct<EditComboBox>
	{
		Rect rect;

		ConStruct(const Rect& rect = Rect(50, 50, 100, 50))
			: rect(rect)
		{}
	};
}

#endif // !COMBOBOX_H