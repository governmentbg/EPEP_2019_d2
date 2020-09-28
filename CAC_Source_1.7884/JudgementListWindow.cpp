//{{TJudgementListWindow Implementation}}

TJudgementListWindow::TJudgementListWindow(TWindow* parent, long sideMode, int resourceId, TDecision *decision,
	TLawsuit *lawsuit)
:
	TSideWindListWindow(parent, sideMode, resourceId, decision, lawsuit)
{
	columns.Insert(new TGroupListColumn("������", judgement_judged, 48), 2);
	columns[6]->width -= 48;
}

void TJudgementListWindow::Enter()
{
	TJudgement *judgement = Judgement();
	const char *s = check_personal_data(Decision(), judgement);

	if (s)
		error(s);
	else
	{
		TJudgement tJudgement;	// 2010:090 LPR: list data protection
		tJudgement << judgement;

		if (TLocalJudgementDialog(this, &tJudgement, Decision(), lawsuit, edit).Execute() == IDOK)
		{
			*judgement << tJudgement;
			RedrawGroup();
		}
	}
}

void TJudgementListWindow::Exec()
{
	if (Decision()->returned.Empty())
		error("������ ���� �� ��������� �� ��������.");
	else
	{
		TJudgement *judgement = Judgement();
		bool tLimited = !strcmp(Decision()->Punishments(lawsuit), PUNISHMENT_NOT_KNOWNS);

		if (tLimited ? any(judgement->text) : strchr(JUDGED_RIGHT_EXECS, judgement->judged) != NULL)
		{
			TJudgement tJudgement;	// 2010:090 LPR: list data protection
			tJudgement << judgement;

			if (TExecJudgementDialog(this, &tJudgement, Decision()).Execute() == IDOK)
			{
				*judgement << tJudgement;
				RedrawGroup();
			}
		}
		else
			error(tLimited ? "���� ������ ����� �� ������." : "������ �� � �������.");
	}
}

void TJudgementListWindow::Extra()
{
	TJudgement *judgement = Judgement();
	const char *s = check_personal_data(Decision(), judgement);

	if (s)
		error(s);
	else
		arrest_sidewind(this, lawsuit, judgement, false);
}

TGroup *TJudgementListWindow::NewGroup() const
{
	TJudgement *judgement = new TJudgement;
	InitGroup(judgement);
	return judgement;
}
