DEFINE_RESPONSE_TABLE1(TSchedListDialog, TGroupListDialog)
//{{TSchedListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
//{{TSchedListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSchedListDialog Implementation}}

TSchedListDialog::TSchedListDialog(TWindow* parent, TSched *tSched, const char *tScheds, int resId)
:
	TGroupListDialog(parent, resId), sched(tSched)
{
	header1 = new TStatic(this, IDC_SCHED_LIST_HEADER_1);
	header2 = new TStatic(this, IDC_SCHED_LIST_HEADER_2);
	scheds = new TSchedListWindow(this, IDC_GROUP_LIST_WINDOW, sched, tScheds);
}

void TSchedListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	mstr m;

	m.cat(Kind->Seek(sched->kind));
	if (!strchr(TYPE_LAWSUITS, sched->key.type))
		m.printf(" %ld/%ldã.", sched->key.no, sched->key.year);
	sched->date.Print(m, " îò %Dã.");
	header1->SetCaption(str(m));

	m.clear();
	m.printf("%s %ld/%ldã.", Type->Seek(sched->lawsuit.type), sched->lawsuit.no, sched->lawsuit.year);
	header2->SetCaption(str(m));
}

void TSchedListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(*scheds->Scheds());
}
