//{{TEmptyListWindow Implementation}}

TEmptyListWindow::TEmptyListWindow(TWindow* parent, int resourceId)
:
	TCadrListWindow(parent, resourceId, ZERO_LONG)
{
}

TGroup *TEmptyListWindow::NewGroup() const
{
	return NULL;
}

void TEmptyListWindow::Refresh()
{
	UpdateVisual();
}

void TEmptyListWindow::Activate()
{
	if (!initialRefresh)
	{
		Refresh();
		initialRefresh = true;
	}
}
