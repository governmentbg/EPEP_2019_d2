#include "cac.h"

#define person OFFICE_GROUP_CAST(TKnownPerson)

static void person_pedname(mstr &m, TGroup *group)
{
	person->PedName(m, true);

	if (person->csjid)
		m.printf("\vРег.№ %ld", person->csjid);
}

static void person_ucn(mstr &m, TGroup *group)
{
	if (is_real_ucn(person))
		m.cat(person->ucn);
	else if (strchr(UCN_FIRMS, person->ucnType))
	{
		TFirm firm;
		firm << person;

		if (firm.Try("F_BULSTAT"))
			m.cat(firm.bulstat);
	}
}

static void person_address(mstr &m, TGroup *group) { print_address(m, person, 1, ADDRESS_DISTINCT); }

static void person_title(mstr &m, TGroup *group)
{
	m.printf("%s - %s", Involvement->Seek(person->involvement), person->flags & FLAG_ACTIVE ? "активни" : "всички");
}

#undef person

static const offield person_fields[] =
{
	{ "F_NAME",		person_pedname },
	{ "F_UCN",		person_ucn },
	{ "F_ADDRESS",	person_address },
	{ "F_TITLE",	person_title },
	{ NULL,		NULL}
};

static void print_knowns(TWindow *parent, TGroupArray *array, char involvement, long active)
{
	TOffice office;

	if (office.Open("KnowList.htm"))
	{
	#if SPEEDTESTING
		#pragma message speed testing only
		DWORD ticks = GetTickCount();
	#endif  // SPEEDTESTING
		TWaitWindow wait(parent, WAIT);
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");
		TKnownPerson person;

		person.involvement = involvement;
		person.flags = active;
		office.Emit(NULL, tr1start, person_fields, &person);

		for (int i = 0; i < array->Count(); i++)
		{
			person << (*array)[i];

			if ((person.flags & FLAG_ACTIVE) >= active)
			{
				office.Emit(tr1start, tr1final, person_fields, &person);
				wait.TickCount(i + 1, "обработени лица");
			}
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
	#if SPEEDTESTING
		message("%lu seconds\n", (GetTickCount() - ticks) / 1000);
	#endif  // SPEEDTESTING
		office.Show(&wait);
	}
}

#define person OFFICE_GROUP_CAST(TKnownPerson)

static void person_pedagog(mstr &m, TGroup *group)
{
	m.cpy(person->involvement == INVOLVEMENT_JUROR && (person->flags & JUROR_PEDAGOG) ? "п" : "");
}

#undef person

class TCountPerson : public TKnownPerson
{
public:
	TCountPerson();

	void Fixup() { print_user(name, ucn, ucnType); }
	bool operator<=(const TCountPerson &that) const { return count <= that.count; }

	char name[0x100];
	long count;
};

TCountPerson::TCountPerson()
{
	Add(
		new TString("F_NAME", name, sizeof name),
		new TLong("F_COUNT", &count),
		NULL
	);
}

static const offield percount_fields[] =
{
	{ "F_NAME", NULL },
	{ "F_UCN", NULL },
	{ "F_COUNT", NULL },
	{ NULL, NULL }
};

static void count_lawer_cases(TWindow *parent)
{
	TWaitWindow wait(parent, WAIT);
	TOffice office;

	if (office.Open("LawerCases.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		TKnownPerson known;
		msql m;
		TGSArray<TCountPerson> personList;

		known.involvement = INVOLVEMENT_LEGALESE;
		known.FloatCriteria(m, "F_UCN, F_UCN_TYPE");
		TQuery q(str(m));

		while (q.Read())
		{
			TCountPerson *person = new TCountPerson;

			*person << q;
			person->Fixup();
			wait.TickText(person->name);

			m.clear();
			m.Begin("T_SIDE_WIND", "DISTINCT F_NO, F_YEAR");
			m.Add(person, "F_UCN, F_UCN_TYPE");
			m.AddChars("F_INVOLVEMENT", INVOLVEMENT_LAWERS);
			m.AddChars("F_TYPE", TYPE_LAWSUITS);

			TQuery r(str(m));
			while (r.Read())
				person->count++;

			if (person->count)
				personList.Add(person);
			else
				delete person;
		}

		office.Emit(NULL, tr1start, null_fields, NULL);
		for (int i = 0; i < personList.Count(); i++)
			office.Emit(tr1start, tr1final, percount_fields, personList[i]);
		office.Emit(tr1final, NULL, null_fields, NULL);

		office.Show(&wait);
	}
}

#include "EditCSJIDDialog.h"
#include "EditCSJIDDialog.cpp"

// 2017:023 IRQ/FIX: non-citizen F_NAME unused / causes overflow
// 2017:116 TRQ/FIX: use in both add and edit
static void trans2known(TKnownPerson *person, TTransGroup &ucnGroup)
{
	if (!strchr(UCN_CITIZENS, ucnGroup.ucnType))
		*ucnGroup.name = '\0';

	*person << ucnGroup;
}

static bool add_known_person(TWindow *parent, TKnownPerson *person)
{
	// 2010:279 FIX: TCitizen -> TTransGroup
	TTransGroup ucnGroup;
	const char *ucnTypes = UCN_CITIZENS;

	// 2006:333 real persons support
	// 2010:274 LRQ: legalese may be firm
	if (strchr(INVOLVEMENT_REAL_PERSONS, person->involvement))
		ucnTypes = C2S[UCN_CITIZEN_UCN];
	else if (person->involvement == INVOLVEMENT_LEGALESE)
		ucnTypes = UCN_REAL_SIDES;

	ucnGroup.involvement = person->involvement;

	if (get_ucn(parent, &ucnGroup, ucnTypes, true))
	{
		trans2known(person, ucnGroup);

		if (person->Insert())
		{
			if (strchr(INVOLVEMENT_CSJIDS, person->involvement))
				TSender(person).Insert();

			return true;
		}
	}

	return false;
}

static bool edit_known_person(TWindow *parent, TKnownPerson *person)
{
	TTransGroup ucnGroup;
	ucnGroup << person;

	if (edit_ucn(parent, &ucnGroup, NULL))
	{
		trans2known(person, ucnGroup);
		edit_addresses(parent, person, ADDRESS_BULGARIAN_ONLY);

		if (strchr(INVOLVEMENT_CSJIDS, person->involvement))
			TSender(person).Update();

		return true;
	}

	return false;
}

#include "KnownPersonListWindow.h"
#include "KnownPersonListWindow.cpp"

#include "KnownPersonListDialog.h"
#include "KnownPersonListDialog.cpp"

void edit_knowns(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_KNOWN_PERSONS, "участници");
	TKnownPersonListDialog(parent).Execute();
}
