//{{TRegisterActListBox Implementation}}

TRegisterActListBox::TRegisterActListBox(TWindow* parent, int resourceId, TGArray<TRegisterAct> *acts)
:
	TGroupListBox(parent, resourceId, acts)
{
}

void TRegisterActListBox::View()
{
	((TRegisterAct *) Group())->ViewFiles();
}

TGroup *TRegisterActListBox::NewGroup() const
{
	return new TRegisterAct;
}

void TRegisterActListBox::GetGroupText(mstr &m, TGroup *group, int)
{
	const TRegisterAct *act = (const TRegisterAct *) group;
	print_select_act(m, act);

	if (act->text)
		m.printf("- %s", act->text);
}
