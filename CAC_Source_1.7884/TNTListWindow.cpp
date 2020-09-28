#include "cac.h"

#undef TListWindow

//{{TNTListWindow Implementation}}

TNTListWindow::TNTListWindow(TWindow* parent, int resourceId)
:
	TListWindow(parent, resourceId)
{
}

void TNTListWindow::GetItem(TListWindItem &item)
{
	if (!TListWindow::GetItem(item))
		fatal("Unable to get list item %d/%d", item.iItem, item.iSubItem);
}

void TNTListWindow::SetItem(TListWindItem &item)
{
	if (!TListWindow::SetItem(item))
		fatal("Unable to set list item %d/%d", item.iItem, item.iSubItem);
}

void TNTListWindow::InsertItem(TListWindItem &item)
{
#if RANGECHECK
	if (item.iItem < 0 || item.iItem > GetItemCount())
		fatal("TNTListWindow::Insert: range check");
#endif  // RANGECHECK

	if (TListWindow::InsertItem(item) < 0)
		fatal("Unable to insert list item %d/%d", item.iItem, item.iSubItem);
}

void TNTListWindow::DeleteAnItem(int itemIndex)
{
#if RANGECHECK
	if (itemIndex < itemIndex || itemIndex > GetItemCount())
		fatal("TNTListWindow::Insert: range check");
#endif  // RANGECHECK

	if (!TListWindow::DeleteAnItem(itemIndex))
		fatal("Unable to delete list item %d", itemIndex);
}

void TNTListWindow::InsertColumn(int colNum, TListWindColumn &colItem)
{
	if (TListWindow::InsertColumn(colNum, colItem) < 0)
		fatal("Unable to insert list column %d", colNum);
}

void TNTListWindow::SetupWindow()
{
	TListWindow::SetupWindow();
	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
}

int TNTListWindow::GetSelIndex(uint state) const
{
	for (int i = 0; i < GetItemCount(); i++)
		if (GetItemState(i, state))
			return i;

	return -1;
}
