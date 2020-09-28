//{{TSurroundmentListWindow Implementation}}

TSurroundmentListWindow::TSurroundmentListWindow(TWindow *parent, int resourceId, TSession *tSession, TDecision *tDecision,
	TGArray<TSurroundment> *surroundments)
	: TGroupListWindow(parent, resourceId, surroundments, 292), session(tSession), decision(tDecision)
{
#if RANGECHECK
	if (!surroundments->Count())
		fatal("TSurroundmentListWindow: surroundments empty");
#endif  // RANGECHECK
	Add(new TGroupListColumn("Документ", group_print, 280));
}

bool TSurroundmentListWindow::EditGroup(TGroup *group)
{
	return TLocalSurroundmentDialog(this, (TSurroundment *) group, session, decision).Execute() == IDOK;
}

void TSurroundmentListWindow::Print()
{
	print_end_docs_and_surroundments(this, session, (TGArray<TSurroundment> *) array);
}

TGroup *TSurroundmentListWindow::NewGroup() const
{
	return new TSurroundment;
}
