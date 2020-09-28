DEFINE_RESPONSE_TABLE1(TJoinStreetsDialog, TQuickDialog)
	EV_LVN_ITEMCHANGING(IDC_MANUALLY_ADD, SourceLVNItemChanging),
	EV_LVN_ITEMCHANGING(IDC_OFFICIALLY_ADD, TargetLVNItemChanging),
	EV_COMMAND_ENABLE(IDJOIN, JoinBNEnable),
	EV_BN_CLICKED(IDJOIN, JoinBNClicked),
END_RESPONSE_TABLE;

TJoinStreetsDialog::TJoinStreetsDialog(TWindow *parent, TGSArray<TJoinStreet> *tSources, TGSArray<TJoinStreet> *tTargets, int resId)
	: TQuickDialog(parent, resId)
{
	sources = new TJoinStreetsListWindow(this, IDC_MANUALLY_ADD, tSources);
	targets = new TJoinStreetsListWindow(this, IDC_OFFICIALLY_ADD, tTargets);
}

void TJoinStreetsDialog::JoinBNClicked()
{
	sources->Join(targets->Street(), targets);
}

void TJoinStreetsDialog::JoinBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sources->Group() && targets->Group());
}

bool TJoinStreetsDialog::SourceLVNItemChanging(TLwNotify &lwn)
{
	return sources->ItemChanging(lwn);
}

bool TJoinStreetsDialog::TargetLVNItemChanging(TLwNotify &lwn)
{
	return targets->ItemChanging(lwn);
}
