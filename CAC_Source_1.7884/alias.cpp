#include "cac.h"

// ----- TAliasGroup ---------------------------------------------------------
TAliasGroup::TAliasGroup(const char *name, TGroup *(*tClone)(), TField *tValueField, const char *nameFieldName,
	char *nameFieldData, int nameFieldSize, TCharAlias *tXField, TLong *tFlagsField, bool ordered)
	: TGroup(name, tClone), valueField(tValueField), nameField(new TString(nameFieldName, nameFieldData, nameFieldSize)),
		xField(tXField), flagsField(tFlagsField), cache(NULL), NameField(nameField), orderField(NULL)
{
	Init(ordered);
}

TAliasGroup::TAliasGroup(const char *name, TGroup *(*tClone)(), TField *tValueField, TString *tNameField,
	TCharAlias *tXField, TLong *tFlagsField, bool ordered)
	: TGroup(name, tClone), valueField(tValueField), nameField(tNameField), xField(tXField), flagsField(tFlagsField),
		cache(NULL), NameField(nameField), orderField(NULL)
{
	Init(ordered);
}

void TAliasGroup::Init(bool ordered)
{
	Add(valueField, nameField, NULL);

	if (xField)
		Add(xField);

	if (flagsField)
		Add(flagsField);

	if (ordered)
	{
		orderField = new TLong("F_ORDER", &order);
		Add(orderField);
	}
}

void TAliasGroup::CacheCollect(TGroupList *glist, TGroupArray *array, TAliasGroup *alias)
{
	TGroup *group = clone();
	*group << alias;

	if (glist)
		CacheInsert(glist, group);
	else
		array->Add(group);
}

void TAliasGroup::CacheCriteria(msql &m)
{
	FloatCriteria(m);
}

void TAliasGroup::CacheInsert(TGroupList *glist, TGroup *group)
{
	if (group)
		glist->InsertGroup(group);
	else
		glist->InsertEmpty();
}

bool TAliasGroup::CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags)
{
	if (xCrit && !any(xCrit))
		return true;

#if RANGECHECK
	if (xCrit && !xField)
		fatal("%s: xCrit without xField", Name);
	if ((flags & FLAG_CRITS) && !flagsField)
		fatal("%s: crit flags without flagsField", Name);
#endif  // RANGECHECK

	if (cache)
	{
		for (int index = 0; index < cache->Count(); index++)
		{
			TAliasGroup *alias = (*cache)[index];
			alias->Fixup(false);	// for Match()

			if (alias->Match(xCrit, flags))
				CacheCollect(glist, array, alias);
		}

		return true;
	}

	return false;
}

void TAliasGroup::AliasCriteria(msql &m, const char *xCrit, long flags)
{
	FloatCriteria(m);

	if (xCrit && any(xCrit))
	{
#if RANGECHECK
		if (!xField)
			fatal("%s: xCrit without xField", Name);
#endif  // TYPECHECK
		m.AddChars(xField->Name, xCrit);
	}

#if RANGECHECK
	if ((flags & FLAG_CRITS) && !flagsField)
		fatal("%s: crit flags without flagsField", Name);
#endif  // TYPECHECK

	if (flags & FLAG_ACTIVE)
	{
		m.Add(flagsField->Name);
		dataBase->CondActive(m, flagsField->Name);
	}
}

void TAliasGroup::ExactCriteria(msql &m, const char *fields)
{
	TAliasGroup::FloatCriteria(m, fields);
	m.Add(valueField);
}

void TAliasGroup::Fixup(bool)
{
}

void TAliasGroup::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
}

const char *TAliasGroup::Found(TAliasGroup *alias)
{
	alias->Fixup(false);
	*this << alias;
	return nameField->S();
}

TGArray<TAliasGroup> *TAliasGroup::Cache = NULL;

void TAliasGroup::InitCache()
{
	msql m;

#if RANGECHECK
	if (!cache)
		fatal("%s: attempt to init null cache", Name);
#endif  // RANGECHECK

	CacheCriteria(m);
	TQuery q(str(m));

	while (q.Read())
	{
		TAliasGroup *group = (TAliasGroup *) clone();
		*group << q;
		cache->Add(group);
	}
}

void TAliasGroup::InitializeCache()
{
	Cache = new TGArray<TAliasGroup>[CACHE_COUNT];
	// (avoid memblock cache placed in executable)
}

void TAliasGroup::ShutdownCache()
{
	delete[] Cache;
	Cache = NULL;
}

void TAliasGroup::InitializeBase()
{
	Court = new TCourt;
	Court->InitCache();
	Type = new TType;
	Type->InitCache();
	Area = new TArea;
	Area->InitCache();
	UCLP = new TUCLP;
	// TUCLP not cached
	User = new TUser;
	// TUser not cached
	EisDepart = new TEisDepart;
	// TEisDepart cached on demand
}

// static DG(X) would have been great, but after a fatal
// exception, it tries to delete from stack(?) post-main
// should try to place all static DG(X) in aaaaa.cpp...

void TAliasGroup::ShutdownBase()
{
	delete EisDepart;
	EisDepart = NULL;
	delete User;
	User = NULL;
	delete UCLP;
	UCLP = NULL;
	delete Area;
	Area = NULL;
	delete Type;
	Type = NULL;
	delete Court;
	Court = NULL;
}

void TAliasGroup::InitializeConst()
{
	Kind = new TKind;
	Kind->InitCache();
	KindAlias = new TKindAlias;
	KindAlias->InitCache();
	ConnectType = new TConnectType;
	ConnectType->InitCache();
}

void TAliasGroup::ShutdownConst()
{
	delete ConnectType;
	ConnectType = NULL;
	delete KindAlias;
	KindAlias = NULL;
	delete Kind;
	Kind = NULL;
}

void TAliasGroup::InitializeNoms()
{
	Hall = new THall;
	Hall->InitCache();
	UserAliasGroup = new TUserAliasGroup;
	UserAliasGroup->InitCache();
	KnownAliasGroup = new TKnownAliasGroup;
	// TKnownAliasGroup not cached
	Country = new TCountry;
	Country->InitCache();
	JailMode = new TJailMode;
	JailMode->InitCache();
	CrimeForm = new TCrimeForm;
	CrimeForm->InitCache();
	CrimeStage = new TCrimeStage;
	CrimeStage->InitCache();
	Recidive = new TRecidive;
	Recidive->InitCache();
	GOP = new TGOP;
	GOP->InitCache();
	SumType = new TSumType;
	SumType->InitCache();
	SumState = new TSumState;
	SumState->InitCache();
	Judged = new TJudged;
	Judged->InitCache();
	Punishment = new TPunishment;
	Punishment->InitCache();
	Age = new TAge;
	Age->InitCache();
	Grant = new TGrant;
	Grant->InitCache();
	Month = new TMonth;
	Month->InitCache();
	Interval = new TInterval;
	Interval->InitCache();
	AccountType = new TAccountType;
	AccountType->InitCache();
	Weekday = new TWeekday;
	Weekday->InitCache();
	Section = new TSection;
	Section->InitCache();
	AppealResolution = new TAppealResolution;
	AppealResolution->InitCache();
	CompanyType = new TCompanyType;
	CompanyType->InitCache();
	ReceivedStatus = new TReceivedStatus;
	ReceivedStatus->InitCache();
	Result = new TNTResult;
	Result->InitCache();
	CalInterval = new TCalInterval;
	CalInterval->InitCache();
	Post = new TPost;
	Post->InitCache();
	Rank = new TRank;
	Rank->InitCache();
	Right = new TRight;
	Right->InitCache();
	Accomply = new TAccomply;
	Accomply->InitCache();
	UXNType = new TUXNType;
	UXNType->InitCache();
	UCNType = new TUCNType;
	UCNType->InitCache();
	Involvement = new TInvolvement;
	Involvement->InitCache();
	ConnectKind = new TConnectKind;
	ConnectKind->InitCache();
	SubpoenaKind = new TSubpoenaKind;
	SubpoenaKind->InitCache();
	Resolution = new TResolution;
	Resolution->InitCache();
	Subject = new TSubject;
	// TSubject not cached
	OldSubject = new TOldSubject;
	// TOldSubject not cached
	PenalityType = new TPenalityType;
	PenalityType->InitCache();
	ReasonType = new TReasonType;
	ReasonType->InitCache();
	College = new TCollege;
	College->InitCache();
	CollegeRight = new TCollegeRight;
	CollegeRight->InitCache();
	FineComposition = new TFineComposition;
	// TFineComposition not cached
	Composition = new TComposition;
	Composition->InitCache();
	CharComposition = new TCharComposition;
	CharComposition->InitCache();
	Receiver = new TReceiver;
	// TReceiver not cached
	Region = new TRegion;
	// TRegion cached later
	Street = new TStreet;
	// TStreet not cached
	Article = new TArticle;
	Article->InitCache();
	Danger = new TDanger;
	Danger->InitCache();
	DangerSubject = new TDangerSubject;
	DangerSubject->InitCache();
	ArticleSubject = new TArticleSubject;
	ArticleSubject->InitCache();
	DebtorStatus = new TDebtorStatus();
	DebtorStatus->InitCache();
	BanConSist = new TBanConSist;
	BanConSist->InitCache();
	XferKind = new TXferKind;
	XferKind->InitCache();
	EisTransl = new TEisTransl;
	EisTransl->InitCache();
	LegalAidType = new TLegalAidType();
	LegalAidType->InitCache();
	ArchiveIndex = new TArchiveIndex;
	ArchiveIndex->InitCache();
	ArchiveSubject = new TArchiveSubject;
	ArchiveSubject->InitCache();
	Currency = new TCurrency();
	Currency->InitCache();
	Account = new TAccount();
	// TAccount not cached
	SLCType = new TSLCType();
	SLCType->InitCache();
	SLCReason = new TSLCReason();
	// SLCReason not cached
	VSSIndexSet = new TVSSIndexSet();
	VSSIndexSet->InitCache();
	VSSIndex = new TVSSIndex();
	// VSSIndex not a table
	What = new TWhat;
	What->InitCache();
}

void TAliasGroup::ShutdownNoms()
{
	delete What;
	What = NULL;
	delete VSSIndex;
	VSSIndex = NULL;
	delete VSSIndexSet;
	VSSIndexSet = NULL;
	delete SLCReason;
	SLCReason = NULL;
	delete SLCType;
	SLCType = NULL;
	delete Account;
	Account = NULL;
	delete Currency;
	Currency = NULL;
	delete ArchiveSubject;
	ArchiveSubject = NULL;
	delete ArchiveIndex;
	ArchiveIndex = NULL;
	delete LegalAidType;
	LegalAidType = NULL;
	delete EisTransl;
	EisTransl = NULL;
	delete XferKind;
	XferKind = NULL;
	delete BanConSist;
	BanConSist = NULL;
	delete DebtorStatus;
	DebtorStatus = NULL;
	delete ArticleSubject;
	ArticleSubject = NULL;
	delete DangerSubject;
	DangerSubject = NULL;
	delete Danger;
	Danger = NULL;
	delete Article;
	Article = NULL;
	delete Street;
	Street = NULL;
	delete Region;
	Region = NULL;
	delete Receiver;
	Receiver = NULL;
	delete CharComposition;
	CharComposition = NULL;
	delete Composition;
	Composition = NULL;
	delete FineComposition;
	FineComposition = NULL;
	delete CollegeRight;
	CollegeRight = NULL;
	delete College;
	College = NULL;
	delete ReasonType;
	ReasonType = NULL;
	delete PenalityType;
	PenalityType = NULL;
	delete OldSubject;
	OldSubject = NULL;
	delete Subject;
	Subject = NULL;
	delete Resolution;
	Resolution = NULL;
	delete SubpoenaKind;
	SubpoenaKind = NULL;
	delete ConnectKind;
	ConnectKind = NULL;
	delete Involvement;
	Involvement = NULL;
	delete UCNType;
	UCNType = NULL;
	delete UXNType;
	UXNType = NULL;
	delete Accomply;
	Accomply = NULL;
	delete Right;
	Right = NULL;
	delete Rank;
	Rank = NULL;
	delete Post;
	Post = NULL;
	delete CalInterval;
	CalInterval = NULL;
	delete Result;
	Result = NULL;
	delete ReceivedStatus;
	ReceivedStatus = NULL;
	delete CompanyType;
	CompanyType = NULL;
	delete AppealResolution;
	AppealResolution = NULL;
	delete Section;
	Section = NULL;
	delete Weekday;
	Weekday = NULL;
	delete AccountType;
	AccountType = NULL;
	delete Interval;
	Interval = NULL;
	delete Month;
	Month = NULL;
	delete Grant;
	Grant = NULL;
	delete Age;
	Age = NULL;
	delete Punishment;
	Punishment = NULL;
	delete Judged;
	Judged = NULL;
	delete SumState;
	SumState = NULL;
	delete SumType;
	SumType = NULL;
	delete GOP;
	GOP = NULL;
	delete Recidive;
	Recidive = NULL;
	delete CrimeStage;
	CrimeStage = NULL;
	delete CrimeForm;
	CrimeForm = NULL;
	delete JailMode;
	JailMode = NULL;
	delete Country;
	Country = NULL;
	delete KnownAliasGroup;
	KnownAliasGroup = NULL;
	delete UserAliasGroup;
	UserAliasGroup = NULL;
	delete Hall;
	Hall = NULL;
}

void TAliasGroup::LoadData(TGroupArray *array, const char *xCrit, long flags)
{
	if (!CacheFill(NULL, array, xCrit, flags))
	{
		msql m;

		AliasCriteria(m, xCrit, flags);
		TQuery q(str(m));

		while (q.Read())
		{
			TGroup *group = clone();
			*group << q;
			array->Add(group);
		}
	}
}

bool TAliasGroup::LoadCache(TGroupList *glist, const char *xCrit, long flags)
{
	return CacheFill(glist, NULL, xCrit, flags);
}

bool TAliasGroup::Match(const char *xCrit, long flags)
{
	return (!xCrit || strchr(xCrit, xField->C())) && (!(flags & FLAG_ACTIVE) || (flagsField->L() & FLAG_ACTIVE));
}

void TAliasGroup::Print(mstr &m)
{
	Fixup(false);
	m.cat(nameField->S());
}

void TAliasGroup::ReloadCache()
{
#if RANGECHECK
	if (!cache)
		fatal("%s: attempt to reload null cache", Name);
#endif  // RANGECHECK
	cache->Flush();
	InitCache();
}

const char *TAliasGroup::Seek()
{
	if (!Get())
		fatal("%s: unable to get data for %s", Name, valueField->Name);

	return nameField->S();
}

bool TAliasGroup::Try(const char *fields)
{
	if (TGroup::Try(fields))
	{
		Fixup(true);
		return true;
	}

	return false;
}

// ----- TStringAliasGroup ---------------------------------------------------
TStringAliasGroup::TStringAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName,
	char *valueFieldData, int valueFieldSize, const char *nameFieldName, char *nameFieldData, int nameFieldSize,
	TCharAlias *xField, TLong *flagsField, bool ordered)
	: TAliasGroup(name, tClone, new TStringAlias(valueFieldName, valueFieldData, valueFieldSize),
		nameFieldName, nameFieldData, nameFieldSize, xField, flagsField, ordered)
{
}

const char *TStringAliasGroup::Seek(const char *s)
{
	if (cache)
	{
		for (int index = 0; index < cache->Count(); index++)
		{
			TStringAliasGroup *alias = (TStringAliasGroup *) (*cache)[index];

			if (!strcmp(alias->ValueField()->S(), s))
				return Found(alias);
		}
	}

	ValueField()->SetS(s);
	return TAliasGroup::Seek();
}

// ----- TPersonAliasGroup ---------------------------------------------------
TPersonAliasGroup::TPersonAliasGroup(const char *name, TGroup *(*tClone)(), TCharAlias *xField)
	: TStringAliasGroup(name, tClone, "F_UCN", ucn, sizeof ucn, "F_UNAME", uname, sizeof uname, xField,
		new TLong("F_FLAGS", &flags))
{
}

void TPersonAliasGroup::Fixup(bool force)
{
	if (force || !any(uname))
	{
		if (atob(ucn))
			print_user(uname, ucn, UCN_CITIZEN_UCN, true, true);
		else	// called by Print()
			*uname = '\0';
	}
}

// ----- TUserAliasGroup -----------------------------------------------------
TUserAliasGroup::TUserAliasGroup()
:
	TPersonAliasGroup("T_USER", TUserAliasGroup::Clone, new TCharAlias("F_POST", &post))
{
	Add(
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TCharList("F_COLLEGES", colleges, sizeof colleges),
		new TCharArray("F_COMPOSITIONS", compositions, sizeof compositions),
		new TCharList("F_FUNCTIONS", functions, sizeof functions),
		NULL
	);

	cache = Cache + CACHE_USERS;
	descs = Descs + TABLE_USER;
}

TGroup *TUserAliasGroup::Clone()
{
	return new TUserAliasGroup;
}

void TUserAliasGroup::CacheCriteria(msql &m)
{
	TPersonAliasGroup::CacheCriteria(m);
	m.AddChars("F_POST", POST_CACHES);
}

void TUserAliasGroup::ExactCriteria(msql &m, const char *fields)
{
	TUserAliasGroup::FloatCriteria(m, fields);
	m.Add(this, "F_UCN");
}

void TUserAliasGroup::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields && strcmp(fields, "*") ? fields :
		"F_UCN, F_FLAGS, F_POST, F_LAW_KINDS, F_COLLEGES, F_COMPOSITIONS, F_FUNCTIONS");
}

bool TUserAliasGroup::Serves(long uclp, long street, const char *adrNo, const char *blockNo)
{
	if (post != POST_DELIVERER || !uclp || !street)
		return false;

	strcpy(StreetMap->ucn, ucn);
	StreetMap->uclp = uclp;
	StreetMap->street = street;

	if (!StreetMap->Try())
		return false;

	int tAdrNo = atoi(adrNo);

	if (StreetMap->Active(tAdrNo))
		return true;

	return !tAdrNo && StreetMap->Active(atoi(blockNo));
}

bool TUserAliasGroup::CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags)
{
	if (xCrit)
	{
		const char *s;

		for (s = xCrit; *s; s++)
			if (!strchr(POST_CACHES, *s))
				break;

		if (!any(s) && TPersonAliasGroup::CacheFill(glist, array, xCrit, flags))
		{
			if (flags & USER_EMPTY)
			{
				if (glist)
					CacheInsert(glist, NULL);
				else
				{
					TGroup *group = clone();

					if (array->Sorted())
						array->Add(group);
					else
						array->Insert(group, 0);
				}
			}
			return true;
		}
	}

	return false;
}

bool TUserAliasGroup::Match(const char *xCrit, long flags)
{
	if (!TPersonAliasGroup::Match(xCrit, flags))
		return false;

	// 2015:300
	if ((flags & USER_INSECT) && strchr(POST_INSECT_TARGETS, post) && strchr(POST_PERSONELS, Default->post))
	{
		if (USER_INTERSECT >= USER_INTERSECT_COLLEGE && !intersect(colleges, Default->colleges))
			return false;

		if (USER_INTERSECT == USER_INTERSECT_FULL && !intersect(compositions, Default->compositions))
			return false;
	}

	if (flags & USER_NOLIMIT)
		return true;

	if (post == POST_DELIVERER && Default->post == POST_DELIVERER)
		return !strcmp(ucn, Default->ucn);

	if (Default->post == POST_SUB_CHIEF && intersect(Default->colleges, colleges))
		return true;

	// 2016:032 LPR/IRQ: POST_JUDGES -> VISIBLE
	if (!strchr(POST_VISIBLE_JUDGES, post) || !strchr(POST_JUDGES, Default->post))
		return true;

	return Default->post == POST_CHIEF || !strcmp(ucn, Default->ucn);
}

TUserAliasGroup *UserAliasGroup = NULL;

// ----- TKnownAliasGroup ----------------------------------------------------
TKnownAliasGroup::TKnownAliasGroup(TGroup *(*const tClone)())
:
	TPersonAliasGroup("T_KNOWN_PERSON", tClone, new TCharAlias("F_INVOLVEMENT", &involvement))
{
	Add(new TCharAlias("F_UCN_TYPE", &ucnType));

	descs = Descs + TABLE_KNOWN_PERSON;
	ucnType = UCN_CITIZEN_UCN;
}

TGroup *TKnownAliasGroup::Clone()
{
	return new TKnownAliasGroup;
}

void TKnownAliasGroup::ExactCriteria(msql &m, const char *fields)
{
	TStringAliasGroup::ExactCriteria(m, fields);
	m.AddChar("F_UCN_TYPE", UCN_CITIZEN_UCN);
}

bool TKnownAliasGroup::Match(const char *xCrit, long flags)
{
	return ucnType == UCN_CITIZEN_UCN && TAliasGroup::Match(xCrit, flags);
}

void TKnownAliasGroup::PrintPrefixed(mstr &m)
{
	if (involvement == INVOLVEMENT_JUROR && (flags & JUROR_PEDAGOG))
		m.cat("ï ");

	TPersonAliasGroup::Print(m);
}

TKnownAliasGroup *KnownAliasGroup = NULL;

// ----- TLongAliasGroup -----------------------------------------------------
TLongAliasGroup::TLongAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData,
	const char *nameFieldName, char *nameFieldData, int nameFieldSize, TCharAlias *xField, TLong *flagsField,
	bool ordered)
	: TAliasGroup(name, tClone, new TLongAlias(valueFieldName, valueFieldData), nameFieldName, nameFieldData,
		nameFieldSize, xField, flagsField, ordered), genLimit(0)
{
}

TLongAliasGroup::TLongAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData,
	TString *nameField, TCharAlias *xField, TLong *flagsField, bool ordered)
	: TAliasGroup(name, tClone, new TLongAlias(valueFieldName, valueFieldData), nameField, xField, flagsField, ordered),
		genLimit(0)
{
}

bool TLongAliasGroup::Insert()
{
	if (genLimit && !((TLongAlias *) valueField)->L())
	{
		constant minMax = genLimit > 0 ? "MAX" : "MIN";
		mstr m;
		long l;

		m.printf("SELECT %s(%s) AS %s FROM %s", minMax, valueField->Name, valueField->Name, Name);
		l = dataBase->GetLong(str(m), valueField->Name);

		if (genLimit > 0 ? ++l > genLimit : --l < genLimit)
			generator_limit(Name);

		((TLongAlias *) valueField)->SetL(l);
	}

	return TAliasGroup::Insert();
}

const char *TLongAliasGroup::Seek(long l)
{
	if (cache)
	{
		for (int index = 0; index < cache->Count(); index++)
		{
			TLongAliasGroup *alias = (TLongAliasGroup *) (*cache)[index];

			if (alias->ValueField()->L() == l)
				return Found(alias);
		}
	}

	ValueField()->SetL(l);
	return TAliasGroup::Seek();
}

// ----- TCharAliasGroup -----------------------------------------------------
TCharAliasGroup::TCharAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, char *valueFieldData,
	const char *nameFieldName, char *nameFieldData, int nameFieldSize, bool ordered)
	: TAliasGroup(name, tClone, new TCharAlias(valueFieldName, valueFieldData), nameFieldName, nameFieldData, nameFieldSize,
		NULL, NULL, ordered)
{
	xField = (TCharAlias *) valueField;		// don't +
}

const char *TCharAliasGroup::Seek(char c)
{
	if (cache)
	{
		for (int index = 0; index < cache->Count(); index++)
		{
			TCharAliasGroup *alias = (TCharAliasGroup *) (*cache)[index];

			if (alias->ValueField()->C() == c)
				return Found(alias);
		}
	}

	ValueField()->SetC(c);
	return TAliasGroup::Seek();
}

// ----- TXferAliasGroup -----------------------------------------------------
TXferAliasGroup::TXferAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, char *valueFieldData,
	const char *nameFieldName, char *nameFieldData, int nameFieldSize, bool ordered)
	: TCharAliasGroup(name, tClone, valueFieldName, valueFieldData, nameFieldName, nameFieldData, nameFieldSize, ordered)
{
	Add(new TLong("F_XFER", &xfer));
}

long TXferAliasGroup::Xfer(char c)
{
	Seek(c);
	return xfer;
}

// ----- TLongAliasArray -----------------------------------------------------
bool TLongAliasArray::LessEqlT(const long t1, const long t2)
{
	// prefer orderField if any?
	return ((TLongAliasGroup *) t1)->ValueField()->L() <= ((TLongAliasGroup *) t2)->ValueField()->L();
}

// ----- TCharAliasArray -----------------------------------------------------
bool TCharAliasArray::LessEqlT(const long t1, const long t2)
{
	return ((TCharAliasGroup *) t1)->OrderField()->L() <= ((TCharAliasGroup *) t2)->OrderField()->L();
}
