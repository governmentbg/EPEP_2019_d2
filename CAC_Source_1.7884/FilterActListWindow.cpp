//{{TFilterActListWindow Implementation}}

TFilterActListWindow::TFilterActListWindow(TWindow* parent, int resourceId, TUnfilteredActsGroup *tUnfiltered)
:
	TGroupListWindow(parent, resourceId, new TFilterActSortArray, 356), unfiltered(tUnfiltered)
{
	Add(
		new TGroupListColumn("", filter_attached, 12, LVCFMT_CENTER | LVCFMT_NO_SEARCH),
		// 2015:327 from TRegisterActListWindow; modified
		new TGroupListColumn("Вид дело  Номер/Година", filter_lawsuit, 104),
		new TGroupListColumn("Предмет", "F_EXTRA_TEXT", 40, LVCFMT_RIGHT),
		new TGroupListColumn("Присъединен към", filter_decision_kind, 136),
		new TGroupListColumn("Дата", filter_decision_date, 52, LVCFMT_RIGHT),
		NULL
	);
}

void TFilterActListWindow::Filter()
{
	filter_act(Parent, (TRCDKeyContainer *) Group());
	RedrawGroup();
}

void TFilterActListWindow::Query()
{
	TRCDKeyContainer *container = (TRCDKeyContainer *) Group();

	if (strchr(KIND_SURROUNDS, container->kind))
		query_surrouendoc((TSurroundment *) container);
	else
		query_event(container);
}

void TFilterActListWindow::Refresh()
{
	invisible.Flush();
	TGroupListWindow::Refresh();
}

bool TFilterActListWindow::Inserted(TIndexContainer *container, long subject, char lawKind)
{
	if (electric_exist(container, TYPE_TEXT) && !electric_exist(container, TYPE_HTML))
	{
		container->AddExtraText(TSubject::Code(subject, lawKind));
		InsertGroup(container);
		return true;
	}

	return false;
}

bool TFilterActListWindow::Inserted(TDecision *decision, long subject, char lawKind)
{
	// decision or rider: re-check date, the select below is inexact
	return decision->fromDate >= unfiltered->minDate && decision->fromDate <= unfiltered->maxDate &&
		Inserted((TIndexContainer *) decision, subject, lawKind);
}

void TFilterActListWindow::LoadData(const char *)
{
	TWaitWindow wait(this, WAIT);
	TSubject subject;
	char lawKind;
	CDate minDate = Today;

	subject.Add(new TChar("F_LAW_KIND", &lawKind));
	minDate.year--;
	minDate.year--;

	if (minDate.year < YEAR_MIN)
		minDate = DATE_MIN;
	else if (!minDate.Valid())
		minDate += 1;

	if (FILTER_DECISIONS)
	{
		msql m("T_DECISION T JOIN T_LAWSUIT L ON L.F_NO = T.F_NO AND L.F_YEAR = T.F_YEAR AND L.F_TYPE = T.F_TYPE",
			"T.*, L.F_SUBJECT, L.F_KIND AS F_LAW_KIND");
		TDecision *decision = new TDecision;

		m.AddRange("T.F_DATE", minDate, unfiltered->maxDate);
		m.AddChars("T.F_TYPE", unfiltered->types);
		m.AddRange("(T.F_FROM_DATE", unfiltered->minDate, unfiltered->maxDate);
		m.NextOr();
		m.AddRange("T.F_RETURNED", unfiltered->minDate, unfiltered->maxDate);
		m.cat(")");
		m.AddFlags("T.F_FLAGS", DECISION_PUBLIC, DECISION_PUBLIC, DECISION_ALL);

		TQuery q(str(m));
		while (q.Read())
		{
			bool required = false;

			*decision << q;
			subject << q;

			if (decision->Motivated())
			{
				TDecisionRider *tContainer = new TDecisionRider(decision, KIND_MOTIVES);

				if (Inserted(tContainer, subject.subject, lawKind))
					required = true;
				else
					delete tContainer;
			}

			if (any(decision->opinion))
			{
				TDecisionRider *tContainer = new TDecisionRider(decision, KIND_OPINION);

				if (Inserted(tContainer, subject.subject, lawKind))
					required = true;
				else
					delete tContainer;
			}

			if (Inserted(decision, subject.subject, lawKind))
				decision = new TDecision;
			else if (required)
			{
				invisible.Add(decision);
				decision = new TDecision;
			}
		}

		delete decision;
	}

	if (FILTER_SESSIONS)
	{
		msql m("T_SESSION T JOIN T_LAWSUIT L ON L.F_NO = T.F_NO AND L.F_YEAR = T.F_YEAR AND L.F_TYPE = T.F_TYPE",
			"T.*, L.F_SUBJECT, L.F_KIND AS F_LAW_KIND");
		TSession *session = new TSession;

		m.AddRange("T.F_DATE", unfiltered->minDate, unfiltered->maxDate);
		m.AddChars("T.F_TYPE", unfiltered->types);
		m.AddFlags("T.F_FLAGS", SESSION_PRIVATE, 0, SESSION_ALL);

		TQuery q(str(m));
		while (q.Read())
		{
			*session << q;
			subject << q;

			if (Inserted(session, subject.subject, lawKind))
				session = new TSession;
		}

		delete session;
	}

	if (FILTER_OTHERS)
	{
		msql m("T_SURROUNDMENT T JOIN T_LAWSUIT L ON L.F_NO = T.F_LAWSUIT_NO AND L.F_YEAR = T.F_LAWSUIT_YEAR "
			"AND L.F_TYPE = T.F_LAWSUIT_TYPE JOIN T_SESSION S ON S.F_NO = T.F_LAWSUIT_NO AND "
			"S.F_YEAR = T.F_LAWSUIT_YEAR AND S.F_TYPE = T.F_LAWSUIT_TYPE AND S.F_KIND = T.F_SESSION_KIND "
			"AND S.F_DATE = T.F_DATE",
			"T.*, L.F_SUBJECT, L.F_KIND AS F_LAW_KIND");
		TSurroundment *surroundment = new TSurroundment;

		m.AddRange("T.F_DATE", minDate, unfiltered->maxDate);
		m.AddChars("T.F_TYPE", unfiltered->types);
		m.AddRange("T.F_DECISION_DATE", unfiltered->minDate, unfiltered->maxDate);
		m.AddIsNull("T.F_DECISION_KIND", false);
		m.AddFlags("S.F_FLAGS", SESSION_PRIVATE, 0, SESSION_ALL);

		TQuery q(str(m));
		while (q.Read())
		{
			*surroundment << q;
			subject << q;

			if (Inserted(surroundment, subject.subject, lawKind))
				surroundment = new TSurroundment;
		}

		delete surroundment;
	}
}
