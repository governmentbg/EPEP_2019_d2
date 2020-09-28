DEFINE_RESPONSE_TABLE1(TForceDecisionDialog, TGroupDialog)
//{{TForceDecisionDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TForceDecisionDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TForceDecisionDialog Implementation}}

TForceDecisionDialog::TForceDecisionDialog(TWindow* parent, TDecision *decision, TLawsuit *lawsuit)
:
	TForceDateDialog(parent, decision, lawsuit,
		decision->flags & DECISION_MOTIVES ? "��������� �� �������� �� �������� ���" : "������� �� �������� ���")
{
}

void TForceDecisionDialog::OKBNClicked()
{
	lawsuit->forceDate = lawsuitForceDate->Date();
	TForceDateDialog::OKBNClicked();
}

bool TForceDecisionDialog::IsValid()
{
	bool isValid = TForceDateDialog::IsValid();

	if (isValid)
	{
		if (lawsuitForceDate->Date().Empty())
			isValid = !strchr(KIND_ENDOCS, kind->C()) || ask("������ �� ������� � ������� ���� �� ������ � ������ �� � ���������. ������� �� �� ����������?");
		// 2007:346 LPR: moved before ask() to make sure it's executed
		else if (lawsuitForceDate->Date() > Today)
			isValid = error("������ �� ������� � ������� ���� �� ������ � ������ � ��-������ �� �������� ����.");
		// 2007:346 LRQ: error() -> ask()
		else if (lawsuitForceDate->Date() < date->Date())
			isValid = ask("������ �� ������� � ������� ���� �� ������ � ������ � ����� ������ �� �������� ���. ������� �� �� ����������?");
	}

	return isValid;
}

void TForceDecisionDialog::SetupWindow()
{
	TForceDateDialog::SetupWindow();
	lawsuitForceDate->EnableWindow(true);
}
