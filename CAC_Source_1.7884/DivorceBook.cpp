#include "cac.h"

#if COURT_TYPE == COURT_REGIONAL

// --------------- TDivorceBookGroup -----------------------
class TDivorceBookGroup : public TBookGroup
{
public:
	TDivorceBookGroup();
	void FormCriteria(msql &m);
};

TDivorceBookGroup::TDivorceBookGroup()
	: TBookGroup("T_SUBPOENA", "F_TYPE", "F_SUBPOENA_DATE", true, true)
{
	strcpy(types, TYPE_NON_PUNISHMENT_LAWS);
}

void TDivorceBookGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChar("F_TYPE", *types);

	if (by == BOOK_BY_NO_YEAR)
	{
		m.AddRange("F_ANNOUNCE_NO", minNo, maxNo);
		m.AddRange("F_SUBPOENA_DATE", CDate(1, 1, year), CDate(31, 12, year));
	}
	else
		m.AddRange("F_SUBPOENA_DATE", minDate, maxDate);

	m.AddChar("F_SUBPOENA_KIND", SUBPOENA_KIND_DIVORCE_MESSAGE);
	m.AddIsNull("F_ANNOUNCE_NO", false);
	m.AddOrder("F_ANNOUNCE_NO");
}

#define book OFFICE_GROUP_CAST(TDivorceBookGroup)

static void timeb_title_type(mstr &m, TGroup *group)
{
	char types[2] = { book->types[0], '\0' };
	print_types_list(m, types, false);
}

static void divorce_pr_date(mstr &m, CDate &dateMin, CDate &dateMax)
{
	if (dateMin != dateMax && !dateMin.Empty())
	{
		m.printf("%0Dг. - %0Dг.", &dateMin, &dateMax);
	}
	else if (!dateMin.Empty())
		m.printf("%0Dг.", &dateMin);
}

static void recap_period(mstr &m, TGroup *group)
{
	if (book->by != BOOK_BY_NO_YEAR)
		divorce_pr_date(m, book->minDate, book->maxDate);
	else
	{
		msql w;
		TSubpoena subpoena;
		CDate minDate, maxDate;

		book->FormCriteria(w);
		TQuery q(str(w));

		while (q.Read())
		{
			subpoena << q;
			if (minDate.Empty()) minDate = subpoena.subpoenaDate;
			maxDate = subpoena.subpoenaDate;
		}
		divorce_pr_date(m, minDate, maxDate);
	}
}

#undef book

static const offield title_fields[]=
{
	{ "F_TYPE", timeb_title_type },
	{ "F_PERIOD", recap_period },
	{NULL, NULL}
};

#define subpoena OFFICE_GROUP_CAST(TSubpoena)

static void divorce_sub_date(mstr &m, TGroup *group) { subpoena->subpoenaDate.Print(m, "%0D"); }
static void divorce_annaunce_no(mstr &m, TGroup *group) { itom(subpoena->announceNo, m); }

static void divorce_law_leter(mstr &m, TGroup *group)
{
	TSubpoena subletter;
	msql w;

	w.Begin("T_SUBPOENA", "F_OUTREG_NO, F_OUTREG_YEAR, F_SUBPOENA_KIND, F_SUBPOENA_DATE");
	w.Add(subpoena, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_UCN, F_UCN_TYPE");
	w.AddChar("F_SUBPOENA_KIND", SUBPOENA_KIND_DIVORCE_LETTER);

	TQuery q(str(w));
	while (q.Read())
	{
		subletter << q;

		if (subletter.outRegNo && subletter.outRegYear)
		{
			TOutReg outReg;

			outReg.key.no = subletter.outRegNo;
			outReg.key.year = subletter.outRegYear;
			SubpoenaKind->Seek(subletter.subpoenaKind);
			m.cat(SubpoenaKind->alias);
			if (outReg.Try())
				m.printf(" изпратено с изх. No %d/%0D", outReg.key.no, &outReg.date);
		}
		else
		{
			SubpoenaKind->Seek(subletter.subpoenaKind);
			m.printf("%s изготвено на %0D", SubpoenaKind->alias, &subletter.subpoenaDate);
		}
	}
}

static void divorce_law_kind(mstr &m, TGroup *group)
{
	print_type_or_kind(m, subpoena->key);
}

static void divorce_compos(mstr &m, TGroup *group)
{
	TLawsuit lawsuit;
	lawsuit.key = subpoena->key;

	if (lawsuit.Try("F_COMPOSITION"))
		print_composition(m, lawsuit.composition, "с-в\v");
}

static void divorce_decision(mstr &m, TGroup *group)
{
	TDecision decision;

	m.cat(Kind->Seek(subpoena->kind));
	decision.key = subpoena->key;
	decision.date = subpoena->date;
	decision.kind = subpoena->kind;
	decision.result = subpoena->eventStatus;

	if (decision.Try())
	{
		if (PRINT_JUDGEMENTS)
			m.printf(" %d", decision.eventNo);	
		
		m.printf(" от %0D", &decision.fromDate);
	}
}

static void divorce_force_date(mstr &m, TGroup *group) 
{
	TDecision decision;

	decision.key = subpoena->key;
	decision.date = subpoena->date;
	decision.kind = subpoena->kind;
	decision.result = subpoena->eventStatus;

	if (decision.Try() && !decision.forceDate.Empty())
		m.printf("Дата на влизане в законна сила от %0D", &decision.forceDate);
}

#undef subpoena

static const offield divorce_date[]=
{
	{ "F_DATE", divorce_sub_date },
	{NULL, NULL}
};

static const offield divorce_fields[]=
{
	{ "F_ANNOUNCE_NO", divorce_annaunce_no },
	{ "F_SUBPOENA_DATE", divorce_sub_date },
	{ "F_RECEIVER", subpoena_receiver }, //subpoena_lreceiver
	{ "F_LETTER", divorce_law_leter },
	{ "F_LAWSUIT_KIND", divorce_law_kind },
	{ "F_LAWSUIT_NO", basewind_key_no },
	{ "F_LAWSUIT_YEAR", basewind_key_year },
	{ "F_COMPOSITION", divorce_compos },
	{ "F_DECISION", divorce_decision },
	{ "F_FORCE_DATE", divorce_force_date },
	{NULL, NULL}
};


// ---------------------------  TDivorceBook ------------------------------
class TDivorceBook : public TBook
{
public:
	TDivorceBook(TWindow *parent, TDivorceBookGroup *group);
	virtual TGroup *Process(TQuery &q);
	virtual void Markers();
	virtual void EmitRow(TGroup *group);

	CDate date;
	const char *tr1head;
};

TDivorceBook::TDivorceBook(TWindow *parent, TDivorceBookGroup *group)
	: TBook(parent, group, "CanMB_f1_r.htm", divorce_date, title_fields, BOOK_FIXED_DATES)
{
}

TGroup *TDivorceBook::Process(TQuery &q)
{
	TSubpoena *subpoena = new TSubpoena;
	*subpoena << q;
	return subpoena;
}

void TDivorceBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr1head = office.Find("tr1 head");
	tr1final = office.Find("tr1 final");
}

void TDivorceBook::EmitRow(TGroup *group)
{
	TSubpoena *subpoena = ((TSubpoena *) group);
	if (date != subpoena->subpoenaDate)
	{
		office.Emit(tr1start, tr1head, divorce_date, group);
		date = subpoena->subpoenaDate;
	}

	office.Emit(tr1head, tr1final, divorce_fields, group);
}

void divorce_book(TWindow *parent)
{
	TDivorceBookGroup group;

	if (TBookDialog(parent, &group, Type, IDD_DEVORCE_BOOK, true).Execute() == IDOK)
		TDivorceBook(parent, &group).Execute(BOOK_EXEC_HEADER);
}
#endif