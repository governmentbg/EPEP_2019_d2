DEFINE_RESPONSE_TABLE1(TGetYearDialog, TGroupDialog)
//{{TGetYearDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetYearDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetYearDialog Implementation}}

TGetYearDialog::TGetYearDialog(TWindow* parent, TGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	year = new TYearFace(this, IDC_GET_YEAR_YEAR, "F_YEAR");
}

void TGetYearDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		CmOk();
	}
}

bool TGetYearDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (year->L() == 0)
			isValid = error("Годината е задължителна.");
	}

	return isValid;
}
