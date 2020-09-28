#include "cac.h"

// ----- TAbcMode ------------------------------------------------------------
enum ABC_MODE
{
	ABC_MODE_BOTH,
	ABC_MODE_LEFT,
	ABC_MODE_RIGHT,
	ABC_MODE_COND	// right if any, otherwise left
};

struct TAbcMode
{
	const char *lawKinds;
	ABC_MODE abcMode;
};

// ----- KIND_ ---------------------------------------------------------------
static const TAbcMode AbcModes[] =
{
#if COURT_TYPE == COURT_APPEAL
	{ "U",	ABC_MODE_COND },
#endif  // APPEAL
#if COURT_TYPE == COURT_AREAL
	{ "O!",	ABC_MODE_RIGHT },
	{ "DI",	ABC_MODE_COND },
	{ "ZT[{",	ABC_MODE_BOTH },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ "OГ",	ABC_MODE_RIGHT },
	{ "DI",	ABC_MODE_COND },
	{ "Z[",	ABC_MODE_BOTH },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ "OR",	ABC_MODE_RIGHT },
	{ "DI",	ABC_MODE_COND },
	{ "mcZpr",	ABC_MODE_BOTH },	// 2014:093 +pr
#endif  // REGIONAL
	{ NULL,	ABC_MODE_LEFT }	// == default
};

// ----- TAbcCol -------------------------------------------------------------
struct TAbcColN
{
	const char *lawKinds;
	int colN;
};

// ----- KIND_ ---------------------------------------------------------------
static const TAbcColN AbcColNs[] =
{
#if COURT_TYPE == COURT_APPEAL
	// 2015:188 +[!П]
	{ "W[",	2 },
	{ "HU]",	6 },
	{ "ZTяН",	1 },
	{ "NnЯЅ",	5 },
	{ "!",	3 },
	{ "П",	4 },
	{ "ї",	7 },	
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ "яЯZ",	1 },
	{ "MNn",	4 },
	{ "НЅ",	5 },
	{ "Ez",	6 },
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	{ "OW",	2 },
	{ "C",	3 },
	{ "DПU",	4 },
	{ "IH",	6 },
	{ "ZNTnяЯ",	1 },
	{ "[]{}НЕ",	5 },
	{ "ж",	1 },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ "OW",	2 },
	{ "ГC",	3 },
	{ "DПU",	4 },
	{ "IH",	6 },
	{ "ZN",	1 },
	{ "[]",	5 },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ "O",	2 },
	{ "R",	3 },
	{ "DQЕ",	4 },
	{ "I",	6 },
	{ "cmp",	1 },	// 2014:093 +p
	{ "Zr",	5 },	// 2014:093 +r
#endif  // REGIONAL
	{ NULL,	0 }
};

// ----- TAbcBookGroup -------------------------------------------------------
class TAbcBookGroup : public TBookGroup
{
public:
	TAbcBookGroup();

	virtual void FormCriteria(msql &m);

	char name[0x100];		// 2012:215 LPR +related
	bool firm;
	char minLetter[SIZE_OF_LETTER];
	char maxLetter[SIZE_OF_LETTER];
	long allSides;
	long composition;
	long involvement;
	long Abc;
};

TAbcBookGroup::TAbcBookGroup()
	: TBookGroup(name, "F_TYPE", "F_DATE", true, true), firm(false)
{
	Add(
		new TString("F_MIN_LETTER", minLetter, sizeof minLetter),
		new TString("F_MAX_LETTER", maxLetter, sizeof maxLetter),
		new TCheck("F_ALL_SIDES", &allSides, TRUE),
		new TCheck("F_COMPOSITION", &composition, TRUE),
		new TCheck("F_INVOLVEMENT", &involvement, TRUE),
		new TRadio("F_ABC", &Abc, TRUE),
		new TRadio("F_INSOL", &Abc, FALSE),
		new TCheck("F_NEW_FORMAT", &flags, BOOK_NEW_FORMAT),
		NULL
	);

	Abc = true;
	minDate = CDate(1, 1, Today.year);
	flags |= BOOK_NEW_FORMAT;
}

void TAbcBookGroup::FormCriteria(msql &m)
{
#if COURT_TYPE == COURT_AREAL
	firm = (types[0] == TYPE_FIRM_LAW);
#endif  // AREAL
	strcpy(name, firm ? "T_DAMN F" : "T_SIDE_WIND F");
	strcat(name, " JOIN T_LAWSUIT L ON L.F_NO = F.F_NO AND L.F_YEAR = F.F_YEAR AND L.F_TYPE = F.F_TYPE");
	fields = firm ? "F.F_NAME, F.F_NO, F.F_COMPANY_TYPE, F.F_BULSTAT, L.F_KIND" :
		"F.F_NO, L.F_KIND, F.F_DATE, F.F_UCN, F.F_UCN_TYPE, F.F_INVOLVEMENT";

	BeginCriteria(m);
	m.AddChar("F.F_TYPE", *types);
	m.AddRange("F.F_NO", minNo, maxNo);
	m.AddLong("F.F_YEAR", year);

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_APPEAL
	if (!Abc)
		m.AddChars("L.F_SOURCE_KIND", KIND_INCONSISTENCE_REQUESTS);
#endif

	if (!firm)
	{
		m.AddRange("F.F_DATE", minDate, maxDate);
		m.AddOrder("L.F_NO, F.F_UCN, F.F_UCN_TYPE, F.F_INVOLVEMENT, F.F_DATE");
	}
}

#define abcGroup OFFICE_GROUP_CAST(TAbcBookGroup)

static void abcTitle_type(mstr &m, TGroup *group)
{
	abcGroup->types[1] = '\0';
	print_types_list(m, abcGroup->types, false);
	strupr(ncstr(m));
}

static void abcTitle_min_no(mstr &m, TGroup *group)
{
	if (abcGroup->minNo)
		m.printf("%d/%d", abcGroup->minNo, abcGroup->year);
}

static void abcTitle_max_no(mstr &m, TGroup *group)
{
	if (abcGroup->maxNo)
		m.printf("%d/%d", abcGroup->maxNo, abcGroup->year);
}

static void abcTitle_period(mstr &m, TGroup *group){ m.printf("%0D - %0D", &abcGroup->minDate, &abcGroup->maxDate); }

#undef abcGroup

static const offield abcTitle_fields[] =
{
	{ "F_TYPE", abcTitle_type },
	{ "F_MIN_NO", abcTitle_min_no },
	{ "F_MAX_NO", abcTitle_max_no },
	{ "F_PERIOD", abcTitle_period },
	{ NULL, NULL }
};

// ----- TAbcSideWind --------------------------------------------------------
class TAbcSideWind : public TSideWind
{
public:
	TAbcSideWind();

	bool operator<=(const TAbcSideWind &that) const;

	char name[0x100];
	char bulstat[SIZE_OF_BULSTAT];
	char lawKind;
	bool showInv;
	long composition;
	int colN;
	// 2018-03-26: old format
	long printComposition;
	long minNo, maxNo;
};

TAbcSideWind::TAbcSideWind()
{
	Add(
		new TString("F_NAME", name, sizeof name),
		new TString("F_BULSTAT", bulstat, sizeof bulstat),
		NULL
	);
}

bool TAbcSideWind::operator<=(const TAbcSideWind &that) const
{
	return name[0] != that.name[0] ? name[0] < that.name[0] : date.year != that.date.year ? date.year < that.date.year :
		date.month != that.date.month ? date.month < that.date.month : key.no <= that.key.no;
}

#define abcWind OFFICE_GROUP_CAST(TAbcSideWind)

static void abcBook_letter(mstr &m, TGroup *group) { m.printf("%c", *abcWind->name); }

static void abcBook_month(mstr &m, TGroup *group)
{
	m.printf("Месец %s %dг.", Month->Seek(abcWind->date.month), abcWind->date.year);
}

static void abcBook_col(mstr &m, TGroup *group)
{
	int colN;

	if (sscanf(abcWind->Name, "F_COL%d", &colN) == 1 && colN == abcWind->colN)
	{
		m.printf("%s\v%d/%d", Kind->Seek(abcWind->lawKind), abcWind->key.no, abcWind->key.year);

		if (abcWind->printComposition)
			m.printf("\v%s с-в.", Composition->Seek(abcWind->composition));
	}
}

static void abcBook_involvement(mstr &m, TGroup *group)
{
	if (abcWind->showInv)
		m.cpy(Involvement->Seek(abcWind->involvement));
}

static void abcBook_name(mstr &m, TGroup *group) { m.sep_cat("\v", abcWind->name); }

static void abcBook_composition(mstr &m, TGroup *group) { m.cat(Composition->Seek(abcWind->composition)); }
static void abcBook_minNo(mstr &m, TGroup *group) { itom(abcWind->minNo, m); }
static void abcBook_maxNo(mstr &m, TGroup *group) { itom(abcWind->maxNo, m); }
static void abcBook_address(mstr &m, TGroup *group) { print_address(m, abcWind); }
static void abcBook_lawKind(mstr &m, TGroup *group) { m.cat(KindAlias->Seek(abcWind->kind)); }

#undef abcWind

static const offield abcBook_fields[] =
{
	{ "F_LETTER",		abcBook_letter },
	{ "F_MONTH",		abcBook_month },
	{ "*F_COL",			abcBook_col },
	{ "F_INVOLVEMENT",	abcBook_involvement },
	{ "F_NAME",			abcBook_name },
	{ "F_BULSTAT",		group_bulstat },
	// 2018-03-26: old format
	{ "F_NO",			basewind_key_no },
	{ "F_YEAR",			basewind_key_year },
	{ "F_COMPOSITION",	abcBook_composition },
	{ "F_MIN_NO",		abcBook_minNo },
	{ "F_MAX_NO",		abcBook_maxNo },
	{ "F_TYPE",			basewind_key_type },
	{ "F_ADDRESS",		abcBook_address },
	{ "F_LAW_KIND",		abcBook_lawKind },	
	{ NULL, NULL }
};

// ----- TAbcBook ------------------------------------------------------------
class TAbcBook : public TBook
{
public:
	TAbcBook(TWindow *parent, TAbcBookGroup *group);

protected:
	TAbcBookGroup *abcGroup;
	TSideWind sideWind;
	TDamn damn;
	TLawsuit lawsuit;

	TAbcSideWind emptyWind;
	TAbcSideWind *lastWind;		// last printed

	const char *tr2start;
	const char *tr2final;
	TGArray<TAbcSideWind> buffer;
	ABC_MODE abcMode;
	// 2018-03-26 olf format
	const char *th1start;
	const char *th1final;

	void Append(TAbcSideWind *abcWind);
	void Commit();
	virtual void Markers();
	TAbcSideWind *NewAbcWind();
	TGroup *Process(TQuery &q);
	virtual void Collect();
	virtual void EmitRow(TGroup *group);
};

static constant abcBook_file_name(const TAbcBookGroup *group)
{
	if (group->flags & BOOK_NEW_FORMAT)
		return CourtType == COURT_APPEAL ? "Abc_B_fi_p.htm" : "Abc_B_fi__.htm";

	return CourtType == COURT_REGIONAL ? "Abc_O_fi_r.htm" : "Abc_O_fi__.htm";
}

TAbcBook::TAbcBook(TWindow *parent, TAbcBookGroup *group)
	: TBook(parent, group, abcBook_file_name(group), abcBook_fields, abcTitle_fields, BOOK_FIXED_DATES,
		new TGSArray<TAbcSideWind>), abcGroup(group)
{
	lastWind = &emptyWind;
}

void TAbcBook::Commit()
{
	for (int i = 0; i < buffer.Count(); i++)
	{
		TAbcSideWind *abcWind = buffer[i];

		MinMax(abcWind->key.no);
		array->Add(abcWind);
	}

	buffer.Clear();
}

void TAbcBook::Markers()
{
	th1start = office.Find("th1 start");
	tr1start = office.Find("tr1 start");
	tr2start = office.Find("tr2 start");
	tr2final = office.Find("tr2 final");
	th1final = office.Find("th1 final");
	tr1final = office.Find("tr1 final");
}

TAbcSideWind *TAbcBook::NewAbcWind()
{
	mstr name;

	if (abcGroup->firm)
	{
		name.cpy(damn.name);
		name.printf(" %s", CompanyType->Seek(damn.companyType));
	}
	else
	{
		msql m(sideWind.Name, "F_UCN");

		m.Add(&sideWind, "F_NO, F_YEAR, F_TYPE, F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
		m.Add("F_DATE");
		m.printf("< %ld", abcGroup->minDate.Value());

		TQuery q(str(m));
		if (q.Read())
			return NULL;

		sideWind.PrintNames(name);
	}

	if (*str(name) < *abcGroup->minLetter || *str(name) > *abcGroup->maxLetter)
		return NULL;

	TAbcSideWind *abcWind = new TAbcSideWind;

	*abcWind << sideWind;
	strcpy(abcWind->name, str(name));
	abcWind->lawKind = lawsuit.kind;
	abcWind->showInv = abcGroup->involvement;
	abcWind->composition = lawsuit.composition;
	abcWind->printComposition = abcGroup->composition;

	if (abcGroup->firm)
	{
		abcWind->date = lawsuit.date;
		strcpy(abcWind->bulstat, damn.bulstat);
	}
	else if (strchr(UCN_FIRMS, abcWind->ucnType))
	{
		TFirm firm;

		strcpy(firm.ucn, abcWind->ucn);
		firm.ucnType = abcWind->ucnType;

		if (firm.Try("F_BULSTAT"))
			strcpy(abcWind->bulstat, firm.bulstat);
		else
			dprintf("Грешка при четене на %s %s\n", UCNType->Seek(abcWind->ucnType), abcWind->ucn);
	}
	else if (PRINT_UCN && abcWind->ucnType == UCN_CITIZEN_UCN)
		strcpy(abcWind->bulstat, abcWind->ucn);

	const TAbcColN *abcColN;

	for (abcColN = AbcColNs; abcColN->lawKinds; abcColN++)
		if (strchr(abcColN->lawKinds, lawsuit.kind))
			break;

	if (abcColN->lawKinds)
		abcWind->colN = abcColN->colN;
	else
	{
		abcWind->colN = 0;
		dprintf("Не е дефинирана отчетна колона за точен вид дело %d\n", lawsuit.kind);
	}

	return abcWind;
}

TGroup *TAbcBook::Process(TQuery &q)
{
	sideWind << q;
	sideWind.key.type = *abcGroup->types;
	sideWind.key.year = abcGroup->year;

	if (buffer.Count() > 0)
	{
		TSideWind *prevWind = buffer[buffer.Count() - 1];

		if (sideWind.key.no != prevWind->key.no)
		{
			// switched to new lawsuit
			Commit();
		}
		else if (!strcmp(sideWind.ucn, prevWind->ucn) && sideWind.ucnType == prevWind->ucnType &&
			sideWind.involvement == prevWind->involvement)
		{
			// duplicate, leave first date
			return NULL;
		}
	}

	if (buffer.Count() == 0)
	{
		lawsuit.key = sideWind.key;

		if (lawsuit.Try("F_KIND, F_DATE, F_COMPOSITION, F_SOURCE_KIND"))
		{
			if (abcGroup->allSides)
				abcMode = ABC_MODE_BOTH;
			else
			{
				const TAbcMode *tAbcMode;

				for (tAbcMode = AbcModes; tAbcMode->lawKinds; tAbcMode++)
					if (strchr(tAbcMode->lawKinds, lawsuit.kind))
						break;

				abcMode = tAbcMode->abcMode;
			}
		}
		else
		{
			dprintf("Грешка при четене на %s %ld/%ld", Type->Seek(sideWind.key.type), sideWind.key.no,
				sideWind.key.year);

			return NULL;
		}
	}

	if (abcGroup->firm)
	{
		damn << q;
		MinMax(sideWind.key.no, lawsuit.date);
		return NewAbcWind();
	}

	char category = lawsuit.Category(sideWind.involvement);
	bool process;

	// ??nonsense, always default??
	switch (category)
	{
		case ABC_MODE_LEFT  : process = (category == SIDEWIND_LEFT); break;
		case ABC_MODE_RIGHT : process = (category == SIDEWIND_RIGHT); break;
		case ABC_MODE_COND  :
		{
			if (category == SIDEWIND_RIGHT)	// 1st right
			{
				buffer.Clear();
				abcMode = ABC_MODE_RIGHT;
			}
		}
		default : process = category != SIDEWIND_THIRD;
	}

	if (process)
	{
		// ??never NULL??
		TAbcSideWind *abcWind = NewAbcWind();

		if (abcWind)
			buffer.Add(abcWind);
	}

	return NULL;	// two-pass
}

void TAbcBook::Collect()
{
	TBook::Collect();

	if (buffer.Count())
		Commit();
}

void TAbcBook::EmitRow(TGroup *group)
{
	TAbcSideWind *abcWind = (TAbcSideWind *) group;
	bool newLetter = abcWind->name[0] != lastWind->name[0];

	if (newLetter || abcWind->date.year != lastWind->date.year || abcWind->date.month != lastWind->date.month)
	{
		office.Emit(th1start, tr1start, null_fields, NULL);

		abcWind->minNo = this->group->minNo;
		abcWind->maxNo = this->group->maxNo;

		if (newLetter)
			office.Emit(tr1start, tr2start, body_fields, group);

		office.Emit(tr2start, tr2final, body_fields, group);
		office.Emit(tr2final, th1final, null_fields, NULL);
	}

	office.Emit(tr2final, tr1final, body_fields, group);
	lastWind = abcWind;
}

// ----- entry ---------------------------------------------------------------
static void abc_book(TWindow *parent, TAbcBookGroup *group)
{
	TAbcBook(parent, group).Execute(group->flags & BOOK_NEW_FORMAT ? BOOK_EXEC_HEADER : BOOK_EXEC_NORMAL);
}

#include "AbcBookDialog.h"
#include "AbcBookDialog.cpp"

void abc_book(TWindow *parent)
{
	TAbcBookGroup group;

	if (TAbcBookDialog(parent, &group).Execute() == IDOK)
		abc_book(parent, &group);
}
