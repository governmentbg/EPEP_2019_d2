#include "cac.h"

// --------------- TInRegBookGroup ----------
class TInRegBookGroup : public TBookGroup
{
public:
	TInRegBookGroup();
	virtual void FormCriteria(msql &m);
};

TInRegBookGroup::TInRegBookGroup()
	: TBookGroup("T_INREG", "F_COLLEGE", "F_DATE", false) { }

void TInRegBookGroup::FormCriteria(msql &m)
{
	const char *fields = "I.F_NO, I.F_YEAR, I.F_TYPE, I.F_LAWSUIT_NO, I.F_LAWSUIT_YEAR, I.F_DATE, I.F_KIND, "
		"I.F_TIME, I.F_LAWSUIT_TYPE, I.F_DECISION_KIND, I.F_SESSION_KIND, I.F_TEXT, "
		"L.F_SUBJECT, L.F_COMPOSITION, L.F_TEXT AS F_LAW_TEXT, L.F_KIND AS F_LAW_KIND";

	m.Begin("T_INREG I LEFT JOIN T_LAWSUIT L ON I.F_LAWSUIT_NO = L.F_NO "
		"AND I.F_LAWSUIT_YEAR = L.F_YEAR AND I.F_LAWSUIT_TYPE = L.F_TYPE",
		fields);

	m.AddChars("I.F_COLLEGE", types, xTypes);

	if (by == BOOK_BY_NO_YEAR)
	{
		m.AddRange("I.F_NO", minNo, maxNo);
		m.AddLong("I.F_YEAR", year);
	}
	else
		m.AddRange("I.F_DATE", minDate, maxDate);

	m.AddOrder("I.F_NO");
}

// ---------------- TBookInReg -------------
class TBookInReg : public TInReg
{
public:
	TBookInReg();

	long composition;
	long subject;
	char lawKind;
	char lawText[SIZE_OF_ORIGIN_TEXT];

	TGArray<TSideWind> sideWinds;
};

TBookInReg::TBookInReg()
{
	Add(
		new TLongAlias("F_COMPOSITION", &composition),
		new TLongAlias("F_SUBJECT", &subject),
		new TString("F_LAW_TEXT", lawText, sizeof lawText),
		new TCharAlias("F_LAW_KIND", &lawKind),
		NULL
	);

}

static void inreg_subject(mstr &m, const TBookInReg *inReg, bool code)
{
	if (inReg->lawKind)
	{
		if (code)
			m.cat(TSubject::Code(inReg->subject, inReg->lawKind));
		else
			m.cat(Subject->Seek(inReg->lawsuit.type, inReg->subject));
	}
}

#define inReg OFFICE_GROUP_CAST(TBookInReg)

static void inreg_date(mstr &m, TGroup *group) { m.printf("%D", &inReg->date); }
static void inreg_time(mstr &m, TGroup *group) { m.printf("%T", &inReg->time); }

static void inreg_discription(mstr &m, TGroup *group)
{
	if (strchr(KIND_TEXTUAL_NON_EPORTS, inReg->kind))
		m.cpy(inReg->text);
	else
		m.printf("%s %s", Kind->Seek(inReg->kind), inReg->text);
}

static void inreg_sender(mstr &m, TGroup *group)
{
	for (int i = 0; i < inReg->sideWinds.Count(); i++)
	{
		TSideWind *sideWind = inReg->sideWinds[i];
		sideWind->kind = inReg->kind;

		if (sideWind->Appealer())
		{
			m.sep("\v");
			print_links_names(m, sideWind);
		}
	}
}

static void inreg_law_type(mstr &m, TGroup *group)
{
	if (inReg->lawsuit.type && strchr(TYPE_LAWSUITS, inReg->lawsuit.type))
		m.cpy(Type->Seek(inReg->lawsuit.type));
}

static void inreg_law_no(mstr &m, TGroup *group)
{
	if (inReg->lawsuit.type && strchr(TYPE_LAWSUITS, inReg->lawsuit.type))
		m.printf("%d",inReg->lawsuit.no);
}

static void inreg_law_year(mstr &m, TGroup *group)
{
	if (inReg->lawsuit.type && strchr(TYPE_LAWSUITS, inReg->lawsuit.type))
		m.printf("%d",inReg->lawsuit.year);
}

static void inreg_composition(mstr &m, TGroup *group) { print_composition(m, inReg->composition, "с-в"); }
static void inreg_subject(mstr &m, TGroup *group) { inreg_subject(m, inReg, true); }
static void inreg_subject_name(mstr &m, TGroup *group) { inreg_subject(m, inReg, false); }
static void inreg_text(mstr &m, TGroup *group) { m.cpy(inReg->lawText); }

static void inreg_appealed_decision(mstr &m, TGroup *group)
{
	if (inReg->decisionKind)
		m.cpy(Kind->Seek(inReg->decisionKind));

	if (inReg->sessionKind)
	{
		m.cat(" от ");
		m.cat(Kind->Seek(inReg->sessionKind));
	}

	if (inReg->decisionDate.day)
		m.printf(" от %D г.", &inReg->decisionDate);
}

#undef inReg

static const offield InReg_fields[]=
{
	{ "F_NO", NULL },
	{ "F_DATE", inreg_date },
	{ "F_TIME", inreg_time },
	{ "F_DESCRIPTION", inreg_discription },
	{ "F_SENDER", inreg_sender },
	{ "F_LAWSUIT_TYPE", inreg_law_type },
	{ "F_LAWSUIT_NO", inreg_law_no },  //keygroup_flag_no
	{ "F_LAWSUIT_YEAR", inreg_law_year},
	{ "F_COMPOSITION", inreg_composition},
	{ "F_SUBJECT", inreg_subject},
	{ "F_SUBJECT_NAME", inreg_subject_name},
	{ "F_TEXT", inreg_text},
	{ "F_APPEALED_DECISION", inreg_appealed_decision},
	{ NULL, NULL }
};

// -------- TInRegBook ----------
class TInRegBook : public TBook
{
public:
	TInRegBook(TWindow *parent, TInRegBookGroup *group);
	virtual TGroup *Process(TQuery &q);
};

TInRegBook::TInRegBook(TWindow *parent, TInRegBookGroup *group)
	: TBook(parent, group, "InreB_fi__.htm", InReg_fields, ::book_fields, BOOK_FIXED_DATES)
{
}

TGroup *TInRegBook::Process(TQuery &q)
{
	TBookInReg *inReg = new TBookInReg;

	*inReg << q;
	load_sidebasicv(inReg, inReg->sideWinds);
	return inReg;
}

// ----- functions -----------------------------------------------------------
static void inreg_book(TWindow *parent, TInRegBookGroup *group)
{
	TInRegBook(parent, group).Execute(BOOK_EXEC_HEADER);
}

#include "InRegBookDialog.h"
#include "InRegBookDialog.cpp"

void inreg_book(TWindow *parent)
{
	TInRegBookGroup group;

	if (TInRegBookDialog(parent, &group).Execute() == IDOK)
		inreg_book(parent, &group);
}
