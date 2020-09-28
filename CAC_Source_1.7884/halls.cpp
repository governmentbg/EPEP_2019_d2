#include "cac.h"

#include "EditHallDialog.h"
#include "EditHallDialog.cpp"

static bool edit_hall(TWindow *parent, THall *hall, bool edit)
{
	return TEditHallDialog(parent, hall, edit).Execute() == IDOK;
}

#include "LoadHallDialog.h"
#include "LoadHallDialog.cpp"

#include "LoadDateDialog.h"
#include "LoadDateDialog.cpp"

static void load_date(TWindow *parent, THall *hall)
{
	TLoadDate loadDate;

	if (get_date(parent, &loadDate, NULL, true, true))
	{
		if (loadDate.date.Work())
		{
			loadDate.hall = hall->hall;
			bool edit = loadDate.Try();

			if (!edit)
			{
				TLoadWeek loadWeek;
				loadWeek.hall = hall->hall;
				loadWeek.weekday = loadDate.date.DigitOfWeek();
				if (loadWeek.Try())
					loadDate << loadWeek;
			}

			TLoadDateDialog(parent, &loadDate, edit).Execute();
		}
		else
			error("Според календара на системата %Dг. е почивен ден", &loadDate.date);
	}
}

#include "LoadWeekDialog.h"
#include "LoadWeekDialog.cpp"

#include "HallListWindow.h"
#include "HallListWindow.cpp"

#include "HallListDialog.h"
#include "HallListDialog.cpp"

void edit_halls(TWindow *parent)
{
	log(LOG_NOM, CM_ADMIN_HALLS, "зали");
	THallListDialog(parent).Execute();
	Hall->ReloadCache();
}

#include "HallListBox.h"
#include "HallListBox.cpp"

#include "GetLoadCriteriaDialog.h"
#include "GetLoadCriteriaDialog.cpp"

#include "HtmlLoadDialog.h"
#include "HtmlLoadDialog.cpp"

#define loadUnit OFFICE_GROUP_CAST(TLoadUnit)

static void loadunit_date(mstr &m, TGroup *group)
{
	if (!loadUnit->subseq)
		m.printf("%D", &loadUnit->date);
}

static void loadunit_weekday(mstr &m, TGroup *group)
{
	if (!loadUnit->subseq)
	{
		Weekday->Seek(loadUnit->dow);
		m.cpy(Weekday->alias);
	}
}

static void loadunit_hall(mstr &m, TGroup *group) { Hall->Seek(loadUnit->hall); m.cpy(Hall->alias); }
static void loadunit_load(mstr &m, TGroup *group) { loadUnit->PrintLoad(m); }

#undef loadUnit

#include "LoadUnitListWindow.h"
#include "LoadUnitListWindow.cpp"

#include "LoadUnitListDialog.h"
#include "LoadUnitListDialog.cpp"

void load_sessions(TWindow *parent)
{
	TLoadGroup loadGroup;
	TLoadGroup invisible;

	if (TGetLoadCriteriaDialog(parent, &loadGroup, &invisible).Execute() == IDOK)
	{
		{
			TWaitWindow wait(parent, WAIT);

			loadGroup.PrepareUnits();

			// PASS 2: add real units
			TSession session;
			msql m(session.Name);
			loadGroup.AddCriteria(m);
			TQuery q(str(m));

			while (q.Read())
			{
				session << q;
				(loadGroup.halls.Contains(session.hall) ? loadGroup : invisible).AddSession(session);
			}

			loadGroup.FillMonDays();
			loadGroup.FixupUnits();
			invisible.FixupUnits();
		}

	#if TESTVER
		if (!ask("Новия вариант?"))
			TLoadUnitListDialog(parent, &loadGroup, &invisible).Execute();
		else
	#endif  // TESTVER
		if (loadGroup.Generate())
			THtmlLoadDialog(parent, &loadGroup, &invisible).Execute();
	}
}
