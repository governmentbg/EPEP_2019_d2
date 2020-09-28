DEFINE_RESPONSE_TABLE1(TExportCSVDialog, TGroupDialog)
//{{TExportCSVDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
//{{TExportCSVDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TExportCSVDialog Implementation}}

TExportCSVDialog::TExportCSVDialog(TWindow* parent, TExportCSVGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	minRequestDate = new TDateFace(this, IDC_EXPORT_CSV_MIN_REQUEST_DATE, "F_MIN_REQUEST_DATE", false);
	maxRequestDate = new TDateFace(this, IDC_EXPORT_CSV_MAX_REQUEST_DATE, "F_MAX_REQUEST_DATE", false);
	minLawsuitDate = new TDateFace(this, IDC_EXPORT_CSV_MIN_LAWSUIT_DATE, "F_MIN_LAWSUIT_DATE", false);
	maxLawsuitDate = new TDateFace(this, IDC_EXPORT_CSV_MAX_LAWSUIT_DATE, "F_MAX_LAWSUIT_DATE", false);
#if TESTVER
	exportRequests = new TCheckFace(this, IDC_EXPORT_CSV_EXPORT_REQUESTS, "F_EXPORT_REQUESTS");
	exportLawsuits = new TCheckFace(this, IDC_EXPORT_CSV_EXPORT_LAWSUITS, "F_EXPORT_LAWSUITS");
	new TCheckFace(this, IDC_EXPORT_CSV_HUMAN_READABLE, "F_HUMAN_READABLE");
	new TCheckFace(this, IDC_EXPORT_CSV_SHOW_WARNINGS, "F_SHOW_WARNINGS");
#endif  // TESTVER
}

bool TExportCSVDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		static constant names[4] =
		{
			"Началната дата на постъпване на входящ документ",
			"Крайната дата на постъпване на входящ документ",
			"Началната дата на образуване на дело",
			"Крайната дата на образуване на дело"
		};

		const CDate *dates[4] = { &minRequestDate->Date(), &maxRequestDate->Date(), &minLawsuitDate->Date(),
			&maxLawsuitDate->Date() };

		for (int i = 0; i < 4; i++)
		{
			const CDate *date = dates[i];

			if (!date->Empty())
			{
				if (*date < ADMC_DATE_MIN)
				{
					isValid = error("%s е преди %Dг.", names[i], date);
					break;
				}

				if (*date > ADMC_DATE_MAX)
				{
					isValid = error("%s е след %Dг.", names[i], date);
					break;
				}
			}
		}

		if (isValid)
		{
			if (invalid_range(minRequestDate, maxRequestDate))
				isValid = error("Началната дата на постъпване на входящ документ е по-голяма от крайната.");
			else if (invalid_range(minLawsuitDate, maxLawsuitDate))
				isValid = error("Началната дата на образуване на дело е по-голяма от крайната.");
		}
	}

	return isValid;
}

void TExportCSVDialog::OKBNEnable(TCommandEnabler &tce)
{
#if TESTVER
	tce.Enable(exportRequests->GetCheck() == BF_CHECKED || exportLawsuits->GetCheck() == BF_CHECKED);
#else  // TESTVER
	tce.Enable(true);
#endif  // TESTVER
}
