DEFINE_RESPONSE_TABLE1(TPunishmentDialog, TGroupDialog)
//{{TPunishmentDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TPunishmentDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TPunishmentDialog Implementation}}

TPunishmentDialog::TPunishmentDialog(TWindow* parent, TDecision *decision, TAccusation *accusation, TLawsuit *lawsuit)
:
	TGroupDialog(parent, accusation, IDD_PUNISHMENT)
{
	new TCharAliasFace(this, IDC_PUNISHMENT_UCN_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_PUNISHMENT_UCN, "F_UCN", SIZE_OF_UCN);
	new TStringFace(this, IDC_PUNISHMENT_ACCUSATION, "F_ACCUSATION", SIZE_OF_ACCUSATION_TEXT);
	new TStringFace(this, IDC_PUNISHMENT_NAME, "F_NAME", SIZE_OF_CITIZEN_NAME);
	new TStringFace(this, IDC_PUNISHMENT_RENAME, "F_RENAME", SIZE_OF_CITIZEN_RENAME);
	new TStringFace(this, IDC_PUNISHMENT_FAMILY, "F_FAMILY", SIZE_OF_CITIZEN_FAMILY);
	new TCharAliasFace(this, IDC_PUNISHMENT_PUNISHMENT, "F_PUNISHMENT", Punishment, decision->Punishments(lawsuit));
}

void TPunishmentDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Accusation() << this;
		if (Accusation()->Update())
			CmOk();
	}
}

void TPunishmentDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	// From enter local judgement
	TCitizen citizen;

	citizen.ucnType = Accusation()->ucnType;
	strcpy(citizen.ucn, Accusation()->ucn);
	citizen.Try();
	citizen >> this;
}
