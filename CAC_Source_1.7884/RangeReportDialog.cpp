DEFINE_RESPONSE_TABLE1(TRangeReportDialog, TGroupDialog)
//{{TRangeReportDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TRangeReportDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRangeReportDialog Implementation}}

TRangeReportDialog::TRangeReportDialog(TWindow* parent, TRangeReportGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	minMonth = new TLongAliasFace(this, IDC_RANGE_REPORT_MIN_MONTH, "F_MIN_MONTH", Month, NULL, FLAG_NULL);
	minYear = new TYearFace(this, IDC_RANGE_REPORT_MIN_YEAR, "F_MIN_YEAR");
	maxMonth = new TLongAliasFace(this, IDC_RANGE_REPORT_MAX_MONTH, "F_MAX_MONTH", Month, NULL, FLAG_NULL);
	maxYear = new TYearFace(this, IDC_RANGE_REPORT_MAX_YEAR, "F_MAX_YEAR");
	instance1st = new TRadioFace(this, IDC_RANGE_REPORT_INSTANCE_1ST, "F_INSTANCE_1ST");
	instance2nd = new TRadioFace(this, IDC_RANGE_REPORT_INSTANCE_2ND, "F_INSTANCE_2ND");
	instanceAll = new TRadioFace(this, IDC_RANGE_REPORT_INSTANCE_ALL, "F_INSTANCE_ALL");
	types = new TCharListFace(this, IDC_RANGE_REPORT_TYPES, "F_TYPES", Type, QUERY_COLLEGES, SIZE_OF_TYPES);
	builtin = new TCheckFace(this, IDC_RANGE_REPORT_BUILTIN, "F_BUILTIN");
	new TRadioFace(this, IDC_RANGE_REPORT_SUBJECTS_NONE, "F_SUBJECTS_NONE");
	new TRadioFace(this, IDC_RANGE_REPORT_SUBJECTS_POSITIVE, "F_SUBJECTS_POSITIVE");
	new TRadioFace(this, IDC_RANGE_REPORT_SUBJECTS_ALL, "F_SUBJECTS_ALL");
	new TLongAliasFace(this, IDC_RANGE_REPORT_COMPOSITION, "F_COMPOSITION", Composition, QUERY_COMPOSITIONS, ALIAS_EMPTY);
}

bool TRangeReportDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		*Group() << this;

		if (!any(RRGroup()->types))
			isValid = error("Ќе е избран нито един вид дело.");
		else if (RRGroup()->start > RRGroup()->final)
			isValid = error("Ќачалните месец и година са по-големи от крайните.");
		else if (RRGroup()->final > Today)
			isValid = error(" райните месец и година са по-големи от текущата дата.");
	}

	return isValid;
}

void TRangeReportDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_REGIONAL
	instance1st->EnableWindow(false);
	instance2nd->EnableWindow(false);
	instanceAll->EnableWindow(false);
#endif  // APPEAL || REGIONAL
	builtin->SetCheck(RRGroup()->attrib == POST_BUILTIN);
	RRGroup()->start.day = 1;
	RRGroup()->final.day = 1;
}

void TRangeReportDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(*types->S());
}

void TRangeReportDialog::OKBNClicked()
{
	if (IsValid())
	{
		RRGroup()->attrib = builtin->GetCheck() == BF_CHECKED ? POST_BUILTIN : '\0';
		RRGroup()->final.day = RRGroup()->final.DaysInMonth();
		CmOk();
	}
}
