#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
// ----- TFloatJurorGroup ----------------------------------------------------
class TFloatJurorGroup : public TFloatGroup
{
public:
	TFloatJurorGroup();

	virtual void FormCriteria(msql &m);

	long load;
	long manualRed;
	char lawKinds[SIZE_OF_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	char compositions[SIZE_OF_COMPOSITIONS];
	char sesKinds[SIZE_OF_KINDS];
	CDate minDate;
	CDate maxDate;
	long important;
	long limited;
	long external;
	TGArray<TUserAliasGroup> sesJudges;
	TGArray<TKnownPerson> jurors;
	char types[SIZE_OF_TYPES];
};

enum { FLOAT_JUROR_LOAD_FLAT = 2 };		// ENOENT

TFloatJurorGroup::TFloatJurorGroup()
	: TFloatGroup("T_SESSION")
{
	Add(
		new TRadio("F_LOAD", &load, TRUE),
		new TRadio("F_LOAD_FLAT", &load, FLOAT_JUROR_LOAD_FLAT),
		new TRadio("F_REDISTRIB", &load, FALSE),
		new TCheck("F_MANUAL_RED", &manualRed, TRUE),
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TCharList("F_SES_KINDS", sesKinds, sizeof sesKinds),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TCheck("F_IMPORTANT", &important, LAWSUIT_IMPORTANT),
		new TCheck("F_LIMITED", &limited, LAWSUIT_LIMITED),
		new TCheck("F_EXTERNAL", &external, TRUE),
		new TCharList("F_TYPES", types, SIZE_OF_TYPES),
		NULL
	);

	load = TRUE;
	manualRed = -1;
	important = limited = -1;
}

void TFloatJurorGroup::FormCriteria(msql &m)
{
	const char *tables;
	const char *fields;

	if (load)
	{
		if (external)
		{
			tables = "T_EXTERN_SESSION T";
			fields = "T.*";
		}
		else
		{
			tables = "T_SESSION T JOIN T_LAWSUIT L ON L.F_NO = T.F_NO AND L.F_YEAR = T.F_YEAR AND L.F_TYPE = T.F_TYPE";
			fields = "T.*, L.F_KIND AS F_LAW_KIND";
		}
	}
	else
	{
		if (external)
		{
			tables = "T_JUROR_LOG T JOIN T_EXTERN_SESSION E ON E.F_NO = T.F_NO AND E.F_YEAR = T.F_YEAR "
				"AND E.F_TYPE = T.F_TYPE AND E.F_KIND = T.F_KIND AND E.F_DATE = T.F_DATE";
			fields = "DISTINCT T.F_NO, T.F_YEAR, T.F_TYPE, T.F_EXCESS";
		}
		else
		{
			tables = "T_JUROR_LOG T JOIN T_LAWSUIT L ON L.F_NO = T.F_NO AND L.F_YEAR = T.F_YEAR "
				"AND L.F_TYPE = T.F_TYPE";
			fields = "DISTINCT T.F_NO, T.F_YEAR, T.F_TYPE, L.F_KIND AS F_LAW_KIND, T.F_EXCESS";
		}
	}

	m.Begin(tables, fields);
	m.AddRange("T.F_NO", minNo, maxNo);
	m.AddRange("T.F_YEAR", minYear, maxYear);
	m.AddCompos(load ? "T.F_COMPOSITION" : external ? "E.F_COMPOSITION" : "L.F_COMPOSITION", compositions);

	if (external)
		m.AddChars("T.F_TYPE", types, TYPE_LAWSUITS);
	else
	{
		m.AddChars("L.F_KIND", lawKinds, KIND_LAWSUITS);

		long mask = 0, value = 0;

		if (important != -1)
		{
			mask |= LAWSUIT_IMPORTANT;
			value |= important;
		}

		if (limited != -1)
		{
			mask |= LAWSUIT_LIMITED;
			value |= limited;
		}

		m.AddFlags("L.F_FLAGS", mask, value, LAWSUIT_ALL);
	}

	if (load)
	{
		m.AddRange("T.F_DATE", minDate, maxDate);
		m.AddChars("T.F_KIND", sesKinds);
	#if 20170174
		static constant JUROR_NAMES[] = { "T.F_JUROR", "T.F_REJUROR", "T.F_JUROR_X", "T.F_RESERVE", NULL };
		m.AddJuSet(JUROR_NAMES, jurors, "F_UCN", true);
	#else  // 20170174
		m.AddTriad("T.F_JUROR", "T.F_REJUROR", "T.F_JUROR_X", jurors, "F_UCN", true);
	#endif  // 20170174

		if (!external)
		{
			m.AddArray("F_UCN", sesJudges, "T.F_JUDGE");
			m.AddChars("NOT T.F_RESULT", RESULT_UNCARRIEDS);
		}
	}
	else
	{
		CDateTime minStamp, maxStamp;

		(CDate &) minStamp = minDate;

		if (!maxDate.Empty())
		{
			(CDate &) maxStamp = maxDate;
			(CTime &) maxStamp = TIME_MAX;
		}

		m.AddRange("T.F_STAMP", minStamp, maxStamp);
		switch (manualRed)
		{
			case TRUE : m.AddChars("T.F_OPER", C2S[JUROR_DISTRIB_MANUAL]); break;
			case FALSE : m.AddChars("T.F_OPER", JUROR_DISTRIB_AUTO_REDS); break;
			default : m.AddChars("T.F_OPER", JUROR_DISTRIB_REDISTRIBS);
		}
		m.AddZLong("T.F_EXCESS", external);
		m.AddOrder("T.F_TYPE, T.F_YEAR, T.F_NO");
	}
}

// ----- TLoadJurorSession ---------------------------------------------------
class TLoadJurorSession : public TSession
{
public:
	TLoadJurorSession();
	static TGroup *Clone();

	bool operator<=(const TLoadJurorSession &that);

	char lawKind;

	static const offield fields[];
};

TLoadJurorSession::TLoadJurorSession()
	: TSession(TLoadJurorSession::Clone)
{
	Add(new TCharAlias("F_LAW_KIND", &lawKind));
}

TGroup *TLoadJurorSession::Clone()
{
	return new TLoadJurorSession;
}

bool TLoadJurorSession::operator<=(const TLoadJurorSession &that)
{
	return strcmp(juror, that.juror) ? strcmp(juror, that.juror) < 0 :
		key.year != that.key.year ? key.year < that.key.year : lawKind != that.lawKind ? lawKind < that.lawKind :
		key.no != that.key.no ? key.no < that.key.no : date != that.date ? date < that.date : kind < that.kind;
}

#define jurorses OFFICE_GROUP_CAST(TLoadJurorSession)

static void jurorses_juror(mstr &m, TGroup *group) { print_name(m, jurorses->juror, UCN_CITIZEN_UCN); }

static void jurorses_law_kind(mstr &m, TGroup *group)
{
	if (jurorses->lawKind)
	{
		Kind->Seek(jurorses->lawKind);
		m.cat(Kind->alias);
	}
	else
	{
		Type->Seek(jurorses->key.type);
		m.cat(Type->alias);
	}
}

static void jurorses_ses_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(jurorses->kind)); }
static void jurorses_composition(mstr &m, TGroup *group) { m.printf("%s с-в", Composition->Seek(jurorses->composition)); }

static void jurorses_key_no(mstr &m, TGroup *group)
{
	m.printf("%s%ld", TBaseSession::ExcessText(jurorses->lawKind == KIND_NULL), jurorses->key.no);
}

static void jurorses_secretar(mstr &m, TGroup *group)
{
	if (atob(jurorses->secretar))
		print_name(m, jurorses->secretar, UCN_CITIZEN_UCN);
}

static void jurorses_judge(mstr &m, TGroup *group)
{
	if (atob(jurorses->judge))
		print_name(m, jurorses->judge, UCN_CITIZEN_UCN);
}

#undef jurorses

const offield TLoadJurorSession::fields[] =
{
	{ "F_JUROR",	jurorses_juror },
	{ "F_LAW_KIND",	jurorses_law_kind },
	{ "F_NO",		jurorses_key_no },
	{ "F_YEAR",		NULL },
	{ "F_SES_KIND",	jurorses_ses_kind },
	{ "F_DATE",		NULL },
	{ "F_COMPOSITION",jurorses_composition },
	{ "F_SECRETAR",	jurorses_secretar },
	{ "F_JUDGE",	jurorses_judge },
	{ NULL, NULL }
};

class TFloatJurorBaseQuery : public TFloatQuery
{
public:
	TFloatJurorBaseQuery(TWindow *parent, const char *criteria, const char *name, TFloatJurorGroup *tGroup);

	virtual void Execute() = 0;

	TFloatJurorGroup *group;
	CDate periodStart;
	CDate periodFinal;
};

TFloatJurorBaseQuery::TFloatJurorBaseQuery(TWindow *parent, const char *criteria, const char *name, TFloatJurorGroup *tGroup)
	: TFloatQuery(parent, criteria, name), group(tGroup)
{
	Add(
		new TDate("F_PERIOD_START", &periodStart, false),
		new TDate("F_PERIOD_FINAL", &periodFinal, false),
		NULL
	);
}

#define jurorquery OFFICE_GROUP_CAST(TFloatJurorBaseQuery)

static void jurorquery_period(mstr &m, TGroup *group)
{
	if (jurorquery->periodStart.Empty())
		m.printf("до %D", &jurorquery->periodFinal);
	else if (jurorquery->periodFinal.Empty())
		m.printf("от %D", &jurorquery->periodStart);
	else
		m.printf("%D-%D", &jurorquery->periodStart, &jurorquery->periodFinal);
}

#undef jurorquery

// ----- TFloatJurorBaseLoadQuery --------------------------------------------
class TFloatJurorBaseLoadQuery : public TFloatJurorBaseQuery
{
public:
	TFloatJurorBaseLoadQuery(TWindow *parent, const char *criteria, const char *name, TFloatJurorGroup *group,
		const offield *const tFields);

	virtual void Execute();

	TGSArray<TLoadJurorSession> sessions;

protected:
	bool Selected(const char *juror);
	void FixDates(const TLoadJurorSession *session);
	virtual void AddJuror(const char *juror, const TLoadJurorSession *session) = 0;

	virtual void GetMarkers();
	void PreCount();

	const char *tr1count;
	const offield *const fields;
};

TFloatJurorBaseLoadQuery::TFloatJurorBaseLoadQuery(TWindow *parent, const char *criteria, const char *name,
	TFloatJurorGroup *group, const offield *const tFields)
	: TFloatJurorBaseQuery(parent, criteria, name, group), fields(tFields)
{
}

void TFloatJurorBaseLoadQuery::Execute()
{
	TWindowHideParent hide(parent);

	if (office.Open(name))
	{
		PreCount();
		GetMarkers();
		office.Emit(NULL, tr1start, fields, this);
		Count();
		office.Emit(tr1final, NULL, fields, this);
		Show();
	}
}

bool TFloatJurorBaseLoadQuery::Selected(const char *juror)
{
	if (atob(juror))
	{
		if (!group->jurors.Count())
			return true;

		if (group->jurors.Count())
			for (int i = 0; i < group->jurors.Count(); i++)
				if (!strcmp(juror, group->jurors[i]->ucn))
					return true;
	}

	return false;
}

void TFloatJurorBaseLoadQuery::FixDates(const TLoadJurorSession *session)
{
	if (periodStart.Empty() || (group->minDate.Empty() && session->date < periodStart))
		periodStart = session->date;

	if (session->date > periodFinal)
		periodFinal = session->date;
}

void TFloatJurorBaseLoadQuery::GetMarkers()
{
	tr1start = office.Find("tr1 start");
	tr1count = office.Find("tr1 count");
	tr1final = office.Find("tr1 final");
}

void TFloatJurorBaseLoadQuery::PreCount()
{
	TQuery q(criteria);
	TLoadJurorSession session;

	periodStart = group->minDate;

	if (periodStart <= Today)
		periodFinal = group->maxDate.Empty() ? Today : group->maxDate;

	while (q.Read())
	{
		wait->TickCount(++count, "обработени заседания");
		session << q;
		AddJuror(session.juror, &session);
		AddJuror(session.reJuror, &session);
		AddJuror(session.jurorX, &session);
		AddJuror(session.reserve, &session);
	}
}

// ----- TFloatJurorLoadQuery ------------------------------------------------
class TFloatJurorLoadQuery : public TFloatJurorBaseLoadQuery
{
public:
	TFloatJurorLoadQuery(TWindow *parent, const char *criteria, TFloatJurorGroup *group);

protected:
	virtual void AddJuror(const char *juror, const TLoadJurorSession *session);
	virtual void Count();

	long totalSessions;
	long totalLawsuits;

	static const offield fields[];
};

TFloatJurorLoadQuery::TFloatJurorLoadQuery(TWindow *parent, const char *criteria, TFloatJurorGroup *group)
	: TFloatJurorBaseLoadQuery(parent, criteria, "FloatJurS.htm", group, TFloatJurorLoadQuery::fields)
{
	Add(
		new TLong("F_TOTAL_SES", &totalSessions),
		new TLong("F_TOTAL", &totalLawsuits),
		NULL
	);
}

void TFloatJurorLoadQuery::AddJuror(const char *juror, const TLoadJurorSession *session)
{
	if (Selected(juror))
	{
		TLoadJurorSession *tSession = new TLoadJurorSession;

		*tSession << session;
		strcpy(tSession->juror, juror);
		sessions.Add(tSession);
		FixDates(session);
	}
}

void TFloatJurorLoadQuery::Count()
{
	TLoadJurorSession *lastSession = NULL;
	count = 0;

	for (int i = 0; i < sessions.Count(); i++)
	{
		TLoadJurorSession *session = sessions[i];

		if (!lastSession || strcmp(lastSession->juror, session->juror))
		{
			if (lastSession)
				office.Emit(tr1count, tr1final, fields, this);

			totalSessions = 1;
			totalLawsuits = 1;
		}
		else
		{
			if (session->key != lastSession->key)
				totalLawsuits++;

			totalSessions++;
		}

		office.Emit(tr1start, tr1count, TLoadJurorSession::fields, session);
		lastSession = session;
		wait->TickCount(++count, "отпечатани записа");
	}

	if (lastSession)
		office.Emit(tr1count, tr1final, fields, this);
}

const offield TFloatJurorLoadQuery::fields[] =
{
	{ "F_PERIOD",	jurorquery_period },
	{ "F_TOTAL_SES",	NULL },
	{ "F_TOTAL",	NULL },
	{ NULL, NULL }
};

// ----- TLoadJurorFlat ------------------------------------------------------
class TLoadJurorFlat : public TKnownAliasGroup
{
public:
	TLoadJurorFlat();
	static TGroup *Clone();

	long composition;
	TGArray<TLoadJurorSession> sessions;

	bool operator<=(const TLoadJurorFlat &that);

	static const offield fields[];
};

TLoadJurorFlat::TLoadJurorFlat()
	: TKnownAliasGroup(TLoadJurorFlat::Clone)
{
	Add(
		new TLongAlias("F_COMPOSITION", &composition),
		NULL
	);
}

TGroup *TLoadJurorFlat::Clone()
{
	return new TLoadJurorFlat;
}

bool TLoadJurorFlat::operator<=(const TLoadJurorFlat &that)
{
	return composition != that.composition ? composition < that.composition : strcmp(ucn, that.ucn) < 0;
}

#define jurorflat OFFICE_GROUP_CAST(TLoadJurorFlat)

static void jurorflat_composition(mstr &m, TGroup *group) { m.printf("%s с-в", Composition->Seek(jurorflat->composition)); }
static void jurorflat_juror(mstr &m, TGroup *group) { print_name(m, jurorflat->ucn, UCN_CITIZEN_UCN); }
static void jurorflat_count(mstr &m, TGroup *group) { itom(jurorflat->sessions.Count(), m); }

#undef jurorflat

const offield TLoadJurorFlat::fields[] =
{
	{ "F_COMPOSITION",jurorflat_composition },
	{ "F_JUROR",	jurorflat_juror },
	{ "F_COUNT",	jurorflat_count },
	{ NULL, NULL }
};

// ----- TFloatJurorLoadFlatQuery --------------------------------------------
class TFloatJurorLoadFlatQuery : public TFloatJurorBaseLoadQuery
{
public:
	TFloatJurorLoadFlatQuery(TWindow *parent, const char *criteria, TFloatJurorGroup *group);

protected:
	TGSArray<TLoadJurorFlat> jurors;
	static const offield fields[];

	virtual void AddJuror(const char *juror, const TLoadJurorSession *session);
	virtual void Count();
};

TFloatJurorLoadFlatQuery::TFloatJurorLoadFlatQuery(TWindow *parent, const char *criteria, TFloatJurorGroup *group)
	: TFloatJurorBaseLoadQuery(parent, criteria, "FloatJurA.htm", group, TFloatJurorLoadFlatQuery::fields)
{
}

void TFloatJurorLoadFlatQuery::AddJuror(const char *juror, const TLoadJurorSession *session)
{
	if (Selected(juror))
	{
		int i;
		TLoadJurorFlat *tJuror;

		for (i = 0; i < jurors.Count(); i++)
		{
			tJuror = jurors[i];

			if (tJuror->composition == session->composition && !strcmp(tJuror->ucn, juror))
			{
				for (int n = 0; n < tJuror->sessions.Count(); n++)
				{
					if (tJuror->sessions[n]->key == session->key)
						return;
				}

				break;
			}
		}

		if (i == jurors.Count())
		{
			tJuror = new TLoadJurorFlat;
			strcpy(tJuror->ucn, juror);
			tJuror->composition = session->composition;
			jurors.Add(tJuror);
		}

		tJuror->sessions.AddCopy(session);
		FixDates(session);
	}
}

void TFloatJurorLoadFlatQuery::Count()
{
	TLoadJurorFlat *lastJuror = NULL;
	count = 0;

	for (int i = 0; i < jurors.Count(); i++)
	{
		TLoadJurorFlat *juror = jurors[i];

		if (lastJuror && lastJuror->composition != juror->composition)
		{
			office.Emit(tr1count, tr1final, fields, this);
			sessions.Clear();
		}

		office.Emit(tr1start, tr1count, TLoadJurorFlat::fields, juror);

		for (int n = 0; n < juror->sessions.Count(); n++)
		{
			TLoadJurorSession *session = juror->sessions[n];
			int k;

			for (k = 0; k < sessions.Count(); k++)
				if (session->key == sessions[k]->key)
					break;

			if (k == sessions.Count())
				sessions.Add(session);
		}

		lastJuror = juror;
		wait->TickCount(++count, "отпечатани записа");
	}

	if (lastJuror)
	{
		office.Emit(tr1count, tr1final, fields, this);
		sessions.Clear();
	}
}

#define jurorflat OFFICE_GROUP_CAST(TFloatJurorLoadFlatQuery)

void jurorflat_total(mstr &m, TGroup *group) { itom(jurorflat->sessions.Count(), m); }

#undef jurorflat

const offield TFloatJurorLoadFlatQuery::fields[] =
{
	{ "F_PERIOD",	jurorquery_period },
	{ "F_TOTAL",	jurorflat_total },
	{ NULL, NULL }
};

// ----- functions -----------------------------------------------------------
static void float_juror_load(TWindow *parent, const char *criteria, TFloatJurorGroup *group)
{
	if (group->load == TRUE)
		TFloatJurorLoadQuery(parent, criteria, group).Execute();
	else
		TFloatJurorLoadFlatQuery(parent, criteria, group).Execute();
}

// ----- TFloatJurorRedistribQuery -------------------------------------------
class TFloatJurorRedistribQuery;

class TJurorFloatHistory : public TJurorHistory
{
public:
	TJurorFloatHistory(TFloatJurorRedistribQuery *tQuery);

protected:
	TFloatJurorRedistribQuery *query;
	char lawsuitKind;

	virtual void Event();
};

class TFloatJurorRedistribQuery : TFloatJurorBaseQuery
{
public:
	TFloatJurorRedistribQuery(TWindow *parent, const char *criteria, TFloatJurorGroup *group);

	void Execute();

	void Emit(TJurorFloatHistory *history);

protected:
	virtual void Count();

	static const offield header_fields[];
};

TFloatJurorRedistribQuery::TFloatJurorRedistribQuery(TWindow *parent, const char *criteria, TFloatJurorGroup *group)
	: TFloatJurorBaseQuery(parent, criteria, "FloatJurPr.htm", group)
{
}

void TFloatJurorRedistribQuery::Execute()
{
	TWindowHideParent hide(parent);

	if (office.Open(name, OFFICE_SKIP_HEADER))
	{
		GetMarkers();
		office.Emit(NULL, tr1start, null_fields, NULL);
		Count();
		office.Emit(tr1final, NULL, float_fields, this);

		TOffice header;

		if (header.Open(name, OFFICE_COPY_HEADER, office.Name))
		{
			header.Emit(NULL, NULL, header_fields, this);
			header.Close();
			Show();
		}
	}
}

TJurorFloatHistory::TJurorFloatHistory(TFloatJurorRedistribQuery *tQuery)
	: query(tQuery)
{
	Add(new TCharAlias("F_LAW_KIND", &lawsuitKind));
}

void TJurorFloatHistory::Event()
{
	query->Emit(this);
}

void TFloatJurorRedistribQuery::Emit(TJurorFloatHistory *history)
{
	if (strchr(JUROR_DISTRIB_REDISTRIBS, history->oper))
	{
		Increment();

		if (periodStart.Empty() || history->date < periodStart)
			periodStart = history->date;

		if (history->date > periodFinal)
			periodFinal = history->date;

		office.Emit(tr1start, tr1final, TJurorHistory::fields, history);
	}
}

void TFloatJurorRedistribQuery::Count()
{
	TQuery q(criteria);
	TJurorFloatHistory history(this);

	periodStart = group->minDate;
	periodFinal = group->maxDate;

	while (q.Read())
	{
		history << q;
		history.Execute();
	}

	if (periodFinal.Empty())
		periodFinal = Today;
}

const offield TFloatJurorRedistribQuery::header_fields[] =
{
	{ "F_PERIOD",	jurorquery_period },
	{ NULL, NULL }
};

static void float_juror_redistrib(TWindow *parent, const char *criteria, TFloatJurorGroup *group)
{
	TFloatJurorRedistribQuery(parent, criteria, group).Execute();
}

#include "FloatJurorDialog.h"
#include "FloatJurorDialog.cpp"

void float_juror(TWindow *parent)
{
	TFloatJurorGroup group;
	TFloatJurorDialog(parent, &group, COURT_FLAT ? IDD_FLOAT_JUROR_FLAT : IDD_FLOAT_JUROR).Execute();
}
#endif  // AREAL || MILITARY || REGIONAL
