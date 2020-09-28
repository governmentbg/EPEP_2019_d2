DEFINE_RESPONSE_TABLE1(TJurorMotivesDialog, TWhateverTextBaseDialog)
//{{TJurorMotivesDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TJurorMotivesDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TJurorMotivesDialog Implementation}}

TJurorMotivesDialog::TJurorMotivesDialog(TWindow* parent, const char *title, char *tMotives, int resId)
:
	TWhateverTextBaseDialog(parent, &field, title, "F_MOTIVES", SIZE_OF_JUROR_MOTIVES, resId), motives(tMotives),
		field("F_MOTIVES", &m, SIZE_OF_JUROR_MOTIVES)
{
	*motives = '\0';
}

bool TJurorMotivesDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!any(text))
			isValid = error("Мотивите са задължителни.");
	}

	return isValid;
}

void TJurorMotivesDialog::OKBNClicked()
{
	if (IsValid())
	{
		strcpy(motives, text->S());
		CmOk();
	}
}
