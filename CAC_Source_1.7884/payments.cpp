#include "cac.h"

// ----- TPaymentTotal -------------------------------------------------------
class TPaymentTotal : public TPayment
{
public:
	TPaymentTotal();

	TGArray<TPayment> subpays;
	double total;
	long count;
};

TPaymentTotal::TPaymentTotal()
{
	Add(
		new TMoney("F_TOTAL", &total),
		new TLong("F_COUNT", &count),
		NULL
	);
}

static void print_person(mstr &m, const char *prefix, const char *ucn, char ucnType)
{
	TSideWind sideWind;

	if (prefix)
		m.cat(prefix);

	sideWind.ucnType = ucnType;
	strcpy(sideWind.ucn, ucn);

	if (!prefix || sideWind.ucnType == UCN_CITIZEN_UCN)
		sideWind.PrintGroup(m);
	else
	{
		if (strchr(UCN_FIRMS, sideWind.ucnType))
		{
			TFirm firm;

			firm.ucnType = ucnType;
			strcpy(firm.ucn, ucn);

			if (firm.Try("F_BULSTAT") && any(firm.bulstat))
				m.printf("ЕИК/БУЛСТАТ %s ", firm.bulstat);
		}

		sideWind.PrintNames(m);
	}
}

#define payment OFFICE_GROUP_CAST(TPayment)

static void payment_sum_type(mstr &m, TGroup *group) { m.cat(SumType->Seek(payment->sumType)); }
static void payment_account(mstr &m, TGroup *group) { m.cat(AccountType->Seek(payment->accountType)); }

static void payment_trans_id(mstr &m, TGroup *group)
{
	m.cat(!strcmp(payment->transId, TRANS_ID_PENDING) ? "- Неплатена -" : payment->transId);
}

static void payment_term_id(mstr &m, TGroup *group)
{
	if (strcmp(payment->transId, TRANS_ID_PENDING))
		itom(payment->termId, m);
}

static void payment_sum_words(mstr &m, TGroup *group) { sum_in_words(payment->sum, m); }

static void payment_key_court(mstr &m, TGroup *group, bool court, const char *sep)
{
	if (payment->key.no)
	{
		ConnectKind->Seek(payment->key.kind);
		m.printf("%s %ld/%ldг.", ConnectKind->name, payment->key.no, payment->key.year);
	}

	if (court || payment->court != COURT_CODE)
	{
		m.sep(sep);
		//m.cat(Court->Seek(payment->court));
		get_sencor_name(payment->court == COURT_CODE ? UCN_INSTANCE_SELF : UCN_INSTANCE_LOWER, payment->court, m);
	}
}

static void payment_key_short(mstr &m, TGroup *group) { payment_key_court(m, group, false, " - "); }
static void payment_key_long(mstr &m, TGroup *group) { payment_key_court(m, group, true, " - "); }
static void payment_stamp(mstr &m, TGroup *group) { m.printf("%0D %0T", &payment->date, &payment->time); }
static void payment_ucn_long(mstr &m, TGroup *group) { print_person(m, NULL, payment->ucn, payment->ucnType); }

static void payment_persons(mstr &m, TGroup *group)
{
	if (payment->oblType == payment->ucnType && !strcmp(payment->obl, payment->ucn))
		print_person(m, "Платец и задължено лице: ", payment->ucn, payment->ucnType);
	else
	{
		print_person(m, "Платец: ", payment->ucn, payment->ucnType);
		print_person(m, "\vЗадължено лице: ", payment->obl, payment->oblType);
	}
}

static void payment_person(mstr &m, TGroup *group) { print_person(m, "", payment->ucn, payment->ucnType); }

static void payment_card_id(mstr &m, TGroup *group)
{
	if (any(payment->cardId))
		m.printf("Карта: XXXX-XXXX-XXXX-%s", payment->cardId);
}

static void payment_descript(mstr &m, TGroup *group)
{
	payment_key_court(m, group, true, "\v");
	m.sep("\v");
	m.printf("%s %.2lf", SumType->Seek(payment->sumType), payment->sum);

	if (any(payment->text))
		m.printf("\v%s", payment->text);

	print_person(m, "\vЗадължено лице: ", payment->obl, payment->oblType);
}

static void payment_bic(mstr &m, TGroup *group) { print_court_account(m, payment->accountType, "F_BIC"); }
static void payment_iban(mstr &m, TGroup *group) { print_court_account(m, payment->accountType, "F_IBAN"); }

#undef payment

#define payment OFFICE_GROUP_CAST(TPaymentTotal)

static void payment_subcount(mstr &m, TGroup *group) { itom(payment->subpays.Count() + 1, m); }

#undef payment

static const offield payment_fields[] =
{
	{ "F_TRANS_ID",	payment_trans_id },
	{ "F_TERM_ID",	payment_term_id },
	{ "F_AUTH_ID",	NULL },
	{ "F_SUM",		NULL },
	{ "F_SUM_WORDS",	payment_sum_words },
	{ "F_STAMP",	payment_stamp },
	{ "F_KEY",		payment_key_long },
	{ "F_PERSON",	payment_person },
	{ "F_CARD_ID",	payment_card_id },
	{ "F_PERSONS",	payment_persons },
	{ "F_SUM_TYPE",	payment_sum_type },
	{ "F_ACCOUNT",	payment_account },
	{ "F_SUBCOUNT",	payment_subcount },
	{ "F_DESCRIPT",	payment_descript },
	{ "F_DATE",		NULL },
	{ "F_TIME",		NULL },
	{ "F_TOTAL",	NULL },
	{ "F_COUNT",	NULL },
	{ "F_TEXT",		NULL },
	{ "F_BIC",		payment_bic },
	{ "F_IBAN",		payment_iban },
	{ NULL, NULL }
};

// ----- TPrintPayment -------------------------------------------------------
class TPrintPayment : public TFloatQuery
{
public:
	TPrintPayment(TWindow *parent, const char *criteria) : TFloatQuery(parent, criteria, "Payments.htm", "справката") { }

protected:
	const char *tb1start;

	virtual void GetMarkers();
	virtual void Count();
};

void TPrintPayment::GetMarkers()
{
	tb1start = office.Find("tb1 start");
	TFloatQuery::GetMarkers();
}

void TPrintPayment::Count()
{
	TQuery q(criteria);
	TPayment payment;
	bool first = true;

	while (q.Read())
	{
		if (first)
			first = false;
		else
			office.EmitBreak(tb1start, tr1start);

		payment << q;
		office.InitAutoGen(1);		// 2013:302 IRQ: each payment anew

		for (int i = 0; i < 3; i++)
			office.Emit(tr1start, tr1final, payment_fields, &payment);
	}
}

static void print_payments(TWindow *parent, TPayment *payment)
{
	msql m;
	payment->FloatCriteria(m);
	TPrintPayment(parent, str(m)).Execute();
}

// ----- TFloatPaymentGroup --------------------------------------------------
class TFloatPaymentGroup : public TFloatGroup
{
public:
	TFloatPaymentGroup();

	virtual void FormCriteria(msql &m);

	char sumTypes[SIZE_OF_SUM_TYPES];
	char minTransId[SIZE_OF_TRANS_ID];
	char maxTransId[SIZE_OF_TRANS_ID];
	CDate minDate;
	CDate maxDate;
	CTime minTime;
	CTime maxTime;
	char accountTypes[SIZE_OF_ACCOUNT_TYPES];
	long ecr;
	char connectKinds[SIZE_OF_CONNECT_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	TGArray<TSender> courts;
	long personType;
	TGArray<TUCNGroup> persons;
	char text[SIZE_OF_PAYMENT_TEXT];
};

TFloatPaymentGroup::TFloatPaymentGroup()
	: TFloatGroup("T_PAYMENT")
{
	Add(
		+ new TCharList("F_SUM_TYPES", sumTypes, sizeof sumTypes),
		+ new TString("F_MIN_TRANS_ID", minTransId, sizeof minTransId),
		+ new TString("F_MAX_TRANS_ID", maxTransId, sizeof maxTransId),
		+ new TDate("F_MIN_DATE", &minDate, false),
		+ new TDate("F_MAX_DATE", &maxDate, false),
		+ new TTime("F_MIN_TIME", &minTime),
		+ new TTime("F_MAX_TIME", &maxTime),
		+ new TCharList("F_ACCOUNT_TYPES", accountTypes, sizeof accountTypes),
		+ new TLong("F_ECR", &ecr),
		+ new TCharList("F_CONNECT_KINDS", connectKinds, sizeof connectKinds),
		+ new TLong("F_MIN_NO", &minNo),
		+ new TLong("F_MAX_NO", &maxNo),
		+ new TLong("F_MIN_YEAR", &minYear),
		+ new TLong("F_MAX_YEAR", &maxYear),
		+ new TLong("F_PERSON_TYPE", &personType),
		+ new TString("F_TEXT", text, sizeof text),
		NULL
	);

	order = ORDER_BY_NULL;
}

enum
{
	PERSON_TYPE_UCN_OBL,
	PERSON_TYPE_UCN,
	PERSON_TYPE_OBL,
	PERSON_TYPE_COUNT
};

void TFloatPaymentGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChars("F_SUM_TYPE", sumTypes, SUM_TYPE_PAYMENTS);
	m.AddRange("F_TRANS_ID", minTransId, maxTransId);
	m.AddRange("F_DATE", minDate, maxDate);
	m.AddRange("F_TIME", minTime, maxTime);

	if (ecr)
		m.AddLong("F_ECR", ecr);

	m.AddChars("F_CONNECT_KIND", connectKinds, CONNECT_KIND_PAYMENTS);
	m.AddRange("F_NO", minNo, maxNo);
	m.AddRange("F_YEAR", minYear, maxYear);

	TIntegerArray tCourts;

	for (int i = 0; i < courts.Count(); i++)
		tCourts.Add(!strcmp(courts[i]->ucn, INSTANCE_SELF) ? COURT_CODE : atoi(courts[i]->ucn));

	m.AddLongs("F_COURT", tCourts);

	if (persons.Count() > 0)
	{
		switch (personType)
		{
			case PERSON_TYPE_UCN : m.AddPairs("F_UCN", "F_UCN_TYPE", persons); break;
			case PERSON_TYPE_OBL : m.AddPairs("F_UCN", "F_UCN_TYPE", persons, "F_OBL", "F_OBL_TYPE"); break;
			default :
			{
				m.AddPairs("F_UCN", "F_UCN_TYPE", persons, NULL, NULL, "(");
				m.NextOr();
				m.AddPairs("F_UCN", "F_UCN_TYPE", persons, "F_OBL", "F_OBL_TYPE");
				m.cat(")");
			}
		}
	}

	m.AddILike1("F_TEXT", text, true);
}

// ----- TPrintTransacts ----------------------------------------------------
class TPrintTransacts : public TProcess
{
public:
	TPrintTransacts(TWindow *parent) : TProcess(parent, "справката", WAIT) { }

	void Execute(const CDate &date, long ecr, TFloatPaymentGroup *group);

protected:
	TOffice office;
	const char *hd1start;
	const char *tr1start;
	const char *tr1break;
	const char *tr1final;
	const char *hd1final;

	void Query(const CDate &date, char accountType, long ecr, TFloatPaymentGroup *group);
	void EmitPayment(TPaymentTotal *payment);
};

void TPrintTransacts::Execute(const CDate &date, long ecr, TFloatPaymentGroup *group)
{
	if (office.Open("PayList.htm"))
	{
		hd1start = office.Find("hd1 start");
		tr1start = office.Find("tr1 start");
		tr1break = office.Find("tr1 break");
		tr1final = office.Find("tr1 final");
		hd1final = office.Find("hd1 final");

		office.Emit(NULL, hd1start, null_fields, NULL);

		if (group)
		{
			if (strchr(group->accountTypes, ACCOUNT_TYPE_BUDGET))
			{
				Query(date, ACCOUNT_TYPE_BUDGET, 0, group);

				if (strchr(group->accountTypes, ACCOUNT_TYPE_DEPOSITS))
					office.EmitBreak();
			}

			if (strchr(group->accountTypes, ACCOUNT_TYPE_DEPOSITS))
				Query(date, ACCOUNT_TYPE_DEPOSITS, 0, group);
		}
		else
		{
			Query(date, ACCOUNT_TYPE_BUDGET, ecr, NULL);
			office.EmitBreak();
			Query(date, ACCOUNT_TYPE_DEPOSITS, ecr, NULL);
		}

		office.Emit(hd1final, NULL, null_fields, NULL);
		office.Show(wait);
	}
}

void TPrintTransacts::Query(const CDate &date, char accountType, long ecr, TFloatPaymentGroup *group)
{
	TPaymentTotal payment;
	msql m(payment.Name);
	double total = 0.0;

	payment.date = Today;
	payment.time = Now();
	payment.accountType = accountType;

	office.Emit(hd1start, tr1start, payment_fields, &payment);

	if (group)
		group->FormCriteria(m);
	else
	{
		m.AddDate("F_DATE", date);
		m.AddString("NOT F_TRANS_ID", TRANS_ID_PENDING);

		if (ecr)
			m.AddLong("F_ECR", ecr);
	}

	m.AddChar("F_ACCOUNT_TYPE", accountType);
	// 2016:342 IRQ/p.47.1: transId -> stamp
	//m.AddOrder("F_TRANS_ID, F_ECR, F_TIME");
	m.AddOrder("F_DATE, F_TIME, F_ECR");

	TQuery q(str(m));
	TPayment tPayment;

	while (q.Read())
	{
		tPayment << q;
		total += tPayment.sum;

		if (tPayment.SameTransact(payment))
		{
			payment.subpays.AddCopy(tPayment);
			payment.total += tPayment.sum;
		}
		else
		{
			payment.count++;

			if (payment.total)
			{
				EmitPayment(&payment);
				payment.subpays.Flush();
			}

			payment << tPayment;
			payment.total = tPayment.sum;
		}
	}

	if (payment.total)
		EmitPayment(&payment);

	payment.total = total;
	office.Emit(tr1final, hd1final, payment_fields, &payment);
}

void TPrintTransacts::EmitPayment(TPaymentTotal *payment)
{
	office.Emit(tr1start, tr1break, payment_fields, payment);

	for (int i = 0; i < payment->subpays.Count(); i++)
		office.Emit(tr1break, tr1final, payment_fields, payment->subpays[i]);
}

static void print_transacts(TWindow *parent, const CDate &date, long ecr)
{
	TPrintTransacts(parent).Execute(date, ecr, NULL);
}

#include "EditPaymentListBox.h"
#include "EditPaymentListBox.cpp"

#include "EditPaymentDialog.h"
#include "EditPaymentDialog.cpp"

#include "PaymentListWindow.h"
#include "PaymentListWindow.cpp"

#include "PaymentListDialog.h"
#include "PaymentListDialog.cpp"

void payments(TWindow *parent)
{
	TPaymentListDialog(parent).Execute();
}

// ----- float payment -------------------------------------------------------
void query_payment(TWindow *parent, TFloatPaymentGroup *group)
{
	TPrintTransacts(parent).Execute(CDate(), 0, group);
}

static constant person_titles[] = { "Платци или задължени лица", "Платци", "Задължени лица" };

#include "FloatPaymentDialog.h"
#include "FloatPaymentDialog.cpp"

void float_payments(TWindow *parent)
{
	TFloatPaymentGroup group;
	TFloatPaymentDialog(parent, &group).Execute();
}

// ----- query payments ------------------------------------------------------
class TQueryPayments : public TPeriodGroup
{
public:
	TQueryPayments();

	long posCount;
	long transTotal;
	double sumTotal;
};

TQueryPayments::TQueryPayments()
	: TPeriodGroup("T_QUERY_PAYMENTS")
{
	Add(
		new TLong("F_POS_COUNT", &posCount),
		new TLong("F_TRANS_TOTAL", &transTotal),
		new TMoney("F_SUM_TOTAL", &sumTotal),
		NULL
	);
}

#define payment OFFICE_GROUP_CAST(TQueryPayments)

static void payment_pos_count(mstr &m, TGroup *group) { m.printf("%ld", payment->posCount); }
static void payment_trans_total(mstr &m, TGroup *group) { m.printf("%ld", payment->transTotal); }
static void payment_sum_total(mstr &m, TGroup *group) { m.printf("%.2lf", payment->sumTotal); }

#undef payment

static const offield query_fields[] =
{
	{ "F_MIN_DATE",	NULL },
	{ "F_MAX_DATE",	NULL },
	{ "F_POS_COUNT",	payment_pos_count },
	{ "F_TRANS_TOTAL",payment_trans_total },
	{ "F_SUM_TOTAL",	payment_sum_total },
	{ "F_NOW",		generic_now },
	{ NULL, NULL }
};

#if RANGECHECK
static void debug_same_trans(const TPayment &payment)
{
	FILE *f = fopen("c:\\tmp\\__same", "a");
	fprintf(f, "%ld %s\n", payment.termId, payment.transId);
	fclose(f);
}
#else  // RANGECHECK
#define debug_same_trans(payment)
#endif  // RANGECHECK

void query_payments(TWindow *parent)
{
	TQueryPayments group;

	group.minDate.day = 1;
	group.minDate.month = 1;

	if (get_period(parent, &group))
	{
		TPayment payment;
		msql m(payment.Name);
		TWaitWindow wait(parent, WAIT);

		m.AddString("NOT F_TRANS_ID", TRANS_ID_PENDING);
		m.AddRange("F_DATE", group.minDate, group.maxDate);
		// 2016:342 IRQ/p.47.1: transId -> date/time
		//m.AddOrder("F_TERM_ID, F_TRANS_ID");
		m.AddOrder("F_TERM_ID, F_DATE, F_TIME");
		TQuery q(str(m));

		// 2018-10-24: LPR/LRQ: try to handle transId 0
		long lastTermId = 0;
		TGArray<TPayment> payments;

		while (q.Read())
		{
			payment << q;

			if (payment.termId != lastTermId)
			{
				group.posCount++;
				payments.AddCopy(payment);
				lastTermId = payment.termId;
			}
			else
			{
				int i;

				for (i = 0; i < payments.Count(); i++)
					if (payments[i]->SameTransact(payment))
						break;

				if (i == payments.Count())
					payments.AddCopy(payment);
				else
					debug_same_trans(payment);
			}

			group.sumTotal += payment.sum;
		}

		group.transTotal = payments.Count();
		show_office("QPosVSS.htm", query_fields, &group);
	}
}
