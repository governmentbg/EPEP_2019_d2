DEFINE_RESPONSE_TABLE1(TFloatPublicationDialog, TGroupDialog)
	EV_BN_CLICKED(IDSAVE, SaveBNClicked),
END_RESPONSE_TABLE;

TFloatPublicationDialog::TFloatPublicationDialog(TWindow* parent, TFloatPublicationGroup *group, int resId)
	: TGroupDialog(parent, group, resId)
{
	types = new TCharAutoListFace(this, IDC_FLOAT_PUBLICATION_TYPES, "F_TYPES", Type, Default->xColleges, SIZE_OF_TYPES);
	minDate = new TDateFace(this, IDC_FLOAT_PUBLICATION_MIN_DATE, "F_MIN_DATE", false);
	maxDate = new TDateFace(this, IDC_FLOAT_PUBLICATION_MAX_DATE, "F_MAX_DATE", false);
	new TRadioFace(this, IDC_FLOAT_PUBLICATION_BY_TNY, "F_BY_TNY");
	new TRadioFace(this, IDC_FLOAT_PUBLICATION_SES_DATE, "F_SES_DATE");
	fileName = new TStringFace(this, IDC_FLOAT_PUBLICATION_FILE_NAME, "F_FILE_NAME", SIZE_OF_OFFICE_TARGET);
}

bool TFloatPublicationDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minDate, maxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
		else if (*fileName->S())
			isValid = TOffice::ValidTargetName(fileName->S());
	}

	return isValid;
}

void TFloatPublicationDialog::SaveBNClicked()
{
	char name[SIZE_OF_PATH];

	if (GetSaveFileName(this, name, "HTML файлове", OUT_PATH, "HTM", sizeof name))
		fileName->SetS(name);
}
