#include "cac.h"

enum
{
	NIA_PROTOCOL_BY_EXEC_DATE,
	NIA_PROTOCOL_BY_OUTREG_DATE
};

// ----- TPrepareNIAProtocolGroup --------------------------------------------
class TPrepareNIAProtocolGroup : public TFloatGroup
{
public:
	TPrepareNIAProtocolGroup();

	virtual void FormCriteria(msql &m);
	void AddUCLP(msql &m, const char *par_pref);

	char niaTDep[SIZE_OF_UCN];
	char types[SIZE_OF_TYPES];
	long byDate;
	CDate minExecDate;
	CDate maxExecDate;
	CDate minOutRegDate;
	CDate maxOutRegDate;
};

TPrepareNIAProtocolGroup::TPrepareNIAProtocolGroup()
	// 2016:070 LEFT JOIN L/O, ignore byDate, moved here from FormCriteria
	: TFloatGroup("T_EXEC_LIST E "
		"JOIN T_SUBPOENA S ON S.F_NO = E.F_NO AND S.F_YEAR = E.F_YEAR AND S.F_TYPE = E.F_TYPE "
			"AND S.F_EXEC_LIST_NO = E.F_EXEC_LIST_NO AND S.F_SUBPOENA_DATE = E.F_EXEC_LIST_DATE "
			"AND S.F_EXEC_LIST_KIND = E.F_EXEC_LIST_KIND "
		"JOIN T_EXEC_LIST_SUM T ON T.F_EXEC_LIST_NO = E.F_EXEC_LIST_NO AND T.F_EXEC_LIST_YEAR = E.F_EXEC_LIST_YEAR "
			"AND T.F_TYPE = E.F_TYPE AND T.F_EXEC_LIST_KIND = E.F_EXEC_LIST_KIND "
		"JOIN T_ACCOUNT A ON A.F_UCN = T.F_SENDER "
		"LEFT JOIN T_UCLP US ON US.F_UCLP = S.F_UCLP "
		"LEFT JOIN T_SUBPOENA L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE "
			"AND L.F_KIND = S.F_KIND AND L.F_DATE = S.F_DATE AND L.F_EVENT_STATUS = S.F_EVENT_STATUS "
			"AND L.F_REGARD = S.F_SUBPOENA_NO "
		"LEFT JOIN T_UCLP UL ON UL.F_UCLP = L.F_UCLP "	// 2016:077 +related
		"LEFT JOIN T_OUTREG O ON O.F_NO = L.F_OUTREG_NO AND O.F_YEAR = L.F_OUTREG_YEAR",
		"DISTINCT E.*")
{
	Add(
		new TStringAlias("F_NIA_TDEP", niaTDep, sizeof niaTDep),
		new TCharList("F_TYPES", types, sizeof types),
		new TRadio("F_BY_EXEC_DATE", &byDate, NIA_PROTOCOL_BY_EXEC_DATE),
		new TRadio("F_BY_OUTREG_DATE", &byDate, NIA_PROTOCOL_BY_OUTREG_DATE),
		new TDate("F_MIN_EXEC_DATE", &minExecDate, false),
		new TDate("F_MAX_EXEC_DATE", &maxExecDate, false),
		new TDate("F_MIN_OUTREG_DATE", &minOutRegDate, false),
		new TDate("F_MAX_OUTREG_DATE", &maxOutRegDate, false),
		NULL
	);
}

void TPrepareNIAProtocolGroup::AddUCLP(msql &m, const char *par_pref)
{
	bool niaTDepForeign = !strcmp(niaTDep, SENDER_NIA_TDEP_FOREIGN);

	m.AddLong(par_pref + !niaTDepForeign, atol(niaTDep) / SENDER_NIA_TDEP_MULT);

	if (niaTDepForeign)
	{
		m.NextOr();
		m.AddIsNull(par_pref + 1, true);
		m.cat(")");
	}
}

void TPrepareNIAProtocolGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	if (byDate == NIA_PROTOCOL_BY_EXEC_DATE)
	{
		m.Add("E.F_EXEC_LIST_YEAR");
		m.printf(">= %ld", minExecDate.year);
		m.AddRange("E.F_EXEC_LIST_DATE", minExecDate, maxExecDate);
	}
	else	// 2016:015 FIX: don't check exec list year >= outreg date
		m.AddRange("O.F_DATE", minOutRegDate, maxOutRegDate);

	m.AddIsNull("(L.F_SUBPOENA_KIND", true);
	m.NextOr();
	m.AddChars("NOT L.F_SUBPOENA_KIND", SUBPOENA_KIND_NONTDEP_EXECLETS);
	m.cat(")");

	m.AddLong("E.F_EXEC_LIST_KIND", EXEC_LIST_COUNTRY);
	m.AddChars("E.F_TYPE", types);
	m.AddIsNull("E.F_NIA_NO", true);

	m.AddIsNull("((L.F_KIND", true);
	AddUCLP(m, "(US.F_AREA");
	m.cat(")");
	m.NextOr();
	m.AddIsNull("(NOT L.F_KIND", true);
	AddUCLP(m, "(UL.F_AREA");
	m.cat("))");

	// 2015:161 IRQ: only lists with at least 1 judicial sum
	m.AddChar("A.F_UCN_TYPE", UCN_EXEC_LIST);
	m.AddFlags("A.F_FLAGS", ACCOUNT_JUDICIAL, ACCOUNT_JUDICIAL, ACCOUNT_ALL);
}

// ----- TExecDebtor ---------------------------------------------------------
class TExecDebtor : public TSideWind
{
public:
	TExecDebtor(const char *tUCN, char tUCNType);

	mstr name;
};

TExecDebtor::TExecDebtor(const char *tUCN, char tUCNType)
{
	strcpy(ucn, tUCN);
	ucnType = tUCNType;
	print_name(name, ucn, ucnType);
}

// ----- TPrintExecList ------------------------------------------------------
enum { RECEIVERS_TDNAP = 1, RECEIVERS_PRIVEX = 2, RECEIVERS_COUNTEX = 4 };

class TPrintExecList : public TExecList
{
public:
	TPrintExecList(TQuery &q);

	void CheckSend() { sent = TExecList::CheckSend(&outReg); }
	void LoadCollects();
	virtual void Print(mstr &m);

	TGArray<TSubpoena> subpoenas;
	mstr exltext;
	mstr debtext;
	mstr debtext1;
	const char *debtext2;

	TOutReg outReg;
	TGArray<TExecListCollect> collects;
	bool sent;

	// 2016:070 +related
	TGArray<TReceiver> receivers;
	int receiverTypes;
	TGArray<TExecDebtor> debtors;

protected:
	bool allTDNap;

	void AddSender(TReceiver *receiver);
	void AddReceiver(char ucnType, const char *ucn);
};

TPrintExecList::TPrintExecList(TQuery &q)
	: sent(false), receiverTypes(0), allTDNap(false)
{
	*this << q;
	exltext.printf("ИЛ %ld/%ld %ld", execListNo, execListYear, type);

	TSubpoena subpoena;
	char letterReceiverUCNType;
	char letterReceiverUCN[SIZE_OF_UCN];
	char letterSubpoenaKind;

	subpoena.Add(
		new TChar("F_LETTER_RECEIVER_UCN_TYPE", &letterReceiverUCNType),
		new TString("F_LETTER_RECEIVER_UCN", letterReceiverUCN, sizeof letterReceiverUCN),
		new TChar("F_LETTER_SUBPOENA_KIND", &letterSubpoenaKind),
		NULL
	);

	msql m("T_SUBPOENA S "
		"LEFT JOIN T_SUBPOENA L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND L.F_TYPE = S.F_TYPE "
			"AND L.F_KIND = S.F_KIND AND L.F_DATE = S.F_DATE AND L.F_EVENT_STATUS = S.F_EVENT_STATUS "
			"AND L.F_REGARD = S.F_SUBPOENA_NO",
		"S.*, L.F_LRECEIVER_UCN_TYPE AS F_LETTER_RECEIVER_UCN_TYPE, L.F_LRECEIVER_UCN AS F_LETTER_RECEIVER_UCN, "
			"L.F_SUBPOENA_KIND AS F_LETTER_SUBPOENA_KIND");

	m.AddLong("S.F_NO", no);
	m.AddLong("S.F_YEAR", year);
	m.AddChar("S.F_TYPE", type);
	m.AddLong("S.F_EXEC_LIST_NO", execListNo);
	m.AddLong("S.F_SUBPOENA_DATE", execListDate.Value());
	m.AddLong("S.F_EXEC_LIST_KIND", execListKind);

	TQuery r(str(m));
	while (r.Read())
	{
		subpoena << r;			// for letter* above
		int i;

		for (i = 0; i < debtors.Count(); i++)
			if (!strcmp(debtors[i]->ucn, subpoena.ucn) && debtors[i]->ucnType == subpoena.ucnType)
				break;

		if (i == debtors.Count())
		{
			TExecDebtor *debtor = new TExecDebtor(subpoena.ucn, subpoena.ucnType);

			debtext.sep(", ");
			debtext.cat(debtor->name);
			debtors.Add(debtor);
		}

		subpoenas.AddCopy(&subpoena);
		bool addTDep = letterReceiverUCNType == UCN_NIA_TDEP;

		if (!letterSubpoenaKind || (letterSubpoenaKind == SUBPOENA_KIND_EXECLET_NIATDEP && !addTDep))
		{
			TReceiver receiver;

			m.Begin(receiver.Name);
			m.AddChar("F_UCN_TYPE", UCN_NIA_TDEP);
			receiver.LoadArray(&receivers, str(m));
			receiverTypes |= RECEIVERS_TDNAP;
			allTDNap = true;
		}
		else if (letterReceiverUCNType && (!addTDep || !allTDNap))
			AddReceiver(letterReceiverUCNType, letterReceiverUCN);
	}

	if (!subpoenas.Count())
		fatal("%s: няма призовки/съобщения\n", str(exltext));

	debtext1.cpy(debtext);
	debtext2 = split_half(ncstr(debtext1));

	if (any(tdep))
		AddReceiver(tdepUCNType, tdep);
}

void TPrintExecList::AddSender(TReceiver *receiver)
{
	TSender sender;
	sender << receiver;

	if (sender.Get())
		strcpy(receiver->name, sender.name);
}

void TPrintExecList::AddReceiver(char ucnType, const char *ucn)
{
	for (int i = 0; i < receivers.Count(); i++)
	{
		TReceiver *tReceiver = receivers[i];

		if (tReceiver->ucnType == ucnType && !strcmp(tReceiver->ucn, ucn))
			return;
	}

	TReceiver *receiver = new TReceiver;

	receiver->ucnType = ucnType;
	strcpy(receiver->ucn, ucn);
	receivers.Add(receiver);

	if (ucnType == UCN_NIA_TDEP)
	{
		AddSender(receiver);
		receiverTypes |= RECEIVERS_TDNAP;
	}
	else if (ucnType == UCN_COUNTEX)
	{
		AddSender(receiver);
		receiverTypes |= RECEIVERS_COUNTEX;
	}
	else
	{
		print_user(receiver->name, ucn, ucnType, false);
		receiverTypes |= RECEIVERS_PRIVEX;
	}
}

void TPrintExecList::LoadCollects()
{
	TExecListCollect collect;
	collect << this;
	collect.LoadFloat(&collects);
}

void TPrintExecList::Print(mstr &m)
{
	Type->Seek(type);
	m.printf("%s %5ld/%4ld ", Type->alias, no, year);
	if (PRINT_EXEC_LIST_NO)
		m.printf("%6ld/", execListNo);
	m.printf("%0D  %s", &execListDate, str(debtext));
}

// ----- TPrintNIAProtocol ---------------------------------------------------
class TPrintNIAProtocol : public TProcess
{
public:
	TPrintNIAProtocol(TWindow *parent, TNIAProtocol *tProtocol, bool tSync);

	void Execute();

	TOffice office;
	TNIAProtocol *protocol;
	TPrintExecList *execList;
	TIndexContainer *container;
	TExecListSum *start;
	typedef TGArray<TExecListSum> TSumGroup;
	TIArray<TSumGroup> sumGroups;
	long subcount1;
	TGArray<TExecListSum> *sums;
	TExecListPerson *person;

protected:
	bool sync;

	const char *tr1start;
	const char *tr1autogen;
	const char *tr1debtor;
	const char *tr1event;
	const char *tr1sums;
	const char *tr1nothing;
	const char *tr1final;

	static const offield fields[];

	void PrintExecList();
};

TPrintNIAProtocol::TPrintNIAProtocol(TWindow *parent, TNIAProtocol *tProtocol, bool tSync)
	: TProcess(parent, "протокола", tSync ? WAIT_MORE : WAIT), protocol(tProtocol), sync(tSync)
{
}

void TPrintNIAProtocol::Execute()
{
	if (office.Open("ProtExList.htm"))
	{
		tr1start = office.Find("tr1 start");
		tr1autogen = office.Find("tr1 autogen");
		tr1debtor = office.Find("tr1 debtor");
		tr1event = office.Find("tr1 event");
		tr1sums = office.Find("tr1 sums");
		tr1nothing = office.Find("tr1 nothing");
		tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, fields, this);

		for (int i = 0; i < protocol->execLists.Count(); i++)
		{
			execList = TYPECHECK_PTR_CAST(TPrintExecList, protocol->execLists[i]);
			PrintExecList();
		}

		office.Emit(tr1final, NULL, fields, this);
		office.ShowSync(wait, sync, "Протоколът ще бъде отворен", "Протоколът е записан");
		wait->ResetForeground();
		Debug();
	}
}

void TPrintNIAProtocol::PrintExecList()
{
	TSubpoena subpoena;
	msql m(subpoena.Name);

	container = TYPECHECK_PTR_CAST(TIndexContainer, kind2DatedContainer(execList->subpoenas[0]->kind));
	*container << execList->subpoenas[0];

	if (!container->Get())
		dprintf("%s: събитието не се чете\n", str(execList->exltext));
	else
	{
		load_exec_list_data(execList);

		if (!execList->sums.Count())
			dprintf("%s: няма задължения\n", str(execList->exltext));
		else
		{
			bool *included = new bool[execList->sums.Count()];
			TBoolPtr BP(included);

			memset(included, '\0', execList->sums.Count() * sizeof(bool));
			subcount1 = 0;

			for (int i = 0; i < execList->sums.Count(); i++)
			{
				if (!included[i])
				{
					TSumGroup *tSumGroup = new TSumGroup;
					start = execList->sums[i];
					subcount1 += start->persons.Count();

					for (int n = i; n < execList->sums.Count(); n++)
					{
						if (execList->sums[n]->SamePersons(start))
						{
							tSumGroup->Add(execList->sums[n]);
							included[n] = true;
						}
					}

					sumGroups.Add(tSumGroup);
				}
			}

			for (int i = 0; i < sumGroups.Count(); i++)
			{
				sums = sumGroups[i];
				start = (*sums)[0];

				for (int n = 0; n < start->persons.Count(); n++)
				{
					office.Emit(tr1start, tr1autogen, null_fields, NULL);
					person = start->persons[n];

					if (n == 0)
						office.Emit(tr1autogen, tr1debtor, fields, this);

					office.Emit(tr1debtor, tr1event, fields, this);

					if (n == 0)
					{
						if (i == 0)
							office.Emit(tr1event, tr1sums, fields, this);

						office.Emit(tr1sums, tr1nothing, fields, this);
					}

					office.Emit(tr1nothing, tr1final, fields, this);
				}

				sums->Clear();
			}

			sumGroups.Flush();
		}
	}

	delete container;
}

#define niaprot OFFICE_GROUP_CAST(TNIAProtocol)

static void niaprot_nia_tdep(mstr &m, TGroup *group) { print_sender(m, UCN_NIA_TDEP, niaprot->niaTDep); }

#undef niaprot

#define printnia OFFICE_GROUP_CAST(TPrintNIAProtocol)

static void printnia_nia_out_no(mstr &m, TGroup *group) { itom(printnia->protocol->outNo, m); }
static void printnia_nia_out_year(mstr &m, TGroup *group) { itom(printnia->protocol->outYear, m); }
static void printnia_nia_no(mstr &m, TGroup *group) { itom(printnia->protocol->nia.no, m); }
static void printnia_nia_year(mstr &m, TGroup *group) { itom(printnia->protocol->nia.year, m); }
static void printnia_nia_tdep(mstr &m, TGroup *group) { niaprot_nia_tdep(m, printnia->protocol); }
static void printnia_debtor(mstr &m, TGroup *group) { printnia->person->Print(m); }

static void printnia_code(mstr &m, TGroup *group)
{
	TExecListPerson *person = printnia->person;

	if (person->ucnType == UCN_CITIZEN_UCN)
		m.printf("ЕГН %s", person->ucn);
	else if (strchr(UCN_FIRMS, person->ucnType))
	{
		TFirm firm;

		firm.ucnType = person->ucnType;
		strcpy(firm.ucn, person->ucn);

		if (firm.Get("F_BULSTAT") && any(firm.bulstat))
			m.printf("ЕИК/БУЛСТАТ %s", firm.bulstat);
	}
}

static void printnia_debtor_address(mstr &m, TGroup *group)
{
	TExecListPerson *person = printnia->person;

	for (int i = 0; i < printnia->execList->subpoenas.Count(); i++)
	{
		TSubpoena *subpoena = printnia->execList->subpoenas[i];

		if (subpoena->ucnType == person->ucnType && !strcmp(subpoena->ucn, person->ucn))
		{
			subpoena->PrintAddress(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
			return;
		}
	}

	TAddress address;
	address << person;

	if (address.Try())
		address.Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
}

static void printnia_subcount_1(mstr &m, TGroup *group)
{
	itom(printnia->subcount1, m);
}

static void printnia_lawsuit_kind(mstr &m, TGroup *group) { print_type_or_kind(m, printnia->container->key); }
static void printnia_lawsuit_no(mstr &m, TGroup *group) { itom(printnia->container->key.no, m); }
static void printnia_lawsuit_year(mstr &m, TGroup *group) { itom(printnia->container->key.year, m); }

static void printnia_composition(mstr &m, TGroup *group)
{
	// 2015:117 IRQ: endoc support
	if (strchr(KIND_ENDOCS, printnia->container->kind))
	{
		TSession session;

		session.key = printnia->container->key;
		session.date = printnia->container->date;
		session.kind = TYPECHECK_PTR_CAST(TDecision, printnia->container)->sessionKind;

		if (session.Try("F_COMPOSITION"))
			print_composition(m, session.composition);
	}
	else if (strchr(KIND_SESSIONS, printnia->container->kind))
		print_composition(m, TYPECHECK_PTR_CAST(TSession, printnia->container)->composition);
}

static void printnia_event_kind(mstr &m, TGroup *group)
{
	if (printnia->container->IndexType())
		m.cat(Kind->Seek(printnia->container->IndexKind()));
	else
		m.cat(Kind->Seek(kind2VisibleKind(printnia->container->kind)));
}

static void printnia_event_no(mstr &m, TGroup *group) { container_event_no(m, printnia->container); }
static void printnia_event_date(mstr &m, TGroup *group) { container_event_date(m, printnia->container); }

static void printnia_force_date(mstr &m, TGroup *group)
{
	TDate *date = TYPECHECK_PTR_CAST(TDate, printnia->container->Find("F_FORCE_DATE"));

	if (!date->Date().Empty())
		m.printf("В законна сила от %D", &date->Date());
}

static void printnia_subcount_2(mstr &m, TGroup *group) { itom(printnia->start->persons.Count(), m); }

static void printnia_sums(mstr &m, TGroup *group)
{
	for (int i = 0; i < printnia->sums->Count(); i++)
	{
		TExecListSum *execsum = (*printnia->sums)[i];

		m.sep("\v");

		if (strchr(SUM_TYPE_VALUES, execsum->sumType))
		{
			Currency->Seek(execsum->currency);
			m.printf("%.2lf %s", execsum->sum, Currency->numeric);
		}
		else
		{
			if (!strchr(SUM_TYPE_OTHERS, execsum->sumType) || !any(execsum->sumText))
			{
				SumType->Seek(execsum->sumType);
				m.cat(SumType->alias);
			}

			if (any(execsum->sumText))
			{
				m.sep(" ");
				m.cat(execsum->sumText);
			}
		}
	}
}

#undef printnia

const offield TPrintNIAProtocol::fields[] =
{
	{ "F_OUT_NO",		printnia_nia_out_no },
	{ "F_OUT_YEAR",		printnia_nia_out_year },
	{ "F_NO",			printnia_nia_no },
	{ "F_YEAR",			printnia_nia_year },
	{ "F_SERV_TAX",		printnia_nia_tdep },
	{ "F_DEBTOR",		printnia_debtor },
	{ "F_CODE",			printnia_code },
	{ "F_DEBTOR_ADDRESS",	printnia_debtor_address },
	{ "F_SUBCOUNT_1",		printnia_subcount_1 },
	{ "F_LAWSUIT_KIND",	printnia_lawsuit_kind },
	{ "F_LAWSUIT_NO",		printnia_lawsuit_no },
	{ "F_LAWSUIT_YEAR",	printnia_lawsuit_year },
	{ "F_COMPOSITION",	printnia_composition },
	{ "F_EVENT_KIND",		printnia_event_kind },
	{ "F_EVENT_NO",		printnia_event_no },
	{ "F_EVENT_DATE",		printnia_event_date },
	{ "F_FORCE_DATE",		printnia_force_date },
	{ "F_SUBCOUNT_2",		printnia_subcount_2 },
	{ "F_SUMS",			printnia_sums },
	{ NULL, NULL }
};

#include "SelectExecListListBox.h"
#include "SelectExecListListBox.cpp"

#include "PerformNIAProtocolDialog.h"
#include "PerformNIAProtocolDialog.cpp"

static void write_protocol(TWindow *parent, TNIAProtocol *protocol)
{
	TOutReg outReg;
	msql m;

	if (NIA_PROT_OUT_LETTER)
	{
		TSender sender;

		sender.ucnType = UCN_NIA_TDEP;
		strcpy(sender.ucn, protocol->niaTDep);

		if (!sender.Get())
			return;

		TFirm firm;

		m.Begin(firm.Name);
		m.Add(&sender, "F_NAME");
		TQuery q(str(m));

		if (q.Read())
			firm << q;
		else
		{
			firm.ucnType = UCN_FIRM_CODE;
			strcpy(firm.name, sender.name);

			if (!firm.Insert())
				return;

			TAddress address;

			address.ucnType = sender.ucnType;
			strcpy(address.ucn, sender.ucn);

			if (address.Try())
			{
				address.ucnType = firm.ucnType;
				strcpy(address.ucn, firm.ucn);
				address.Insert();
			}
		}

		outReg.kind = KIND_OUT_LETTER;
		outReg.date = Today;
		outReg.text.cat("Към протокол за предаване на ИЛ в НАП");
		outReg.resultII = ACCOMPLY_ACCOMPANY_LETTER;
		outReg.resultKS = ACCOMPLY_ACCOMPANY_LETTER;
		outReg.resultMJ = ACCOMPLY_ACCOMPANY_LETTER;
		outReg.college = protocol->nia.type;

		if (!outReg.Insert())
			return;

		log(LOG_SEND, 0, &outReg, "на протокол в НАП");
		protocol->outNo = outReg.key.no;
		protocol->outYear = outReg.key.year;

		TSideWind tSideWind;

		tSideWind.key = outReg.key;
		tSideWind.kind = KIND_OUT_LETTER;
		tSideWind.date = outReg.date;
		tSideWind.ucnType = firm.ucnType;
		strcpy(tSideWind.ucn, firm.ucn);
		tSideWind.involvement = INVOLVEMENT_RECEIVER;
		tSideWind.Insert();	// outreg created, can't return
	}

	if (!protocol->Insert())
		return;

	m.clear();
	m.printf("протокол %ld/%ld предаване ИЛ в НАП", protocol->nia.no, protocol->nia.year);
	log(LOG_WRITE, protocol->nia.type, str(m));

	TPrintNIAProtocol printnia(parent, protocol, NIA_PROT_OUT_LETTER);
	printnia.Execute();

	if (NIA_PROT_OUT_LETTER && any(Electrics[TYPE_TEXT].docketPath))
	{
		char name[SIZE_OF_PATH];
		electric_name(&outReg, Electrics[TYPE_TEXT].docketPath, name, TYPE_TEXT);
		copy_overead_file(printnia.office.Name, name, false, true);
	}

	for (int i = 0; i < protocol->execLists.Count(); i++)
	{
		TExecList *execList = protocol->execLists[i];

		execList->nia = protocol->nia;
		strcpy(execList->tdep, protocol->niaTDep);
		execList->tdepUCNType = UCN_NIA_TDEP;	// 2016:078 LPR: was missing
		execList->Update();
	}
}

bool nia_protocol(TWindow *parent, TPrepareNIAProtocolGroup *group)
{
	TNIAProtocol protocol;

	{
		TWaitWindow wait(parent, WAIT);
		msql m;

		group->FormCriteria(m);
		TQuery q(str(m));

		while (q.Read())
			protocol.execLists.Add(new TPrintExecList(q));
	}

	if (protocol.execLists.Count())
	{
		parent->ShowWindow(SW_HIDE);

		protocol.nia.type = NIA_PROT_NUMBERING ? *group->types : TYPE_COMMON_ADM;
		protocol.nia.year = Today.year;
		strcpy(protocol.niaTDep, group->niaTDep);

		switch (TPerformNIAProtocolDialog(parent, &protocol).Execute())
		{
			case IDPRINT :
			{
				TPrintNIAProtocol(parent, &protocol, false).Execute();
				break;
			}
			case IDENTER : write_protocol(parent, &protocol);
		}

		return true;
	}

	return error("Няма изпълнителни листи отговарящи на зададените критерии.");
}

#include "PrepareNIAProtocolDialog.h"
#include "PrepareNIAProtocolDialog.cpp"

void nia_prepare(TWindow *parent)
{
	TPrepareNIAProtocolGroup group;
	TPrepareNIAProtocolDialog(parent, &group).Execute();
}

#define execList OFFICE_GROUP_CAST(TPrintExecList)

static void execlist_lawsuit(mstr &m, TGroup *group)
{
	Type->Seek(execList->type);
	m.printf("%s %ld/%ld", Type->alias, execList->no, execList->year);
}

static void execlist_exnodate(mstr &m, TGroup *group)
{
	if (PRINT_EXEC_LIST_NO)
		m.printf("%ld/", execList->execListNo);
	m.printf("%0D", &execList->execListDate);
}

static void execlist_debtors_line_1(mstr &m, TGroup *group) { m.cat(str(execList->debtext1)); }

#undef execList

#define collect OFFICE_GROUP_CAST(TExecListCollect)

static void collect_names(mstr &m, TGroup *group) { print_name(m, collect->ucn, collect->ucnType); }

#undef collect

void print_exec_list_set_headers(TPrintExecList *execList, TStatic *header1, TStatic *header2, TStatic *header3)
{
	mstr m("Изпълнителен лист ");

	execlist_exnodate(m, execList);
	m.cat(" по ");
	execlist_lawsuit(m, execList);
	header1->SetCaption(str(m));

	m.clear();
	TOutReg *outReg = &execList->outReg;

	if (outReg->key.no)
	{
		m.printf("Изпратен с %s %ld/", Kind->Seek(outReg->kind), outReg->key.no);

		if (outReg->date.Empty())
			m.printf("%ld", outReg->key.year);
		else
			m.printf("%D", &outReg->date);
	}
	header2->SetCaption(str(m));

	m.clear();
	if (execList->nia.no)
	{
		m.printf("Приемо-предавателен протокол %ld/%ld до ", execList->nia.no, execList->nia.year);
		print_sender(m, UCN_NIA_TDEP, execList->tdep);
	}
	header3->SetCaption(str(m));
}

#include "EditExecListTDep.h"
#include "EditExecListTDep.cpp"

#include "SelectDebtorListBox.h"
#include "SelectDebtorListBox.cpp"

#include "EditExecListCollectDialog.h"
#include "EditExecListCollectDialog.cpp"

#include "ExecListCollectListWindow.h"
#include "ExecListCollectListWindow.cpp"

#include "ExecListCollectListDialog.h"
#include "ExecListCollectListDialog.cpp"

#include "ExecListListWindow.h"
#include "ExecListListWindow.cpp"

#include "ExecListListDialog.h"
#include "ExecListListDialog.cpp"

void nia_collect(TWindow *parent)
{
	TExecListListDialog(parent).Execute();
}

#define period OFFICE_GROUP_CAST(TPeriodGroup)

static void period_period(mstr &m, TGroup *group) { m.printf("%D-%D", &period->minDate, &period->maxDate); }

#undef period

static void another_user_name(mstr &m, TGroup *) { print_user(m, Default->ucn); }

static const offield period_fields[] =
{
	{ "F_PERIOD",	period_period },
	{ "F_USER_NAME",	another_user_name },
	{ NULL, NULL }
};

#define protocol OFFICE_GROUP_CAST(TNIAProtocol)

static void protocol_out_text(mstr &m, TGroup *group)
{
	bool alink = false;
	char name[SIZE_OF_PATH];
	TDate *outDate = TYPECHECK_PTR_CAST(TDate, protocol->Find("F_OUT_DATE"));

	if (any(Electrics[TYPE_TEXT].docketPath))
	{
		TOutReg outReg;

		outReg.key.no = protocol->outNo;
		outReg.key.year = protocol->outYear;
		outReg.kind = KIND_OUT_LETTER;
		outReg.date = outDate->Date();
		electric_name(&outReg, name, TYPE_TEXT);
		alink = exist(name);
	}

	m.cat("Изпратен с ");

	if (alink)
	{
		m.cat("\aa href=\"file:");
		strrep(name, '\\', '/');

		if (strncmp(name, "//", 2))
			m.cat("///");

		m.printf("%s\" target=\"blank\"\b", name);
	}

	m.printf("изх. № %ld/%Dг.", protocol->outNo, &outDate->Date());

	if (alink)
		m.cat("\a/a\b");
}

#undef protocol

static const offield protocol_fields[] =
{
	{ "F_NIA_TDEP",	niaprot_nia_tdep },
	{ "F_NIA_NO",	NULL },
	{ "F_NIA_YEAR",	NULL },
	{ "F_OUT_TEXT",	protocol_out_text },
	{ NULL, NULL }
};

void query_nia_protocols(TWindow *parent)
{
	TNIAProtocol protocol;
	TPeriodGroup period(protocol.Name);
	TOffice office;

	if (get_period(parent, &period) && office.Open("ExecLstTax.htm"))
	{
		TWaitWindow wait(parent, WAIT);
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		CDate date;
		protocol.Add(new TDate("F_OUT_DATE", &date, false));

		msql m("T_NIA_PROTOCOL P "
			"JOIN T_OUTREG O ON O.F_NO = P.F_OUT_NO AND O.F_YEAR = P.F_OUT_YEAR ",
			"P.*, O.F_DATE AS F_OUT_DATE");
		m.AddChar("O.F_KIND", KIND_OUT_LETTER);
		m.AddRange("O.F_DATE", period.minDate, period.maxDate);

		TQuery q(str(m));
		office.Emit(NULL, tr1start, period_fields, &period);

		while (q.Read())
		{
			protocol << q;
			office.Emit(tr1start, tr1final, protocol_fields, &protocol);
		}

		office.Emit(tr1final, NULL, period_fields, NULL);
		office.Show(&wait);
	}
}
