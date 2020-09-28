DEFINE_RESPONSE_TABLE1(TEditPenalityDialog, TGroupDialog)
//{{TEditPenalityDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_PENALITY_SUM_STATE, SumStateCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_PENALITY_REJECT_KIND, RejectKindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_EDIT_PENALITY_PENALITY_TYPE, PenalityTypeCBNSelChange),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TEditPenalityDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditPenalityDialog Implementation}}

TEditPenalityDialog::TEditPenalityDialog(TWindow* parent, TExpense *expense, TLawsuit *, bool tEdit)
:
	TGroupDialog(parent, expense, IDD_EDIT_PENALITY), edit(tEdit)
{
	strcpy(deciments, KIND_DECISIONS);
	strcach(deciments, KIND_SETTLEMENT);

	const char *penalities = "";
	{
		TLawsuit lawsuit;
		lawsuit.key = expense->key;

		if (lawsuit.Get("F_TYPE, F_KIND, F_SUBJECT") && Subject->Seek(lawsuit.key.type, lawsuit.subject))
			penalities = Subject->Penalities(lawsuit.kind);
	}

	penalityType = new TLongAliasFace(this, IDC_EDIT_PENALITY_PENALITY_TYPE, "F_PENALITY_TYPE", PenalityType,
		any(penalities) ? penalities : NULL, edit ? ALIAS_ABCSORT : FLAG_ACTIVE | ALIAS_ABCSORT);
	new TMoneyFace(this, IDC_EDIT_PENALITY_SUM_1, "F_SUM_1", SIZE_OF_SUM);
	sumState = new TCharAliasFace(this, IDC_EDIT_PENALITY_SUM_STATE, "F_SUM_STATE", SumState, SUM_STATE_PENALITIES);
	new TStringFace(this, IDC_EDIT_PENALITY_TEXT, "F_TEXT", SIZE_OF_EXPENSE_TEXT);
	paidDate = new TDateFace(this, IDC_EDIT_PENALITY_PAID_DATE, "F_PAID_DATE", false);
	rejectKind = new TCharAliasFace(this, IDC_EDIT_PENALITY_REJECT_KIND, "F_REJECT_KIND", Kind, KIND_VISIBLE_SESSIONS,
		ALIAS_EMPTY);
	rejectDate = new TDateFace(this, IDC_EDIT_PENALITY_REJECT_DATE, "F_REJECT_DATE", false);
	rejectIndex = new TIndexFace(this, IDC_EDIT_PENALITY_REJECT_INDEX, "F_REJECT_INDEX", KIND_CLOSED_SESSIONS,
		SIZE_OF_SESSION_INDEX);
	paidNote = new TStringFace(this, IDC_EDIT_PENALITY_PAID_NOTE, "F_PAID_NOTE", SIZE_OF_EXPENSE_NOTE);

	rejectKindLabel = new TStatic(this, IDC_EDIT_PENALITY_REJECT_KIND_LABEL);
	rejectIndexLabel = new TStatic(this, IDC_EDIT_PENALITY_REJECT_INDEX_LABEL);
	rejectCourt = new TSenderAliasFace(this, IDC_EDIT_PENALITY_REJECT_COURT, "F_REJECT_COURT", UCN_INSTANCE_HIGHER,
		ALIAS_EMPTY);
}

void TEditPenalityDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Expense() << this;
		Expense()->rejectKind = adjust_xxx_kind(Expense()->rejectKind, rejectIndex, KIND_CLOSED_SESSIONS);
		if (Expense()->Save(edit))
			CmOk();
	}
}

void TEditPenalityDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	PenalityTypeCBNSelChange();
}

void TEditPenalityDialog::SumStateCBNSelChange()
{
	PenalityType->Seek(penalityType->L());

	if (sumState->C() == SUM_STATE_PAID)
	{
		paidDate->EnableWindow(true);
		paidDate->SetDate(Expense()->paidDate);
		paidNote->EnableWindow(true);
		paidNote->SetS(Expense()->paidNote);
	}
	else
	{
		paidDate->EnableWindow(false);
		paidDate->SetS("");
		paidNote->EnableWindow(false);
		paidNote->SetS("");
	}

	if (sumState->C() == SUM_STATE_REJECTED || sumState->C() == SUM_STATE_ALTERED)
	{
		rejectKind->EnableWindow(true);
		rejectKind->SetC(Expense()->rejectKind);
		rejectDate->EnableWindow(true);
		rejectDate->SetDate(Expense()->rejectDate);
		if (PenalityType->flags & PENALITY_TYPE_HIREJ)
		{
			rejectCourt->SetS(Expense()->rejectCourt);
			rejectCourt->EnableWindow(true);
		}
		else
		{
			rejectCourt->SetS("");
			rejectCourt->EnableWindow(false);
		}
	}
	else
	{
		rejectKind->EnableWindow(false);
		rejectKind->SetC(KIND_NULL);
		rejectDate->EnableWindow(false);
		rejectDate->SetS("");
		rejectIndex->EnableWindow(false);
		rejectCourt->EnableWindow(false);
		rejectCourt->SetS("");
	}

	RejectKindCBNSelChange();
}

void TEditPenalityDialog::RejectKindCBNSelChange()
{
	adjust_xxx_index(rejectKind->C(), rejectIndex, KIND_CLOSED_SESSIONS);
}

bool TEditPenalityDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		bool hirej = rejectCourt->IsWindowEnabled();

		if (sumState->C() == SUM_STATE_PAID && paidDate->Date().Empty())
			isValid = error("Липсва дата на внасяне.");
		else if ((sumState->C() == SUM_STATE_REJECTED || sumState->C() == SUM_STATE_ALTERED)  && (rejectKind->C() == '\0' || rejectDate->Date().Empty()))
			isValid = error("Липсват вид и/или дата на %s за изменение/отмяна на глобата.", hirej ? "съдебен акт" : "заседание");
		else if ((sumState->C() == SUM_STATE_REJECTED || sumState->C() == SUM_STATE_ALTERED) && hirej && !atob(rejectCourt))
			isValid = error("Липсва съд за изменение/отмяна на глобата.");
		else if ((sumState->C() == SUM_STATE_REJECTED || sumState->C() == SUM_STATE_ALTERED) && !hirej)
		{
			TSession session;

			session.key = Expense()->key;
			session.kind = rejectKind->C();
			session.date = rejectDate->Date();

			if (!session.Get())
				isValid = false;
		}
	}

	return isValid;
}

void TEditPenalityDialog::PenalityTypeCBNSelChange()
{
	PenalityType->Seek(penalityType->L());

	if (PenalityType->flags & PENALITY_TYPE_HIREJ)
	{
		// 2005:056 //KIND_VISIBLE_DECIMENTS
		rejectKind->SetXCrit(deciments);
		rejectKindLabel->SetText("Съдебен акт");
		rejectIndexLabel->SetText("Съд");
		rejectCourt->ShowWindow(SW_SHOW);
	}
	else
	{
		rejectKind->SetXCrit(KIND_VISIBLE_SESSIONS);
		rejectKindLabel->SetText("Заседание");
		rejectIndexLabel->SetText("Пореден номер");
		rejectIndex->ShowWindow(SW_SHOW);
		rejectCourt->ShowWindow(SW_HIDE);
	}

	SumStateCBNSelChange();
}

void TEditPenalityDialog::PrintBNClicked()
{
	if (IsValid())
	{
		TExpense tExpense;

		tExpense << Expense();
		tExpense << this;
		print_single_penalty(this, &tExpense);
	}
}
