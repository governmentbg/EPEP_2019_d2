DEFINE_RESPONSE_TABLE1(TEditMotivesDialog, TWhateverTextBaseDialog)
//{{TEditMotivesDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
	EV_BN_CLICKED(IDSWAP, SwapBNClicked),
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
//{{TEditMotivesDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditMotivesDialog Implementation}}

TEditMotivesDialog::TEditMotivesDialog(TJudicialActDialog *parent, TDecision *decision, bool tEdit, char tMotivesKind,
	const char *title, const char *what)
:
	TWhateverTextBaseDialog(parent, decision, title, what, SIZE_OF_MOTIVES, IDD_EDIT_MOTIVES), edit(tEdit),
		trouble(this, TYPE_TEXT, tEdit), decisionKind(decision->kind),
		motivesKind(tMotivesKind), judicialActDialog(parent)
{
	if (motivesKind == KIND_OPINION)
		text->Attr.Style |= ES_PASSWORD;	// blank string signifies some opinion
}

void TEditMotivesDialog::OKBNClicked()
{
	if (IsValid())
	{
		TDatedContainer *container = Container();

		*container << this;
		container->kind = motivesKind;
		// 2008:141 LPR: avoid tContainer and pass motivesKind
		Parent->ShowWindow(SW_HIDE);
		trouble.PostMove(container, container->date, decisionKind);
		Parent->ShowWindow(SW_SHOW);
		container->kind = decisionKind;
		CmOk();
	}
}

void TEditMotivesDialog::SetupWindow()
{
	TWhateverTextBaseDialog::SetupWindow();

	if (edit)
	{
		Container()->kind = motivesKind;
		trouble.Name(Container());
		Container()->kind = decisionKind;
		trouble.BackupNames();
	}
}

void TEditMotivesDialog::AttachBNClicked()
{
	Container()->kind = motivesKind;
	trouble.Edit(edit);
	Container()->kind = decisionKind;
}

void TEditMotivesDialog::SwapBNClicked()
{
	Container()->kind = motivesKind;
	trouble.Swap();
	Container()->kind = decisionKind;
}

void TEditMotivesDialog::RightBNClicked()
{
	judicialActDialog->AttachRight(this, trouble, motivesKind);
}
