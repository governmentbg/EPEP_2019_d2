#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
class TPrintExecutive : public TDecision
{
public:
	TPrintExecutive(TWindow *parent, TDecision *tDecision, TSession *tSession, TLawsuit *tLawsuit);
	~TPrintExecutive();

	void TextSides(mstr &m, const char *involvements);

	TSession *session;
	TLawsuit *lawsuit;
	long ucnstat;

	char *categories;
	TGArray<TSideWind> sessWinds;
	TGArray<TLegalSide> credebtors;

	TWaitWindow wait;
};

TPrintExecutive::TPrintExecutive(TWindow *parent, TDecision *tDecision, TSession *tSession, TLawsuit *tLawsuit)
	: session(tSession), lawsuit(tLawsuit), wait(parent, WAIT)
{
	*this << tDecision;

	Add(
		new TRadio("F_EXEC_KIND_410_MONEY", &execKind, EXEC_KIND_410_MONEY),
		new TRadio("F_EXEC_KIND_410_MOVABLE", &execKind, EXEC_KIND_410_MOVABLE),
		new TRadio("F_EXEC_KIND_417_MONEY", &execKind, EXEC_KIND_417_MONEY),
		new TRadio("F_EXEC_KIND_417_MOVABLE", &execKind, EXEC_KIND_417_MOVABLE),
		new TCheck("F_UCN_STAT", &ucnstat, TRUE),
		NULL
	);

	TLegalSide legalSide;
	msql m(legalSide.Name);
	categories = load_sidebasic(session, sessWinds);

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.AddChar("F_KIND", eventNo ? kind : sessionKind);
	m.Add(this, "F_DATE");
	m.AddChars("F_INVOLVEMENT", INVOLVEMENT_CREDEBTORS);
	legalSide.LoadArray(credebtors, str(m));

	m.Begin(Name, "F_KIND, F_FROM_DATE, F_EVENT_NO");
	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.AddChar("F_SESSION_KIND", session->kind);
	m.AddDate("F_DATE", session->date);

	TQuery q(str(m));
	if(q.Read())
		*this << q;
}

TPrintExecutive::~TPrintExecutive()
{
	delete[] categories;
}

void TPrintExecutive::TextSides(mstr &m, const char *involvements)
{
	for (int i = 0; i < credebtors.Count(); i++)
	{
		TLegalSide *legalSide = credebtors[i];

		if (!strchr(involvements, legalSide->involvement))
			continue;

		m.printf(any(m) ? ",\v%s " : "%s ", Involvement->Seek(legalSide->involvement));
		legalSide->PrintNames(m);
		if (ucnstat)
			legalSide->PrintIdent(m);
		m.cat(" с адрес  ");
		legalSide->address->Print(m, ADDRESS_PHONES);

		TLinkWind linkWind;
		msql w;

		linkWind << legalSide;
		linkWind.FloatCriteriaByOrdinary(w);
		w.AddChar("F_PROXY_KIND", SIDE_MARK_LIKE_LAWYER);

		TQuery q(str(w));
		if (q.Read())
		{
			TLinkWind linkWind;

			linkWind << q;
			m.printf(" представляван(а) от %s ", Involvement->Seek(linkWind.proxyInvolvement));
			linkWind.ucnType = linkWind.proxyUcnType;
			strcpy(linkWind.ucn, linkWind.proxyUcn);
			linkWind.PrintNames(m);
			if (ucnstat)
				linkWind.PrintIdent(m);
		}
	}
}

void print_sides(mstr &m, char *categories, TGArray<TSideWind> *sessWinds, char side)
{
	TAddress addr;

	for (int i = 0; i < sessWinds->Count(); i++)
	{
		if (categories[i] != side)
			continue;

		TSideWind *sessWind = (*sessWinds)[i];

		if (atof(sessWind->ucn))
		{
			if (any(m))
				m.cat(", ");

			if (strchr(UCN_CITIZENS, sessWind->ucnType))
			{
				TCitizen citizen;

				citizen.ucnType = sessWind->ucnType;
				strcpy(citizen.ucn, sessWind->ucn);

				if (citizen.Try())
					m.printf("%s %s %s ЕГН ", citizen.name, citizen.reName, citizen.family);

				if (is_real_ucn(sessWind))
					m.cat(sessWind->ucn);
				else
					m.cat(ELLIPSIS_STD);
			}
			else if (strchr(UCN_FIRMS, sessWind->ucnType))
			{
				TFirm firm;

				firm << sessWind;

				if(firm.Try())
					m.cat(firm.name);
			}

			TAnyAddress *address = &addr;
			strcpy(address->ucn, sessWind->ucn);
			address->ucnType = sessWind->ucnType;

			if (address->Try())
			{
				address->MakeLines();
				m.printf(" с адрес %s %s ", address->line2, address->line3 );
			}
			else
				m.cat(ELLIPSIS_STD);
		}
	}
}

#define printexe OFFICE_GROUP_CAST(TPrintExecutive)
#define lawsuit (printexe->lawsuit)
#define session (printexe->session)

static void printexe_college(mstr &m, TGroup *group) { m.printf("%s %s", College->Seek(lawsuit->college), COLLEGE); }
static void printexe_session_judge(mstr &m, TGroup *group) { print_user(m, session->judge); }
static void printexe_lawsuit_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(lawsuit->kind)); }
static void printexe_deptors(mstr &m, TGroup *group) { printexe->TextSides(m, INVOLVEMENT_DEBTORS); }
static void printexe_creditors(mstr &m, TGroup *group) { printexe->TextSides(m, INVOLVEMENT_CREDITORS); }

static void printexe_composition(mstr &m, TGroup *group)
{
	if (PRINT_COMPOSITIONS)
		m.printf("%s състав", Composition->Seek(session->composition));
}

static void printexe_decision_no(mstr &m, TGroup *group) { itom(printexe->eventNo, m); }
static void printexe_decision_date(mstr &m, TGroup *group) { printexe->fromDate.Print(m); }

static void printexe_zzdn_suitors(mstr &m, TGroup *group)
{
	print_sides(m, printexe->categories, &printexe->sessWinds, SIDEWIND_LEFT);
}

static void printexe_zzdn_defendants(mstr &m, TGroup *group)
{
	print_sides(m, printexe->categories, &printexe->sessWinds, SIDEWIND_RIGHT);
}

#undef session
#undef lawsuit
#undef printexe

static const offield printexe_fields[] =
{
	{ "F_SESSION_DATE",	container_date },
	{ "F_COLLEGE",		printexe_college },
	{ "F_LAWSUIT_NO",		keygroup_key_no },
	{ "F_LAWSUIT_YEAR",	keygroup_key_year },
	{ "F_JUDGE",		printexe_session_judge },
	{ "F_LAWSUIT_KIND",	printexe_lawsuit_kind },
	{ "F_INSTANCE_III",	generic_instance_III },
	{ "F_DEBTORS",		printexe_deptors },
	{ "F_CREDITORS",		printexe_creditors },
	{ "F_COMPOSITION",	printexe_composition },
	{ "F_TODAY", 		container_event_date },
	{ "F_DECISION_NO", 	printexe_decision_no },
	{ "F_DECISION_DATE", 	printexe_decision_date },
	{ "F_ZZDN_DEFENDANTS", 	printexe_zzdn_defendants },
	{ "F_ZZDN_SUITORS", 	printexe_zzdn_suitors },
	{ "F_JUDGE_TYPE",		generic_judge_type },
	{ "F_JUDGE_TYPE_COURT",	generic_judge_type_court },
	{ NULL,			NULL }
};

#if 20170243
static const char *print_order(TWindow *parent, const char *name, TPrintExecutive &printexe)
{
	TOffice office;
	char target[SIZE_OF_PATH];

	build_fn(tempdir, name, target);

	if (office.Open(name, OFFICE_COPY_ALL, target))
	{
		TWaitWindow wait(parent, WAIT_MORE);

		office.Emit(NULL, NULL, printexe_fields, &printexe);
		office.ShowSync(&wait, TRUE, "Заповедта ще бъде отворена", "Заповедта е записана");
		wait.ResetForeground();
		return name;
	}

	return NULL;
}
#else  // 20170243
static void print_order(TWindow *parent, const char *name, const TDecision *decision, TPrintExecutive &printexe, int index)
{
	if (SAVE_SUBPOENAS)
	{
		TOffice office;
		char target[SIZE_OF_PATH];

		// mostly from subplist.cpp
		electric_name(decision, target, TYPE_TEXT, index);

		if (office.Open(name, OFFICE_COPY_ALL, basename(target)))
		{
			office.Emit(NULL, NULL, printexe_fields, &printexe);
			{
				TWaitWindow wait(parent, WAIT_MORE);

				office.ShowSync(&wait, TRUE, "Заповедта ще бъде отворена", "Заповедта е записана");
				wait.ResetForeground();
			}
			electric_copy(office.Name, target);
		}
	}
	else
		show_office(name, printexe_fields, &printexe);
}
#endif  // 20170243

#include "LegalSideAddressListBox.h"
#include "LegalSideAddressListBox.cpp"

#include "LegalSideAddressDialog.h"
#include "LegalSideAddressDialog.cpp"

#if 20170243
const char *print_protective(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit, const char *name)
{
	TPrintExecutive printexe(parent, decision, session, lawsuit);
	return print_order(parent, name, printexe);
}
#else  // 20170243
void print_protective(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit, const char *name)
{
	TPrintExecutive printexe(parent, decision, session, lawsuit);

	print_order(parent, name, decision, printexe,
		decision->kind == KIND_PROTECTIVE ? EXTRA_NO_PROTECTIVE : EXTRA_NO_IMMED_PROTECT);
}
#endif  // 20170243
#endif  // AREAL || REGIONAL

#if COURT_TYPE == COURT_REGIONAL
// 2017:157 LPR: regional only 
#include "SelectExecKindDialog.h"
#include "SelectExecKindDialog.cpp"

#if 20170243
const char *print_executive(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit)
#else  // 20170243
void print_executive(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit)
#endif  // 20170243
{
	TPrintExecutive printexe(parent, decision, session, lawsuit);

	if (TSelectExecKindDialog(parent, &printexe).Execute() == IDOK)
	{
		enum { EXEC_KIND_COUNT = EXTRA_NO_EXECUTIVE_LAST - EXTRA_NO_EXECUTIVE_BASE + 1 };

		struct TExecKindName
		{
			char execKind;
			const char *htemplate;
		};

		static const TExecKindName names[EXEC_KIND_COUNT] =
		{
			{ EXEC_KIND_410_MONEY,   "O-410-PZ.htm" },
			{ EXEC_KIND_410_MOVABLE, "O-410-DV.htm" },
			{ EXEC_KIND_417_MONEY,   "O-417-PZ.htm" },
			{ EXEC_KIND_417_MOVABLE, "O-417-DV.htm" },
		};

		int index;

		for (index = 0; index < EXEC_KIND_COUNT; index++)
			if (names[index].execKind == printexe.execKind)
				break;

		if (index == EXEC_KIND_COUNT)
			fatal("printexe: unsupported exec kind %ld", decision->execKind);
		else
		{
			decision->execKind = printexe.execKind;

			if (TLegalSideAddressDialog(parent, printexe.credebtors).Execute() == IDOK)
			#if 20170243
				return print_order(parent, names[index].htemplate, printexe);
			#else  // 20170243
				print_order(parent, names[index].htemplate, decision, printexe, EXTRA_NO_EXECUTIVE_BASE + index);
			#endif  // 20170243
		}
	}
#if 20170243
	return NULL;
#endif
}
#endif  // REGIONAL
