DEFINE_RESPONSE_TABLE1(TGetObjectCountDialog, TGroupDialog)
//{{TGetObjectCountDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDC_GET_OBJECT_COUNT_BY_DATES, ByYearsBNClicked),
	EV_BN_CLICKED(IDC_GET_OBJECT_COUNT_BY_YEARS, ByYearsBNClicked),
//{{TGetObjectCountDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetObjectCountDialog Implementation}}

TGetObjectCountDialog::TGetObjectCountDialog(TWindow* parent, TObjectCountGroup *group)
:
	TGroupDialog(parent, group, IDD_GET_OBJECT_COUNT)
{
	byYears = new TRadioFace(this, IDC_GET_OBJECT_COUNT_BY_YEARS, "F_BY_YEARS");
	byDates = new TRadioFace(this, IDC_GET_OBJECT_COUNT_BY_DATES, "F_BY_DATES");
	minYear = new TYearFace(this, IDC_GET_OBJECT_COUNT_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_GET_OBJECT_COUNT_MAX_YEAR, "F_MAX_YEAR");
	minDate = new TDateFace(this, IDC_GET_OBJECT_COUNT_MIN_DATE, "F_MIN_DATE");
	maxDate = new TDateFace(this, IDC_GET_OBJECT_COUNT_MAX_DATE, "F_MAX_DATE");
	logging = new TCheckFace(this, IDC_GET_OBJECT_COUNT_LOGGING, "F_LOGGING");
	attached = new TCheckFace(this, IDC_GET_OBJECT_COUNT_ATTACHED, "F_ATTACHED");
}

bool TGetObjectCountDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (byYears->GetCheck() == BF_CHECKED)
		{
			if (!minYear->L() || !maxYear->L())
				isValid = error("Годините са задължителни.");
			else if (minYear->L() > maxYear->L())
				isValid = error("Началната година е по-голяма от крайната.");
		}
		else
		{
			if (minDate->Date().Empty() || maxDate->Date().Empty())
				isValid = error("Датите са задължителни.");
			else if (minDate->Date() > maxDate->Date())
				isValid = error("Началната дата е по-голяма от крайната.");
		}
	}

	return isValid;
}

void TGetObjectCountDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	ByYearsBNClicked();
}

void TGetObjectCountDialog::OKBNClicked()
{
	if (IsValid())
	{
		TObjectCountGroup *group = ObjectGroup();
		*group << this;

		if (group->byYears)
		{
			group->minDate = CDate(1, 1, minYear->L());
			group->maxDate = CDate(31, 12, maxYear->L());
		}

		CmOk();
	}
}

void TGetObjectCountDialog::ByYearsBNClicked()
{
	TObjectCountGroup *group = ObjectGroup();

	if (byYears->GetCheck() == BF_CHECKED)
	{
		minDate->EnableWindow(false);
		minDate->SetS("");
		maxDate->EnableWindow(false);
		maxDate->SetS("");
		minYear->SetL(group->minDate.year);
		minYear->EnableWindow(true);
		maxYear->SetL(group->maxDate.year);
		maxYear->EnableWindow(true);
	}
	else
	{
		minYear->EnableWindow(false);
		minYear->SetL(0);
		maxYear->EnableWindow(false);
		maxYear->SetL(0);
		minDate->SetDate(group->minDate);
		minDate->EnableWindow(true);
		maxDate->SetDate(group->maxDate);
		maxDate->EnableWindow(true);
	}
}
