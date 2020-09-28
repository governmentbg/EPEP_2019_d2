//{{TSelectExecListListBox Implementation}}

TSelectExecListListBox::TSelectExecListListBox(TWindow* parent, int resourceId, TGArray<TExecList> *execLists)
:
	TCloneListBox(parent, resourceId, execLists, NULL)
{
}

bool TSelectExecListListBox::CheckCollects()
{
	for (int i = 0; i < array->Count(); i++)
	{
		if (GetSel(i))
		{
			TExecList *execList = TYPECHECK_PTR_CAST(TExecList, (*array)[i]);
			TExecListCollect collect;
			msql m;

			collect << execList;
			collect.FloatCriteria(m, "F_TYPE");
			TQuery q(str(m));

			if (q.Read() && !ask("�� ������������ ���� %ld/%D �� %s %ld/%ld ��� ������� ����������. %s",
				execList->execListNo, &execList->execListDate, Type->Seek(execList->type), execList->no,
				execList->year, Q_CONTINUE))
			{
				return false;
			}
		}
	}

	return true;
}
