//{{TPerformAssignmentListWindow Implementation}}

TPerformAssignmentListWindow::TPerformAssignmentListWindow(TWindow* parent, int resourceId, TGSArray<TAssignment> *array, TDistribution *distribution, const char *kinds, TUserLoadListBox *tLoad)
:
	TAssignmentListWindow(parent, resourceId, array, distribution, kinds, 100), load(tLoad)
{
	columns.Insert(new TGroupListColumn("Съдия", assignment_assign, 72), 2);
	columns.Add(new TGroupListColumn("Разпореждане", assignment_resolution, 120));
	resolutions = RESOLUTION_LAWABLES;
}

void TPerformAssignmentListWindow::InsertVisual(TGroup *group, int index)
{
	TAssignment *assignment = (TAssignment *) group;

	if (assignment->resolution != RESOLUTION_ASSIGNED)
	{
		distribution->Distribute(assignment);
		assignment->resolution = RESOLUTION_ASSIGNED;
		assignment->text.cpy("");
	}

	strcpy(assignment->judge, ::Default->ucn);
	TAssignmentListWindow::InsertVisual(group, index);
}

void TPerformAssignmentListWindow::DrawSecond(TListWindItem &lwi, TGroup *group)
{
	TAssignment *assignment = (TAssignment *) group;

	TAssignmentListWindow::DrawSecond(lwi, group);

	lwi.iSubItem = 6;
	lwi.SetText(str(assignment->text));
	SetItem(lwi);
}

bool TPerformAssignmentListWindow::EditGroup(TGroup *group)
{
	TAssignment *assignment = (TAssignment *) group;
	char oldJudge[SIZE_OF_UCN];

	strcpy(oldJudge, assignment->assign);

	if (TEditPerformAssignmentDialog(this, assignment, distribution).Execute() == IDOK)
	{
		load->Change(oldJudge, assignment->assign);
		return true;
	}

	return false;
}

bool TPerformAssignmentListWindow::AskDiscard(int count)
{
	TResolution tResolution;

	return ask("Резолюциите %s или %s на %d преписк%c ще бъдат отменени заради липса на съдия. "
		"Желаете ли да продължите?", tResolution.Seek(RESOLUTION_PREPARED), Resolution->Seek(RESOLUTION_ASSIGNED),
		count, "иа"[count == 1]);
}
