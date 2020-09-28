#include "cac.h"

#include "SelectKindDialog.h"
#include "SelectKindDialog.cpp"

bool select_kind(TWindow *parent, TRCDKeyContainer *container, const char *kinds)
{
	return TSelectKindDialog(parent, container, kinds).Execute() == IDOK;
}

#include "QueryConnectDialog.h"
#include "QueryConnectDialog.cpp"

bool get_connect(TWindow *parent, TConnect *connect)
{
	return TQueryConnectDialog(parent, connect).Execute() == IDOK;
}

#include "SelectSubjectsDialog.h"
#include "SelectSubjectsDialog.cpp"

bool select_subjects(TWindow* parent, TGArray<TSubject> *subjects, const char *kinds, bool saas, sign hidden)
{
	return TSelectSubjectsDialog(parent, subjects, kinds, saas, hidden).Execute() == IDOK;
}

#include "SelectOldSubjectsDialog.h"
#include "SelectOldSubjectsDialog.cpp"

bool select_old_subjects(TWindow* parent, TGArray<TOldSubject> *oldSubjects, const char *kinds)
{
	return TSelectOldSubjectsDialog(parent, oldSubjects, kinds).Execute() == IDOK;
}

#include "SelectUserDialog.h"
#include "SelectUserDialog.cpp"

bool select_user(TWindow* parent, const char *posts, long flags, TUser *user, const char *exclude)
{
	return TSelectUserDialog(parent, posts, flags, user, exclude).Execute() == IDOK;
}

#include "SelectStringDialog.h"
#include "SelectStringDialog.cpp"

int select_string(TWindow *parent, TStringArray *strings, const char *title, const char *label, int resId)
{
	int choice;
	return TSelectStringDialog(parent, strings, title, label, choice, resId).Execute() == IDOK ? choice : -1;
}

class TCollectEvents
{
public:
	TCollectEvents(const char *tStatuses, match_func tMatch);

	const char *statuses;
	match_func match;

	void Accept(mstr &m, TRCDKeyContainer *container, int pos, char status);
	bool Collect(mstr &m, TRCDKeyContainer *container, int pos);
	void Reject(mstr &doc, const char *why);
	void RejectMessage(const char *what);

	TGArray<TRCDKeyContainer> array;
	TStringArray strings;
	mstr stats;
	int rejected;
	mstr m;
};

TCollectEvents::TCollectEvents(const char *tStatuses, match_func tMatch)
	: statuses(tStatuses), match(tMatch), rejected(0)
{
}

void TCollectEvents::Accept(mstr &m, TRCDKeyContainer *container, int pos, char status)
{
	array.Insert(container, pos);
	strings.Insert(strnew(str(m)), pos);
	mbk_insert(&stats, &status, pos, 1);
}

bool TCollectEvents::Collect(mstr &m, TRCDKeyContainer *container, int pos)
{
	char status = container->Status();
	const char *ts = wrong_status;

	m.printf("  %s", rrseek(container, status));

	if (strmatch(status, statuses) && (ts = match(container, status)) == NULL)
	{
		Accept(m, container, pos, status);
		return true;
	}

	Reject(m, ts);
	return false;
}

void TCollectEvents::Reject(mstr &doc, const char *why)
{
	if (rejected < REJECTEDS_MAX)
	{
		if (rejected)
			m.cat(",\n");

		int i, ti;

		while ((i = doc.index("  ")) != INT_MAX)
		{
			i++;
			for (ti = i + 1; isspace(doc[ti]); ti++);
			mbk_remove(&doc, i, ti - i);
			mbk_insert(&doc, "- ", i, 2);
		}

		// 2009:287 LPR: why may be empty
		//m.printf("%s - %s", doc, why);
		m.cat(doc);
		if (any(why))
			m.printf(" - %s", why);
	}
	else if (rejected == REJECTEDS_MAX)
		m.cat(",\n..");

	rejected++;
}

#include "SelectEventDialog.h"
#include "SelectEventDialog.cpp"

#define _KIND_V_SESSIONS "\1\2"

TRCDKeyContainer *select_event(TWindow *parent, const TRCDKeyGroup *group, char kind, const CDate *date, const CTime *time,
	const char *statuses, match_func match, const char *what)
{
	char _kind = kind;	// for _ only

	if (kind == KIND_V_SURROUND)
		kind = KIND_ANOTHER_SURROUND;
	// for _ only
	else if (strchr(_KIND_V_SESSIONS, kind))
		kind = KIND_OTHER_OPEN_SESSION;

	TDatedContainer *container = kind2DatedContainer(kind);
	msql m(container->Name);

	*container << group;
	if (kind == KIND_ANOTHER_SURROUND || kind == KIND_PROVE)
	{
		m.AddLong("F_LAWSUIT_NO", container->key.no);
		m.AddLong("F_LAWSUIT_YEAR", container->key.year);
		m.AddChar("F_LAWSUIT_TYPE", container->key.type);
	}
	else
	{
		m.Add(container, "F_NO, F_YEAR, F_TYPE");

		switch (_kind)
		{
			case KIND_CLOSED_SESSION : m.AddChars("F_KIND", KIND_CLOSED_SESSIONS); break;
			case KIND_DECISION : m.AddChars("F_KIND", KIND_DECISIONS); break;
			case _KIND_V_ALL_SESSIONS : m.AddChars("F_KIND", KIND_SESSIONS); break;			// for _ only
			case _KIND_V_PREV_SESSIONS : m.AddChars("F_KIND", KIND_OPEN_SESSIONS); break;		// for _ only
			case KIND_INREG_USER_EVENT : m.AddChars("F_KIND", KIND_INREG_USER_EVENTS); break;	// 2017:156
			default : m.AddChar("F_KIND", kind);
		}
	}

	bool automatic = kind == KIND_PROVE;

	if (date && !date->Empty())
	{
		m.AddLong("F_DATE", date->Value());
		if (time && !time->Empty())
			m.AddLong("F_TIME", time->Value());
		automatic = true;
	}

	TTime *tTime = (TTime *) container->Find("F_TIME");
	m.AddOrder(tTime ? "F_DATE, F_TIME" : "F_DATE");

	TCollectEvents e(statuses, match);
	TQuery q(str(m));

	while (q.Read())
	{
		*container << q;
		m.clear();
		m.printf("%-25s  %-D", Kind->Seek(container->kind), &container->date);

		if (tTime)
		{
			if (tTime->Time().Empty())
				m.cat("      ");
			else
				m.printf(" %2d:%02d", tTime->Time().hour, tTime->Time().minute);
		}

		// 2020:022 LPR/TRQ: container sorting
		int pos = 0;
		while (pos < e.array.Count() && (*e.array[pos] <= *container))
			pos++;

		if (e.Collect(m, container, pos))
		{
			container = kind2DatedContainer(container->kind);
			tTime = (TTime *) container->Find("F_TIME");
		}
	}

	int choice = -1;

	if (e.array.Count())
	{
		if (automatic && e.array.Count() == 1)
			choice = 0;
		else
		{
			m.clear();
			m.printf("%s %ld/%ldг.", Type->Seek(group->key.type), group->key.no, group->key.year);

			if (what)
				m.printf(" - %s", what);

			if (TSelectEventDialog(parent, str(m), &e, choice, &e.array).Execute() == IDCANCEL)
				choice = -1;
		}
	}
	else
	{
		if (automatic && e.rejected == 0)
			say_no_data(container->Name);
		else if (automatic && e.rejected == 1)
			error("%s.", str(e.m));
		else if (e.rejected)
			error("Няма %s.\n\n%s.", what ? what : "обекти съответстващи на зададения критерий", str(e.m));
		else
			error("Няма %s.", what ? what : "намерени обекти");
	}

	if (choice >= 0)
		*container << e.array[choice];
	else
	{
		delete container;
		container = NULL;
	}

	return container;
}

// for _ only
static const char *match_prev(TRCDKeyGroup *group, char status)
{
	if (status == RESULT_FIXED)
		return wrong_status;
	if (((TSession *) group)->date > Today)
		return "Датата на заседанието е по-голяма от текущата дата";

	return NULL;
}

bool _select_session(TWindow *parent, TSession *session, char kind, const char *statuses)
{
	TRCDKeyContainer *const container = select_event(parent, session, kind, NULL, NULL, statuses,
		kind == _KIND_V_PREV_SESSIONS ? match_prev : match_null,
		kind == _KIND_V_PREV_SESSIONS ? "предходни открити заседания" : "заседания");
	TVirtPtr VP(container);

	if (container)
	{
		*session << *container;
		return true;
	}

	return false;
}

#include "SelectContainerDialog.h"
#include "SelectContainerDialog.cpp"

#if COURT_TYPE == COURT_AREAL
#include "P24a1TradeAlikeDialog.h"
#include "P24a1TradeAlikeDialog.cpp"

static const char *lawsuit_p24a1(TRCDKeyGroup *group, char)
{
	TLawsuit *lawsuit = TYPECHECK_PTR_CAST(TLawsuit, group);
	// 2016:053 IRQ/p.3 new subjects: comout subject liquidation
	return strchr(KIND_INCONSISTENCE_INDOCS, lawsuit->sourceKind) ? NULL : "Не е несъстоятелност/ликвидация";
}
#endif  // AREAL

static bool side_wind_exists(TGArray<TBaseWind> &found, const TBaseWind &sideWind, const char *kinds)
{
	for (int i = 0; i < found.Count(); i++)
	{
		TBaseWind *tSideWind = found[i];

		if (tSideWind->key == sideWind.key &&
			(!kinds || (tSideWind->kind == sideWind.kind && tSideWind->date == sideWind.date)) &&
			tSideWind->Excess() == sideWind.Excess())
		{
			return true;
		}
	}

	return false;
}

// ----- TSelectSidesArray ---------------------------------------------------
class TSelectSidesArray : public TGArray<TBaseWind>
{
public:
	TSelectSidesArray() { lesseql = LessEqual; }

protected:
	static bool LessEqual(const intptr_t value1, const intptr_t value2);
};

bool TSelectSidesArray::LessEqual(const intptr_t value1, const intptr_t value2)
{
	const TBaseWind *sideWind1 = (const TBaseWind *) value1;
	const TBaseWind *sideWind2 = (const TBaseWind *) value2;

	if ((sideWind1->key.type == TYPE_PROVE) != (sideWind2->key.type == TYPE_PROVE))
		return sideWind1->key.type != TYPE_PROVE;

	// N.B. date desc
	return sideWind1->date != sideWind2->date ? sideWind1->date > sideWind2->date :
		sideWind1->key.type != sideWind2->key.type ? sideWind1->key.type < sideWind2->key.type :
		sideWind1->key.no != sideWind2->key.no ? sideWind1->key.no < sideWind2->key.no :
		sideWind1->Excess() <= sideWind2->Excess();
}

// ----- select_container ----------------------------------------------------
bool select_container(TWindow *parent, TRCDKeyContainer *container, TUCNGroup *ucnGroup, const char *types,
	const char *kinds, const char *statuses, match_func match)
{
	TSideWind sideWind;
	msql m(sideWind.Name);
	bool query = !container;
	TWaitWindow *wait = new TWaitWindow(parent, WAIT);

	sideWind.ucnType = ucnGroup->ucnType;
	strcpy(sideWind.ucn, ucnGroup->ucn);
	m.Add(&sideWind, "F_UCN, F_UCN_TYPE");

	if (types)
		m.AddChars("F_TYPE", types);

	m.AddOrder("F_DATE DESC, F_TYPE, F_NO");

	TCollectEvents e(statuses, match);
	TQuery q(str(m));
	TGArray<TBaseWind> found;
	TSelectSidesArray sideWinds;
	TGArray<TRCDKeyContainer> basics;

	while (q.Read())
	{
		sideWind << q;

		// using catset(F_KIND) becomes too long
		if (!strmatch(sideWind.kind, kinds))
			continue;

		// 2010:055 LPR/LRQ: side appeared first in lawsuit/request IOReg
		if (query && strchr(TYPE_IOREGS, sideWind.key.type))
		{
			TIOReg *const ioReg = (TIOReg *) type2RCDKeyContainer(sideWind.key.type);
			TVirtPtr VP(ioReg);

			ioReg->key = sideWind.key;

			if (ioReg->Get("F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE") && ioReg->lawsuit.type &&
				strchr(TYPE_ORIGINS, ioReg->lawsuit.type))
			{
				sideWind.key = ioReg->lawsuit;
			}
		}

		if (side_wind_exists(found, sideWind, kinds))
			continue;

		TSideWind *tSideWind = new TSideWind;
		*tSideWind << sideWind;
		found.Add(tSideWind);

		TRCDKeyContainer *basic = NULL;

		for (int i = 0; i < basics.Count(); i++)
		{
			if (basics[i]->key == sideWind.key)
			{
				basic = type2RCDKeyContainer(sideWind.key.type);
				*basic << basics[i];

				TString *extraText = basics[i]->FindExtraText();
				if (extraText)
					basic->AddExtraText(extraText->S());

				break;
			}
		}

		if (!basic)
		{
			mstr cause;

			if ((basic = can_access_basic(sideWind.key, !query, !query, &cause)) == NULL)
			{
				m.clear();
				m.printf("%s %ld/%ldг.", Type->Seek(sideWind.key.type), sideWind.key.no, sideWind.key.year);
				e.Reject(m, str(cause));
				continue;
			}

			if (any(cause))
				basic->AddExtraText(str(cause));
		}

		TRCDKeyContainer *extra;	// basic or event

		// 2009:174 check if kind already matches
		// 2009:237 sideWind kind -> basic kind
		if (kinds && !strchr(kinds, basic->kind))
		{
			extra = kind2DatedContainer(sideWind.kind);
			*extra << sideWind;
			if (!extra->Get())
			{
				delete basic;
				delete extra;
				continue;
			}
			// 2009:288 LPR: cut/extend kind name
			m.cpy(Kind->Seek(extra->kind));
			m.setlen(25);
			m.printf("%5ld/%4ld  %-D", sideWind.key.no, sideWind.key.year, &extra->date);
		}
		else
		{
			extra = type2RCDKeyContainer(sideWind.key.type);
			*extra << basic;
			// 2009:288 LPR: cut/extend kind name
			m.cpy(Kind->Seek(extra->kind));
			m.setlen(25);
			m.printf("%5ld/%4ld", sideWind.key.no, sideWind.key.year);
			// 2009:160 LPR: for print_sidewinds()
			sideWind.kind = basic->kind;
		}

		m.printf("  %-20s", Involvement->Seek(sideWind.involvement));

		tSideWind = new TSideWind;
		*tSideWind << sideWind;
		int pos = sideWinds.Add(tSideWind);

		if (e.Collect(m, extra, pos))
			basics.Insert(basic, pos);
		else
		{
			delete basic;
			delete extra;
			sideWinds.Remove(pos);
		}
	}

	// 2017:167
	if (query && COURT_CODE == COURT_SOFGS)
	{
		TExternSideWind tSideWind;

		m.Begin(tSideWind.Name);
		m.Add(&sideWind, "F_UCN, F_UCN_TYPE");

		if (types)
			m.AddChars("F_TYPE", types);

		m.AddOrder("F_DATE DESC, F_TYPE, F_NO");

		TQuery q(str(m));
		while (q.Read())
		{
			tSideWind << q;

			if (side_wind_exists(found, tSideWind, kinds))
				continue;

			found.AddCopy(&tSideWind);

			TExternLawsuit *lawsuit = new TExternLawsuit;
			lawsuit->key = tSideWind.key;

			if (lawsuit->Get())
			{
				int pos = sideWinds.AddCopy(&tSideWind);

				m.cpy(Kind->Seek(lawsuit->kind));
				m.setlen(23);
				m.printf("С %5ld/%4ld", tSideWind.key.no, tSideWind.key.year);
				m.printf("  %-20s", Involvement->Seek(tSideWind.involvement));
				// TODO? better status
				e.Accept(m, lawsuit, pos, RESULT_FAKE);
				basics.InsertCopy(lawsuit, pos);
			}
			else
				delete lawsuit;
		}
	}

	bool result = false;
	delete wait;

	if (e.array.Count())
	{
		int choice;
		int resId = kinds ? IDD_SELECT_CONTAINER : IDD_SELECT_EVENT;

		m.clear();
		sideWind.PrintGroup(m);
	#if COURT_TYPE == COURT_AREAL
		if (match == lawsuit_p24a1)
		{
			if (TP24a1TradeAlikeDialog(parent, str(m), &e, &basics, &sideWinds).Execute() == IDOK)
				result = true;
		}
		else
	#endif  // AREAL
		if (query)
		{
			while (TSelectContainerDialog(parent, str(m), &e, choice, NULL, &sideWinds, resId).Execute() == IDOK)
				query_group(parent, basics[choice]);
		}
		else if (TSelectContainerDialog(parent, str(m), &e, choice, &basics, &sideWinds, resId).Execute() == IDOK)
		{
			*container << e.array[choice];
			result = true;
			SetPrimaryGroup(basics.Detach(choice));	// 2009:184
		}
	}
#if COURT_TYPE == COURT_AREAL
	else if (match == lawsuit_p24a1)
		result = true;
#endif  // AREAL
	else if (e.rejected)
		error("Няма обекти съответстващи на зададения критерий.\n\n%s.", str(e.m));
	else
		error("Няма намерени обекти.");

	basics.DeleteExtraText();
	return result;
}

#if COURT_TYPE == COURT_AREAL
bool p24a1tradealike(TWindow *parent, TUCNGroup *ucnGroup)
{
	return select_container(parent, NULL, ucnGroup, TYPE_TRADEALIKE_LAWS, NULL, NULL, lawsuit_p24a1);
}

bool select_citizenwind(TWindow *parent, TSideWind *sideWind, TLawsuit *lawsuit)
{
	TGArray<TSideWind> sideWinds;
	TStringArray strings;
	msql m(sideWind->Name, "F_UCN, F_UCN_TYPE, F_INVOLVEMENT");

	m.Add(lawsuit, "F_NO, F_YEAR, F_TYPE, F_KIND");
	m.AddChars("F_UCN_TYPE", UCN_CITIZENS);

	TQuery q(str(m));
	while (q.Read())
	{
		TSideWind *tSideWind = new TSideWind;

		*tSideWind << q;
		sideWinds.Add(tSideWind);
		m.clear();
		tSideWind->Print(m);
		strings.Add(m.newstr());
	}

	m.clear();
	lawsuit->Print(m);
	int choice = select_string(parent, &strings, "Избор на страна", str(m));

	if (choice < 0)
		return false;

	*sideWind << sideWinds[choice];
	return true;
}
#endif  // AREAL

#include "SelectUCLPListBox.h"
#include "SelectUCLPListBox.cpp"

#include "SelectUCLPDialog.h"
#include "SelectUCLPDialog.cpp"

bool select_uclp(TWindow *parent, TUCLP *uclp)
{
	if (!any(uclp->name))
		return error("Критерия населено място '%s' не е валиден. Коригирайте го.", uclp->name);

	if (strlen(uclp->name) - (strchr(uclp->name, '.') != NULL) < 3)
		return error("Критерия населено място '%s' е прекалено общ. Уточнете го.", uclp->name);

	return TSelectUCLPDialog(parent, uclp).Execute() == IDOK;
}

#include "SelectStreetDialog.h"
#include "SelectStreetDialog.cpp"

bool select_street(TWindow *parent, TStreet *street)
{
	return TSelectStreetDialog(parent, street).Execute() == IDOK;
}

bool select_send_inreg(TWindow *parent, TInReg *inReg, TInReg *tInReg, TGArray<TInRegEvent> *already)
{
	msql m;
	char status = inReg->Status();

	inReg->FloatCriteria(m);
	m.AddOrder("F_YEAR, F_NO");

	TCollectEvents e(RESULT_SENDABLES, match_null);
	TQuery q(str(m));
	while (q.Read())
	{
		TInReg *tInReg = new TInReg;

		*tInReg << q;
		m.clear();
		tInReg->Print(m);

		const char *result = tInReg->SameComplect(inReg, status, tInReg->Status());

		if (result)
		{
			e.Reject(m, result);
			delete tInReg;
		}
		else
		{
			int i;

			for (i = 0; i < already->Count(); i++)
				if ((*already)[i]->key == tInReg->key)
					break;

			if (i == already->Count())
				e.Collect(m, tInReg, e.array.Count());
			else
			{
				e.Reject(m, "Вече е в списъка за изпращане");
				delete tInReg;
			}
		}
	}

	int choice;

	m.clear();
	m.printf("Документи по %s %ld/%ldг.", Type->Seek(inReg->lawsuit.type), inReg->lawsuit.no, inReg->lawsuit.year);

	if (TSelectEventDialog(parent, str(m), &e, choice, &e.array).Execute() == IDOK)
	{
		*tInReg << e.array[choice];
		return true;
	}

	return false;
}
