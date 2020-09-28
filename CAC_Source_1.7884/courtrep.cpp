#include "cac.h"

// ----- TReportCourt --------------------------------------------------------
TReportCourt::TReportCourt(const char *tUCN, char tUCNType)
{
	strcpy(ucn, tUCN);
	// for Try(): EQUAL contains all courts as senders
	ucnType = tUCNType != UCN_INSTANCE_LOWER ? tUCNType : UCN_INSTANCE_EQUAL;
	memset(values, '\0', sizeof values);

	if (Try())
	{
		if (ucnType != UCN_INSTANCE_EQUAL)
			prior = PRIOR_PRIV_EXEC;
		else
		{
			Court->Seek(atol(ucn));
			ucnType = UCN_INSTANCE_LOWER;
			prior = Court->courtArea == COURT_AREA ? PRIOR_THIS_AREA : PRIOR_OTHER_AREA;
		}
	}
	else
	{
		strcpy(name, ucn);
		prior = PRIOR_UNKNOWN;
	}
}

TReportCourt::TReportCourt(const char *tName)
{
	strcpy(name, tName);
	strcpy(ucn, EMPTY);
	memset(values, '\0', sizeof values);
	prior = PRIOR_SUMMARY;
}

bool TReportCourt::operator<=(const TReportCourt &that) const
{
	return prior != that.prior ? prior < that.prior : strcmpi(name, that.name) <= 0;
}

size_t TReportCourt::Positive() const
{
	for (size_t column = 0; column < COURT_COLUMNS_MAX; column++)
		if (values[column])
			return column + 1;

	return 0;
}

#define repcourt OFFICE_GROUP_CAST(TReportCourt)

static void repcourt_index(mstr &m, TGroup *group)
{
	if (repcourt->index)
		m.printf("0%ld0", repcourt->index);
}

static void repcourt_value(mstr &m, TGroup *group)
{
	int col;

	if (sscanf(repcourt->Name, "F_C_%d", &col) > 0 && --col < COURT_COLUMNS_MAX)
	{
		itom(repcourt->values[col], m);
		repcourt->values[col] = 0;		// mark as used
	}
}

#undef repcourt

static const offield repcourt_fields[] =
{
	{ "F_NAME",		NULL },
	{ "F_INDEX",	repcourt_index },
	{ "*F_C_",		repcourt_value },
	{ NULL,		NULL }
};

// ----- TReportCourtArray ---------------------------------------------------
void TReportCourtArray::Increment(const char *ucn, char ucnType, int column)
{
	TReportCourt *court;
	int i;

	for (i = 0; i < Count(); i++)
		if (!strcmp(CourtAt(i)->ucn, ucn) && CourtAt(i)->ucnType == ucnType)
			break;

	if (i < Count())
		court = CourtAt(i);
	else
	{
		court = new TReportCourt(ucn, ucnType);
		Add(court);
	}

	court->values[column]++;
}

TReportCourt *TReportCourtArray::Summary(const char *name)
{
	TReportCourt *total = new TReportCourt(name);

	for (int i = 0; i < Count(); i++)
		for (int col = 0; col < COURT_COLUMNS_MAX; col++)
			total->values[col] += CourtAt(i)->values[col];

	return total;
}

// ----- TCourtReport --------------------------------------------------------
TCourtReport::TCourtReport(TWindow *parent, const TReportGroup &group)
	: TReport(parent, group)
{
}

int TCourtReport::FindFinishedColumn(constant *resultSplitFinisheds, int columnCount)
{
	if (FindFinalizer())
	{
		mstr resultCombinedFinisheds;

		for (int i = 0; i < columnCount; i++)
			resultCombinedFinisheds.cat(resultSplitFinisheds[i]);

		decisions.Flush();
		decision = find_finalizer(session, decisions, str(resultCombinedFinisheds));

		if (!decision)
			dprintf("Липсва съдебен акт за %s от %D по %s\n", Kind->Seek(session.kind), &session.date, sLawKey);
		else
		{
			for (int column = 0; column < columnCount; column++)
				if (strchr(resultSplitFinisheds[column], decision->result))
					return column;

			dprintf("Резултат '%s' от %s/%D по %s не попада в нито една колона/клетка\n",
				Result->Seek(decision->result), Kind->Seek(decision->kind), &decision->fromDate, sLawKey);
		}
	}

	return INT_MAX;
}

void TCourtReport::Increment(int column)
{
	if (!atob(lawsuit.ucn))
	{
		if (reportGroup.connectType == CONNECT_TYPE_LOWER_INSTANCE)
		{
			dlprintf("За %s няма първоинстанционно дело\n", sLawKey);
			return;
		}

		TConnect connect;

		connect.key = lawsuit.key;
		connect.connectType = CONNECT_TYPE_LOWER_INSTANCE;

		if (!connect.Try("F_UCN_TYPE, F_UCN"))
		{
			dlprintf("За %s няма %s/Първоинстанционно дело\n", sLawKey, ConnectType->Seek(reportGroup.connectType));
			return;
		}

		strcpy(lawsuit.ucn, connect.ucn);
		lawsuit.ucnType = connect.ucnType;
	}

	courts.Increment(lawsuit.ucn, lawsuit.ucnType, column);
}

void TCourtReport::Check()
{
	for (int i = 0; i < courts.Count(); i++)
	{
		const TReportCourt *court = courts[i];
		size_t positive = court->Positive();

		if (positive)
		{
			dprintf("Непрехвърлени данни: съд %s, колона %u, стойност %ld\n", court->name, positive,
				court->values[positive - 1]);
		}
	}
}

void TCourtReport::ShowUpper()
{
	tr1start = office.Find("tr1 start");
	tr1final = office.Find("tr1 final");

	office.Emit(NULL, tr1start, report_fields, this);
	courts.Add(courts.Summary("ОБЩО:"));

	int Count = courts.Count();

	for (int i = 0; i < Count; i++)
	{
		TReportCourt *court = courts[i];

		court->index = (i + 1) % Count;
		office.Emit(tr1start, tr1final, repcourt_fields, court);
	}
}
