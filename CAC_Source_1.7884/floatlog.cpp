#include "cac.h"

// ----- TFloatLogGroup ------------------------------------------------------
enum
{
	FLOAT_LOG_TEXT,
	FLOAT_LOG_HYPER,
	FLOAT_LOG_COUNT	// 2018-07-05 LRQ/LPR: +related
};

class TFloatLogGroup : public TFloatGroup
{
public:
	TFloatLogGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_EXT_TYPES];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	CDateTime minStamp;
	CDateTime maxStamp;
	TGArray<TUserAliasGroup> users;
	char gops[SIZE_OF_GOPS];
	long variant;
};

TFloatLogGroup::TFloatLogGroup()
	: TFloatGroup("T_LOG")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TDate("F_MIN_DATE", &minStamp, true),
		new TTime("F_MIN_TIME", &minStamp),
		new TDate("F_MAX_DATE", &maxStamp, true),
		new TTime("F_MAX_TIME", &maxStamp),
		new TRadio("F_TEXT", &variant, FLOAT_LOG_TEXT),
		new TRadio("F_HYPER", &variant, FLOAT_LOG_HYPER),
		new TRadio("F_COUNT", &variant, FLOAT_LOG_COUNT),
		new TCharList("F_GOPS", gops, sizeof gops),
		NULL
	);

	maxStamp.hour = 23;
	maxStamp.minute = 59;
	maxStamp.second = 59;
}

void TFloatLogGroup::FormCriteria(msql &m)
{
	if (variant == FLOAT_LOG_COUNT)
		m.Begin("T_LOG", "COUNT(F_LOG) AS F_COUNT");
	else
		BeginCriteria(m);

	if (non_equal(types, TYPE_LOGS))
	{
		if (strchr(types, TYPE_OTHER))
		{
			m.AddChars("(F_TYPE", types, TYPE_LOGS);
			m.NextOr();
			m.Add("F_TYPE");
			m.cat("IS NULL)");
		}
		else
			m.AddChars("F_TYPE", types, TYPE_LOGS);
	}

	m.AddRange("F_NO", minNo, maxNo);
	m.AddRange("F_YEAR", minYear, maxYear);
	m.AddRange("F_STAMP", minStamp, maxStamp);
	m.AddArray("F_UCN", users);
	m.AddChars("F_GOP", gops, GOP_ALLS);
	// 2015:005 +F_LOG
	if (variant != FLOAT_LOG_COUNT)
		m.AddOrder("F_STAMP, F_LOG, F_TYPE, F_YEAR, F_NO");
}

// ----- query ---------------------------------------------------------------
#define log OFFICE_GROUP_CAST(TLog)

static void floatlog_date(mstr &m, TGroup *group) { log->stamp.Date().Print(m); }
static void floatlog_time(mstr &m, TGroup *group) { log->stamp.Time().Print(m, "%lT"); }
static void floatlog_type(mstr &m, TGroup *group) { log->TypeName(m); }

static void floatlog_gop(mstr &m, TGroup *group)
{
	GOP->gop = log->gop;

	if (GOP->Try())
		m.cpy(GOP->name);
	else
		m.printf("%d", log->gop);
}

static void floatlog_key_no_year(mstr &m, TGroup *group)
{
	if (log->key.no || log->key.year)
		m.printf("%ld/%ld", log->key.no, log->key.year);
}

static void floatlog_kind(mstr &m, TGroup *group)
{
	// maximum information/compatibility/steadiness is logging priority
	if (log->kind)
	{
		Kind->kind = log->kind;
		if (Kind->Try())
			m.cpy(Kind->name);
		else
			m.printf("%d", log->kind);
	}
	if (!log->date.Empty())
	{
		if (any(m))
			m.cat(", ");
		log->date.Print(m);
	}
}

static void floatlog_user(mstr &m, TGroup *group) { print_user(m, log->ucn, UCN_CITIZEN_UCN, false); }

#undef log

static const offield floatlog_fields[] =
{
	{ "F_DATE", floatlog_date },
	{ "F_TIME", floatlog_time },
	{ "F_GOP", floatlog_gop },
	{ "F_OPC", NULL },
	{ "F_TYPE", floatlog_type },
	{ "F_NO_YEAR", floatlog_key_no_year },
	{ "F_KIND", floatlog_kind },
	{ "F_TEXT", NULL },
	{ "F_USER", floatlog_user },
	{ NULL, NULL }
};

class TFloatLogQuery : public TFloatQuery
{
public:
	TFloatLogQuery(TWindow *parent, const char *criteria);

protected:
	virtual void Count();
};

TFloatLogQuery::TFloatLogQuery(TWindow *parent, const char *criteria)
	: TFloatQuery(parent, criteria, "FloatLog.htm")
{
	maximum = 100000;
}

void TFloatLogQuery::Count()
{
	TQuery q(criteria);
	TLog tLog;

	while (q.Read() && Increment())
	{
		tLog << q;
		tLog.CheckMD5();
		office.Emit(tr1start, tr1final, floatlog_fields, &tLog);
	}
}

static void query_log(TWindow *parent, const char *criteria, long variant)
{
	if (variant == FLOAT_LOG_HYPER)
		TFloatLogQuery(parent, criteria).Execute();
	else if (variant == FLOAT_LOG_COUNT)
		message("%ld записа съответстват на зададените критерии.", AppName->GetLong(criteria, "F_COUNT"));
	else
	{
		TTextFile tf;
		FILE *f = tf.OpenTemp();
		TWaitWindow wait(parent, WAIT);
		TWindowHideParent hide(parent);
		TQuery q(criteria);
		TLog tLog;
		mstr m;
		int count = 0;

		fputs("Журнал\n\n", f);
		fputs("Дата       Час      Операция          Номер Год. Допълнителна класификация                           Потребител\n", f);
		fputs("---------- -------- --------------- - ----- ---- --------------------------------------------------- -----------------------------------\n", f);
		while (q.Read())
		{
			tLog << q;
			tLog.CheckMD5();
			m.clear();
			tLog.Print(m);
			fprintf(f, "%s\n", str(m));
			wait.TickCount(++count, "обработени записа");
		}
		fputc('\n', f);
		switch (count)
		{
			case 0 : fputs("Няма записи отговарящи", f); break;
			case 1 : fputs("Един запис отговаря", f); break;
			default : fprintf(f, "%d записа отговарят", count);
		}
		fputs(" на зададените критерии.\n", f);
		tf.Show(&wait);
	}

	log(LOG_LOG, CM_ADMIN_LOG);
}

#include "FloatLogDialog.h"
#include "FloatLogDialog.cpp"

void float_log(TWindow *parent)
{
	TFloatLogGroup group;
	TFloatLogDialog(parent, &group).Execute();
}
