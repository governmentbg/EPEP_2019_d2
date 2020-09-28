DEFINE_RESPONSE_TABLE1(TAddSchedDialog, TGroupDialog)
//{{TAddSchedDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_ADD_SCHED_SCHED, SchedCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TAddSchedDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAddSchedDialog Implementation}}

TAddSchedDialog::TAddSchedDialog(TWindow* parent, TSched *tSched, const char *scheds)
:
	TGroupDialog(parent, tSched, IDD_ADD_SCHED)
{
	sched = new TCharAliasFace(this, IDC_ADD_SCHED_SCHED, "F_SCHED", CalInterval, scheds);
	start = new TDateFace(this, IDC_ADD_SCHED_START, "F_START");
	final = new TDateFace(this, IDC_ADD_SCHED_FINAL, "F_FINAL");
	judge = new TUserAliasFace(this, IDC_ADD_SCHED_JUDGE, "F_JUDGE", "", FLAG_ACTIVE | ALIAS_OPTION);
}

void TAddSchedDialog::OKBNClicked()
{
	if (IsValid())
	{
		TSched tSched;

		*Group() << this;
		tSched << Group();

		if (tSched.Try())
			error("%s: операцията %s би довела до дублиране на ключ.", tSched.Name, "добавяне");
		else if (Group()->Insert())
			CmOk();
	}
}

bool TAddSchedDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (start->Date() > final->Date())
			isValid = error("Началната дата на срока е по-голяма от крайната.");
		else if (!final->Date().Work())
			isValid = error("Срокът не може да изтича на почивен ден.");
	}

	return isValid;
}

void TAddSchedDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	SchedCBNSelChange();
}

void TAddSchedDialog::SchedCBNSelChange()
{
	judge->SetXCrit(strchr(SCHED_LOCALS, sched->C()) ? POST_VISIBLE_JUDGES : "");
}
