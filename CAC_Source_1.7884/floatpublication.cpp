#include "cac.h"

class TFloatPublicationGroup : public TFloatGroup
{
public:
	TFloatPublicationGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	CDate minDate;
	CDate maxDate;
	char fileName[SIZE_OF_OFFICE_TARGET];
};

TFloatPublicationGroup::TFloatPublicationGroup()
	: TFloatGroup("T_SUBPOENA S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TRadio("F_BY_TNY", &order, TRUE),
		new TRadio("F_SES_DATE", &order, FALSE),
		new TString("F_FILE_NAME", fileName, sizeof fileName),
		NULL
	);

	order = TRUE;
}

void TFloatPublicationGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "S.F_NO, S.F_YEAR, S.F_TYPE, L.F_KIND, S.F_INVOLVEMENT, S.F_UCN, S.F_UCN_TYPE, "
		"L.F_SUBJECT AS F_LAW_SUBJECT, L.F_TEXT AS F_LAW_TEXT, S.F_DATE AS F_SES_DATE, S.F_KIND AS F_SES_KIND");

	m.AddChars("S.F_TYPE", types, COLLEGE_ALLS);
	m.AddRange("S.F_DATE", minDate, maxDate);
	m.AddChar("S.F_SUBPOENA_KIND", SUBPOENA_KIND_STATE_PAPER);
	m.AddOrder(order ? "S.F_TYPE, S.F_NO, S.F_YEAR" : "S.F_DATE");
}

class TPublicationSubpoena : public TSubpoena
{
public:
	TPublicationSubpoena();

	long lawSubject;
	char lawText[SIZE_OF_ORIGIN_TEXT];
	char sesKind;
	CDate sesDate;
	CTime sesTime;
};

TPublicationSubpoena::TPublicationSubpoena()
{
	Add(
		new TLong("F_LAW_SUBJECT", &lawSubject),
		new TString("F_LAW_TEXT", lawText, sizeof lawText),
		new TDate("F_SES_DATE", &sesDate, false),
		new TChar("F_SES_KIND", &sesKind),
		NULL
	);
}

#define subpoena OFFICE_GROUP_CAST(TPublicationSubpoena)

static void publication_title(mstr &m, TGroup *) { m.cpy("Справка публикации в ДВ"); }
static void publication_key(mstr &m, TGroup *group) { m.printf("%ld/%ld", subpoena->key.no, subpoena->key.year); }
static void publication_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(subpoena->kind)); }
static void publication_subject(mstr &m, TGroup *group) { m.cpy(Subject->Seek(subpoena->key.type, subpoena->lawSubject)); }
static void publication_remark(mstr &m, TGroup *group) { m.cpy(subpoena->lawText); }
static void publication_involvement(mstr &m, TGroup *group) { m.cpy(Involvement->Seek(subpoena->involvement)); }

static void publication_receiver_address(mstr &m, TGroup *group)
{
	print_address(m, subpoena, 1, ADDRESS_DISTINCT);
}

static void publication_date_time(mstr &m, TGroup *group)
{
	subpoena->sesDate.Print(m);
	subpoena->sesTime.Print(m, "\v%T");
}

#undef subpoena

static const offield publication_fields[] =
{
	{ "F_TITLE",		publication_title },
	{ "F_KEY",			publication_key },
	{ "F_KIND",			publication_kind },
	{ "F_SUBJECT",		publication_subject },
	{ "F_REMARK",		publication_remark },
	{ "F_INVOLVEMENT",	publication_involvement },
	{ "F_RECEIVER",		subpoena_receiver },
	{ "F_RECEIVER_ADDRESS",	publication_receiver_address },
	{ "F_DATE_TIME",		publication_date_time },
	{ NULL,		NULL }
};

class TPrintPublicate : public TOffice
{
public:
	TPrintPublicate(TWindow *tParent, TFloatPublicationGroup *tGroup);

	void Execute();

protected:
	TWindow *parent;
	TFloatPublicationGroup *group;

	const char *tr1start;
	const char *tr1final;
};

TPrintPublicate::TPrintPublicate(TWindow *tParent, TFloatPublicationGroup *tGroup)
	: parent(tParent), group(tGroup)
{
}

void TPrintPublicate::Execute()
{
	TWaitWindow wait(parent, WAIT);
	TCounterGroup counter("Няма публикации", "Една публикация", "публикации");

	if (Open("LstPubl.htm", OFFICE_COPY_ALL, any(group->fileName) ? group->fileName : NULL))
	{
		TSession session;
		tr1start = Find("tr1 start");
		tr1final = Find("tr1 final");

		TPublicationSubpoena subpoena;
		msql m;

		group->FormCriteria(m);
		Emit(NULL, tr1start, publication_fields, NULL);

		TQuery q(str(m));
		while (q.Read())
		{
			subpoena << q;

			session.key = subpoena.key;
			session.date = subpoena.sesDate;
			session.kind = subpoena.sesKind;

			if (session.Try("F_TIME"))
				subpoena.sesTime = session.time;

			Emit(tr1start, tr1final, publication_fields, &subpoena);
			counter.count++;
		}

		Emit(tr1final, NULL, counter_fields, &counter);
		Show(&wait);
	}
}


#include "FloatPublicationDialog.h"
#include "FloatPublicationDialog.cpp"

void float_publication(TWindow *parent)
{
	TFloatPublicationGroup group;

	if (TFloatPublicationDialog(parent, &group).Execute() == IDOK)
		TPrintPublicate(parent, &group).Execute();
}
