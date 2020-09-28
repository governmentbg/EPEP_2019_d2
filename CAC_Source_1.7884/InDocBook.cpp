#include "cac.h"

enum
{
	BOOK_DOC_REQUEST	= 1,
	BOOK_DOC_SURROUND	= 2,
	BOOK_DOC_INREG	= 3,
};

enum
{
	BOOK_CONTENTS_BY_JUDGE		= 4,
	BOOK_CONTENTS_BY_DIRECTION	= 5
};

class TInDocBookGroup : public TBookGroup
{
public:
	TInDocBookGroup();

	virtual void FormCriteria(msql &m);
	virtual void Title(mstr &m);

	long inRequest;
	long inInReg;
	long inSurround;
	long contents;
	bool book251;
	long printRightSides;
};

TInDocBookGroup::TInDocBookGroup()
	: TBookGroup("T_BOOK", "F_COLLEGE", "F_DATE", false), printRightSides(false)
{
	Add(
		new TCheck("F_INSTANCE_1ST", &flags, BOOK_1ST_INST),
		new TCheck("F_INSTANCE_2ND", &flags, BOOK_2ND_INST),
		new TCheck("F_REQUEST", &inRequest, BOOK_DOC_REQUEST),
		new TCheck("F_SURROUND", &inSurround, BOOK_DOC_SURROUND),
		new TCheck("F_INREG", &inInReg, BOOK_DOC_INREG),
		new TCheck("F_PRINT_RIGHT", &printRightSides, TRUE),
		new TRadio("F_DIRECTION", &contents, BOOK_CONTENTS_BY_DIRECTION),
		new TRadio("F_JUDGE", &contents, BOOK_CONTENTS_BY_JUDGE),
		NULL
	);

	if (by == BOOK_BY_PERIOD)
		minDate = Today;

	inRequest = BOOK_DOC_REQUEST;
	inSurround = BOOK_DOC_SURROUND;
	inInReg = BOOK_DOC_INREG;
	printRightSides = FALSE;
	contents =  BOOK_CONTENTS_BY_DIRECTION;
}

void TInDocBookGroup::FormCriteria(msql &m)
{
#if COURT_TYPE == COURT_ADMIN
	if (book251)
	{
		BeginCriteria(m);
		m.AddRange("F_DATE", minDate, maxDate);
		m.AddChars("F_KIND", KIND_BOOK_251_INDOCS);
		m.AddOrder("F_DATE");	// == EndCriteria()
	}
	else
#endif  // ADMIN
	{
		// --- InDocBook ---
		TBookGroup::FormCriteria(m);
		m.AddChars("F_COLLEGE", types, xTypes);
		EndCriteria(m);
	}
}

void TInDocBookGroup::Title(mstr &m)
{
	book_types(m, this);
}

class TAnyInEvent : public TSurroundment
{
public:
	TAnyInEvent();
	char judge[SIZE_OF_UCN];
	char resolution;
};

TAnyInEvent::TAnyInEvent()
{
	Add(
		new TString("F_JUDGE", judge, sizeof judge),
		new TChar("F_RESOLUTION", &resolution),
		NULL
	);
}

// ----- TInDocInReg ---------------------------------------------------------
class TInDocInReg : public TInReg
{
public:
	TInDocInReg();
	virtual ~TInDocInReg();

	bool operator<=(const TInDocInReg &that) const;

	long serialNo;
	char lawKind;	// 2017:088

	bool printRightSides;
	char *categories;
	TGArray<TAnyInEvent> surArray;
	TGArray<TSideWind> sideWinds;
	TProcess *book;
};

TInDocInReg::TInDocInReg()
	: printRightSides(false), categories(NULL)
{
	Add(
		new TLong("F_SERIAL_NO", &serialNo),
		new TCharAlias("F_LAW_KIND", &lawKind),
		NULL
	);
}

TInDocInReg::~TInDocInReg()
{
	sideWinds.Flush();
	delete[] categories;
}

bool TInDocInReg::operator<=(const TInDocInReg &that) const
{
	return key.year != that.key.year ? key.year < that.key.year : key.no <= that.key.no;
}

#define inDocInReg OFFICE_GROUP_CAST(TInDocInReg)

static void	inDoc_book_slash(mstr &m, TGroup *group)
{
	if (inDocInReg->key.no && inDocInReg->key.year)
		m.cpy("/");
}

static void inDoc_book_time(mstr &m, TGroup *group) { inDocInReg->time.Print(m); }

static void inDoc_book_description(mstr &m, TGroup *group)
{
	if (inDocInReg->key.type == TYPE_SURROUND || inDocInReg->key.type == TYPE_INREG)
	{
		if (strchr(KIND_TEXTUAL_SURROUNDS, inDocInReg->kind) || KIND_OTHER_IN_DOCUMENT == inDocInReg->kind)
		{
			m.printf("%s ", inDocInReg->text);
		}
		else
			m.printf("%s %s ", Kind->Seek(inDocInReg->kind), inDocInReg->text);

		if (inDocInReg->lawsuit.type)
		{
			TRCDKeyContainer *container = type2RCDKeyContainer(inDocInReg->lawsuit.type);
			container->key = inDocInReg->lawsuit;
			container->Try("F_KIND");

			if (inDocInReg->key.type == TYPE_SURROUND)
			{
				TSurround *surround = new TSurround;
				*surround << inDocInReg;
				surround_act(m, surround);
				delete surround;
			}

			m.printf(" по %s № %d/%dг.", Kind->Seek(container->kind), inDocInReg->lawsuit.no, inDocInReg->lawsuit.year);

			if (inDocInReg->key.type == TYPE_INREG)
			{
				if (inDocInReg->decisionKind && inDocInReg->sessionKind)
				{
					TKind tKind;
					m.printf(" срещу  %s от %s ", tKind.Seek(inDocInReg->decisionKind), Kind->Seek(inDocInReg->sessionKind));

					if (!inDocInReg->decisionDate.Empty())
						m.printf("от %Dг.", &inDocInReg->decisionDate);
				}
			}
			delete container;
		}
	}
	else
	{
		m.printf("%s %s\v", Kind->Seek(inDocInReg->kind), inDocInReg->text);

		TGArray<TConnect> connects;
		load_connects(inDocInReg, &connects);

		for (int i = 0; i < connects.Count(); i++)
		{
			connects[i]->composition = 0;
			connects[i]->Print(m);
			m.cat("\v");
		}
	}
}

static void inDoc_book_defendants(mstr &m, TGroup *group)
{
	if (inDocInReg->printRightSides)
	{
		for (int i = 0; i < inDocInReg->sideWinds.Count(); i++)
		{
			TSideWind *sideWind = inDocInReg->sideWinds[i];

			if ((inDocInReg->categories ? inDocInReg->categories[i] : sideWind->involvement) == SIDEWIND_RIGHT)
			{
				m.sep("\v");
				m.printf("%s ", Involvement->Seek(sideWind->involvement));
				add_side_name(m, inDocInReg->sideWinds[i], "");
			}
		}
	}
}

static void inDoc_book_sender(mstr &m, TGroup *group)
{
	TSideWind sideWind;

	for (int i = 0; i < inDocInReg->sideWinds.Count(); i++)
	{
		if (inDocInReg->key.type == TYPE_REQUEST ? inDocInReg->categories[i] == SIDEWIND_LEFT : inDocInReg->sideWinds[i]->Appealer())
		{
			sideWind << inDocInReg->sideWinds[i];
			sideWind << inDocInReg;
			print_links_names(m, &sideWind);
		}
	}
}

static void	inDoc_book_direction(mstr &m, TGroup *group)
{
	if ((inDocInReg->key.type == TYPE_SURROUND || inDocInReg->key.type == TYPE_INREG) && !any(inDocInReg->direction))
	{
		if (inDocInReg->lawsuit.no && inDocInReg->lawsuit.no && strchr(TYPE_LAWSUITS, inDocInReg->lawsuit.type))
		{
			TLawsuit lawsuit;
			lawsuit.key = inDocInReg->lawsuit;

			if (lawsuit.Try("F_JUDGE"))
			{
				m.cat("На доклад на съдия-докл. ");
				print_user(m, lawsuit.judge);
			}
			else if (inDocInReg->key.type == TYPE_SURROUND)
			{
				inDocInReg->book->dprintf("Липсва съдия по %s %ld/%ldг.\n", Type->Seek(inDocInReg->lawsuit.type),
					inDocInReg->lawsuit.no, inDocInReg->lawsuit.year);
			}
		}
	}
	else if (any(inDocInReg->direction))
		m.cpy(inDocInReg->direction);
	else
		m.cpy("На доклад за разпределение");
}

static void inDoc_book_subcount(mstr &m, TGroup *group) { subcount(m, inDocInReg->surArray.Count()); }

#undef inDocInReg

#define anyInEvent OFFICE_GROUP_CAST(TAnyInEvent)

static void inDoc_book_judge(mstr &m, TGroup *group)
{
	if (atof(anyInEvent->judge))
		print_user(m, anyInEvent->judge);
}

static void inDoc_book_event(mstr &m, TGroup *group)
{
	if (anyInEvent->key.type == TYPE_REQUEST || anyInEvent->key.type == TYPE_INREG)
	{
		if (anyInEvent->key.type == TYPE_REQUEST)
			m.cpy(Resolution->Seek(anyInEvent->resolution));
		else
			m.cpy(Result->Seek(anyInEvent->result));

		if (PRINT_JUDGEMENTS && anyInEvent->eventNo)
			m.printf(" № %d/", anyInEvent->eventNo);
		else
			m.cat(" от ");

		m.printf("%Dг. ", &anyInEvent->date);
		m.cat(anyInEvent->text);
		m.cat("\v");
	}
	else if (anyInEvent->key.type == TYPE_SURROUND)
	{
		// 2016:112 FIX: sessionDate.year -> sessionDate
		m.printf("%s от %Dг. ", Kind->Seek(anyInEvent->sessionKind), &anyInEvent->sessionDate);
		m.cat(anyInEvent->text);
	}
}

/*static void inDoc_book_composition(mstr &m, TGroup *group)
{
	if (PRINT_COMPOSITIONS)
	{
		bool found = false;
		TLawsuit lawsuit;

		if (anyInEvent->key.type == TYPE_REQUEST)
			found = lawsuit.TryBy(anyInEvent->key, "F_COMPOSITION");
		else
		{
			TPrimeContainer *const container = (anyInEvent->key.type == TYPE_REQUEST) ?
				(TPrimeContainer *) new TSurround : (TPrimeContainer *) new TInReg;
			TVirtPtr VP(container);

			container->key = anyInEvent->key;

			if (container->Try("F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE"))
			{
				lawsuit.key = container->lawsuit;
				found = lawsuit.Try("F_COMPOSITION");
			}
		}

		if (found)
			m.printf("%s състав", Composition->Seek(lawsuit.composition));
	}
}*/

#undef anyInEvent

static const offield byDirection_fields[] =
{
	{ "F_NO",                keygroup_key_no },
	{ "F_SLASH",             inDoc_book_slash },
	{ "F_YEAR",              keygroup_key_year },
	{ "F_DATE",              container_date },
	{ "F_TIME",              inDoc_book_time },
	{ "F_INDOC_DESCRIPTION", inDoc_book_description },
	{ "F_DEFENDANTS",        inDoc_book_defendants },
	{ "F_SENDER",            inDoc_book_sender },
	{ "F_DIRECTION",         inDoc_book_direction },
	{ "F_SUBCOUNT",          inDoc_book_subcount },
	{NULL, NULL}
};

static const  offield byJudge_fields[]=
{
	{ "F_JUDGE",       inDoc_book_judge },
	/*{ "F_COMPOSITION", inDoc_book_composition },*/
	{ "F_EVENT",       inDoc_book_event },
	{ NULL, NULL }
};



//--------------------------- TInDocBook --------------------------------------
class TInDocBook : public TBook
{
public:
	TInDocBook(TWindow *parent, TBookGroup *group, const char *fileName, const offield *bodyFields, const offield *bookFields,
		const long *tTables, long tContents, bool tBook251 = false);

protected:
	bool book251;
	const long *tables;
	long contents;
	const char *tr1noyear;
	const char *tr1judge;

	void AddEvents(TInDocInReg *inReg, TGArray<TAnyInEvent> *surArray);
	virtual void Markers();
	virtual void EmitRow(TGroup *group);
	virtual TGroup *Process(TQuery &q);
	virtual void Collect();
	bool Matches(TRCDKeyContainer *container, TInDocInReg *inReg);
};

TInDocBook::TInDocBook(TWindow *parent, TBookGroup *group, const char *fileName, const offield *bodyFields, const offield *bookFields,
	const long *tTables, long tContents, bool tBook251)
	: TBook(parent, group, fileName, bodyFields, bookFields, BOOK_OLD_FORMAT, new TGSArray<TInDocInReg>),
		tables(tTables), contents(tContents), book251(tBook251)
{
}

bool TInDocBook::Matches(TRCDKeyContainer *container, TInDocInReg *inReg)
{
	if (strchr(COLLEGE_COMMONS, inReg->college))
		return true;

	if (strchr(TYPE_ORIGINS, container->key.type))
	{
		char lawKind = container_to_law_kind(container);

		if (lawKind && strchr(instance_law_kinds(group->flags), lawKind))
			return true;
	}

	return false;
}

enum { TABLES = 3 };

class TInDocContainer : public TDummyContainer
{
public:
	TInDocContainer() { Add(new TCharAlias("F_LAW_KIND", &lawKind)); }

	char lawKind;
};

void TInDocBook::Collect()
{
	TInDocBookGroup tGroup;
	TInDocContainer container;
	constant tableNames[TABLES] = { "T_REQUEST", "T_SURROUND", "T_INREG" };
	msql m;

	tGroup << group;
	tGroup.book251 = book251;
	group->minNo = group->maxNo = 0;

	for (int i = 0; i < TABLES; i++)
	{
		if (i + 1 == tables[i])
		{
			tGroup.SetName(tableNames[i]);
			tGroup.FormCriteria(m);

			TQuery q(str(m));
			while (q.Read())
			{
				TInDocInReg *inReg = new TInDocInReg;
				*inReg << q;

				if (contents == BOOK_CONTENTS_BY_JUDGE)
					AddEvents(inReg, &inReg->surArray);

				if (inReg->key.type == TYPE_INREG && tables[0])
				{
					int index = ((TGSArray<TInDocInReg> *) array)->Find(inReg);

					if (index != INT_MAX)
						array->Remove(index);
				}

				container.key = (i + 1 == tables[0]) ? inReg->key : inReg->lawsuit;
				container.kind = inReg->kind;
				container.lawKind = inReg->lawKind;

				if (container.key.type == TYPE_INREG)
				{
					TInReg inReg;
					inReg.key = container.key;

					if (inReg.Try("F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE"))
						container.key = inReg.lawsuit;
				}

				if (!(container.key.no && container.key.year))
					inReg->college = *COLLEGE_COMMONS;

				if (book251 || Matches(&container, inReg))
				{
					inReg->book = this;

					if (inReg->key.type == TYPE_REQUEST)
						inReg->printRightSides = tGroup.printRightSides;

					inReg->categories = load_sidebasic(inReg, inReg->sideWinds);
					array->Add(inReg);
					Increment();
				}
				else
					delete inReg;

				Increment();
			}
		}
	}

	count = 0;
}

void TInDocBook::Markers()
{
	if (contents == BOOK_CONTENTS_BY_DIRECTION)
		TBook::Markers();
	else
	{
		tr1start = office.Find("tr1 start");
		tr1noyear = office.Find("tr1 noyear");
		tr1judge = office.Find("tr1 judge");
		tr1final = office.Find("tr1 final");
	}
}

void TInDocBook::EmitRow(TGroup *group)
{
	if (contents == BOOK_CONTENTS_BY_DIRECTION)
		TBook::EmitRow(group);
	else
	{
		TInDocInReg *inDocField = (TInDocInReg *) group;

		if (inDocField->surArray.Count())
		{
			for (int i = 0; i < inDocField->surArray.Count(); i++)
			{
				office.Emit(tr1start, i ? tr1noyear : tr1judge, byDirection_fields, inDocField);
				office.Emit(tr1judge, tr1final, byJudge_fields, inDocField->surArray[i]);
			}
		}
		else
			TBook::EmitRow(group);
	}

	wait->TickCount(++count, "отпечатани записа");
}

void TInDocBook::AddEvents(TInDocInReg *inReg, TGArray<TAnyInEvent> *surArray)
{
	msql m;

	if (inReg->key.type == TYPE_REQUEST)
	{
		TRequestResolution resolution;

		resolution << inReg;
		resolution.FloatCriteria(m);
		m.AddOrder("F_DATE");

		TQuery q(str(m));
		while (q.Read())
		{
			TAnyInEvent *anyInEvent = new TAnyInEvent;
			*anyInEvent << q;

			if (RESOLUTION_LAWED == anyInEvent->resolution)
			{
				TLawsuit lawsuit;

				if (lawsuit.TryBy(inReg, "F_JUDGE"))
					strcpy(anyInEvent->judge, lawsuit.judge);
			}

			surArray->Add(anyInEvent);
		}
	}
	else if (inReg->key.type == TYPE_SURROUND)
	{
		TSurroundment surroundment;

		surroundment << inReg;
		surroundment.FloatCriteria(m);

		TQuery q(str(m));
		while (q.Read())
		{
			TAnyInEvent *anyInEvent = new TAnyInEvent;
			TSession session;

			*anyInEvent << q;
			session.key = anyInEvent->lawsuit;
			session.kind = anyInEvent->sessionKind;
			session.date = anyInEvent->sessionDate;

			if (session.Try("F_JUDGE, F_TEXT"))
			{
				strcpy(anyInEvent->judge, session.judge);
				if (!any(anyInEvent->text))
					anyInEvent->text.cpy(session.text);
			}
			else
			{
				dprintf("Липсва %s от %Dг. по %s %d/%dг.\n", Kind->Seek(anyInEvent->sessionKind),
					&anyInEvent->sessionDate, Type->Seek(anyInEvent->lawsuit.type), anyInEvent->lawsuit.no,
					anyInEvent->lawsuit.year);
			}

			surArray->Add(anyInEvent);
		}
	}
	else
	{
		TInRegEvent inRegEvent;

		inRegEvent << inReg;
		inRegEvent.FloatCriteria(m);

		TQuery q(str(m));
		while (q.Read())
		{
			TAnyInEvent *anyInEvent = new TAnyInEvent;
			*anyInEvent << q;

			if (strchr(RESULT_INREG_RESOLUTIONS, anyInEvent->result))
				surArray->Add(anyInEvent);
			else
				delete anyInEvent;
		}
	}
}

TGroup *TInDocBook::Process(TQuery &)
{
	return NULL;
}

// ----- functions -----------------------------------------------------------
static void indoc_book(TWindow *parent, TInDocBookGroup *group)
{
	long tables[TABLES] = { group->inRequest, group->inSurround, group->inInReg };

	TInDocBook(parent, group, group->contents == BOOK_CONTENTS_BY_DIRECTION ? "InDocBk1.htm" : "InDocBk2.htm",
		byDirection_fields, book_fields, tables, group->contents).Execute();
}

#include "InDocBookDialog.h"
#include "InDocBookDialog.cpp"

void indoc_book(TWindow *parent)
{
	TInDocBookGroup group;

	if (TInDocBookDialog(parent, &group).Execute() == IDOK)
		indoc_book(parent, &group);
}

// ------------------------------ Book_251 ------------------------------
#if COURT_TYPE == COURT_ADMIN

#define inDocBk OFFICE_GROUP_CAST(TInDocBookGroup)

static void book251_min_date(mstr &m, TGroup *group) { inDocBk->minDate.Print(m); }
static void book251_max_date(mstr &m, TGroup *group) { inDocBk->maxDate.Print(m); }

#undef inDocBk

static const offield book251_title_fields[]=
{
	{ "F_MIN_DATE", book251_min_date },
	{ "F_MAX_DATE", book251_max_date },
	{ NULL, NULL }
};

// ----- functions -----------------------------------------------------------
static void book_251(TWindow *parent, TInDocBookGroup *group)
{
	static const long tables[TABLES] = { BOOK_DOC_REQUEST, BOOK_DOC_REQUEST, BOOK_DOC_REQUEST };

	TInDocBook(parent, group, "book251_.htm", byDirection_fields, book251_title_fields, tables,
		BOOK_CONTENTS_BY_DIRECTION, true).Execute(BOOK_EXEC_HEADER);
}

#include "Book251Dialog.h"
#include "Book251Dialog.cpp"

void book_251(TWindow *parent)
{
	TInDocBookGroup group;

	if (TBook251Dialog(parent, &group).Execute() == IDOK)
		book_251(parent, &group);
}
#endif  // ADMIN
