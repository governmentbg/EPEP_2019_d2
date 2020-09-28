//{{TSelectRegisterActListWindow Implementation}}

TSelectRegisterActListWindow::TSelectRegisterActListWindow(TWindow* parent, int resourceId, TGArray<TRegisterAct> *acts)
:
	TGroupListWindow(parent, resourceId, acts, 332)
{
	Add(new TGroupListColumn("Регистриран акт", group_print, 320));
}

void TSelectRegisterActListWindow::View()
{
	((TRegisterAct *) Group())->ViewFiles(Parent);
}

TGroup *TSelectRegisterActListWindow::NewGroup() const
{
	return new TRegisterAct;
}

void TSelectRegisterActListWindow::GetGroupText(mstr &m, TGroup *group, int)
{
	const TRegisterAct *act = (const TRegisterAct *) group;
	print_select_act(m, act);

	if (any(act->text))
		m.printf(" - %s", act->text);
}
