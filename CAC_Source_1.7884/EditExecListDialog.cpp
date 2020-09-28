DEFINE_RESPONSE_TABLE1(TEditExecListDialog, TGroupDialog)
//{{TEditExecListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDADDRESS, AddressBNEnable),
	EV_BN_CLICKED(IDADDRESS, AddressBNClicked),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_COMMAND(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDEDIT, EditBNEnable),
	EV_COMMAND(IDEDIT, EditBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_COMMAND(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDOK, OKBNEnable),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_LVN_ITEMCHANGING(IDC_EDIT_EXEC_LIST_LAIDS, LaidsLVNItemChanging),
	EV_LVN_ITEMCHANGING(IDC_EDIT_EXEC_LIST_SUMS, SumsLVNItemChanging),
//{{TEditExecListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditExecListDialog Implementation}}

TEditExecListDialog::TEditExecListDialog(TWindow* parent, TExecList *execList, TGArray<TLinkWind> *xLinks, bool tEdit,
	int resId)
:
	TGroupDialog(parent, execList, resId), edit(tEdit),
		sumsEnabled((execList->execListKind == EXEC_LIST_COUNTRY || EXEC_LIST_PRIV_SUMS) &&
			(!edit || execList->sums.Count()))
{
	links = new TExecListLinkListWindow(this, IDC_EDIT_EXEC_LIST_LAIDS, xLinks);
	sums = new TExecListSumListWindow(this, IDC_EDIT_EXEC_LIST_SUMS, execList);
	execListNo = new TLongFace(this, IDC_EDIT_EXEC_LIST_EXEC_LIST_NO, "F_EXEC_LIST_NO", SIZE_OF_NO);
}

bool TEditExecListDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		TExecList execList;

		*Group() << this;
		execList << Group();

		if (!edit && execList.execListNo)
		{
			msql m;

			execList.FloatCriteria(m, "F_EXEC_LIST_NO");
			m.AddLong("F_EXEC_LIST_NO", execList.execListNo);
			TQuery q(str(m));

			if (q.Read())
				isValid = error("Вече има изпълнителен лист с този номер.");
		}

		if (isValid)
			isValid = links->IsValid();
	}

	return isValid;
}

void TEditExecListDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	execListNo->EnableWindow(!edit && Default->post == POST_ADMIN);
	sums->EnableWindow(sumsEnabled);
}

void TEditExecListDialog::AddressBNEnable(TCommandEnabler &tce)
{
	tce.Enable(links->Group());
}

void TEditExecListDialog::AddressBNClicked()
{
	links->Address();
}

void TEditExecListDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sumsEnabled);
}

void TEditExecListDialog::AddBNClicked()
{
	sums->Add();
}

void TEditExecListDialog::EditBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sumsEnabled && sums->Group());
}

void TEditExecListDialog::EditBNClicked()
{
	sums->Edit();
}

void TEditExecListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sumsEnabled && sums->Group());
}

void TEditExecListDialog::DeleteBNClicked()
{
	sums->Delete();
}

void TEditExecListDialog::OKBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sums->GetCount() > 0 || !sumsEnabled);
}

void TEditExecListDialog::OKBNClicked()
{
	if (IsValid() && (!edit || ExecList()->Save(true)))
		CmOk();
}

bool TEditExecListDialog::LaidsLVNItemChanging(TLwNotify &lwn)
{
	return links->ItemChanging(lwn);
}

bool TEditExecListDialog::SumsLVNItemChanging(TLwNotify &lwn)
{
	return sums->ItemChanging(lwn);
}
