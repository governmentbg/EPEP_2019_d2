#include "cac.h"

TFloatQuery::TFloatQuery(TWindow *parent, const char *tCriteria, const char *tName, const char *tTitle)
	: TProcess(parent, "справката", WAIT), criteria(tCriteria), name(tName), title(tTitle), copy(OFFICE_COPY_ALL)
{
	SetNames("Няма записи", "Един запис", "записа");
	maximum = FLOAT_QUERY_MAX;
}

bool TFloatQuery::ExecuteLL()
{
	if (office.Open(name, copy, copy == OFFICE_SKIP_HEADER ? "" : NULL))
	{
		TWindowHideParent hide(parent);

		GetMarkers();
		office.Emit(NULL, tr1start, float_fields, this);
		Count();
		office.Emit(tr1final, NULL, float_fields, this);
		return true;
	}

	return false;
}

void TFloatQuery::Execute()
{
	if (ExecuteLL())
		Show();
}

void TFloatQuery::GetMarkers()
{
	tr1start = office.Find("tr1 start");
	tr1final = office.Find("tr1 final");
}

bool TFloatQuery::Increment()
{
	wait->TickCount(++count, "обработени записа");
	return count <= maximum;
}

void TFloatQuery::Show()
{
	if (count <= FLOAT_QUERY_MAX + 1)
		office.Show(wait);
	else
	{
		office.Close();
		THTMLFile::Show(office.Name, wait, false);
	}

	Debug();
}

#define floatquery OFFICE_GROUP_CAST(TFloatQuery)

void floatquery_title(mstr &m, TGroup *group)
{
	m.cpy(floatquery->title ? floatquery->title : "Нерегламентирана справка");
}

void floatquery_count(mstr &m, TGroup *group)
{
	counter_matches(m, floatquery);
}

#undef floatquery

const offield float_fields[] =
{
	{ "F_JUDGE", generic_judge },
	{ "F_TITLE", floatquery_title },
	{ "F_COUNT", floatquery_count },
	{ NULL, NULL }
};
