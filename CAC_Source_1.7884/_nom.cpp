#include "cac.h"

TSubjectRange :: TSubjectRange () : TGroup ("T_SUBJECT_RANGE", NULL)
{
	Add(
		new TLong ("F_SUBJECT", & subject),
		new TChar("F_TYPE", & type),
		new TLong ("F_LOW_SUBJECT_RANGE", & lowSubjectRange),
		new TLong ("F_HIGH_SUBJECT_RANGE", & highSubjectRange),
		new TString ("F_NAME", name, sizeof (name)),
		new TString ("F_ATTRIB", attrib, sizeof (attrib)),
		new TString("F_LETTERS", letters, sizeof letters),
		NULL
	);

	descs = Descs + TABLE_SUBJECT_RANGE;
}  // TSubjectRange :: TSubjectRange

void TSubjectRange::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_SUBJECT, F_LOW_SUBJECT_RANGE, F_HIGH_SUBJECT_RANGE, F_TYPE");
	// 2016:098 LPR: key changed
	fatal("%s: exact criteria not supported for %s", str(m), Name);
}

void TSubjectRange::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_TYPE");
}

bool TSubjectRange::InsideRange(long lowSubjectRange, long highSubjectRange, long tSubject)
{
	return tSubject >= lowSubjectRange && tSubject <= highSubjectRange;
//	return (tSubject >= lowSubjectRange && tSubject <= highSubjectRange) ||
//		(subject < lowSubjectRange && subject > highSubjectRange && tSubject == subject);
}

bool TSubjectRange::InsideRange(long tSubject) const
{
	return InsideRange(lowSubjectRange, highSubjectRange, tSubject);
}
