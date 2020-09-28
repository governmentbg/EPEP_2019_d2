//{{TRegisterActListWindow Implementation}}

TRegisterActListWindow::TRegisterActListWindow(TWindow* parent, int resourceId, TDateFace *tParentDate)
:
	TGroupListWindow(parent, resourceId, NULL, 324), parentDate(tParentDate)
{
	Add(
		new TGroupListColumn("", already_attached, 20, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		new TGroupListColumn("Вид дело  Номер/Година", group_print, 104),
		new TGroupListColumn("Вид документ", container_kind, 136),
		new TGroupListColumn("Дата", "F_DATE", 52, LVCFMT_RIGHT),
		NULL
	);
}

void TRegisterActListWindow::SetCrit(char tKind, const CDate &tDate)
{
	kind = tKind;
	date = tDate;
	Refresh();
}

bool TRegisterActListWindow::DiffDate(const TRegisterAct *act)
{
	if (act->date != date)
	{
		date = act->date;
		parentDate->SetDate(date);
		Refresh();

		for (int index = 0; index < array->Count(); index++)
		{
			TRegisterAct *tAct = (TRegisterAct *) GroupAt(index);

			if (tAct->key == act->key && tAct->kind == act->kind && tAct->date == act->date &&
				!strcmp(tAct->judge, act->judge) && tAct->actNo == act->actNo)
			{
				SetVisualIndex(index);
				return true;
			}
		}
	}

	return false;
}

void TRegisterActListWindow::View()
{
	((TRegisterAct *) Group())->ViewFiles(NULL);
}

void TRegisterActListWindow::Criteria(msql &m, TGroup *group)
{
	group->FloatCriteria(m, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_JUDGE, F_ACT_NO, F_TEXT_MTIME, F_IMAGE_MTIME, "
		"F_TARGET_KIND");
	m.AddOrder("F_DATE, F_ACT_NO");
}

TGroup *TRegisterActListWindow::NewGroup() const
{
	TRegisterAct *act = new TRegisterAct;

	act->kind = kind;
	act->date = date;
	return act;
}

bool TRegisterActListWindow::AddGroup(TGroup *group)
{
	return register_act(this, (TRegisterAct *) group, false, OF_JUDICIAL_ACT "%02ld + документ(и)");
}

bool TRegisterActListWindow::EditGroup(TGroup *group)
{
	return register_act(this, (TRegisterAct *) group, true, "на документ(и) към регистриран акт %02ld");
}

bool TRegisterActListWindow::DeleteGroup(TGroup *group)
{
	if (TGroupListWindow::DeleteGroup(group))
	{
		TRegisterAct *act = (TRegisterAct *) group;
		char text[SIZE_OF_LOG_TEXT];

		sprintf(text, "на регистриран акт %02ld + документ(и)", act->actNo);
		log(LOG_DELETE, CM_JUDGE_REGISTER_ACT, act, text);
		return true;
	}

	return false;
}
