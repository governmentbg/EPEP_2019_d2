#include "cac.h"

// ----- TExecLink -----------------------------------------------------------
TExecLink::TExecLink()
	: TLinkWind("T_LINK_WIND", TExecLink::Clone), TTryAddress(NULL)
{
}

TExecLink::TExecLink(TSubpoena *subpoena)
	: TLinkWind("T_LINK_WIND", TExecLink::Clone), TTryAddress(TAnyAddress::NewGroup(!subpoena->uclp))
{
	*this << subpoena;
	*address << subpoena;
}

TGroup *TExecLink::Clone()
{
	return new TExecLink;
}

void TExecLink::Print(mstr &m)
{
	if (!Proxied())
		m.printf("%s - ", Involvement->Seek(involvement));

	print_side_links(m, this);
	m.cat(" - ");
	execlink_address(m, this);
	m.compress();
}

// ----- functions -----------------------------------------------------------
#include "ExecListLinkListWindow.h"
#include "ExecListLinkListWindow.cpp"

static void execlistsum_type(mstr &m, TExecListSum *sum, bool alias)
{
	SumType->Seek(sum->sumType);
	m.cat(alias ? SumType->alias : SumType->name);
}

#define execsum OFFICE_GROUP_CAST(TExecListSum)

static void execlistsum_alias(mstr &m, TGroup *group) { execlistsum_type(m, execsum, true); }

static void execlistsum_sum(mstr &m, TGroup *group)
{
	if (execsum->sumType && strchr(SUM_TYPE_VALUES, execsum->sumType))
		m.printf("%.2lf", execsum->sum);
}

static void execlistsum_sender(mstr &m, TGroup *group)
{
	if (atob(execsum->sender))
	{
		Account->ucnType = UCN_EXEC_LIST;
		strcpy(Account->ucn, execsum->sender);
		m.cat(Account->Get() ? Account->name : Account->ucn);
	}
	else
		m.cpy("Държавата");
}

static const char *execlistsum_persons(mstr &m, TGroup *group)
{
	if (execsum->execListKind == EXEC_LIST_COUNTRY)
		execlistsum_sender(m, group);
	else
	{
		for (int i = 0; i < execsum->persons.Count(); i++)
		{
			m.sep(", ");
			execsum->persons[i]->Print(m);
		}
	}

	return split_at(ncstr(m), 60);
}

static void execlistsum_persons_line_1(mstr &m, TGroup *group)
{
	mstr w;
	execlistsum_persons(w, group);
	m.cat(str(w));
}

static void execlistsum_persons_line_2(mstr &m, TGroup *group)
{
	mstr w;
	m.cat(execlistsum_persons(w, group));
}

#undef execsum

#include "ExecListPersonListBox.h"
#include "ExecListPersonListBox.cpp"

#include "AccountAliasFace.h"
#include "AccountAliasFace.cpp"

#include "EditExecListSumDialog.h"
#include "EditExecListSumDialog.cpp"

static bool edit_exec_list_sum(TWindow* parent, TExecList *execList, TExecListSum *sum, bool edit)
{
	return TEditExecListSumDialog(parent, execList, sum, edit, execList->execListKind == EXEC_LIST_PRIVATE ?
		IDD_EDIT_EXEC_LIST_SUM_PRIVATE : IDD_EDIT_EXEC_LIST_SUM_COUNTRY).Execute() == IDOK;
}

#include "ExecListSumListWindow.h"
#include "ExecListSumListWindow.cpp"

#include "EditExecListDialog.h"
#include "EditExecListDialog.cpp"

void load_exec_list_data(TExecList *execList)
{
	TExecListSum sum;
	TExecListPerson person;
	TGArray<TExecListAddress> addresses;
	msql m;

	sum << execList;
	sum.FloatCriteria(m);

	if (execList->execListKind == EXEC_LIST_PRIVATE)
	{
		TExecListAddress address;
		address << execList;
		address.LoadFloat(addresses);
	}

	TQuery q(str(m));
	while (q.Read())
	{
		TExecListSum *tSum = new TExecListSum;

		*tSum << q;
		execList->sums.Add(tSum);

		person << tSum;
		person.FloatCriteria(m);

		TQuery r(str(m));
		while (r.Read())
		{
			TExecListPerson *tPerson = new TExecListPerson;

			*tPerson << r;
			tPerson->SetAddress(addresses);
			tSum->persons.Add(tPerson);
		}
	}

	for (int i = 0; i < execList->allSumPersons.Count(); i++)
		execList->allSumPersons[i]->SetAddress(addresses);
}

bool process_exec_list(TWindow *parent, TExecList *execList, TGArray<TLinkWind> &xLinks, TGArray<TSideWind> &sideWinds,
	const char *categories, char category, bool edit)
{
	if (execList->execListKind == EXEC_LIST_COUNTRY)
	{
		for (int i = 0; i < xLinks.Count(); i++)
			execList->AddSumPerson(xLinks[i]);
	}
	else
	{
		// 2014:317 PRQ: all (but self) sides if no opposite
		// 2014:338 PRQ: all third, all (but self) if none
		// 2015:071 PRQ: all sides, LPR: by category
		int non_sames = 0;

		for (int i = 0; i < sideWinds.Count(); i++)
		{
			TSideWind *sideWind = sideWinds[i];
			int n;

			for (n = 0; n < xLinks.Count(); n++)
				if (!strcmp(xLinks[n]->ucn, sideWind->ucn) && xLinks[n]->ucnType == sideWind->ucnType)
					break;

			if (n == xLinks.Count())
			{
				if (categories[i] == category)
					execList->AddSumPerson(sideWind);
				else if (execList->InsertSumPerson(sideWind, non_sames))
					non_sames++;
			}
		}
	}

	if (edit)
	{
		execList->execListDate = Today;
		load_exec_list_data(execList);
	}

	return TEditExecListDialog(parent, execList, &xLinks, edit).Execute() == IDOK;
}

class TPrintExecListSum : public TExecListSum
{
public:
	TPrintExecListSum() : recipient(0) { }

	TAccount account;
	int recipient;

	static const offield fields[];
};

#define execsum OFFICE_GROUP_CAST(TPrintExecListSum)

static void execlistsum_sum_type(mstr &m, TGroup *group)	// 2014:300 other -> text
{
	if (!strchr(SUM_TYPE_OTHERS, execsum->sumType) || !any(execsum->sumText))
	{
		size_t len = m.len();

		execlistsum_type(m, execsum, false);
		strlwr(ncstr(m) + len);		// 2014:337 p.75.1
		m.cach(' ');
	}

	m.cat(execsum->sumText);
}

static void execlistsum_bank_account(mstr &m, TGroup *group) { m.cat(execsum->account.bankAccount); }
static void execlistsum_bic(mstr &m, TGroup *group) { m.cat(execsum->account.bic); }
static void execlistsum_bank_name(mstr &m, TGroup *group) { m.cat(execsum->account.bankName); }

static void execlistsum_recipient(mstr &m, TGroup *group)
{
	execsum->recipient = execsum->account.flags & ACCOUNT_JUDICIAL ? ACCOUNT_JUDICIAL : FLAG_ACTIVE;
	print_recipient(m, execsum->recipient);
}

#undef execsum

const offield TPrintExecListSum::fields[] =
{
	{ "F_SUM_TYPE",		execlistsum_sum_type },
	{ "F_SENDER_NAME",	execlistsum_sender },
	{ "F_IBAN",			execlistsum_bank_account },
	{ "F_BIC",			execlistsum_bic },
	{ "F_BANK",			execlistsum_bank_name },
	{ "F_RECIPIENT",		execlistsum_recipient },
	{ "F_SUM_TEXT",		NULL },
	{ NULL, NULL }
};

void emit_exec_list_additional_text(mstr &m, TExecList *execList, TGArray<TLinkWind> &xLinks)
{
	bool *printed = new bool[execList->sums.Count()];
	TPrintExecListSum sum;

	memset(printed, '\0', execList->sums.Count() * sizeof(bool));

	for (int i = 0; i < execList->sums.Count(); i++)
	{
		if (printed[i])
			continue;

		TGArray<TLegalSide> laids;
		TExecListSum *start = execList->sums[i];

		m.sep("\r\n");
		m.cat("\r\nОсъжда ");
		for (int l = 0; l < start->persons.Count(); l++)
		{
			TLegalSide *laid = new TLegalSide;

			*laid << start->persons[l];
			laids.Add(laid);

			// 2014:317 get address if 1 side
			if (execList->execListKind == EXEC_LIST_COUNTRY)
			{
				for (int n = 0; n < xLinks.Count(); n++)
				{
					TExecLink *xlink = TYPECHECK_PTR_CAST(TExecLink, xLinks[n]);

					if (xlink->ucnType == laid->ucnType && !strcmp(xlink->ucn, laid->ucn))
					{
						if (laid->address)	// 2+ found
						{
							laid->address = NULL;
							break;
						}

						laid->address = xlink->address;
					}
				}
			}
			else
				laid->address = start->persons[l]->address;
		}

		print_laids(m, &laids, execList->execListKind != EXEC_LIST_COUNTRY, "\r\n");
		m.cach(':');

		for (int l = 0; l < laids.Count(); l++)
			laids[l]->address = NULL;

		for (int n = i; n < execList->sums.Count(); n++)
		{
			TExecListSum *tSum = execList->sums[n];
			bool values = strchr(SUM_TYPE_VALUES, tSum->sumType);

			if (!tSum->SamePersons(start))
				continue;

			sum << tSum;
			SumType->Seek(sum.sumType);
			m.cat("\r\n- ");

			if (atob(sum.sender))
			{
				sum.account.ucnType = UCN_EXEC_LIST;
				strcpy(sum.account.ucn, sum.sender);
				sum.account.Get();
			}

			if (values)
			{
				m.printf("да заплат%s сумата от %.2lf", tSum->persons.Count() == 1 ? "и" : "ят", sum.sum);
				if (!strcmp(m.end() - 2, "00"))
					m.cut(m.len() - 3);
				Currency->Seek(sum.currency);
				m.printf(" %s (словом: ", Currency->numeric);
				sum_in_words(sum.sum, m, sum.currency, false);
				m.cach(')');
			}

			TOffice::Emit(m, TPrintExecListSum::fields, &sum, (execList->execListKind == EXEC_LIST_COUNTRY ||
				!values) && any(SumType->execListTemplate) ? SumType->execListTemplate :
				", представляваща {F_SUM_TYPE}");

			execList->recipient |= sum.recipient;
			printed[n] = true;
		}
	}

	delete[] printed;
}

// ----- TFloatExecListGroup -------------------------------------------------
enum
{
	EXEC_LIST_JUDICIALS = 0,
	EXEC_LIST_ALLS = EXEC_LIST_COUNTRY | EXEC_LIST_PRIVATE
};

class TFloatExecListGroup : public TFloatGroup
{
public:
	TFloatExecListGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	long execListKind;
	char lawKinds[SIZE_OF_LAW_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	char compositions[SIZE_OF_COMPOSITIONS];
	long minExecListNo;
	long maxExecListNo;
	CDate minExecListDate;
	CDate maxExecListDate;
	char sumTypes[SIZE_OF_SUM_TYPES];
	TGArray<TSender> senders;
	CDate minCollectDate;
	CDate maxCollectDate;
	long voluntary;
	// 2016:078
	TGArray<TSender> receivers;
	CDate minSendDate;
	CDate maxSendDate;
	long letterOutDate;
};

TFloatExecListGroup::TFloatExecListGroup()
	: TFloatGroup("T_EXEC_LIST",
		"DISTINCT E.F_EXEC_LIST_NO, E.F_EXEC_LIST_YEAR, E.F_TYPE, E.F_EXEC_LIST_KIND, E.F_EXEC_LIST_DATE, E.F_NO, "
		"E.F_YEAR, E.F_TDEP_TEXT, E.F_TDEP, E.F_TDEP_UCN_TYPE, E.F_TDEP_DATE, E.F_TDEP_IN_NO, T.F_SUM_NO, "
		"T.F_SUM_TYPE, T.F_SUM_TEXT, T.F_SUM, T.F_SENDER, T.F_CURRENCY")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TRadio("F_EXEC_LIST_ALLS", &execListKind, EXEC_LIST_ALLS),
		new TRadio("F_EXEC_LIST_JUDICIALS", &execListKind, 0),
		new TRadio("F_EXEC_LIST_COUNTRY", &execListKind, EXEC_LIST_COUNTRY),
		new TRadio("F_EXEC_LIST_PRIVATE", &execListKind, EXEC_LIST_PRIVATE),
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TLong("F_MIN_EXEC_LIST_NO", &minExecListNo),
		new TLong("F_MAX_EXEC_LIST_NO", &maxExecListNo),
		new TDate("F_MIN_EXEC_LIST_DATE", &minExecListDate, false),
		new TDate("F_MAX_EXEC_LIST_DATE", &maxExecListDate, false),
		new TCharList("F_SUM_TYPES", sumTypes, sizeof sumTypes),
		new TDate("F_MIN_COLLECT_DATE", &minCollectDate, false),
		new TDate("F_MAX_COLLECT_DATE", &maxCollectDate, false),
		new TCheck("F_VOLUNTARY", &voluntary, EXEC_LIST_COLLECT_VOLUNTARY),
		new TDate("F_MIN_SEND_DATE", &minSendDate, false),
		new TDate("F_MAX_SEND_DATE", &maxSendDate, false),
		new TCheck("F_LETTER_OUT_DATE", &letterOutDate, TRUE),
		NULL
	);

	execListKind = EXEC_LIST_ALLS;
	voluntary = -1;
}

void TFloatExecListGroup::FormCriteria(msql &m)
{
	mstr tables("T_EXEC_LIST E");
	bool sendate = !minSendDate.Empty() || !maxSendDate.Empty();

	if (sendate || receivers.Count())
	{
		tables.cat(" JOIN T_SUBPOENA S ON S.F_NO = E.F_NO AND S.F_YEAR = E.F_YEAR AND S.F_TYPE = E.F_TYPE "
			"AND S.F_EXEC_LIST_NO = E.F_EXEC_LIST_NO AND S.F_SUBPOENA_DATE = E.F_EXEC_LIST_DATE "
			"AND S.F_EXEC_LIST_KIND = E.F_EXEC_LIST_KIND "
			"LEFT JOIN T_SUBPOENA L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE "
			"AND L.F_KIND = S.F_KIND AND L.F_DATE = S.F_DATE AND L.F_EVENT_STATUS = S.F_EVENT_STATUS "
			"AND L.F_REGARD = S.F_SUBPOENA_NO "
			"LEFT JOIN T_NIA_PROTOCOL P ON P.F_NIA_NO = E.F_NIA_NO AND P.F_NIA_YEAR = E.F_NIA_YEAR "
			"AND P.F_NIA_TYPE = E.F_NIA_TYPE "
			"LEFT JOIN T_OUTREG O ON O.F_NO = P.F_OUT_NO AND O.F_YEAR = P.F_OUT_YEAR");

		if (letterOutDate)
			tables.cat(" LEFT JOIN T_OUTREG LO ON LO.F_NO = L.F_OUTREG_NO AND LO.F_YEAR = L.F_OUTREG_YEAR"); 
	}

	tables.cat(" LEFT JOIN T_EXEC_LIST_SUM T ON T.F_EXEC_LIST_NO = E.F_EXEC_LIST_NO "
		"AND T.F_EXEC_LIST_YEAR = E.F_EXEC_LIST_YEAR AND T.F_TYPE = E.F_TYPE "
		"AND T.F_EXEC_LIST_KIND = E.F_EXEC_LIST_KIND");

	if (!minCollectDate.Empty() || !maxCollectDate.Empty() || voluntary != -1)
	{
		tables.cat(" LEFT JOIN T_EXEC_LIST_COLLECT C ON C.F_EXEC_LIST_NO = E.F_EXEC_LIST_NO "
			"AND C.F_EXEC_LIST_YEAR = E.F_EXEC_LIST_YEAR AND C.F_TYPE = E.F_TYPE "
			"AND C.F_EXEC_LIST_KIND = E.F_EXEC_LIST_KIND");
	}

	if (execListKind == EXEC_LIST_PRIVATE)
	{
		minCollectDate.Clear();
		maxCollectDate.Clear();
		senders.Flush();
	}
	else if (execListKind == EXEC_LIST_JUDICIALS)
		tables.printf(" JOIN T_ACCOUNT A ON A.F_UCN = T.F_SENDER", UCN_EXEC_LIST);

	m.Begin(str(tables), fields);

	CDate tMinExecListDate = minExecListDate;

	if (tMinExecListDate.Empty() && minYear)
	{
		tMinExecListDate.day = 1;
		tMinExecListDate.month = 1;
		tMinExecListDate.year = minYear;
	}

	m.AddRange("E.F_EXEC_LIST_DATE", tMinExecListDate, maxExecListDate);

	if (execListKind == EXEC_LIST_JUDICIALS)
	{
		m.AddLong("E.F_EXEC_LIST_KIND", EXEC_LIST_COUNTRY);
		m.AddChar("A.F_UCN_TYPE", UCN_EXEC_LIST);
		m.AddFlags("A.F_FLAGS", ACCOUNT_JUDICIAL, ACCOUNT_JUDICIAL, ACCOUNT_ALL);
	}
	else if (execListKind != EXEC_LIST_ALLS)
		m.AddLong("E.F_EXEC_LIST_KIND", execListKind);

	char tTypes[SIZE_OF_TYPES];
	strcpy(tTypes, types);

	if (!any(tTypes))
		for (const char *k = lawKinds; *k; k++)
			strcach_nx(tTypes, kind2type(*k));

	m.AddChars("E.F_TYPE", tTypes);
	m.AddRange("E.F_NO", minNo, maxNo);
	m.AddRange("E.F_YEAR", minYear, maxYear);
	m.AddRange("E.F_EXEC_LIST_NO", minExecListNo, maxExecListNo);
	m.AddChars("T.F_SUM_TYPE", sumTypes);
	m.AddArray("F_UCN", senders, "T.F_SENDER");

	// 2014:210 FIX: select count, not joined
	m.AddRange("C.F_DATE", minCollectDate, maxCollectDate);

	if (voluntary != -1)
		m.AddFlags("C.F_FLAGS", EXEC_LIST_COLLECT_VOLUNTARY, voluntary, EXEC_LIST_COLLECT_ALL);

	if (sendate)
	{
		m.AddRange(letterOutDate ? "(LO.F_DATE" : "(L.F_SUBPOENA_DATE", minSendDate, maxSendDate);
		m.NextOr();
		m.AddRange("O.F_DATE", minSendDate, maxSendDate);
		m.cat(")");
	}

	if (receivers.Count())
	{
		TGArray<TSender> tReceivers;

		for (int i = 0; i < receivers.Count(); i++)
		{
			TSender *receiver = receivers[i];
			TSender *tReceiver = new TSender;

			strcpy(tReceiver->ucn, receiver->ucn);
			tReceiver->ucnType = receiver_ucn_type(receiver->ucnType);
			tReceivers.Add(tReceiver);
		}

		m.AddIsNull("((NOT E.F_NIA_NO", true);
		m.AddPairs("F_UCN", "F_UCN_TYPE", tReceivers, "E.F_TDEP", "E.F_TDEP_UCN_TYPE");
		m.cat(")");
		m.NextOr();
		m.AddPairs("F_UCN", "F_UCN_TYPE", tReceivers, "L.F_LRECEIVER_UCN", "L.F_LRECEIVER_UCN_TYPE");
		m.cat(")");
	}

	// 2014:290 LPR: we need them sorted on query read, not later with array add
	m.AddOrder("E.F_EXEC_LIST_YEAR");
	// 2014:301 TRQ: type order for numbering without type produces wrong result
	if (EXEC_LIST_NUMBERING & EXEC_LIST_NUMBERING_TYPE)
		m.cat(", E.F_TYPE");
	m.cat(", E.F_EXEC_LIST_NO, E.F_EXEC_LIST_KIND, T.F_SENDER, T.F_SUM_NO");
}

// ----- TFloatExecListTotal -------------------------------------------------
TFloatExecListTotal::TFloatExecListTotal()
{
	Add(new TMoney("F_SUM", &sum));
}

// ----- TFloatExecListTotalArray --------------------------------------------
void TFloatExecListTotalArray::AddSum(const TExecListSumBase *sum)
{
	TFloatExecListTotal *tSum = NULL;

	for (int i = 0; i < Count(); i++)
	{
		if (!strcmp((*this)[i]->currency, sum->currency))
		{
			tSum = (*this)[i];
			break;
		}
	}

	if (!tSum)
	{
		tSum = new TFloatExecListTotal;
		Currency->Seek(sum->currency);
		*tSum << Currency;
		Add(tSum);
	}

	tSum->sum += sum->sum;
}

void TFloatExecListTotalArray::Print(mstr &m, const char *separator) const
{
	for (int i = 0; i < Count(); i++)
	{
		m.sep(separator);
		m.printf("%.2lf %s", (*this)[i]->sum, (*this)[i]->numeric);
	}
}

// ----- TFloatExecListSum ---------------------------------------------------
class TFloatExecListSum : public TExecListSum
{
public:
	TFloatExecListSum();
	virtual ~TFloatExecListSum();

	CDate execListDate;
	long no;
	long year;
	mstr tdepText;
	char tdep[SIZE_OF_UCN];
	char tdepInNo[SIZE_OF_NIA_TDEP_IN_NO];
	CDate tdepDate;
	char tdepUCNType;

	TGArray<TSubpoena> *subpoenas;
	TLawsuit *lawsuit;
	TIndexContainer *event;
	TGArray<TOutReg> *outRegs;
	long execListCount;
	long personCount;
	long targetCount;
	long voluntary;

	void LoadCollects();
	TGArray<TExecListCollect> collects;

	static const offield fields[];
	static const offield fields2[];
};

TFloatExecListSum::TFloatExecListSum()
	: personCount(0), targetCount(0)
{
	Add(
		new TDate("F_EXEC_LIST_DATE", &execListDate, false),
		new TLong("F_NO", &no),
		new TLong("F_YEAR", &year),
		new TText("F_TDEP_TEXT", &tdepText, SIZE_OF_TDEP_TEXT),
		new TStringAlias("F_TDEP", tdep, sizeof tdep),
		new TLong("F_SUBCOUNT_1", &execListCount),
		new TLong("F_SUBCOUNT_2", &personCount),
		new TLong("F_SUBCOUNT_3", &targetCount),
		new TString("F_TDEP_IN_NO", tdepInNo, sizeof tdepInNo),
		new TDate("F_TDEP_DATE", &tdepDate, false),
		new TCharAlias("F_TDEP_UCN_TYPE", &tdepUCNType),
		NULL
	);
}

void TFloatExecListSum::LoadCollects()
{
	TExecListCollect collect;
	msql m;

	collect << this;
	collect.FloatCriteria(m, "F_DATE, F_SUM, F_CURRENCY, F_FLAGS, F_SUM_TEXT");

	if (voluntary != -1)
		m.AddFlags("F_FLAGS", EXEC_LIST_COLLECT_VOLUNTARY, voluntary, EXEC_LIST_COLLECT_ALL);

	collect.LoadArray(collects, str(m));
}

TFloatExecListSum::~TFloatExecListSum()
{
	if (execListCount)
	{
		delete subpoenas;
		delete lawsuit;
		delete event;
		delete outRegs;
	}
}

#define execsum OFFICE_GROUP_CAST(TFloatExecListSum)

static void execlistsum_exec_list_no(mstr &m, TGroup *group)
{
	print_exec_list_no(m, execsum->execListNo, execsum->execListKind);
}

static void execlistsum_lawsuit_kind(mstr &m, TGroup *group)
{
	Kind->Seek(execsum->lawsuit->kind);
	m.cat(Kind->alias);
}

static void execlistsum_lawsuit_no(mstr &m, TGroup *group) { itom(execsum->lawsuit->key.no, m); }
static void execlistsum_lawsuit_year(mstr &m, TGroup *group) { itom(execsum->lawsuit->key.year, m); }

static void execlistsum_composition(mstr &m, TGroup *group)
{
	if (PRINT_COMPOSITIONS)
		m.printf("%s състав", Composition->Seek(execsum->lawsuit->composition));
}

static void execlistsum_event_kind(mstr &m, TGroup *group) { m.cat(Kind->Seek(execsum->event->kind)); }

static void execlistsum_event_no(mstr &m, TGroup *group)
{
	if (PRINT_JUDGEMENTS && execsum->event->eventNo)
		m.printf("№ %ld", execsum->event->eventNo);
}

static void execlistsum_event_date(mstr &m, TGroup *group)
{
	container_event_date1(execsum->event).Print(m);
}

static void execlistsum_force_date(mstr &m, TGroup *group)
{
	TDate *forceDate = (TDate *) execsum->event->Find("F_FORCE_DATE");
	forceDate->Date().Print(m, "В законна сила от %D");
}

static void execlistsum_name(mstr &m, TGroup *group) { if (execsum->sumType) execlistsum_type(m, execsum, false); }

static void execlistsum_currency(mstr &m, TGroup *group)
{
	if (execsum->sumType && strchr(SUM_TYPE_VALUES, execsum->sumType))
	{
		Currency->Seek(execsum->currency);
		m.cat(Currency->numeric);
	}
}

static void execlistsum_debtors(mstr &m, TGroup *group)
{
	for (int i = 0; i < execsum->persons.Count(); i++)
	{
		TExecListPerson *person = execsum->persons[i];

		m.sep("\v");
		print_name(m, person->ucn, person->ucnType);
		// 2017:005
		if (PRINT_UCN && person->ucnType == UCN_CITIZEN_UCN)
			m.printf(", ЕГН %s", person->ucn);
		else if (strchr(UCN_FIRMS, person->ucnType))
		{
			TFirm firm;

			firm.ucnType = person->ucnType;
			strcpy(firm.ucn, person->ucn);

			if (firm.Try("F_BULSTAT") && any(firm.bulstat))
				m.printf(", ЕИК %s", firm.bulstat);
		}
	}
}

static void execlistsum_collectors(mstr &m, TGroup *group)
{
	if (execsum->execListKind == EXEC_LIST_COUNTRY)
		execlistsum_sender(m, group);
	else
	{
		for (int i = 0; i < execsum->subpoenas->Count(); i++)
		{
			m.sep("\v"),
			(*execsum->subpoenas)[i]->PrintNames(m);
		}
	}
}

static void execlistsum_collect_sums(mstr &m, TGroup *group)
{
	for (int i = 0; i < execsum->collects.Count(); i++)
	{
		TExecListCollect *collect = execsum->collects[i];

		m.sep("\ahr\b");

		if (execsum->voluntary == -1 && (collect->flags & EXEC_LIST_COLLECT_VOLUNTARY))
			m.cat("Доброволно изпълнение\v");

		if (collect->sum == 0.0)
			m.printf("%s на %D", collect->sumText, &collect->date);
		else
		{
			Currency->Seek(collect->currency);
			m.printf("%.2lf %s внесена на %D", collect->sum, Currency->numeric, &collect->date);
		}
	}
}

static void execlistsum_recv_left(mstr &m, TGroup *group)
{
	(*execsum->subpoenas)[0]->subpoenaReceived.Print(m);
}

static void execlistsum_recv_right(mstr &m, TGroup *group)
{
	TSubpoena *subpoena = (*execsum->subpoenas)[0];

	m.cat(subpoena->receivedText);

	if (!subpoena->subpoenaReturned.Empty())
	{
		m.sep("\v");
		subpoena->subpoenaReturned.Print(m, "Върнат на %Dг.");
	}

	if (any(execsum->tdepInNo))
	{
		m.sep("\v\v");
		print_exec_list_sendrecv(m, execsum->tdep, execsum->tdepUCNType);
		m.printf("\vПолучен вх.№ %s/%Dг.", execsum->tdepInNo, &execsum->tdepDate);
	}
}

#undef execsum

const offield TFloatExecListSum::fields[] =
{
	{ "F_SUBCOUNT_1",		NULL },
	{ "F_SUBCOUNT_2",		NULL },
	{ "F_SUBCOUNT_3",		NULL },
	{ "F_EXEC_LIST_NO",	execlistsum_exec_list_no },
	{ "F_LAWSUIT_KIND",	execlistsum_lawsuit_kind },
	{ "F_LAWSUIT_NO",		execlistsum_lawsuit_no },
	{ "F_LAWSUIT_YEAR",	execlistsum_lawsuit_year },
	{ "F_COMPOSITION",	execlistsum_composition },
	{ "F_EVENT_KIND",		execlistsum_event_kind },
	{ "F_EVENT_NO",		execlistsum_event_no },
	{ "F_EVENT_DATE",		execlistsum_event_date },
	{ "F_FORCE_DATE",		execlistsum_force_date },
	{ "F_SUM_TYPE",		execlistsum_name },
	{ "F_SUM_TEXT",		NULL },
	{ "F_SUM",			execlistsum_sum },
	{ "F_CURRENCY",		execlistsum_currency },
	{ "F_DEBTORS",		execlistsum_debtors },
	{ "F_COLLECTORS",		execlistsum_collectors },
	{ "F_EXEC_LIST_DATE",	NULL },
	// 2015:082 register only
	{ "F_TDEP_TEXT",		NULL },
	{ "F_COLLECT_SUMS",	execlistsum_collect_sums },
	// 2016:076 +related
	{ "F_LEFT",			execlistsum_recv_left },
	{ "F_RIGHT",		execlistsum_recv_right },
	{ NULL, NULL }
};

#define outReg OFFICE_GROUP_CAST(TOutReg)

static void outreg_send_left(mstr &m, TGroup *group)
{
	m.printf("Изх.№ %ld/", outReg->key.no);

	if (outReg->date.Empty())
		m.printf("%ldг.", outReg->key.year);
	else
		m.printf("%D", &outReg->date);
}

static void outreg_send_right(mstr &m, TGroup *group)
{
	print_exec_list_sendrecv(m, outReg->receiver, outReg->receiverType);
	outReg->returned.Print(m, "\vВърнат на %Dг.");
}

#undef outReg

const offield TFloatExecListSum::fields2[] =
{
	{ "F_LEFT",		outreg_send_left },
	{ "F_RIGHT",	outreg_send_right },
	{ NULL, NULL }
};

// ----- TFloatExecListQuery -------------------------------------------------
class TFloatExecListQuery : public TFloatQuery
{
public:
	TFloatExecListQuery(TWindow *parent, const char *criteria, const char *name, TFloatExecListGroup *tGroup, sign tQuery);

	void Execute();

	TFloatExecListGroup *group;
	TFloatExecListTotalArray sumTotal;
	TFloatExecListTotalArray collectTotal;

protected:
	sign query;
	static const offield fields[];

	const char *tr1execlistno;
	const char *tr1sumtype;
	const char *tr1debtors;
	const char *tr1collectors;
	const char *tr1execlistdate;
	const char *tb2start;
	const char *tr2upper;
	const char *tr2alone;
	const char *tr2lower;
	const char *tr2final;
	const char *tb2final;
	const char *tr1nothing;

	virtual void Count();
	virtual void GetMarkers();
};

TFloatExecListQuery::TFloatExecListQuery(TWindow *parent, const char *criteria, const char *name,
	TFloatExecListGroup *tGroup, sign tQuery)
	: TFloatQuery(parent, criteria, name), group(tGroup), query(tQuery)
{
	SetNames("Няма изпълнителни листи", "Един изпълнителен лист", "изпълнителни листа");
}

void TFloatExecListQuery::Count()
{
	TGArray<TFloatExecListSum> sums;
	TFloatExecListSum *lastExecList = NULL;
	TFloatExecListSum *lastPersons = NULL;
	TFloatExecListSum *lastTargets = NULL;
	TExecListPerson person;
	TQuery q(criteria);

	while (q.Read() && Increment())
	{
		TFloatExecListSum *sum = new TFloatExecListSum;

		*sum << q;
		sum->voluntary = group->voluntary;
		person << sum;
		person.LoadFloat(sum->persons);

		if (lastExecList && sum->SameList(lastExecList))
		{
			sum->subpoenas = lastExecList->subpoenas;
			sum->lawsuit = lastExecList->lawsuit;
			sum->event = lastExecList->event;
			sum->outRegs = lastExecList->outRegs;

			if (!sum->SamePersons(lastPersons))
				lastPersons = lastTargets = sum;
			else if (strcmp(sum->sender, lastTargets->sender))
				lastTargets = sum;
		}
		else
		{
			sum->subpoenas = new TGArray<TSubpoena>;
			sum->lawsuit = new TLawsuit;
			sum->outRegs = new TGArray<TOutReg>;

			msql m("T_SUBPOENA");

			m.AddLong("F_NO", sum->no);
			m.AddLong("F_YEAR", sum->year);
			m.Add(sum, "F_TYPE");
			m.AddDate("F_SUBPOENA_DATE", sum->execListDate);
			m.Add(sum, "F_EXEC_LIST_NO, F_EXEC_LIST_KIND");

			TQuery r(str(m));
			while (r.Read())
			{
				TSubpoena *subpoena = new TSubpoena;

				*subpoena << sum;
				*subpoena << r;

				if (!sum->subpoenas->Count())
				{
					bool session = strchr(KIND_SESSIONS, subpoena->kind);

					sum->event = session ? (TIndexContainer *) new TSession : (TIndexContainer *) new TDecision;
					*sum->event << subpoena;
					sum->lawsuit->key = subpoena->key;

					if (!sum->event->Get(session ? "F_EVENT_NO, F_FORCE_DATE" : "F_EVENT_NO, F_FROM_DATE, "
						"F_FORCE_DATE") ||
						!sum->lawsuit->Get("F_KIND, F_COMPOSITION") ||
						(query && (!strchr(group->lawKinds, sum->lawsuit->kind) ||
						!strchr(group->compositions, sum->lawsuit->composition))))
					{
						delete sum;
						sum = NULL;
						break;
					}

					TSubpoena tSubpoena;

					m.Begin(tSubpoena.Name, "F_OUTREG_NO, F_OUTREG_YEAR, F_LRECEIVER_UCN, F_LRECEIVER_UCN_TYPE");
					m.AddLong("F_NO", sum->no);
					m.AddLong("F_YEAR", sum->year);
					m.Add(sum, "F_TYPE");
					m.Add(subpoena, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_EVENT_STATUS");
					m.AddChars("F_SUBPOENA_KIND", SUBPOENA_KIND_EXECBOOK_LETS);
					m.AddLong("F_REGARD", subpoena->subpoenaNo);
					m.AddIsNull("F_OUTREG_NO", false);

					TQuery q(str(m));
					while (q.Read())
					{
						TOutReg *outReg = new TOutReg;

						tSubpoena << q;
						outReg->key.no = tSubpoena.outRegNo;
						outReg->key.year = tSubpoena.outRegYear;
						outReg->date.Clear();
						outReg->Try(sum->execListKind == EXEC_LIST_COUNTRY ? "F_DATE, F_RETURNED" : "F_DATE");
						strcpy(outReg->receiver, tSubpoena.lReceiverUcn);
						outReg->receiverType = tSubpoena.lReceiverUcnType;
						sum->outRegs->Add(outReg);
					}
				}

				sum->subpoenas->Add(subpoena);

				if (sum->execListKind == EXEC_LIST_COUNTRY)
					break;
			}

			if (sum)
			{
				if (sum->subpoenas->Count())
					lastExecList = lastPersons = lastTargets = sum;
				else
				{
					say_no_data("T_SUBPOENA");
					delete sum;
					sum = NULL;
				}
			}
		}

		if (sum)
		{
			sums.Add(sum);

			if (sum->sumType && strchr(SUM_TYPE_VALUES, sum->sumType))
				sumTotal.AddSum(sum);

			lastExecList->execListCount++;
			lastPersons->personCount++;
			lastTargets->targetCount++;
		}
	}

	count = 0;

	for (int i = 0; i < sums.Count(); i++)
	{
		TFloatExecListSum *sum = sums[i];
		bool mainCount = sum->execListCount || (query && sum->personCount);

		office.Emit(tr1start, tr1execlistno, TFloatExecListSum::fields, sum);

		if (mainCount)
		{
			office.Emit(tr1execlistno, tr1sumtype, TFloatExecListSum::fields, sum);
			count++;
		}

		office.Emit(tr1sumtype, tr1debtors, TFloatExecListSum::fields, sum);

		if (sum->targetCount)
		{
			if (sum->personCount)
				office.Emit(tr1debtors, tr1collectors, TFloatExecListSum::fields, sum);

			office.Emit(tr1collectors, tr1execlistdate, TFloatExecListSum::fields, sum);

			if (mainCount)
			{
				if (query != -1)
				{
					sum->LoadCollects();

					for (int i = 0; i < sum->collects.Count(); i++)
					{
						TExecListCollect *tCollect = sum->collects[i];

						if (tCollect->sum != 0.0)
							collectTotal.AddSum(tCollect);
					}
				}

				if (query)
					office.Emit(tr1execlistdate, tr1nothing, TFloatExecListSum::fields, sum);
				else
				{
					TSubpoena *subpoena = (*sum->subpoenas)[0];
					bool received = !subpoena->subpoenaReceived.Empty() || any(subpoena->receivedText) ||
						!subpoena->subpoenaReturned.Empty() || any(sum->tdepInNo);

					office.Emit(tr1execlistdate, tb2start, TFloatExecListSum::fields, sum);

					if (sum->outRegs->Count() || received)
					{
						office.Emit(tb2start, tr2upper, null_fields, NULL);

						for (int i = 0; i < sum->outRegs->Count(); i++)
						{
							TOutReg *outReg = (*sum->outRegs)[i];

							if (i < sum->outRegs->Count() - 1 || received)
								office.Emit(tr2upper, tr2alone, TFloatExecListSum::fields2, outReg);
							else if (i == 0)
								office.Emit(tr2alone, tr2lower, TFloatExecListSum::fields2, outReg);
							else
								office.Emit(tr2lower, tr2final, TFloatExecListSum::fields2, outReg);
						}

						if (received)
						{
							if (sum->outRegs->Count())
								office.Emit(tr2lower, tr2final, TFloatExecListSum::fields, sum);
							else
								office.Emit(tr2alone, tr2lower, TFloatExecListSum::fields, sum);
						}

						office.Emit(tr2final, tb2final, null_fields, NULL);
					}
					else
					{
						office.Emit(tb2start, tr2upper, null_fields, NULL);
						office.Emit(tr2alone, tr2lower, null_fields, NULL);
						//office.Emit(tr2alone, tr2lower, null_fields, NULL);		// -> margin 1cm/1cm
						office.Emit(tr2final, tb2final, null_fields, NULL);
					}

					office.Emit(tb2final, tr1nothing, TFloatExecListSum::fields, sum);
				}
			}
		}

		office.Emit(tr1nothing, tr1final, null_fields, NULL);
	}
}

void TFloatExecListQuery::Execute()
{
	TWindowHideParent hide(parent);

	if (query)
	{
		if (office.Open(name))
		{
			GetMarkers();
			office.Emit(NULL, tr1start, fields, this);
			Count();
			office.Emit(tr1final, NULL, fields, this);
			Show();
		}
	}
	else if (office.Open(name, OFFICE_COPY_HEADER))
	{
		office.Emit(NULL, NULL, fields, this);
		office.Close();

		if (office.Open(name, OFFICE_SKIP_HEADER, ""))
		{
			GetMarkers();
			office.Emit(NULL, tr1start, null_fields, NULL);
			Count();
			office.Emit(tr1final, NULL, fields, this);
			Show();
		}
	}
}

void TFloatExecListQuery::GetMarkers()
{
	tr1start = office.Find("tr1 start");
	tr1execlistno = office.Find("tr1 execlistno");
	tr1sumtype = office.Find("tr1 sumtype");
	tr1debtors = office.Find("tr1 debtors");
	tr1collectors = office.Find("tr1 collectors");
	tr1execlistdate = office.Find("tr1 execlistdate");

	if (!query)
	{
		tb2start = office.Find("tb2 start");
		tr2upper = office.Find("tr2 upper");
		tr2alone = office.Find("tr2 alone");
		tr2lower = office.Find("tr2 lower");
		tr2final = office.Find("tr2 final");
		tb2final = office.Find("tb2 final");
	}

	tr1nothing = office.Find("tr1 nothing");
	tr1final = office.Find("tr1 final");
}

#define execlist OFFICE_GROUP_CAST(TFloatExecListQuery)

static void execlist_types(mstr &m, TGroup *group) { print_types_list(m, execlist->group->types, false); }

static void execlist_voluntary(mstr &m, TGroup *group)
{
	switch (execlist->group->voluntary)
	{
		case 0 : m.cat(" - Принудително изпълнение"); break;
		case EXEC_LIST_COLLECT_VOLUNTARY : m.cat(" - Доброволно изпълнение");
	}
}

static void execlist_sum_total(mstr &m, TGroup *group) { execlist->sumTotal.Print(m, "\v"); }
static void execlist_collect_total(mstr &m, TGroup *group) { execlist->collectTotal.Print(m, "\v"); }

#undef execlist

const offield TFloatExecListQuery::fields[] =
{
	{ "F_TYPES",		execlist_types },
	{ "F_VOLUNTARY",		execlist_voluntary },
	{ "F_SUM_TOTAL",		execlist_sum_total },
	{ "F_COUNT",		floatquery_count },
	{ "F_COLLECT_TOTAL",	execlist_collect_total },
	{ NULL, NULL }
};

// ----- float_exec_list -----------------------------------------------------
static void float_exec_list(TWindow *parent, const char *criteria, TFloatExecListGroup *group)
{
	sign query = group->minCollectDate.Empty() && group->maxCollectDate.Empty() ? -1 : true;
	TFloatExecListQuery(parent, criteria, query == -1 ? "FloatExLst.htm" : "FltExLst_.htm", group, true).Execute();
}

#include "AccountListBox.h"
#include "AccountListBox.cpp"

#include "FloatExecListDialog.h"
#include "FloatExecListDialog.cpp"

void float_exec_list(TWindow *parent)
{
	TFloatExecListGroup group;
	TFloatExecListDialog(parent, &group).Execute();
}

// ----- exec_list_book ------------------------------------------------------
#include "ExecListBookDialog.h"
#include "ExecListBookDialog.cpp"

void exec_list_book(TWindow *parent)
{
	TFloatExecListGroup group;

	strcpy(group.lawKinds, KIND_LAWSUITS);
	strcpy(group.compositions, COMPOSITION_ALLS);
	group.minExecListDate = Today;
	group.maxExecListDate = Today;

	if (TExecListBookDialog(parent, &group).Execute() == IDOK)
	{
		msql m;
		group.FormCriteria(m);
		TFloatExecListQuery(parent, str(m), "ExecListBk.htm", &group, false).Execute();
	}
}
