DEFINE_RESPONSE_TABLE1(TEditExecListTDepDialog, TEditDialog)
//{{TEditExecListTDepDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditExecListTDepDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditExecListTDepDialog Implementation}}

TEditExecListTDepDialog::TEditExecListTDepDialog(TWindow* parent, TPrintExecList *execList)
:
	TEditDialog(parent, execList, true, IDD_EDIT_EXEC_LIST_TDEP)
{
	header1 = new TStatic(this, IDC_EDIT_EXEC_LIST_TDEP_HEADER_1);
	header2 = new TStatic(this, IDC_EDIT_EXEC_LIST_TDEP_HEADER_2);
	header3 = new TStatic(this, IDC_EDIT_EXEC_LIST_TDEP_HEADER_3);
	tdep = new TNIATDepAliasFace(this, IDC_EDIT_EXEC_LIST_TDEP_TDEP, "F_TDEP", FLAG_NULL, &execList->receivers);
	tdepNo = new TNIATDepInNoFace(this, IDC_EDIT_EXEC_LIST_TDEP_IN_NO, "F_TDEP_IN_NO");
	tdepDate = new TDateFace(this, IDC_EDIT_EXEC_LIST_TDEP_DATE, "F_TDEP_DATE", false);
	new TStringFace(this, IDC_EDIT_EXEC_LIST_TDEP_TEXT, "F_TDEP_TEXT", SIZE_OF_TDEP_TEXT);
}

void TEditExecListTDepDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	TPrintExecList *execList = ExecList();
	print_exec_list_set_headers(ExecList(), header1, header2, header3);

	tdep->SelectElement(execList->tdep, execList->tdepUCNType);

	if (execList->nia.no && execList->receiverTypes == RECEIVERS_TDNAP)
		tdep->EnableWindow(false);
}

bool TEditExecListTDepDialog::IsValid()
{
	bool isValid = TEditDialog::IsValid();

	if (isValid)
	{
		if (!any(tdepNo) != tdepDate->Date().Empty())
			isValid = error("Входящия номер и датата на получаване не са комплектни.");
		else if (tdepDate->Date() > Today)
			isValid = error("Датата на получаване е по-голяма от текущата дата.");
		else if (!any(tdepNo))
		{
			for (int i = 0; i < ExecList()->collects.Count(); i++)
			{
				if (!(ExecList()->collects[i]->flags & EXEC_LIST_COLLECT_VOLUNTARY))
				{
					isValid = error("Входящия номер и датата не могат да бъдат изтрити докато има задължения "
						"които не са събрани доброволно.");
					break;
				}
			}
		}
	}

	return isValid;
}

void TEditExecListTDepDialog::OKBNClicked()
{
	ExecList()->tdepUCNType = tdep->UCNType();
	TEditDialog::OKBNClicked();
}
