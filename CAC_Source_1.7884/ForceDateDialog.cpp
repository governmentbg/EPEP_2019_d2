//{{TForceDateDialog Implementation}}

TForceDateDialog::TForceDateDialog(TWindow* parent, TIndexContainer *container, TLawsuit *tLawsuit, const char *tWhat)
:
	TEditDialog(parent, container, true, IDD_FORCE_DATE), lawsuit(tLawsuit), what(tWhat)
{
	new TCharAliasFace(this, IDC_FORCE_DATE_TYPE, "F_TYPE", Type, NULL);
	new TLongFace(this, IDC_FORCE_DATE_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_FORCE_DATE_YEAR, "F_YEAR");
	kind = new TCharAliasFace(this, IDC_FORCE_DATE_KIND, "F_KIND", Kind, NULL);
	date = new TDateFace(this, IDC_FORCE_DATE_FROM_DATE, strchr(KIND_ENDOCS, container->kind) ? "F_FROM_DATE" : "F_DATE");
	forceDate = new TDateFace(this, IDC_FORCE_DATE_FORCE_DATE, "F_FORCE_DATE", false);
	lawsuitForceDate = new TDateFace(this, IDC_FORCE_DATE_LAWSUIT_FORCE_DATE, "F_LAWSUIT_FORCE_DATE", false);
}

bool TForceDateDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!forceDate->Date().Empty() && forceDate->Date() > Today)
			isValid = error("Датата на влизане в законна сила е по-голяма от днешната дата.");
		else if (!forceDate->Date().Empty() && forceDate->Date() < date->Date())
			isValid = error("Датата на влизане в законна сила е преди датата на съдебния акт.");
		else if (!forceDate->Date().Empty() && forceDate->Date() < ((TDate *) Group()->Find("F_RETURNED"))->Date())
			isValid = ask("Датата на влизане в законна сила е преди датата на %s. Желаете ли да продължите?", what);
	}

	return isValid;
}

void TForceDateDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	lawsuitForceDate->SetDate(lawsuit->forceDate);
}
