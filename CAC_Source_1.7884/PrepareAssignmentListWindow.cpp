//{{TPrepareAssignmentListWindow Implementation}}

TPrepareAssignmentListWindow::TPrepareAssignmentListWindow(TWindow* parent, int resourceId, TGSArray<TAssignment> *array, TDistribution *distribution, char *kinds)
:
	TAssignmentListWindow(parent, resourceId, array, distribution, kinds, 112)
{
	columns.Insert(new TGroupListColumn("Докл. на", assignment_judge, 76), 2);
	resolutions = RESOLUTION_PREPARABLES;
}

bool TPrepareAssignmentListWindow::IsValid()
{
	bool isValid = TAssignmentListWindow::IsValid();

	if (isValid)
	{
		for (int i = 0; i < array->Count(); i++)
		{
			TAssignment *assignment = (TAssignment *) (*array)[i];

			if (atof(assignment->judge) && !assignment->subject)
			{
				isValid = error("Вх.№ %ld/%ldг. има попълнен \"Докл. на\", но не и предмет.", assignment->key.no,
					assignment->key.year);
				break;
			}
		}
	}

	return isValid;
}

void TPrepareAssignmentListWindow::InsertVisual(TGroup *group, int index)
{
	TAssignment *assignment = (TAssignment *) group;

	if (assignment->resolution != RESOLUTION_PREPARED)
	{
		suggest_prepare_judge(assignment);
		assignment->resolution = RESOLUTION_PREPARED;
	}

	if (DISTRIB_NOTIFYING)
		assignment->text.cpy(DISTRIB_NOTIFY_NAME);

	TAssignmentListWindow::InsertVisual(group, index);
}

bool TPrepareAssignmentListWindow::EditGroup(TGroup *group)
{
	return TEditPrepareAssignmentDialog(Parent, (TAssignment *) group, distribution).Execute() == IDOK;
}

bool TPrepareAssignmentListWindow::AskDiscard(int count)
{
	return ask("Резолюциите %s на %d %s ще бъдат отменени заради липса на съдия. Желаете ли да продължите?", Resolution->Seek(RESOLUTION_PREPARED), count, count == 1 ? "преписка" : "преписки");
}
