//{{TInRegEventListWindow Implementation}}

TInRegEventListWindow::TInRegEventListWindow(TWindow* parent, int resourceId, TOutReg *tOutReg, TInReg *tInReg, bool tSave)
:
	TGroupListWindow(parent, resourceId, new TGArray<TInRegEvent>, 344), outReg(tOutReg), inReg(tInReg), save(tSave)
{
	Add(
		new TGroupListColumn("Вид документ", inreg_event_kind, 136),
		new TGroupListColumn("Номер/година", inreg_event_no_year, 60, LVCFMT_RIGHT),
		new TGroupListColumn("Статус", inreg_event_status, 136),
		NULL
	);

	if (!save)
	{
		TGArray<TInReg> inRegs;
		load_list(inReg, &inRegs);

		for (int i = 0; i < inRegs.Count(); i++)
		{
			TInRegEvent *inRegEvent = new TInRegEvent;
			init_event(outReg, inRegs[i], inRegEvent);
			array->Add(inRegEvent);
		}

		loadData = false;
	}
}

void TInRegEventListWindow::GetEvents(TGArray<TInRegEvent> *events)
{
	TInReg inReg;

	for (int i = 0; i < array->Count(); i++)
	{
		TInRegEvent *tInRegEvent = new TInRegEvent;

		*tInRegEvent << (*array)[i];
		inReg.key = tInRegEvent->key;

		if (inReg.Try("F_KIND, F_DATE"))
		{
			tInRegEvent->kind = inReg.kind;
			tInRegEvent->date = inReg.date;
		}

		events->Add(tInRegEvent);
	}
}

TGroup *TInRegEventListWindow::NewGroup() const
{
	return new TInRegEvent;
}

void TInRegEventListWindow::Criteria(msql &m, TGroup *group)
{
	((TInRegEvent *) group)->OutCriteria(outReg);
	TGroupListWindow::Criteria(m, group);
}

bool TInRegEventListWindow::AddGroup(TGroup *group)
{
	TInReg tInReg;
	TInRegEvent *inRegEvent = TYPECHECK_PTR_CAST(TInRegEvent, group);

	if (!select_send_inreg(this, inReg, &tInReg, (TGArray<TInRegEvent> *) array))
		return false;

	// 2015:107 LPR: FIX: non-save was missing init_event()
	if (save)
		return send_inreg(outReg, &tInReg, inRegEvent);

	init_event(outReg, &tInReg, inRegEvent);
	return true;
}

bool TInRegEventListWindow::DeleteGroup(TGroup *group)
{
	bool result = !save || delete_inreg_event((TInRegEvent *) group);
	return result;
}
