#include "cac.h"

class TArchiveBookGroup : public TBookGroup
{
public:
	TArchiveBookGroup();

	virtual void FormCriteria(msql &m);
	virtual void Title(mstr &m);

	long minYear;
	long maxYear;
	long flags;
	long sides;
};

TArchiveBookGroup::TArchiveBookGroup()
	: TBookGroup("T_ARCHIVE", "F_TYPE", "F_ARCHIVE_DATE", true)
{
	Add(
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TCheck("F_NEW_FORMAT", &flags, BOOK_NEW_FORMAT),
		new TCheck("F_SIDES", &sides, TRUE),
		NULL
	);

	CDate d = Today;
	CDate min = d;
	CDate max = d;

	if (d.month < 7)
	{
		min.day = min.month = 1;
		max.month = 6;
		max.day = 30;
	}
	else
	{
		min.day = 1;
		min.month = 7;
		max.month = 12;
		max.day = 31;
	}

	minDate = min;
	maxDate = max;

	flags = BOOK_NEW_FORMAT;
	order = ORDER_BY_NULL;
}

void TArchiveBookGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChars("F_TYPE", types, COLLEGE_ALLS);
	m.AddRange("F_YEAR", minYear, maxYear);
	m.AddRange("F_ARCHIVE_DATE", minDate, maxDate);
	m.AddOrder("F_ARCHIVE_YEAR, F_TYPE, F_ARCHIVE_NO");
}

void TArchiveBookGroup::Title(mstr &m)
{
	book_types(m, this);

	m.cat(" \vвнесени в архива ");
	book_dates(m, this);

	if (minYear || maxYear)
	{
		if (minYear == maxYear && minYear && maxYear)
			m.printf(" образувани през %ldг.", minYear);
		else  if (minYear && maxYear)
			m.printf(" образувани от %ldг. до %ldг.", minYear, maxYear);
		else  if (minYear)
			m.printf(" образувани от 01.01.%ldг.", minYear);
		else
			m.printf(" образувани до 31.12.%ldг.", maxYear);
	}
}

static char *load_side_winds(const TArchive *archive, TGArray<TSideWind> &sideWinds)
{
	TExternContainer *lawsuit = TLawsuit::NewGroup(archive->excess);
	TVirtPtr VP(lawsuit);
	char *categories = NULL;

	lawsuit->key = archive->key;

	if (lawsuit->Get())
	{
		msql m;

		if (archive->excess)
			m.Begin("T_EXTERN_SIDE_WIND");
		else
			m.Begin("T_SIDE_WIND", "DISTINCT F_UCN, F_UCN_TYPE, F_INVOLVEMENT");

		m.Add(lawsuit, "F_NO, F_YEAR, F_TYPE");
		load_sidebasicv(sideWinds, str(m));
		categories = load_categories(lawsuit, sideWinds);
	}

	return categories;
}

#define archive OFFICE_GROUP_CAST(TArchive)

static void archive_no(mstr &m, TGroup *group) { itom(archive->archiveNo, m); }

static void archive_key(mstr &m, TGroup *group)
{
	Type->Seek(archive->key.type);
	m.printf("%s %s%ld/%ld", Type->name, archive->ExcessText(), flag_key_no(&archive->key), archive->key.year);
}

static void archive_archive_date(mstr &m, TGroup *group) { archive->archiveDate.Print(m); }
static void archive_link_no(mstr &m, TGroup *group) { itom(archive->linkNo, m); }

static void archive_killer(mstr &m, TGroup *group)
{
	if (archive->killerNo)
	{
		m.printf("Унищожено на %D\v съгласно протокол\v № %d/%D", &archive->killDate, archive->killerNo,
			&archive->killerDate);
	}
}

static void archive_remainder(mstr &m, TGroup *group) { m.cpy(archive->remainder); }

static void archive_remainder_no_year(mstr &m, TGroup *group)
{
	if (archive->remainderNo)
		m.printf("Том № %d/%dг.", archive->remainderNo, archive->remainderYear);
}

static void archive_sides(mstr &m, TGroup *group)
{
	mstr defendants;
	TGArray<TSideWind> sideWinds;
	char *const categories = load_side_winds(archive, sideWinds);
	TCharPtr CP(categories);

	if (categories)
	{
		full_side_list(defendants, &sideWinds, categories, SIDEWIND_RIGHT, false);

		if (archive->key.type != TYPE_PUNISHMENT_LAW)
			full_side_list(m, &sideWinds, categories, SIDEWIND_LEFT, false);

		if (any(m) && any(defendants))
			m.printf("\vсрещу\v%s", str(defendants));
		else if (any(defendants))
			m.cpy(defendants);
	}
}

static void archive_remark(mstr &m, TGroup *group) { m.cpy(archive->archiveRemark); }

#undef archive

static const offield archive_fields[]=
{
	{ "F_NO", archive_no },
	{ "F_KEY", archive_key },
	{ "F_ARCHIVE_DATE", archive_archive_date },
	{ "F_LINK_NO", archive_link_no },
	{ "F_KILLER", archive_killer },
	{ "F_REMAINDER", archive_remainder },
	{ "F_REMAINDER_NO_YEAR", archive_remainder_no_year },
	{ "F_SIDES", archive_sides },
	{ "F_REMARK", archive_remark },
	{ "F_ARCHIVE_INDEX", NULL },
	{ NULL, NULL }
};

#include "ArchiveBookDialog.h"
#include "ArchiveBookDialog.cpp"

class TArchiveBook : public TBook
{
public:
	TArchiveBook(TWindow *parent, TBookGroup *group, const char *fileName);

protected:
	virtual TGroup *Process(TQuery &q);
};

TArchiveBook::TArchiveBook(TWindow *parent, TBookGroup *group, const char *fileName)
	: TBook(parent, group, fileName, archive_fields, ::book_fields, BOOK_FIXED_DATES)
{
}

TGroup *TArchiveBook::Process(TQuery &q)
{
	TArchive *archive = new TArchive;
	*archive << q;
	return archive;
}

void archive_book(TWindow *parent)
{
	TArchiveBookGroup group;

	if (TArchiveBookDialog(parent, &group).Execute() == IDOK)
	{
		mstr m("ArcBook");

		if (group.sides)
			m.cat("S");

		if ((group.flags & BOOK_NEW_FORMAT) == 0)
			m.cat("_");

		m.cat(".htm");

		TArchiveBook(parent, &group, str(m)).Execute(BOOK_EXEC_HEADER);
	}
}
