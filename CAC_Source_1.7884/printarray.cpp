#include "cac.h"

class TPrintArray : public TProcess
{
public:
	TPrintArray(const char *tFileName, const offield *tBodyField, TGroupArray *tArray);
	void Execute();
	mstr text;

protected:
	TOffice office;
	TGroupArray *array;
	const offield *body_fields;
	const char *fileName;
	const char *tr1start;
	const char *tr1final;
};

#define printarr OFFICE_GROUP_CAST(TPrintArray)

static void printLaw_title(mstr &m, TGroup *group) { m.cat(str(printarr->text)); }

#undef printarr

static const offield title_fields[] =
{
	{ "F_TITLE", printLaw_title },
	{ NULL, NULL }
};


TPrintArray::TPrintArray(const char *tFileName, const offield *tBodyField, TGroupArray *tArray)
	: TProcess(NULL, "списъка", WAIT), fileName(tFileName), body_fields(tBodyField), array(tArray)
{
}

void TPrintArray::Execute()
{
	if (office.Open(fileName))
	{
		tr1start = office.Find("tr1 start");
		tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, title_fields, this);

		for (int i = 0; i < array->Count(); i++)
				office.Emit(tr1start, tr1final, body_fields, (*array)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}

	Debug();
}

class TApFishLawsuit : public TLawsuit
{
public:
	TApFishLawsuit();
	char citizenName[SIZE_OF_FIRM_NAME];
	char damnName[SIZE_OF_FIRM_NAME];
	char companyType;
};

TApFishLawsuit::TApFishLawsuit() {}

#define lawsuit OFFICE_GROUP_CAST(TApFishLawsuit)

static void apfish_law_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(lawsuit->kind)); }
static void apfish_company_type(mstr &m, TGroup *group) { m.cpy(CompanyType->Seek(lawsuit->companyType)); }
static void apfish_firm_name(mstr &m, TGroup *group) { m.cpy(lawsuit->damnName); }
static void apfish_name(mstr &m, TGroup *group) { m.cpy(lawsuit->citizenName); }

#undef lawsuit

static const offield apfish_fields[] =
{
	{ "F_KIND", apfish_law_kind },
	{ "F_NO", keygroup_key_no },
	{ "F_DATE", container_date },
	{ "F_TYPE", apfish_company_type },
	{ "F_FIRM_NAME", apfish_firm_name },
	{ "F_NAME", apfish_name },
	{ NULL, NULL }
};

bool print_apfish(TLawsuit *lawsuit, TSideWind *sideWind)
{
	if (lawsuit->key.type != TYPE_FIRM_LAW)
		return error("%d: type not supported by print_apfish", lawsuit->key.type);

	TDamn damn;
	TCitizen citizen;
	TApFishLawsuit tLawsuit;

	tLawsuit << lawsuit;

	if (!find_damn(&tLawsuit, &damn))
		return false;

	citizen << sideWind;

	if (!citizen.Get())
		return false;

	tLawsuit.companyType = damn.companyType;
	strcpy(tLawsuit.damnName, damn.name);
	sprintf(tLawsuit.citizenName, "%s %s %s", citizen.name, citizen.reName, citizen.family);

	show_office("PrntApF.htm", apfish_fields, &tLawsuit);
	return true;
}

class TWeekOrDate : public TLoadHall
{
public:
	TWeekOrDate();

	CDate date;
	char weekday;
};

TWeekOrDate::TWeekOrDate()
	: TLoadHall("T_WEEK_OR_DATE", NULL)
{
	Add(
		new TDate("F_DATE", &date, false),
		new TChar("F_WEEKDAY", &weekday),
		NULL
	);
}

void print_data(mstr &m, TLoadData *data)
{
	char s[81];

	if (data->flags && LOAD_HALL_RESERVED)
		strcpy(s, "Резервирана ");
	else if (data->college)
	{
		sprintf(s, "%c%c", *College->Seek(data->college), toupper(*COLLEGE));
		if (data->composition)
			scprintf(s, ", %s състав ", Composition->Seek(data->composition));
	}
	else *s = '\0';

	if (data->flags || data->college)
	{
		if (*data->text)
		{
			if (*s)
				strcat(s, " - ");
			strcat(s, data->text);
		}
	}
	m.cpy(s);
}

#define weekOrDate OFFICE_GROUP_CAST(TWeekOrDate)

static void halls_day(mstr &m, TGroup *group)
{
	char dow;

	if (weekOrDate->date.Empty())
		dow = weekOrDate->weekday;
	else
	{
		m.printf("%Dг., ", &weekOrDate->date);
		dow = weekOrDate->date.DigitOfWeek();
	}

	m.cat(Weekday->Seek(dow));
}

static void halls_hall(mstr &m, TGroup *group)
{
	Hall->Seek(weekOrDate->hall);
	m.cpy(Hall->alias);
}

static void halls_before(mstr &m, TGroup *group) { print_data(m, &weekOrDate->bef); }
static void halls_after(mstr &m, TGroup *group) { print_data(m, &weekOrDate->aft); }

#undef container

static const offield halls_fields[] =
{
	{ "F_DAY", halls_day },
	{ "F_HALL", halls_hall },
	{ "F_BEFORE", halls_before },
	{ "F_AFTER", halls_after },
	{ NULL, NULL }
};

void print_halls(TPeriodGroup *period)
{
	msql m;
	TOffice office;
	TWeekOrDate loadWeekOrDate;

	if (office.Open("PrintHal.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr2start = office.Find("tr2 start");
		constant tr2final = office.Find("tr2 final");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < 2; i++)
		{
			if (i == 0)
				m.cpy("SELECT * FROM T_LOAD_WEEK ORDER BY F_WEEKDAY;");
			else
			{
				m.Begin("T_LOAD_DATE");
				period->AddRange(m);
				m.AddOrder("F_DATE");
			}

			TQuery q(str(m));

			if (i)
				office.Emit(tr1start, tr2final, null_fields, NULL);

			while (q.Read())
			{
				loadWeekOrDate << q;
				office.Emit( i ? tr2final : tr1start, i ? tr1final : tr2start, halls_fields, &loadWeekOrDate);
			}
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

#define subject OFFICE_GROUP_CAST(TSubject)

static void subject_code(mstr &m, TGroup *group) { m.cat(subject->Code(subject->saas, true)); }
static void subject_limited(mstr &m, TGroup *group) { m.cat(strchr(subject->SaasD().attrib, POST_LIMITED) ? "Да" : "Не"); }
static void subject_active(mstr &m, TGroup *group) { m.cat(subject->SaasD().flags & FLAG_ACTIVE ? "Да" : "Не"); }

static void subject_report(mstr &m, TGroup *group)
{
	itom(subject->SaasD().judgeReport, m);
	long quickReport = subject->SaasD().quickReport;

	if (quickReport && quickReport != subject->SaasD().judgeReport)
		m.printf(", %ld", quickReport);
}

#undef subject

static const offield subject_fields[] =
{
	{ "F_SUBJECT",	subject_code },
	{ "F_NAME",		NULL },
	{ "F_LIMITED",	subject_limited },
	{ "F_ACTIVE",	subject_active },
	{ "F_REPORT",	subject_report },
	{ NULL, NULL }
};

// ----- TPrintSubject -------------------------------------------------------
class TPrintSubject : public TPrintArray
{
public:
	TPrintSubject(TGSArray<TSubject> *subjects, char lawKind);
};

TPrintSubject::TPrintSubject(TGSArray<TSubject> *subjects, char lawKind)
	: TPrintArray("PrintSubj.htm", subject_fields, subjects)
{
	text.cpy(Kind->Seek(lawKind));
}

void print_subjects(TGSArray<TSubject> *subjects, char lawKind)
{
	TPrintSubject(subjects, lawKind).Execute();
}

#define oldsubj OFFICE_GROUP_CAST(TOldSubject)

static void oldsubj_limited(mstr &m, TGroup *group) { m.cat(strchr(oldsubj->attrib, POST_LIMITED) ? "Да" : "Не"); }

static void oldsubj_report(mstr &m, TGroup *group)
{
	itom(oldsubj->judgeReport, m);

	if (oldsubj->quickReport && oldsubj->quickReport != oldsubj->judgeReport)
		m.printf(", %ld", oldsubj->quickReport);
}

#undef subject

static const offield oldsubj_fields[] =
{
	{ "F_SUBJECT",	NULL },
	{ "F_NAME",		NULL },
	{ "F_LIMITED",	oldsubj_limited },
	{ "F_ACTIVE",	group_active },
	{ "F_REPORT",	oldsubj_report },
	{ NULL, NULL }
};

// ----- TPrintOldSubject ----------------------------------------------------
class TPrintOldSubject : public TPrintArray
{
public:
	TPrintOldSubject(TGSArray<TOldSubject> *oldSubjects, char lawKind);
};

TPrintOldSubject::TPrintOldSubject(TGSArray<TOldSubject> *oldSubjects, char lawKind)
	: TPrintArray("PrintSubj.htm", oldsubj_fields, oldSubjects)
{
	text.printf("%s - стари предмети", Kind->Seek(lawKind));
}

void print_old_subjects(char lawKind)
{
	TOldSubject oldSubject;
	msql m(oldSubject.Name);
	TGSArray<TOldSubject> oldSubjects;

	m.AddChar("F_KIND", lawKind);
	oldSubject.LoadArray(oldSubjects, str(m));
	TPrintOldSubject(&oldSubjects, lawKind).Execute();
}

// ----- print sides ---------------------------------------------------------
static const offield sideWind_fields[] =
{
	{ "F_TYPE",	basewind_key_type },
	{ "F_NO",	NULL },
	{ "F_YEAR",	NULL },
	{ "F_TEXT",	group_print },
	{ NULL, NULL }
};

void print_trade_sides(TGArray<TBaseWind> *sideWinds)
{
	TPrintArray("TradeSide.htm", sideWind_fields, sideWinds).Execute();
}

#define fineComposition OFFICE_GROUP_CAST(TFineComposition)

static void composition_composition(mstr &m, TGroup *group) { m.cpy(fineComposition->name); }

static void composition_president(mstr &m, TGroup *group)
{
	if (atof(fineComposition->president))
		print_user(m, fineComposition->president);
}

static void composition_judge(mstr &m, TGroup *group)
{
#if COURT_TYPE == COURT_REGIONAL
	if (atof(fineComposition->judge))
		print_user(m, fineComposition->judge);
#endif  // REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (atof(fineComposition->judgeX) && strcmp(fineComposition->president, fineComposition->judgeX))
		print_user(m, fineComposition->judgeX);
#endif  // APPEAL || ADMIN || AREAL || MILITARY
}

static void composition_re_judge(mstr &m, TGroup *group)
{
	if (atof(fineComposition->reJudgeX) && strcmp(fineComposition->president, fineComposition->reJudgeX))
		print_user(m, fineComposition->reJudgeX);
}

static void composition_secretary(mstr &m, TGroup *group)
{
	if (atof(fineComposition->secretar))
		print_user(m, fineComposition->secretar);
}

#undef fineComposition

static const offield composition_fields[] =
{
	{ "F_COMPOSITION", composition_composition },
	{ "F_PRESIDENT", composition_president },
	{ "F_JUDGE", composition_judge },
	{ "F_RE_JUDGE", composition_re_judge },
	{ "F_SECRETARY", composition_secretary },
	{ NULL, NULL }
};

// ----- TPrintComposition ---------------------------------------------------
class TPrintComposition : public TPrintArray
{
public:
	TPrintComposition(TGArray<TFineComposition> *tCompositions, const char *fileName, const offield *bodyField, char college);
	void Execute();

protected:
	TGArray<TFineComposition> compositions;
};

TPrintComposition::TPrintComposition(TGArray<TFineComposition> *tCompositions, const char *fileName, const offield *bodyField, char college)
	: TPrintArray(fileName, bodyField, tCompositions)
{
	text.printf("- %s %s", College->Seek(college), COLLEGE);

	for(int i = 0; i < tCompositions->Count(); i++)
	{
#if COURT_TYPE == COURT_REGIONAL
		if (atof((*tCompositions)[i]->secretar))
#endif  // REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		if (atof((*tCompositions)[i]->president) || (atof((*tCompositions)[i]->judgeX) && strcmp((*tCompositions)[i]->president, (*tCompositions)[i]->judgeX)) || (atof((*tCompositions)[i]->reJudgeX) && strcmp((*tCompositions)[i]->president, (*tCompositions)[i]->reJudgeX)))
#endif  // APPEAL || ADMIN || AREAL || MILITARY
			compositions.Add((*tCompositions)[i]);
	}
	array = &compositions;
}

void TPrintComposition::Execute()
{
	TPrintArray::Execute();
	compositions.Clear();
}

void print_compositions(TGArray<TFineComposition> *compositions, char college)
{

#if COURT_TYPE == COURT_REGIONAL
	const char *fileName = "PrntCom2.htm";
#endif  // REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	const char *fileName = "PrntCom1.htm";
#endif  // APPEAL || ADMIN || AREAL || MILITARY
	TPrintComposition(compositions, fileName, composition_fields, college).Execute();
}

#define realticle OFFICE_GROUP_CAST(TLongAliasGroup)

static void realticles_code(mstr &m, TGroup *group) { itom(realticle->ValueField()->L(), m); }
static void realticles_reason(mstr &m, TGroup *group) { m.cpy(realticle->NameField->S()); }

#undef realticl

static const offield realticles_fields[] =
{
	{ "F_CODE", realticles_code },
	{ "F_REASON", realticles_reason },
	{ "F_ACTIVE", group_active},
	{ NULL, NULL }
};

// ----- TPrintRealticles ----------------------------------------------------
class TPrintRealticles : public TPrintArray
{
public:
	TPrintRealticles(TLongAliasArray *realticles, const char *fileName, const offield *bodyField, const char *what);
};

TPrintRealticles::TPrintRealticles(TLongAliasArray *realticles, const char *fileName, const offield *bodyField, const char *what)
	: TPrintArray(fileName, bodyField, realticles)
{
	text.cpy(what);
}

void print_realticles(TLongAliasArray *realticles, const char *what)
{
	TPrintRealticles(realticles, "PrintReal.htm", realticles_fields, what).Execute();
}

#define dangers OFFICE_GROUP_CAST(TDanger)

static void dangers_code(mstr &m, TGroup *group) { itom(dangers->danger, m); }
static void dangers_name(mstr &m, TGroup *group) { m.cpy(dangers->name); }

#undef dangers

static const offield dangers_fields[] =
{
	{ "F_CODE", dangers_code },
	{ "F_NAME", dangers_name },
	{ "F_ACTIVE", group_active },
	{ NULL, NULL }
};

// ----- TPrintDanger --------------------------------------------------------
class TPrintDanger : public TPrintArray
{
public:
	TPrintDanger(TGArray<TDanger> *dangers, const char *fileName, const offield *bodyField);
};

TPrintDanger::TPrintDanger(TGArray<TDanger> *dangers, const char *fileName, const offield *bodyField)
	: TPrintArray(fileName, bodyField, dangers)
{
}

void print_dangers(TGArray<TDanger> *dangers)
{
	TPrintDanger(dangers, "PrntDang.htm", dangers_fields).Execute();
}
