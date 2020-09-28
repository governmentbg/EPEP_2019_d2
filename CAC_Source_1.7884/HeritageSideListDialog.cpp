DEFINE_RESPONSE_TABLE1(THeritageSideListDialog, TDecisionSideListDialog)
//{{THeritageSideListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDC_HERITAGE_SIDE_LIST_HERITAGE, HeritageBNEnable),
	EV_BN_CLICKED(IDC_HERITAGE_SIDE_LIST_HERITAGE, HeritageBNClicked),
//{{THeritageSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{THeritageSideListDialog Implementation}}

THeritageSideListDialog::THeritageSideListDialog(TWindow* parent, TDecision *decision, TSession *session, TLawsuit *lawsuit,
	long sideMode)
:
	TDecisionSideListDialog(parent, decision, session, lawsuit, sideMode, IDD_HERITAGE_SIDE_LIST)
{
	heritage = new TPushButton(this, IDC_HERITAGE_SIDE_LIST_HERITAGE);
}

void THeritageSideListDialog::HeritageBNEnable(TCommandEnabler &tce)
{
	TSideWind *sideWind = sideWinds->SideWind();
	tce.Enable(sideWind && strchr(INVOLVEMENT_HERITAGES, sideWind->involvement));
}

void THeritageSideListDialog::HeritageBNClicked()
{
	if (GetTickCount() > lastTicks + 100)
	{
		TRect rect = heritage->GetWindowRect();
		uint flags = TPM_CENTERALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON;
		TPopupMenu menu;
		mstr results(RESULT_HERITAGE_STATES);

		results.cach(IndexContainer()->Status());

		for (const char *result = str(results); *result; result++)
			menu.AppendMenu(MF_STRING, *result, Result->Seek(*result));

		int command = TrackPopupMenu(menu.GetHandle(), flags, (rect.left + rect.right) / 2, rect.bottom, 0, Handle, NULL);

		sideWinds->Heritage(command);
		lastTicks = GetTickCount();
	}
}
