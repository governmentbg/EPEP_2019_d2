DEFINE_RESPONSE_TABLE1(TLaunchPerformAssignmentDialog, TScaleDialog)
//{{TLaunchPerformAssignmentDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDASSIGN, AssignBNClicked),
	EV_COMMAND_ENABLE(IDASSIGN, AssignBNEnable),
//{{TLaunchPerformAssignmentDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLaunchPerformAssignmentDialog Implementation}}

// 2007:186 LPR: twin engine: unknown count for firm lawsuits
TLaunchPerformAssignmentDialog::TLaunchPerformAssignmentDialog(TWindow* parent, TGSArray<TAssignment> *tArray, const char *tLawKinds, int resId)
:
	TScaleDialog(parent, resId), array(tArray), lawKinds(tLawKinds)
{
	kinds = new TStringListBox(this, IDC_LAUNCH_PERFORM_ASSIGNMENT_KINDS);
	new TPushButton(this, IDCANCEL);
}

void TLaunchPerformAssignmentDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();

	for (int i = 0; lawKinds[i]; i++)
	{
		mstr m;
	#if COURT_TYPE == COURT_AREAL
		if (lawKinds[i] == KIND_FIRM_LAW)
			m.cat(Kind->Seek(KIND_FIRM_LAW));
		else
	#endif  // AREAL
			m.printf("%s - %d преписк%c", Kind->Seek(lawKinds[i]), array[i].Count(), array[i].Count() == 1 ? 'а' : 'и');
		kinds->AddString(str(m));
	}
}

void TLaunchPerformAssignmentDialog::AssignBNClicked()
{
	int index = kinds->GetSelIndex();
	TGSArray<TAssignment> *tArray = array + index;

	// N.B. only non-firm perform assignment may return true
	if (perform_assignment(this, tArray, lawKinds[index]))
	{
		mstr m;

		tArray->Flush();
		realize_crit(C2S[lawKinds[index]], tArray);

		m.printf("%s - %d преписк%c", Kind->Seek(lawKinds[index]), tArray->Count(), tArray->Count() == 1 ? 'а' : 'и');
		kinds->InsertString(str(m), index);
		kinds->DeleteString(++index);
	}
}

void TLaunchPerformAssignmentDialog::AssignBNEnable(TCommandEnabler &tce)
{
	tce.Enable(kinds->GetSelIndex() >= 0);
}
