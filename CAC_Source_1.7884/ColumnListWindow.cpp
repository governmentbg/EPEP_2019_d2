#include "cac.h"

DEFINE_RESPONSE_TABLE1(TColumnListWindow, TListWindow)
//{{TColumnListWindowRSP_TBL_BEGIN}}
	EV_WM_RBUTTONDOWN,
	EV_WM_RBUTTONUP,
	EV_WM_RBUTTONDBLCLK,
//{{TColumnListWindowRSP_TBL_END}}
END_RESPONSE_TABLE;

TColumnListWindow::TColumnListWindow(TWindow* parent, int resourceId, int tAdjustWidth)
:
	TListWindow(parent, resourceId), adjustWidth(tAdjustWidth), drawLockCount(-1)
{
}

void TColumnListWindow::Add(TGroupListColumn *column1, TGroupListColumn *column2, ...)
{
	va_list ap;

	va_start(ap, column2);
	Add(column1, column2, ap);
	va_end(ap);
}

void TColumnListWindow::Add(TGroupListColumn *column1, TGroupListColumn *column2, va_list ap)
{
	Add(column1);

	if (column2)
	{
		Add(column2);
		while ((column2 = va_arg(ap, TGroupListColumn *)) != NULL)
			Add(column2);
	}
}

void TColumnListWindow::AddColumns(int targetWidth)
{
	double factor = (double) targetWidth / adjustWidth;

	for (int index = 0; index < columns.Count(); index++)
	{
		TGroupListColumn *column = columns[index];
		TListWindColumn lwc;

		column->width *= factor;
		lwc.SetWidth(column->width);
		lwc.SetFormat((TListWindColumn::TFormat) column->format);
		lwc.SetText(column->title);
		InsertColumn(index, lwc);
	}
}

void TColumnListWindow::DrawGroupAt(TGroup *group, int index, bool insert)
{
	TListWindItem lwi;

	lwi.iItem = index;
	lwi.iSubItem = 0;
	lwi.mask = 0;

	if (insert)
	{
		mstr m;
		int image = GetImage(group);

		GetGroupText(m, group, 0);
		lwi.SetText(str(m));
		if (image >= 0)
			lwi.SetImageIndex(image);
		InsertItem(lwi);
		lwi.iSubItem++;
		lwi.mask = 0;
	}

	while (lwi.iSubItem < columns.Count())
	{
		mstr m;

		GetGroupText(m, group, lwi.iSubItem);
		lwi.SetText(str(m));
		SetItem(lwi);
		lwi.iSubItem++;
	}
}

void TColumnListWindow::GetGroupText(mstr &m, TGroup *group, int column)
{
	columns[column]->Print(m, group);
}

void TColumnListWindow::UpdateVisual()
{
	int width, index;
	int summaryWidth = GetColumnWidth(0);

	if (drawLockCount)
		SetRedraw(false);

	for (index = 1; index < columns.Count(); index++)
	{
		TGroupListColumn *column = columns[index];

		SetColumnWidth(index, column->resize ? LVSCW_AUTOSIZE : column->width);
		width = GetColumnWidth(index);

		if (width < column->width)
		{
			SetColumnWidth(index, column->width);
			width = GetColumnWidth(index);
		}

		summaryWidth += width;
	}

	int clientWidth = initialWidth;

	if (GetStyle() & WS_VSCROLL)
		clientWidth -= GetSystemMetrics(SM_CXVSCROLL);

	if (clientWidth > summaryWidth)
		SetColumnWidth(index - 1, width + clientWidth - summaryWidth);

	if (!drawLockCount)
		SetWaitCursor(false);

	drawLockCount = -1;
	SetRedraw(true);
}

bool TColumnListWindow::UnblockFocus(int command)
{
	TWindow::THandle handle = Parent->GetDlgItem(command);

	if (handle && GetFocus() == handle)
	{
		SetFocus();
		::SendMessage(handle, BM_SETSTYLE, ::GetWindowLong(handle, GWL_STYLE) & ~BS_DEFPUSHBUTTON, MAKELPARAM(TRUE, 0));

		uint defId = LOWORD(Parent->SendMessage(DM_GETDEFID));

		if (defId)
		{
			handle = Parent->GetDlgItem(defId);
			::SendMessage(handle, BM_SETSTYLE, ::GetWindowLong(handle, GWL_STYLE) | BS_DEFPUSHBUTTON, MAKELPARAM(TRUE, 0));
		}

		return true;
	}

	return false;
}

void TColumnListWindow::ParentNotify(int command)
{
	THandle handle = Parent->GetDlgItem(command);

	if (handle && ::IsWindowEnabled(handle))
		Parent->PostMessage(WM_COMMAND, MAKEWPARAM(command, BN_CLICKED), (LPARAM) Handle);
}

void TColumnListWindow::SetupWindow()
{
	TListWindow::SetupWindow();
	initialWidth = GetClientRect().Width();
}

void TColumnListWindow::EvRButtonDown(uint, TPoint&)
{
}

void TColumnListWindow::EvRButtonUp(uint, TPoint&)
{
}

void TColumnListWindow::EvRButtonDblClk(uint, TPoint&)
{
}
