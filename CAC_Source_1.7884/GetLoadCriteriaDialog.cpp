DEFINE_RESPONSE_TABLE1(TGetLoadCriteriaDialog, TPeriodDialog)
//{{TGetLoadCriteriaDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetLoadCriteriaDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetLoadCriteriaDialog Implementation}}

TGetLoadCriteriaDialog::TGetLoadCriteriaDialog(TWindow* parent, TLoadGroup *loadGroup, TLoadGroup *tInvisible, int resId)
:
	TPeriodDialog(parent, loadGroup, resId), invisible(tInvisible)
{
	halls = new THallListBox(this, IDC_GET_LOAD_CRITERIA_HALLS, FLAG_ACTIVE);
}

void TGetLoadCriteriaDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(halls->GetSelCount());
}

bool TGetLoadCriteriaDialog::IsValid()
{
	bool isValid = TPeriodDialog::IsValid();

	if (isValid)
	{
		CDate date = minDate->Date();
		date.AddMonths(2);

		if (maxDate->Date() > date)
			isValid = error("Периода е по-голям от 2 месеца.");
	}

	return isValid;
}

void TGetLoadCriteriaDialog::OKBNClicked()
{
	if (IsValid())
	{
		*LoadGroup() << this;
		*invisible << this;
		halls->GetMarks(&LoadGroup()->halls, &invisible->halls);
		CmOk();
	}
}
