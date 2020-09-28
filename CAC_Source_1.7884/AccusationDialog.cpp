//{{TAccusationDialog Implementation}}

TAccusationDialog::TAccusationDialog(TWindow* parent, TRCDKeyContainer *tContainer, TAccusation *accusation, bool edit)
:
	TEditDialog(parent, accusation, edit, IDD_ACCUSATION), container(tContainer)
{
	new TCharAliasFace(this, IDC_ACCUSATION_UCN_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_ACCUSATION_UCN, "F_UCN", SIZE_OF_UCN);
	new TStringFace(this, IDC_ACCUSATION_ACCUSATION, "F_ACCUSATION", SIZE_OF_ACCUSATION_TEXT);
	new TCharAliasFace(this, IDC_ACCUSATION_CRIME_FORM, "F_CRIME_FORM", CrimeForm, CRIME_FORM_ALLS);
	new TCharAliasFace(this, IDC_ACCUSATION_CRIME_STAGE, "F_CRIME_STAGE", CrimeStage, CRIME_STAGE_ALLS);
	new TDateFace(this, IDC_ACCUSATION_CRIME_DATE, "F_CRIME_DATE", true);
	new TCharAliasFace(this, IDC_ACCUSATION_RECIDIVE, "F_RECIDIVE", Recidive, NULL);
	new TStringFace(this, IDC_ACCUSATION_NAME, "F_NAME", SIZE_OF_CITIZEN_NAME);
	new TStringFace(this, IDC_ACCUSATION_RENAME, "F_RENAME", SIZE_OF_CITIZEN_RENAME);
	new TStringFace(this, IDC_ACCUSATION_FAMILY, "F_FAMILY", SIZE_OF_CITIZEN_FAMILY);
}

void TAccusationDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	// From enter local judgement
	TCitizen citizen;

	citizen.ucnType = Accusation()->ucnType;
	strcpy(citizen.ucn, Accusation()->ucn);
	citizen.Try();
	citizen >> this;
}
