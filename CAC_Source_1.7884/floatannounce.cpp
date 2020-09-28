#include "cac.h"

// ----- TFloatAnnounceGroup -----------------------------------------------
class TFloatAnnounceGroup : public TFloatGroup
{
public:
	TFloatAnnounceGroup();

	virtual void FormCriteria(msql &m);

	char kinds[SIZE_OF_CONNECT_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	TGroupArray senders;
	CDate minSessionDate;
	CDate maxSessionDate;
	CDate minSubpoenaDate;
	CDate maxSubpoenaDate;
	char subpoenaKinds[SIZE_OF_SUBPOENA_KINDS];
	char receivedStatuses[SIZE_OF_RECEIVED_STATUSES];
	TGroupArray deliverers;
	CDate minReturnedDate;
	CDate maxReturnedDate;
};

TFloatAnnounceGroup::TFloatAnnounceGroup()
	: TFloatGroup("T_ANNOUNCE")
{
	Add(
		+ new TCharList("F_KINDS", kinds, sizeof kinds),
		+ new TLong("F_MIN_NO", &minNo),
		+ new TLong("F_MAX_NO", &maxNo),
		+ new TLong("F_MIN_YEAR", &minYear),
		+ new TLong("F_MAX_YEAR", &maxYear),
		+ new TDate("F_MIN_SESSION_DATE", &minSessionDate, false),
		+ new TDate("F_MAX_SESSION_DATE", &maxSessionDate, false),
		+ new TDate("F_MIN_SUBPOENA_DATE", &minSubpoenaDate, false),
		+ new TDate("F_MAX_SUBPOENA_DATE", &maxSubpoenaDate, false),
		+ new TCharList("F_SUBPOENA_KINDS", subpoenaKinds, sizeof subpoenaKinds),
		+ new TCharList("F_RECEIVED_STATUSES", receivedStatuses, sizeof receivedStatuses),
		+ new TDate("F_MIN_RETURNED_DATE", &minReturnedDate, false),
		+ new TDate("F_MAX_RETURNED_DATE", &maxReturnedDate, false),
		+ new TRadio("F_BY_SDKYN", &order, ORDER_BY_SDKYN),
		+ new TRadio("F_BY_DKYN", &order, ORDER_BY_DKYN),
		+ new TRadio("F_BY_NMKYN", &order, ORDER_BY_NMKYN),
		NULL
	);

	order = ORDER_BY_DKYN;
}

void TFloatAnnounceGroup::FormCriteria(msql &m)
{
	mstr tables;
	tables.printf("T_ANNOUNCE A LEFT JOIN T_CITIZEN C ON C.F_UCN = A.F_DELIVERER_UCN AND C.F_UCN_TYPE = '%c'",
		UCN_CITIZEN_UCN);

	m.Begin(str(tables), "A.*");

	m.AddChars("A.F_KIND", kinds, CONNECT_KIND_ANNOUNCES);
	m.AddRange("A.F_NO", minNo, maxNo);
	m.AddRange("A.F_YEAR", minYear, maxYear);
	if (senders.Count())
		m.AddArray("A.F_COURT", senders);
	else
		m.AddLong("NOT A.F_COURT", COURT_CODE);
	m.AddRange("A.F_SESSION_DATE", minSessionDate, maxSessionDate);
	m.AddRange("A.F_SUBPOENA_DATE", minSubpoenaDate, maxSubpoenaDate);
	m.AddChars("A.F_SUBPOENA_KIND", subpoenaKinds, SUBPOENA_KIND_GENERICS);
	m.AddChars("A.F_RECEIVED_STATUS", receivedStatuses, SRS_ANNOUNCES);
	m.AddRange("A.F_SUBPOENA_RETURNED", minReturnedDate, maxReturnedDate);
	m.AddArray("F_UCN", deliverers, "A.F_DELIVERER_UCN");

	const char *sOrder;

	switch (order)
	{
		case ORDER_BY_SDTYN :
		case ORDER_BY_SDKYN : sOrder = "A.F_SUBPOENA_DATE, A.F_KIND, A.F_YEAR, A.F_NO"; break;
		case ORDER_BY_DKYN :
		case ORDER_BY_DTYN : sOrder = "A.F_SESSION_DATE, A.F_KIND, A.F_YEAR, A.F_NO"; break;
		case ORDER_BY_TYN : sOrder = "A.F_KIND, A.F_YEAR, A.F_NO"; break;
		default : sOrder = "C.F_NAME, C.F_FAMILY, C.F_RENAME, C.F_REFAMILY, A.F_KIND, A.F_YEAR, A.F_NO";
	}

	m.AddOrder(sOrder);
}

// ----- query ---------------------------------------------------------------
#if 20180176
typedef TAnnounce TFloatAnnounce;
#else  // 20180176
class TFloatAnnounce : public TAnnounce, public TCitizenNames
{
public:
	TFloatAnnounce() : TCitizenNames(this) { }
};
#endif  // 20180176

#define announce OFFICE_GROUP_CAST(TFloatAnnounce)

static void floatano_key_kind(mstr &m, TGroup *group) { m.cpy(ConnectKind->Seek(announce->key.kind)); }
static void floatano_court(mstr &m, TGroup *group) { announce->GetSenderName(m); }
static void floatano_subpoena_kind(mstr &m, TGroup *group) { m.cpy(SubpoenaKind->Seek(announce->subpoenaKind)); }

static void floatano_deliverer(mstr &m, TGroup *group)
{
#if 20180176
	if (atob(announce->delivererUcn))
		print_user(m, announce->delivererUcn);
#else  // 20180176
	if (any(announce->name))
		print_user(m, announce);
#endif  // 20180176
}

static void floatano_status(mstr &m, TGroup *group)
{
	print_received(m, announce->receivedStatus, announce->subpoenaReceived, announce->receivedTime, announce->coordN,
		announce->coordE);
	m.sep_cat("\v", announce->receivedText);
}

static void floatano_address(mstr &m, TGroup *group)
{
	if (announce->uclp)
	{
		TAddress address;

		address << announce;
		address.Print(m, ADDRESS_SKIP_LN1);
		TAnnounceVisit().PrintVisits(m, announce);
	}
}

#undef announce

const offield floatano_fields[]=
{
	{ "F_KIND",			floatano_key_kind },
	{ "F_NO",			NULL },
	{ "F_YEAR",			NULL },
	{ "F_COURT",		floatano_court },
	{ "F_SUBPOENA_KIND",	floatano_subpoena_kind },
	{ "F_SUBPOENA_DATE",	NULL },
	{ "F_DELIVERER",		floatano_deliverer },
	{ "F_STATUS",		floatano_status },
	{ "F_ADDRESS",		floatano_address },
	{ NULL, NULL }
};

// ----- TFloatAnnounceQuery -----------------------------------------------
class TFloatAnnounceQuery : public TFloatQuery
{
public:
	TFloatAnnounceQuery(TWindow *parent, const char *criteria);

protected:
	virtual void Count();
};

TFloatAnnounceQuery::TFloatAnnounceQuery(TWindow *parent, const char *criteria)
	: TFloatQuery(parent, criteria, "FloatAno.htm")
{
	maximum = LONG_MAX;
}

void TFloatAnnounceQuery::Count()
{
	TQuery q(criteria);
	TFloatAnnounce announce;

	while (q.Read() && Increment())
	{
		announce << q;
		office.Emit(tr1start, tr1final, floatano_fields, &announce);
	}
}

// ----- functions ---------------------------------------------------------
static void query_announce(TWindow *parent, const char *criteria)
{
	TFloatAnnounceQuery(parent, criteria).Execute();
}

#include "FloatAnnounceDialog.h"
#include "FloatAnnounceDialog.cpp"

void float_announce(TWindow *parent)
{
	TFloatAnnounceGroup group;
	TFloatAnnounceDialog(parent, &group).Execute();
}

// ---- float announce-subpoena ----------------------------------------------
class TFloatAnnounceSubpoenaGroup : public TFloatAnnounceGroup
{
public:
	TFloatAnnounceSubpoenaGroup(const TGroup *group, TGroupArray &tDeliverers);

	char types[SIZE_OF_TYPES];
};

TFloatAnnounceSubpoenaGroup::TFloatAnnounceSubpoenaGroup(const TGroup *group, TGroupArray &tDeliverers)
{
	Add(new TCharList("F_TYPES", types, sizeof types));

	*this << group;
	*kinds = '\0';

	for (const char *s = types; *s; s++)
	{
		// 2018-07-20: origins -> annotypes
		if (strchr(CONNECT_KIND_ANNOTYPES, *s))
			strcach(kinds, type2ConnectKind(*s));
		// 2018-07-20
		if (*s == TYPE_EXECUTIVE_LAW)
			strcach(kinds, CONNECT_KIND_PRIVATE_EXECUTOR);
	}

	TSender *sender = new TSender;
	sender->court = COURT_CODE;
	senders.Add(sender);

	for (int i = 0; i < tDeliverers.Count(); i++)
		deliverers.AddCopy(tDeliverers[i]);
}

void load_announces_criteria(msql &m, const TGroup *group, TGroupArray &deliverers)
{
	TFloatAnnounceSubpoenaGroup(group, deliverers).FormCriteria(m);
}

void load_announces(const TGroup *group, TGroupArray &announces, TGroupArray &deliverers)
{
	msql m;
	load_announces_criteria(m, group, deliverers);

	TQuery q(str(m));
	while (q.Read())
	{
		TFloatAnnounce *announce = new TFloatAnnounce;
		*announce << q;
		announces.Add(announce);
	}
}
