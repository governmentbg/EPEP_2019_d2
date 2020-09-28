#include "cac.h"

#if COURT_TYPE == COURT_AREAL
static bool add_bankrupt(TWindow *parent, TBankrupt *bankrupt)
{
	TFirm firm;

	if (get_ucn(parent, &firm, UCN_FIRMS, -1, true))
	{
		*bankrupt << firm;
		return bankrupt->Insert();
	}

	return false;
}

static bool edit_bankrupt(TWindow *parent, TBankrupt *bankrupt)
{
	// 2011:088 LRQ: no edit address
	return edit_ucn(parent, bankrupt, NULL, true, can_rename(), true);
}

// ----- TBanConSideGroup ---------------------------------------------------
class TBanConSideGroup : public TInconSideGroup
{
public:
	TBanConSideGroup(TBankrupt *bankrupt, const char *tables, const char *fields, bool tLawsuit);

	virtual void FormCriteria(msql &m);

protected:
	char bulstat[SIZE_OF_BULSTAT];
	bool lawsuit;
};

TBanConSideGroup::TBanConSideGroup(TBankrupt *bankrupt, const char *tables, const char *fields, bool tLawsuit)
	: TInconSideGroup(bankrupt, tables, fields), lawsuit(tLawsuit)
{
	strcpy(bulstat, bankrupt->bulstat);
}

void TBanConSideGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddString("F.F_BULSTAT", bulstat);
	m.AddRange("S.F_DATE", minDate, maxDate);

	if (lawsuit)
	{
		m.AddChars("S.F_TYPE", TYPE_TRADEALIKE_LAWS);
		m.AddChars("S.F_KIND", KIND_TRADEALIKE_LAWS);
		m.AddChars("L.F_SOURCE_KIND", KIND_INCONSISTENCE_REQUESTS);
	}
	else
		m.AddChars("R.F_KIND", KIND_INCONSISTENCE_REQUESTS);

	TInconBaseGroup::FormCriteria(m);
	EndCriteria(m, lawsuit ? "L." : "R.");
}

// ----- TQueryBankrupt -----------------------------------------------------
class TQueryBankrupt : public TBankrupt
{
public:
	TQueryBankrupt(TPeriodGroup *tPeriod);

	TPeriodGroup *period;
};

TQueryBankrupt::TQueryBankrupt(TPeriodGroup *tPeriod)
	: period(tPeriod)
{
}

static void bankrupt_criteria(msql &w, TQueryBankrupt *bankrupt, bool lawsuit)
{
	mstr m;
	char T = "RL"[lawsuit];

	m.printf("T_FIRM F JOIN T_SIDE_WIND S ON S.F_UCN = F.F_UCN AND S.F_UCN_TYPE = F.F_UCN_TYPE "
		"JOIN %s %c ON %c.F_NO = S.F_NO AND %c.F_YEAR = S.F_YEAR AND %c.F_TYPE = S.F_TYPE",
		lawsuit ? "T_LAWSUIT" : "T_REQUEST", T, T, T, T);

	TBanConSideGroup bancons(bankrupt, str(m), lawsuit ? "L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_DATE, L.F_FINISHED" :
		"R.F_NO, R.F_YEAR, R.F_TYPE, R.F_KIND, R.F_DATE", lawsuit);

	bancons << bankrupt->period;
	bancons.creditor = -1;
	bancons.order = ORDER_BY_TYN;
	bancons.FormCriteria(w);
}

static void bankrupt_sidewind(mstr &m, TSideWind *sideWind)
{
	m.printf("%s ", Involvement->Seek(sideWind->involvement));
	sideWind->PrintNames(m);
	m.cat("\v");
}

static void bankrupt_sidewinds(mstr &m, TRCDKeyContainer *container)
{
	TSideWind sideWind;
	msql w;

	sideWind << container;
	sideWind.FloatCriteria(w);
	TQuery q(str(w));

	while (q.Read())
	{
		sideWind << q;
		bankrupt_sidewind(m, &sideWind);
	}
}

#define bankrupt OFFICE_GROUP_CAST(TQueryBankrupt)

static void bankrupt_firm(mstr &m, TGroup *group)
{
	TFirm firm;

	firm << group;
	if (firm.Get())
		m.cpy(firm.name);
	else
		print_missing(m, firm.ucnType, firm.ucn);
}

static void bankrupt_bulpref(mstr &m, TGroup *group)
{
	TFirm firm;

	firm << group;
	firm.Get();
	group_bulpref(m, &firm);
}

static void bankrupt_decisions(mstr &m, TSession *session)
{
	TGArray<TDecision> decisions;

	try2decide(session, &decisions);

	for (int i = 0; i < decisions.Count(); i++)
	{
		TDecision *decision = decisions[i];

		m.printf("\v%s от %D\v", Kind->Seek(decision->kind), &decision->fromDate);

		if (any(decision->text))
		{
			m.cat(decision->text);
			m.cat("\v");
		}
	}
}

static void bankrupt_lawsuits(mstr &m, TGroup *group)
{
	int count = 0;
	TLawsuit lawsuit;
	TSession session;

	msql w;
	bankrupt_criteria(w, bankrupt, true);
	TQuery q(str(w));

	while (q.Read())
	{
		lawsuit << q;

		if (count)
			m.cat("\ahr\b");

		m.printf("%s %ld/%ld, образувано на %Dг.\v\v", Kind->Seek(lawsuit.kind), lawsuit.key.no, lawsuit.key.year,
			&lawsuit.date);

		bankrupt_sidewinds(m, &lawsuit);

		CDate final;

		if (find_finalizer(&lawsuit, session))
		{
			final = session.date;
			bankrupt_decisions(m, &session);
		}

		lawsuit.LastKey(w, &session, "F_KIND, F_DATE, F_RESULT, F_TEXT");
		TQuery p(str(w));

		if (p.Read())
		{
			session << p;

			if (session.date > final)
			{
				TSession tSession;

				while (p.Read())
				{
					tSession << p;
					if (tSession.date < session.date)
						break;
					if (strchr(KIND_SESSIONS, tSession.kind) > strchr(KIND_SESSIONS, session.kind))
						session << tSession;
				}

				m.printf("\v%s от %D", Kind->Seek(session.kind), &session.date);
				if (!session.time.Empty())
					m.printf(" %d:%02d", session.time.hour, session.time.minute);
				m.cat("\v");
				if (strchr(RESULT_UNCARRIEDS, session.result))
					m.printf("\vРезултат: %s\v", Result->Seek(session.result));
				if (any(session.text))
				{
					m.cat(session.text);
					m.cat("\v");
				}
				bankrupt_decisions(m, &session);
			}
		}

		count++;
	}

	if (!count)
		m.cpy("Няма образувани съдебни дела по несъстоятелност");
}

static void bankrupt_requests(mstr &m, TGroup *group)
{
	int count = 0;
	TRequest request;

	msql w;
	bankrupt_criteria(w, bankrupt, false);
	TQuery q(str(w));

	while (q.Read())
	{
		request << q;

		if (request.Status() == RESOLUTION_LAWED)
			continue;

		m.printf("%s %ld/%ld, постъпила на %Dг.\v\v", Kind->Seek(request.kind), request.key.no, request.key.year,
			&request.date);

		if (count)
			m.cat("\ahr\b");

		bankrupt_sidewinds(m, &request);
		count++;
	}

	if (!count)
		m.cpy("Няма постъпили молби за обявяване в несъстоятелност");
}

#undef bankrupt

static const offield banklaw_fields[] =
{
	{ "F_SERIAL_NO",	NULL },
	{ "F_FIRM", 	bankrupt_firm },
	{ "F_BULPREF",	bankrupt_bulpref },
	{ "F_LAWSUITS", 	bankrupt_lawsuits },
	{ NULL, NULL }
};

static const offield bankreq_fields[] =
{
	{ "F_REQUESTS", 	bankrupt_requests },
	{ NULL, NULL }
};

// ----- TBankruptQuery -----------------------------------------------------
class TBankruptQuery : public TProcess
{
public:
	TBankruptQuery(TWindow *parent, TPeriodGroup *period, TGArray<TBankrupt> *bankrupts);

	TPeriodGroup *period;

	void Execute();

protected:
	TGArray<TBankrupt> *bankrupts;
};

TBankruptQuery::TBankruptQuery(TWindow *parent, TPeriodGroup *tPeriod, TGArray<TBankrupt> *tBankrupts)
	: TProcess(parent, "справката", WAIT), period(tPeriod), bankrupts(tBankrupts)
{
}

#define bankrupt OFFICE_GROUP_CAST(TBankruptQuery)

static void bankrupt_period(mstr &m, TGroup *group)
{
	m.printf("от %D до %D", &bankrupt->period->minDate, &bankrupt->period->maxDate);
}

#undef bankrupt

static const offield title_fields[] =
{
	{ "F_COURT_UCLP",	generic_court_uclp },
	{ "F_PERIOD",	bankrupt_period },
	{ NULL, NULL }
};

void TBankruptQuery::Execute()
{
	TOffice(office);
	TQueryBankrupt bankrupt(period);

	if (office.Open("BancReq.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1break = office.Find("tr1 break");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, title_fields, this);
		for (int i = 0; i < bankrupts->Count(); i++)
		{
			bankrupt << (*bankrupts)[i];

			office.Emit(tr1start, tr1break, banklaw_fields, &bankrupt);
			office.Emit(tr1break, tr1final, bankreq_fields, &bankrupt);

			wait->TickCount(i, "обработени лица");
		}
		office.Emit(tr1final, NULL, title_fields, this);

		office.Show(wait);
		Debug();
	}
}

#define bankrupt OFFICE_GROUP_CAST(TBankrupt)

static void bankrupt_name(mstr &m, TGroup *group)
{
	TFirm firm;

	firm << group;

	if (firm.Get())
		m.cpy(firm.name);
	else
		print_missing(m, firm.ucnType, firm.ucn);

	strcpy(bankrupt->bulstat, firm.bulstat);
}

#undef bankrupt

// ----- bankrupts ----------------------------------------------------------
enum { BANKRUPT_SELECT_ALL = INT_MIN };

#include "BankruptListBox.h"
#include "BankruptListBox.cpp"

#include "QueryBankruptDialog.h"
#include "QueryBankruptDialog.cpp"

#include "BankruptListWindow.h"
#include "BankruptListWindow.cpp"

#include "BankruptListDialog.h"
#include "BankruptListDialog.cpp"

void edit_bankrupts(TWindow *parent)
{
	TBankruptListDialog(parent).Execute();
}

// ----- TBankOutReg --------------------------------------------------------
class TBankOutReg : public TOutReg
{
public:
	TBankOutReg() { }

	TGSArray<TInReg> inRegs;
};

// ----- TBankArray ---------------------------------------------------------
class TBankArray : public TGSArray<TRCDKeyContainer>
{
public:
	TBankArray() : TGSArray<TRCDKeyContainer>() { }
	void Unsort() { lesseql = NULL; }
};

// ----- TBankWind ----------------------------------------------------------
class TBankWind : public TSideWind
{
public:
	TBankWind(TQuery &r, TFirm *firm, TLawsuit *tLawsuit);

	void LoadAppeals(TRCDKeyContainer *container, TGSArray<TBankOutReg> &outRegs);

	char name[SIZE_OF_FIRM_NAME];
	char bulstat[SIZE_OF_BULSTAT];
	TLawsuit lawsuit;
	TSession session;
	TGArray<TDecision> decisions;
	TBankArray events;
};

TBankWind::TBankWind(TQuery &r, TFirm *firm, TLawsuit *tLawsuit)
{
	Add(
		new TString("F_NAME", name, sizeof name),
		new TString("F_BULSTAT", bulstat, sizeof bulstat),
		NULL
	);

	*this << r;
	*this << firm;
	lawsuit << tLawsuit;

	msql m;

	// pass 1: events
	session.key = key;
	session.FloatCriteria(m);
	TQuery q(str(m));
	while (q.Read())
	{
		TGArray<TDecision> decisions;

		session << q;
		if (lawsuit.finished.Empty() || !try2decide(&session, &decisions))
		{
			TSession *tSession = new TSession;
			*tSession << session;
			events.Add(tSession);
		}
		else
		{
			for (int i = 0; i < decisions.Count(); i++)
				events.Add(decisions[i]);
			decisions.Clear();
		}
	}

	// pass 2: leader, s con'd
	m.AddChars("F_RESULT", RESULT_BANCONSIST);
	m.AddOrder("F_DATE DESC");

	TQuery p(str(m));
	if (p.Read())
	{
		session << p;
		try2decide(&session, &decisions);
	}
	else
	{
		TSession tSession;
		session << tSession;
	}

	// pass 3: appealing
	events.Unsort();

	for (int i = 0; i < events.Count(); i++)
	{
		TRCDKeyContainer *container = events[i];

		if (strchr(KIND_SESSIONS_ENDOCS, container->kind))
		{
			TGSArray<TBankOutReg> outRegs;

			LoadAppeals(container, outRegs);

			for (int n = 0; n < outRegs.Count(); n++)
			{
				i++;
				events.Insert(outRegs[n], i);
			}

			outRegs.Clear();
		}
	}
}

void TBankWind::LoadAppeals(TRCDKeyContainer *container, TGSArray<TBankOutReg> &outRegs)
{
	mstr results;
	msql m("T_INREG");

	results.cach(RESULT_IOREG_SEND);
	results.cat(RESULT_IOREG_RETURNEDS);

	m.AddLong("F_LAWSUIT_NO", container->key.no);
	m.AddLong("F_LAWSUIT_YEAR", container->key.year);
	m.AddChar("F_LAWSUIT_TYPE", container->key.type);
	m.AddDate("F_DECISION_DATE", container->date);

	if (strchr(KIND_ENDOCS, container->kind))
	{
		m.AddChar("F_SESSION_KIND", ((TDecision *) container)->sessionKind);
		m.AddChar("F_DECISION_KIND", container->kind);
	}
	else if (strchr(KIND_SESSIONS, container->kind))
	{
		m.AddChar("F_SESSION_KIND", container->kind);
		m.AddChars("F_DECISION_KIND", KIND_IN_UNDECIDABLES);
	}

	TQuery q(str(m));
	while (q.Read())
	{
		TInReg inReg;
		TInRegEvent inRegEvent;
		bool added = false;

		inReg << q;

		m.Begin(inRegEvent.Name, "DISTINCT F_OUT_NO, F_OUT_YEAR");
		m.Add(&inReg, "F_NO, F_YEAR");
		m.AddChars("F_RESULT", str(results));

		TQuery p(str(m));
		while (p.Read())
		{
			inRegEvent << p;

			for (int i = 0; i < outRegs.Count(); i++)
			{
				TBankOutReg *outReg = outRegs[i];

				if (outReg->key.no == inRegEvent.outNo && outReg->key.year == inRegEvent.outYear)
				{
					outReg->inRegs.AddCopy(&inReg);
					added = true;
				}
			}

			if (!added)		// outReg not loaded yet
			{
				TBankOutReg *outReg = new TBankOutReg;

				outReg->key.no = inRegEvent.outNo;
				outReg->key.year = inRegEvent.outYear;
				outReg->Get();
				outReg->inRegs.AddCopy(&inReg);
				outRegs.Add(outReg);
				added = true;
			}
		}

		if (!added)		// no send/receive, fake outReg
		{
			TBankOutReg *outReg = new TBankOutReg;

			outReg->inRegs.AddCopy(&inReg);
			outRegs.Add(outReg);
		}
	}
}

#define bankWind OFFICE_GROUP_CAST(TBankWind)

static void bancons_today(mstr &m, TGroup *) { Today.Print(m, "%0D"); }

static void bancons_first_date(mstr &m, TGroup *group)
{
	char bulstat[SIZE_OF_BULSTAT];

	strcpy(bulstat, ((TString *) group->Find("F_BULSTAT"))->S());
	bulstat[3] = '\0';
	m.cpy(any(bulstat) && atoi(bulstat) < 200 ? "01.01.1991" : ELLIPSIS(20));
}

static void bancons_yesterday(mstr &m, TGroup *)
{
	CDate yesterday = Today;
	yesterday += -1;
	yesterday.Print(m, "%0D");
}

static void bancons_firm(mstr &m, TGroup *group)
{
	const char *name = ((TString *) group->Find("F_NAME"))->S();
	m.cpy(any(name) ? name : ELLIPSIS(48));
}

static void bancons_firm_case(mstr &m, TGroup *group)
{
	TConnect connect;

	connect.key = bankWind->key;
	connect.connectType = CONNECT_TYPE_FIRM_DEBTOR;

	if (connect.Try())
	{
		m.printf("%s %s/%ld на ", ConnectKind->Seek(connect.connectKind), connect.connectNo, connect.connectYear);

		TSender sender;
		sender << connect;
		m.cat(sender.Try() ? sender.name : sender.ucn);
	}
}

static void bancons_composition(mstr &m, TGroup *group) { print_composition(m, bankWind->lawsuit.composition, "с-в"); }
static void bancons_source_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(bankWind->lawsuit.sourceKind)); }
static void bancons_source_no(mstr &m, TGroup *group) { itom(bankWind->lawsuit.source.no, m); }

static void bancons_source_date(mstr &m, TGroup *group)
{
	TRequest request;
	request.key = bankWind->lawsuit.source;

	if (request.Try("F_DATE"))
		request.date.Print(m, "%0D");
	else
		itom(bankWind->lawsuit.source.year, m);
}

static void bancons_suitors(mstr &m, TGroup *group)
{
	TGArray<TSideWind> sideWinds;
	char *categories = load_sidebasic(&bankWind->lawsuit, sideWinds);

	for (int i = 0; i < sideWinds.Count(); i++)
	{
		if (categories[i] == SIDEWIND_LEFT)
		{
			TSideWind *sideWind = sideWinds[i];

			m.sep(", ");
			m.printf(Involvement->Seek(sideWind->involvement));
			m.cat(" ");
			sideWind->PrintNames(m);
		}
	}

	delete[] categories;
}

static void bancons_extras(mstr &m, TGroup *group)
{
	TLawsuit &lawsuit = bankWind->lawsuit;
	TConnect connect;
	TGArray<TConnect> connects;
	msql w(connect.Name);
	TArchive archive;

	w.Add("F_CONNECT_NO");
	w.printf("= '%ld'", lawsuit.key.no);
	w.AddLong("F_CONNECT_YEAR", lawsuit.key.year);
	w.AddChars("F_CONNECT_TYPE", CONNECT_TYPE_LAW_LINKS);
	w.AddChar("F_CONNECT_KIND", type2ConnectKind(lawsuit.key.type));
	w.AddChars("F_TYPE", TYPE_LAWSUITS);

	TQuery q(str(w));
	while (q.Read())
	{
		TLawsuit lawsuit;

		connect << q;
		m.printf("Преобразувано в %s %ld от %ld", Type->Seek(connect.key.type), connect.key.no, connect.key.year);

		lawsuit.key = connect.key;
		if (lawsuit.Get("F_COLLEGE, F_COMPOSITION"))
		{
			m.printf(", %s %s", College->Seek(lawsuit.college), COLLEGE);
			if (PRINT_COMPOSITIONS)
				m.printf(", %s състав", Composition->Seek(lawsuit.composition));
		}

		m.cat("\v");
	}

	archive.key = lawsuit.key;

	if (archive.Try())
	{
		m.printf("Архивирано на %0Dг. с архивен № %ld/%ldг., връзка %ld\v", &archive.archiveDate, archive.archiveNo,
			archive.archiveYear, archive.linkNo);
	}
}

#undef bankWind

static void print_connect(mstr &m, TOutReg *outReg, char connectType, const char *suffix)
{
	// mostly from query_connect()
	TConnect connect;

	connect.key = outReg->key;
	connect.connectType = connectType;

	if (connect.Try())
	{
		if (connect.decisionKind || !connect.decisionDate.Empty())
		{
			m.printf("\v%s", connect.decisionKind ? Kind->Seek(connect.decisionKind) : "Съдебен акт");
			connect.decisionDate.Print(m, " от %0Dг.");
		}

		TSender sender;

		sender.ucnType = UCN_INSTANCE_HIGHER;
		strcpy(sender.ucn, connect.ucn);
		m.printf("\v%s%s", sender.Get() ? sender.name : sender.ucn, suffix);
	}
}

static void print_resolution(mstr &m, const char *name, char resolution, const char *text)
{
	if (resolution != RESOLUTION_FAKE || any(text))
	{
		m.sep("\v");
		m.printf("\v%s: %s", name, Resolution->Seek(resolution));

		if (any(text))
			m.printf("\v%s", text);
	}
}

#define container OFFICE_GROUP_CAST(TRCDKeyContainer)
#define outReg OFFICE_GROUP_CAST(TBankOutReg)

static void bancons_left(mstr &m, TGroup *group)
{
	if (container->key.type == TYPE_OUTREG)
	{
		for (int i = 0; i < outReg->inRegs.Count(); i++)
		{
			TInReg *inReg = outReg->inRegs[i];

			m.printf("Входящ № %ld от %0Dг. - %s, %0T\v", inReg->key.no, &inReg->date, Kind->Seek(inReg->kind),
				&inReg->time);

			// 2013:198 LRQ: p.147.2: comout; 2013:199 IRQ: separate format
			TGArray<TSideWind> sideWinds;
			load_sidewinds(inReg, &sideWinds);

			for (int i = 0; i < sideWinds.Count(); i++)
			{
				TSideWind *sideWind = sideWinds[i];

				if (sideWind->Appealer())
					bankrupt_sidewind(m, sideWind);
			}
		}

		if (outReg->key.no)
		{
			constant mult = "и" + (outReg->inRegs.Count() == 1);
			TSender sender;

			m.printf("Изпратен%s на %0Dг. с %s %ld от %ldг. на %s\v", mult, &outReg->date, Kind->Seek(outReg->kind),
				outReg->key.no, outReg->key.year, get_receiver(outReg, &sender) ? sender.name : sender.ucn);

			if (strchr(RESULT_IOREG_RETURNEDS, outReg->Status()))
			{
				m.printf("Върнат%s на %0Dг.\v", mult, &outReg->returned);
				print_connect(m, outReg, CONNECT_TYPE_VZ_LAWSUIT, "\v");
				print_connect(m, outReg, CONNECT_TYPE_KS_LAWSUIT, "");
			}
		}
	}
	else
	{
		container_kind(m, group);
		m.cat(" от ");
		container_event_date1(container).Print(m, "%0D");

		TString *accomplys = (TString *) group->Find("F_ACCOMPLYS");

		if (accomplys)
			for (const char *s = accomplys->S(); *s; s++)
				m.printf("%s%s", s == accomplys->S() ? "\v" : ", ", Accomply->Seek(*s));

		TCharAlias *debtorStatus = (TCharAlias *) group->Find("F_DEBTOR_STATUS");

		if (debtorStatus && debtorStatus->C())
			m.printf("\v%s", DebtorStatus->Seek(debtorStatus->C()));
	}
}

static void bancons_right(mstr &m, TGroup *group)
{
	if (container->key.type == TYPE_OUTREG)
	{
		print_resolution(m, "Резултат от въззивна инстанция", outReg->resultI, str(outReg->remarkVZ));
		print_resolution(m, "Резултат от касационна инстанция", outReg->resultKS, str(outReg->remarkKS));
		print_resolution(m, "II Резултат от въззивна инстанция", outReg->resultII, "");
	}
	else
		container_text(m, container);
}

#undef outReg
#undef container

static const offield bancons_fields[] =
{
	{ "F_COURT_UCLP", 	generic_camel_uclp },
	{ "F_TODAY",		bancons_today },
	{ "F_TITLE",		group_title },
	{ "F_FIRST_DATE",		bancons_first_date },
	{ "F_LAST_DATE",		bancons_yesterday },
	{ "F_FIRM",			bancons_firm },
	{ "F_FIRM_CASE",		bancons_firm_case },
	{ "F_BULPREF",		group_bulpref },
	{ "F_LAW_TYPE",		basewind_key_type },
	{ "F_LAW_NO",		basewind_key_no },
	{ "F_LAW_YEAR",		basewind_key_year },
	{ "F_COMPOSITION",	bancons_composition },
	{ "F_SOURCE_KIND",	bancons_source_kind },
	{ "F_SOURCE_NO",		bancons_source_no },
	{ "F_SOURCE_DATE",	bancons_source_date },
	{ "F_SUITORS",		bancons_suitors },
	{ "F_REASON",		NULL },
	{ "F_EXTRAS",		bancons_extras },
	{ "F_LEFT",			bancons_left },
	{ "F_RIGHT",		bancons_right },
	{ "F_NOW",			generic_now },	
	{ "F_USER_INITIALS",	generic_user_initials },
	{ NULL,			NULL }
};

// ----- bank_cert ----------------------------------------------------------
#include "EditBanConSistDialog.h"
#include "EditBanConSistDialog.cpp"

#define banConSist OFFICE_GROUP_CAST(TBanConSist)

static void banconsist_text1(mstr &m, TGroup *group)
{
	char *text2;

	strcpy(banConSist->text1, str(banConSist->text));
	text2 = banConSist->text2 = split_at(banConSist->text1, 95);

	if (any(text2))
	{
		strmove(text2 + 8, text2);
		memset(text2, ' ', 8);
	}

	m.cat(banConSist->text1);
}

#undef banConSist

static const offield banConSist_fields[] =
{
	{ "F_BANCONSIST",	NULL },
	{ "F_TEXT",		NULL },
	{ "F_ACTIVE",	group_active },
	{ NULL,		NULL }
};

static void print_banconsist(TWindow *parent, TGroupArray *array)
{
	TOffice office;

	if (office.Open("BanConSist.htm"))
	{
		TWaitWindow wait(parent, WAIT);
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < array->Count(); i++)
			office.Emit(tr1start, tr1final, banConSist_fields, (*array)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(&wait);
	}
}

#include "BanConSistListWindow.h"
#include "BanConSistListWindow.cpp"

#include "BanConSistListDialog.h"
#include "BanConSistListDialog.cpp"

static constant CERT_USAGE = "Настоящото удостоверение да послужи ........................................................";

static void bank_cert(TWindow *parent, TFirm *firm)
{
	mstr reason;
	TOffice office;
	TText *field = new TText("F_REASON", &reason, SIZE_OF_BANCONSIST_TEXT);
	const char *name;

	firm->Add(field);
	reason.cat(CERT_USAGE);

	switch (TBanConSistListDialog(parent, field, IDD_BANCONSIST_SELECT1).Execute())
	{
		case IDENTER : name = "BancCert.htm"; break;
		case IDEXEC : name = "BancCert1.htm"; break;
		default : name = NULL;
	}

	if (name && office.Open(name, OFFICE_COPY_HEADER))
	{
		office.Emit(NULL, NULL, bancons_fields, firm);
		office.Close();

		if (office.Open(name, OFFICE_SKIP_HEADER, ""))
		{
			office.Emit(NULL, NULL, bancons_fields, firm);
			office.Show(NULL);
		}
	}

	firm->Remove("F_REASON");
}

static void bank_cert(TWindow *parent, TGArray<TBankWind> *bankWinds, const char *tTitle)
{
	TDummyGroup group;
	mstr reason;
	TText *field = new TText("F_REASON", &reason, SIZE_OF_BANCONSIST_TEXT);

	group.Add(field);
	reason.cat(CERT_USAGE);

	if (!BanConSist->Count() || TBanConSistListDialog(parent, field, IDD_BANCONSIST_SELECT).Execute() == IDOK)
	{
		static constant name = "BancCert_.htm";
		TOffice office;

		if (office.Open(name, OFFICE_COPY_HEADER))
		{
			TTitleGroup title(tTitle);

			office.Emit(NULL, NULL, bancons_fields, &title);
			office.Close();

			if (office.Open(name, OFFICE_SKIP_HEADER, ""))
			{
				constant tb1start = office.Find("tb1 start");
				constant tr1start = office.Find("tr1 start");
				constant tr1final = office.Find("tr1 final");
				constant tb1final = office.Find("tb1 final");

				office.Emit(NULL, tb1start, bancons_fields, &title);
				
				for (int i = 0; i < bankWinds->Count(); i++)
				{
					TBankWind *bankWind = (*bankWinds)[i];

					office.Emit(tb1start, tr1start, bancons_fields, bankWind);

					for (int i = 0; i < bankWind->events.Count(); i++)
						if (bankWind->events[i]->date >= bankWind->lawsuit.finished)
							office.Emit(tr1start, tr1final, bancons_fields, bankWind->events[i]);

					office.Emit(tr1final, tb1final, bancons_fields, bankWind);
				}

				office.Emit(tb1final, NULL, bancons_fields, &group);
				office.Show(NULL);
			}
		}
	}

	group.Remove("F_REASON");
}

// ----- banconside ---------------------------------------------------------
#include "BanConSideListBox.h"
#include "BanConSideListBox.cpp"

#include "BanConSideDialog.h"
#include "BanConSideDialog.cpp"

void query_banconside(TWindow *parent, TGArray<TFirm> *firms)
{
	if (banconsimilar(firms))
	{
		TFirm *firm = (*firms)[0];

		if (firms->Count() == 1 && !atob(firm->ucn))
			bank_cert(parent, firm);
		else
		{
			TGArray<TBankWind> bankWinds;
			int rejected = 0;
			mstr m;

			TWaitWindow wait(parent, WAIT);
			TLawsuit lawsuit;
			TSession session;

			for (int i = 0; i < firms->Count(); i++)
			{
				TInconSideGroup incons((*firms)[i],
					"T_SIDE_WIND S JOIN T_LAWSUIT L ON L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR AND "
					"L.F_TYPE = S.F_TYPE", NULL);
				msql w;

				incons.creditor = -1;
				//incons.order = ORDER_BY_TYN;	// 2013:196 LPR: p.54.1
				incons.FormCriteria(w);

				TQuery q(str(w));
				while (q.Read())
				{
					mstr cause;
					lawsuit << q;

					if (can_access_exact(&lawsuit, false, false, &cause))
					{
						// real finalized is not a good criteria
						if (find_finalizer(&lawsuit, session))
							lawsuit.finished = session.date;
						bankWinds.Add(new TBankWind(q, (*firms)[i], &lawsuit));
					}
					else
					{
						m.sep(",\n");

						if (rejected++ == REJECTEDS_MAX)
						{
							m.cat("..");
							break;
						}

						m.printf("%s %ld/%ldг. - %s", Type->Seek(lawsuit.key.type), lawsuit.key.no,
							lawsuit.key.year, str(cause));
					}
				}
			}

			if (rejected)
				error("%s.", str(m));
			else if (!bankWinds.Count())
				bank_cert(parent, firm);
			else
				TBanConSideDialog(parent, &bankWinds).Execute();
		}
	}
}

void edit_banconsists(TWindow *parent)
{
	TBanConSistListDialog(parent, NULL, IDD_BANCONSIST_LIST).Execute();
	BanConSist->ReloadCache();
}
#endif  // AREAL
