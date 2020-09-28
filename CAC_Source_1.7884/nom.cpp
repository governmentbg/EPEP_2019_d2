#include "cac.h"

// ----- TType ---------------------------------------------------------------
TType::TType()
	: TCharAliasGroup("T_TYPE", TType::Clone, "F_TYPE", &type, "F_NAME", name, sizeof name)
{
	Add(
		new TString("F_ALIAS", alias, sizeof alias),
		new TLong("F_ECODE", &ecode),
		NULL
	);

	cache = Cache + CACHE_TYPE;
	descs = Descs + TABLE_TYPE;
}

TGroup *TType::Clone()
{
	return new TType;
}

#if CUTCITILAWS
void TType::CacheCriteria(msql &m)
{
	TCharAliasGroup::CacheCriteria(m);
	m.AddChar("NOT F_TYPE", TYPE_CITIZEN_LAW);
}
#endif  // CUTCITILAWS

TType *Type = NULL;

// ----- TKind ---------------------------------------------------------------
TKind::TKind()
	: TXferAliasGroup("T_KIND", TKind::Clone, "F_KIND", &kind, "F_NAME", name, sizeof name)
{
	Add(
		new TCharAlias("F_TYPE", &type),
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TString("F_SUITORS", suitors, sizeof suitors),
		new TString("F_DEFENDANTS", defendants, sizeof defendants),
		new TString("F_OTHERS", others, sizeof others),
		new TString("F_ALIAS", alias, sizeof alias),
		new TLong("F_EISPP", &eispp),
		NULL
	);

	cache = Cache + CACHE_KIND;
	descs = Descs + TABLE_KIND;
}

TGroup *TKind::Clone()
{
	return new TKind;
}

void TKind::InitCache()
{
	TCharAliasGroup::InitCache();

	for (int index = 0; index < cache->Count(); index++)
	{
		*this << (*cache)[index];
		strcat_nx(_INVOLVEMENT_ALLS, suitors);
		strcat_nx(_INVOLVEMENT_ALLS, defendants);
		strcat_nx(_INVOLVEMENT_ALLS, others);
	}

	for (const char *s = INVOLVEMENT_ALLS; *s; s++)
		if (!strchr(INVOLVEMENT_LAWERS, *s))
			strcach(_INVOLVEMENT_NON_LAWERS, *s);
}

#if CUTCITILAWS
void TKind::CacheCriteria(msql &m)
{
	TCharAliasGroup::CacheCriteria(m);

	m.AddChar("(F_TYPE IS NULL OR NOT F_TYPE", TYPE_CITIZEN_LAW);
	m.cat(")");
	m.AddChars("(F_LAW_KINDS IS NULL OR NOT F_LAW_KINDS", KIND_CITIZEN_LAWS);
	m.cat(")");
}
#endif  // CUTCITILAWS

void TKind::XferCriteria(msql &m, long xferKind, char type)
{
	m.Begin(Name);
	m.AddLong("F_XFER", xferKind);
	if (type)
		m.AddChar("F_TYPE", type);
}

TKind *Kind = NULL;

// ----- KindAlias -----------------------------------------------------------
TKindAlias::TKindAlias()
	: TCharAliasGroup("T_KIND", TKindAlias::Clone, "F_KIND", &kind, "F_ALIAS", alias, sizeof alias)
{
	cache = Cache + CACHE_KIND_ALIAS;
	descs = Descs + TABLE_KIND;
}

TGroup *TKindAlias::Clone()
{
	return new TKindAlias;
}

void TKindAlias::InitCache()
{
	TGArray<TAliasGroup> *tCache = Cache + CACHE_KIND;

	for (int index = 0; index < tCache->Count(); index++)
	{
		TAliasGroup *group = (TAliasGroup *) clone();
		*group << (*tCache)[index];
		cache->Add(group);
	}
}

TKindAlias *KindAlias;

// ----- TSubjectData --------------------------------------------------------
void TSubjectData::AddTo(TGroup *self, long si)
{
	self->Add(
		new TCharAlias(TSubject::FNI("F_KIND", si), &kind),
		new TLong(TSubject::FNI("F_FLAGS", si), &flags),
		new TCharList(TSubject::FNI("F_ATTRIB", si), attrib, sizeof attrib),
		new TCharList(TSubject::FNI("F_RESULTS", si), results, sizeof results),
		new TCharList(TSubject::FNI("F_CLOSEDS", si), closeds, sizeof closeds),
		new TCharArray(TSubject::FNI("F_PENALITIES", si), penalities, sizeof penalities),
		new TCharArray(TSubject::FNI("F_REASONS", si), reasons, sizeof reasons),
		new TLong(TSubject::FNI("F_JUDGE_REPORT", si), &judgeReport),
		new TLong(TSubject::FNI("F_QUICK_REPORT", si), &quickReport),
		new TLong(TSubject::FNI("F_VID", si), &vid),
		new TString(TSubject::FNI("F_SBES", si), sbes, sizeof sbes),
		new TCheck(TSubject::FNI("F_FLAGS_ACTIVE", si), &flags, FLAG_ACTIVE),
		NULL
	);
}

// ----- TSubject ------------------------------------------------------------
TSubject::TSubject()
	: TLongAliasGroup("T_SUBJECT", TSubject::Clone, "F_SUBJECT", &subject, new TLongString("F_NAME", name, sizeof name),
		new TCharAlias("F_TYPE", &type), NULL)
{
	Add(
		new TString("F_ALIAS", alias, sizeof alias),
		new TLong("F_FLAGS_X", &flagsX),
		new TLong("F_SUBJECT_LONG", &subject),
	#if COURT_TYPE == COURT_REGIONAL
		new TCheck("F_FLAGS_APC_TMPL", &sd[0].flags, SUBJECT_APC_TMPL),
	#endif  // REGIONAL
		new TLong("F_SAAS", &saas),
		NULL
	);

	sd[0].AddTo(this, 0);
	sd[1].AddTo(this, 1);
	sd[0].flags = sd[1].flags = FLAG_ACTIVE;

	descs = Descs + TABLE_SUBJECT;
}

TGroup *TSubject::Clone()
{
	return new TSubject;
}

TISArray<TSubject::TCodeName> TSubject::CodeNames;

bool TSubject::TCodeName::operator<=(const TSubject::TCodeName &that)
{
	return subject != that.subject ? subject < that.subject : type <= that.type;
}

const char *TSubject::Code(long subject, char type, long si, bool saas)
{
	TCodeName *codeName = new TCodeName;

	codeName->subject = subject;
	codeName->type = type;

	int index = CodeNames.Find(codeName);

	if (index != INT_MAX)
	{
		delete codeName;
		codeName = CodeNames[index];
	}
	else
	{
		if (subject % SUBJECT_MULTIPLY)
		{
			struct TSubjectCodeName
			{
				char type;
				long subject;
				const char *name;
			};

			// ----- SUBJECT -------------------------------------------
			static const TSubjectCodeName SubjectCodeNames[] =
			{
			#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
				{ TYPE_PUNISHMENT_LAW, 83450, "800А" },
				{ TYPE_PUNISHMENT_LAW, 99950, "900А" },
			#endif  // AREAL || MILITARY || REGIONAL
			#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
				{ TYPE_PUNISHMENT_LAW, 5083450, "50800А" },
				{ TYPE_PUNISHMENT_LAW, 5099950, "50900А" },
			#endif  // APPEAL || AREAL || MILITARY
			#if COURT_TYPE == COURT_ADMIN
				{ TYPE_ADMIN_LAW,  430050, "4300" },
				{ TYPE_ADMIN_LAW,  560050, "5600" },
				{ TYPE_ADMIN_LAW,  90050,  "900" },
				{ TYPE_ADMIN_LAW,  450050, "4500" },
				{ TYPE_TRICKY_LAW, 450050, "4500" },
			#endif  // ADMIN
				{ '\0', 0, NULL }
			};

			const TSubjectCodeName *scn;

			for (scn = SubjectCodeNames; scn->type; scn++)
				if (scn->type == type && scn->subject == subject)
					break;

			if (!scn->type)
				fatal("unsupported subject code: %d %ld", type, subject);

			strcpy(codeName->name, scn->name);
		}
		else
			sprintf(codeName->name, "%ld", subject / SUBJECT_MULTIPLY);

		strcpy(codeName->name1, codeName->name);
		strcpy(codeName->name2, codeName->name);

		if (Twin(type))
		{
			strcat(codeName->name1, SUBJECT_SI_1_SUFFIX);
			strcat(codeName->name2, SUBJECT_SI_2_SUFFIX);
		}

		CodeNames.Add(codeName);
	}

	return saas ? si ? codeName->name2 : codeName->name1 : codeName->name;
}

const char *TSubject::Code(long subject, char lawKind)
{
	if (!lawKind || !strchr(KIND_LAWSUITS, lawKind))
		fatal("Code: %d is not a lawsuit kind", lawKind);

	return TSubject::Code(subject, kind2type(lawKind), TSubject::SI(lawKind), true);
}

bool TSubject::Delete(bool exact)
{
	if (exact)
	{
		DangerSubject->subject = subject;
		if (!DangerSubject->Delete(false))
			return false;

		ArticleSubject->subject = subject;
		if (!ArticleSubject->Delete(false))
			return false;

		for (long si = 0; si <= 1; si++)
		{
			ArchiveSubject->kind = sd[si].kind;
			ArchiveSubject->subject = subject;
			if (!ArchiveSubject->Delete(false))
				return false;
		}
	}

	return TLongAliasGroup::Delete(exact);
}

void TSubject::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_SUBJECT, F_TYPE");
}

const char *TSubject::FNI(const char *name, long si)
{
	enum { NAMES = 12 };
	static constant names[NAMES][3] =
	{
		{ "F_KIND", "F_KIND_I", "F_KIND_II" },
		{ "F_FLAGS", "F_FLAGS_I", "F_FLAGS_II" },
		{ "F_ATTRIB", "F_ATTRIB_I", "F_ATTRIB_II" },
		{ "F_RESULTS", "F_RESULTS_I", "F_RESULTS_II" },
		{ "F_CLOSEDS", "F_CLOSEDS_I", "F_CLOSEDS_II" },
		{ "F_PENALITIES", "F_PENALITIES_I", "F_PENALITIES_II" },
		{ "F_REASONS", "F_REASONS_I", "F_REASONS_II" },
		{ "F_JUDGE_REPORT", "F_JUDGE_REPORT_I", "F_JUDGE_REPORT_II" },
		{ "F_QUICK_REPORT", "F_QUICK_REPORT_I", "F_QUICK_REPORT_II" },
		{ "F_VID", "F_VID_I", "F_VID_II" },
		{ "F_SBES", "F_SBES_I", "F_SBES_II" },
		{ "F_FLAGS_ACTIVE", "F_FLAGS_ACTIVE_I", "F_FLAGS_ACTIVE_II" }
	};

#if RANGECHECK
	if (si != 0 && si != 1)
		fatal("FNI: invalid si = %u", si);
#endif  // RANGECHECK

	for (int index = 0; index < NAMES; index++)
		if (!strcmp(names[index][0], name))
			return names[index][si + 1];

	fatal("FNI: missing %s", name);
	return NULL;
}

bool TSubject::Insert()
{
	type = kind2type(sd[0].kind ? sd[0].kind : sd[1].kind);
	return TLongAliasGroup::Insert();
}

void TSubject::AddVisible(msql &m)
{
	m.AddFlags("F_FLAGS_X", SUBJECT_X_HIDDEN, 0, SUBJECT_X_ALL);
}

const char *TSubject::Seek(char tType, long subject)
{
	type = tType;
	return TLongAliasGroup::Seek(subject);
}

void TSubject::Print(mstr &m)
{
	m.printf("%s %s", Code(0, false), name);
}

#if COURT_TYPE == COURT_APPEAL
long TSubject::SI(char)
{
	return 0;	// "-2" via prefix
}
#endif  // APPEAL
#if  COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
long TSubject::SI(char lawKind)
{
	if (!strchr(KIND_LAWSUITS, lawKind))
		fatal("SI: %d is not a law kind", lawKind);

	return !strchr(KIND_1ST_LAWSUITS, lawKind);
}
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
long TSubject::SI(char)
{
	return 0;
}
#endif  // REGIONAL

bool TSubject::operator<=(const TSubject &that) const
{
	// 2016:074 support for empty subject
	long order = type && Type->Seek(type) ? Type->OrderField()->L() : 0;
	long that_order = that.type && Type->Seek(that.type) ? Type->OrderField()->L() : 0;
	// 2014:107 LRQ: type, subject -> type order, subject
	return order != that_order ? order < that_order : subject <= that.subject;
}

TSubject *Subject = NULL;

// ----- TOldSubject ---------------------------------------------------------
TOldSubject::TOldSubject()
	: TLongAliasGroup("T_OLD_SUBJECT", TOldSubject::Clone, "F_SUBJECT", &subject,
		new TLongString("F_NAME", name, sizeof name), new TCharAlias("F_KIND", &kind), new TLong("F_FLAGS", &flags))
{
	Add(
		new TCharAlias("F_TYPE", &type),
		new TCharList("F_ATTRIB", attrib, sizeof attrib),
		new TCharList("F_RESULTS", results, sizeof results),
		new TString("F_ALIAS", alias, sizeof alias),
		new TCharAlias("F_COLLEGE", &college),
		new TCharList("F_POSTS", posts, sizeof posts),
		new TCharArray("F_PENALITIES", penalities, sizeof penalities),
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		new TCharArray("F_REASONS", reasons, sizeof reasons),
		new TLong("F_SUBJECT_LONG", &subject),
		new TLong("F_JUDGE_REPORT", &judgeReport),
		new TLong("F_QUICK_REPORT", &quickReport),
		new TCharList("F_CLOSEDS", closeds, sizeof closeds),
		new TLong("F_VID", &vid),
		new TString("F_SBES", sbes, sizeof sbes),
	#if COURT_TYPE == COURT_REGIONAL
		new TCheck("F_FLAGS_APC_TMPL", &flags, SUBJECT_APC_TMPL),
	#endif  // REGIONAL
		NULL
	);

	descs = Descs + TABLE_OLD_SUBJECT;
	flags = FLAG_ACTIVE;
}

TGroup *TOldSubject::Clone()
{
	return new TOldSubject;
}

void TOldSubject::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_SUBJECT, F_TYPE");
}

void TOldSubject::Print(mstr &m)
{
	m.printf("%ld %s", subject, name);
}

const char *TOldSubject::Seek(char tType, long subject)
{
	type = tType;
	return TLongAliasGroup::Seek(subject);
}

bool TOldSubject::operator<=(const TOldSubject &that) const
{
	Type->Seek(type);
	long order = Type->OrderField()->L();
	Type->Seek(that.type);
	long that_order = Type->OrderField()->L();
	// 2014:107 LRQ: type, subject -> type order, subject
	return order != that_order ? order < that_order : subject <= that.subject;
}

TOldSubject *OldSubject = NULL;

// ----- TResolution ---------------------------------------------------------
TResolution::TResolution()
	: TCharAliasGroup("T_RESOLUTION", TResolution::Clone, "F_RESOLUTION", &resolution, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_RESOLUTION;
	descs = Descs + TABLE_RESOLUTION;
}

TGroup *TResolution::Clone()
{
	return new TResolution;
}

TResolution *Resolution = NULL;

// ----- TNTResult -------------------------------------------------------------
TNTResult::TNTResult()
	: TXferAliasGroup("T_RESULT", TNTResult::Clone, "F_RESULT", &result, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_RESULT;
	descs = Descs + TABLE_RESULT;
}

TGroup *TNTResult::Clone()
{
	return new TNTResult;
}

TNTResult *Result = NULL;

// ----- TUXNType ------------------------------------------------------------
TUXNType::TUXNType()
	: TCharAliasGroup("T_UXN_TYPE", TUXNType::Clone, "F_UCN_TYPE", &ucnType, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_UXN_TYPE;
	descs = Descs + TABLE_UXN_TYPE;
}

TUXNType::TUXNType(const char *tName, TGroup *(*tClone)())
	: TCharAliasGroup(tName, tClone, "F_UCN_TYPE", &ucnType, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_UXN_TYPE;
	descs = Descs + TABLE_UXN_TYPE;
}

TGroup *TUXNType::Clone()
{
	return new TUXNType;
}

TUXNType *UXNType = NULL;

// ----- TUCNType ------------------------------------------------------------
TUCNType::TUCNType()
	: TUXNType("T_UCN_TYPE", TUCNType::Clone)
{
	Add(new TCharAlias("F_UXN_TYPE", &uxnType));

	cache = Cache + CACHE_UCN_TYPE;
	descs = Descs + TABLE_UCN_TYPE;
}

TGroup *TUCNType::Clone()
{
	return new TUCNType;
}

TUCNType *UCNType = NULL;

// ----- TInvolvement --------------------------------------------------------
TInvolvement::TInvolvement()
	: TXferAliasGroup("T_INVOLVEMENT", TInvolvement::Clone, "F_INVOLVEMENT", &involvement, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_INVOLVEMENT;
	descs = Descs + TABLE_INVOLVEMENT;
}

TGroup *TInvolvement::Clone()
{
	return new TInvolvement;
}

TInvolvement *Involvement = NULL;

// ----- TPost ---------------------------------------------------------------
TPost::TPost()
	: TCharAliasGroup("T_POST", TPost::Clone, "F_POST", &post, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_POST;
	descs = Descs + TABLE_POST;
}

TGroup *TPost::Clone()
{
	return new TPost;
}

TPost *Post = NULL;

// ----- TCollege ------------------------------------------------------------
TCollege::TCollege()
	: TCharAliasGroup("T_COLLEGE", TCollege::Clone, "F_COLLEGE", &college, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_COLLEGE;
	descs = Descs + TABLE_COLLEGE;
}

TGroup *TCollege::Clone()
{
	return new TCollege;
}

void TCollege::Fixup(bool)
{
	FixupDepartName(college, name);
}

void TCollege::FixupDepartName(char college, char *name)
{
	if (!strchr(COLLEGE_COMMONS, college))
	{
		for (char *s = name; *s; s++)
			if (strchr("ао", *s) && strchr(" ", s[1]))
				*s = "ао"[DEPARTMENT];
	}
}

#if CUTCITILAWS
void TCollege::CacheCriteria(msql &m)
{
	TCharAliasGroup::CacheCriteria(m);
	m.AddChar("NOT F_COLLEGE", TYPE_CITIZEN_LAW);
}
#endif  // CUTCITILAWS

TCollege *College = NULL;

// ----- TCollegeRight -------------------------------------------------------
TCollegeRight::TCollegeRight()
	: TCharAliasGroup("T_COLLEGE", TCollegeRight::Clone, "F_COLLEGE", &college, "F_RIGHT_NAME", rightName, sizeof rightName)
{
	cache = Cache + CACHE_COLLEGE_RIGHT;
	descs = Descs + TABLE_COLLEGE;
}

TGroup *TCollegeRight::Clone()
{
	return new TCollegeRight;
}

void TCollegeRight::Fixup(bool)
{
	TCollege::FixupDepartName(college, rightName);
}

#if CUTCITILAWS
void TCollegeRight::CacheCriteria(msql &m)
{
	TCharAliasGroup::CacheCriteria(m);
	m.AddChar("NOT F_COLLEGE", TYPE_CITIZEN_LAW);
}
#endif  // CUTCITILAWS

TCollegeRight *CollegeRight = NULL;

// ----- TUser ---------------------------------------------------------------
TUser::TUser()
{
	Add(
		new TString("F_MD5SUM", md5sum, sizeof md5sum),
		new TCharList("F_MENUS", menus, sizeof menus),
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		new TDate("F_DATE", &date, true),
		// 2010:245 LPR: back from TUserBase
		new TString("F_LOGIN", login, sizeof login),
		new TCharAlias("F_KIND", &kind),
		new TLong("F_PERCENT", &percent),
		new TLong("F_PRELOAD", &preload),
		new TLong("F_FAILOGS", &failogs),
		new TLongAlias("F_COMPOS", &compos),
		new TLong("F_ACTYPE", &actype),
		new TCheck("F_ACTYPE_DOMAIN", &actype, USER_DOMAIN),
		NULL
	);

	cache = NULL;
	flags = FLAG_ACTIVE;
	percent = 100;
}

TGroup *TUser::Clone()
{
	return new TUser;
}

bool TUser::Delete(bool exact)
{
	if (exact)
	{
		msql m(StreetMap->Name, MQ_DELETE);
		m.AddString("F_UCN", ucn);
		dataBase->Execute(str(m), ET_FATAL);
	}

	return TUserAliasGroup::Delete(exact);
}

void TUser::ExactCriteria(msql &m, const char *fields)
{
	TPersonAliasGroup::ExactCriteria(m, fields);
}

void TUser::FloatCriteria(msql &m, const char *fields)
{
	TPersonAliasGroup::FloatCriteria(m, fields);
}

double TUser::FullLoad()
{
	//return (preload + rawload) * 100.0 / percent;
	// 2007:256 LPR: new formula
	return rawload * 100.0 / percent + preload;
}

long TUser::RawLoadA()
{
	return round(rawload * 100.0 / percent);
}

void TUser::PrintLoad(mstr &m, bool twoDec)
{
	Print(m);
	m.cat(": ");
	m.printf(twoDec ? "%.2lf" : "%.0lf", FullLoad());
	m.printf(" дела (%ld/%ld%% %+ld)", rawload, percent, preload);
}

bool TUser::Update(const char *fields)
{
	if (!strcmp(ucn, SYSAD))
	{
		if (!strchr(menus, RIGHT_ADMIN))
			return error("Потребителя трябва да има достъп до меню администратор.");

		if (!strchr(functions, RIGHT_NOM))
			return error("Потребителя трябва да има достъп до работа с номенклатури.");
	}

	return TUserAliasGroup::Update(fields);
}

TUser *User = NULL;

// ----- TDefault ------------------------------------------------------------
TDefault::TDefault()
{
	strcpy(ucn, EMPTY);	// 2009:077 LPR: to log() initial failed login
}

TDefault *Default = NULL;

// ----- TRight --------------------------------------------------------------
TRight::TRight()
	: TCharAliasGroup("T_RIGHT", TRight::Clone, "F_RIGHT", &right, "F_NAME", name, sizeof name, false)
{
	cache = Cache + CACHE_RIGHT;
	descs = Descs + TABLE_RIGHT;
}

TGroup *TRight::Clone()
{
	return new TRight;
}

TRight *Right = NULL;

// ----- TComposAlias --------------------------------------------------------
class TComposAlias : public TPersonAlias
{
public:
	TComposAlias(const char *name, char *data, int size) : TPersonAlias(name, data, size) { }

	virtual bool IsNULL() const { return strcmp(Data, EMPTY) && TPersonAlias::IsNULL(); }
};

// ----- TFineComposition ----------------------------------------------------
TFineComposition::TFineComposition()
	: TLongAliasGroup("T_COMPOSITION", TFineComposition::Clone, "F_COMPOSITION", &composition, "F_NAME", name, sizeof name,
		NULL, NULL)
{
	Add(
		new TComposAlias("F_JUDGE", judge, sizeof judge),
		new TPersonAlias("F_SECRETAR", secretar, sizeof secretar),
		new TPersonAlias("F_JUDGE_X", judgeX, sizeof judgeX),
		new TPersonAlias("F_REJUDGE_X", reJudgeX, sizeof reJudgeX),
		new TPersonAlias("F_PRESIDENT", president, sizeof president),
		new TCharAlias("F_COLLEGE", &college),
		new TLong("F_FLAGS", &flags),
		NULL
	);

	descs = Descs + TABLE_COMPOSITION;
}

TGroup *TFineComposition::Clone()
{
	return new TFineComposition;
}

void TFineComposition::ExactCriteria(msql &m, const char *fields)
{
	TFineComposition::FloatCriteria(m, fields);
	m.Add(this, "F_COMPOSITION");
}

void TFineComposition::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_COLLEGE");
}

TFineComposition *FineComposition = NULL;

// ----- TCompositionCrit ----------------------------------------------------
void TCompositionCrit::_ExactCriteria(msql &m, const char *fields)
{
	TCompositionCrit::_FloatCriteria(m, fields);
	m.AddLong("F_COMPOSITION", composition);
}

void TCompositionCrit::_FloatCriteria(msql &m, const char *fields)
{
	m.Begin(_Name(), fields && strcmp(fields, "*") ? fields : "F_COMPOSITION, F_NAME");
	m.AddChar("F_COLLEGE", *COLLEGE_ALLS);
}

void TCompositionCrit::_AliasCriteria(msql &m, const char *xCrit, long)
{
	m.Begin(_Name(), "F_COMPOSITION, F_NAME");
	m.AddChar("F_COLLEGE", *COLLEGE_ALLS);

	if (xCrit)
		m.AddCompos("F_COMPOSITION", xCrit);
}

// ----- TComposition --------------------------------------------------------
TComposition::TComposition()
	: TLongAliasGroup("T_COMPOSITION", TComposition::Clone, "F_COMPOSITION", &composition, "F_NAME", name, sizeof name,
		new TCharAlias("F_CHAR_COMPOSITION", (char *) &composition), NULL)
{
	cache = Cache + CACHE_COMPOSITION;
	descs = Descs + TABLE_COMPOSITION;
}

TGroup *TComposition::Clone()
{
	return new TComposition;
}

void TComposition::ExactCriteria(msql &m, const char *fields) { TCompositionCrit::_ExactCriteria(m, fields); }
void TComposition::FloatCriteria(msql &m, const char *fields) { TCompositionCrit::_FloatCriteria(m, fields); }
void TComposition::AliasCriteria(msql &m, const char *xCrit, long flags) { TCompositionCrit::_AliasCriteria(m, xCrit, flags); }

TComposition *Composition = NULL;

// ----- TCharComposition ----------------------------------------------------
TCharComposition::TCharComposition()
	: TCharAliasGroup("T_COMPOSITION", TCharComposition::Clone, "F_CHAR_COMPOSITION", (char *) &composition,
		"F_NAME", name, sizeof name)
{
	orderField = new TLong("F_COMPOSITION", &composition);
	Add(orderField);

	cache = Cache + CACHE_CHAR_COMPOSITION;
	descs = Descs + TABLE_COMPOSITION;
}

TGroup *TCharComposition::Clone()
{
	return new TCharComposition;
}

void TCharComposition::InitCache()
{
	TGArray<TAliasGroup> *tCache = Cache + CACHE_COMPOSITION;

	for (int index = 0; index < tCache->Count(); index++)
	{
		TAliasGroup *group = (TAliasGroup *) clone();
		*group << (*tCache)[index];
		cache->Add(group);
	}
}

void TCharComposition::ExactCriteria(msql &m, const char *fields) { TCompositionCrit::_ExactCriteria(m, fields); }
void TCharComposition::FloatCriteria(msql &m, const char *fields) { TCompositionCrit::_FloatCriteria(m, fields); }
void TCharComposition::AliasCriteria(msql &m, const char *xCrit, long flags) { TCompositionCrit::_AliasCriteria(m, xCrit, flags); }

TCharComposition *CharComposition = NULL;

// ----- TAge ----------------------------------------------------------------
TAge::TAge()
	: TCharAliasGroup("T_AGE", TAge::Clone, "F_AGE", &age, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_AGE;
	descs = Descs + TABLE_AGE;
}

TGroup *TAge::Clone()
{
	return new TAge;
}

TAge *Age = NULL;

// ----- TGrant --------------------------------------------------------------
TGrant::TGrant()
	: TCharAliasGroup("T_GRANT", TGrant::Clone, "F_GRANT", &grant, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_GRANT;
	descs = Descs + TABLE_GRANT;
}

TGroup *TGrant::Clone()
{
	return new TGrant;
}

TGrant *Grant = NULL;

// ----- TJudged -------------------------------------------------------------
TJudged::TJudged()
	: TCharAliasGroup("T_JUDGED", TJudged::Clone, "F_JUDGED", &judged, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_JUDGED;
	descs = Descs + TABLE_JUDGED;
}

TGroup *TJudged::Clone()
{
	return new TJudged;
}

TJudged *Judged = NULL;

// ----- TPunishment ---------------------------------------------------------
TPunishment::TPunishment()
	: TCharAliasGroup("T_PUNISHMENT", TPunishment::Clone, "F_PUNISHMENT", &punishment, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_PUNISHMENT;
	descs = Descs + TABLE_PUNISHMENT;
}

TGroup *TPunishment::Clone()
{
	return new TPunishment;
}

TPunishment *Punishment = NULL;

// ----- TRecidive -----------------------------------------------------------
TRecidive::TRecidive()
	: TCharAliasGroup("T_RECIDIVE", TRecidive::Clone, "F_RECIDIVE", &recidive, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_RECIDIVE;
	descs = Descs + TABLE_RECIDIVE;
}

TGroup *TRecidive::Clone()
{
	return new TRecidive;
}

TRecidive *Recidive = NULL;

// ----- TCountry ------------------------------------------------------------
TCountry::TCountry()
	: TStringAliasGroup("T_COUNTRY", TCountry::Clone, "F_COUNTRY", country, sizeof country, "F_NAME", name, sizeof name,
		NULL, NULL)
{
	Add(
		new TString("F_COUNTRY_STRING", country, sizeof country),
		new TLong("F_EISPP", &eispp),
		NULL
	);

	cache = Cache + CACHE_COUNTRIES;
	descs = Descs + TABLE_COUNTRY;
}

TGroup *TCountry::Clone()
{
	return new TCountry;
}

static inline bool is_required(TCountry *country) { return !strcmp(country->country, BUILDIN_COUNTRY); }

bool TCountry::Delete(bool exact)
{
	return (exact && is_required(this)) ? say_cant_delete() : TStringAliasGroup::Delete(exact);
}

bool TCountry::Update(const char *fields)
{
	return is_required(this) ? say_cant_update() : TStringAliasGroup::Update(fields);
}

TCountry *Country = NULL;

// ----- TAddrAliasGroup -----------------------------------------------------
TAddrAliasGroup::TAddrAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData,
	const char *nameFieldName, char *nameFieldData, int nameFieldSize)
	: TLongAliasGroup(name, tClone, valueFieldName, valueFieldData, nameFieldName, nameFieldData, nameFieldSize, NULL, NULL)
{
}

bool TAddrAliasGroup::Delete(bool exact)
{
	return exact && Required() ? say_cant_delete() : TLongAliasGroup::Delete(exact);
}

bool TAddrAliasGroup::Update(const char *fields)
{
	return Required() ? say_cant_update() : TLongAliasGroup::Update(fields);
}

// ----- TArea ---------------------------------------------------------------
TArea::TArea()
	: TAddrAliasGroup("T_AREA", TArea::Clone, "F_AREA", &area, "F_NAME", name, sizeof name)
{
	Add(new TLong("F_AREA_LONG", &area));

	cache = Cache + CACHE_AREA;
	descs = Descs + TABLE_AREA;
}

TGroup *TArea::Clone()
{
	return new TArea;
}

bool TArea::Insert()
{
	if (!area)
	{
		msql m(Name, "MIN(F_AREA)");
		TGetGroup g(str(m), new TLong("MIN", &area));

		if (area > 0)
			area = 0;

		if (--area < -9)
			generator_limit(Name);
	}

	return TLongAliasGroup::Insert();
}

bool TArea::Required()
{
	return area == DEFAULT_AREA || area == BUILDIN_AREA;
}

TArea *Area = NULL;

// ----- TRegion -------------------------------------------------------------
TRegion::TRegion()
	: TAddrAliasGroup("T_REGION", TRegion::Clone, "F_REGION", &region, "F_NAME", name, sizeof name)
{
	Add(
		new TLongAlias("F_AREA", &area),
		new TLong("F_REGION_LONG", &region),
		NULL
	);

	cache = Cache + CACHE_REGION;
	descs = Descs + TABLE_REGION;
}

TGroup *TRegion::Clone()
{
	return new TRegion;
}

void TRegion::CacheCriteria(msql &m)
{
	area = DEFAULT_AREA;
	FloatCriteria(m);
}

bool TRegion::CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags)
{
	return area == DEFAULT_AREA ? TAddrAliasGroup::CacheFill(glist, array, xCrit, flags) : false;
}

void TRegion::ExactCriteria(msql &m, const char *fields)
{
	TRegion::FloatCriteria(m, fields);
	m.Add(this, "F_REGION");
}

void TRegion::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_AREA");
}

bool TRegion::Insert()
{
	if (!region)
	{
		msql m(Name, "MIN(F_REGION)");

		m.AddLong("F_AREA", area);
		TGetGroup g(str(m), new TLong("MIN", &region));

		if (region > 0)
			region = 0;

		if (--region < -9)
			generator_limit(Name);
	}

	return TLongAliasGroup::Insert();
}

bool TRegion::Required()
{
	return (area == DEFAULT_AREA && region == DEFAULT_REGION) || (area == BUILDIN_AREA && region == BUILDIN_REGION);
}

const char *TRegion::Seek(long tArea, long tRegion)
{
	if (cache)
	{
		for (int index = 0; index < cache->Count(); index++)
		{
			TRegion *alias = (TRegion *) (*cache)[index];

			if (alias->area == tArea && alias->region == tRegion)
				return Found(alias);
		}
	}

	area = tArea;
	region = tRegion;
	return TAliasGroup::Seek();
}

TRegion *Region = NULL;

// ----- TFindAddrAliasGroup -------------------------------------------------
TFindAddrAliasGroup::TFindAddrAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName,
	long *valueFieldData, const char *nameFieldName, char *nameFieldData, int nameFieldSize)
	: TAddrAliasGroup(name, tClone, valueFieldName, valueFieldData, nameFieldName, nameFieldData, nameFieldSize)
{
}

void TFindAddrAliasGroup::AliasCriteria(msql &m, const char *, long flags)
{
	if (flags & ADDR_NAMED)
	{
		FloatCriteria(m);
		m.AddLike("F_NAME", NameField->S(), true);
	}
	else
		ExactCriteria(m);
}

// ----- TUCLP ---------------------------------------------------------------
TUCLP::TUCLP()
	: TFindAddrAliasGroup("T_UCLP", TUCLP::Clone, "F_UCLP", &uclp, "F_NAME", name, sizeof name)
{
	Add(
		new TLongAlias("F_AREA", &area),
		new TLongAlias("F_REGION", &region),
		new TLong("F_UCLP_LONG", &uclp),
		NULL
	);

	descs = Descs + TABLE_UCLP;
}

TGroup *TUCLP::Clone()
{
	return new TUCLP;
}

void TUCLP::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_AREA, F_REGION");
}

bool TUCLP::Insert()
{
	if (!uclp)
	{
		msql m(Name, "MIN(F_UCLP)");
		TGetGroup g(str(m), new TLong("MIN", &uclp));

		if (uclp > 0)
			uclp = 0;

		if (--uclp < -9999)
			generator_limit(Name);
	}

	return TLongAliasGroup::Insert();
}

bool TUCLP::Required()
{
	return uclp == BUILDIN_UCLP || (_COURT_UCLP && uclp == COURT_UCLP);
}

TUCLP *UCLP = NULL;

// ----- TStreet -------------------------------------------------------------
TStreet::TStreet()
	: TFindAddrAliasGroup("T_STREET", TStreet::Clone, "F_STREET", &street, "F_NAME", name, sizeof name)
{
	Add(
		new TLongAlias("F_UCLP", &uclp),
		new TLong("F_STREET_LONG", &street),
		NULL
	);

	descs = Descs + TABLE_STREET;
}

TGroup *TStreet::Clone()
{
	return new TStreet;
}

void TStreet::ExactCriteria(msql &m, const char *fields)
{
	TStreet::FloatCriteria(m, fields);
	m.Add(this, "F_STREET");
}

void TStreet::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCLP");
}

bool TStreet::Insert()
{
	if (!street)
	{
		msql m(Name, "MIN(F_STREET)");

		m.AddLong("F_UCLP", uclp);
		TGetGroup g(str(m), new TLong("MIN", &street));

		if (street > 0)
			street = 0;

		if (--street < -9999)
			generator_limit(Name);
	}

	return TLongAliasGroup::Insert();
}

bool TStreet::Required()
{
	return uclp == BUILDIN_UCLP && street == BUILDIN_STREET;
}

const char *TStreet::Seek(long tUCLP, long tStreet)
{
	uclp = tUCLP;
	street = tStreet;
	return TAliasGroup::Seek();
}

TStreet *Street = NULL;

// ----- TCompanyType --------------------------------------------------------
TCompanyType::TCompanyType()
	: TCharAliasGroup("T_COMPANY_TYPE", TCompanyType::Clone, "F_COMPANY_TYPE", &companyType, "F_NAME", name, sizeof name)
{
	Add(
		new TLong("F_SUBJECT", &subject),
		new TLong("F_REGISTER", &registry),
		new TString("F_RENAME", reName, sizeof reName),
		NULL
	);

	cache = Cache + CACHE_COMPANY_TYPE;
	descs = Descs + TABLE_COMPANY_TYPE;
}

TGroup *TCompanyType::Clone()
{
	return new TCompanyType;
}

bool TCompanyType::DamnName(const char *s)
{
	for (int i = 0; i < cache->Count(); i++)
	{
		const char *name = (*cache)[i]->NameField->S();
		size_t len = strlen(name);

		if (!strncmpi(name, s, len) && !isalpha(s[len]))
			return true;
	}

	return false;
}

TCompanyType *CompanyType = NULL;

// ----- THall ---------------------------------------------------------------
THall::THall()
	: TLongAliasGroup("T_HALL", THall::Clone, "F_HALL", &hall, "F_NAME", name, sizeof name, NULL,
		new TLong("F_FLAGS", &flags))
{
	Add(
		new TString("F_ALIAS", alias, sizeof alias),
		new TString("F_ADDRESS", address, sizeof address),
		new TLong("F_HALL_LONG", &hall),
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		NULL
	);

	cache = Cache + CACHE_HALLS;
	descs = Descs + TABLE_HALL;
	genLimit = 99;
	flags = FLAG_ACTIVE;
}

TGroup *THall::Clone()
{
	return new THall;
}

bool THall::Delete(bool exact)
{
	if (exact)
	{
		TLoadWeek loadWeek;
		TLoadDate loadDate;
		loadWeek.hall = loadDate.hall = hall;
		
		if (!loadWeek.Delete(false) || !loadDate.Delete(false))
			return false;
	}

	return TLongAliasGroup::Delete(exact);
}

THall *Hall = NULL;

// ----- TInterval -----------------------------------------------------------
TInterval::TInterval()
	: TCharAliasGroup("T_INTERVAL", TInterval::Clone, "F_INTERVAL", &interval, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_INTERVAL;
	descs = Descs + TABLE_INTERVAL;
}

TGroup *TInterval::Clone()
{
	return new TInterval;
}

TInterval *Interval = NULL;

// ----- TAppealResolution ---------------------------------------------------
TAppealResolution::TAppealResolution()
	: TCharAliasGroup("T_APPEAL_RESOLUTION", TAppealResolution::Clone, "F_RESOLUTION", &resolution,
		"F_NAME", name, sizeof name)
{
	Add(new TString("F_RESOLUTIONS", resolutions, sizeof resolutions));

	cache = Cache + CACHE_APPEAL_RESOLUTION;
	descs = Descs + TABLE_APPEAL_RESOLUTION;
}

TGroup *TAppealResolution::Clone()
{
	return new TAppealResolution;
}

TAppealResolution *AppealResolution = NULL;

// ----- TCrimeForm ----------------------------------------------------------
TCrimeForm::TCrimeForm()
	: TCharAliasGroup("T_CRIME_FORM", TCrimeForm::Clone, "F_CRIME_FORM", &crimeForm, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_CRIME_FORM;
	descs = Descs + TABLE_CRIME_FORM;
}

TGroup *TCrimeForm::Clone()
{
	return new TCrimeForm;
}

TCrimeForm *CrimeForm = NULL;

// ----- TCrimeStage ---------------------------------------------------------
TCrimeStage::TCrimeStage()
	: TCharAliasGroup("T_CRIME_STAGE", TCrimeStage::Clone, "F_CRIME_STAGE", &crimeStage, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_CRIME_STAGE;
	descs = Descs + TABLE_CRIME_STAGE;
}

TGroup *TCrimeStage::Clone()
{
	return new TCrimeStage;
}

TCrimeStage *CrimeStage = NULL;

// ----- TJailMode -----------------------------------------------------------
TJailMode::TJailMode()
	: TCharAliasGroup("T_JAIL_MODE", TJailMode::Clone, "F_JAIL_MODE", &jailMode, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_JAIL_MODE;
	descs = Descs + TABLE_JAIL_MODE;
}

TGroup *TJailMode::Clone()
{
	return new TJailMode;
}

TJailMode *JailMode = NULL;

// ----- TAccomply -----------------------------------------------------------
TAccomply::TAccomply()
	: TXferAliasGroup("T_ACCOMPLY", TAccomply::Clone, "F_ACCOMPLY", &accomply, "F_NAME", name, sizeof name)
{
	// 2012:004 LRQ: flags support
	flagsField = new TLong("F_FLAGS", &flags);
	Add(flagsField, new TCharAlias("F_DEBTOR_STATUS", &debtorStatus), NULL);

	cache = Cache + CACHE_ACCOMPLY;
	descs = Descs + TABLE_ACCOMPLY;
}

TGroup *TAccomply::Clone()
{
	return new TAccomply;
}

TAccomply *Accomply = NULL;

// ----- TCalendar -----------------------------------------------------------
TCalendar::TCalendar()
	: TGroup("T_CALENDAR", TCalendar::Clone)
{
	Add(
		new TLong("F_YEAR", &year),
		new TString("F_JAN", jan, sizeof jan),
		new TString("F_FEB", feb, sizeof feb),
		new TString("F_MAR", mar, sizeof mar),
		new TString("F_APR", apr, sizeof apr),
		new TString("F_MAY", may, sizeof may),
		new TString("F_JUN", jun, sizeof jun),
		new TString("F_JUL", jul, sizeof jul),
		new TString("F_AUG", aug, sizeof aug),
		new TString("F_SEP", sep, sizeof sep),
		new TString("F_OCT", oct, sizeof oct),
		new TString("F_NOV", nov, sizeof nov),
		new TString("F_DEC", dec, sizeof dec),
		NULL
	);

	descs = Descs + TABLE_CALENDAR;
	months[0] = jan;
	months[1] = feb;
	months[2] = mar;
	months[3] = apr;
	months[4] = may;
	months[5] = jun;
	months[6] = jul;
	months[7] = aug;
	months[8] = sep;
	months[9] = oct;
	months[10] = nov;
	months[11] = dec;
}

TGroup *TCalendar::Clone()
{
	return new TCalendar;
}

void TCalendar::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_YEAR");
}

void TCalendar::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.AddOrder("F_YEAR");
}

void TCalendar::Initialize()
{
	CDate date;

	date.year = year;
	for (date.month = 1; date.month <= 12; date.month++)
	{
		int dom = CDate::DaysInMonth(date.year, date.month);

#if RANGECHECK
		if (*months[date.month - 1])
			fatal("initialize non-empty calendar");
#endif  // RANGECHECK

		for (date.day = 1; date.day <= dom; date.day++)
			strcach(months[date.month - 1], date.DayOfWeek() <= 5 ? DAY_WORK : DAY_HOLY);
	}
}

bool TCalendar::Insert()
{
	Initialize();
	return TGroup::Insert();
}

void TCalendar::SetWork(int month, int day, bool work)
{
	months[month - 1][day - 1] = work ? DAY_WORK : DAY_HOLY;
}

bool TCalendar::Work(long tYear, int month, int day)
{
	if (year != tYear)
	{
		year = tYear;
		// 2005:069 LPR: can't happen (tm), but just in case...
		if (!Get())
			Initialize();
	}

	// 2003:075 Any missing days are now work
	return months[month - 1][day - 1] != DAY_HOLY;
}

TCalendar *Calendar = NULL;

// ----- TMonth --------------------------------------------------------------
TMonth::TMonth()
	: TLongAliasGroup("T_MONTH", TMonth::Clone, "F_MONTH", &month, "F_NAME", name, sizeof name, NULL, NULL)
{
	Add(new TString("F_ALIAS", alias, sizeof alias));

	cache = Cache + CACHE_MONTH;
	descs = Descs + TABLE_MONTH;
}

TGroup *TMonth::Clone()
{
	return new TMonth;
}

TMonth *Month = NULL;

// ----- TCalInterval --------------------------------------------------------
TCalInterval::TCalInterval()
	: TCharAliasGroup("T_CAL_INTERVAL", TCalInterval::Clone, "F_CAL_INTERVAL", &calInterval, "F_NAME", name, sizeof name)
{
	Add(
		new TCharAlias("F_INTERVAL", &interval),
		new TLong("F_COUNT", &count),
		new TLong("F_DELTA", &delta),
		new TCharAlias("F_POST", &post),
		NULL
	);

	cache = Cache + CACHE_CAL_INTERVAL;
	descs = Descs + TABLE_CAL_INTERVAL;
}

TGroup *TCalInterval::Clone()
{
	return new TCalInterval;
}

void TCalInterval::Fixup(bool)
{
	if (delta == INTERVAL_DELTA_Y)
		delta = SCHED_DELTA_Y;
	else if (delta == INTERVAL_DELTA_X)
		delta = SCHED_DELTA_X;
}

void TCalInterval::InitCache()
{
	TCharAliasGroup::InitCache();

	for (int index = 0; index < cache->Count(); index++)
	{
		*this << (*cache)[index];
		strcach_nx(_SCHED_ALLS, calInterval);
	}
}

TCalInterval *CalInterval = NULL;

// ----- TSumType ------------------------------------------------------------
TSumType::TSumType()
	: TCharAliasGroup("T_SUM_TYPE", TSumType::Clone, "F_SUM_TYPE", &sumType, "F_NAME", name, sizeof name)
{
	Add(
		new TString("F_ALIAS", alias, sizeof alias),
		new TCharAlias("F_ACCOUNT_TYPE", &accountType),
		new TString("F_EXEC_LIST_TEMPLATE", execListTemplate, sizeof execListTemplate),
		new TLong("F_FLAGS", &flags),
		NULL
	);

	cache = Cache + CACHE_SUM_TYPE;
	descs = Descs + TABLE_SUM_TYPE;
}

TGroup *TSumType::Clone()
{
	return new TSumType;
}

void TSumType::InitCache()
{
	TCharAliasGroup::InitCache();

	for (int index = 0; index < cache->Count(); index++)
	{
		*this << (*cache)[index];

		if (accountType)
		{
			strcach(_SUM_TYPE_PAYMENTS, sumType);
			strcach(accountType == ACCOUNT_TYPE_BUDGET ? _SUM_TYPE_BUDGETS : _SUM_TYPE_DEPOSITS, sumType);
		}
		// 2014:302
		CatchFlag(_SUM_TYPE_EXEC_LISTS, SUM_FLAG_EXEC_LIST_PRIVATE | SUM_FLAG_EXEC_LIST_COUNTRY);
		CatchFlag(_SUM_TYPE_EXEC_LIST_PRIVATES, SUM_FLAG_EXEC_LIST_PRIVATE);
		CatchFlag(_SUM_TYPE_EXEC_LIST_COUNTRIES, SUM_FLAG_EXEC_LIST_COUNTRY);
		CatchFlag(_SUM_TYPE_OTHERS, SUM_FLAG_OTHER);
		if ((flags & SUM_FLAG_ZERO) == 0)
			strcach(_SUM_TYPE_VALUES, sumType);
	}
}

TSumType *SumType = NULL;

// ----- TSumState ------------------------------------------------------------
TSumState::TSumState()
	: TCharAliasGroup("T_SUM_STATE", TSumState::Clone, "F_SUM_STATE", &sumState, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_SUM_STATE;
	descs = Descs + TABLE_SUM_STATE;
}

TGroup *TSumState::Clone()
{
	return new TSumState;
}

TSumState *SumState = NULL;

// ----- TAccountType ---------------------------------------------------------
TAccountType::TAccountType()
	: TCharAliasGroup("T_ACCOUNT_TYPE", TAccountType::Clone, "F_ACCOUNT_TYPE", &accountType, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_ACCOUNT_TYPE;
	descs = Descs + TABLE_ACCOUNT_TYPE;
}

TGroup *TAccountType::Clone()
{
	return new TAccountType;
}

TAccountType *AccountType = NULL;

// ----- TStreetMap -----------------------------------------------------------
TStreetMap::TStreetMap()
	: TGroup("T_STREET_MAP", TStreetMap::Clone)
{
	Add(
		new TString("F_UCN", ucn, sizeof ucn),
		new TLongAlias("F_UCLP", &uclp),
		new TLongAlias("F_STREET", &street),
		new TTextList("F_MAP", &map, SIZE_OF_STREET_MAP),
		new TString("F_STREET_NAME", streetName, SIZE_OF_STREET_NAME),
		NULL
	);

	descs = Descs + TABLE_STREET_MAP;
	// 2010:018 LPR/LRQ: comout, many uclps; 2010:272 LPR: uncomout, default
	uclp = DEFAULT_UCLP;
	// 2004:016 BRW: cache
	lastUCLP = lastStreet = 0;
}

TGroup *TStreetMap::Clone()
{
	return new TStreetMap;
}

bool TStreetMap::Active(int no)
{
	return no > 0 && no <= map.len() && map[no - 1] == NUMAP_ACTIVE;
}

void TStreetMap::Adjust()
{
	const char *end = str(map);

	for (const char *s = end; *s; s++)
		if (*s == NUMAP_ACTIVE)
			end = s + 1;

	map.cut(end);
}

void TStreetMap::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN, F_UCLP, F_STREET");
}

void TStreetMap::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN, F_UCLP");
	m.AddOrder("F_STREET");
}

bool TStreetMap::Insert()
{
	Adjust();
	return TGroup::Insert();
}

void TStreetMap::PrintMap(mstr &m)
{
	constant s = str(map);
	bool first = true;
	int i = 0;

	while (s[i])
	{
		if (s[i] == NUMAP_ACTIVE)
		{
			if (first)
			{
				m.cat(" # ");
				first = false;
			}
			else
				m.cat(", ");

			if (!strncmp(s + i, NUMAP_FULL_RANGE, LENGTH_OF_NUMAP_FULL_RANGE))
			{
				int end = i + LENGTH_OF_NUMAP_FULL_RANGE;

				while (s[end] == NUMAP_ACTIVE)
					end++;

				m.printf("%d..%d", ++i, end);
				i = end;
			}
			else if (!strncmp(s + i, NUMAP_HALF_RANGE, LENGTH_OF_NUMAP_HALF_RANGE))
			{
				int end = i + LENGTH_OF_NUMAP_HALF_RANGE;

				while (!strncmp(s + end, NUMAP_CONT_RANGE, LENGTH_OF_NUMAP_CONT_RANGE))
					end += LENGTH_OF_NUMAP_CONT_RANGE;

				i++;
				m.printf("%d..%d %sчетни", i, end, i % 2 ? "не" : "");
				i = end;
			}
			else
				m.printf("%d", ++i);
		}
		else
			i++;
	}
}

void TStreetMap::Print(mstr &m)
{
	m.cat(StreetName());
	PrintMap(m);
}

char *TStreetMap::StreetName()
{
	if (uclp != lastUCLP || street != lastStreet)
	{
		TStreet tStreet;
		mstr m;

		tStreet << *this;
		tStreet.Get();
		tStreet.Print(m);
		strzcpy(streetName, str(m), sizeof streetName);
		lastUCLP = uclp;
		lastStreet = street;
	}

	return streetName;
}

bool TStreetMap::Update(const char *fields)
{
	Adjust();
	return TGroup::Update(fields);
}

TStreetMap *StreetMap = NULL;		// 2003:254

// ----- TKnownPerson --------------------------------------------------------
TKnownPerson::TKnownPerson()
	: TUCNGroup("T_KNOWN_PERSON", TKnownPerson::Clone)
{
	Add(
		new TCharAlias("F_INVOLVEMENT", &involvement),
		new TLong("F_FLAGS", &flags),
		new TLong("F_CSJID", &csjid),
		new TLong("F_EXPERT_ID", &expertId),
		new TString("F_PERSONAL_NO", personalNo, sizeof personalNo),
		new TTimeStamp("F_BIRTH_DATE", &birthDate),
		new TDate("F_BIRTH_PART", &birthDate, false),
		NULL
	);

	descs = Descs + TABLE_KNOWN_PERSON;
	flags = FLAG_ACTIVE;
}

TGroup *TKnownPerson::Clone()
{
	return new TKnownPerson;
}

void TKnownPerson::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
}

void TKnownPerson::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_INVOLVEMENT");
	m.AddOrder("F_UCN_TYPE DESC, F_UCN");
}

void TKnownPerson::PedName(mstr &m, bool full)
{
	m.cat(involvement == INVOLVEMENT_JUROR && (flags & JUROR_PEDAGOG) ? "п " : "  ");

	if (full && strchr(UCN_CITIZENS, ucnType))
	{
		TSideWind sideWind;
		sideWind << this;
		sideWind.PrintNames(m);
	}
	else
		Print(m);
}

void TKnownPerson::Print(mstr &m)
{
	if (strchr(UCN_CITIZENS, ucnType))
		print_user(m, ucn, ucnType, !strchr(INVOLVEMENT_CSJIDS, involvement), true);
	else if (strchr(UCN_FIRMS, ucnType))
	{
		TFirm firm;

		firm.ucnType = ucnType;
		strcpy(firm.ucn, ucn);

		if (firm.Get())
			m.cat(firm.name);
	}
	else
		fatal("%s: ucn %d not supported by print", Name, ucnType);
}

// ----- TCourt --------------------------------------------------------------
TCourt::TCourt()
	: TLongAliasGroup("T_COURT", TCourt::Clone, "F_COURT", &court, "F_NAME", name, sizeof name,
		new TCharAlias("F_COURT_TYPE", &courtType), NULL)
{
	Add(
		new TLongAlias("F_UCLP", &uclp),
		new TLongAlias("F_COURT_AREA", &courtArea),
		new TLong("F_EISPP", &eispp),
		new TLong("F_FLAGS", &flags),
		NULL
	);

	cache = Cache + CACHE_COURTS;
	descs = Descs + TABLE_COURT;
}

TGroup *TCourt::Clone()
{
	return new TCourt;
}

TCourt *Court = NULL;

// ----- TWeekday ------------------------------------------------------------
TWeekday::TWeekday()
	: TCharAliasGroup("T_WEEKDAY", TWeekday::Clone, "F_WEEKDAY", &weekday, "F_NAME", name, sizeof name)
{
	Add(new TString("F_ALIAS", alias, sizeof alias));

	cache = Cache + CACHE_WEEKDAY;
	descs = Descs + TABLE_WEEKDAY;
}

TGroup *TWeekday::Clone()
{
	return new TWeekday;
}

TWeekday *Weekday = NULL;		// 2005:060

// ----- TLoadHall -----------------------------------------------------------
TLoadHall::TLoadHall(const char *name, TGroup *(*const tClone)())
	: TGroup(name, tClone)
{
	Add(
		new TLongAlias("F_HALL", &hall),

		new TCharAlias("F_BEF_COLLEGE", &bef.college),
		new TLongAlias("F_BEF_COMPOSITION", &bef.composition),
		new TLong("F_BEF_FLAGS", &bef.flags),
		new TCheck("F_BEF_FLAGS_RESERVED", &bef.flags, LOAD_HALL_RESERVED),
		new TString("F_BEF_TEXT", bef.text, sizeof bef.text),

		new TCharAlias("F_AFT_COLLEGE", &aft.college),
		new TLongAlias("F_AFT_COMPOSITION", &aft.composition),
		new TLong("F_AFT_FLAGS", &aft.flags),
		new TCheck("F_AFT_FLAGS_RESERVED", &aft.flags, LOAD_HALL_RESERVED),
		new TString("F_AFT_TEXT", aft.text, sizeof aft.text),

		NULL
	);
}

bool TLoadHall::Empty()
{
	return !bef.college && !bef.flags && !aft.college && !aft.flags;
}

bool TLoadHall::Save(bool edit)
{
	if (edit)
		return Empty() ? Delete(true) : Update();

	if (!Empty())
		return Insert();

	return true;
}

// ----- TLoadWeek -----------------------------------------------------------
TLoadWeek::TLoadWeek()
	: TLoadHall("T_LOAD_WEEK", TLoadWeek::Clone)
{
	Add(new TCharAlias("F_WEEKDAY", &weekday));

	descs = Descs + TABLE_LOAD_WEEK;
}

TGroup *TLoadWeek::Clone()
{
	return new TLoadWeek;
}

void TLoadWeek::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_HALL, F_WEEKDAY");
}

void TLoadWeek::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_HALL");
	m.AddOrder("F_WEEKDAY");
}

// ----- TLoadDate -----------------------------------------------------------
TLoadDate::TLoadDate()
	: TLoadHall("T_LOAD_DATE", TLoadDate::Clone)
{
	Add(new TDate("F_DATE", &date, false));

	descs = Descs + TABLE_LOAD_DATE;
}

TGroup *TLoadDate::Clone()
{
	return new TLoadDate;
}

void TLoadDate::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_HALL, F_DATE");
}

void TLoadDate::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_HALL");
	m.AddOrder("F_DATE");
}

// ----- TSection ------------------------------------------------------------
TSection::TSection()
	: TCharAliasGroup("T_SECTION", TSection::Clone, "F_SECTION", &section, "F_NAME", name, sizeof name, false)
{
	cache = Cache + CACHE_SECTION;
	descs = Descs + TABLE_SECTION;
}

TGroup *TSection::Clone()
{
	return new TSection;
}

TSection *Section = NULL;

// ----- TConnectType --------------------------------------------------------
TConnectType::TConnectType()
	: TXferAliasGroup("T_CONNECT_TYPE", TConnectType::Clone, "F_CONNECT_TYPE", &connectType, "F_NAME", name, sizeof name)
{
	Add(
		new TString("F_ALIAS", alias, sizeof alias),
		new TString("F_CONNECT_KINDS", connectKinds, sizeof connectKinds),
		new TCharAlias("F_SENDER_TYPE", &senderType),
		new TLong("F_EISPP", &eispp),
		NULL
	);

	cache = Cache + CACHE_CONNECT_TYPE;
	descs = Descs + TABLE_CONNECT_TYPE;
}

TGroup *TConnectType::Clone()
{
	return new TConnectType;
}

TConnectType *ConnectType = NULL;

// ----- TArticle -----------------------------------------------------------
TArticle::TArticle()
	: TLongAliasGroup("T_ARTICLE", TArticle::Clone, "F_ARTICLE", &article, "F_NAME", name, sizeof name, NULL,
		new TLong("F_FLAGS", &flags))
{
	Add(
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		new TLong("F_ARTICLE_LONG", &article),
		NULL
	);

	cache = Cache + CACHE_ARTICLES;
	descs = Descs + TABLE_ARTICLE;
}

TGroup *TArticle::Clone()
{
	return new TArticle;
}

bool TArticle::Delete(bool exact)
{
	if (exact)
	{
		mstr m;

		m.printf("SELECT * FROM %s WHERE F_ARTICLE = %ld;", ArticleSubject->Name, article);
		TQuery q(str(m));
		while (q.Read())
		{
			*ArticleSubject << q;
			if (!ArticleSubject->Delete(true))
				return false;
		}
	}

	return TLongAliasGroup::Delete(exact);
}

TArticle *Article = NULL;

// ----- TArticleSubject -----------------------------------------------------
TArticleSubject::TArticleSubject()
	: TLongAliasGroup("T_ARTICLE_SUBJECT", TArticleSubject::Clone, "F_ARTICLE", &article, "F_NAME", aname, sizeof aname,
		NULL, new TLong("F_FLAGS", &aflags))	// 2014:212 LPR: "F_FLAGS" is invalid and must be cached
{
	Add(new TLongAlias("F_SUBJECT", &subject));

	cache = Cache + CACHE_ARTICLE_SUBJECT;
	descs = Descs + TABLE_ARTICLE_SUBJECT;
}

TGroup *TArticleSubject::Clone()
{
	return new TArticleSubject;
}

void TArticleSubject::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_ARTICLE, F_SUBJECT");
}

void TArticleSubject::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_SUBJECT");
}

void TArticleSubject::Fixup(bool force)
{
	if (force || !any(aname))
	{
		strcpy(aname, Article->Seek(article));
		aflags = Article->flags;
	}
}

void TArticleSubject::Print(mstr &m)
{
	if (article)
	{
		Fixup(false);
		m.printf("%5ld %s", article, aname);
	}
}

bool TArticleSubject::Match(const char *xCrit, long flags)
{
	return subject == ArticleSubject->subject && TLongAliasGroup::Match(xCrit, flags);
}

TArticleSubject *ArticleSubject = NULL;

// ----- TRealityType --------------------------------------------------------
TRealityType::TRealityType(const char *tName, TGroup *(*tClone)(), const char *valueName, const char *tLongValueName)
	: TLongAliasGroup(tName, tClone, valueName, &realityType, "F_NAME", name, sizeof name,
		new TCharAlias("F_XCRIT", (char *) &realityType), new TLong("F_FLAGS", &flags)), longValueName(tLongValueName)
{
	Add(
		new TLong(longValueName, &realityType),
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		NULL
	);

	flags = FLAG_ACTIVE;
}

bool TRealityType::Insert()
{
	if (!realityType)
	{
		mstr m;

		m.printf("SELECT MAX(%s) FROM %s", ValueField()->Name, Name);
		TGetGroup g(str(m), new TLong("MAX", &realityType));

		if (++realityType > 99)
			generator_limit(Name);
	}

	return TLongAliasGroup::Insert();
}

// ----- TCharRealityType ---------------------------------------------------
TCharRealityType::TCharRealityType(TRealityType *tRealityType, TGroup *(*tClone)())
	: TCharAliasGroup(tRealityType->Name, tClone, "F_CHAR_REALITY_TYPE", (char *) &realityType,
		"F_NAME", name, sizeof name, false)
{
	orderField = new TLong(tRealityType->ValueField()->Name, &realityType);

	Add(
		orderField,
		new TLong("F_FLAGS", &flags),
		NULL
	);
}

TGroup *TCharRealityType::ClonePenality()
{
	return new TCharRealityType(PenalityType, TCharRealityType::ClonePenality);
}

TGroup *TCharRealityType::CloneReason()
{
	return new TCharRealityType(ReasonType, TCharRealityType::CloneReason);
}

// ----- TPenalityType -------------------------------------------------------
TPenalityType::TPenalityType()
	: TRealityType("T_PENALITY_TYPE", TPenalityType::Clone, "F_PENALITY_TYPE", "F_PENALITY_TYPE_LONG"),
		penalityType(realityType)
{
	Add(
		new TCheck("F_FLAGS_HIREJ", &flags, PENALITY_TYPE_HIREJ),
		new TCheck("F_FLAGS_BOOK", &flags, PENALITY_TYPE_BOOK),
		NULL
	);

	cache = Cache + CACHE_PENALITY_TYPES;
	descs = Descs + TABLE_PENALITY_TYPE;
	// 2005:353 LRQ/LPR: book by default
	flags |= PENALITY_TYPE_BOOK;
}

TGroup *TPenalityType::Clone()
{
	return new TPenalityType;
}

TPenalityType *PenalityType = NULL;

// ----- TReasonType ---------------------------------------------------------
TReasonType::TReasonType()
	: TRealityType("T_REASON_TYPE", TReasonType::Clone, "F_REASON_TYPE", "F_REASON_TYPE_LONG"), reasonType(realityType)
{
	cache = Cache + CACHE_REASON_TYPES;
	descs = Descs + TABLE_REASON_TYPE;
}

TGroup *TReasonType::Clone()
{
	return new TReasonType;
}

TReasonType *ReasonType = NULL;

// ----- TDanger ------------------------------------------------------------
TDanger::TDanger(TGroup *(*const tClone)())
	: TLongAliasGroup("T_DANGER", tClone, "F_DANGER", &danger, "F_NAME", name, sizeof name, NULL,
		new TLong("F_FLAGS", &flags))
{
	Add(
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		new TLong("F_DANGER_LONG", &danger),
		NULL
	);

	cache = Cache + CACHE_DANGERS;
	descs = Descs + TABLE_DANGER;
}

TGroup *TDanger::Clone()
{
	return new TDanger;
}

bool TDanger::Delete(bool exact)
{
	if (exact)
	{
		mstr m;

		m.printf("SELECT * FROM %s WHERE F_DANGER = %ld;", DangerSubject->Name, danger);
		TQuery q(str(m));
		while (q.Read())
		{
			*DangerSubject << q;
			if (!DangerSubject->Delete(true))
				return false;
		}
	}

	return TLongAliasGroup::Delete(exact);
}

void TDanger::FloatCriteria(msql &m, const char *fields)
{
	TLongAliasGroup::FloatCriteria(m, fields);
}

TDanger *Danger = NULL;

// ----- TDangerSubject ------------------------------------------------------
TDangerSubject::TDangerSubject()
	: TLongAliasGroup("T_DANGER_SUBJECT", TDangerSubject::Clone, "F_DANGER", &danger, "F_NAME", dname, sizeof dname, NULL,
		new TLong("F_FLAGS", &dflags))	// 2014:212 LPR: "F_FLAGS" is invalid and must be cached
{
	Add(new TLongAlias("F_SUBJECT", &subject));

	cache = Cache + CACHE_DANGER_SUBJECT;
	descs = Descs + TABLE_DANGER_SUBJECT;
}

TGroup *TDangerSubject::Clone()
{
	return new TDangerSubject;
}

void TDangerSubject::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_DANGER, F_SUBJECT");
}

void TDangerSubject::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_SUBJECT");
}

void TDangerSubject::Fixup(bool force)
{
	if (force || !any(dname))
	{
		if (danger)
		{
			strcpy(dname, Danger->Seek(danger));
			dflags = Danger->flags;
		}
		else	// called by Print()
			*dname = '\0';
	}
}

bool TDangerSubject::Match(const char *xCrit, long flags)
{
	return subject == DangerSubject->subject && TLongAliasGroup::Match(xCrit, flags);
}

TDangerSubject *DangerSubject = NULL;

// ----- TRank ---------------------------------------------------------------
TRank::TRank()
	: TLongAliasGroup("T_RANK", TRank::Clone, "F_RANK", &rank, "F_NAME", name, sizeof name, NULL, NULL)
{
	Add(new TString("F_ALIAS", alias, sizeof alias));

	cache = Cache + CACHE_RANK;
	descs = Descs + TABLE_RANK;
}

TGroup *TRank::Clone()
{
	return new TRank;
}

bool TRank::Seek(const char *name)
{
	for (int i = 0; i < cache->Count(); i++)
	{
		TRank *rank = (TRank *) (*cache)[i];

		if (!strcmpi(name, rank->name) || !strcmpi(name, rank->alias))
		{
			*this << rank;
			return true;
		}
	}

	return false;
}

TRank *Rank = NULL;

// ----- TGOP ----------------------------------------------------------------
TGOP::TGOP()
	: TCharAliasGroup("T_GOP", TGOP::Clone, "F_GOP", &gop, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_GOPS;
	descs = Descs + TABLE_GOP;
}

TGroup *TGOP::Clone()
{
	return new TGOP;
}

TGOP *GOP = NULL;

// ----- TConnectKind --------------------------------------------------------
TConnectKind::TConnectKind()
	: TXferAliasGroup("T_CONNECT_KIND", TConnectKind::Clone, "F_CONNECT_KIND", &connectKind, "F_NAME", name, sizeof name)
{
	Add(new TString("F_ALIAS", alias, sizeof alias));

	cache = Cache + CACHE_CONNECT_KIND;
	descs = Descs + TABLE_CONNECT_KIND;
}

TGroup *TConnectKind::Clone()
{
	return new TConnectKind;
}

TConnectKind *ConnectKind = NULL;

// ----- TConfigBase ---------------------------------------------------------
TConfigBase::TConfigBase(const char *name, const char *tConfig, int tIndesc)
	: TGroup(name, NULL)
{
	Add(new TString("F_CONFIG", config, sizeof config));

	if (tConfig)
		strcpy(config, tConfig);

	descs = Descs + tIndesc;
}

void TConfigBase::ExactCriteria(msql &m, const char *fields)
{
	TConfigBase::FloatCriteria(m, fields);
	m.Add(this, "F_CONFIG");
}

void TConfigBase::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
}

bool TConfigBase::Global() const
{
	return !strcmp(config, CONFIG_NAME_GLOBAL);
}

// ----- TConfigName ---------------------------------------------------------
TConfigName::TConfigName(const char *config)
	: TConfigBase("T_CONFIG_NAME", config, TABLE_CONFIG_NAME)
{
	Add(
		new TLong("F_NO", &no),
		new TDate("F_DATE", &date, false),
		new TString("F_VERSION", version, sizeof version),
		NULL
	);

	date = Today;
	CreateVersion();
}

bool TConfigName::Check()
{
	// not <= since *GLOBAL is free and new_serial generates +1
	if (no >= 1 && no < CONFIG_MAX)
	{
	#if TESTVER  // 2018-07-09
		msql m(Name, "COUNT(F_NO)");
		m.AddLong("F_NO", no);

		if (dataBase->GetLong(str(m), "COUNT") == 1)
	#else  // 20180709
		msql m(Name, "COUNT(F_NO)");
		long count;

		m.AddLong("F_NO", no);
		TGetGroup g(str(m), new TLong("COUNT", &count));

		if (count == 1)
	#endif  // 20180709
		{
			if (date == Today && !strcmp(version, VERSION))
				return true;

			date = Today;
			CreateVersion();
		}
		else if (!Generate("модифициране"))
			return false;
	}

	return TConfigName::Update();
}

void TConfigName::CreateVersion()
{
	sprintf(version, "%s%c%s", VERSION, tolower(*dataBase->Protocol()), EXTRAVER);
}

bool TConfigName::Generate(const char *what)
{
	// not <= see above
	for (long i = 1; i < CONFIG_MAX; i++)
	{
		msql m(Name, "F_NO");

		m.AddLong("F_NO", i);
		TQuery q(str(m));

		if (!q.Read())
		{
			no = i;
			return true;
		}
	}

	return error("%s: %s: надминат е максималния брой конфигурации.", Name, what);
}

bool TConfigName::Insert()
{
	return Generate("добавяне") && TConfigBase::Insert();
}

bool TConfigName::Delete(bool exact)
{
	if (exact)
	{
		TConfigData configData(NULL);
		msql m(configData.Name, MQ_DELETE);

		m.AddString("F_CONFIG", config);
		dataBase->Execute(str(m), ET_FATAL);
	}

	return TGroup::Delete(exact);
}

// ----- TConfigData ---------------------------------------------------------
TConfigData::TConfigData(const char *config)
	: TConfigBase("T_CONFIG_DATA", config, TABLE_CONFIG_DATA)
{
	Add(
		new TString("F_SECTION", section, sizeof section),
		new TStringAlias("F_NAME", name, sizeof name),
		new TString("F_VALUE", value, sizeof value),
		NULL
	);
}

#if 20180328
bool TConfigData::AskEdit(const char *value, const TSettingData *setting) const
{
	if (!AskEdit(setting->flags))
		return false;

	if (Global() && (setting->type == SETTING_PROGRAM || setting->type == SETTING_DIRECTORY) && !absolute(value))
	{
		return ask("В глобалната конфигурация е желателно да се указват абсолютни имена на %s.\n\n%s",
			setting->type == SETTING_PROGRAM ? "програми" : "директориии", Q_CONTINUE);
	}

	return true;
}

bool TConfigData::AskEdit(long flags) const
{
	if (flags & (Global() ? SETTING_NON_GLOBAL : SETTING_NON_LOCAL))
	{
		const char *scope;

		switch (flags & (SETTING_NON_GLOBAL | SETTING_NON_LOCAL))
		{
			case SETTING_NON_GLOBAL : scope = " глобално"; break;
			case SETTING_NON_LOCAL : scope = " локално"; break;
			default : scope = "";	// "at all"
		}

		return ask("Тази настройка не би трябвало да бъде добавяна или редактирана%s.\n\n%s", scope, Q_CONTINUE);
	}

	return true;
}
#else  // 20180328
bool TConfigData::AskEdit(long flags) const
{
	if (flags & (Global() ? SETTING_NON_GLOBAL : SETTING_NON_LOCAL))
	{
		const char *s;

		switch (flags & (SETTING_NON_GLOBAL | SETTING_NON_LOCAL))
		{
			case SETTING_NON_GLOBAL : s = " глобално"; break;
			case SETTING_NON_LOCAL : s = " локално"; break;
			default : s = "";	// "never"
		}

		return ask("Тази настройка не би трябвало да бъде добавяна или редактирана%s.\nЖелаете ли да продължите?", s);
	}

	return true;
}
#endif  // 20180328

void TConfigData::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_CONFIG, F_SECTION, F_NAME");
}

void TConfigData::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_CONFIG");

	if (any(section))
		m.Add(this, "F_SECTION");
}

void TConfigData::Print(mstr &m)
{
	m.printf("%s=%s", name, value);
}

// ----- TSubpoenaKind -------------------------------------------------------
TSubpoenaKind::TSubpoenaKind() :
	TCharAliasGroup("T_SUBPOENA_KIND", TSubpoenaKind::Clone, "F_SUBPOENA_KIND", &subpoenaKind, "F_NAME", name, sizeof name)
{
	Add(
		new TString("F_ALIAS", alias, sizeof alias),
		new TString("F_KINDS", kinds, sizeof kinds),
		new TString("F_STATUSES", statuses, sizeof statuses),
		new TString("F_INVOLVEMENTS", involvements, sizeof involvements),
		new TString("F_LAWSUIT_KINDS", lawsuitKinds, sizeof lawsuitKinds),
		new TString("F_ATTRIBUTES", attributes, sizeof attributes),
		new TChar("F_PROC", &proc),
		new TCharAlias("F_GENERIC_KIND", &genericKind),
		new TString("F_TEMPLATE", htemplate, sizeof htemplate),
		new TString("F_INFO", info, sizeof info),
		new TText("F_ADDITIONAL_TEXT", &additionalText, SIZE_OF_ADDITIONAL_TEXT),
		new TString("F_SURROUND_KINDS", surroundKinds, sizeof surroundKinds),
		NULL
	);

	cache = Cache + CACHE_SUBPOENA_KIND;
	descs = Descs + TABLE_SUBPOENA_KIND;
}

TGroup *TSubpoenaKind::Clone()
{
	return new TSubpoenaKind;
}

bool TSubpoenaKind::HasAttribute(char attr) const
{
	return strchr(attributes, attr) != NULL;
}

TSubpoenaKind *SubpoenaKind = NULL;

// ----- TReceivedStatus -----------------------------------------------------
TReceivedStatus::TReceivedStatus() :
	TCharAliasGroup("T_RECEIVED_STATUS", TReceivedStatus::Clone,
		"F_RECEIVED_STATUS", &receivedStatus, "F_NAME", name, sizeof name)
{
	Add(
		new TString("F_ALIAS", alias, sizeof alias),
		new TChar("F_CLERK", &clerk),
		new TChar("F_DELIVERER", &deliverer),
		new TChar("F_ALLOW_DATE_RECEIVED", &allowDateReceived),
		new TChar("F_ALLOW_DATE_RETURNED", &allowDateReturned),
		new TChar("F_ALLOW_SCHEDULE", &allowSchedule),
		new TChar("F_SHOW_SUBPOENA_DATE", &showSubpoenaDate),
		NULL
	);

	cache = Cache + CACHE_RECEIVED_STATUS;
	descs = Descs + TABLE_RECEIVED_STATUS;
}

TGroup *TReceivedStatus::Clone()
{
	return new TReceivedStatus;
}

TReceivedStatus *ReceivedStatus = NULL;

// ----- TDebtorStatus -------------------------------------------------------
TDebtorStatus::TDebtorStatus()
	: TXferAliasGroup("T_DEBTOR_STATUS", TDebtorStatus::Clone, "F_DEBTOR_STATUS", &debtorStatus,
		"F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_DEBTOR_STATUS;
	descs = Descs + TABLE_DEBTOR_STATUS;
}

TGroup *TDebtorStatus::Clone()
{
	return new TDebtorStatus;
}

TDebtorStatus *DebtorStatus = NULL;

// ----- TBanConSist ---------------------------------------------------------
TBanConSist::TBanConSist()
	: TLongAliasGroup("T_BANCONSIST", TBanConSist::Clone, "F_BANCONSIST", &banConSist, "F_FAKE", fake, sizeof fake, NULL,
		new TLong("F_FLAGS", &flags))
{
	Add(
		new TLong("F_BANCONSIST_LONG", &banConSist),
		new TText("F_TEXT", &text, SIZE_OF_BANCONSIST_TEXT),
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		NULL
	);

	*text1 = '\0';
	genLimit = 9999;
	cache = Cache + CACHE_BANCONSIST;
	descs = Descs + TABLE_BANCONSIST;
}

TGroup *TBanConSist::Clone()
{
	return new TBanConSist;
}

TBanConSist *BanConSist = NULL;

// ----- TEisAliasGroup ------------------------------------------------------
TEisAliasGroup::TEisAliasGroup(const char *tName, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData)
	: TLongAliasGroup(tName, tClone, valueFieldName, valueFieldData, "F_NAME", name, sizeof name, NULL,
		new TLong("F_FLAGS", &flags)), lset(NULL)
{
}

bool TEisAliasGroup::MatchCode(long code, long flags) const
{
	return !(flags & EISA_LIMIT) || !lset->Count() || lset->Find(code) != INT_MAX;
}

// ----- TEisVal -------------------------------------------------------------
TEisVal::TEisVal()
	: TEisAliasGroup("T_EISPP_VAL", TEisVal::Clone, "F_CODE", &code)
{
	Add(
		new TLongAlias("F_NOM", &nom),
		new TString("F_ALIAS", alias, sizeof alias),
		NULL
	);

	descs = Descs + TABLE_EISPP_VAL;
}

TGroup *TEisVal::Clone()
{
	return new TEisVal;
}

void TEisVal::CacheCriteria(msql &m)
{
	m.Begin(Name);
}

bool TEisVal::CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags)
{
	if (!cache)
	{
		cache = Cache + CACHE_EISPP_VAL;
		InitCache();
	}

	return TEisAliasGroup::CacheFill(glist, array, xCrit, flags);
}

bool TEisVal::Contains(long tNom, long tCode)
{
	nom = tNom;
	code = tCode;
	return Try();
}

void TEisVal::ExactCriteria(msql &m, const char *fields)
{
	TEisVal::FloatCriteria(m, fields);
	m.Add(this, "F_CODE");
}

void TEisVal::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NOM");
}

bool TEisVal::Match(const char *xCrit, long flags)
{
	return EisVal->MatchCode(code, flags) && nom == EisVal->nom && TEisAliasGroup::Match(xCrit, flags);
}

const char *TEisVal::Seek(long tNom, long tCode)
{
	nom = tNom;
	code = tCode;
	return TAliasGroup::Seek();
}

bool TEisVal::operator<=(const TEisVal &that) const
{
#if TESTVER
	return strcmpi(name, that.name);
#else  // TESTVER
	return eispp_instance(code) <= eispp_instance(that.code);
#endif  // TESTVER
}

TEisVal *EisVal = NULL;

// ----- TSbeDesc ------------------------------------------------------------
TSbeDesc::TSbeDesc()
	: TGroup("T_SBE_DESC", TSbeDesc::Clone)
{
	Init();
}

TSbeDesc::TSbeDesc(const char *tName, long tFlags, const char *tValue)
	: TGroup("T_SBE_DESC", TSbeDesc::Clone)
{
	Init();

	vid = -1;
	strcpy(name, tName);
	flags = tFlags;

	if (tValue)
		strcpy(value, tValue);
}

void TSbeDesc::Init()
{
	Add(
		new TLong("F_VID", &vid),
		new TString("F_NAME", name, sizeof name),
		new TLong("F_FLAGS", &flags),
		new TString("F_VALUE", value, sizeof value),
		NULL
	);

	descs = Descs + TABLE_SBE_DESC;
}

TGroup *TSbeDesc::Clone()
{
	return new TSbeDesc;
}

void TSbeDesc::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_VID, F_NAME");
}

void TSbeDesc::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_VID");
	m.AddLike("NOT F_NAME", ".", false);
}

TSbeDesc *SbeDesc = NULL;

// ----- TSessionPrice -------------------------------------------------------
TSessionPrice::TSessionPrice()
	: TGroup("T_SESSION_PRICE", TSessionPrice::Clone)
{
	Add(
		new TDate("F_DATE", &date, false),
		new TMoney("F_PRICE", &price),
		NULL
	);

	descs = Descs + TABLE_SESSION_PRICE;
}

TGroup *TSessionPrice::Clone()
{
	return new TSessionPrice;
}

void TSessionPrice::ExactCriteria(msql &m, const char *fields)
{
	TSessionPrice::FloatCriteria(m, fields);
	m.Add(this, "F_DATE");
}

void TSessionPrice::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
}

TSessionPrice *SessionPrice = NULL;

// ----- TEisDepart ----------------------------------------------------------
TEisDepart::TEisDepart()
	: TEisAliasGroup("T_EISPP_DEPART", TEisDepart::Clone, "F_CODE", &code)
{
	Add(
		new TLongAlias("F_NOM", &nom),
		new TString("F_TLACR", tlacr, sizeof tlacr),
		new TString("F_ALIAS", alias, sizeof alias),
		new TString("F_UNAME", uname, sizeof uname),
		NULL
	);

	descs = Descs + TABLE_EISPP_DEPART;
}

TGroup *TEisDepart::Clone()
{
	return new TEisDepart;
}

bool TEisDepart::CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags)
{
	if (!cache)
	{
		cache = Cache + CACHE_EISPP_DEPART;
		InitCache();
	}

	if (flags & EISA_NAMED)
	{
		char *names = strnew(name);
		char *s;

		for (s = names; (s = strstr(s, "ГР.")) != NULL; s += 3)
			if (s == names || !isalnum(s[-1]))
				memset(s, ' ', 3);

		for (s = names; (s = strstr(s, "ГРАД")) != NULL; s += 4)
			if ((s == names || !isalnum(s[-1])) && !isalnum(s[4]))
				memset(s, ' ', 4);

		unsigned words = split_words(names, "-");

		for (int index = 0; index < cache->Count(); index++)
		{
			TEisDepart *depart = (TEisDepart *) (*cache)[index];
			depart->Fixup(false);

			if (xCrit && !eispp_ucn_match(*xCrit, depart->nom))
				continue;

			if (find_words(names, depart->uname, words, flags))
				CacheCollect(glist, array, depart);
		}

		delete[] names;
		return true;
	}

	return TEisAliasGroup::CacheFill(glist, array, xCrit, flags);
}

void TEisDepart::CacheCriteria(msql &m)
{
	// 2012:353 LPR: eisnom now removes poisons
	//sprintf(s, "SELECT * FROM %s", Name);
	m.Begin(Name);
}

void TEisDepart::AliasCriteria(msql &m, const char *, long) { m.clear(); }

void TEisDepart::Fixup(bool force)
{
	if (force || !any(uname))
	{
		strcpy(uname, name);
		strupr(uname);
	}
}

void TEisDepart::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NOM");
}

bool TEisDepart::Match(const char *xCrit, long flags)
{
	return EisDepart->MatchCode(code, flags) && (EisDepart->nom == EISPP_DEPART || nom == EisDepart->nom) &&
		TEisAliasGroup::Match(xCrit, flags);
}

TEisDepart *EisDepart = NULL;

// ----- TEisCrime -----------------------------------------------------------
TEisCrime::TEisCrime()
	: TEisAliasGroup("T_EISPP_CRIME", TEisCrime::Clone, "F_CODE", &code)
{
	Add(
		new TString("F_ALIAS", alias, sizeof alias),
		new TString("F_UNAME", uname, sizeof uname),
		NULL
	);

	descs = Descs + TABLE_EISPP_CRIME;
}

TGroup *TEisCrime::Clone()
{
	return new TEisCrime;
}

bool TEisCrime::CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags)
{
	if (!cache)
	{
		cache = Cache + CACHE_EISPP_CRIME;
		InitCache();
	}

	if (flags & EISA_NAMED)
	{
		char *const names = strnew(name);
		TCharPtr CP(names);
		unsigned words = split_words(names);

		for (int index = 0; index < cache->Count(); index++)
		{
			TEisCrime *crime = (TEisCrime *) (*cache)[index];
			crime->Fixup(false);

			if (find_words(names, crime->uname, words, flags))
				CacheCollect(glist, array, crime);
		}

		return true;
	}

	return TEisAliasGroup::CacheFill(glist, array, xCrit, flags);
}

void TEisCrime::Fixup(bool force)
{
	if (force || !any(uname))
	{
		strcpy(uname, name);
		strupr(uname);
	}
}

void TEisCrime::FloatCriteria(msql &m, const char *fields)
{
	TEisAliasGroup::FloatCriteria(m, fields);
	//m.AddOrder("F_ORDER");	// 2012:353 IRQ: alphabetical order
}

bool TEisCrime::Match(const char *xCrit, long flags)
{
	return EisCrime->MatchCode(code, flags) && TEisAliasGroup::Match(xCrit, flags);
}

TEisCrime *EisCrime = NULL;

// ----- TEisTransl ----------------------------------------------------------
TEisTransl::TEisTransl()
	: TStringAliasGroup("T_EISPP_TRANSL", TEisTransl::Clone, "F_NAME", name, sizeof name, "F_TEXT", text, sizeof text,
		NULL, NULL)
{
	cache = Cache + CACHE_EISPP_TRANSL;
	descs = Descs + TABLE_EISPP_TRANSL;
}

TGroup *TEisTransl::Clone()
{
	return new TEisTransl;
}

const char *TEisTransl::Seek(const char *name)
{
	if (!EISPP_DIAG_NAMES)
	{
		for (int index = 0; index < cache->Count(); index++)
		{
			TEisTransl *transl = (TEisTransl *) (*cache)[index];

			if (!strcmp(transl->name, name))
				return transl->text;
		}
	}

	return name;
}

TEisTransl *EisTransl = NULL;

// ----- TEisPlace -----------------------------------------------------------
TEisPlace::TEisPlace()
	: TEisAliasGroup("T_EISPP_PLACE", TEisPlace::Clone, "F_CODE", &code)
{
	Add(
		new TLongAlias("F_NOM", &nom),
		new TLong("F_UCLP", &uclp),
		new TString("F_ALIAS", alias, sizeof alias),
		NULL
	);

	descs = Descs + TABLE_EISPP_PLACE;
}

TGroup *TEisPlace::Clone()
{
	return new TEisPlace;
}

void TEisPlace::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_CODE");
}

void TEisPlace::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NOM");
}

void TEisPlace::AliasCriteria(msql &m, const char *, long flags)
{
	if (flags & ADDR_NAMED)
	{
		FloatCriteria(m);
		m.AddLike("F_NAME", NameField->S(), true);
	}
	else
		ExactCriteria(m);
}

TEisPlace *EisPlace = NULL;

// ----- TAccount -------------------------------------------------------------
TAccount::TAccount()
	: TStringAliasGroup("T_ACCOUNT", TAccount::Clone, "F_UCN", ucn, sizeof ucn, "F_NAME", name, sizeof name,
		new TCharAlias("F_UCN_TYPE", &ucnType), new TLong("F_FLAGS", &flags), true)
{
	Add(
		new TCharAlias("F_ACCOUNT_TYPE", &accountType),
		new TString("F_BANK_ACCOUNT", bankAccount, sizeof bankAccount),
		new TString("F_BIC", bic, sizeof bic),
		new TString("F_BANK_NAME", bankName, sizeof bankName),
		new TString("F_NAME_STRING", name, sizeof name),
		new TCheck("F_FLAGS_JUDICIAL", &flags, ACCOUNT_JUDICIAL),
		NULL
	);

	accountType = ACCOUNT_TYPE_BUDGET;
	descs = Descs + TABLE_ACCOUNT;
}

TGroup *TAccount::Clone()
{
	return new TAccount;
}

void TAccount::AliasCriteria(msql &m, const char *xCrit, long)
{
	m.Begin(Name);
	m.AddChars("F_UCN_TYPE", xCrit);
}

void TAccount::ExactCriteria(msql &m, const char *fields)
{
	TAccount::FloatCriteria(m, fields);
	// 2014:303 LPR: lists senders have no accountType 
	if (ucnType != UCN_EXEC_LIST)
		m.Add(this, "F_ACCOUNT_TYPE");
}

void TAccount::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN, F_UCN_TYPE");
}

void TAccount::Generate()
{
	TUCNGroup ucnGroup(Name, Clone);

	ucnGroup.ucnType = ucnType;
	ucnGroup.Generate();
	strcpy(ucn, ucnGroup.ucn);
}

bool TAccount::Insert()
{
	if (is_code(ucnType) && !atob(ucn))
		Generate();

	return TGroup::Insert();
}

void TAccount::Print(mstr &m)
{
	m.cat(name);
}

TAccount *Account = NULL;

// ----- TLegalAidType -------------------------------------------------------
TLegalAidType::TLegalAidType()
	: TLongAliasGroup("T_LEGAL_AID_TYPE", TLegalAidType::Clone, "F_LEGAL_AID_TYPE", &legalAidType, "F_NAME", name,
		sizeof name, NULL, NULL)
{
	cache = Cache + CACHE_LEGAL_AID_TYPE;
	descs = Descs + TABLE_LEGAL_AID_TYPE;
}

TGroup *TLegalAidType::Clone()
{
	return new TLegalAidType;
}

TLegalAidType *LegalAidType = NULL;

// ----- TArchiveIndex -------------------------------------------------------
TArchiveIndex::TArchiveIndex()
	: TStringAliasGroup("T_ARCHIVE_INDEX", TArchiveIndex::Clone, "F_ARCHIVE_INDEX", archiveIndex, sizeof archiveIndex,
		"F_ANAME", archiveIndex, sizeof archiveIndex, NULL, new TLong("F_FLAGS", &flags))
{
	Add(
		new TString("F_NAME", name, sizeof name),
		new TString("F_ARCHIVE_INDEX_STRING", archiveIndex, sizeof archiveIndex),
		new TLong("F_KEEP", &keep),
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		new TCheck("F_FLAGS_EC", &flags, ARCHIVE_INDEX_EC),
		new TCheck("F_FLAGS_FLOAT", &flags, ARCHIVE_INDEX_FLOAT),
		NULL
	);

	cache = Cache + CACHE_ARCHIVE_INDEX;
	descs = Descs + TABLE_ARCHIVE_INDEX;
	flags = FLAG_ACTIVE;
}

TGroup *TArchiveIndex::Clone()
{
	return new TArchiveIndex;
}

bool TArchiveIndex::Delete(bool exact)
{
	if (exact)
	{
		msql m(ArchiveSubject->Name, MQ_DELETE);

		m.AddString("F_ARCHIVE_INDEX", archiveIndex);
		if (!dataBase->Execute(str(m), ET_ERROR, ArchiveSubject->Name, "изтриване"))
			return false;
	}

	return TStringAliasGroup::Delete(exact);
}

TArchiveIndex *ArchiveIndex = NULL;

// ----- TArchiveSubject -----------------------------------------------------
TArchiveSubject::TArchiveSubject()
	: TStringAliasGroup("T_ARCHIVE_SUBJECT", TArchiveSubject::Clone, "F_ARCHIVE_INDEX", archiveIndex, sizeof archiveIndex,
		// 2014:212 LPR: "F_FLAGS" is invalid and must be cached
		"F_NAME", archiveIndex, sizeof archiveIndex, NULL, new TLong("F_FLAGS", &aflags))
{
	Add(
		new TLongAlias("F_SUBJECT", &subject),
		new TCharAlias("F_KIND", &kind),
		NULL
	);

	cache = Cache + CACHE_ARCHIVE_SUBJECT;
	descs = Descs + TABLE_ARCHIVE_SUBJECT;
	aflags = -1;
}

TGroup *TArchiveSubject::Clone()
{
	return new TArchiveSubject;
}

void TArchiveSubject::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_ARCHIVE_INDEX, F_SUBJECT, F_KIND");
}

void TArchiveSubject::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_SUBJECT, F_KIND");
}

void TArchiveSubject::Fixup(bool force)
{
	if (force || aflags == -1)
	{
		if (any(archiveIndex))
		{
			ArchiveIndex->Seek(archiveIndex);
			aflags = ArchiveIndex->flags;
		}
		else
			aflags = 0;
	}
}

bool TArchiveSubject::Match(const char *xCrit, long flags)
{
	return subject == ArchiveSubject->subject && kind == ArchiveSubject->kind && TStringAliasGroup::Match(xCrit, flags);
}

TArchiveSubject *ArchiveSubject = NULL;

// ----- TCurrency -----------------------------------------------------------
TCurrency::TCurrency()
	: TStringAliasGroup("T_CURRENCY", TCurrency::Clone, "F_CURRENCY", currency, sizeof currency, "F_NAME", name,
		sizeof name, NULL, NULL)
{
	Add(
		new TString("F_CURRENCY_STRING", currency, sizeof currency),
		new TLong("F_ORDER", &order),
		new TString("F_NUMERIC", numeric, sizeof numeric),		
		new TString("F_INTEGER_2", integer[2], sizeof integer[2]),
		new TString("F_INTEGER_1", integer[1], sizeof integer[1]),
		new TString("F_INTEGER_0", integer[0], sizeof integer[0]),
		new TLong("F_INTEGER_MFO", &integerMfo),
		new TRadio("F_INTEGER_MALE", &integerMfo, MFO_MALE),
		new TRadio("F_INTEGER_FEMALE", &integerMfo, MFO_FEMALE),
		new TRadio("F_INTEGER_OTHER", &integerMfo, MFO_OTHER),
		new TString("F_FRACTION_2", fraction[2], sizeof fraction[2]),
		new TString("F_FRACTION_1", fraction[1], sizeof fraction[1]),
		new TString("F_FRACTION_0", fraction[0], sizeof fraction[0]),
		new TLong("F_FRACTION_MFO", &fractionMfo),
		new TRadio("F_FRACTION_MALE", &fractionMfo, MFO_MALE),
		new TRadio("F_FRACTION_FEMALE", &fractionMfo, MFO_FEMALE),
		new TRadio("F_FRACTION_OTHER", &fractionMfo, MFO_OTHER),
		NULL
	);

	cache = Cache + CACHE_CURRENCY;
	descs = Descs + TABLE_CURRENCY;
}

TGroup *TCurrency::Clone()
{
	return new TCurrency;
}

void TCurrency::Fixup(bool)
{
	if (!any(integer[1]))
		strcpy(integer[1], integer[2]);

	if (!any(integer[0]))
		strcpy(integer[0], integer[2]);

	if (!any(fraction[1]))
		strcpy(fraction[1], fraction[2]);

	if (!any(fraction[0]))
		strcpy(fraction[0], fraction[2]);
}

const char *TCurrency::Suffix(const char *sum, bool intPart)
{
	unsigned n = atoi(sum);

	if (n >= 2)
		n = 2;

	return intPart ? integer[n] : fraction[n];
}

void TCurrency::CacheCriteria(msql &m)
{
	TStringAliasGroup::CacheCriteria(m);
	m.AddOrder("F_ORDER");
}

TCurrency *Currency = NULL;

// ----- TSLCType ------------------------------------------------------------
TSLCType::TSLCType()
	: TCharAliasGroup("T_SLC_TYPE", TSLCType::Clone, "F_SLC_TYPE", &slcType, "F_NAME", name, sizeof name)
{
	cache = Cache + CACHE_SLC_TYPE;
	descs = Descs + TABLE_SLC_TYPE;
}

TGroup *TSLCType::Clone()
{
	return new TSLCType;
}

TSLCType *SLCType = NULL;

// ----- TSLCReason ----------------------------------------------------------
TSLCReason::TSLCReason()
	: TLongAliasGroup("T_SLC_REASON", TSLCReason::Clone, "F_SLC_REASON", &slcReason, "F_NAME", name, sizeof name,
		new TCharAlias("F_SLC_TYPE", &slcType), new TLong("F_FLAGS", &flags), true)
{
	Add(
		new TLong("F_REASON_LONG", &slcReason),
		new TCharList("F_LAW_KINDS", lawKinds, sizeof lawKinds),
		new TCheck("F_FLAGS_ACTIVE", &flags, FLAG_ACTIVE),
		NULL
	);

	descs = Descs + TABLE_SLC_REASON;
	flags = FLAG_ACTIVE;
}

TGroup *TSLCReason::Clone()
{
	return new TSLCReason;
}

void TSLCReason::FloatCriteria(msql &m, const char *fields)
{
	TLongAliasGroup::FloatCriteria(m, fields);
	m.Add(this, "F_SLC_TYPE");
}

bool TSLCReason::Insert()
{
	msql m;

	m.Begin(Name, "MAX(F_SLC_REASON)");
	SLCType->Seek(slcType);
	TGetGroup g(str(m), new TLong("MAX", &slcReason));
	slcReason++;
	return TLongAliasGroup::Insert();
}

void TSLCReason::PrintPrefixed(mstr &m, const char *slcTypes)
{
	if (strlen(slcTypes) >= 2)
		m.cach(SLC_TYPE_TO_LETTERS[strchr(SLC_TYPE_ALLS, slcType) - SLC_TYPE_ALLS]);
	else
		m.cat(flags & SLC_REASON_BOOK ? "р " : "  ");

	m.cat(" ");
	m.cat(name);
}

TSLCReason *SLCReason = NULL;

// ----- TVSSIndex -----------------------------------------------------------
TVSSIndex::TVSSIndex()
	: TStringAliasGroup("T_VSS_INDEX", TVSSIndex::Clone, "F_INDEX", index, sizeof index, "F_NAME", index, sizeof index,
		NULL, NULL)
{
}

TGroup *TVSSIndex::Clone()
{
	return new TVSSIndex;
}

TVSSIndex *VSSIndex = NULL;

// ----- TVSSIndexSet --------------------------------------------------------
TVSSIndexSet::TVSSIndexSet()
	: TCharAliasGroup("T_VSS_INDEXES", TVSSIndexSet::Clone, "F_COURT_TYPE", &courtType, "F_INDEXES", indexes,
		sizeof indexes)
{
	Add(
		new TDate("F_START", &start, false),
		new TDate("F_FINAL", &final, false),
		new TString("F_TYPES", types, sizeof types),
		new TString("F_KINDS", kinds, sizeof kinds),
		new TString("F_ACCEPTS", accepts, sizeof accepts),
		new TString("F_REJECTS", rejects, sizeof rejects),
		NULL
	);

	cache = Cache + CACHE_VSS_INDEXES;
	descs = Descs + TABLE_VSS_INDEXES;
}

TGroup *TVSSIndexSet::Clone()
{
	return new TVSSIndexSet;
}

void TVSSIndexSet::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add("F_COURT_TYPE, F_ORDER, F_START");
}

void TVSSIndexSet::InitCache()
{
	TCharAliasGroup::InitCache();

	for (int index = 0; index < cache->Count(); index++)
	{
		TVSSIndexSet *indexSet = (TVSSIndexSet *) (*cache)[index];

		if (any(indexSet->accepts))
			strvalue_to_lset(indexSet->accepts, &indexSet->iAccepts, "F_ACCEPTS");

		if (any(indexSet->rejects))
			strvalue_to_lset(indexSet->rejects, &indexSet->iRejects, "F_REJECTS");
	}
}

void TVSSIndexSet::AddIndexes(TGroupArray &array) const
{
	mstr m(indexes);
	char *p;

	for (p = ncstr(m); *p; p++)
		if (isspace(*p) || *p == ',')
			*p = '\0';

	char *next;

	for (char *s = ncstr(m); s < p; s = next)
	{
		next = s + strlen(s) + 1;
		strimall(s);

		if (strlen(s) >= LENGTH_OF_ALIAS)
			fatal("%s: vss index too long", s);

		if (!any(s))
			continue;

		int index;
		int result;

		for (index = 0; index < array.Count(); index++)
			if ((result = strcmp(TYPECHECK_PTR_CAST(TVSSIndex, array[index])->index, s)) >= 0)
				break;

		if (index == array.Count() || result)
		{
			TVSSIndex *tVSSIndex = new TVSSIndex;
			strcpy(tVSSIndex->index, s);
			array.Insert(tVSSIndex, index);
		}
	}
}

void TVSSIndexSet::AddMatching(TGroupArray &array, char tCourtType, const char *tTypes, char kind, const CDate &tStart,
	const CDate &tFinal, bool entireSet)
{
	TGArray<TAliasGroup> *cache = Cache + CACHE_VSS_INDEXES;

	for (int index = 0; index < cache->Count(); index++)
	{
		TVSSIndexSet *indexSet = (TVSSIndexSet *) (*cache)[index];

		if (indexSet->Matches(tCourtType, tTypes, kind, tStart.Empty() ? DATE_MIN : tStart,
			tFinal.Empty() ? DATE_MAX : tFinal))
		{
			if (entireSet)
				array.AddCopy(indexSet);
			else
				indexSet->AddIndexes(array);
		}
	}
}

bool TVSSIndexSet::Matches(char tCourtType, const char *tTypes, char kind, const CDate &tStart, const CDate &tFinal) const
{
	return courtType == tCourtType &&
		(tTypes ? intersect(types, tTypes) : strcmp(types, C2S[TYPE_PUNISHMENT_LAW])) &&
		(!any(kinds) || strchr(kinds, kind))
		&& ((tStart >= start && tStart <= final) || (start >= tStart && start <= tFinal));
}

bool TVSSIndexSet::operator<=(const TVSSIndexSet &that) const
{
	return start != that.start ? start < that.start : order <= that.order;
}

TVSSIndexSet *TVSSIndexSet::Seek(char kind, CDate &date)
{
	TGArray<TAliasGroup> *cache = Cache + CACHE_VSS_INDEXES;
	char type = kind2type(kind);

	for (int index = 0; index < cache->Count(); index++)
	{
		TVSSIndexSet *indexSet = (TVSSIndexSet *) (*cache)[index];

		if (indexSet->Matches(COURT_TYPE + '0', C2S[type], kind, date, date))
			return indexSet;
	}

	return NULL;
}

TVSSIndexSet *VSSIndexSet = NULL;

// ----- TReportRange --------------------------------------------------------
TReportRange::TReportRange()
	: TGroup("T_REPORT_RANGE", TReportRange::Clone)
{
	Add(
		new TString("F_TYPES", types, sizeof types),
		new TLong("F_INSTANCES", &instances),
		new TLong("F_RANGE_ID", &rangeId),
		new TLong("F_SINGLE_SUBJECT", &singleSubject),
		new TString("F_SUBJECTS", subjects, sizeof subjects),
		new TString("F_ATTRIB", attrib, sizeof attrib),
		new TLongString("F_NAME", name, sizeof name),
		new TString("F_ALIAS", alias, sizeof alias),
		//new TChar("F_LETTER", &letter),	// 2017:198 LPR: comout, unused
		NULL
	);

	descs = Descs + TABLE_REPORT_RANGE;
	memset(values, '\0', sizeof values);
}

TGroup *TReportRange::Clone()
{
	return new TReportRange;
}

void TReportRange::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_TYPES, F_INSTANCES, F_RANGE_ID");
}

#if SPLITREPSUBJ
void TReportRange::Load(TGroupArray &reportRanges, const TRangeReportGroup &group, long si, bool instance)
{
	if (!Subject->sd[si].kind)
		return;

	int index;
	bool reportLine = group.attrib == POST_BUILTIN && strchr(Subject->sd[si].attrib, POST_REPORT_LINE);

	for (index = 0; index < reportRanges.Count(); index++)
	{
		TReportRange *tReportRange = (TReportRange *) reportRanges[index];

		if (tReportRange->Consumes(Subject->subject, si))
		{
			strcach_nx(tReportRange->types, Subject->type);

			if (tReportRange->singleSubject && reportLine)
				strcach_nx(tReportRange->attrib, POST_REPORT_LINE);
			return;
		}
	}

	for (int i = 0; i < reportRanges.Count(); i++)
	{
		TReportRange *tReportRange = (TReportRange *) reportRanges[i];

		if (tReportRange->includes.Find(Subject->subject) == INT_MAX &&
			tReportRange->InsideRange(Subject->sd[si].kind, Subject->subject))
		{
			tReportRange->includes.Add(Subject->subject);
		}
	}

	TReportRange *reportRange = new TReportRange;

	strcpy(reportRange->types, C2S[Subject->type]);
	reportRange->instances = instance ? INSTANCE_ALL : si;
	reportRange->rangeId = Subject->subject;
	reportRange->singleSubject = true;
	sprintf(reportRange->subjects, "%ld", Subject->subject);

	if (reportLine)
		strcach(reportRange->attrib, POST_REPORT_LINE);

#if COURT_TYPE == COURT_REGIONAL
	if (Subject->sd[si].kind == KIND_ADMIN_PUNISHMENT_LAW)
		strcach(reportRange->attrib, POST_ALTERNATE);
#endif  // REGIONAL

	strcpy(reportRange->name, Subject->name);
	strcpy(reportRange->alias, any(Subject->alias) ? Subject->alias : Subject->Code(si, instance));
	reportRange->ranges.Add(Subject->subject);
	reportRange->ranges.Add(Subject->subject);
	reportRanges.Add(reportRange);
}

bool TReportRange::Consumes(long subject, long si) const
{
	return ranges.Count() == 2 && ranges[0] == subject && ranges[1] == subject && (instances & (si + 1));
}
#else  // SPLITREPSUBJ
bool TReportRange::InsideRange(const TSubject *subject) const
{
	return (subject->sd[0].kind && InsideRange(subject->sd[0].kind, subject->subject)) ||
		(subject->sd[1].kind && InsideRange(subject->sd[1].kind, subject->subject));
}

bool TReportRange::Consumes(char type, long subject) const
{
	return ranges.Count() == 2 && ranges[0] == subject && ranges[1] == subject && strchr(types, type);
}
#endif  // SPLITREPSUBJ

void TReportRange::Load(TGroupArray &reportRanges, const TRangeReportGroup &group)
{
	msql m(ReportRange->Name);
	bool instance = group.instances % INSTANCE_ALL;
	long si = group.instances - 1;

	for (const char *s = group.types; *s; s++)
	{
		m.AddLike("(F_TYPES" + (s > group.types), C2S[*s], true);
		m.NextOr();
	}

	m.AddString("F_TYPES", "*");
	m.cat(")");

	if (instance)
	{
		m.AddLong("(F_INSTANCES", group.instances);
		m.NextOr();
		m.AddLong("F_INSTANCES", INSTANCE_ALL);
		m.cat(")");
	}

	if (group.attrib)
		m.AddLike("F_ATTRIB", C2S[group.attrib], true);
	else
	{
		m.AddLike("(F_ATTRIB", C2S[POST_BUILTIN], true);
		m.NextOr();
		m.AddIsNull("F_ATTRIB", true);
		m.cat(")");
	}

#if COURT_TYPE == COURT_REGIONAL
	m.AddRange("NOT F_RANGE_ID", group.excludeLow, group.excludeHigh);
#endif  // REGIONAL

	ReportRange->LoadArray(reportRanges, str(m));

	for (int i = 0; i < reportRanges.Count(); i++)
	{
		TReportRange *reportRange = (TReportRange *) reportRanges[i];

		if (!strcmp(reportRange->types, "*"))
			strcpy(reportRange->types, TYPE_LAWSUITS);

		strvalue_to_lranges(reportRange->subjects, reportRange->ranges, "F_SUBJECTS");
	}

	if (group.attrib == POST_BUILTIN || group.withSubjects)
	{
		m.Begin(Subject->Name);
		m.AddChars("F_TYPE", group.types);

		if (instance)
			m.AddIsNull(TSubject::FNI("F_KIND", si), false);

		if (group.attrib == POST_BUILTIN && !group.withSubjects)
		{
			if (instance)
				m.AddLike(TSubject::FNI("F_ATTRIB", si), C2S[POST_REPORT_LINE], true);
			else
			{
				m.AddLike("(F_ATTRIB_I", C2S[POST_REPORT_LINE], true);
				m.NextOr();
				m.AddLike("F_ATTRIB_II", C2S[POST_REPORT_LINE], true);
				m.cat(")");
			}
		}

	#if SPLITREPSUBJ
	#else  // SPLITREPSUBJ
		const char *attrib[2];

		if (instance)
			attrib[0] = attrib[1] = Subject->sd[si].attrib;
		else
		{
			attrib[0] = Subject->sd[0].attrib;
			attrib[1] = Subject->sd[1].attrib;
		}
	#endif  // SPLITREPSUBJ

	#if COURT_TYPE == COURT_REGIONAL
		m.AddRange("NOT F_SUBJECT", group.excludeLow, group.excludeHigh);
	#endif  // REGIONAL

		TQuery q(str(m));
		while (q.Read())
		{
			*Subject << q;

		#if SPLITREPSUBJ
			if (instance)
				Load(reportRanges, group, si, true);
			else
			{
				Load(reportRanges, group, 0, false);
				Load(reportRanges, group, 1, false);
			}
		#else  // SPLITREPSUBJ

			int index;
			bool reportLine = (group.attrib == POST_BUILTIN) &&
				(strchr(attrib[0], POST_REPORT_LINE) || strchr(attrib[1], POST_REPORT_LINE));

			for (index = 0; index < reportRanges.Count(); index++)
			{
				TReportRange *tReportRange = (TReportRange *) reportRanges[index];

				if (tReportRange->Consumes('\0', Subject->subject))
				{
					strcach_nx(tReportRange->types, Subject->type);

					if (tReportRange->singleSubject && reportLine)
						strcach_nx(tReportRange->attrib, POST_REPORT_LINE);
					break;
				}
			}

			if (index < reportRanges.Count())
				continue;

			for (int i = 0; i < reportRanges.Count(); i++)
			{
				TReportRange *tReportRange = (TReportRange *) reportRanges[i];

				if (tReportRange->includes.Find(Subject->subject) == INT_MAX && tReportRange->InsideRange(Subject))
					tReportRange->includes.Add(Subject->subject);
			}

			TReportRange *reportRange = new TReportRange;

			strcpy(reportRange->types, C2S[Subject->type]);
			reportRange->instances = Subject->sd[1].kind ? Subject->sd[0].kind ? INSTANCE_ALL : INSTANCE_2ND :
				INSTANCE_1ST;
			reportRange->rangeId = Subject->subject;
			reportRange->singleSubject = true;
			sprintf(reportRange->subjects, "%ld", Subject->subject);

			if (reportLine)
				strcach(reportRange->attrib, POST_REPORT_LINE);

		#if COURT_TYPE == COURT_REGIONAL
			if (Subject->sd[0].kind == KIND_ADMIN_PUNISHMENT_LAW)
				strcach(reportRange->attrib, POST_ALTERNATE);
		#endif  // REGIONAL

			strcpy(reportRange->name, Subject->name);
			strcpy(reportRange->alias, any(Subject->alias) ? Subject->alias : Subject->Code(si, instance));
			reportRange->ranges.Add(Subject->subject);
			reportRange->ranges.Add(Subject->subject);
			reportRanges.Add(reportRange);
		#endif  // SPLITREPSUBJ
		}
	}

	for (int i = 0; i < reportRanges.Count() - 1; i++)
	{
		const TReportRange *reportRange = (const TReportRange *) reportRanges[i];
		const TReportRange *subjectRange = (const TReportRange *) reportRanges[i + 1];

		if (reportRange->includes.Count() == 1 && subjectRange->singleSubject &&
			!strchr(subjectRange->attrib, POST_REPORT_LINE) && reportRange->ranges[0] == subjectRange->ranges[0])
		{
			reportRanges.Remove(i + 1);
		}
	}
}

bool TReportRange::operator<=(const TReportRange &that) const
{
	return rangeId != that.rangeId ? rangeId < that.rangeId :
		singleSubject != that.singleSubject ? singleSubject < that.singleSubject :
		instances != that.instances ? instances < that.instances : strcmp(types, that.types);
}

bool TReportRange::InsideRange(char kind, long subject) const
{
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	long lawinst = kind ? (strchr(KIND_1ST_LAWSUITS, kind) ? INSTANCE_1ST : INSTANCE_2ND) : (INSTANCE_1ST | INSTANCE_2ND);

	if (lawinst & instances)
#endif  // ADMIN || AREAL ||
	{
		if (strchr(types, kind2type(kind)))
			for (int i = 0; i < ranges.Count(); i += 2)
				if (subject >= ranges[i] && subject <= ranges[i + 1])
					return true;
	}

	return false;
}

size_t TReportRange::Positive() const
{
	for (size_t column = 0; column < RANGE_COLUMNS_MAX; column++)
		if (values[column])
			return column + 1;

	return 0;
}

bool TReportRange::Visible() const
{
	if (strchr(attrib, REPORT_RANGE_HIDDEN))
		return false;

	if (!singleSubject)
		return true;

	if (Positive())
		return true;

	return strchr(attrib, POST_REPORT_LINE);
}

TReportRange *ReportRange = NULL;

// ----- TWhat ---------------------------------------------------------------
TWhat::TWhat()
	: TLongAliasGroup("T_WHAT", TWhat::Clone, "F_WHAT", &what, "F_NAME", name, sizeof name, NULL, NULL)
{
	cache = Cache + CACHE_WHATS;
	descs = Descs + TABLE_WHAT;
}

TGroup *TWhat::Clone()
{
	return new TWhat;
}

TWhat *What = NULL;
