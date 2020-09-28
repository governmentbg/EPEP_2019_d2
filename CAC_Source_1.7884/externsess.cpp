#include "cac.h"

#if COURT_TYPE == COURT_AREAL
// ----- TExternSessionListGroup ---------------------------------------------
class TExternSessionListGroup : public TFloatGroup
{
public:
	TExternSessionListGroup();

	virtual void FormCriteria(msql &m);

	char type;
	CDate minDate;
	CDate maxDate;
};

TExternSessionListGroup::TExternSessionListGroup()
	: TFloatGroup("T_EXTERN_SESSION")
{
	Add(
		new TCharAlias("F_TYPE", &type),
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		NULL
	);

	minDate = WeekAgo;
}

void TExternSessionListGroup::FormCriteria(msql &m)
{
	m.Begin(Name, fields);
	m.AddChar("F_TYPE", type);
	m.AddRange("F_DATE", minDate, maxDate);
	m.AddOrder("F_DATE, F_TIME");
}

#define session OFFICE_GROUP_CAST(TExternSession)

static void extern_key_no_year(mstr &m, TGroup *group) { m.printf("С %ld/%ld", session->key.no, session->key.year); }

static void extern_kind(mstr &m, TGroup *group)
{
	m.cat(strchr(KIND_CLOSED_SESSIONS, session->kind) ? "Закрито заседание" : Kind->Seek(session->kind));
}

#undef session

#include "ExternSessionListWindow.h"
#include "ExternSessionListWindow.cpp"

#include "ExternSessionListDialog.h"
#include "ExternSessionListDialog.cpp"

// ----- entry point ---------------------------------------------------------
void extern_sessions(TWindow *parent)
{
	TExternSessionListDialog(parent).Execute();
}
#endif  // AREAL
