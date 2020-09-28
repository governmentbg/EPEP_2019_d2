//{{TExpenseListWindow Implementation}}

TExpenseListWindow::TExpenseListWindow(TWindow* parent, int resourceId, TSideWind *tSideWind, TRCDKeyContainer *tContainer,
	TSession *tSession, TLawsuit *tLawsuit)
:
	TGroupListWindow(parent, resourceId, NULL, 288), sideWind(tSideWind), container(tContainer), session(tSession),
		lawsuit(tLawsuit)
{
	Add(new TGroupListColumn("Глоба/възнаграждение", group_print, 276));
	// 2016:134 FIX: was removing container F_TEXT
}

void TExpenseListWindow::Penality()
{
	TExpense *expense = (TExpense *) NewGroup();
	expense->sumType = SUM_TYPE_PENALITY;

	if (TEditPenalityDialog(this, expense, lawsuit, false).Execute() == IDOK)
	{
		SetVisualIndex(InsertGroup(expense));
		UpdateVisual();
	}
	else
		delete expense;
}

void TExpenseListWindow::Reward()
{
	if (sideWind->ucnType == UCN_CITIZEN_UCN || ask("Лицето е регистрирано в базата данни със служебен код вместо с ЕГН. "
		"Желаете ли да продължите?"))
	{
		TExpense *expense = (TExpense *) NewGroup();

		expense->sumType = SUM_TYPE_REWARD;
		expense->sumState = (container->Status() == RESULT_FIXED) ? SUM_STATE_PREPARE : SUM_STATE_NEW;

		if (TEditRewardDialog(this, expense, TYPECHECK_PTR_CAST(TIndexContainer, container), session, lawsuit, false,
			false).Execute() == IDOK)
		{
			SetVisualIndex(InsertGroup(expense));
			UpdateVisual();
		}
		else
			delete expense;
	}
}

TGroup *TExpenseListWindow::NewGroup() const
{
	TExpense *expense = new TExpense;
	// 2016:134 FIX: sideWind/container F_TEXT were too big
	TSideWind tSideWind;

	tSideWind << sideWind;
	*expense << tSideWind;
	return expense;
}

bool TExpenseListWindow::AddGroup(TGroup *group)
{
	TExpense *expense = (TExpense *) group;

	expense->sumType = SUM_TYPE_TAX;
	expense->sumState = SUM_STATE_PAID;
	return TEditTaxDialog(this, expense, false).Execute() == IDOK;
}

bool TExpenseListWindow::EditGroup(TGroup *group)
{
	TExpense *expense = (TExpense *) group;
	expense->involvement = sideWind->involvement;

	switch (expense->sumType)
	{
		case SUM_TYPE_PENALITY : return TEditPenalityDialog(this, expense, lawsuit, true).Execute() == IDOK;
		case SUM_TYPE_REWARD : return TEditRewardDialog(this, expense, TYPECHECK_PTR_CAST(TIndexContainer, container),
			session, lawsuit, true, container->Status() != RESULT_FIXED).Execute() == IDOK;
		case SUM_TYPE_TAX : return TEditTaxDialog(this, expense, true).Execute() == IDOK;
	}

	fatal("%d: sumType not supported by edit", expense->sumType);
	return false;
}
