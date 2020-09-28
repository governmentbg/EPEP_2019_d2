DEFINE_RESPONSE_TABLE1(TExpireSchedDialog, TGroupDialog)
//{{TExpireSchedDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TExpireSchedDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TExpireSchedDialog Implementation}}

TExpireSchedDialog::TExpireSchedDialog(TWindow* parent, TExpireSchedGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	types = new TCharAutoListFace(this, IDC_EXPIRE_SCHED_TYPES, "F_TYPES", Type, QUERY_COLLEGES, SIZE_OF_TYPES);
	new TDateFace(this, IDC_EXPIRE_SCHED_DATE, "F_DATE");
	judges = new TUserListBox(this, IDC_EXPIRE_SCHED_JUDGES, POST_VISIBLE_JUDGES, USER_INTER);
}

void TExpireSchedDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		judges->GetMark(&ExpireSchedGroup()->judges);
		query_sched(this, ExpireSchedGroup());
		CmOk();
	}
}

bool TExpireSchedDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!*types->S())
			isValid = error("Не е избран нито един вид дело.");
	}

	return isValid;
}
