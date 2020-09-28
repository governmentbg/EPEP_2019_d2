DEFINE_RESPONSE_TABLE1(TEditRewardDialog, TGroupDialog)
//{{TEditRewardDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_REWARD_ACCOUNT_TYPE, AccountTypeCBNSelChange),
//{{TEditRewardDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditRewardDialog Implementation}}

TEditRewardDialog::TEditRewardDialog(TWindow* parent, TExpense *expense, TIndexContainer *tContainer, TSession *tSession,
	TLawsuit *tLawsuit, bool tEdit, bool tState, int resId)
:
	TGroupDialog(parent, expense, resId), container(tContainer), session(tSession), lawsuit(tLawsuit), edit(tEdit),
		state(tState)
{
	// 2005:363 LPR: copied from penalities
	const char *reasons = "";
	{
		TLawsuit lawsuit;
		lawsuit << expense;

		if (lawsuit.Get("F_TYPE, F_KIND, F_SUBJECT") && Subject->Seek(lawsuit.key.type, lawsuit.subject))
			reasons = Subject->Reasons(lawsuit.kind);
	}

	accountType = new TCharAliasFace(this, IDC_EDIT_REWARD_ACCOUNT_TYPE, "F_ACCOUNT_TYPE", AccountType, NULL);
	sum1 = new TMoneyFace(this, IDC_EDIT_REWARD_SUM_1, "F_SUM_1", SIZE_OF_SUM);
	sumState = new TCharAliasFace(this, IDC_EDIT_REWARD_SUM_STATE, "F_SUM_STATE", SumState, NULL);
	new TStringFace(this, IDC_EDIT_REWARD_TEXT, "F_TEXT", SIZE_OF_EXPENSE_TEXT);
	//new TCheckFace(this, IDC_EDIT_REWARD_EXPENSE, "F_FLAGS", SUM_EXPENSE);
	sum2 = new TMoneyFace(this, IDC_EDIT_REWARD_SUM_2, "F_SUM_2", SIZE_OF_SUM);
	paidNote = new TStringFace(this, IDC_EDIT_REWARD_PAID_NOTE, "F_PAID_NOTE", SIZE_OF_EXPENSE_NOTE);
	reasonType = new TLongAliasFace(this, IDC_EDIT_REWARD_REASON_TYPE, "F_REASON_TYPE", ReasonType,
		any(reasons) ? reasons : NULL, edit ? ALIAS_EMPTY | ALIAS_ABCSORT : FLAG_ACTIVE | ALIAS_EMPTY | ALIAS_ABCSORT);
	//print = new TTwinButton(this, IDPRINT, IDRIGHT);
	print = new TPushButton(this, IDPRINT);
}

void TEditRewardDialog::Save(char subpoenaKind)
{
	//if (IsValid())	// 2017:059 comout, checked by callers
	{
		*Expense() << this;

		if (Expense()->Save(edit))
		{
			if (subpoenaKind)
				print_rko(this, Expense(), container, session, lawsuit, subpoenaKind);
			CmOk();
		}
	}
}

void TEditRewardDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	// 2003:177 LRQ; 2009:357 LRQ/LPR: zeros ask moved to IsValid()
	if (edit && state && sumState->C() == SUM_STATE_PREPARE)
	{
		sumState->SetXCrit(SUM_STATE_REWARDS);
		sumState->EnableWindow(true);
	}

	// 2006:003 LRQ
	if (strchr(INVOLVEMENT_REASONABLES, Expense()->involvement))
		reasonType->EnableWindow(true);
	else
		reasonType->SetL(0);

	AccountTypeCBNSelChange();
}

void TEditRewardDialog::OKBNClicked()
{
	// 2006:179 warn for empty records
	if (IsValid())
		Save('\0');
}

void TEditRewardDialog::PrintBNClicked()
{
	if (IsValid())
	{
		if (COURT_LOCK)
			Save(*SUBPOENA_KIND_REWARDS);
		else
		{
			// TODO: make some popup menu utility functions if more menus
			TPopupMenu menu;
			TRect rect = print->GetWindowRect();

			for (const char *sk = SUBPOENA_KIND_REWARDS; *sk; sk++)
			{
				SubpoenaKind->Seek(*sk);
				if (!menu.AppendMenu(MF_STRING, *sk, SubpoenaKind->name))
					fatal("%d: failed to append menu item", *sk);
			}

			uint flags = TPM_CENTERALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON;
			int command = TrackPopupMenu(menu.GetHandle(), flags, (rect.left + rect.right) / 2, rect.bottom, 0, Handle, NULL);

			if (command > 0)
				Save((char) command);
		}
	}
}

void TEditRewardDialog::AccountTypeCBNSelChange()
{
	paidNote->EnableWindow(accountType->C() == ACCOUNT_TYPE_DEPOSITS);
}

bool TEditRewardDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (reasonType->IsWindowEnabled() && reasonType->L() == 0)
			isValid = error("Основанието за назначаване е задължително.");
		// 2006:179 LRQ: comout and allow empty records
		// 2006:200 LRQ: allow zeros for SUM_PREPARE only
		else if (sum1->M() == 0.0 && sum2->M() == 0.0)
		{
			isValid = sumState->C() == SUM_STATE_PREPARE
				? ask("Не е попълнена нито една от двете суми. Желаете ли да продължите?")
				: error("Една от двете суми е задължителна.");
		}
	}

	return isValid;
}
