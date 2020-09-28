//{{TXMLGroupListWindow Implementation}}

TXMLGroupListWindow::TXMLGroupListWindow(TWindow* parent, int resourceId, TXMLArray *array, int adjustWidth)
:
	TGroupListWindow(parent, resourceId, array, adjustWidth)
{
	dynamic = !array;
	sortcol = 1;
}

TGroup *TXMLGroupListWindow::NewGroup() const
{
	return ((TXMLArray *) array)->NewObject();
}
