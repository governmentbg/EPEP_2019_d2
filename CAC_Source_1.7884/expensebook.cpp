#include "cac.h"

// --------------- TExpenseBookGroup -----------------------
class TExpenseBookGroup : public TBookGroup
{
public:
	TExpenseBookGroup();

	virtual void FormCriteria(msql &m);

	char compositions[SIZE_OF_COMPOSITIONS];
	long all;
};

#if 20180116
TExpenseBookGroup::TExpenseBookGroup()
	: TBookGroup("T_EXPENSE E JOIN T_LAWSUIT L ON L.F_NO = E.F_NO AND L.F_YEAR = E.F_YEAR AND L.F_TYPE = E.F_TYPE"
		" JOIN T_PENALITY_TYPE P ON P.F_PENALITY_TYPE = E.F_PENALITY_TYPE", "F_TYPE", "F_DATE", true)
#else  // 20180116
TExpenseBookGroup::TExpenseBookGroup() : TBookGroup("T_EXPENSE_BOOK", "F_TYPE", "F_DATE", true)
#endif  // 20180116
{
	Add(
		new TCharList("F_COMPOSITION", compositions, sizeof compositions),
		new TCheck("F_ALL", &all, true),
		NULL
	);

	all = TRUE;
}

void TExpenseBookGroup::FormCriteria(msql &m)
{
#if 20180116
	// TODO: move to ctor after the old variant is deleted
	fields = "E.F_NO, E.F_YEAR, E.F_TYPE, E.F_DATE, E.F_EXPENSE, E.F_SUM_STATE, E.F_SUM_1, E.F_TEXT, E.F_UCN, "
		"E.F_UCN_TYPE, E.F_PENALITY_TYPE, E.F_PAID_DATE, E.F_REJECT_KIND, E.F_KIND, E.F_REJECT_COURT, "
		"E.F_INVOLVEMENT, E.F_PAID_NOTE, L.F_COMPOSITION AS F_LAW_COMPOSITION, L.F_KIND AS F_LAW_KIND, "
		"E.F_REJECT_DATE";
	BeginCriteria(m);
#else  // 20180116
	mstr name, fields;

	fields.cpy("E.F_NO, E.F_YEAR, E.F_TYPE, E.F_DATE, E.F_EXPENSE, E.F_SUM_STATE, E.F_SUM_1, E.F_TEXT, E.F_UCN, "
	"E.F_UCN_TYPE, E.F_PENALITY_TYPE, E.F_PAID_DATE, E.F_REJECT_KIND, E.F_KIND, E.F_REJECT_COURT, E.F_INVOLVEMENT, "
	"E.F_PAID_NOTE, L.F_COMPOSITION AS F_LAW_COMPOSITION, L.F_KIND AS F_LAW_KIND ");
	name.cat("T_EXPENSE E ");
	name.cat("JOIN T_LAWSUIT L ON "
	"L.F_NO = E.F_NO AND L.F_YEAR = E.F_YEAR AND L.F_TYPE = E.F_TYPE ");
	name.cat("JOIN T_PENALITY_TYPE P ON "
	"P.F_PENALITY_TYPE = E.F_PENALITY_TYPE ");

	m.Begin(str(name), str(fields));
#endif  // 20180116

	m.AddChars("E.F_TYPE", types);
	m.AddRange("E.F_DATE", minDate, maxDate);
	m.AddCompos("L.F_COMPOSITION", compositions);
	m.AddChar("E.F_SUM_TYPE", SUM_TYPE_PENALITY);

	if (!all)
		m.AddFlags("P.F_FLAGS", PENALITY_TYPE_BOOK, PENALITY_TYPE_BOOK, PENALITY_TYPE_ALL);

	m.AddOrder("E.F_DATE, E.F_TYPE, E.F_YEAR, E.F_NO");
}

#define exp_group OFFICE_GROUP_CAST(TExpenseBookGroup)

static void expense_title_compos(mstr &m, TGroup *group) { print_compositions_list(m, exp_group->compositions); }

#undef exp_group

static const offield title_fields[]=
{
	{ "F_TYPES", book_types },
	{ "F_COMPOSITIONS", expense_title_compos },
	{ "F_PERIOD", book_dates },
	{NULL, NULL}
};

// ---------------- TBookExpense ---------------------
class TBookExpense : public TExpense
{
public:
	TBookExpense();

	char lawKind;
	long compositionLaw;
};

TBookExpense::TBookExpense()
{
	Add(
		new TLong("F_LAW_COMPOSITION", &compositionLaw),
		new TCharAlias("F_LAW_KIND", &lawKind),
		NULL
	);
}

#define expense OFFICE_GROUP_CAST(TBookExpense)

static void expense_law_no(mstr &m, TGroup *group) { itom(flag_key_no(&expense->key), m); }
static void expense_law_year(mstr &m, TGroup *group) { itom(expense->key.year, m); }
static void expense_law_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(expense->lawKind)); }
static void expense_composition(mstr &m, TGroup *group)
{
	if (PRINT_COMPOSITIONS && expense->compositionLaw)
		m.printf(" %s с-в", Composition->Seek(expense->compositionLaw));
}
static void expense_involvement(mstr &m, TGroup *group)
{
	m.cpy(Involvement->Seek(expense->involvement));
}
static void expense_person(mstr &m, TGroup *group)
 {
	if (strchr(UCN_CITIZENS, expense->ucnType))  // UCN_CITIZENS = "cC"
	{
		TCitizen citizen;

		citizen.ucnType = expense->ucnType;
		strcpy(citizen.ucn, expense->ucn);
		if (citizen.Get())
			m.printf("%s %s %s", citizen.name, citizen.reName, citizen.family);
		if (any(citizen.reFamily))
			m.printf("-%s", citizen.reFamily);
	}
	else
	{
		TFirm firm;

		firm.ucnType = expense->ucnType;
		strcpy(firm.ucn, expense->ucn);
		if (firm.Get())
			m.cpy(firm.name);
	}
}
static void expense_ses_kind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(expense->kind)); }
static void expense_date(mstr &m, TGroup *group) { m.printf("%D", &expense->date); }
static void expense_ses_composition(mstr &m, TGroup *group)
{
	TDecision decision;
	decision << expense;
	if (decision.Try())
	{
		TSession session;
		session << decision;
		session.kind = decision.sessionKind;
		if (session.Get())
		{
			if (PRINT_COMPOSITIONS && (expense->compositionLaw != session.composition))
				m.printf("%s с-в", Composition->Seek(session.composition));
		}
	}
}
static void expense_sum(mstr &m, TGroup *group) { m.printf("%.2lf ", expense->sum1); }

static void expense_penality(mstr &m, TGroup *group)
{
	m.printf("%s\v", PenalityType->Seek(expense->penalityType));

	switch (expense->sumState)
	{
		case SUM_STATE_FORCED :
		{
			m.cat(expense->text);
			break;
		}
		case SUM_STATE_PAID :
		{
			m.cat(SumState->Seek(expense->sumState));

			if (!expense->paidDate.Empty() || any(expense->paidNote))
			{
				if (!expense->paidDate.Empty())
					m.printf(" %D", &expense->paidDate);

				if (any(expense->paidNote))
					m.printf( " вн.б. %s ",	expense->paidNote);
			}

			if (any(expense->text))
				m.printf("\v%s", expense->text);

			break;
		}
		case SUM_STATE_ALTERED:
		case SUM_STATE_REJECTED:
		{
			m.cat(expense->text);
			m.printf(" %s", SumState->Seek(expense->sumState));
			m.printf(" в %s", KindAlias->Seek(expense->rejectKind));

		#if 20180116
			if (!expense->rejectDate.Empty())
				m.printf(" %D", &expense->rejectDate);
		#else  // 20180116
			if (!expense->date.Empty())
				m.printf(" %D", &expense->date);
		#endif  // 20180116

			if (atob(expense->rejectCourt))
			{
				TSender sender;

				sender.ucnType = UCN_INSTANCE_HIGHER;
				strcpy(sender.ucn, expense->rejectCourt);

				if (sender.Get("F_NAME"))
					m.printf(" на %s", sender.name);
			}

			break;
		}
	}
}
static void expense_month(mstr &m, TGroup *group)
{
	m.printf("Месец %s %ld", Month->Seek(expense->date.month), expense->date.year);
}

#undef expense

static const offield expense_fields[]=
{
	{ "F_LAWSUIT_KIND", expense_law_kind },
	{ "F_LAWSUIT_NO",  expense_law_no }, // keygroup_flag_no  - da go sloja
	{ "F_LAWSUIT_YEAR", expense_law_year },
	{ "F_COMPOSITION", expense_composition },
	{ "F_INVOLVEMENT", expense_involvement },
	{ "F_NAME", expense_person },
	{ "F_SESSION_KIND", expense_ses_kind },
	{ "F_DATE", expense_date },
	{ "F_SESSION_COMPOSITION", expense_ses_composition },
	{ "F_SUM", expense_sum },
	{ "F_PENALTY", expense_penality },
	{ "F_MONTH", expense_month },
	{ NULL, NULL }
};

// ---------------------------  TExpenseBook ------------------------------
class TExpenseBook : public TBook
{
public:
	TExpenseBook(TWindow *parent, TExpenseBookGroup *group);
	virtual TGroup *Process(TQuery &q);
	virtual void Markers();
	virtual void EmitRow(TGroup *group);

	long count;
	int month;
	const char *tr2start;
	const char *tr2final;
};

TExpenseBook::TExpenseBook(TWindow *parent, TExpenseBookGroup *group)
	: TBook(parent, group, "PnltB_fi__.htm", expense_fields, title_fields, BOOK_FIXED_DATES)
{
	count = 0;
	month = 0;
}

TGroup *TExpenseBook::Process(TQuery &q)
{
	TBookExpense *expense = new TBookExpense;
	*expense << q;
	return expense;
}

void TExpenseBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr1final = office.Find("tr1 final");
	tr2start = office.Find("tr2 start");
	tr2final = office.Find("tr2 final");

	if (!array->Count())
		tr1final = tr2final;
}

void TExpenseBook::EmitRow(TGroup *group)
{
	if (((TBookExpense *) group)->date.month != month)
	{
		office.Emit(tr1start, tr1final, expense_fields, group);
		month = ((TBookExpense *) group)->date.month;
	}

	office.Emit(tr2start, tr2final, expense_fields, group);
	count++;
	if (count == array->Count())
		tr1final = tr2final;
}

#include "ExpenseBookDialog.h"
#include "ExpenseBookDialog.cpp"

void expense_book(TWindow *parent)
{
	TExpenseBookGroup group;

	if (TExpenseBookDialog(parent, &group).Execute() == IDOK)
		TExpenseBook(parent, &group).Execute(BOOK_EXEC_HEADER);
}