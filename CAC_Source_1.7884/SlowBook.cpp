#include "cac.h"

#if COURT_TYPE == COURT_AREAL
class TSlowBookGroup : public TBookGroup
{
public:
	TSlowBookGroup();

	virtual void FormCriteria(msql &m);
	virtual void Title(mstr &m);
};

TSlowBookGroup::TSlowBookGroup()
	: TBookGroup("T_LAWSUIT L JOIN T_SESSION S ON S.F_NO = L.F_NO AND S.F_YEAR = L.F_YEAR AND S.F_TYPE = L.F_TYPE ",
		"L.F_TYPE", "L.F_DATE", false)
{
	*types = '\0';
}

void TSlowBookGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "L.F_NO, L.F_YEAR, L.F_TYPE, S.F_KIND, S.F_DATE, S.F_JUDGE, L.F_SOURCE_NO, L.F_SOURCE_YEAR, "
		"L.F_SOURCE_TYPE, L.F_SOURCE_KIND, L.F_DATE AS F_LAW_DATE, S.F_TEXT AS F_SES_TEXT, S.F_RESULT AS F_SES_RESULT, "
		"S.F_RETURNED AS F_SES_RETURNED, L.F_KIND AS F_LAW_KIND");

	m.AddChars("L.F_TYPE", types, COLLEGE_ALLS);
	m.AddChars("L.F_SOURCE_KIND", KIND_SLOW_REQUESTS);
	m.AddRange("S.F_DATE", minDate, maxDate);
	m.AddOrder("S.F_DATE");
}

void TSlowBookGroup::Title(mstr &m) { book_dates(m, this); }

class TSlowLawsuit : public TLawsuit
{
public:
	TSlowLawsuit();

	char lawKind;
	char sesResult;
	CDate lawDate;
	CDate sesReturned;
	mstr sesText;
	TGArray<TDecision> decisions;
};

TSlowLawsuit::TSlowLawsuit()
{
	Add(
		new TDate("F_LAW_DATE", &lawDate, true),
		new TChar("F_LAW_KIND", &lawKind),
		new TDate("F_SES_RETURNED", &sesReturned, true),
		new TChar("F_SES_RESULT", &sesResult),
		new TText("F_SES_TEXT", &sesText, SIZE_OF_SESSION_TEXT),
		NULL
	);
}

#define lawsuit OFFICE_GROUP_CAST(TSlowLawsuit)

static void slow_key(mstr &m, TGroup *group)
{
	m.printf("%s\v%d/%d", Kind->Seek(lawsuit->lawKind), flag_key_no(&lawsuit->key), lawsuit->key.year);
}

static void slow_law_date(mstr &m, TGroup *group) { lawsuit->lawDate.Print(m); }

static void slow_connect_key(mstr &m, TGroup *group)
{
	TGArray<TConnect> connects;

	if (load_connects(lawsuit, &connects))
	{
		TSender sender;

		for (int i = 0; i < connects.Count(); i++)
		{
			TConnect *connect = connects[i];

			sender.ucnType = connect->ucnType;
			strcpy(sender.ucn, connect->ucn);

			if (!sender.Get())
				strcpy(sender.name, sender.ucn);

			if (connect->connectKind)
			{
				ConnectKind->Seek(connect->connectKind);
				m.cpy(ConnectKind->name);
			}

			m.printf(" %s/%ldг. ", connect->connectNo, connect->connectYear);

			if (connect->composition)
				m.printf("%s състав ", connect->CompositionName());

			m.cat(sender.name);
		}
	}
}

static void slow_request_date(mstr &m, TGroup *group)
{
	if (lawsuit->source.no && lawsuit->source.year)
	{
		TRequest request;

		request.key = lawsuit->source;

		if (request.Try("F_DATE"))
			request.date.Print(m);
	}
}

static void slow_appealer(mstr &m, TGroup *group)
{
	TLinkWind linkWind;
	TGArray<TSideWind> sideWinds;

	char *categories = load_sidebasic(lawsuit, sideWinds);

	for (int i = 0; i < sideWinds.Count(); i++)
	{
		TSideWind *sideWind = sideWinds[i];

		if (strchr(INVOLVEMENT_COMPLAINTLODGERS, sideWind->involvement))
		{
			msql w;
			bool links = false;

			sideWind->key = lawsuit->key;
			sideWind->kind = lawsuit->kind;
			sideWind->date = lawsuit->date;
			linkWind.SetOrdinary(sideWind);
			linkWind.FloatCriteriaByOrdinary(w);

			TQuery q(str(w));
			while (q.Read())
			{
				linkWind << q;

				m.cat(" ");
				print_side_links(m, &linkWind);
				links = true;
			}

			if (!links)
				add_side_name(m, sideWind);
		}
	}

	delete[] categories;
}

static void slow_decision_date(mstr &m, TGroup *group)
{
	for (int i = 0; i < lawsuit->decisions.Count(); i++)
		lawsuit->decisions[i]->date.Print(m, " %D" + !i);

	if (!lawsuit->decisions.Count())
		lawsuit->date.Print(m);
}

static void slow_judge(mstr &m, TGroup *group) { print_user(m, lawsuit->judge); }

static void slow_decision(mstr &m, TGroup *group)
{
	for (int i = 0; i < lawsuit->decisions.Count(); i++)
	{
		m.cat(Kind->Seek(lawsuit->decisions[i]->kind));

		if (lawsuit->decisions[i]->eventNo)
			m.printf(" № %d\v", lawsuit->decisions[i]->eventNo);
		else
			m.cat("\v");

		m.printf("%s\v", str(lawsuit->decisions[i]->text));
	}

	if (!any(m))
		m.printf("%s\v%s", Result->Seek(lawsuit->sesResult), str(lawsuit->sesText));
}

static void slow_returned(mstr &m, TGroup *group)
{
	for (int i = 0; i < lawsuit->decisions.Count(); i++)
		lawsuit->decisions[i]->returned.Print(m, " %D" + !i);

	if (!lawsuit->decisions.Count())
		lawsuit->sesReturned.Print(m);
}

#undef lawsuit

static const offield slow_fields[] =
{
	{ "F_KEY", slow_key },
	{ "F_LAW_DATE", slow_law_date },
	{ "F_CONNECT_KEY", slow_connect_key },
	{ "F_REQUEST_DATE", slow_request_date },
	{ "F_APPEALER", slow_appealer },
	{ "F_DECISION_DATE", slow_decision_date },
	{ "F_JUDGE", slow_judge },
	{ "F_DECISION", slow_decision },
	{ "F_RETURNED", slow_returned },
	{ NULL, NULL }
};

class TSlowBook : public TBook
{
public:
	TSlowBook(TWindow *parent, TBookGroup *group);

protected:
	virtual TGroup *Process(TQuery &q);
};

TSlowBook::TSlowBook(TWindow *parent, TBookGroup *group)
	: TBook(parent, group, "SlowBk.htm", slow_fields, ::book_fields, BOOK_FIXED_DATES)
{
}

TGroup *TSlowBook::Process(TQuery &q)
{
	TSlowLawsuit *lawsuit = new TSlowLawsuit;

	*lawsuit << q;
	try2decide((TSession*)lawsuit, &lawsuit->decisions, NULL, NULL);
	return lawsuit;
}

#include "SlowBookDialog.h"
#include "SlowBookDialog.cpp"

void slow_book(TWindow *parent)
{
	TSlowBookGroup group;

	if (TSlowBookDialog(parent, &group).Execute() == IDOK)
		TSlowBook(parent, &group).Execute(BOOK_EXEC_HEADER);
}
#endif  // AREAK
