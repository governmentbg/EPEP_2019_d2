#include "cac.h"

// ----- TFloatArchiveGroup --------------------------------------------------
enum { ORDER_BY_ATYN = ORDER_BY_USER };
 
class TFloatArchiveGroup : public TFloatGroup
{
public:
	TFloatArchiveGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	long minLinkNo;
	long maxLinkNo;
	long minArchiveNo;
	long maxArchiveNo;
	long minArchiveYear;
	long maxArchiveYear;
	CDate minArchiveDate;
	CDate maxArchiveDate;
	long minKeep;
	long maxKeep;
	// 2013:093 URQ: from floatcombo +related
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	char generalKinds[SIZE_OF_KINDS];
	// 2014:056
	TGArray<TArchiveIndex> indexes;
	long flagsEC;
	long nullKeep;
	long excess;
};

TFloatArchiveGroup::TFloatArchiveGroup()
	: TFloatGroup("T_ARCHIVE A JOIN T_LAWSUIT L ON L.F_NO = A.F_NO AND L.F_YEAR = A.F_YEAR AND L.F_TYPE = A.F_TYPE "
		"LEFT JOIN T_ARCHIVE_INDEX I ON I.F_ARCHIVE_INDEX = A.F_ARCHIVE_INDEX",
		"A.F_NO, A.F_YEAR, A.F_TYPE, A.F_LINK_NO, A.F_ARCHIVE_NO, A.F_ARCHIVE_YEAR, A.F_ARCHIVE_DATE, A.F_KEEP, "
		"A.F_ARCHIVE_INDEX, I.F_FLAGS, A.F_EXCESS")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TLong("F_MIN_LINK_NO", &minLinkNo),
		new TLong("F_MAX_LINK_NO", &maxLinkNo),
		new TLong("F_MIN_ARCHIVE_NO", &minArchiveNo),
		new TLong("F_MAX_ARCHIVE_NO", &maxArchiveNo),
		new TLong("F_MIN_ARCHIVE_YEAR", &minArchiveYear),
		new TLong("F_MAX_ARCHIVE_YEAR", &maxArchiveYear),
		new TDate("F_MIN_ARCHIVE_DATE", &minArchiveDate, false),
		new TDate("F_MAX_ARCHIVE_DATE", &maxArchiveDate, false),
		new TLong("F_MIN_KEEP", &minKeep),
		new TLong("F_MAX_KEEP", &maxKeep),
		new TCharList("F_GENERAL_KINDS", generalKinds, sizeof generalKinds),
		new TRadio("F_BY_ATYN", &order, ORDER_BY_ATYN),
		new TCheck("F_FLAGS_EC", &flagsEC, ARCHIVE_INDEX_EC),
		new TCheck("F_EXCESS", &excess, 1),
		NULL
	);

	flagsEC = -1;
	excess = -1;
	order = ORDER_BY_TYN;
}

void TFloatArchiveGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddChars("A.F_TYPE", types, TYPE_LAWSUITS);
	m.AddRange("A.F_NO", minNo, maxNo);
	m.AddRange("A.F_YEAR", minYear, maxYear);
	m.AddRange("A.F_LINK_NO", minLinkNo, maxLinkNo);
	m.AddRange("A.F_ARCHIVE_NO", minArchiveNo, maxArchiveNo);
	m.AddRange("A.F_ARCHIVE_YEAR", minArchiveYear, maxArchiveYear);
	m.AddRange("A.F_ARCHIVE_DATE", minArchiveDate, maxArchiveDate);

	if (nullKeep)
	{
		m.AddIsNull("A.F_KEEP", true);
		m.AddIsNull("A.F_ARCHIVE_INDEX", true);
	}
	else if (minKeep || maxKeep)
	{
		m.AddRange("(A.F_KEEP", minKeep, maxKeep);
		m.NextOr();
		m.AddRange("I.F_KEEP", minKeep, maxKeep);
		m.cat(")");
	}

	m.AddChars("L.F_SOURCE_KIND", adjust_general_kinds(generalKinds), KIND_ALLDOCS);
	m.AddArray("L.F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "L.F_OLD_SUBJECT");
	m.AddIsNull("A.F_KILL_DATE", true);
	// 2014:064 LPR/IRQ: was a check flag like nullKeep
	m.AddArray("A.F_ARCHIVE_INDEX", indexes);

	if (flagsEC != -1)
		m.AddFlags("I.F_FLAGS", ARCHIVE_INDEX_EC, flagsEC, ARCHIVE_INDEX_ALL);

	if (excess != -1)
		m.AddZLong("F_EXCESS", excess);

	m.AddOrder(order == ORDER_BY_ATYN ?
		"F_ARCHIVE_DATE, F_TYPE, F_YEAR, F_NO, F_EXCESS" : "F_TYPE, F_YEAR, F_NO, F_EXCESS", "A.");
}

// ----- TFloatDestroyGroup --------------------------------------------------
enum { ORDER_BY_KTYN = ORDER_BY_USER };

class TFloatDestroyGroup : public TFloatGroup
{
public:
	TFloatDestroyGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	CDate minKillDate;
	CDate maxKillDate;
	long minKillerNo;
	long maxKillerNo;
	CDate minKillerDate;
	CDate maxKillerDate;
	long minRemainderNo;
	long maxRemainderNo;
	long minRemainderYear;
	long maxRemainderYear;
	TGArray<TArchiveIndex> indexes;
};

TFloatDestroyGroup::TFloatDestroyGroup()
	: TFloatGroup("T_ARCHIVE")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TDate("F_MIN_KILL_DATE", &minKillDate, false),
		new TDate("F_MAX_KILL_DATE", &maxKillDate, false),
		new TLong("F_MIN_KILLER_NO", &minKillerNo),
		new TLong("F_MAX_KILLER_NO", &maxKillerNo),
		new TDate("F_MIN_KILLER_DATE", &minKillerDate, false),
		new TDate("F_MAX_KILLER_DATE", &maxKillerDate, false),
		new TLong("F_MIN_REMAINDER_NO", &minRemainderNo),
		new TLong("F_MAX_REMAINDER_NO", &maxRemainderNo),
		new TLong("F_MIN_REMAINDER_YEAR", &minRemainderYear),
		new TLong("F_MAX_REMAINDER_YEAR", &maxRemainderYear),
		new TRadio("F_BY_KTYN", &order, ORDER_BY_KTYN),
		NULL
	);

	order = ORDER_BY_TYN;
}

void TFloatDestroyGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);
	m.AddChars("F_TYPE", types, TYPE_LAWSUITS);
	m.AddRange("F_KILL_DATE", minKillDate, maxKillDate);
	m.AddRange("F_KILLER_NO", minKillerNo, maxKillerNo);
	m.AddRange("F_KILLER_DATE", minKillerDate, maxKillerDate);
	m.AddRange("F_REMAINDER_NO", minRemainderNo, maxRemainderNo);
	m.AddRange("F_REMAINDER_YEAR", minRemainderYear, maxRemainderYear);
	m.AddIsNull("F_KILL_DATE", false);
	m.AddArray("F_ARCHIVE_INDEX", indexes);
	m.AddOrder(order == ORDER_BY_KTYN ? "F_KILL_DATE, F_TYPE, F_YEAR, F_NO" : "F_TYPE, F_YEAR, F_NO");
}

// ----- query ---------------------------------------------------------------
#define archive OFFICE_GROUP_CAST(TArchive)

static void archive_key_no(mstr &m, TGroup *group) { m.printf("%s%ld", archive->ExcessText(), archive->key.no); }

static void archive_keep(mstr &m, TGroup *group)
{
	if (archive->keep)
		itom(archive->keep, m);
	else if (any(archive->archiveIndex))
	{
		ArchiveIndex->Seek(archive->archiveIndex);
		itom(ArchiveIndex->keep, m);
	}
}

#undef archive

static const offield floatarc_fields[] =
{
	{ "F_TYPE", keygroup_key_type },
	{ "F_NO", archive_key_no },
	{ "F_YEAR", NULL },
	{ "F_LINK_NO", NULL },
	{ "F_ARCHIVE_NO", NULL },
	{ "F_ARCHIVE_YEAR", NULL },
	{ "F_ARCHIVE_DATE", NULL },
	{ "F_KEEP", archive_keep },
	{ "F_KILL_DATE", NULL },
	{ "F_ARCHIVE_INDEX", NULL },
	{ "F_KILLER_NO", NULL },
	{ "F_KILLER_DATE", NULL },
	{ "F_REMAINDER_NO", NULL },
	{ "F_REMAINDER_YEAR", NULL },
	{ NULL, NULL }
};

// ----- TFloatArchiveQery --------------------------------------------------
class TFloatArchiveQuery : public TFloatQuery
{
public:
	TFloatArchiveQuery(TWindow *parent, const char *criteria, const char *name);

protected:
	virtual void Count();
};

TFloatArchiveQuery::TFloatArchiveQuery(TWindow *parent, const char *criteria, const char *name)
	: TFloatQuery(parent, criteria, name)
{
}

void TFloatArchiveQuery::Count()
{
	TQuery q(criteria);
	TArchive archive;

	while (q.Read() && Increment())
	{
		archive << q;
		office.Emit(tr1start, tr1final, floatarc_fields, &archive);
	}
}

//---------------------------------------------------------------------------
static void query_archive(TWindow *parent, const char *criteria)
{
	TFloatArchiveQuery(parent, criteria, "FloatArc.htm").Execute();
}

static void query_kill(TWindow *parent, const char *criteria)
{
	TFloatArchiveQuery(parent, criteria, "FloatDes.htm").Execute();
}

#include "FloatArchiveDialog.h"
#include "FloatArchiveDialog.cpp"

void float_archive(TWindow *parent)
{
	TFloatArchiveGroup group;
	TFloatArchiveDialog(parent, &group).Execute();
}

#include "FloatDestroyDialog.h"
#include "FloatDestroyDialog.cpp"

void float_destroy(TWindow *parent)
{
	TFloatDestroyGroup group;
	TFloatDestroyDialog(parent, &group).Execute();
}
