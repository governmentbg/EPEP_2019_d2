#include "cac.h"

// ----- TFloatExpenseGroup --------------------------------------------------
enum
{
	ORDER_BY_PAID	= ORDER_BY_USER,
	ORDER_BY_YNTK
};

class TFloatExpenseGroup : public TFloatGroup
{
public:
	TFloatExpenseGroup(char tSumType);

	virtual void FormCriteria(msql &m);

	const char sumType;
	char types[SIZE_OF_TYPES];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	char kinds[SIZE_OF_KINDS];
	CDate minDate;
	CDate maxDate;
	TGroupArray penalityTypes;
	char accountTypes[SIZE_OF_ACCOUNT_TYPES];
	char sumStates[SIZE_OF_SUM_STATES];
	long flags;
	char compositions[SIZE_OF_COMPOSITIONS];
	TGroupArray reasonTypes;
	CDate minPaidDate;
	CDate maxPaidDate;
	char paidNote[SIZE_OF_EXPENSE_NOTE];	// 2008:196 +related
	char text[SIZE_OF_EXPENSE_TEXT];
};

TFloatExpenseGroup::TFloatExpenseGroup(char tSumType)
	// 2014:210 FB2: nested selects -> join, see below
	: TFloatGroup("T_EXPENSE E LEFT JOIN T_SESSION S ON S.F_NO = E.F_NO AND S.F_YEAR = E.F_YEAR AND S.F_TYPE = E.F_TYPE "
		"AND S.F_KIND = E.F_KIND AND S.F_DATE = E.F_DATE", "E.*"), sumType(tSumType)
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TCharList("F_ACCOUNT_TYPES", accountTypes, sizeof accountTypes),
		new TCharList("F_SUM_STATES", sumStates, sizeof sumStates),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TDate("F_MIN_PAID_DATE", &minPaidDate, false),
		new TDate("F_MAX_PAID_DATE", &maxPaidDate, false),
		new TString("F_PAID_NOTE", paidNote, sizeof paidNote),
		new TString("F_TEXT", text, sizeof text),
		new TRadio("F_BY_PAID", &order, ORDER_BY_PAID),
		new TRadio("F_BY_YNTK", &order, ORDER_BY_YNTK),
		NULL
	);

	order = ORDER_BY_YNTK;	// array sorting
}

void TFloatExpenseGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChar("E.F_SUM_TYPE", sumType);
	m.AddChars("E.F_TYPE", types, sumType == SUM_TYPE_TAX ? TYPE_MONEYABLES : TYPE_LAWSUITS);
	m.AddRange("E.F_NO", minNo, maxNo);
	m.AddRange("E.F_YEAR", minYear, maxYear);
	m.AddChars("E.F_KIND", adjust_xxx_kinds(adjust_xxx_kinds(kinds, KIND_VISIBLE_DECISIONS, KIND_DECISIONS),
		KIND_VISIBLE_CLOSEDS, KIND_CLOSED_SESSIONS), KIND_MONEYABLES);
	m.AddRange("E.F_DATE", minDate, maxDate);

	if (sumType == SUM_TYPE_TAX || sumType == SUM_TYPE_PENALITY)
		m.AddRange("E.F_PAID_DATE", minPaidDate, maxPaidDate);

	if (sumType == SUM_TYPE_PENALITY)
		m.AddArray("E.F_PENALITY_TYPE", penalityTypes);
	else if (sumType == SUM_TYPE_REWARD)
		m.AddArray("E.F_REASON_TYPE", reasonTypes);

	m.AddChars("E.F_ACCOUNT_TYPE", accountTypes, ACCOUNT_TYPE_ALLS);
	m.AddChars("E.F_SUM_STATE", sumStates, SUM_STATE_ALLS);

	// 2011:136 LPR: need_crit() -> strequal()
	// 2014:210 FB2: nested select -> joined
	if (any(compositions) && !strequal(compositions, COMPOSITION_ALLS))
	{
		m.AddChars("(NOT E.F_KIND", KIND_SESSIONS);
		m.NextOr();
		m.AddCompos("S.F_COMPOSITION", compositions);
		m.cat(")");
	}

	m.AddILike1("E.F_PAID_NOTE", paidNote, true);

	if (sumType == SUM_TYPE_REWARD)
		m.AddILike1("E.F_TEXT", text, true);
}

// ----- query ---------------------------------------------------------------
#define expense OFFICE_GROUP_CAST(TExpense)

static void floatexp_law_kind(mstr &m, TGroup *group)
{
	Type->Seek(expense->key.type);
	m.cpy(Type->name);
}
static void floatexp_ses_kind(mstr &m, TGroup *group)
{
	Kind->Seek(expense->kind);
	m.cpy(Kind->name);
}
static void floatexp_person(mstr &m, TGroup *group)
{
	if (strchr(UCN_CITIZENS, expense->ucnType))
	{
		TCitizen citizen;

		citizen.ucnType = expense->ucnType;
		strcpy(citizen.ucn, expense->ucn);
		if (citizen.Try())
			m.printf("%s %s %s", citizen.name, citizen.reName, citizen.family);
		else
			print_missing(m, expense->ucnType, expense->ucn);
	}
	else
	{
		TFirm firm;

		firm.ucnType = expense->ucnType;
		strcpy(firm.ucn, expense->ucn);
		if (firm.Try())
			m.cpy(firm.name);
		else
			print_missing(m, expense->ucnType, expense->ucn);
	}
}
static void floatexp_account(mstr &m, TGroup *group)
{
	if (expense->accountType)
		m.cpy(AccountType->Seek(expense->accountType));
}

static void floatexp_sum(mstr &m, TGroup *group)
{
	m.printf("%.2lf", expense->sum1 + expense->sum2);
}

static void floatexp_road_sum(mstr &m, TGroup *group)
{
	if (expense->sum2)
		m.printf("%.2lf", expense->sum2);
}

static void floatexp_status(mstr &m, TGroup *group) { m.cat(SumState->Seek(expense->sumState)); }

static void floatexp_text(mstr &m, TGroup *group)
{
	if (any(expense->text))
		m.cpy(expense->text);
}
static void floatexp_penalty(mstr &m, TGroup *group)
{
	m.printf("%s\v", PenalityType->Seek(expense->penalityType));

	switch (expense->sumState)
	{
		case SUM_STATE_FORCED :
		{
			if (any(expense->text))	m.cat(expense->text);
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
			if (any(expense->text))
				m.printf("%s\v", expense->text);

			m.cat(SumState->Seek(expense->sumState));
			m.printf(" в %s", KindAlias->Seek(expense->rejectKind));

			if (!expense->date.Empty())
				m.printf(" %D", &expense->date);

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

#undef expense

// ----- TFloatExpense -------------------------------------------------------
class TFloatExpense : public TExpense
{
public:
	TFloatExpense() : summary(0.0) { }

	TRCDKey origin;
	double summary;

	// 2014:212 LPR: were manual sort functions
	static bool LessEqlYtnk(const long t1, const long t2);
	static bool LessEqlPaid(const long t1, const long t2);
	bool operator<=(const TFloatExpense &that) { return LessEqlYtnk((const long) this, (const long) &that); }
};

bool TFloatExpense::LessEqlYtnk(long t1, const long t2)
{
	// note: says "Ytnk", but the original sort function compares origin by TYN
	return ((const TFloatExpense *) t1)->origin <= ((const TFloatExpense *) t2)->origin;
}

bool TFloatExpense::LessEqlPaid(long t1, const long t2)
{
	// note: says "Paid", but the original sort function compares by date (and origin)
	const TFloatExpense *e1 = (const TFloatExpense *) t1;
	const TFloatExpense *e2 = (const TFloatExpense *) t2;
	return e1->date != e2->date ? e1->date < e2->date : e1->origin <= e2->origin;
}

// ----- TFloatExpenseQuery --------------------------------------------------
class TFloatExpenseQuery : public TFloatQuery
{
public:
	TFloatExpenseQuery(TWindow *parent, const char *criteria, const char *name, long tOrderBy = ORDER_BY_YNTK);

protected:
	void Count();
	long orderBy;
};

TFloatExpenseQuery::TFloatExpenseQuery(TWindow *parent, const char *criteria, const char *name, long tOrderBy)
	: TFloatQuery(parent, criteria, name, "Нерегламентирана справка за държавни такси"), orderBy(tOrderBy)
{
}

#define floatexp OFFICE_GROUP_CAST(TFloatExpense)

static void floatexp_summary(mstr &m, TGroup *group)
{
	if (floatexp->summary)
		m.printf("%s: %.2lf лв.", floatexp->sumState ? SumState->Seek(floatexp->sumState) : "Общо", floatexp->summary);
}

static void floatexp_rko(mstr &m, TGroup *group)
{
	TBaseWind baseWind("T_SUBPOENA", NULL);
	TSubpoena subpoena;
	msql w;

	baseWind << group;
	baseWind.ExactCriteria(w, "F_SUBPOENA_KIND, F_SUBPOENA_DATE, F_SUBPOENA_RECEIVED, F_RECEIVED_STATUS");
	w.AddLong("F_EXPENSE", floatexp->expense);
	w.AddOrder("F_SUBPOENA_NO");

	TQuery q(str(w));
	while (q.Read())
	{
		if (any(m))
			m.cat("\v");
		subpoena << q;
		SubpoenaKind->Seek(subpoena.subpoenaKind);
		m.printf("%s %s", SubpoenaKind->alias, ReceivedStatus->Seek(subpoena.receivedStatus));
		(subpoena.Received() ? subpoena.subpoenaReceived : subpoena.subpoenaDate).Print(m, " на %D");
	}
}
static void floatexp_origin(mstr &m, TGroup *group)
{
	Type->Seek(floatexp->origin.type);
	m.printf("%s %ld/%d", Type->alias, floatexp->origin.no, floatexp->origin.year);
}
static void floatexp_document(mstr &m, TGroup *group)
{
	if (floatexp->key.type != floatexp->origin.type)
	{
		Type->Seek(floatexp->key.type);
		m.printf("%s %ld/%d", Type->alias, floatexp->key.no, floatexp->key.year);
	}
}

#undef floatexp

static const offield floatexp_fields[]=
{
	{ "F_TYPE", floatexp_law_kind },
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_KIND", floatexp_ses_kind },
	{ "F_DATE", NULL },
	{ "F_PERSON", floatexp_person },
	{ "F_ACCOUNT", floatexp_account },
	{ "F_SUM", floatexp_sum },
	{ "F_ROAD_SUM", floatexp_road_sum },
	{ "F_STATUS", floatexp_status },
	{ "F_PAID_DATE", NULL },
	{ "F_PAID_NOTE", NULL },
	{ "F_SUMMARY", floatexp_summary },
	{ "F_TEXT", floatexp_text },
	{ "F_RKO", floatexp_rko },
	{ "F_ORIGIN", floatexp_origin },
	{ "F_DOCUMENT", floatexp_document },
	{ "F_PENALTY", floatexp_penalty },
	{ NULL, NULL }
};

void TFloatExpenseQuery::Count()
{
	TFloatExpense expense;
	TQuery q(criteria);
	TGSArray<TFloatExpense> floatExpenseArray(orderBy == ORDER_BY_PAID ? TFloatExpense::LessEqlPaid :
		TFloatExpense::LessEqlYtnk);
	TGArray<TFloatExpense> expenseSummary;
	TFloatExpense *total = new TFloatExpense;
	constant tr2start = office.Find("tr2 start");
	constant tr2final = office.Find("tr2 final");

	total->sumState = '\0';
	expenseSummary.Add(total);

	while (q.Read())
	{
		int i;
		TFloatExpense *tExpense;

		expense << q;
		total->summary += expense.sum1 + expense.sum2;

		for (i = 0; i < expenseSummary.Count(); i++)
		{
			tExpense = expenseSummary[i];
			if (tExpense->sumState == expense.sumState)
				break;
		}

		if (i == expenseSummary.Count())
		{
			tExpense = new TFloatExpense;
			tExpense->sumState = expense.sumState;
			expenseSummary.Add(tExpense);
		}

		tExpense->summary += expense.sum1 + expense.sum2;

		TRCDKeyContainer *tContainer = type2RCDKeyContainer(expense.key.type);
		*tContainer << expense;
		if (tContainer->Try() && find_origin(tContainer, expense.origin))
		{
			TFloatExpense *exp = new TFloatExpense();
			*exp << expense;
			exp->origin = expense.origin;
			exp->summary = expense.summary;
			floatExpenseArray.Add(exp);
			if (!Increment())
				break;
		}
		delete tContainer;
	}

	for (int i = 0; i < floatExpenseArray.Count(); i++)
	{
		wait->TickCount(i, "отпечатани записа");
		office.Emit(tr1start, tr1final, floatexp_fields, floatExpenseArray[i]);
	}

	office.Emit(tr1final, tr2start, null_fields, NULL);

	for (int i = 0; i < expenseSummary.Count(); i++)
		office.Emit(tr2start, tr2final, floatexp_fields, expenseSummary[i]);

	tr1final = tr2final;
}

static void query_expense(TWindow *parent, const char *criteria, char sumType, long orderBy = ORDER_BY_YNTK)
{
	TFloatExpenseQuery(parent, criteria, sumType == SUM_TYPE_REWARD ? "FloatRew.htm" :
		sumType == SUM_TYPE_TAX ? "FloatTax.htm" : "FloatPen.htm", orderBy).Execute();
}

#include "FloatExpenseDialog.h"
#include "FloatExpenseDialog.cpp"

void float_expense(TWindow *parent, char sumType)
{
	TFloatExpenseGroup group(sumType);
	TFloatExpenseDialog(parent, &group, sumType == SUM_TYPE_PENALITY ? IDD_FLOAT_PENALITY :
		sumType == SUM_TYPE_REWARD ? IDD_FLOAT_REWARD : IDD_FLOAT_TAX).Execute() == IDOK;
}
