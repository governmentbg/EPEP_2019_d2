#include "cac.h"

// ----- TBookGroup ----------------------------------------------------------
TBookGroup::TBookGroup(const char *name, const char *tTypeName, const char *tDateName, bool tCollege, bool tAliasFace,
	const char *tNoName, const char *tYearName)
	: TFloatGroup(name), typeName(tTypeName), dateName(tDateName), college(tCollege), aliasFace(tAliasFace),
		noName(tNoName ? tNoName : "F_NO"), yearName(tYearName ? tYearName : "F_YEAR")
{
	Add(
		new TRadio("F_BY_NO_YEAR", &by, BOOK_BY_NO_YEAR),
		new TRadio("F_BY_PERIOD", &by, BOOK_BY_PERIOD),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_YEAR", &year),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TLong("F_AUTO_SERIAL_NO", &autoSerialNo),
		NULL
	);

	if (aliasFace)
		Add(new TCharAlias("F_TYPE", types));
	else
		Add(new TCharList("F_TYPES", types, sizeof types));

	if (college)
	{
		strcpy(types, Default->colleges);
		xTypes = COLLEGE_ALLS;
	}
	else
	{
		strcpy(types, Default->xColleges);
		xTypes = COLLEGE_XALLS;
	}

	by = BOOK_BY_NO_YEAR;
	minNo = 1;
	year = Today.year;
	minDate = Today;
	maxDate = Today;
}

void TBookGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	if (aliasFace)
		m.AddChar(typeName, *types);
	else
		m.AddChars(typeName, types, xTypes);

	if (by == BOOK_BY_NO_YEAR)
	{
		m.AddRange(noName, minNo, maxNo);
		m.AddLong(yearName, year);
		order = ORDER_BY_TYN;
	}
	else
	{
		m.AddRange(dateName, minDate, maxDate);
		order = ORDER_BY_DTYN;
	}

	// EndCriteria() in derived classes
}

void TBookGroup::Title(mstr &m)
{
	if (by == BOOK_BY_NO_YEAR)
	{
		book_types(m, this);
		m.cat("\v");
		book_range(m, this);
	}
	else
		book_dates(m, this);
}

#define book OFFICE_GROUP_CAST(TBookGroup)

void book_types(mstr &m, TGroup *group)
{
	print_types_list(m, book->types, book->college);
}

void book_range(mstr &m, TGroup *group)
{
	if (book->minNo && book->maxNo)
	{
		if (book->minNo == book->maxNo)
		{
			if (book->minDate.year == book->maxDate.year)
				m.printf("№ %ld за %ldг.", book->minNo, book->minDate.year);
			else
				m.printf("№ %ld от %ldг. до %ldг.", book->minNo, book->minDate.year, book->maxDate.year);
		}
		else
		{
			if (book->minDate.year == book->maxDate.year)
				m.printf("от № %ld до № %ld за %ldг.", book->minNo, book->maxNo, book->minDate.year);
			else
				m.printf("от № %ld за %ldг. до № %ld за %ldг.", book->minNo, book->minDate.year, book->maxNo, book->maxDate.year);
		}
	}
	else if (book->minNo)
		m.printf("№ %ld за %ldг.", book->minNo, book->minDate.year);
}

void book_dates(mstr &m, TGroup *group)
{
	if (book->maxDate.Empty())
		book->maxDate = Today;

	if (!book->minDate.Empty() && !book->maxDate.Empty())
	{
		if (book->minDate != book->maxDate)
			m.printf("от %Dг. до %Dг.", &book->minDate, &book->maxDate);
		else
			m.printf("за %Dг.", &book->minDate);
	}
	else
		m.printf("до %Dг.", &book->maxDate);
}

static void book_year(mstr &m, TGroup *group)
{
	if (book->by == BOOK_BY_NO_YEAR ? book->year : book->minDate.year)
		m.printf("%ldг.", book->by == BOOK_BY_NO_YEAR ? book->year : book->minDate.year);
}

static void book_title(mstr &m, TGroup *group) { book->Title(m); }

static void book_count(mstr &m, TGroup *group)
{
	if (book->count == 0)
		m.printf("Няма документи отговарящи на зададените критерии.");
	else if (book->count == 1)
		m.printf("Един документ отговаря на зададените критерии.");
	else
		m.printf("%ld документа отговарят на зададените критерии", book->count);
}

#undef book

const offield book_fields[] =
{
	{ "F_TYPES",	book_types },
	{ "F_RANGE",	book_range },
	{ "F_DATES",	book_dates },
	{ "F_YEAR",		book_year },
	{ "F_TITLE",	book_title },
	{ "F_COURT_PRESIDENT",	generic_court_president },
	{ "F_JUDGE",	generic_judge },
	{ "F_COUNT",	book_count },
	{ NULL,		NULL }
};

// ----- TSessionBookGroup ---------------------------------------------------
TSessionBookGroup::TSessionBookGroup()
	: TBookGroup("T_SESSION", "F_TYPE", "F_DATE", true, true), total(0)
{
}

void TSessionBookGroup::CountResults(TSession *session, CDate &lawFinished)
{
	TMResult *result = new TMResult;

	result->sesResult = session->result;
	total++;

	if (!lawFinished.Empty())
	{
		if (session->date > lawFinished)
			result->dateAfterFinish = true;

		if (session->returned > lawFinished)
			result->returnedAfterFinish = true;
	}

	if (strchr(RESULT_ENTERABLES, session->result))
	{
		TGArray<TDecision> decisions;

		if (try2decide(session, &decisions, NULL, "F_RESULT") && decisions.Count() == 1)
			result->decResult = decisions[0]->result;
	}

	int index = results.Find(result);

	if (index == INT_MAX)
		results.Add(result);
	else
	{
		results[index]->count++;
		delete result;
	}
}

// ----- TSessionBookGroup::TMResult -----------------------------------------
TSessionBookGroup::TMResult::TMResult()
	: sesResult('\0'), decResult('\0'), dateAfterFinish(false), returnedAfterFinish(false), count(1)
{
}

bool TSessionBookGroup::TMResult::operator<=(const TMResult &that) const
{
	return sesResult != that.sesResult ? sesResult < that.sesResult :
		decResult != that.decResult ? decResult < that.decResult :
		dateAfterFinish != that.dateAfterFinish ? dateAfterFinish < that.dateAfterFinish :
		returnedAfterFinish <= that.returnedAfterFinish;
}

#define result OFFICE_GROUP_CAST(TSessionBookGroup::TMResult)

static void closes_session_book_ses_result(mstr &m, TGroup *group)
{
	if (result->sesResult)
		m.cpy(Result->Seek(result->sesResult));
}

static void closes_session_book_dec_result(mstr &m, TGroup *group)
{
	if (result->decResult)
		m.cpy(Result->Seek(result->decResult));
}

static void closes_session_book_flag_1(mstr &m, TGroup *group)
{
	if (result->dateAfterFinish)
		m.cpy("*");
}

static void closes_session_book_flag_2(mstr &m, TGroup *group)
{
	if (result->returnedAfterFinish)
		m.cpy("*");
}

static void closes_session_book_count(mstr &m, TGroup *group) { m.printf("%d", result->count); }

#undef result

static const offield session_results_fields[] =
{
	{ "F_SES_RESULT",	closes_session_book_ses_result },
	{ "F_DEC_RESULT",	closes_session_book_dec_result },
	{ "F_FLAG_1",	closes_session_book_flag_1 },
	{ "F_FLAG_2",	closes_session_book_flag_2 },
	{ "F_COUNT",	closes_session_book_count },
	{ "F_TOTAL",	closes_session_book_count },
	{ NULL, NULL }
};

// ----- TBaseBook -----------------------------------------------------------
TBaseBook::TBaseBook(TWindow *parent, TBookGroup *tGroup, const char *tFileName, const offield *bodyFields, const offield *bookFields, long tFlags)
	: TOutput(parent, "книгата", WAIT), fileName(tFileName), group(tGroup), body_fields(bodyFields), book_fields(bookFields), flags(tFlags)
{
	office.InitAutoGen(group->autoSerialNo + !group->autoSerialNo);
}

void TBaseBook::EmitBody()
{
	office.Emit(NULL, NULL, body_fields, group);
}

bool TBaseBook::Execute(int bookExec)
{
	if (bookExec == BOOK_EXEC_HEADER)
	{
	#if RANGECHECK
		if (basename(fileName) != fileName)
			return error("book execute: %s is not a basename.", fileName);
		if (!html_name(fileName))
			return error("book execute: %s is not a html name.", fileName);
	#endif  // RANGECHECK

		if (!office.Open(fileName, OFFICE_COPY_HEADER))
			return false;

		office.Emit(NULL, NULL, book_fields, group);
		office.Close();
	}

	if (bookExec)
	{
		if (!office.Open(fileName, bookExec == BOOK_EXEC_HEADER ? OFFICE_SKIP_HEADER : OFFICE_COPY_ALL,
			bookExec == BOOK_EXEC_HEADER ? "" : NULL))
		{
			return false;
		}

		Markers();
		EmitBody();
	#if SPEEDTESTING
		#pragma message speed testing only
		message("test version: %lu d-seconds\n", (GetTickCount() - speedTicks) / 100);
	#endif  // SPEEDTESTING
		office.Show(wait);
		Debug();
	}

	return true;
}

void TBaseBook::MinMax(long no)
{
	if (!group->minNo)
		group->minNo = group->maxNo = no;
	else
	{
		if (no < group->minNo)
			group->minNo = no;
		if (no > group->maxNo)
			group->maxNo = no;
	}
}

void TBaseBook::Markers()
{
}

void TBaseBook::MinMax(long no, const CDate &date)
{
	if (!group->minNo)
	{
		group->minNo = group->maxNo = no;
		group->minDate = group->maxDate = date;
	}
	else
	{
		if (date.year < group->minDate.year || date.year == group->minDate.year && no < group->minNo)
			group->minNo = no;
		if (date.year > group->maxDate.year || date.year == group->maxDate.year && no > group->maxNo)
			group->maxNo = no;
		if (date < group->minDate)
			group->minDate = date;
		if (date > group->maxDate)
			group->maxDate = date;
	}
}

void TBaseBook::Increment()
{
	wait->TickCount(++count, "обработени записа");
}

// ----- TBook ---------------------------------------------------------------
TBook::TBook(TWindow *parent, TBookGroup *group, const char *fileName, const offield *bodyFields, const offield *bookFields, long flags,
	TGroupArray *tArray)
	: TBaseBook(parent, group, fileName, bodyFields, bookFields, flags), array(tArray ? tArray : new TGroupArray)
{
}

TBook::~TBook()
{
	delete array;
}

bool TBook::Execute(int bookExec)
{
#if SPEEDTESTING
	#pragma message speed testing only
	speedTicks = GetTickCount();
#endif  // SPEEDTESTING
	Collect();
	return TBaseBook::Execute(bookExec);
}

void TBook::EmitBody()
{
	office.Emit(NULL, tr1start, book_fields, group);

	int index;

	for (index = 0; index < array->Count(); index++)
	{
		EmitRow((*array)[index]);
		wait->TickCount(index, "отпечатани записа");
	}

	group->count = index;
	office.Emit(tr1final, NULL, book_fields, group);
}

void TBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr1final = office.Find("tr1 final");
}

void TBook::EmitRow(TGroup *group)
{
	office.Emit(tr1start, tr1final, body_fields, group);
}

void TBook::Collect(const char *criteria)
{
	TQuery q(criteria);

	while (q.Read())
	{
		TGroup *tGroup = Process(q);

		if (tGroup)
			array->Add(tGroup);

		Increment();
	}
}

void TBook::Collect()
{
	msql m;

	group->FormCriteria(m);
	group->minNo = group->maxNo = 0;

	if (!(flags & BOOK_FIXED_DATES))
	{
		group->minDate.Clear();
		group->maxDate.Clear();
	}

	Collect(str(m));
}

// ----- TSessionBookResults -------------------------------------------------
TSessionBookResults::TSessionBookResults(TWindow *parent, TSessionBookGroup *group, const char *fileName,
	const offield *bookFields)
	: TBaseBook(parent, group, fileName, session_results_fields, bookFields)
{
}

void TSessionBookResults::Markers()
{
	tb1start = office.Find("tb1 start");
	tr1start = office.Find("tr1 start");
	tr1break = office.Find("tr1 break");
	tr1final = office.Find("tr1 final");
	tb1final = office.Find("tb1 final");
}

void TSessionBookResults::EmitContent(TOffice &office)
{
	TSessionBookGroup *tGroup = (TSessionBookGroup *) group;

	office.Emit(tb1start, tr1start, book_fields, group);

	for (int i = 0; i < tGroup->results.Count(); i++)
		office.Emit(tr1start, tr1break, body_fields, tGroup->results[i]);

	// 2011:157 FIX: check count
	if (tGroup->results.Count())
	{
		tGroup->results[0]->count = tGroup->total;
		office.Emit(tr1break, tr1final, body_fields, tGroup->results[0]);
	}

	office.Emit(tr1final, tb1final, null_fields, NULL);
}

void TSessionBookResults::EmitPartial(TOffice &tOffice)
{
	if (office.Open(fileName, OFFICE_COPY_NULL))
	{
		Markers();
		EmitContent(tOffice);
	}
}

void TSessionBookResults::EmitBody()
{
	office.Emit(NULL, tb1start, null_fields, NULL);
	EmitContent(office);
	office.Emit(tb1final, NULL, null_fields, NULL);
}
