//{{TSideWindListBox Implementation}}

TSideWindListBox::TSideWindListBox(TWindow* parent, int resourceId, TGArray<TSideWind> *sideWinds)
:
	TCloneListBox(parent, resourceId, sideWinds, TSideWind::Clone)
{
	dynamic = loadData = false;
}
