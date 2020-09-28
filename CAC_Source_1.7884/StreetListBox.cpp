//{{TStreetListBox Implementation}}

TStreetListBox::TStreetListBox(TWindow* parent, int resourceId, long tUCLP)
:
	TGroupListBox(parent, resourceId, NULL), uclp(tUCLP)
{
	sortcol = 0;
	qsMode = QUICK_SEARCH_AUTO;
}

TGroup *TStreetListBox::NewGroup() const
{
	TStreet *street = new TStreet;
	street->uclp = uclp;
	return street;
}
