#include "cac.h"

static const offield sesprice_fields[] =
{
	{ "F_DATE",		NULL },
	{ "F_PRICE",	NULL },
	{ NULL,		NULL }
};

static void price_list(TWindow *, TGroupArray *array)
{
	TOffice office;

	if (office.Open("SessPrice.htm"))
	{
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < array->Count(); i++)
			office.Emit(tr1start, tr1final, sesprice_fields, (*array)[i]);

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(NULL);
	}
}

#include "EditSessionPriceDialog.h"
#include "EditSessionPriceDialog.cpp"

#define stimes OFFICE_GROUP_CAST(TSessionTimes)

static void start_date(mstr &m, TGroup *group)
{
	((const CDate &) stimes->start).Print(m);
}

static void start_time(mstr &m, TGroup *group)
{
	((const CTime &) stimes->start).Print(m);
}

#undef stimes

#include "SessionPriceListWindow.h"
#include "SessionPriceListWindow.cpp"

#include "SessionPriceListDialog.h"
#include "SessionPriceListDialog.cpp"

void edit_session_prices(TWindow *parent)
{
	TSessionPriceListDialog(parent).Execute();
}

static bool check_equal_times(const TTimeFace *start, const TTimeFace *entime)
{
	const CTime tStart = start->Time();
	return tStart != entime->Time() || tStart.Empty() || ask("Началния и крайния час съвпадат. %s", Q_CONTINUE);
}

bool check_session_times(const TTimeFace *start, const TTimeFace *entime, int oldCount)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	(void) start;
	(void) entime;
	(void) oldCount;
#endif  // APPEAL || ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (start->Time().Empty())
	{
		if (oldCount && !ask("Данните за продължителността по заседателни дни ще бъдат изтрити. %s", Q_CONTINUE))
			return false;
	}
	else if (!check_equal_times(start, entime))
		return false;
#endif  // AREAL || MILITARY || REGIONAL
	return true;
}

#include "EditSessionTimesDialog.h"
#include "EditSessionTimesDialog.cpp"

#include "SessionTimesListWindow.h"
#include "SessionTimesListWindow.cpp"

#include "SessionTimesListDialog.h"
#include "SessionTimesListDialog.cpp"

bool edit_session_times(TWindow *parent, TGSArray<TSessionTimes> &times)
{
	bool edited = false;

	TSessionTimesListDialog(parent, times, &edited).Execute();
	return edited;
}

const char *session_times_complect(bool optional, const CTime &startime, const CTime &entime)
{
	if (optional)
		return startime.Empty() == entime.Empty() ? NULL : "Началния и крайния час не са комплектни";

	static constant statuses[] =
	{
		NULL,
		"Началния час е задължителен",
		"Крайния час е задължителен",
		"Началния и крайния час за задължителни"
	};

	return statuses[(entime.Empty() << 1) + ((CTime &) startime).Empty()];
}

const char *session_times_invalid(bool internal, const CDate &stdate, const CTime &startime, const CTime &entime)
{
	const char *status = session_times_complect(internal && !stdate.Work(), startime, entime);
	return status ? status : startime > entime ? "Началния час е след крайния" : NULL;
}

static int session_times_chours(const CTime &startime, const CTime &tEntime)
{
	CTime entime = tEntime;

	if (entime.minute % 5 == 0)  // ends "14:30" or "10:25"
		entime += -1;

	int minutes = entime.Value() - startime.Value();

	return minutes <= 0 ? 1 : (minutes + 59) / 60;
}

const char *session_times_invalid(bool internal, const CDate &stdate, const CTime &startime, const CTime &entime, int hours)
{
	const char *status = session_times_invalid(internal, stdate, startime, entime);

	if (!status)
	{
		if (!startime.Empty())
		{
			if (stdate >= JUROR_MIN_CORR_DATE && !hours)
				status = "Отработените часове са задължителени";
			else if (hours > session_times_chours(startime, entime))
				status = "Отработените часове надвишават времевия интервал";
		}
		else if (hours)
			status = "Отработените часове изискват времеви интервал";
	}

	return status;
}

bool session_times_valid(TGSArray<TSessionTimes> *times, bool reportErr)
{
	for (int i = 0; i < times->Count(); i++)
	{
		const TSessionTimes *stimes = (const TSessionTimes *) (*times)[i];
		const char *status = stimes->Invalid(i > 0 && i < times->Count() - 1);

		if (status)
		{
			return reportErr ? error("Продължителност по заседателни дни, %D: %s.",
				&(CDate &) stimes->start, status) : false;
		}
	}

	return true;
}

bool session_times_valid(TGSArray<TSessionTimes> *times, mstr &m)
{
	int errors = 0;

	for (int i = 0; i < times->Count(); i++)
	{
		const TSessionTimes *stimes = (const TSessionTimes *) (*times)[i];
		const char *status = stimes->Invalid(i > 0 && i < times->Count() - 1);

		if (status)
		{
			m.cat(errors ? ",\n" : "Данните за продължителност по заседателни дни не са попълнени коректно:\n\n");
			m.printf("За %D: %s", &(CDate &) stimes->start, status);

			if (++errors == 5)
				break;
		}
	}

	return !errors;
}

void recalc_session_hours(TTimeFace *tStarTime, TTimeFace *tEnTime, TLongFace *hours)
{
	if (tStarTime->IsValid(false) && tEnTime->IsValid(false))
	{
		const CTime &startime = tStarTime->Time();
		const CTime &entime = tEnTime->Time();

		if (!startime.Empty() && entime >= startime)
			hours->SetL(session_times_chours(startime, entime));
	}
}
