DEFINE_RESPONSE_TABLE1(TFindReplaceDialog, TNTDialog)
//{{TFindReplaceDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDADD, AddBNClicked),
	EV_COMMAND_ENABLE(IDADD, AddBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_LBN_SELCHANGE(IDC_FIND_REPLACE_PAIRS, PairsCBNSelChange),
	EV_WM_CLOSE,
	EV_WM_TIMER,
//{{TFindReplaceDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFindReplaceDialog Implementation}}

TFindReplaceDialog::TFindReplaceDialog(TWindow* parent, TFilterDocument *tFD, TIArray<TReplacePair> *tPairs)
:
	TNTDialog(parent, IDD_FIND_REPLACE), fd(tFD), locked(true), timer(0)
{
	find = new TStringFace(this, IDC_FIND_REPLACE_FIND, "F_FIND", SIZE_OF_FIND_REPLACE_TEXT);
	replace = new TStringFace(this, IDC_FIND_REPLACE_REPLACE, "F_REPLACE", SIZE_OF_FIND_REPLACE_TEXT);
	pairs = new TFindReplaceListBox(this, IDC_FIND_REPLACE_PAIRS, tPairs);
}

TFindReplaceDialog::~TFindReplaceDialog()
{
	if (timer)
		KillTimer(timer);
}

bool TFindReplaceDialog::IsValid()
{
	return find_replace_valid(find->S(), replace->S(), true);
}

bool TFindReplaceDialog::IsOpen()
{
	if (locked && !fd->IsOpen())
	{
		locked = false;
		sleep_sync();
		CloseDialog(IDCANCEL);
	}

	return locked;
}

void TFindReplaceDialog::SetupWindow()
{
	TNTDialog::SetupWindow();

	window_move_upright_adjusted(this);

	if ((timer = SetTimer(0, 250, NULL)) == 0)
		error("Грешка %lu при създаване на таймер.", GETLASTERROR);
}

void TFindReplaceDialog::AddBNClicked()
{
	if (IsValid())
	{
		TReplacePair *pair = new TReplacePair(find->S(), replace->S());

		if (!pairs->AddPair(pair))
		{
			error("Текстът за търсене се дублира.");
			delete pair;
		}
		else if (IsOpen())
		{
			TWaitWindow wait(this, "Търсене и замяна...");
			fd->FindReplace(pair);
		}
	}
}

void TFindReplaceDialog::AddBNEnable(TCommandEnabler &tce)
{
	tce.Enable(pairs->GetCount() < FIND_REPLACES_MAX);
}

void TFindReplaceDialog::DeleteBNClicked()
{
	pairs->DeletePair();
}

void TFindReplaceDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(pairs->Pair());
}

void TFindReplaceDialog::PairsCBNSelChange()
{
	TReplacePair *pair = pairs->Pair();

	if (pair)
	{
		find->SetS(pair->find);
		replace->SetS(pair->replace);
	}
}

void TFindReplaceDialog::EvClose()
{
	if (IsOpen())
		ShowWindow(SW_MINIMIZE);
}

void TFindReplaceDialog::EvTimer(uint)
{
	if (!IsOpen())
	{
		KillTimer(timer);
		timer = 0;
	}
}

TResult TFindReplaceDialog::EvCommand(uint id, THandle hWndCtl, uint notifyCode)
{
	if (id == IDCANCEL)
	{
		EvClose();
		return 0;
	}

	return TNTDialog::EvCommand(id, hWndCtl, notifyCode);
}
