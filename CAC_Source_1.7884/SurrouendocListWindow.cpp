//{{TSurrouendocListWindow Implementation}}

TSurrouendocListWindow::TSurrouendocListWindow(TWindow *parent, int resourceId, TSession *tSession,
	TGArray<TSurroundment> *surrouendocs, TLawsuit *tLawsuit)
	: TGroupListWindow(parent, resourceId, surrouendocs, 292), session(tSession), lawsuit(tLawsuit)
{
	Add(
		new TGroupListColumn("Вид документ", surrouendoc_kind, 128),
		new TGroupListColumn("Номер/Година", keygroup_key_no_year, 72, LVCFMT_RIGHT),
		new TGroupListColumn("Вид акт", surrouendoc_act_kind, 72),
		NULL
	);

	memset(electricMap, '\0', sizeof electricMap);

	for (int i = 0; i < surrouendocs->Count(); i++)
		electricMap[(*surrouendocs)[i]->extraNo] = true;
}

void TSurrouendocListWindow::Appeal()
{
	new_inreg(this, lawsuit, session, (TSurroundment *) Group());
}

void TSurrouendocListWindow::Clear()
{
	TSurroundment *surrouendoc = (TSurroundment *) Group();

	if (AskDelete(surrouendoc) && delete_surroundment(surrouendoc, lawsuit, -1))
		RedrawVisual(GetSelIndex());
}

bool TSurrouendocListWindow::EditGroup(TGroup *group)
{
	TSurroundment *surrouendoc = (TSurroundment *) group;

	if (!surrouendoc->extraNo)
	{
		long &i = surrouendoc->extraNo;

		for (i = 1; i <= SURROUNDMENTS_MAX; i++)
			if (!electricMap[i])
				break;

		electricMap[i] = true;
	}

	return edit_surrouendoc(this, (TSurroundment *) group, session, lawsuit);
}

TGroup *TSurrouendocListWindow::NewGroup() const
{
	return new TSurroundment;
}
