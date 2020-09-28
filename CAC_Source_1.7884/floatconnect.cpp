#include "cac.h"

// ----- TFloatConnectGroup --------------------------------------------------
class TFloatConnectGroup : public TFloatGroup
{
public:
	TFloatConnectGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	char connectTypes[SIZE_OF_CONNECT_TYPES];
	TGroupArray senders;
	char generalKinds[SIZE_OF_KINDS];
	char connectKinds[SIZE_OF_CONNECT_KINDS];
	char connectNo[SIZE_OF_CONNECT_NO];
	long exactNo;
	CDate minDate;
	CDate maxDate;
	long minConnectYear;
	long maxConnectYear;
	CDate minFinished;
	CDate maxFinished;
	long queryActs;
	char results[SIZE_OF_RESULTS];
	char text[SIZE_OF_CONNECT_TEXT];
};

TFloatConnectGroup::TFloatConnectGroup()
	// 2014:210 FB2: nested selects -> join, see below
	: TFloatGroup("T_CONNECT C "  // 2017:097 intervals
		"LEFT JOIN T_LAW_INTERVAL I ON I.F_NO = C.F_NO AND I.F_YEAR = C.F_YEAR AND I.F_TYPE = C.F_TYPE "
		"LEFT JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE "
		"LEFT JOIN T_REQUEST R ON R.F_NO = C.F_NO AND R.F_YEAR = C.F_YEAR AND R.F_TYPE = C.F_TYPE",
		"C.*")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCharList("F_CONNECT_TYPES", connectTypes, sizeof connectTypes),
		new TCharList("F_GENERAL_KINDS", generalKinds, sizeof generalKinds),
		new TCharList("F_CONNECT_KINDS", connectKinds, sizeof connectKinds),
		new TString("F_CONNECT_NO", connectNo, sizeof connectNo),
		new TCheck("F_EXACT_NO", &exactNo, TRUE),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TLong("F_MIN_CONNECT_YEAR", &minConnectYear),
		new TLong("F_MAX_CONNECT_YEAR", &maxConnectYear),
		new TDate("F_MIN_FINISHED", &minFinished, false),
		new TDate("F_MAX_FINISHED", &maxFinished, false),
		new TCheck("F_QUERY_ACTS", &queryActs, TRUE),
		new TCharList("F_RESULTS", results, sizeof results),
		new TString("F_TEXT", text, sizeof text),
		NULL
	);

	exactNo = TRUE;
	//order = ORDER_BY_TYN;		// fixed array sorting
}

void TFloatConnectGroup::FormCriteria(msql &m)
{
	if (queryActs)
	{
		m.Begin("T_DECISION D "  // 2017:097 intervals
			"JOIN T_CONNECT C ON C.F_NO = D.F_NO AND C.F_YEAR = D.F_YEAR AND C.F_TYPE = D.F_TYPE "
			"LEFT JOIN T_LAW_INTERVAL I ON I.F_NO = D.F_NO AND I.F_YEAR = D.F_YEAR AND I.F_TYPE = D.F_TYPE "
			"LEFT JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE "
			"LEFT JOIN T_REQUEST R ON R.F_NO = D.F_NO AND R.F_YEAR = D.F_YEAR AND R.F_TYPE = D.F_TYPE",
			"D.F_KIND AS F_DEC_KIND, D.F_FROM_DATE, D.F_RESULT, C.*");
		m.AddChars("D.F_TYPE", types, TYPE_CONNECTABLES);
		m.AddRange("D.F_NO", minNo, maxNo);
		m.AddRange("D.F_YEAR", minYear, maxYear);
		m.AddRange("D.F_FROM_DATE", minFinished, maxFinished);
		m.AddChars("D.F_RESULT", results, RESULT_ENDOCS);
	}
	else
	{
		BeginCriteria(m);
		m.AddChars("C.F_TYPE", types, TYPE_CONNECTABLES);
		m.AddRange("C.F_NO", minNo, maxNo);
		m.AddRange("C.F_YEAR", minYear, maxYear);
	}

	if (any(connectTypes))
	{
		if (strchr(connectTypes, CONNECT_TYPE_JURISDICTION))
			strcach(connectTypes, CONNECT_TYPE_HIGH_JURISDICTION);

		if (strchr(connectTypes, CONNECT_TYPE_HIGHER_INSTANCE))
		{
			strcach(connectTypes, CONNECT_TYPE_COMPETENT_HIGH);
			m.AddChars("C.F_CONNECT_TYPE", connectTypes, CONNECT_TYPE_ALLS);
		}
		else
			m.AddChars("C.F_CONNECT_TYPE", connectTypes, CONNECT_TYPE_ALLS);
	}

	if (senders.Count())
	{
		m.Add("");
		m.cach('(');
		bool flag = false;
		for (int i = 0; i < senders.Count(); i++)
		{
			if (flag) m.cat(" OR ");
			flag = true;
			m.printf("(C.F_UCN = '%s' AND C.F_UCN_TYPE = '%c')",
				 ((TString *) senders[i]->Find("F_UCN"))->S(),
				 ((TChar *) senders[i]->Find("F_UCN_TYPE"))->C());
		}
		m.cach(')');
	}

	adjust_general_kinds(generalKinds);
	// 2011:136 LPR: need_crit() -> strequal()
	// 2014:210 FB2: nested select -> joined
	if (any(generalKinds) && !strequal(generalKinds, KIND_ALLDOCS))
	{
		m.AddChars("(L.F_SOURCE_KIND", generalKinds);
		m.NextOr();
		m.AddChars("R.F_KIND", generalKinds);
		m.cach(')');
	}

	m.AddChars("C.F_CONNECT_KIND", connectKinds, CONNECT_KIND_ALLS);

	if (any(connectNo))
	{
		if (exactNo)
			m.AddString("C.F_CONNECT_NO", connectNo);
		else
			m.AddLike("C.F_CONNECT_NO", connectNo, true);
	}

	// 2014:210 FB2: nested select -> joined
	if (!minDate.Empty() || !maxDate.Empty())
	{
		m.AddRange("(L.F_DATE", minDate, maxDate);
		m.NextOr();
		m.AddRange("R.F_DATE", minDate, maxDate);
		m.cach(')');
	}

	m.AddRange("C.F_CONNECT_YEAR", minConnectYear, maxConnectYear);

	if (!minFinished.Empty() || !maxFinished.Empty())
	{
		m.AddChar("(C.F_TYPE", TYPE_REQUEST);
		m.NextOr();
		m.AddRange("L.F_FINISHED", minFinished, maxFinished);
		m.cach(')');
	}

	m.AddILike1("C.F_TEXT", text, true);
}

// ----- query ---------------------------------------------------------------
class TFloatConnect : public TConnect
{
public:
	TFloatConnect();

	TRCDKey origin;
	char name[SIZE_OF_NAME];
	char decKind;
	CDate fromDate;
	char result;

	// 2014:212 LPR: was manual sort function
	bool operator<=(TFloatConnect &that) { return origin <= that.origin; }
};

TFloatConnect::TFloatConnect()
{
	Add(
		new TCharAlias("F_DEC_KIND", &decKind),
		new TDate("F_FROM_DATE", &fromDate, false),
		new TCharAlias("F_RESULT", &result),
		NULL
	);
}

#define floatcon OFFICE_GROUP_CAST(TFloatConnect)

static void floatcon_kind(mstr &m, TGroup *group)
{
	TLawsuit lawsuit;
	lawsuit.key = floatcon->key;
	if (lawsuit.Try())
		m.cpy(Kind->Seek(lawsuit.kind));
	else
		m.cpy(Type->Seek(floatcon->key.type));
}
static void floatcon_connect_type(mstr &m, TGroup *group) { m.cpy(ConnectType->Seek(floatcon->connectType)); }

static void floatcon_connect_kind(mstr &m, TGroup *group)
{
	if (floatcon->connectKind && strchr(CONNECT_KIND_LAWSUITS, floatcon->connectKind))
		m.cpy(ConnectKind->Seek(floatcon->connectKind));

}

static void floatcon_sender(mstr &m, TGroup *group) { m.cpy(floatcon->name); }
static void floatcon_type(mstr &m, TGroup *group) { m.cpy(Type->Seek(floatcon->key.type)); }
static void floatcon_origin(mstr &m, TGroup *group)
{
	Type->Seek(floatcon->origin.type);
	m.printf("%s %ld/%d", Type->alias, floatcon->origin.no, floatcon->origin.year);
}
static void floatcon_document(mstr &m, TGroup *group)
{
	if (floatcon->origin.type != floatcon->key.type)
	{
		Type->Seek(floatcon->key.type);
		m.printf("%s %ld/%d", Type->alias, floatcon->key.no, floatcon->key.year);
	}
}

static void floatcon_dec_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(floatcon->decKind)); }
static void floatcon_dec_result(mstr &m, TGroup *group) { m.cat(Result->Seek(floatcon->result)); }
static void floatcon_from_date(mstr &m, TGroup *group) { floatcon->fromDate.Print(m); }

#undef floatcon

class TFloatConnectQuery : public TFloatQuery
{
public:
	TFloatConnectQuery(TWindow *parent, const char *criteria, const char *name);
	
protected:
	virtual void Count();
};

static const offield floatcon_fields[] =
{
	{ "F_KIND", floatcon_kind },
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_CONNECT_TYPE", floatcon_connect_type },
	{ "F_CONNECT_KIND", floatcon_connect_kind },
	{ "F_CONNECT_NO", NULL },
	{ "F_CONNECT_YEAR", NULL },
	{ "F_SENDER", floatcon_sender },
	{ "F_TYPE", floatcon_type },
	{ "F_ORIGIN", floatcon_origin },
	{ "F_DOCUMENT", floatcon_document },
	{ "F_DEC_KIND", floatcon_dec_kind },
	{ "F_DEC_RESULT", floatcon_dec_result },
	{ "F_DEC_FROM_DATE", floatcon_from_date },
	{ NULL, NULL }
};

TFloatConnectQuery::TFloatConnectQuery(TWindow *parent, const char *criteria, const char *name)
	: TFloatQuery(parent, criteria, name)
{
}

void TFloatConnectQuery::Count()
{
	TQuery q(criteria);
	TFloatConnect connect;
	TSender sender;
	long nom = 0;
	TGSArray<TFloatConnect> floatConnArray;

	while (q.Read())
	{
		wait->TickCount(++nom, "обработени записа");
		connect << q;

		TRCDKeyContainer *tContainer = type2RCDKeyContainer(connect.key.type);
		*tContainer << connect;
		if (tContainer->Try() && find_origin(tContainer, connect.origin))
		{
			TFloatConnect *conn = new TFloatConnect();
			*conn << connect;
			conn->origin = connect.origin;
			floatConnArray.Add(conn);
			if (++count > maximum)
				break;
		}
		delete tContainer;
	}
	
	for (int i = 0; i < floatConnArray.Count(); i++)
	{
		wait->TickCount(i, "отпечатани записа");
		connect << floatConnArray[i];
		connect.origin = floatConnArray[i]->origin;
		
		sender.ucnType = connect.ucnType;
		strcpy(sender.ucn, connect.ucn);
		if (sender.Try())
			strcpy(connect.name, sender.name);
		else
		{
			*connect.name = '\0';
			dprintf("Липсва адресант %s %s за %s %ld/%ldг.\n", UCNType->Seek(connect.ucnType), connect.ucn,
				Type->Seek(connect.key.type), connect.key.no, connect.key.year);
		}

		office.Emit(tr1start, tr1final, floatcon_fields, &connect);
	}
}

static void query_connect(TWindow *parent, const char *criteria, bool queryActs)
{
	TFloatConnectQuery(parent, criteria, queryActs ? "FloatCon1.htm" : "FloatCon.htm").Execute();
}

class TConnectQuery : public TFloatQuery
{
public:
	TConnectQuery(TWindow *parent, TGArray<TConnect> *tConnects, const char *title);

protected:
	TGArray<TConnect> *connects;

	virtual void Count();
};

TConnectQuery::TConnectQuery(TWindow *parent, TGArray<TConnect> *tConnects, const char *title)
	: TFloatQuery(parent, NULL, "QueryCon.htm", title), connects(tConnects)
{
}

void TConnectQuery::Count()
{
	TFloatConnect connect;		// floatcon_fields[] compatibility

	for (count = 0; count < connects->Count(); count++)
	{
		connect << (*connects)[count];
		office.Emit(tr1start, tr1final, floatcon_fields, &connect);
	}
}

static void query_con(TWindow *parent, TGArray<TConnect> *connects, const char *title)
{
	TConnectQuery(parent, connects, title).Execute();
}

#include "QueryConnectListBox.h"
#include "QueryConnectListBox.cpp"

#include "QueryConnectListDialog.h"
#include "QueryConnectListDialog.cpp"

void query_connect(TWindow *parent, TConnect *connect)
{
	TQueryConnectListDialog(parent, connect).Execute();
}

#include "FloatConnectDialog.h"
#include "FloatConnectDialog.cpp"

void float_connect(TWindow *parent)
{
	TFloatConnectGroup group;
	TFloatConnectDialog(parent, &group).Execute();
}
