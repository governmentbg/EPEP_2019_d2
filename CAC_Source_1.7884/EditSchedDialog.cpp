DEFINE_RESPONSE_TABLE1(TEditSchedDialog, TGroupDialog)
//{{TEditSchedDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditSchedDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditSchedDialog Implementation}}

TEditSchedDialog::TEditSchedDialog(TWindow* parent, TSched *sched, int resId)
:
	TGroupDialog(parent, sched, resId)
{
	new TCharAliasFace(this, IDC_EDIT_SCHED_SCHED, "F_SCHED", CalInterval, NULL);
	start = new TDateFace(this, IDC_EDIT_SCHED_START, "F_START");
	final = new TDateFace(this, IDC_EDIT_SCHED_FINAL, "F_FINAL");
	new TUserAliasFace(this, IDC_EDIT_SCHED_JUDGE, "F_JUDGE", strchr(SCHED_LOCALS, sched->sched) ? POST_VISIBLE_JUDGES : "", FLAG_ACTIVE | ALIAS_OPTION);
}

bool TEditSchedDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (start->Date() > final->Date())
			isValid = error("Ќачалната дата на срока е по-гол€ма от крайната.");
	}

	return isValid;
}

void TEditSchedDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		TSched tSched;

		tSched << Group();
		if ((!tSched.Try() || tSched.Delete(true)) && Group()->Insert())
			CmOk();
	}
}
