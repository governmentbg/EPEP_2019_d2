#include "cac.h"

#define NEWLVER 1

// ----- TLoadUnit -----------------------------------------------------------
TLoadUnit::TLoadUnit()
{
	Add(
		new TCharAlias("F_COLLEGE", &college),
		new TLongAlias("F_COMPOSITION", &composition),
		new TLong("F_FLAGS", &flags),
		new TString("F_TEXT", text, sizeof text),
		new TDate("F_DATE", &date, false),
		new TLongAlias("F_HALL", &hall),
		NULL
	);
}

TLoadUnit::~TLoadUnit()
{
	delete session;
}

void TLoadUnit::PrintTime(mstr &m)
{
	if (session)
	{
		if (start)
			m.printf("%s %T", Kind->Seek(session->kind), &session->time);

		if (ender)
			m.printf("%s %T", start ? "-" : "до", &session->entime);
	}

	if (!session || (!start && !ender))
		m.cat(bef ? "Преди обяд" : "След обяд");
}

void TLoadUnit::PrintSession(mstr &m)
{
	m.cat("\ab\b");
	print_type_or_kind(m, session->key);
	m.printf(" %ld/%ld\a/b\b", session->key.no, session->key.year);

	TConnect connect;

	if (connect.TryBy(session, CONNECT_TYPE_FLOOR_LAWSUIT, "F_CONNECT_NO, F_CONNECT_YEAR"))
	{
		ConnectType->Seek(CONNECT_TYPE_FLOOR_LAWSUIT);
		m.printf(", %s %s/%ld", ConnectType->alias, connect.connectNo, connect.connectYear);
	}

	TLawsuit lawsuit;
	lawsuit.key = session->key;

	if (lawsuit.Get("F_SUBJECT, F_TEXT"))
	{
		m.printf("\v%s", Subject->Seek(session->key.type, lawsuit.subject));
		if (any(lawsuit.text))
			m.printf("\v%s", lawsuit.text);
	}

	constant judges[JUDGE_COUNT] = { session->president, session->judgeX, session->reJudgeX };
	bool judge_printed = false;	// 2014:223 bold main judge only, fallback

	for (int i = 0; i < JUDGE_COUNT; i++)
	{
		if (atob(judges[i]))
		{
			bool judge_match = !strcmp(judges[i], session->judge);

			m.cat(judge_match ? "\vСъдия \ab\b" : "\vСъдия ");
			print_user(m, judges[i]);

			if (judge_match)
			{
				m.cat("\a/b\b");
				judge_printed = true;
			}
		}
	}

	if (!judge_printed)
	{
		m.cat("\vСъдия \ab\b");
		print_user(m, session->judge);
		m.cat("\a/b\b");
	}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	constant proceed = "\v\ab\bДаден ход на съдебното следствие\a/b\b";

	if (session->flags & SESSION_PROCEED)
		m.cat(proceed);
	else if (session->key.type == TYPE_PUNISHMENT_LAW)
	{
		msql w(session->Name, "F_NO");

		w.Add(session, "F_NO, F_YEAR, F_TYPE");
		w.AddChars("F_KIND", KIND_OPEN_SESSIONS);
		w.Add("F_DATE");
		w.printf("<= %ld", session->date.Value());
		w.AddFlags("F_FLAGS", SESSION_PROCEED, SESSION_PROCEED, SESSION_ALL);

		TQuery q(str(w));
		if (q.Read())
			m.cat(proceed);
	}
#endif  // AREAL || MILITARY || REGIONAL

	if (atob(session->prosecutor))
	{
		m.cat("\vПрокурор ");
		print_user(m, session->prosecutor);
	}

	constant jurors[JUROR_COUNT] = { session->juror, session->reJuror, session->jurorX, session->reserve };

	for (int i = 0; i < JUROR_COUNT; i++)
	{
		constant juror = jurors[i];

		if (atob(juror))
		{
			m.cat(juror == session->reserve ? "\vРезервен съдебен заседател " : "\vСъдебен заседател ");
			print_user(m, juror);
		}
	}
}

void TLoadUnit::PrintNonSession(mstr &m)
{
#if NEWLVER
	if (college)
	{
		m.cat(College->Seek(college));

		if (composition)
			m.printf(" %s с-в", Composition->Seek(composition));

		if (any(text))
			m.printf("\v%s", text);
	}
	else if (any(text))
		m.cat(text);
	else
		m.cat(flags & LOAD_HALL_RESERVED ? "Резервирана" : "Свободна");
#else  // NEWLVER
	if (any(text))
		m.cat(text);
	else
	{
		m.cat("Свободна");

		if (college)
		{
			m.printf(" - %s", College->Seek(college));

			if (composition)
				m.printf(" %s с-в", Composition->Seek(composition));
		}
	}
#endif  // NEWLVER
}

void TLoadUnit::PrintLoad(mstr &m)
{
	PrintTime(m);
	m.cat(" ");

	if (session)
	{
		print_type_or_kind(m, session->key);
		m.printf(" %ld/%ld ", session->key.no, session->key.year);
		print_user(m, session->judge);
	}
	else
		PrintNonSession(m);
}

void TLoadUnit::PrintHtml(mstr &m)
{
	if (session)
	{
		m.cat("\ab\b");
		PrintTime(m);
		m.cat("\a/b\b\v");
		PrintSession(m);
	}
	else
	{
		//m.cat("преди обяд -", "след обяд -");
		PrintNonSession(m);
	}
}

bool TLoadUnit::Empty()
{
	return !(session || college || (flags & LOAD_HALL_RESERVED) || any(text));
}

bool TLoadUnit::SameSlot(const TLoadUnit &that)
{
	return date == that.date && hall == that.hall && bef == that.bef;
}

bool TLoadUnit::operator<=(const TLoadUnit &that)
{
	return date != that.date ? date < that.date : hall != that.hall ? hall < that.hall : bef != that.bef ? bef > that.bef :
		!session != !that.session ? !session > !that.session :	// equal NULL sessions have different heading data
		session->date != that.session->date ? session->date < that.session->date : session->time <= that.session->time;
}

#define unit OFFICE_GROUP_CAST(TLoadUnit)

static void unit_date(mstr &m, TGroup *group) { m.printf("%0D", &unit->date); }
static void unit_weekday(mstr &m, TGroup *group) { m.cat(Weekday->Seek(unit->date.DigitOfWeek())); }
static void unit_time(mstr &m, TGroup *group) { unit->PrintTime(m); }
static void unit_hall(mstr &m, TGroup *group) { m.cat(Hall->Seek(unit->hall)); }

static void unit_usage(mstr &m, TGroup *group)
{
	if (unit->session)
		unit->PrintSession(m);
	else
		unit->PrintNonSession(m);
}

#undef unit

static const offield unit_fields[] =
{
	{ "F_DATE",		unit_date },
	{ "F_WEEKDAY",	unit_weekday },
	{ "F_TIME",		unit_time },
	{ "F_HALL",		unit_hall },
	{ "F_USAGE",	unit_usage },
	{ NULL, NULL }
};

// ----- TLoadGroup ----------------------------------------------------------
TLoadGroup::TLoadGroup()
	: TPeriodGroup("T_HALL_GROUP")
{
	maxDate.AddMonths(2);
	names[0] = NULL;
	names[1] = NULL;
}

TLoadGroup::~TLoadGroup()
{
	delete[] names[0];
	delete[] names[1];
}

void TLoadGroup::PrepareUnits()
{
	CDate date;
	units.Flush();

	// PASS 1: fillout with fake units
	for (date = minDate; date <= maxDate; date += 1)
	{
		for (int i = 0; i < halls.Count(); i++)
		{
			FillUnits(date, halls[i], true);
			FillUnits(date, halls[i], false);
		}
	}
}

void TLoadGroup::AddCriteria(msql &m)
{
	// PASS 2: AddSession() now outside
	m.AddRange("((F_DATE", minDate, maxDate);
	m.AddIsNull("F_END", true);
	m.cat(")");
	m.NextOr();
	m.Add("(F_DATE");
	m.printf("<= %ld", maxDate.Value());
	m.Add("F_END");
	m.printf(">= %ld", minDate.Value());
	m.cat("))");

	//m.AddArray("F_HALL", halls);
	m.AddChars("NOT F_RESULT", RESULT_UNLINKEDS);
	m.AddOrder("F_DATE, F_TIME, F_YEAR, F_NO, F_TYPE");
}

void TLoadGroup::AddSession(TSession &session)
{
	if (session.end.Empty())
		session.end = session.date;

	for (date = session.date > minDate ? session.date : minDate; date <= session.end && date <= maxDate; date += 1)
	{
		if ((date == session.date && session.time.Bef()) || date > session.date)
			AddUnit(session, date, true);

		if ((date == session.end && !(session.entime.Empty() ? session.time : session.entime).Bef()) ||
			date < session.end)
		{
			AddUnit(session, date, false);
		}
	}
}

void TLoadGroup::FixupUnits()
{
	// PASS 3: remove discardable units
	for (int i = 0; i < units.Count() - 1; )
	{
		TLoadUnit *loadUnit = units[i];
		TLoadUnit *nextUnit = units[i + 1];

		if (loadUnit->Empty() && loadUnit->SameSlot(*nextUnit))
			units.Remove(i);
		else
			i++;
	}

	// PASS 4: mark sequential dates
	for (int i = 0; i < units.Count(); i++)
		units[i]->subseq = i > 0 && units[i]->date == units[i - 1]->date;
}

void TLoadGroup::FillUnits(const CDate &date, long hall, bool bef)
{
	TLoadData *loadData = NULL;

	{
		TLoadDate loadDate;

		loadDate.hall = hall;
		loadDate.date = date;

		if (loadDate.Try())
			loadData = bef ? &loadDate.bef : &loadDate.aft;
	}

	if (!loadData)
	{
		TLoadWeek loadWeek;

		loadWeek.hall = hall;
		loadWeek.weekday = date.DigitOfWeek();
		if (loadWeek.Try())
			loadData = bef ? &loadWeek.bef : &loadWeek.aft;
	}

	if (loadData && ((loadData->flags & LOAD_HALL_RESERVED) ||
		loadData->college && strchr(Default->colleges, loadData->college)))
	{
		TLoadUnit *loadUnit = new TLoadUnit;

		loadUnit->date = date;
		loadUnit->dow = date.DigitOfWeek();
		loadUnit->hall = hall;
		loadUnit->bef = bef;
		loadUnit->session = NULL;
		loadUnit->start = false;
		loadUnit->ender = false;
		loadUnit->college = loadData->college;
		loadUnit->composition = loadData->composition;
		loadUnit->flags = loadData->flags;
		strcpy(loadUnit->text, loadData->text);
		units.Add(loadUnit);	// pass 1 - simply append
	}
}

void TLoadGroup::AddUnit(TSession &session, const CDate &date, bool bef)
{
	TLoadUnit *loadUnit = new TLoadUnit;

	loadUnit->date = date;
	loadUnit->dow = date.DigitOfWeek();
	loadUnit->hall = session.hall;
	loadUnit->bef = bef;
	loadUnit->session = new TSession;
	*loadUnit->session << session;
	loadUnit->start = date == session.date && session.time.Bef() == bef;
	loadUnit->ender = date == session.end && !session.entime.Empty() && session.entime.Bef() == bef;
	loadUnit->college = session.key.type;
	loadUnit->composition = session.composition;
	// 2014:156 LPR: using pass 2 manual sort -> operator<=
	units.Add(loadUnit);
}

void TLoadGroup::FillMonDays()
{
	CDate date(1, minDate.month, minDate.year);

	for (mon_count = 0; date <= maxDate; mon_count++, date.AddMonths(1))
	{
		int count = date.DaysInMonth();
		int offset = date.DayOfWeek() - 1;

		for (int i = 0; i < CALENDAR_DAYS_MAX; i++)
			mon_days[mon_count][i] = i < offset || i >= offset + count ? 0 : i - offset + 1;
	}
}

void TLoadGroup::CheckPeriod(mstr &m, TSession *session)
{
	if (session->end.Empty())
		session->end = session->date;

	if (session->date < minDate || session->date > maxDate)
		m.printf("Началната дата %D е извън избрания период.\n", &session->date);

	if (session->end != session->date && (session->end < minDate || session->end > maxDate))
		m.printf("Крайната дата %D е извън избрания период.\n", &session->end);
}

static void check_jurors_overlap(mstr &m, const TSession *session, const TLoadUnit *unit)
{
	// 2018-06-06: LPR/IRQ: also check reserve
	constant sesJurors[JUROR_COUNT] = { session->juror, session->reJuror, session->jurorX, session->reserve };
	constant uniJurors[JUROR_COUNT] = { unit->session->juror, unit->session->reJuror, unit->session->jurorX,
		unit->session->reserve };

	for (int n = 0; n < 3; n++)
	{
		constant sesJuror = sesJurors[n];

		for (int u = 0; u < 3; u++)
		{
			if (atob(sesJuror) && atob(uniJurors[u]) && !strcmp(sesJuror, uniJurors[u]))
			{
				print_user(m, sesJuror);
				m.printf(" участва в заседание по %s %ld/%ldг.",
					Type->Seek(unit->session->key.type), unit->session->key.no,
					unit->session->key.year);

				if (unit->hall && unit->hall != session->hall)
					m.printf(" (%s)", Hall->Seek(unit->hall));

				m.cat("\n");
			}
		}
	}
}

void TLoadGroup::CheckUsage(mstr &m, TSession *session, const TLawsuit *lawsuit)
{
	for (date = session->date > minDate ? session->date : minDate; date <= session->end && date <= maxDate; date += 1)
	{
		CTime sesTime, sesEntime;

		if (date == session->date)
			sesTime = session->time;	// else remains 00:00

		if (date == session->end)
		{
			if (session->entime.Empty())
			{
				if (date == session->date && session->time.Bef())
				{
					sesEntime.hour = 12;
					sesEntime.minute = 29;
				}
				else
				{
					sesEntime.hour = 23;
					sesEntime.minute = 59;
				}
			}
			else
				sesEntime = session->entime;
		}
		else
		{
			sesEntime.hour = 23;
			sesEntime.minute = 59;
		}

		for (int i = 0; i < units.Count(); i++)
		{
			TLoadUnit *unit = units[i];

			if (unit->date == date)
			{
				CTime uniTime;
				CTime uniEntime;

				if (unit->start)
					uniTime = unit->session->time;
				else if (!unit->bef)	// on bef remains 00:00
				{
					uniTime.hour = 12;
					uniTime.minute = 30;
				}

				if (unit->ender)
					uniEntime = unit->session->entime;
				else if (unit->bef)
				{
					uniEntime.hour = 12;
					uniEntime.minute = 29;
				}
				else
				{
					uniEntime.hour = 23;
					uniEntime.minute = 59;
				}

				if (sesTime <= uniEntime && sesEntime >= uniTime)
				{
					size_t pre_len = m.len();
					m.printf("На %D %s обяд ", &date, unit->bef ? "преди" : "след");
					size_t post_len = m.len();

					if (unit->hall == session->hall)
					{
						if (unit->session)
						{
							m.printf("заседанието застъпва %s по %s %ld/%ldг.\n",
								Kind->Seek(unit->session->kind), Type->Seek(unit->session->key.type),
								unit->session->key.no, unit->session->key.year);
						}
						else if (unit->flags & LOAD_HALL_RESERVED)
							m.cat("залата е резервирана.\n");
						else if (unit->college && (unit->college != lawsuit->college || (unit->composition &&
							unit->composition != lawsuit->composition)))
						{
							m.cat("залата се използва от ");
							if (unit->composition)
								m.printf("%s състав на ", Composition->Seek(unit->composition));
							m.printf("%s %s.\n", College->Seek(unit->college), COLLEGE);
						}
					}

					if (unit->session)
						check_jurors_overlap(m, session, unit);

					const char *nl = strchr(str(m) + post_len, '\n');

					if (!nl)
						m.cut(pre_len);
					else if (strchr(nl + 1, '\n'))
					{
						mbk_remove(&m, post_len - 1, 1);
						mbk_insert(&m, ":\n", post_len - 1, 2);
						ncstr(m)[post_len + 1] = toupper(str(m)[post_len + 1]);
					}
				}
			}
			else if (unit->date > session->end)
				break;
		}
	}
}

bool TLoadGroup::CheckAsk(mstr &m, TSession *session, const TLawsuit *lawsuit, bool outside)
{
	bool jurors = atob(session->juror) || atob(session->reJuror) || atob(session->jurorX);  // can't have reserve only

	if (outside)
		m.printf("Използването на залата%s извън избрания период не е проверено.\n", jurors ? " и заседателите" : "");

	if (!jurors)
	{
		Subject->Seek(lawsuit->key.type, lawsuit->subject);

		if (strchr(Subject->Attrib(lawsuit->kind), POST_JUROR))
		{
			m.printf("Няма данни за съдебни заседатели. "
				"Не е извършена проверка за участието на заседатели по други дела.\n");
		}
	}

	return !any(m) || ask("%s\nЖелаете ли да продължите?", str(m));
}

#define loadGroup OFFICE_GROUP_CAST(TLoadGroup)

//static void load_styles(mstr &m, TGroup *)
//{
//	DWORD color = GetSysColor(COLOR_BTNFACE);
//	m.printf("@media screen,projection { body { background:#%02lx%02lx%02lx; } }",
//		color & 0xFF, (color >> 8) & 0xFF, color >> 16);
//}

static void load_period(mstr &m, TGroup *group) { m.printf("%D-%D", &loadGroup->minDate, &loadGroup->maxDate); }

static void load_halls(mstr &m, TGroup *group)
{
	for (int i = 0; i < loadGroup->halls.Count(); i++)
	{
		m.sep(", ");
		m.cat(Hall->Seek(loadGroup->halls[i]));
	}
}

static void load_month(mstr &m, TGroup *group)
{
	CDate date = loadGroup->minDate;
	date.AddMonths(loadGroup->mon_idx);
	m.cat(Month->Seek(date.month));
}

static constant
	COLOR_WORK	= "blue",
	COLOR_HOLY	= "red",
	COLOR_NULL	= "silver",
	COLOR_CALL	= "black;text-decoration:none";

static void load_day(mstr &m, TGroup *group)
{
	int day;

	if (sscanf(loadGroup->Name, "F_DAY_%d", &day) == 1 && day >= 1 && day <= 7)
	{
		day += loadGroup->day_idx - 1;

		if (day < CALENDAR_DAYS_MAX && (day = loadGroup->mon_days[loadGroup->mon_idx][day]) != 0)
		{
			CDate date = loadGroup->minDate;

			date.AddMonths(loadGroup->mon_idx);
			date.day = day;

			if (date >= loadGroup->minDate && date <= loadGroup->maxDate)
			{
				m.printf("\aa style=\"color:%s\" href=\"#m%02dd%02d\"\b%d\a/a\b",
					date.Work() ? COLOR_WORK : COLOR_HOLY, date.month, day, day);
			}
			else
				m.printf("\aspan style=\"color:%s\"\b%d\a/span\b", COLOR_NULL, day);
		}
	}
}

static void load_hall(mstr &m, TGroup *group)
{
	m.cat(Hall->Seek(loadGroup->halls[loadGroup->hall_idx]));
}

static void load_holy(mstr &m, TGroup *group)
{
	if (!loadGroup->date.Work())
		m.printf(";color=%s", COLOR_HOLY);
}

static void load_date(mstr &m, TGroup *group)
{
	m.printf("\aa");
	if (!loadGroup->date.Work())
		m.printf(" style=\"color:%s\"", COLOR_HOLY);
	m.printf(" name=\"m%02dd%02d\"\b\x06%0D\x06\a/a\b", loadGroup->date.month, loadGroup->date.day, &loadGroup->date);
}

static void load_weekday(mstr &m, TGroup *group)
{
	m.printf("\aspan");
	if (!loadGroup->date.Work())
		m.printf(" style=\"color:%s\"", COLOR_HOLY);
	m.printf("\b%s\a/span\b", Weekday->Seek(loadGroup->date.DigitOfWeek()));
}

static void load_usage(mstr &m, TGroup *group, bool bef)
{
	for (int i = 0; i < loadGroup->units.Count(); i++)
	{
		TLoadUnit *loadUnit = loadGroup->units[i];

		if (loadUnit->date == loadGroup->date && loadUnit->hall == loadGroup->halls[loadGroup->hall_idx] &&
			loadUnit->bef == bef)
		{
			if (any(m))
				m.cat("\v\x97\x97\x97\v");
			else
			{
				m.printf("\aa style=\"color:%s\" href=\"#s%02ld%02d%02d%02d%d\" title=\"%s\n%s обяд\"\b", COLOR_CALL,
					loadUnit->date.year - YEAR_MIN, loadUnit->date.month, loadUnit->date.day, loadUnit->hall,
					loadUnit->session ? 2 : loadUnit->bef, Hall->Seek(loadUnit->hall), bef ? "Преди" : "След");
			}

			loadUnit->PrintHtml(m);
		}
		else if (loadUnit->date > loadGroup->date)
			break;
	}

	if (any(m))
		m.cat("\a/a\b");
}

static void load_bef(mstr &m, TGroup *group) { load_usage(m, group, true); }
static void load_aft(mstr &m, TGroup *group) { load_usage(m, group, false); }

#undef loadGroup

static const offield load_fields[] =
{
	//{ "F_STYLES",	load_styles },
	{ "F_PERIOD",	load_period },
	{ "F_HALLS",	load_halls },
	{ "F_MONTH",	load_month },
	{ "*F_DAY_",	load_day },
	{ "F_HALL",		load_hall },
	{ "F_HOLY",		load_holy },
	{ "F_DATE",		load_date },
	{ "F_WEEKDAY",	load_weekday },
	{ "F_BEF",		load_bef },
	{ "F_AFT",		load_aft },
	{ NULL, NULL }
};

bool TLoadGroup::Generate()
{
	if (!office.Open("Sched1c.htm", OFFICE_COPY_ALL, "Sched1c.htm"))
		return false;

	if (!names[0])
		names[0] = strnew(office.Name);

	{
		constant monstart = office.Find("mon start");
		constant monbreak = office.Find("mon break");
		constant monfinal = office.Find("mon final");

		office.Emit(NULL, monstart, load_fields, this);

		for (mon_idx = 0; mon_idx < mon_count; mon_idx++)
		{
			if (mon_idx)
				office.Emit(monbreak, monfinal, null_fields, NULL);

			office.Emit(monstart, monbreak, load_fields, this);
		}

		constant dowstart = office.Find("dow start");
		constant dowbreak = office.Find("dow break");
		constant dowfinal = office.Find("dow final");

		office.Emit(monfinal, dowstart, null_fields, NULL);

		for (mon_idx = 0; mon_idx < mon_count; mon_idx++)
		{
			if (mon_idx)
				office.Emit(dowbreak, dowfinal, null_fields, NULL);

			office.Emit(dowstart, dowbreak, null_fields, NULL);
		}

		constant daystart = office.Find("day start");
		constant daybreak = office.Find("day break");
		constant dayfinal = office.Find("day final");

		for (day_idx = 0; day_idx < CALENDAR_DAYS_MAX; day_idx += 7)
		{
			office.Emit(dowfinal, daystart, null_fields, NULL);

			for (mon_idx = 0; mon_idx < mon_count; mon_idx++)
			{
				if (mon_idx)
					office.Emit(daybreak, dayfinal, null_fields, NULL);

				office.Emit(daystart, daybreak, load_fields, this);
			}
		}

		constant hallstart = office.Find("hall start");
		constant hallfinal = office.Find("hall final");

		office.Emit(dayfinal, hallstart, null_fields, NULL);

		for (hall_idx = 0; hall_idx < halls.Count(); hall_idx++)
			office.Emit(hallstart, hallfinal, load_fields, this);

		constant datestart = office.Find("date start");
		constant befstart = office.Find("bef start");
		constant beffinal = office.Find("bef final");
		constant aftstart = office.Find("aft start");
		constant aftfinal = office.Find("aft final");
		constant datefinal = office.Find("date final");

		for (date = minDate; date <= maxDate; date += 1)
		{
			office.Emit(datestart, befstart, load_fields, this);

			for (hall_idx = 0; hall_idx < halls.Count(); hall_idx++)
				office.Emit(befstart, beffinal, load_fields, this);

			office.Emit(beffinal, aftstart, null_fields, NULL);

			for (hall_idx = 0; hall_idx < halls.Count(); hall_idx++)
				office.Emit(aftstart, aftfinal, load_fields, this);

			office.Emit(aftfinal, datefinal, null_fields, NULL);
		}

		office.Emit(datefinal, NULL, load_fields, this);
		office.Close();
	}

	if (!office.Open("Sched2.htm", OFFICE_COPY_ALL, "Sched2.htm"))
		return false;

	if (!names[1])
		names[1] = strnew(office.Name);

	{
		constant pr1start = office.Find("pr1 start");
		constant pr1break = office.Find("pr1 break");
		constant pr1final = office.Find("pr1 final");
		CDate lastDate;

		office.Emit(NULL, pr1start, load_fields, this);

		for (int i = 0; i < units.Count(); i++)
		{
			TLoadUnit *unit = units[i];

			if (!unit->session)
				continue;

			// 2015:149 JRQ: FIX: was !unit->subseq
			if (unit->date != lastDate)
			//if (!unit->subseq)
			{
				office.Emit(pr1start, pr1break, unit_fields, unit);
				lastDate = unit->date;
			}

			office.Emit(pr1break, pr1final, unit_fields, unit);
		}

		office.Emit(pr1final, NULL, null_fields, NULL);
		office.Close();
	}

	return true;
}
