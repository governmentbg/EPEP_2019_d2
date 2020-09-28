#include "cac.h"

// ----- TFloatSubpoenaGroup -------------------------------------------------
class TFloatSubpoenaGroup : public TFloatGroup
{
public:
	TFloatSubpoenaGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	char lawKinds[SIZE_OF_LAW_KINDS];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	char kinds[SIZE_OF_KINDS];
	CDate minSessionDate;
	CDate maxSessionDate;
	CDate minSubpoenaDate;
	CDate maxSubpoenaDate;
	char subpoenaKinds[SIZE_OF_SUBPOENA_KINDS];
	char receivedStatuses[SIZE_OF_RECEIVED_STATUSES];
	long firmData, multy, nameListWind, noLetter;
	TGArray<TUserAliasGroup> deliverers;
	CDate minReturnedDate;
	CDate maxReturnedDate;
	long flags;
	long courier;
	long fax;
	long email;
	TGArray<TUserAliasGroup> users;	// 2014:238 +related

	bool Deliverer() { return deliverers.Count() || order == ORDER_BY_NMTYN; }
	bool Announces() { return courier <= 0 && fax <= 0 && email <= 0; }
};

TFloatSubpoenaGroup::TFloatSubpoenaGroup()
	: TFloatGroup("T_SUBPOENA")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCharList("F_KINDS", kinds, sizeof kinds),
		new TDate("F_MIN_SESSION_DATE", &minSessionDate, false),
		new TDate("F_MAX_SESSION_DATE", &maxSessionDate, false),
		new TDate("F_MIN_SUBPOENA_DATE", &minSubpoenaDate, false),
		new TDate("F_MAX_SUBPOENA_DATE", &maxSubpoenaDate, false),
		new TCharList("F_SUBPOENA_KINDS", subpoenaKinds, sizeof subpoenaKinds),
		new TCharList("F_RECEIVED_STATUSES", receivedStatuses, sizeof receivedStatuses),
		new TCheck("F_FIRM_LAWSUIT", &firmData, TRUE),
		new TCheck("F_MULTY", &multy, TRUE),
		new TCheck("F_NAME_LIST_WIND", &nameListWind, TRUE),
		new TRadio("F_BY_SDTYN", &order, ORDER_BY_SDTYN),
		new TRadio("F_BY_NMTYN", &order, ORDER_BY_NMTYN),
		new TDate("F_MIN_RETURNED_DATE", &minReturnedDate, false),
		new TDate("F_MAX_RETURNED_DATE", &maxReturnedDate, false),
		new TRadio("F_FLAGS_NONE", &flags, OUTREG_NONE),
		new TRadio("F_FLAGS_REGULAR", &flags, OUTREG_REGULAR),
		new TRadio("F_FLAGS_R", &flags, OUTREG_R),
		new TRadio("F_FLAGS_AR", &flags, OUTREG_AR),
		new TRadio("F_FLAGS_PACKAGE", &flags, OUTREG_PACKAGE),
		new TCheck("F_FLAGS_COURIER", &courier, OUTREG_X_COURIER),
		new TCheck("F_FLAGS_FAX", &fax, OUTREG_X_FAX),
		new TCheck("F_FLAGS_EMAIL", &email, OUTREG_X_EMAIL),
		new TCheck("F_NOLETTER", &noLetter, TRUE),
		NULL
	);

	flags = OUTREG_NONE;
	courier = fax = email = noLetter = -1;
	order = ORDER_BY_DTYN;
}

void TFloatSubpoenaGroup::FormCriteria(msql &m)
{
	mstr name("T_SUBPOENA S");
	mstr fields("S.*");

	// 2011:178 FIX: +outreg no/year; 2012:054 FIX: +address fields
	// 2012:130 +subpoena received; 2012:265 FIX: +F_ADR_NO

	if (Deliverer())
	{
		name.printf(" LEFT JOIN T_CITIZEN C ON C.F_UCN = S.F_DELIVERER_UCN AND C.F_UCN_TYPE = '%c'", UCN_CITIZEN_UCN);
		fields.cat(", C.F_NAME, C.F_RENAME, C.F_FAMILY, C.F_REFAMILY, C.F_RANK");
	}

	if (noLetter != -1)
		name.cat(" JOIN T_SUBPOENA_KIND K ON K.F_SUBPOENA_KIND = S.F_SUBPOENA_KIND");

	m.Begin(str(name), str(fields));

	m.AddChars("S.F_TYPE", types, TYPE_SUBPOENABLES);
	m.AddRange("S.F_NO", minNo, maxNo);
	m.AddRange("S.F_YEAR", minYear, maxYear);
	m.AddChars("S.F_KIND", adjust_xxx_kinds(adjust_xxx_kinds(kinds, KIND_VISIBLE_DECISIONS, KIND_DECISIONS),
		KIND_VISIBLE_CLOSEDS, KIND_CLOSED_SESSIONS), KIND_SUBPOENABLES);
	m.AddRange("S.F_DATE", minSessionDate, maxSessionDate);
	m.AddRange("S.F_SUBPOENA_DATE", minSubpoenaDate, maxSubpoenaDate);
	m.AddChars("S.F_SUBPOENA_KIND", adjust_subpoena_kinds(subpoenaKinds), SUBPOENA_KIND_ALLS);
	m.AddChars("S.F_RECEIVED_STATUS", receivedStatuses, SRS_SUBPOENAS);
	m.AddRange("S.F_SUBPOENA_RETURNED", minReturnedDate, maxReturnedDate);

	// 2014:064 LPR/IRQ: copied from floatarchive indexes
	// 2015:071 LPR: moved to msql; 2015:168 -> AddArray
	m.AddArray("F_UCN", deliverers, "S.F_DELIVERER_UCN");

	if (multy)
		m.AddChar("NOT S.F_MULTI", SUBPOENA_MULTI);

	if (flags)
		m.AddFlags("S.F_BY_POST", OUTREG_POSTAL, flags, OUTREG_POSTAL);

	long mask = 0, value = 0;

	if (courier != -1)
	{
		mask |= OUTREG_X_COURIER;
		value |= courier;
	}

	if (fax != -1)
	{
		mask |= OUTREG_X_FAX;
		value |= fax;
	}

	if (email != -1)
	{
		mask |= OUTREG_X_EMAIL;
		value |= email;
	}

	m.AddFlags("S.F_BY_POST_X", mask, value, OUTREG_X_ALL);
	m.AddArray("F_UCN", users, "S.F_USER_UCN");

	if (noLetter != -1)
	{
		m.AddLike("K.F_ATTRIBUTES", C2S[SK_ATTR_LETTER_ALLOWED], true);
		m.Add("(SELECT COUNT(L.F_NO) FROM T_SUBPOENA L WHERE L.F_NO = S.F_NO AND L.F_YEAR = S.F_YEAR "
			"AND L.F_TYPE = S.F_TYPE AND L.F_KIND = S.F_KIND AND L.F_DATE = S.F_DATE "
			"AND L.F_EVENT_STATUS = S.F_EVENT_STATUS AND L.F_REGARD = S.F_SUBPOENA_NO");
		m.printf(") %c 0", noLetter ? '=' : '>');
	}
}

// ----- TFloatSubpoena ------------------------------------------------------
class TFloatSubpoena : public TSubpoena, public TCitizenNames
{
public:
	TFloatSubpoena() : TCitizenNames(this) { }

	TRCDKey origin;
	long nameListWind;
	char originKind;  // Print Name or ListWind
	TDamn damn;

	static bool LessEql1(const long t1, const long t2);
	static bool LessEql2(const long t1, const long t2);
	static bool LessEql3(const long t1, const long t2);
	static bool LessEql4(const long t1, const long t2);
	bool operator<=(const TFloatSubpoena &that) { return LessEql1((const long) this, (const long) &that); }
};

bool TFloatSubpoena::LessEql1(const long t1, const long t2) // дата на заседание акт
{ // F_DATE, F_TYPE, F_YEAR, F_NO, F_KIND, F_SUBPOENA_DATE, F_SUBPOENA_NO
	const TFloatSubpoena *s1 = (const TFloatSubpoena *) t1;
	const TFloatSubpoena *s2 = (const TFloatSubpoena *) t2;

	return s1->date != s2->date ? s1->date < s2->date :
		s1->origin != s2->origin ? s1->origin < s2->origin :
		s1->kind != s2->kind ? s1->kind < s2->kind :
		s1->subpoenaDate != s2->subpoenaDate ? s1->subpoenaDate < s2->subpoenaDate :
		s1->subpoenaNo <= s2->subpoenaNo;
}

bool TFloatSubpoena::LessEql2(const long t1, const long t2) // Дата на призовка/съобщение
{ // F_SUBPOENA_DATE, F_DATE, F_TYPE, F_YEAR, F_NO, F_KIND, F_SUBPOENA_NO
	const TFloatSubpoena *s1 = (const TFloatSubpoena *) t1;
	const TFloatSubpoena *s2 = (const TFloatSubpoena *) t2;

	return s1->subpoenaDate != s2->subpoenaDate ? s1->subpoenaDate < s2->subpoenaDate :
		s1->date != s2->date ? s1->date < s2->date :
		s1->origin != s2->origin ? s1->origin < s2->origin :
		s1->kind != s2->kind ? s1->kind < s2->kind :
		s1->subpoenaNo <= s2->subpoenaNo;
}

bool TFloatSubpoena::LessEql3(const long t1, const long t2) // Призовкар
{ // F_NAME, F_FAMILY, F_REFAMILY, F_DATE, F_TYPE, F_YEAR, F_NO, F_KIND, F_SUBPOENA_DATE, F_SUBPOENA_NO
	const TFloatSubpoena *s1 = (const TFloatSubpoena *) t1;
	const TFloatSubpoena *s2 = (const TFloatSubpoena *) t2;
	// 2014:213 LPR: fixed, was string != < string
	bool name_cmp = strcmp(s1->name, s2->name);
	bool family_cmp = strcmp(s1->family, s2->family);
	bool refamily_cmp = strcmp(s1->reFamily, s2->reFamily);

	return name_cmp ? name_cmp < 0 :
		family_cmp ? family_cmp < 0 :
		refamily_cmp ? refamily_cmp < 0 :
		s1->date != s2->date ? s1->date < s2->date :
		s1->origin != s2->origin ? s1->origin < s2->origin :
		s1->kind != s2->kind ? s1->kind < s2->kind :
		s1->subpoenaDate != s2->subpoenaDate ? s1->subpoenaDate < s2->subpoenaDate :
		s1->subpoenaNo <= s2->subpoenaNo;
}

bool TFloatSubpoena::LessEql4(const long t1, const long t2) // Основенвид дело/номер/година
{ // F_TYPE, F_YEAR, F_NO
	return ((const TFloatSubpoena *) t1)->origin <= ((const TFloatSubpoena *) t2)->origin;
}

#define floatsub OFFICE_GROUP_CAST(TFloatSubpoena)
static void floatsub_origin(mstr &m, TGroup *group)
{
	Type->Seek(floatsub->origin.type);
	m.printf("%s %ld/%d", Type->alias, floatsub->origin.no, floatsub->origin.year);
}
static void floatsub_document(mstr &m, TGroup *group)
{
	if (floatsub->origin.type != floatsub->key.type)
	{
		Type->Seek(floatsub->key.type);
		m.printf("%s %ld/%d", Type->alias, floatsub->key.no, floatsub->key.year);
	}
}
static void floatsub_type(mstr &m, TGroup *group)
{
	Type->Seek(floatsub->key.type);
	m.cpy(Type->alias);
}
static void floatsub_kind(mstr &m, TGroup *group)
{
	if (floatsub->originKind)
		m.cat(KindAlias->Seek(floatsub->originKind));
	else
		floatsub_type(m, group);
}
static void floatsub_session_kind(mstr &m, TGroup *group)
{
	m.cat(KindAlias->Seek(floatsub->kind));
}
static void floatsub_session_date(mstr &m, TGroup *group) { floatsub->date.Print(m); }
static void floatsub_firm_name(mstr &m, TGroup *group) { m.cpy(floatsub->damn.name); }
static void floatsub_firm_type(mstr &m, TGroup *group) { m.cpy(CompanyType->Seek(floatsub->damn.companyType)); }
static void floatsub_name(mstr &m, TGroup *group)
{
	if (strchr(UCN_CITIZENS, floatsub->ucnType))
		print_user(m, floatsub->ucn, floatsub->ucnType);
	else if (strchr(UCN_FIRMS, floatsub->ucnType))
	{
		TFirm firm;
		firm << floatsub;
		if (firm.Try())
			m.cpy(firm.name);
	}
	else
	{
		TSender sender;
		sender << floatsub;
		if (sender.Try())
			m.cpy(sender.name);
	}
}
static void floatsub_subpoena_kind(mstr &m, TGroup *group) 
{
	m.cpy(SubpoenaKind->Seek(floatsub->subpoenaKind)); 
	if (floatsub->multi && floatsub->multi != SUBPOENA_SINGLE)
		m.cat(" - обща(о)");
}
static void floatsub_status(mstr &m, TGroup *group)
{
	print_received(m, floatsub->receivedStatus, floatsub->subpoenaReceived, floatsub->receivedTime, floatsub->coordN,
		floatsub->coordE);
	m.sep_cat("\v", floatsub->receivedText);

	if (floatsub->outRegNo)
		m.printf("\vИзх.№ %ld/%ld", floatsub->outRegNo, floatsub->outRegYear);
}

static void floatsub_deliverer(mstr &m, TGroup *group) { if (any(floatsub->name)) print_user(m, floatsub); }

static void floatsub_address(mstr &m, TGroup *group)
{
	TAnyAddress *const address = TAnyAddress::NewGroup(any(floatsub->foreign));
	TVirtPtr VP(address);

	*address << floatsub;

	if (floatsub->uclp || any(floatsub->foreign))
	{
		address->Print(m, ADDRESS_DISTINCT | ADDRESS_REMARK);
		TSubpoenaVisit().PrintVisits(m, floatsub);
	}
}

static void floatsub_notifieds(mstr &m, TGroup *group)
{
	if (floatsub->nameListWind)
		floatsub->PrintNotifieds(m, "\v");
	else
		floatsub_name(m, group);
}

static void floatsub_returned(mstr &m, TGroup *group) { floatsub->subpoenaReturned.Print(m); }

#undef floatsub

static const offield floatsub_fields[] =
{
	{ "F_TYPE", floatsub_type },
	{ "F_KIND", floatsub_kind },
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_SESSION_KIND", floatsub_session_kind },
	{ "F_SESSION_DATE", floatsub_session_date },
	{ "F_FIRM_NAME", floatsub_firm_name },
	{ "F_FIRM_TYPE", floatsub_firm_type },
	{ "F_NAME", floatsub_name },
	{ "F_SUBPOENA_KIND", floatsub_subpoena_kind },
	{ "F_DATE", subpoena_subpoena_date },
	{ "F_STATUS", floatsub_status },
	{ "F_DELIVERER", floatsub_deliverer },
	{ "F_ADDRESS", floatsub_address },
	{ "F_ORIGIN" , floatsub_origin},
	{ "F_DOCUMENT" , floatsub_document},
	{ "F_NOTIFIEDS" , floatsub_notifieds},
	{ "F_SUBPOENA_RETURNED", floatsub_returned },
	{ NULL, NULL }
};

// ----- TFloatSubpoenaQuery -------------------------------------------------
class TFloatSubpoenaQuery : public TFloatQuery
{
public:
	TFloatSubpoenaQuery(TWindow *parent, const char *criteria, const char *name, TFloatSubpoenaGroup *tGroup);

	void Count();
	void Execute();	// 2009:019 LPR: form-specific

	TFloatSubpoenaGroup *group;
	// 2009:019 LPR: form-specific
	const char *hd1start, *hd1final;
	const char *tr2start, *tr2final;
	// 2018-01-29
	const char *tr3start, *tr3final;
	const char *subfinal, *anofinal;

protected:
	void QueryAnnounces();	
};

TFloatSubpoenaQuery::TFloatSubpoenaQuery(TWindow *parent, const char *criteria, const char *name, TFloatSubpoenaGroup *tGroup)
	: TFloatQuery(parent, criteria, name, "Нерегламентирана справка за призовки/съобщения/писма/обявления"), group(tGroup)
{
	maximum = 50000;
}

void TFloatSubpoenaQuery::Count()
{
	TLawsuit lawsuit;
	TGSArray<TFloatSubpoena> array(group->order == ORDER_BY_DTYN ? TFloatSubpoena::LessEql1 :
		group->order == ORDER_BY_SDTYN ? TFloatSubpoena::LessEql2 :
		group->order == ORDER_BY_NMTYN ? TFloatSubpoena::LessEql3 : TFloatSubpoena::LessEql4);
	TQuery q(criteria);

	while (q.Read() && Increment())
	{
		TFloatSubpoena *subpoena = new TFloatSubpoena;
		*subpoena << q;

		TRCDKeyContainer *const container = kind2RCDKeyContainer(subpoena->kind);
		TVirtPtr VP(container);

		*container << subpoena;

		if (container->Get() && find_origin(container, subpoena->origin))
		{
			if (strchr(TYPE_LAWSUITS, subpoena->origin.type) && !strequal(group->lawKinds, Default->xKinds))
			{
				lawsuit.key = subpoena->origin;

				if (!lawsuit.Get("F_KIND") || !strchr(group->lawKinds, lawsuit.kind))
					continue;
			}

			array.Add(subpoena);
		}
		else
			delete subpoena;
	}

	count = array.Count();

	for (int i = 0; i < count; i++)
	{
		TFloatSubpoena *subpoena = array[i];

		wait->TickCount(i + 1, "отпечатани записа");
		subpoena->nameListWind = group->nameListWind;

		if (group->firmData)
		{
			lawsuit.key = subpoena->origin;
			find_damn(&lawsuit, &subpoena->damn, false);
		}

		office.Emit(tr1start, tr1final, null_fields, NULL);

		if (group->Deliverer())
			office.Emit(tr1final, tr2start, floatsub_fields, subpoena);

		office.Emit(tr2start, tr2final, floatsub_fields, subpoena);
	}
}

void TFloatSubpoenaQuery::Execute()
{
	TWindowHideParent hide(parent);

	if (office.Open(name))
	{
		hd1start = office.Find("hd1 start");
		hd1final = office.Find("hd1 final");
		tr1start = office.Find("tr1 start");
		tr1final = office.Find("tr1 final");
		tr2start = office.Find("tr2 start");
		tr2final = office.Find("tr2 final");
		subfinal = office.Find("sub final");
		tr3start = office.Find("tr3 start");
		tr3final = office.Find("tr3 final");
		anofinal = office.Find("ano final");

		office.Emit(NULL, hd1start, float_fields, this);

		if (group->Deliverer())
			office.Emit(hd1start, hd1final, null_fields, NULL);

		office.Emit(hd1final, tr1start, null_fields, NULL);
		Count();
		office.Emit(tr2final, subfinal, float_fields, this);

		if (group->Announces())
			QueryAnnounces();

		office.Emit(tr3final, anofinal, null_fields, NULL);
		Show();
	}
}


void TFloatSubpoenaQuery::QueryAnnounces()
{
	TGroupArray announces;

	wait->SetText(WAIT);
	load_announces(group, announces, group->deliverers);

	if (announces.Count() > 0)
	{
		office.Emit(subfinal, tr3start, null_fields, NULL);

		for (int i = 0; i < announces.Count(); i++)
			office.Emit(tr3start, tr3final, floatano_fields, announces[i]);

		count = announces.Count();
		office.Emit(tr2final, subfinal, float_fields, this);
	}
}

// ----- query ---------------------------------------------------------------
static void query_subpoena(TWindow *parent, const char *criteria, TFloatSubpoenaGroup *group)
{
	TFloatSubpoenaQuery(parent, criteria, group->firmData ? "FloatSuF.htm" : "FloatSub.htm", group).Execute();
}

#include "FloatSubpoenaDialog.h"
#include "FloatSubpoenaDialog.cpp"

void float_subpoena(TWindow *parent)
{
	TFloatSubpoenaGroup group;
	TFloatSubpoenaDialog(parent, &group).Execute();
}
