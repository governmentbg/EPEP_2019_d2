DEFINE_RESPONSE_TABLE1(TExecListListDialog, TScaleDialog)
//{{TExecListListDialogRSP_TBL_BEGIN}}
	EV_CBN_SETFOCUS(IDC_EXEC_LIST_LIST_TYPE, SetSearch),
	EV_EN_SETFOCUS(IDC_EXEC_LIST_LIST_NO, SetSearch),
	EV_EN_SETFOCUS(IDC_EXEC_LIST_LIST_YEAR, SetSearch),
	EV_EN_SETFOCUS(IDC_EXEC_LIST_LIST_EXEC_LIST_DATE, SetSearch),
	EV_MESSAGE(WM_FOCUS_FACE, EvFocusFace),
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_COMMAND_ENABLE(IDENTER, EnterBNEnable),
	EV_BN_CLICKED(IDENTER, EnterBNClicked),
	EV_COMMAND_ENABLE(IDLOCAL, LocalBNEnable),
	EV_BN_CLICKED(IDLOCAL, LocalBNClicked),
	EV_LVN_ITEMCHANGING(IDC_EXEC_LIST_LIST_EXEC_LISTS, ExecListsLVNItemChanging),
//{{TExecListListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TExecListListDialog Implementation}}

TExecListListDialog::TExecListListDialog(TWindow* parent, int resId)
:
	TScaleDialog(parent, resId), lastReceiverTypes(0)
{
	type = new TCharAliasFace(this, IDC_EXEC_LIST_LIST_TYPE, "F_TYPE", Type, Default->colleges);
	no = new TLongFace(this, IDC_EXEC_LIST_LIST_NO, "F_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_EXEC_LIST_LIST_YEAR, "F_YEAR");
	execListDate = new TDateFace(this, IDC_EXEC_LIST_LIST_EXEC_LIST_DATE, "F_EXEC_LIST_DATE", false);
	new TDateFaceUpDown(this, IDC_EXEC_LIST_LIST_EXEC_LIST_DATE_UPDN, execListDate);
	execLists = new TExecListListWindow(this, IDC_EXEC_LIST_LIST_EXEC_LISTS);
	search = new TPushButton(this, IDSEARCH);
	enter = new TPushButton(this, IDENTER);
	local = new TPushButton(this, IDLOCAL);
}

void TExecListListDialog::SetSearch()
{
	SetDefault(enter, search);
}

bool TExecListListDialog::IsValid()
{
	bool isValid = TScaleDialog::IsValid();

	if (isValid)
	{
		if (!no->L() && !year->L() && !*execListDate->S())
			isValid = error("Необходимо е да се попълни номер, година или дата.");
	}

	return isValid;
}

void TExecListListDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();
	execListDate->Clear();
}

void TExecListListDialog::SearchBNClicked()
{
	if (IsValid())
	{
		TExecList execList;
		execList << this;
		execLists->Search(&execList);
	}
}

void TExecListListDialog::EnterBNEnable(TCommandEnabler &tce)
{
	TPrintExecList *execList = execLists->ExecList();
	int receiverTypes;

	if (execList)
	{
		tce.Enable(execList->sent);
		receiverTypes = execList->receiverTypes;
	}
	else
	{
		tce.Enable(false);
		receiverTypes = 0;
	}

	if (receiverTypes != lastReceiverTypes)
	{
		static constant titles[3] = { "в НАП", "от ЧСИ", "от ДСИ" };
		mstr m("Получаване");
		bool first = true;

		for (int i = 0; i < 3; i++)
		{
			if (receiverTypes & (1 << i))
			{
				m.printf("%s%s", first ? "\n" : " / ", titles[i]);
				first = false;
			}
		}

		enter->SetCaption(str(m));
		lastReceiverTypes = receiverTypes;
	}
}

void TExecListListDialog::EnterBNClicked()
{
	TEditExecListTDepDialog(this, execLists->ExecList()).Execute();
}

void TExecListListDialog::LocalBNEnable(TCommandEnabler &tce)
{
	tce.Enable(execLists->Group());
}

void TExecListListDialog::LocalBNClicked()
{
	TExecListCollectListDialog(this, execLists->ExecList()).Execute();
}

long TExecListListDialog::EvFocusFace(TParam1 wParam, TParam2)
{
	if (wParam)
		SetDefault(search, enter);

	return MW_FACE_FOCUSED;
}

bool TExecListListDialog::ExecListsLVNItemChanging(TLwNotify &lwn)
{
	return execLists->ItemChanging(lwn);
}
