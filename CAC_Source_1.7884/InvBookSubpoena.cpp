#include "cac.h"

// ----- TInvBookSubpoenaGroup -----------------------------------------------
class TInvBookSubpoenaGroup : public TBookGroup
{
public:
	TInvBookSubpoenaGroup();

	// 2014:213 FB2: nested selects -> join
	void SubpoenaCriteria(msql &m, const char *fields);
	void InRegCriteria(msql &m, const char *fields);
	void AnnounceCriteria(msql &m, const char *fields);
	void FinishCriteria(msql &m);

	char type;
	char compositions[SIZE_OF_COMPOSITIONS];
	long receivedOnly;
	TGArray<TUserAliasGroup> deliverers;
};

TInvBookSubpoenaGroup::TInvBookSubpoenaGroup()
	: TBookGroup("T_BOOK", "F_TYPE", "F_DATE", false)
{
	Add(
		new TCharAlias("F_TYPE", &type),
		new TCharList("F_COMPOSITIONS", compositions, sizeof compositions),
		new TRadio("F_BY_SUBPOENA_DATE", &receivedOnly, FALSE),
		new TRadio("F_BY_RECEIVED_DATE", &receivedOnly, TRUE),
		new TCheck("F_NEW_FORMAT", &flags, BOOK_NEW_FORMAT),
		NULL
	);

	type = kind2type(Default->kind);
}

void TInvBookSubpoenaGroup::SubpoenaCriteria(msql &m, const char *fields)
{
	mstr name("T_SUBPOENA T JOIN T_SUBPOENA_KIND K ON K.F_SUBPOENA_KIND = T.F_SUBPOENA_KIND");

	if (type != TYPE_REQUEST)
		name.cat(" JOIN T_LAWSUIT L ON L.F_NO = T.F_NO AND L.F_YEAR = T.F_YEAR AND L.F_TYPE = T.F_TYPE");

	m.Begin(str(name), fields);
	internal_subpoena_uclp_lead(m);
	m.AddChar("T.F_TYPE", type);
	m.AddIsNull("K.F_GENERIC_KIND", false);
	m.AddNonElectron("T.");
	m.AddCompos("L.F_COMPOSITION", compositions);
	FinishCriteria(m);
}

void TInvBookSubpoenaGroup::InRegCriteria(msql &m, const char *fields)
{
	m.Begin("T_SUBPOENA T JOIN T_SUBPOENA_KIND K ON K.F_SUBPOENA_KIND = T.F_SUBPOENA_KIND JOIN T_INREG I "
		"ON I.F_NO = T.F_NO AND I.F_YEAR = T.F_YEAR AND I.F_TYPE = T.F_TYPE JOIN T_LAWSUIT L "
		"ON L.F_NO = I.F_LAWSUIT_NO AND L.F_YEAR = I.F_LAWSUIT_YEAR AND L.F_TYPE = I.F_LAWSUIT_TYPE",
		fields);

	m.AddChar("T.F_TYPE", TYPE_INREG);
	m.AddChar("I.F_LAWSUIT_TYPE", type);
	m.AddIsNull("K.F_GENERIC_KIND", false);
	internal_subpoena_uclp_lead(m);
	m.AddNonElectron("T.");

	if (type != TYPE_REQUEST)
		m.AddCompos("L.F_COMPOSITION", compositions);

	FinishCriteria(m);
}

void TInvBookSubpoenaGroup::AnnounceCriteria(msql &m, const char *fields)
{
	m.Begin("T_ANNOUNCE T", fields);
	m.AddChar("T.F_KIND", type2ConnectKind(type));
	m.AddLong("T.F_COURT", COURT_CODE);
	FinishCriteria(m);
}

void TInvBookSubpoenaGroup::FinishCriteria(msql &m)
{
	if (receivedOnly)
	{
		m.AddChars("T.F_RECEIVED_STATUS", SRS_DELIV_BASICS);
		m.AddRange("T.F_SUBPOENA_RECEIVED", minDate, maxDate);
	}
	else
		m.AddRange("T.F_SUBPOENA_DATE", minDate, maxDate);

	m.AddArray("F_UCN", deliverers, "T.F_DELIVERER_UCN");
}

// ----- TSubpoenaAnnounce ---------------------------------------------------
class TSubpoenaAnnounce : public TAnnounce
{
public:
	TSubpoenaAnnounce() : repeat(1) { }

	mstr list;
	int repeat;

	bool operator<=(const TSubpoenaAnnounce &that) const;
};

bool TSubpoenaAnnounce::operator<=(const TSubpoenaAnnounce &that) const
{
	int compare = subpoenaDate.Compare(that.subpoenaDate);

	return compare ? compare < 0 : subpoenaKind != that.subpoenaKind ? subpoenaKind < that.subpoenaKind :
		key.year != that.key.year ? key.year < that.key.year : key.no <= that.key.no;
}

#define announce OFFICE_GROUP_CAST(TSubpoenaAnnounce)

static void invSubpoena_subpoena_date(mstr &m, TGroup *group) { announce->subpoenaDate.Print(m); }
static void invSubpoena_subpoena_generic_kind(mstr &m, TGroup *group) { m.cat(SubpoenaKind->Seek(announce->subpoenaKind)); }
static void invSubpoena_list(mstr &m, TGroup *group) { m.cat(announce->list); }

#undef announce

static const offield invSubpoena_fields[]=
{
	{ "F_DATE", invSubpoena_subpoena_date },
	{ "F_SUBPOENA_GENERIC_KIND", invSubpoena_subpoena_generic_kind },
	{ "F_LIST", invSubpoena_list },
	{ NULL, NULL }
};

#define invGroup OFFICE_GROUP_CAST(TInvBookSubpoenaGroup)

static void invSubpoenaTitle_title_text(mstr &m, TGroup *group) { m.cat(invGroup->receivedOnly ? "ÂÐÚ×ÅÍÈ" : ""); }
static void invSubpoenaTitle_connect_kind(mstr &m, TGroup *group) { m.cat(ConnectKind->Seek(type2ConnectKind(invGroup->type))); }
static void invSubpoenaTitle_compositions(mstr &m, TGroup *group) { print_compositions_list(m, invGroup->compositions); }

#undef invGroup

static const offield invSubpoenaTitle_fields[]=
{
	{ "F_TITLE_TEXT", invSubpoenaTitle_title_text },
	{ "F_CONNECT_KIND", invSubpoenaTitle_connect_kind },
	{ "F_COMPOSITIONS", invSubpoenaTitle_compositions },
	{ NULL, NULL }
};

// ----- TInvSubpoenaBook ----------------------------------------------------
class TInvSubpoenaBook : public TBook
{
public:
	TInvSubpoenaBook(TWindow *parent, TInvBookSubpoenaGroup *tGroup);

	const char *tr1break;
	TGSArray<TSubpoenaAnnounce> announces;

	void CreateList();
	void ReadSubpoenas(const char *criteria);
	virtual void Collect();
	virtual void Markers();
	void SortedArray(TSubpoenaAnnounce *announce);
	virtual void EmitRow(TGroup *group);
	virtual TGroup *Process(TQuery &);
};

TInvSubpoenaBook::TInvSubpoenaBook(TWindow *parent, TInvBookSubpoenaGroup *tGroup)
	: TBook(parent, tGroup, "InvB_Subp.htm", invSubpoena_fields, invSubpoenaTitle_fields, BOOK_FIXED_DATES)
{
}

TGroup *TInvSubpoenaBook::Process(TQuery &)
{
	return NULL;
}

void TInvSubpoenaBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr1break = office.Find("tr1 break");
	tr1final = office.Find("tr1 final");
}

void TInvSubpoenaBook::SortedArray(TSubpoenaAnnounce *announce)
{
	int index = announces.Find(announce);
	TRCDKey key;

	key.no = announce->key.no;
	key.year = announce->key.year;
	key.type = connectKind2Type(announce->key.kind);

	if (index == INT_MAX)
	{
		announce->list.printf("%ld/%ld", flag_key_no(&key), key.year);
		announces.Add(announce);
	}
	else
	{
		delete announce;
		announce = announces[index];
		announce->repeat++;
		announce->list.clear();
		announce->list.printf("%ld/%ld - %d", flag_key_no(&key), key.year, announce->repeat);
	}
}

void TInvSubpoenaBook::CreateList()
{
	int i = 0;

	while (i != announces.Count() - 1 && announces.Count() > 1)
	{
		if (announces[i]->subpoenaDate == announces[i + 1]->subpoenaDate &&
			announces[i]->subpoenaKind == announces[i + 1]->subpoenaKind)
		{
			announces[i]->list.printf(", %s", str(announces[i + 1]->list));
			announces.Remove(i + 1);
		}
		else
			i++;
	}
}

void TInvSubpoenaBook::ReadSubpoenas(const char *criteria)
{
	TInvBookSubpoenaGroup *tGroup = (TInvBookSubpoenaGroup *) group;
	TQuery q(criteria);

	while (q.Read())
	{

		TSubpoena *subpoena = new TSubpoena;
		*subpoena << q;

		if (subpoena->HasRegards())
			delete subpoena;
		else
		{
			TSubpoenaAnnounce *announce = new TSubpoenaAnnounce;
			announce->SetSubpoena(subpoena);

			if (tGroup->receivedOnly)
				announce->subpoenaDate = announce->subpoenaReceived;

			SortedArray(announce);
		}
	}
}

void TInvSubpoenaBook::Collect()
{
	static constant SUBPOENA_FIELDS = "T.F_NO, T.F_YEAR, T.F_SUBPOENA_DATE, T.F_SUBPOENA_RECEIVED, T.F_KIND, "
		"T.F_DATE, T.F_TYPE, T.F_UCN_TYPE, T.F_UCN, T.F_INVOLVEMENT, T.F_SUBPOENA_NO, T.F_EVENT_STATUS, "
		"K.F_GENERIC_KIND AS F_SUBPOENA_KIND";
	TInvBookSubpoenaGroup *tGroup = (TInvBookSubpoenaGroup *) group;
	msql m;

	// SUBPOENA
	{
		tGroup->SubpoenaCriteria(m, SUBPOENA_FIELDS);
		ReadSubpoenas(str(m));
	}

	// INREG
	{
		tGroup->InRegCriteria(m, SUBPOENA_FIELDS);
		ReadSubpoenas(str(m));
	}

	// ANNOUNCE
	{
		tGroup->AnnounceCriteria(m, "T.F_NO, T.F_YEAR, T.F_SUBPOENA_DATE, T.F_SUBPOENA_RECEIVED, T.F_KIND, "
			"T.F_COURT, T.F_SUBPOENA_KIND");
		TQuery q(str(m));

		while (q.Read())
		{
			TSubpoenaAnnounce *announce = new TSubpoenaAnnounce;
			*announce << q;

			if (tGroup->receivedOnly)
				announce->subpoenaDate = announce->subpoenaReceived;

			SortedArray(announce);
		}
	}

	CreateList();
	array->Add(new TAnnounce);
}

void TInvSubpoenaBook::EmitRow(TGroup *)
{
	CDate date = CDate(0, 0, 0);

	for (int i = 0; i < announces.Count(); i++)
	{
		if (announces[i]->subpoenaDate != date || !i)
			office.Emit(tr1start, tr1break, invSubpoena_fields, announces[i]);

		office.Emit(tr1break, tr1final, invSubpoena_fields, announces[i]);
		date = announces[i]->subpoenaDate;
	}
}

// ----- TSubpoenaDescBook ---------------------------------------------------
class TSubpoenaDescBook : public TBook
{
public:
	TSubpoenaDescBook(TWindow *parent, TInvBookSubpoenaGroup *tGroup);

protected:
	class TAnSArray : public TGroupArray
	{
	public:
		TAnSArray() { lesseql = LessEqlT; }

	protected:
		static bool LessEqlT(const long t1, const long t2);
	};

	const char *th1start;

	virtual void Markers();
	void ReadSubpoenas(const char *criteria);
	virtual void Collect();
	virtual void EmitBody();
	virtual TGroup *Process(TQuery &);
};

TSubpoenaDescBook::TSubpoenaDescBook(TWindow *parent, TInvBookSubpoenaGroup *tGroup)
	: TBook(parent, tGroup, "DescB_Subp.htm", invBookAnno_fields, invSubpoenaTitle_fields, BOOK_FIXED_DATES, new TAnSArray)
{
}

bool TSubpoenaDescBook::TAnSArray::LessEqlT(const long t1, const long t2)
{
	const TAnnounce *a1 = (const TAnnounce *) t1;
	const TAnnounce *a2 = (const TAnnounce *) t2;

	return a1->subpoenaDate != a2->subpoenaDate ? a1->subpoenaDate < a2->subpoenaDate :
		a1->key.kind != a2->key.kind ? a1->key.kind < a2->key.kind : a1->key.no <= a2->key.no;
}

void TSubpoenaDescBook::Markers()
{
	th1start = office.Find("th1 start");
	TBook::Markers();
}

void TSubpoenaDescBook::ReadSubpoenas(const char *criteria)
{
	TQuery q(criteria);

	while (q.Read())
	{
		TAnnounce *announce = new TAnnounce;
		TSubpoena *subpoena = new TSubpoena;

		*subpoena << q;
		announce->SetSubpoena(subpoena);
		array->Add(announce);
	}
}

void TSubpoenaDescBook::Collect()
{
	TInvBookSubpoenaGroup *tGroup = (TInvBookSubpoenaGroup *) group;
	msql m;

	// SUBPOENAS
	{
		tGroup->SubpoenaCriteria(m, "T.*");
		ReadSubpoenas(str(m));
	}
	// INREGS
	{
		tGroup->InRegCriteria(m, "T.*");
		ReadSubpoenas(str(m));
	}
	// ANNOUNCES
	{
		tGroup->AnnounceCriteria(m, "T.*");
		TAnnounce().LoadArray(array, str(m));
	}
}

void TSubpoenaDescBook::EmitBody()
{
	CDate lastSubpoenaDate;

	office.Emit(NULL, th1start, book_fields, group);

	for (int i = 0; i < array->Count(); i++)
	{
		TAnnounce *announce = (TAnnounce *) (*array)[i];

		if (announce->subpoenaDate != lastSubpoenaDate)
		{
			office.Emit(th1start, tr1start, body_fields, announce);
			lastSubpoenaDate = announce->subpoenaDate;
		}

		office.Emit(tr1start, tr1final, body_fields, announce);
	}

	office.Emit(tr1final, NULL, book_fields, group);
}

TGroup *TSubpoenaDescBook::Process(TQuery &)
{
	return NULL;
}

// ----- invSuboena_book -----------------------------------------------------
#include "InvBookSubpoenaDialog.h"
#include "InvBookSubpoenaDialog.cpp"

void invSubpoena_book(TWindow *parent)
{
	TInvBookSubpoenaGroup group;

	if (TInvBookSubpoenaDialog(parent, &group).Execute() == IDOK)
	{
		if (group.flags & BOOK_NEW_FORMAT)
			TSubpoenaDescBook(parent, &group).Execute(BOOK_EXEC_HEADER);
		else
			TInvSubpoenaBook(parent, &group).Execute(BOOK_EXEC_HEADER);
	}
}
