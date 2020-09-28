DEFINE_RESPONSE_TABLE1(TRenumberSideWindDialog, TJoinUXNDialog)
	EV_EN_UPDATE(IDC_RENUMBER_SIDE_WIND_FILE_NAME, FileNameENUpdate),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
END_RESPONSE_TABLE;

TRenumberSideWindDialog::TRenumberSideWindDialog(TWindow* parent, TUCNGroup *source, TUCNGroup *target,
	TRenumberSideLawsuit *tLawsuit)
	: TJoinUXNDialog(parent, source, target, IDD_RENUMBER_SIDE_WIND), lawsuit(tLawsuit)
{
	type = new TCharAliasFace(this, IDC_RENUMBER_SIDE_WIND_TYPE, "F_TYPE", Type, Default->colleges);
	year = new TYearFace(this, IDC_RENUMBER_SIDE_WIND_YEAR, "F_YEAR");
	no = new TLongFace(this, IDC_RENUMBER_SIDE_WIND_NO, "F_NO", SIZE_OF_NO);
	fileName = new TStringFace(this, IDC_RENUMBER_SIDE_WIND_FILE_NAME, "F_FILE_NAME", SIZE_OF_PATH);
}

bool TRenumberSideWindDialog::IsValid()
{
	bool isValid = TJoinUXNDialog::IsValid();

	if (isValid)
	{
		*lawsuit << this;

		if (!any(lawsuit->fileName))
			isValid = lawsuit->Get();
	}

	return isValid;
}

void TRenumberSideWindDialog::FileNameENUpdate()
{
	bool enable = !any(fileName);

	no->EnableWindow(enable);
	year->EnableWindow(enable);
	type->EnableWindow(enable);
}

void TRenumberSideWindDialog::AttachBNClicked()
{
	char name[SIZE_OF_PATH];

	if (GetOpenFileName(this, name, "Текстови файлове", ".", "TXT", sizeof name, NULL, false))
		fileName->SetS(name);
}
