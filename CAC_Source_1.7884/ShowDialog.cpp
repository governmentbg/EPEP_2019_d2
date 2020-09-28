#include "cac.h"

//{{TShowDialog Implementation}}

TShowDialog::TShowDialog(TWindow* parent, int showMax, int resId)
:
	TResizeDialog(parent, showMax, resId), cleanedUp(false)
{
}

TIArray<TShowDialog> TShowDialog::Dialogs;

void TShowDialog::Cleanup()
{
	for (int i = 0; i < Dialogs.Count(); )
	{
		if (Dialogs[i]->cleanedUp)
			Dialogs.Remove(i);
		else
			i++;
	}
}

int TShowDialog::GetCount()
{
	Cleanup();
	return Dialogs.Count();
}

void TShowDialog::CloseAll()
{
	while (Dialogs.Count())
		Dialogs.Remove(0);
}

bool TShowDialog::FindWindow(HANDLE hWnd)
{
	Cleanup();

	for (int i = 0; i < Dialogs.Count(); i++)
		if (Dialogs[i]->Handle == hWnd)
			return true;

	return false;
}

void TShowDialog::SetupWindow()
{
	TResizeDialog::SetupWindow();

	if (!Parent)
		Dialogs.Add(this);
}

void TShowDialog::CleanupWindow()
{
	TResizeDialog::CleanupWindow();
	cleanedUp = true;
}
