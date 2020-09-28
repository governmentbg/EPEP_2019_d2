#include "cac.h"

// ----- TReportJudge --------------------------------------------------------
TReportJudge::TReportJudge(const char *tUCN)
{
	strcpy(ucn, tUCN);

	if (atob(ucn))
	{
		if (COURT_CODE == COURT_SOFGS)
			print_name(name, ucn, UCN_CITIZEN_UCN);
		else
			print_user(name, ucn);
	}

	memset(values, '\0', sizeof values);
}

#define judge OFFICE_GROUP_CAST(TReportJudge)

static void repjudge_no(mstr &m, TGroup *group) { itom(judge->no, m); }
static void repjudge_name(mstr &m, TGroup *group) { m.cat(judge->name); }

static void repjudge_value(mstr &m, TGroup *group)
{
	unsigned sect, col;

	if (sscanf(judge->Name, "F_S_%d_C_%d", &sect, &col) == 2 && --sect < SECTIONS_MAX && --col < JUDGE_COLUMNS_MAX)
	{
		itom(judge->values[sect][col], m);
		judge->values[sect][col] = 0;		// mark as used
	}
}

#undef judge

const offield repjudge_fields[] =
{
	{ "F_NO",		repjudge_no },
	{ "F_JUDGE", 	repjudge_name },
	{ "*F_S_",		repjudge_value },
	{ NULL,		NULL }
};

// ----- TReportJudgeArray ---------------------------------------------------
void TReportJudgeArray::Increment(const char *ucn, int section, int column, bool total)
{
	TReportJudge *judge;
	int i;

	for (i = 0; i < Count(); i++)
		if (!strcmp(JudgeAt(i)->ucn, ucn))
			break;

	if (i < Count())
		judge = JudgeAt(i);
	else
	{
		judge = new TReportJudge(ucn);

		for (i = 0; i < Count(); i++)
			if (JudgeAt(i)->name.cmp(judge->name) > 0)
				break;

		Insert(judge, i);
	}

	judge->values[section][column]++;

	if (total)
		judge->values[section][0]++;
}

void TReportJudgeArray::Summary(const char *name)
{
	TReportJudge *total = new TReportJudge(EMPTY);
	total->name.cpy(name);

	for (int i = 0; i < Count(); i++)
		for (int sect = 0; sect < SECTIONS_MAX; sect++)
			for (int col = 0; col < JUDGE_COLUMNS_MAX; col++)
				total->values[sect][col] += JudgeAt(i)->values[sect][col];
	Insert(total, 0);
}
