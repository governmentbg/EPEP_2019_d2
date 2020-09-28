DEFINE_RESPONSE_TABLE1(TFloatElectricDialog, TFloatDialog)
//{{TFloatElectricDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_FLOAT_ELECTRIC_TYPE, TypeCBNSelChange),
	EV_BN_CLICKED(IDC_FLOAT_ELECTRIC_DOCUMENT, VariantBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_ELECTRIC_SHEETABLE, VariantBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_ELECTRIC_MISSING, VariantBNClicked),
//{{TFloatElectricDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatElectricDialog Implementation}}

TFloatElectricDialog::TFloatElectricDialog(TWindow* parent, TFloatElectricGroup *group, int resId)
:
	TFloatDialog(parent, group, resId)
{
	build_ctypes(types, TYPE_ELECTRICABLES, false);
	type = new TCharAliasFace(this, IDC_FLOAT_ELECTRIC_TYPE, "F_TYPE", Type, types);
	minNo = new TLongFace(this, IDC_FLOAT_ELECTRIC_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_ELECTRIC_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_ELECTRIC_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_ELECTRIC_MAX_YEAR, "F_MAX_YEAR");
	minDateLabel = new TStatic(this, IDC_FLOAT_ELECTRIC_MIN_DATE_LABEL);
	finishedLabel = new TStatic(this, IDC_FLOAT_ELECTRIC_FINISHED_LABEL);
	minDate = new TDateFace(this, IDC_FLOAT_ELECTRIC_MIN_DATE, "F_MIN_DATE", false);
	maxDateLabel = new TStatic(this, IDC_FLOAT_ELECTRIC_MAX_DATE_LABEL);
	maxDate = new TDateFace(this, IDC_FLOAT_ELECTRIC_MAX_DATE, "F_MAX_DATE", false);
	document = new TRadioFace(this, IDC_FLOAT_ELECTRIC_DOCUMENT, "F_DOCUMENT");
	sheetable = new TRadioFace(this, IDC_FLOAT_ELECTRIC_SHEETABLE, "F_SHEETABLE");
	missing = new TRadioFace(this, IDC_FLOAT_ELECTRIC_MISSING, "F_MISSING");
}

bool TFloatElectricDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Ќачални€ номер дело е по-гол€м от крайни€.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Ќачалната година е по-гол€ма от крайната.");
		else if (invalid_range(minDate, maxDate))
			isValid = error("Ќачалната дата е по-гол€ма от крайната.");
	}

	return isValid;
}

void TFloatElectricDialog::Action(bool execute)
{
	msql m;

	FloatElectricGroup()->FormCriteria(m);

	if (execute)
		query_electric(this, str(m), FloatElectricGroup());
	else
		MessageEx(m);
}

void TFloatElectricDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	TypeCBNSelChange();
	VariantBNClicked();
}

void TFloatElectricDialog::TypeCBNSelChange()
{
	if (type->C() == TYPE_PUNISHMENT_LAW)
		missing->EnableWindow(true);
	else
	{
		missing->EnableWindow(false);

		if (missing->GetCheck() == BF_CHECKED)
		{
			document->SetCheck(BF_CHECKED);
			VariantBNClicked();
		}
	}
}

void TFloatElectricDialog::VariantBNClicked()
{
	if (missing->GetCheck() == BF_CHECKED)
	{
		minDateLabel->ShowWindow(SW_HIDE);
		maxDateLabel->ShowWindow(SW_HIDE);
		finishedLabel->ShowWindow(SW_SHOW);
	}
	else
	{
		finishedLabel->ShowWindow(SW_HIDE);
		minDateLabel->ShowWindow(SW_SHOW);
		maxDateLabel->ShowWindow(SW_SHOW);
	}
}
