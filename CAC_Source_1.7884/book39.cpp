#include "cac.h"

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
// ----- TBook39Group ------------------------------------------------------
class TBook39Group : public TFloatGroup
{
public:
	TBook39Group();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	TGroupArray judges;
	char compositions[SIZE_OF_COMPOSITIONS];
	TGroupArray receivers;
	TGroupArray slcReasons;
	CDate minOutRegDate;
	CDate maxOutRegDate;
	long eventText;
};

TBook39Group::TBook39Group()
	: TFloatGroup("T_BOOK_39")
{
	Add(
		+ new TCharList("F_TYPES", types, sizeof types),
		+ new TLong("F_MIN_NO", &minNo),
		+ new TLong("F_MAX_NO", &maxNo),
		+ new TLong("F_MIN_YEAR", &minYear),
		+ new TLong("F_MAX_YEAR", &maxYear),
		+ new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		+ new TDate("F_MIN_OUTREG_DATE", &minOutRegDate, false),
		+ new TDate("F_MAX_OUTREG_DATE", &maxOutRegDate, false),
		+ new TCheck("F_EVENT_TEXT", &eventText, TRUE),
		NULL
	);

	strcpy(types, Default->colleges);
	strcach(types, TYPE_REQUEST);
	order = ORDER_BY_TYN;
}

void TBook39Group::FormCriteria(msql &m)
{
	bool lawsuits = strcmp(types, C2S[TYPE_REQUEST]);
	mstr tables(lawsuits ? "T_LAWSUIT T" : "T_REQUEST T");
	mstr fields("T.*, O.F_DECISION_KIND, O.F_DECISION_DATE AS F_SESSION_DATE, O.F_DATE AS F_RETURNED_DATE, "
		"O.F_NO AS F_OUTREG_NO, O.F_DATE AS F_OUTREG_DATE, O.F_KIND AS F_OUTREG_KIND, E.F_EVENT_NO");

	tables.cat(" JOIN T_OUTREG O ON O.F_LAWSUIT_NO = T.F_NO AND O.F_LAWSUIT_YEAR = T.F_YEAR"
		" AND O.F_LAWSUIT_TYPE = T.F_TYPE");

	if (lawsuits)
	{
		tables.cat(" JOIN T_DECISION E ON E.F_NO = T.F_NO AND E.F_YEAR = T.F_YEAR AND E.F_TYPE = T.F_TYPE"
			" AND E.F_KIND = O.F_DECISION_KIND AND E.F_DATE = O.F_DECISION_DATE");
		fields.cat(", E.F_SLC_REASON, E.F_SESSION_KIND, E.F_RESULT, E.F_FROM_DATE AS F_DECISION_DATE");
	}
	else
	{
		tables.cat(" JOIN T_REQUEST_RESOLUTION E ON E.F_NO = T.F_NO AND E.F_YEAR = T.F_YEAR"
			" AND E.F_KIND = O.F_DECISION_KIND AND E.F_DATE = O.F_DECISION_DATE");
		fields.cat(", E.F_JUDGE, E.F_DATE AS F_DECISION_DATE");
	}

	if (eventText)
		fields.cat(", E.F_TEXT AS F_EVENT_TEXT");

	m.Begin(str(tables), str(fields));

	m.AddRange("T.F_NO", minNo, maxNo);
	m.AddRange("T.F_YEAR", minYear, maxYear);
	m.AddPairs("F_UCN", "F_UCN_TYPE", receivers, "O.F_RECEIVER", "O.F_RECEIVER_TYPE");
	m.AddRange("O.F_DATE", minOutRegDate, maxOutRegDate);

	if (lawsuits)
	{
		m.AddChars("T.F_TYPE", types, TYPE_LAWSUITS);
		m.AddArray("F_UCN", judges, "T.F_JUDGE");
		m.AddCompos("T.F_COMPOSITION", compositions);

		if (slcReasons.Count())
			m.AddArray("E.F_SLC_REASON", slcReasons);
		else
			m.AddIsNull("E.F_SLC_REASON", false);
	}
	else
	{
		m.AddArray("F_UCN", judges, "E.F_JUDGE");
		m.AddChar("E.F_RESOLUTION", RESOLUTION_REJECTED);
	}

	m.AddOrder("F_TYPE, F_YEAR, F_NO", "T.");
}

#define header OFFICE_GROUP_CAST(TBook39Group)

static void header_types(mstr &m, TGroup *group) { print_types_list(m, header->types, false); }

static void header_period(mstr &m, TGroup *group)
{
	if (header->maxOutRegDate.Empty())
		header->maxOutRegDate = Today;

	m.printf("%D - %D", &header->minOutRegDate, &header->maxOutRegDate);
}

#undef header

static const offield header_fields[] =
{
	{ "F_TYPES",	header_types },
	{ "F_PERIOD",	header_period },
	{ NULL, NULL }
};

// ----- TLawsuit39 ----------------------------------------------------------
class TLawsuit39 : public TLawsuit
{
public:
	TLawsuit39();
	virtual ~TLawsuit39() { delete next; }

	void MakeCriteria(msql &m, bool request);

	char decisionKind;
	CDate sessionDate;
	CDate decisionDate;
	char sessionKind;
	char result;
	long slcReason;
	long outRegNo;
	char outRegKind;
	CDate outRegDate;
	long eventNo;
	TOriginContainer *next;
	mstr eventText;
};

TLawsuit39::TLawsuit39()
	: next(NULL)
{
	Add(
		new TCharAlias("F_DECISION_KIND", &decisionKind),
		new TDate("F_SESSION_DATE", &sessionDate, false),
		new TDate("F_DECISION_DATE", &decisionDate, false),
		new TCharAlias("F_SESSION_KIND", &sessionKind),
		new TCharAlias("F_RESULT", &result),
		new TLongAlias("F_SLC_REASON", &slcReason),
		new TLong("F_OUTREG_NO", &outRegNo),
		new TDate("F_OUTREG_DATE", &outRegDate, false),
		new TLong("F_EVENT_NO", &eventNo),
		new TText("F_EVENT_TEXT", &eventText, SIZE_OF_DECISION_TEXT),
		NULL
	);
}

void TLawsuit39::MakeCriteria(msql &m, bool request)
{
	mstr tables;
	mstr fields("T.F_NO, T.F_YEAR, T.F_TYPE, T.F_KIND, ");

	tables.printf("T_CONNECT C JOIN %s T ON T.F_NO = C.F_NO AND T.F_YEAR = C.F_YEAR AND T.F_TYPE = C.F_TYPE",
		request ? "T_REQUEST" : "T_LAWSUIT");
	fields.cat(request ? "T.F_DATE" : "T.F_PREVIOUS_DATE, T.F_JUDGE, T.F_SOURCE_NO, T.F_SOURCE_YEAR, T.F_SOURCE_TYPE");
	m.Begin(str(tables), str(fields));

	m.Add("C.F_CONNECT_NO");
	m.printf(" = '%ld'", key.no);
	m.AddLong("C.F_CONNECT_YEAR", key.year);
	m.AddChar("C.F_CONNECT_TYPE", CONNECT_TYPE_AFTER_ADMINISTER);
	m.AddChar("C.F_CONNECT_KIND", type2ConnectKind(key.type));
	m.AddChar(request ? "C.F_TYPE" : "NOT C.F_TYPE", TYPE_REQUEST);
	m.Add("T.F_DATE");
	m.printf("> %ld", &sessionDate);
	m.AddOrder("T.F_DATE");
}

#define lawsuit OFFICE_GROUP_CAST(TLawsuit39)

static void lawsuit_indoc_description(mstr &m, TGroup *group)
{
	if (strchr(KIND_LAWSUITS, lawsuit->kind) && lawsuit->source.type == TYPE_REQUEST)
	{
		TRequest request;
		request.key = lawsuit->source;

		if (request.Get("F_KIND, F_DATE"))
			m.printf("%s %ld/%Dã.", Kind->Seek(request.kind), request.key.no, &request.date);
		else
			m.printf("%s %ld/%ldã.", Type->Seek(request.key.type), request.key.no, request.key.year);
	}

	print_connects(m, lawsuit->key, false, true, "ñ-â");
}

static void lawsuit_kind_alias(mstr &m, TGroup *group)
{
	m.cat(lawsuit->key.type == TYPE_REQUEST ? Kind->Seek(lawsuit->kind) : KindAlias->Seek(lawsuit->kind));
}

static void lawsuit_composition(mstr &m, TGroup *group) { print_composition(m, lawsuit->composition, "ñ-â"); }
static void lawsuit_judge(mstr &m, TGroup *group) { print_user(m, lawsuit->judge, UCN_CITIZEN_UCN, true, -1); }
static void lawsuit_decision_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(lawsuit->decisionKind)); }
static void lawsuit_decision_no(mstr &m, TGroup *group) { itom(lawsuit->eventNo, m); }
static void lawsuit_decision_date(mstr &m, TGroup *group) { m.printf("%Dã.", &lawsuit->decisionDate); }

static void lawsuit_slc_reason(mstr &m, TGroup *group)
{
	if (lawsuit->slcReason)
		m.cat(SLCReason->Seek(lawsuit->slcReason));
}

static void lawsuit_return(mstr &m, TGroup *group)
{
	m.printf("Èçõ ¹ %ld/%Dã.", lawsuit->outRegNo, &lawsuit->outRegDate);
}

static void lawsuit_user_post(mstr &m, TGroup *group)
{
	TSubpoena subpoena;
	msql w(subpoena.Name, "F_USER_UCN");

	subpoena.key.no = lawsuit->outRegNo;
	subpoena.key.year = lawsuit->outRegDate.year;
	subpoena.key.type = TYPE_OUTREG;
	subpoena.kind = lawsuit->outRegKind;
	subpoena.date = lawsuit->outRegDate;

	w.Add(&subpoena, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");
	w.AddLong("F_SUBPOENA_NO", 1);

	TQuery q(str(w));
	if (q.Read())
	{
		subpoena << q;
		User->Seek(subpoena.userUcn);
		m.printf("%s ", Post->Seek(User->post));
		print_user(m, subpoena.userUcn, UCN_CITIZEN_UCN, true, -1);
	}
}

static void lawsuit_indoc_description_new(mstr &m, TGroup *group)
{
	delete lawsuit->next;
	lawsuit->next = NULL;

	// 2015:168 TRQ/IRQ: comout
	//if (lawsuit->key.type != TYPE_REQUEST)
	{
		TRequest tRequest;
		TLawsuit tLawsuit;
		TOriginContainer *tNext = NULL;
		msql w;

		lawsuit->MakeCriteria(w, true);
		TQuery q(str(w));
		if (q.Read())
			tRequest << q;

		lawsuit->MakeCriteria(w, false);
		TQuery r(str(w));
		if (r.Read())
			tLawsuit << r;

		if (!tRequest.key.no)
		{
			if (tLawsuit.key.no)
				tNext = &tLawsuit;
		}
		else if (!tLawsuit.key.no)
			tNext = &tRequest;
		else if (tLawsuit.date < tRequest.date || tLawsuit.source == tRequest.key)
			tNext = &tLawsuit;
		else
			tNext =  &tRequest;

		if (tNext)
		{
			lawsuit->next = (TOriginContainer *) type2RCDKeyContainer(tNext->key.type);
			*lawsuit->next << tNext;

			if (tNext->key.type != TYPE_REQUEST)
				m.printf("%s %ld/%Dã.", Kind->Seek(tRequest.kind), tRequest.key.no, &tRequest.date);
		}
	}
}

static void lawsuit_lawsuit_new(mstr &m, TGroup *group)
{
	if (lawsuit->next)
		m.printf("%s %ld/%ld", KindAlias->Seek(lawsuit->next->kind), lawsuit->next->key.no, lawsuit->next->key.year);
}

static void lawsuit_date_new(mstr &m, TGroup *group)
{
	if (lawsuit->next)
	{
		if (lawsuit->next->key.type == TYPE_REQUEST)
			m.printf(", %D", &lawsuit->next->date);
		else
		{
			TLawsuit *tLawsuit = (TLawsuit *) lawsuit->next;

			if (tLawsuit->source.type == TYPE_REQUEST)
			{
				TRequest request;

				request.key = tLawsuit->source;
				if (request.Get("F_DATE"))
					m.printf(", %D", &request.date);
			}
			else
				m.printf(", %D", &tLawsuit->receivedDate);
		}
	}
}

static void lawsuit_judge_new(mstr &m, TGroup *group)
{
	if (lawsuit->next && lawsuit->next->key.type != TYPE_REQUEST)
		print_user(m, ((TLawsuit *) lawsuit->next)->judge, UCN_CITIZEN_UCN, true, -1);
}

#undef lawsuit

static const offield book_fields[] =
{
	{ "F_INDOC_DESCRIPTION",	lawsuit_indoc_description },
	{ "F_NO",				NULL },
	{ "F_YEAR",				NULL },
	{ "F_QUICK_SHORT",		origin_quick_short },
	{ "F_KIND",				lawsuit_kind_alias },
	{ "F_COMPOSITION",		lawsuit_composition },
	{ "F_JUDGE",			lawsuit_judge },
	{ "F_DECISION_KIND",		lawsuit_decision_kind },
	{ "F_DECISION_NO",		lawsuit_decision_no },
	{ "F_DECISION_DATE",		lawsuit_decision_date },
	{ "F_SLC_REASON",			lawsuit_slc_reason },
	{ "F_EVENT_TEXT",			NULL },
	{ "F_LAWSUIT_RETURN",		lawsuit_return },
	{ "F_USER_POST",			lawsuit_user_post },
	{ "F_INDOC_DESCRIPTION_NEW",	lawsuit_indoc_description_new },
	{ "F_LAWSUIT_NEW",		lawsuit_lawsuit_new },
	{ "F_DATE_NEW",			lawsuit_date_new },
	{ "F_JUDGE_NEW",			lawsuit_judge_new },
	{ NULL, NULL }
};

// ----- TBook39 -------------------------------------------------------------
class TBook39 : public TFloatQuery
{
public:
	TBook39(TWindow *parent, TBook39Group *tGroup);

	void Execute();

protected:
	TBook39Group *group;

	virtual void Count() { }
};

TBook39::TBook39(TWindow *parent, TBook39Group *tGroup)
	: TFloatQuery(parent, criteria, "Book39.htm", NULL), group(tGroup)
{
	fdwhat = "êíèãàòà";
}

void TBook39::Execute()
{
	if (office.Open(name, OFFICE_COPY_HEADER))
	{
		office.Emit(NULL, NULL, header_fields, group);
		office.Close();

		if (office.Open(name, OFFICE_SKIP_HEADER, ""))
		{
			char types[SIZE_OF_TYPES];
			strcpy(types, group->types);

			TLawsuit39 lawsuit;
			msql m;

			GetMarkers();
			office.Emit(NULL, tr1start, null_fields, NULL);

			// pass 1: requests
			if (strchr(types, TYPE_REQUEST))
			{
				strcpy(group->types, C2S[TYPE_REQUEST]);
				group->FormCriteria(m);
				TQuery q(str(m));

				while (q.Read())
				{
					lawsuit << q;
					TLawsuit tLawsuit;

					if (!tLawsuit.TryBy(lawsuit.key, "F_NO"))
						office.Emit(tr1start, tr1final, book_fields, &lawsuit);

					Increment();
				}
			}

			// pass 2: lawsuits
			if (strcmp(types, C2S[TYPE_REQUEST]))
			{
				strcpy(group->types, types);
				stremove(group->types, TYPE_REQUEST);
				group->FormCriteria(m);
				TQuery q(str(m));

				while (q.Read())
				{
					lawsuit << q;
					Increment();
					// 2015:083 LPR: slcReason is required -> removed sesion cancel check
					office.Emit(tr1start, tr1final, book_fields, &lawsuit);
				}
			}

			office.Emit(tr1final, NULL, null_fields, NULL);
			office.Close();
			office.Show(wait);
		}
	}
}

// ----- functions -----------------------------------------------------------
#include "Book39Dialog.h"
#include "Book39Dialog.cpp"

void book_39(TWindow *parent)
{
	TBook39Group group;

	if (TBook39Dialog(parent, &group).Execute() == IDOK)
		TBook39(parent, &group).Execute();
}
#endif  // APPEAL || ADMIN || AREAL
