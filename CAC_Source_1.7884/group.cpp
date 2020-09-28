#include "cac.h"

// ----- TRCDKey -------------------------------------------------------------
int TRCDKey::Compare(const TRCDKey &that) const
{
	return type != that.type ? type - that.type : year != that.year ? year - that.year : no - that.no;
}

// ----- TRCDKeyGroup --------------------------------------------------------
TRCDKeyGroup::TRCDKeyGroup(const char *name, TGroup *(*const tClone)())
	: TGroup(name, tClone), antiNoRight('\0')
{
	Add(
		NEW_RCDKEY("F_", &key),
		NULL
	);

	key.type = kind2type(Default->kind);
	key.year = Today.year;
}

void TRCDKeyGroup::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE");
}

void TRCDKeyGroup::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_YEAR, F_TYPE");
}

bool TRCDKeyGroup::Generate()
{
	msql m(Name, "MAX(F_NO)");

	m.AddLong("F_YEAR", key.year);
	m.AddChar("F_TYPE", key.type);

	return generate_max(Name, str(m), &key.no, NO_MAX,
		antiNoRight && strchr(Default->functions, antiNoRight) ? 0 : key.year);
}

void TRCDKeyGroup::LastKey(msql &m, TDatedContainer *target, const char *fields, const char *kinds) const
{
	m.Begin(target->Name, fields);
	m.Add(this, "F_NO, F_YEAR, F_TYPE");

	if (kinds)
		m.AddChars("F_KIND", kinds);

	m.AddOrder("F_DATE DESC");
}

TMovement *TRCDKeyGroup::LastMove(const CDate *date, const CTime *time) const
{
	if ((date != NULL) != (time != NULL))
		fatal("LastMove: date and time must be a complect");

	TMovement *movement = new TMovement;
	msql m(movement->Name);

	m.Add(this, "F_NO, F_YEAR, F_TYPE");
	m.AddChars("F_KIND", KIND_MOVEMENTS);

	if (date)
	{
		m.Add("(F_DATE");
		m.printf("< %ld", date->Value());
		m.NextOr();
		m.AddLong("(F_DATE", date->Value());
		m.Add("F_TIME");
		m.printf("< %ld))", time->Value());
	}

	m.AddOrder("F_DATE DESC, F_TIME DESC");
	TQuery q(str(m));

	if (q.Read())
		*movement << q;
	else
	{
		delete movement;
		movement = NULL;
	}

	return movement;
}

void TRCDKeyGroup::Print(mstr &m)
{
	TChar *kind = (TChar *) Find("F_KIND");

	// 2007:027 LPR: %20s -> %35s, hope that won't break anything
	// 2007:036 URQ: FIX: && kind->C()
	if (kind && kind->C())
		m.printf("%-25s  %5ld/%4ld", Kind->Seek(kind->C()), key.no, key.year);
	else
		m.printf("%-25s  %5ld/%4ld", Type->Seek(key.type), key.no, key.year);
}

struct TLowLevel
{
	const char *name;
	bool infix;
	int elem;
};

static TLowLevel LowLevels[] =
{
	{ "T_SESSION", false, RT_TYPE_LAWSUITS },
	{ "T_DECISION", false, RT_TYPE_LAWSUITS },
	{ "T_SURROUND", true, RT_TYPE_ORIGINS | RT_TYPE_INREGS },
	{ "T_INREG", true, RT_TYPE_ORIGINS },
	{ "T_OUTREG", true, RT_TYPE_ORIGINS },
	{ "T_MOVEMENT", false, RT_TYPE_ORIGINS | RT_TYPE_INREGS | RT_TYPE_PROVES | RT_TYPE_SURROUNDS },
	{ "T_PROVE", false, RT_TYPE_OTHERS },
	{ "T_ARCHIVE", false, RT_TYPE_LAWSUITS },
	{ "T_JUROR_DISTRIB", false, RT_TYPE_LAWSUITS },
	{ NULL, false }
};

char TRCDKeyGroup::Status() const
{
	int elem;
	char status = RESULT_NEW;

	switch (key.type)
	{
		case TYPE_REQUEST : elem = RT_TYPE_REQUESTS; break;
		case TYPE_SURROUND : elem = RT_TYPE_SURROUNDS; status = RESULT_DOCUMENT_NEW; break;
		case TYPE_INREG : elem = RT_TYPE_INREGS; status = RESULT_DOCUMENT_NEW; break;
		case TYPE_OUTREG : elem = RT_TYPE_OUTREGS; break;
		case TYPE_PROVE : elem = RT_TYPE_PROVES; status = RESOLUTION_ATTACHED; break;
		default :
		{
#if RANGECHECK
			if (!strchr(TYPE_LAWSUITS, key.type))
				fatal("%d: type not supported by basic group status", key.type);
#endif  // RANGECHECK
			elem = RT_TYPE_LAWSUITS;
		}
	}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (strchr(TYPE_PROVABLES, key.type))
		elem |= RT_TYPE_OTHERS;
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

	for (TLowLevel *ll = LowLevels; ll->name; ll++)
	{
		if (elem & ll->elem)
		{
			msql m(ll->name, "F_NO");

			m.AddLong(ll->infix ? "F_LAWSUIT_NO" : "F_NO", key.no);
			m.AddLong(ll->infix ? "F_LAWSUIT_YEAR" : "F_YEAR", key.year);
			m.AddChar(ll->infix ? "F_LAWSUIT_TYPE" : "F_TYPE", key.type);

			TQuery q(str(m));
			if (q.Read())
				return RESULT_V_MOVEMENT;
		}
	}

	return status;
}

// ----- TUCNGroup -----------------------------------------------------------
TUCNGroup::TUCNGroup(const char *name, TGroup *(*const tClone)())
	: TGroup(name, tClone)
{
	Add(
		new TString("F_UCN", ucn, sizeof ucn),
		new TCharAlias("F_UCN_TYPE", &ucnType),
		NULL
	);
}

void TUCNGroup::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_UCN, F_UCN_TYPE");
}

void TUCNGroup::FloatCriteria(msql &m, const char *fields)
{
	TUCNGroup::ExactCriteria(m, fields);
}

void TUCNGroup::Generate()
{
	mstr m;

	m.printf("SELECT CAST(MAX(F_UCN) AS CHAR(%d)) AS F_MAX FROM %s WHERE F_UCN_TYPE = '%c'", LENGTH_OF_UCN, Name, ucnType);
	double d = dataBase->GetString(str(m), "F_MAX", ucn) ? atof(ucn) : 0.0;

	if (d < 0.0 || d >= 9999999999.0)
		fatal("%s: generator outside limits", Name);

	sprintf(ucn, "%010.0lf", ++d);
}

bool TUCNGroup::Insert()
{
	if (is_code(ucnType) && !atob(ucn))
		Generate();

	return TGroup::Insert();
}

void TUCNGroup::Print(mstr &m)
{
	m.printf("%3s %10s", UCNType->Seek(ucnType), ucn);
}

bool TUCNGroup::FindKey(const char *name)
{
	TString *value = (TString *) Find(name);

	if (value && any(value->S()))
	{
		msql m(Name, "F_UCN");

		m.AddString(name, value->S());
		m.printf(" AND (NOT F_UCN = '%s' OR NOT F_UCN_TYPE = '%c')", ucn, ucnType);

		TQuery q(str(m));
		return q.Read();
	}

	return false;
}

// ----- TFoundGroup ---------------------------------------------------------
TFoundGroup::TFoundGroup()
	: TUCNGroup("T_UCN_GROUP", NULL)
{
}

TFoundGroup::TFoundGroup(const char *name, TGroup *(*const tClone)())
	: TUCNGroup(name, tClone)
{
}

// ----- TNamedGroup ---------------------------------------------------------
TNamedGroup::TNamedGroup(TName *tNameField)
	: nameField(tNameField)
{
	Add(nameField);
}

TNamedGroup::TNamedGroup(const char *name, TGroup *(*const tClone)(), TName *tNameField)
	: TFoundGroup(name, tClone), nameField(tNameField)
{
	Add(nameField);
}

void TNamedGroup::FloatCriteria(msql &m, const char *fields)
{
	NamedCriteria(m, fields, "", INVOLVEMENT_INTERNAL, false);
}

void TNamedGroup::KnownCriteria(msql &m, char involvement, bool active)
{
	if (strchr(INVOLVEMENT_CHECK_PERSONS, involvement))
	{
		m.Add("(SELECT COUNT(F_UCN) FROM T_KNOWN_PERSON K");
		m.printf("WHERE K.F_UCN = %s.F_UCN AND K.F_UCN_TYPE = %s.F_UCN_TYPE AND K.F_INVOLVEMENT = '%c'",
			Name, Name, involvement2known(involvement));

		if (active)
		{
			m.Add("K.F_FLAGS");
			dataBase->CondActive(m, "K.F_FLAGS");
		}

		m.cat(") > 0");
	}
#if COURT_TYPE == COURT_AREAL
	else if (involvement == INVOLVEMENT_BANCONSIST)
	{
		m.Add("(SELECT COUNT(W.F_NO) FROM T_SIDE_WIND W JOIN T_LAWSUIT L ON L.F_NO = W.F_NO");
		m.printf("AND L.F_YEAR = W.F_YEAR AND L.F_TYPE = W.F_TYPE WHERE W.F_UCN = %s.F_UCN "
			"AND W.F_UCN_TYPE = %s.F_UCN_TYPE", Name, Name);

		m.AddChars("L.F_TYPE", TYPE_TRADE_OR_CITIZEN_LAWS);
		m.AddChars("L.F_SOURCE_KIND", KIND_INCONSISTENCE_REQUESTS);
		m.cat(") > 0");
	}
#endif  // AREAL
}

TNamedGroup *TNamedGroup::NewGroup(char ucnType)
{
	TNamedGroup *ucnGroup = NewXGroup(ucn2uxn(ucnType));

	if (ucnGroup)
		ucnGroup->ucnType = ucnType;

	return ucnGroup;
}

TNamedGroup *TNamedGroup::NewXGroup(char uxnType)
{
	TNamedGroup *ucnGroup;

	switch (uxnType)
	{
		case UXN_CITIZEN : ucnGroup = new TCitizen; break;
		case UXN_FIRM : ucnGroup = new TFirm; break;
		case UXN_ANONYMOUS : ucnGroup = new TSender; break;
		case UXN_UNKNOWN : ucnGroup = new TSender; break;
		case UXN_SENDER : ucnGroup = new TSender; break;
		case UXN_DAMN : ucnGroup = new TDamn; break;
		default : fatal("%d: type not supported by NewXGroup", uxnType);
	}

	return ucnGroup;
}

// ----- TTransGroup ---------------------------------------------------------
TTransGroup::TTransGroup()
	: TNamedGroup(new TName("F_NAME", name, sizeof name))
{
	Add(
		new TName("F_FAMILY", family, sizeof family),
		new TName("F_RENAME", reName, sizeof reName),
		new TName("F_REFAMILY", reFamily, sizeof reFamily),
		new TCharAlias("F_INVOLVEMENT", &involvement),
		new TCharAlias("F_FROM_SIDE", &fromSide),
		new TString("F_BULSTAT", bulstat, sizeof bulstat),
		new TStringAlias("F_COUNTRY", country, sizeof country),
		NULL
	);

	strcpy(country, BUILDIN_COUNTRY);	// 2012:026 IRQ: FIX
}

void TTransGroup::NamedCriteria(msql &, const char *, const char *, char, bool)
{
	fatal("%s: abstract call to NamedCriteria", Name);
}

bool TTransGroup::ValidCriteria()
{
	fatal("%s: abstract call to ValidCriteria", Name);
	return false;
}

// ----- TPeriodGroup --------------------------------------------------------
TPeriodGroup::TPeriodGroup(const char *name)
	: TGroup(name, NULL)
{
	Add(
		new TDate("F_MIN_DATE", &minDate, true),
		new TDate("F_MAX_DATE", &maxDate, true),
		NULL
	);
}

// ----- TGetGroup -----------------------------------------------------------
TGetGroup::TGetGroup(const char *expr, TField *field)
{
	Add(field);

	TQuery q(expr);
	if (q.Read())
		*this << q;
}
