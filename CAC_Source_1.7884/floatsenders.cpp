#include "cac.h"

class TFloatSendersGroup : public TFloatGroup
{
public:
	TFloatSendersGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	CDate minDate;
	CDate maxDate;
	char kinds[SIZE_OF_KINDS];
	long flags;
	long courier;
	long fax;
	long email;
	TGArray<TSender> senders;
	char text[SIZE_OF_OUTREG_REMARK];
};

TFloatSendersGroup::TFloatSendersGroup()
	: TFloatGroup("T_OUTREG")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TRadio("F_FLAGS_NONE", &flags, OUTREG_NONE),
		new TRadio("F_FLAGS_REGULAR", &flags, OUTREG_REGULAR),
		new TRadio("F_FLAGS_R", &flags, OUTREG_R),
		new TRadio("F_FLAGS_AR", &flags, OUTREG_AR),
		new TRadio("F_FLAGS_PACKAGE", &flags, OUTREG_PACKAGE),
		new TCheck("F_FLAGS_COURIER", &courier, OUTREG_X_COURIER),
		new TCheck("F_FLAGS_FAX", &fax, OUTREG_X_FAX),
		new TCheck("F_FLAGS_EMAIL", &email, OUTREG_X_EMAIL),
		new TString("F_TEXT", text, sizeof text),
		NULL
	);

	flags = OUTREG_NONE;
	courier = fax = email = -1;
	order = ORDER_BY_DTYN;		// 2011:133 LPR: was in FormCriteria()
}

void TFloatSendersGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChars("F_COLLEGE", types, COLLEGE_XALLS);
	m.AddRange("F_NO", minNo, maxNo);
	m.AddRange("F_YEAR", minYear, maxYear);
	m.AddRange("F_DATE", minDate, maxDate);
	m.AddChars("F_KIND", kinds, KIND_OUT_REG_DOCS);
	m.AddPairs("F_UCN", "F_UCN_TYPE", senders, "F_RECEIVER", "F_RECEIVER_TYPE");

	if (flags)
		m.AddFlags("F_FLAGS", OUTREG_POSTAL, flags, OUTREG_POSTAL);

	long mask = 0, value = 0;

	if (courier != -1)  // mark
	{  // mark
		mask |= OUTREG_X_COURIER;
		value |= courier;
	}

	if (fax != -1)
	{
		mask |= OUTREG_X_FAX;
		value |= fax;
	}

	if (email != -1)
	{
		mask |= OUTREG_X_EMAIL;
		value |= email;
	}

	m.AddFlags("F_FLAGS_X", mask, value, OUTREG_X_ALL);

	if (any(text))
	{
		m.AddLike("(F_TEXT", text, true);
		m.NextOr();
		m.AddLike("F_REMARK", text, true);
		m.cach(')');
	}

	EndCriteria(m);
}

#define outReg OFFICE_GROUP_CAST(TOutReg)

static void senders_lawsuit_key(mstr &m, TGroup *group)
{
	if (outReg->lawsuit.no && outReg->lawsuit.year)
		m.printf("%s\v%d/%d", Type->Seek(outReg->lawsuit.type), outReg->lawsuit.no, outReg->lawsuit.year);
}

static void senders_descript(mstr &m, TGroup *group)
{
	m.printf("%s", Kind->Seek(outReg->kind));

	if (outReg->inNo)
	{
		TInReg inReg;

		inReg.key.no = outReg->inNo;
		inReg.key.year = outReg->inYear;

		m.printf("\v%s № %ld/%ldг.",
			inReg.Try("F_KIND") ? Kind->Seek(inReg.kind) : "Входящ документ", outReg->inNo, outReg->inYear);
	}
}

static void senders_post(mstr &m, TGroup *group) { print_postal(m, outReg); }

#undef outReg

static const offield senders_fields[] =
{
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_DATE",		NULL },
	{ "F_KIND", 	container_kind },
	{ "F_KEY",		senders_lawsuit_key },
	{ "F_DESCRIPT",  	senders_descript },
	{ "F_LAWSUIT",	outreg_lawsuit },
	{ "F_RECEIVERS",	outreg_receivers },
	{ "F_TEXT",		container_text },
	{ "F_POST",		senders_post },
	{ NULL,		NULL }
};

class TFloatSendersQuery : public TFloatQuery
{
public:
	TFloatSendersQuery(TWindow *parent, const char *criteria, const char *name, const char *title, TGArray<TSender> *tSenders);

	TGArray<TSender> *senders;
	virtual void Count();
};

TFloatSendersQuery::TFloatSendersQuery(TWindow *parent, const char *criteria, const char *name, const char *title, TGArray<TSender> *tSenders)
	: TFloatQuery(parent, criteria, name, title), senders(tSenders)
{
}

void TFloatSendersQuery::Count()
{
	TOutReg outReg;

	TQuery q(criteria);
	while (q.Read())
	{
		outReg << q;
		office.Emit(tr1start, tr1final, senders_fields, &outReg);
	}
}

static void query_senders(TWindow *parent, const char *criteria, TGArray<TSender> *senders)
{
	TFloatSendersQuery(parent, criteria, "OutRegDoc.htm", "Справка за документи от изходящ регистър", senders).Execute();
}

#include "FloatSendersDialog.h"
#include "FloatSendersDialog.cpp"

void float_senders(TWindow * parent)
{
	TFloatSendersGroup group;
	TFloatSendersDialog(parent, &group).Execute();
}
