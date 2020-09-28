//{{TFzlListBox Implementation}}

TFzlListBox::TFzlListBox(TWindow* parent, int resourceId, TGroupArray *array)
:
	TGroupListBox(parent, resourceId, array)
{
	loadData = dynamic = false;
}

void TFzlListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	fzl_name(m, group);
}
