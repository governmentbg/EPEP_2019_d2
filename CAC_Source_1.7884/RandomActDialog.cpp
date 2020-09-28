DEFINE_RESPONSE_TABLE1(TRandomActDialog, TGroupDialog)
//{{TRandomActDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_RANDOM_ACT_FORCE_DATE, ForceDateBNClicked),
//{{TRandomActDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRandomActDialog Implementation}}

TRandomActDialog::TRandomActDialog(TWindow* parent, TRandomActGroup *group)
:
	TGroupDialog(parent, group, IDD_RANDOM_ACT)
{
	judge = new TUserAliasFace(this, IDC_RANDOM_ACT_JUDGE, "F_JUDGE", POST_VISIBLE_JUDGES, FLAG_ACTIVE);
	new TRadioFace(this, IDC_RANDOM_ACT_ALLS, "F_ALLS");
	new TRadioFace(this, IDC_RANDOM_ACT_DECISIONS, "F_DECISIONS");
	new TRadioFace(this, IDC_RANDOM_ACT_FINALIZERS, "F_FINALIZERS");
	dateGroup = new TGroupBox(this, IDC_RANDOM_ACT_DATE_GROUP);
	minDate = new TDateFace(this, IDC_RANDOM_ACT_MIN_DATE, "F_MIN_DATE", true);
	maxDate = new TDateFace(this, IDC_RANDOM_ACT_MAX_DATE, "F_MAX_DATE", true);
	forceDate = new TCheckFace(this, IDC_RANDOM_ACT_FORCE_DATE, "F_FORCE_DATE");
	new TDateFaceUpDown(this, IDC_RANDOM_ACT_MIN_DATE_UPDN, minDate);
	new TDateFaceUpDown(this, IDC_RANDOM_ACT_MAX_DATE_UPDN, maxDate);
}

bool TRandomActDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
	}

	return isValid;
}

void TRandomActDialog::ForceDateBNClicked()
{
	//dateGroup->SetCaption(forceDate->GetCheck() == BF_CHECKED ? "Влезли в законна сила" : "&Дата на съдебен акт");
	dateGroup->SetCaption(forceDate->GetCheck() == BF_CHECKED ? "&Дата на влизане в з.с." : "&Дата на съдебен акт");
}
