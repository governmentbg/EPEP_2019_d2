#include "cac.h"

#if TESTVER
#define ET_UPDATE ET_ERROR
#else  // TESTVER
#define ET_UPDATE ET_SILENT
#endif  // TESTVER

// ----- TUpdate -------------------------------------------------------------
class TUpdate : protected TProcess
{
public:
	TUpdate(TWindow *parent, int tCommand, const char *tItems, const char *tAction, const char *wait,
		const char *tWhatWrong = NULL);

	virtual void Execute() = 0;

protected:
	void UpdateLog(const char *phase);
	void UpdateStart();
	void UpdateTotal();	// the standard Count() + Final() + Show() sequence
	void UpdateCount();
	void UpdateFinal();
	void UpdateShow() { tf.Show(wait); }

	int command;
	const char *items;
	const char *action;
	int total;
	int wrong, ignored;
	const char *whatWrong;
};

TUpdate::TUpdate(TWindow *parent, int tCommand, const char *tItems, const char *tAction, const char *wait,
	const char *tWhatWrong)
	: TProcess(parent, NULL, wait), command(tCommand), items(tItems), action(tAction ? tAction : "Промяна"),
		whatWrong(tWhatWrong ? tWhatWrong : "Грешки при запис")
{
}

void TUpdate::UpdateLog(const char *phase)
{
	CTime time = Now();
	mstr m;

	m.printf("%s - %s", phase, items);
	log(LOG_UPDATE, command, str(m));
	dprintf("%s: %2d.%02d.%02d\n", phase, time.hour, time.minute, time.second);
}

void TUpdate::UpdateStart()
{
	total = count = 0;
	ignored = wrong = 0;
	dprintf("%s на %s\n", action, items);
	UpdateLog("Начало");
	fputc('\n', fd);
}

void TUpdate::UpdateTotal()
{
	UpdateCount();
	UpdateFinal();
	UpdateShow();
}

void TUpdate::UpdateCount()
{
	dprintf("\n"
		"Проверени %s: %d\n"
		"Актуализирани %s: %ld\n"
		"Игнорирани %s: %d\n"
		"%s: %d\n",
		items, total, items, count, items, ignored, whatWrong, wrong);
}

void TUpdate::UpdateFinal()
{
	fputc('\n', fd);
	UpdateLog("Край");
}

// ----- discard citizens ----------------------------------------------------
static int ucksum(long l)
{
	static char multi[9] = { 2, 4, 8, 5, 10, 9, 7, 3, 6 };
	int sum = 0;

	for (int i = 0; i < 9; i++)
	{
		sum += multi[8 - i] * (l % 10);
		l /= 10;
	}

	return sum % 11 % 10;
}

struct TUCName
{
	const char *table;
	const char *ucnName;
	const char *ucnTypeName;
	char ucnType;
};

static const TUCName UCNames[] =
{
	// nom
	{ "T_USER", "F_UCN", NULL, '\0' },
	{ "T_COMPOSITION", "F_JUDGE", NULL, '\0' },
	{ "T_COMPOSITION", "F_SECRETAR", NULL, '\0' },
	{ "T_COMPOSITION", "F_JUDGE_X", NULL, '\0' },
	{ "T_COMPOSITION", "F_REJUDGE_X", NULL, '\0' },
	{ "T_COMPOSITION", "F_PRESIDENT", NULL, '\0' },
	{ "T_STREET_MAP", "F_UCN", NULL, '\0' },
	{ "T_KNOWN_PERSON", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	// doc
	{ "T_LAWSUIT", "F_JUDGE", NULL, '\0' },
	{ "T_LAWSUIT", "F_REJUDGE", NULL, '\0' },
	{ "T_SESSION", "F_JUDGE", NULL, '\0' },
	{ "T_SESSION", "F_SECRETAR", NULL, '\0' },
	{ "T_SESSION", "F_PROSECUTOR", NULL, '\0' },
	{ "T_SESSION", "F_JUROR", NULL, '\0' },
	{ "T_SESSION", "F_REJUROR", NULL, '\0' },
	{ "T_SESSION", "F_JUDGE_X", NULL, '\0' },
	{ "T_SESSION", "F_REJUDGE_X", NULL, '\0' },
	{ "T_SESSION", "F_JUROR_X", NULL, '\0' },
	{ "T_SESSION", "F_PRESIDENT", NULL, '\0' },
	{ "T_SESSION", "F_RESERVE", NULL, '\0' },
	{ "T_SENDER", "F_UCN", "F_UCN_TYPE", UCN_EXECUTOR_UCN },
	{ "T_JUDGEMENT", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_SURROUND", "F_JUDGE", NULL, '\0' },
	{ "T_LOG", "F_UCN", NULL, '\0' },
	{ "T_INREG", "F_JUDGE", NULL, '\0' },
	{ "T_INREG_EVENT", "F_JUDGE", NULL, '\0' },
	{ "T_OUTREG", "F_RECEIVER", "F_RECEIVER_TYPE", UCN_EXECUTOR_UCN },
	{ "T_MOVEMENT", "F_UCN", NULL, '\0' },
	{ "T_ACCUSATION", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_REQUEST_RESOLUTION", "F_JUDGE", NULL, '\0' },
	{ "T_REQUEST_RESOLUTION", "F_ASSIGN", NULL, '\0' },
	{ "T_SCHED", "F_JUDGE", NULL, '\0' },
	{ "T_EXPENSE", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_REGISTERED_MAIL", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_SIDE_WIND", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_SUBPOENA", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_SUBPOENA", "F_PROXY_UCN", "F_PROXY_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_SUBPOENA", "F_REDIRECT_UCN", "F_REDIRECT_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_SUBPOENA", "F_DELIVERER_UCN", NULL, NULL },
	{ "T_SUBPOENA", "F_USER_UCN", NULL, NULL },
	{ "T_SUBPOENA", "F_LRECEIVER_UCN", "F_LRECEIVER_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_LINK_WIND", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_LINK_WIND", "F_PROXY_UCN", "F_PROXY_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_LINK_WIND", "F_REDIRECT_UCN", "F_REDIRECT_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_CONNECT", "F_UCN", "F_UCN_TYPE", UCN_EXECUTOR_UCN },
	{ "T_JUROR_DISTRIB", "F_JUROR", NULL, '\0' },
	{ "T_JUROR_DISTRIB", "F_REJUROR", NULL, '\0' },
	{ "T_JUROR_DISTRIB", "F_JUROR_X", NULL, '\0' },
	{ "T_JUROR_DISTRIB", "F_RESERVE", NULL, '\0' },
	{ "T_ANNOUNCE", "F_DELIVERER_UCN", NULL, '\0' },
	{ "T_BANKRUPT", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_FZL", "fzlegn", NULL, '\0' },
	{ "T_PAYMENT", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_PAYMENT", "F_OBL", "F_OBL_TYPE", UCN_CITIZEN_UCN },
	{ "T_EXEC_LIST_PERSON", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_EXEC_LIST_COLLECT", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_EXEC_LIST_ADDRESS", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_JUROR_LOG", "F_UCN", NULL, '\0' },
	{ "T_JUROR_LOG", "F_JUROR", NULL, '\0' },
	{ "T_EXTERN_SESSION", "F_JUDGE", NULL, '\0' },
	{ "T_EXTERN_SESSION", "F_SECRETAR", NULL, '\0' },
	{ "T_EXTERN_SESSION", "F_JUROR", NULL, '\0' },
	{ "T_EXTERN_SESSION", "F_REJUROR", NULL, '\0' },
	{ "T_EXTERN_SESSION", "F_JUROR_X", NULL, '\0' },
	{ "T_EXTERN_SESSION", "F_RESERVE", NULL, '\0' },
	{ "T_EXTERN_SIDE_WIND", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_XFER_DECISION", "F_JUDGE1_EGN", NULL, '\0' },
	{ "T_XFER_DECISION", "F_JUDGE2_EGN", NULL, '\0' },
	{ "T_XFER_DECISION", "F_JUDGE3_EGN", NULL, '\0' },
	{ "T_XFER_SIDE_CITIZEN", "F_SIDE_EGN", NULL, '\0' },
	{ "T_XFER_SIDE_CITIZEN", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_XFER_EXPERT", "F_EXPERT_EGN", NULL, '\0' },
	{ "T_EPORT_SEND", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ "T_EPORT_GUID", "F_UCN", "F_UCN_TYPE", UCN_CITIZEN_UCN },
	{ NULL, NULL, NULL, NULL }
};

class TDiscardCitizens : public TUpdate
{
public:
	TDiscardCitizens(TWindow *parent);

	virtual void Execute();

protected:
	void CreateDiscarder(const char *table, msql &m, TIntegerArray &kills);
	void DiscardCitizens(msql &m, TIntegerArray &kills);
};

TDiscardCitizens::TDiscardCitizens(TWindow *parent)
	: TUpdate(parent, CM_ADMIN_DISCARD_CITIZENS, "лица с ЕГН", "Изтриване на неизползвани", WAIT)
{
}

void TDiscardCitizens::CreateDiscarder(const char *table, msql &m, TIntegerArray &kills)
{
	m.Begin(table, MQ_DELETE);
	m.Add("F_UCN");
	for (int i = 0; i < kills.Count(); i++)
		m.printf("%s'%09ld%d'", i ? ", " : "IN (", kills[i], ucksum(kills[i]));
	m.cat(")");
	m.AddChar("F_UCN_TYPE", UCN_CITIZEN_UCN);
}

void TDiscardCitizens::DiscardCitizens(msql &m, TIntegerArray &kills)
{
	static constant tables[] = { "T_OLD_CITIZEN", "T_ADDRESS", "T_FOREIGN_ADDRESS", NULL };

	CreateDiscarder("T_CITIZEN", m, kills);
#if TESTVER
	dprintf("%s\n", str(m));
	count += kills.Count();
	for (constant *table = tables; *table; table++)
	{
		CreateDiscarder(*table, m, kills);
		dprintf("%s\n", str(m));
	}
#else  // TESTVER
	int killed = AppName->ExecCount(str(m), ET_ERROR);

	if (killed != kills.Count())
		dprintf("%s: грешка при изтриване: %ld вместо %ld\n", str(m), killed, kills.Count());

	count += killed;
	wrong += (kills.Count() - killed);

	if (killed)
	{
		for (constant *table = tables; *table; table++)
		{
			CreateDiscarder(*table, m, kills);
			AppName->ExecCount(str(m), ET_ERROR);
		}
	}
#endif  // TESTVER
	kills.Clear();
}

// skip <= (0)206
#define MIN_UCN 20
#define NEXT(C) ((C) + rand() % 100)

void TDiscardCitizens::Execute()
{
	UpdateStart();

	TIntegerArray ucns(true);
	char ucn[SIZE_OF_UCN];
	int xproc = 0;
	srand(GetTickCount());
	int next = NEXT(50);

	for (const TUCName *ucname = UCNames; ucname->table; ucname++)
	{
		TDummyGroup g;
		g.Add(new TString(ucname->ucnName, ucn, sizeof ucn));

		mstr fields;
		fields.printf("DISTINCT(%s)", ucname->ucnName);

	#if RANGECHECK
		if ((ucname->ucnTypeName != NULL) != (ucname->ucnType != '\0'))
			fatal("discard: range check at %s, %s", ucname->table, ucname->ucnName);
	#endif  // RANGECHECK

		msql m(ucname->table, str(fields));
		if (ucname->ucnTypeName)
			m.AddChar(ucname->ucnTypeName, ucname->ucnType);

		TQuery q(str(m));
		while (q.Read())
		{
			g << q;
			ucn[LENGTH_OF_UCN - 1] = '\0';
			long l = atoi(ucn);

			if (l > MIN_UCN && ucns.Find(l) == INT_MAX)
				ucns.Add(l);

			if (xproc++ % next == 0)
			{
				wait->TickCount(xproc, "заредени ЕГН");
				next = NEXT(50);
			}
		}
	}

	{
		TLog log;
		msql m(log.Name, "F_TEXT");

		m.AddChar("F_GOP", LOG_CREATE);
		m.AddLike("F_TEXT", "ЕГН ", false);
		TQuery q(str(m));

		while (q.Read())
		{
			log << q;
			memcpy(ucn, log.text + 4, LENGTH_OF_UCN - 1);
			ucn[LENGTH_OF_UCN - 1] = '\0';
			long l = atoi(ucn);

			if (l > MIN_UCN && ucns.Find(l) == INT_MAX)
				ucns.Add(l);

			if (xproc++ % next == 0)
			{
				wait->TickCount(xproc, "проверени ЕГН");
				next = NEXT(50);
			}
		}
	}

	{
		TCitizen citizen;
		msql m(citizen.Name);
		TIntegerArray kills;

		m.AddChar("F_UCN_TYPE", UCN_CITIZEN_UCN);
		TQuery q(str(m));

		while (q.Read())
		{
			citizen << q;
			strcpy(ucn, citizen.ucn);
			ucn[LENGTH_OF_UCN - 1] = '\0';
			long l = atoi(ucn);

			if (ucksum(l) + '0' != citizen.ucn[LENGTH_OF_UCN - 1])
			{
			#if TESTVER
				// can't reproduce string ucn
				dprintf("%s: невалидно ЕГН\n", citizen.ucn);
			#endif  // TESTVER
				ignored++;
			}
			else if (l > MIN_UCN && ucns.Find(l) == INT_MAX)
			{
				dprintf("%s %s %s %s\n", citizen.ucn, citizen.name, citizen.reName, citizen.family, citizen.reFamily);
				kills.Add(l);

				if (kills.Count() % next == 0)
				{
					DiscardCitizens(m, kills);
					wait->TickCount(count, "изтрити ЕГН");
					next = NEXT(250);
				}
			}
			else
				ignored++;

			total++;
		}

		if (kills.Count())
			DiscardCitizens(m, kills);
	}

	dprintf("\n"
		"Проверени %s: %d\n"
		"Запазени %s: %d\n"
		"Изтрити %s: %ld\n"
		"Грешки при изтриване: %d\n",
		items, total, items, ignored, items, count, wrong);

	UpdateFinal();
	UpdateShow();
}

void discard_citizens(TWindow *parent) { TDiscardCitizens(parent).Execute(); }

// ----- update session times -------
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
class TUpdateSessionTimes : public TUpdate
{
public:
	TUpdateSessionTimes(TWindow *parent);

	virtual void Execute();
};

TUpdateSessionTimes::TUpdateSessionTimes(TWindow *parent)
	: TUpdate(parent, CM_ADMIN_UPDATE_SESSION_TIMES, "продължителности по заседателни дни", "Актуализация", WAIT)
{
}

void TUpdateSessionTimes::Execute()
{
	UpdateStart();

	TSessionTimes sTimes;
	CDateTime minCorrStamp;
	msql m(sTimes.Name);

	m.Add("F_START");
	(CDate &) minCorrStamp = JUROR_MIN_CORR_DATE;
	m.printf(">= %ld", minCorrStamp.Value());
	m.AddIsNull("F_HOURS", true);
	TQuery q(str(m));

	while (q.Read())
	{
		sTimes << q;
		CTime entime = sTimes.entime;

		if (entime.minute % 5 == 0)	// ends "14:30" or "10:25"
			entime += -1;

		int minutes = entime.Value() - ((const CTime &) sTimes.start).Value();

		sTimes.hours = minutes <= 0 ? 1 : (minutes + 59) / 60;

		if (sTimes.Update("F_HOURS"))
			count++;
		else
		{
			dprintf("%d %ld/%ld %d %D: грешка при запис\n", sTimes.key.type, sTimes.key.no, sTimes.key.year,
				sTimes.kind, &(CDate &) sTimes.start);
			wrong++;
		}

		wait->TickCount(++total, "обработени продължителности");
	}

	UpdateFinal();
}

void update_session_times(TWindow *parent)
{
	TUpdateSessionTimes(parent).Execute();
}
#endif  // AREAL || MILITARY || REGIONAL

#include "migrate.cpp"

#include "update_mini.cpp"

#include "exportCSV.cpp"

// ----- TUpdateIntervals ----------------------------------------------------
class TUpdateIntervals : public TUpdate
{
public:
	TUpdateIntervals(TWindow *parent);

	virtual void Execute();
};

TUpdateIntervals::TUpdateIntervals(TWindow *parent)
	: TUpdate(parent, CM_ADMIN_UPDATE_INTERVALS, "времеви интервали", "Актуализация", WAIT)
{
}

void TUpdateIntervals::Execute()
{
	TLawsuit lawsuit;
	TLawInterval interval;

	UpdateStart();

	for (int year = YEAR_MIN; year <= Today.year; year++)
	{
		msql m;

		m.printf("Интервали от %dг...", year);
		wait->SetText(str(m));

		m.Begin("T_LAWSUIT L JOIN T_LAW_INTERVAL I ON I.F_NO = L.F_NO AND I.F_YEAR = L.F_YEAR AND I.F_TYPE = L.F_TYPE",
			"L.F_NO, L.F_YEAR, L.F_TYPE, L.F_GRANTED, I.F_DATE");
		m.AddLong("L.F_YEAR", year);
		m.AddChar("NOT L.F_GRANTED", GRANT_NONE_YET);
		m.AddIsNull("I.F_FINISHED", false);
		m.AddChar("I.F_GRANTED", GRANT_NONE_YET);

		TQuery q(str(m));
		while (q.Read())
		{
			lawsuit << q;
			m.Begin(interval.Name, MQ_UPDATE);
			lawsuit.Quote(m, "F_GRANTED");
			m.Add(&lawsuit, "F_NO, F_YEAR, F_TYPE, F_DATE");

			if (interval.DataBase->Execute(str(m), ET_UPDATE))
				count++;
			else
			{
				dprintf("%s %ld/%ld начало %D: грешка при запис\n", Type->Seek(lawsuit.key.type), lawsuit.key.no,
					lawsuit.key.year, &lawsuit.date);
				wrong++;
			}

			wait->TickCount(++total, "обработени интервала");
		}
	}

	UpdateTotal();
}

void update_intervals(TWindow *parent) { TUpdateIntervals(parent).Execute(); }

// ----- TUpdateRequests -----------------------------------------------------
class TUpdateRequests : public TUpdate
{
public:
	TUpdateRequests(TWindow *parent);

	virtual void Execute();
};

TUpdateRequests::TUpdateRequests(TWindow *parent)
	: TUpdate(parent, CM_ADMIN_UPDATE_REQUESTS, "входящи документи", "Актуализация", WAIT_MORE)
{
}

struct TKindFromTo
{
	const char *fromKinds;
	char toKind;
	char baseType;  // 'p' 'c' 'd'
};

static const TKindFromTo KindFromTos[] =
{
#if COURT_TYPE == COURT_AREAL
	{ "8",   'w', 'c' },
	{ "r",   'j', 'c' },
	{ "y",   'Г', 'c' },
	{ "q",   'm', 'c' },
	{ "23Ф", '1', 'p' },
	{ "Вт",  '5', 'p' },
	{ "Q",   'ѕ', 'c' },
	{ "…",   '¶', 'c' },
	{ "”",   '„', 'c' },
	{ "›",   '‹', 'c' },
	{ "­",   '¬', 'c' },
	{ "•",   '·', 'c' },
	{ "Э",   'Ћ', 'c' },
	{ "7",   'Ж', 'c' },
	{ "0",   'Ч', 'c' },
	{ "Ґ",   'ґ', 'c' },
	{ "d",   '™', 'c' },
	{ "Ј",   'І', 'c' },
	{ "†",   '‡', 'c' },
	{ "‘",   '‚', 'c' },
	{ "\\",  'В', 'c' },  // 2017:097 LPR: reserved
#endif  // AREAL
	{ NULL, '\0' }
};

void TUpdateRequests::Execute()
{
	TRequest request;
	TRequest tRequest;
	TElectric electric;
	TLawsuit lawsuit;
	char tNewKind;
	TKind tKind;
	TLog tLog;

	if (!electric_rekind_access_check())
		return;

	request.Add(new TChar("F_NEW_KIND", &tNewKind));
	UpdateStart();

	for (int year = YEAR_MIN; year <= Today.year; year++)
	{
		msql m;

		m.printf("Входящи документи от %dг...", year);
		wait->SetText(str(m));

		m.Begin("T_REQUEST R JOIN T_LAWSUIT L ON L.F_SOURCE_NO = R.F_NO AND L.F_SOURCE_YEAR = R.F_YEAR "
			"AND L.F_SOURCE_TYPE = R.F_TYPE AND NOT L.F_KIND = R.F_LAW_KIND",
			"R.F_NO, R.F_YEAR, R.F_KIND, R.F_LAW_KIND, L.F_KIND AS F_NEW_KIND");
		m.AddLong("R.F_YEAR", year);

		TQuery q(str(m));
		while (q.Read())
		{
			request << q;
			m.Begin(request.Name, MQ_UPDATE);
			m.printf("F_LAW_KIND = '%c'", tNewKind);
			m.Add(&request, "F_NO, F_YEAR, F_TYPE");

			if (request.DataBase->Execute(str(m), ET_UPDATE))
			{
			#if TESTVER
				dprintf("Входящ документ %ld/%ld -> %s\n", request.key.no, request.key.year, Kind->Seek(tNewKind));
			#endif  // TESTVER
			}
			else
			{
				dprintf("Входящ документ %ld/%ld: грешка при запис\n", request.key.no, request.key.year);
				wrong++;
			}
		}

		for (const TKindFromTo *tk = KindFromTos; tk->fromKinds; tk++)
		{
			m.Begin("T_REQUEST", "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_LAW_KIND");
			m.AddLong("F_YEAR", year);
			m.AddChars("F_KIND", tk->fromKinds);

			TQuery r(str(m));
			while (r.Read())
			{
				request << r;
				total++;

				{  // 2017:097 use incompat baseType for the new docs
					char baseType = kind2type(request.lawKind);

					if (strchr(TYPE_CITIZALIKE_LAWS, baseType))
						baseType = TYPE_CITIZEN_LAW;

					if (baseType != tk->baseType)
					{
						dprintf("Входящ документ %ld/%ld -> игнориран, различен основен вид\n");
						ignored++;
						continue;
					}
				}

				tRequest << request;
				tRequest.kind = tk->toKind;
				// electric; 2017:101 moved 1st, return if failed
				if (!electric_rekind(&request, tRequest.kind, request.date))
					return;
				// winds
				move_winds(&request, &tRequest);
				m.Begin(electric.Name, MQ_UPDATE);
				tRequest.Quote(m, "F_KIND");
				m.Add(&request, "F_NO, F_YEAR, F_TYPE");
				electric.DataBase->Execute(str(m), ET_UPDATE);
				// lawsuit
				if (lawsuit.TryBy(tRequest.key, "F_NO, F_YEAR, F_TYPE"))
				{
					lawsuit.sourceKind = tRequest.kind;

					if (lawsuit.Update("F_SOURCE_KIND"))
					{
					#if TESTVER
						dprintf("%s %ld/%ld -> образувано по %s\n", Type->Seek(lawsuit.key.type), lawsuit.key.no,
							lawsuit.key.year, Kind->Seek(tRequest.kind));
					#endif  // TESTVER
					}
					else
					{
						dprintf("%s %ld/%ld: грешка при замяна вида на входящия документ\n",
							Type->Seek(lawsuit.key.type), lawsuit.key.no, lawsuit.key.year);
					}
				}
				// 2017:097 LPR: TLog
				{
					m.Begin(tLog.Name);
					m.Add(&request, "F_NO, F_YEAR, F_TYPE, F_KIND");
					TQuery p(str(m));
					while (p.Read())
					{
						tLog << p;
						tLog.kind = tRequest.kind;

						if (tLog.Update())
							;  // too many messages
						else
						{
							dprintf("Входящ документ %ld/%ld: грешка при корекция на запис %ld в журнала\n",
								request.key.no, request.key.year, tLog.log);
						}
					}
				}
				// request
				m.Begin(tRequest.Name, MQ_UPDATE);
				tRequest.Quote(m, "F_KIND");
				m.Add(&request, "F_NO, F_YEAR, F_TYPE");

				if (tRequest.DataBase->Execute(str(m), ET_UPDATE))
				{
				#if TESTVER
					dprintf("Входящ документ %ld/%ld -> %s\n", request.key.no, request.key.year,
						Kind->Seek(tRequest.kind));
				#endif  // TESTVER
					count++;
				}
				else
				{
					dprintf("Входящ документ %ld/%ld: грешка при запис\n", request.key.no, request.key.year);
					wrong++;
				}

				wait->TickCount(total, "обработени входящи документа");
			}
		}
	}

	UpdateTotal();
}

void update_requests(TWindow *parent) { TUpdateRequests(parent).Execute(); }

// ----- TUpdateLongTexts ----------------------------------------------------
class TUpdateLongTexts : public TUpdate
{
public:
	TUpdateLongTexts(TWindow *parent);

	virtual void Execute();
};

TUpdateLongTexts::TUpdateLongTexts(TWindow *parent)
	: TUpdate(parent, CM_ADMIN_UPDATE_LONG_TEXTS, "входящи документи и изпълнителни листи", "Актуализация", WAIT_MORE)
{
}

void TUpdateLongTexts::Execute()
{
	UpdateStart();

	for (int year = YEAR_MIN; year <= Today.year; year++)
	{
		msql m;
		TOutReg outReg;

		m.printf("Изходящи документи от %dг...", year);
		wait->SetText(str(m));

		m.Begin(outReg.Name, "F_NO, F_YEAR, F_TYPE, F_TEXT");
		m.AddLong("F_YEAR", year);
		m.Add("F_TEXT");
		m.cat("LIKE '% '");
		TQuery q(str(m));

		while (q.Read())
		{
			outReg << q;
			strimtrail(ncstr(outReg.text));
			outReg.text.fixize();

			m.Begin(outReg.Name, MQ_UPDATE);
			outReg.Quote(m, "F_TEXT");
			outReg.ExactCriteria(m, MQ_SUBSEQ);

			if (outReg.DataBase->Execute(str(m), ET_UPDATE))
				count++;
			else
			{
				dprintf("Изходящ документ %ld/%ld: грешка при запис\n", outReg.key.no, outReg.key.year);
				wrong++;
			}

			wait->TickCount(++total, "обработени изходящи документа");
		}
	}

	for (int year = YEAR_MIN; year <= Today.year; year++)
	{
		msql m;
		TExecList execList;

		m.printf("Изходящи документи от %dг...", year);
		wait->SetText(str(m));

		m.Begin(execList.Name, "F_EXEC_LIST_NO, F_EXEC_LIST_YEAR, F_TYPE, F_EXEC_LIST_KIND, F_TDEP_TEXT");
		m.AddLong("F_EXEC_LIST_YEAR", year);
		m.Add("F_TDEP_TEXT");
		m.cat("LIKE '% '");
		TQuery q(str(m));

		while (q.Read())
		{
			execList << q;
			strimtrail(ncstr(execList.tdepText));
			execList.tdepText.fixize();

			m.Begin(execList.Name, MQ_UPDATE);
			execList.Quote(m, "F_TDEP_TEXT");
			execList.ExactCriteria(m, MQ_SUBSEQ);

			if (execList.DataBase->Execute(str(m), ET_UPDATE))
				count++;
			else
			{
				dprintf("Изпълнителен лист %ld/%ld: грешка при запис\n", execList.execListNo, execList.execListYear);
				wrong++;
			}

			wait->TickCount(++total, "обработени изпълнителни листи");
		}
	}

	dprintf("\n"
		"Актуализирани %s: %ld\n"
		"%s: %d\n",
		items, count, whatWrong, wrong);

	UpdateFinal();
	UpdateShow();
}

void update_long_texts(TWindow *parent) { TUpdateLongTexts(parent).Execute(); }

// ----- TUpdateLogKinds -----------------------------------------------------
class TUpdateLogKinds : public TUpdate
{
public:
	TUpdateLogKinds(TWindow *parent);

	virtual void Execute();

protected:
	void ExecuteLL();
};

TUpdateLogKinds::TUpdateLogKinds(TWindow *parent)
	: TUpdate(parent, CM_ADMIN_UPDATE_LOG_KINDS, "журнални записи", "Актуализация", WAIT_MORE)
{
}

void TUpdateLogKinds::ExecuteLL()
{
	static constant NEXIST = "Липсват дата и час на извършване на актуализация на входящи документи\n";
	TLog tLog;
	CDateTime maxStamp;
	msql m(tLog.Name, "MIN(F_STAMP) AS F_STAMP");

	m.Add("F_STAMP");
	m.printf(">= '15.3.2017 0:0:0'");
	m.AddLong("F_OPC", CM_ADMIN_UPDATE_REQUESTS);
	TQuery q(str(m));

	if (!q.Read())
	{
		dprintf(NEXIST);
		return;
	}

	tLog << q;
	maxStamp = tLog.stamp;

	if (maxStamp.Empty())
	{
		dprintf(NEXIST);
		return;
	}

	for (int year = YEAR_MIN; year <= maxStamp.year; year++)
	{
		CDateTime endStamp = (year < maxStamp.year) ? CDateTime(31, 12, year, 23, 59, 59) : maxStamp;

		m.Begin(tLog.Name);
		m.AddRange("F_STAMP", CDateTime(1, 1, year, 0, 0, 0), endStamp);
		m.AddChar("F_TYPE", TYPE_REQUEST);
		m.AddIsNull("F_KIND", false);

		TQuery q(str(m));
		while (q.Read())
		{
			tLog << q;

			const TKindFromTo *tk;

			for (tk = KindFromTos; tk->fromKinds; tk++)
			{
				if (strchr(tk->fromKinds, tLog.kind))
				{
					tLog.kind = tk->toKind;

					if (tLog.Update())
						count++;
					else
					{
						dprintf("Входящ документ %ld/%ld: грешка при корекция на запис %ld в журнала\n",
							tLog.key.no, tLog.key.year, tLog.log);
						wrong++;
					}

					break;
				}
			}

			if (!tk->fromKinds)
				ignored++;

			wait->TickCount(++total, "обработени журнални записа");
		}
	}
}

void TUpdateLogKinds::Execute()
{
	UpdateStart();
	ExecuteLL();
	UpdateTotal();
}

void update_log_kinds(TWindow *parent) { TUpdateLogKinds(parent).Execute(); }

// ----- TUpdateEPortals -----------------------------------------------------
class TUpdateEPortals : public TUpdate
{
public:
	TUpdateEPortals(TWindow *parent);

	virtual void Execute();
};

TUpdateEPortals::TUpdateEPortals(TWindow *parent)
	: TUpdate(parent, CM_ADMIN_UPDATE_EPORTALS, "обекти изпратени към " EPORTAL_NAME, "Актуализация", WAIT_MORE)
{
}

void TUpdateEPortals::Execute()
{
	UpdateStart();

	// Decisions
	{
		msql m("T_EPORT_GUID G JOIN T_DECISION D ON D.F_NO = G.F_NO AND D.F_YEAR = G.F_YEAR AND D.F_TYPE = G.F_TYPE "
			"AND D.F_KIND = G.F_KIND AND D.F_DATE = G.F_DATE", "D.*");
		TDecision decision;

		m.AddLong("F_WHAT", WHAT_ENDOC);
		m.AddFlags("D.F_FLAGS", DECISION_PUBLIC, 0, DECISION_ALL);

		TQuery q(str(m));
		while (q.Read())
		{
			decision << q;

			if (eportal_delete_update(&decision, WHAT_ENDOC))
				count++;
			else
				wrong++;
		}
	}

	// Sessions
	{
		TSession session;
		msql m("T_EPORT_GUID G JOIN T_SESSION S ON S.F_NO = G.F_NO AND S.F_YEAR = G.F_YEAR AND S.F_TYPE = G.F_TYPE "
			"AND S.F_KIND = G.F_KIND AND S.F_DATE = G.F_DATE", "S.*, G.F_WHAT");

		long what;
		session.Add(new TLong("F_WHAT", &what));

		m.AddRange("F_WHAT", WHAT_SESSION, WHAT_SESS_ENDOC);
		m.AddFlags("S.F_FLAGS", SESSION_PRIVATE, SESSION_PRIVATE, SESSION_ALL);
		m.AddOrder("F_WHAT DESC");

		TQuery q(str(m));
		while (q.Read())
		{
			session << q;

			if (eportal_delete_update(&session, what))
				count++;
			else
				wrong++;
		}
	}

	// Lawsuits
	if (XFER_EPORTAL == 2)
	{
		msql m("T_EPORT_GUID G JOIN T_LAWSUIT L ON L.F_NO = G.F_NO AND L.F_YEAR = G.F_YEAR AND L.F_TYPE = G.F_TYPE",
			"L.*");
		TLawsuit lawsuit;

		m.AddLong("F_WHAT", WHAT_LAWSUIT);
		m.AddFlags("L.F_FLAGS", LAWSUIT_LIMITED, LAWSUIT_LIMITED, LAWSUIT_ALL);

		TQuery q(str(m));
		while (q.Read())
		{
			lawsuit << q;

			if (eportal_delete_limited(&lawsuit))
				count++;
			else
				wrong++;
		}
	}

	// Session/SessEndoc Public Protocol Files
	{
		char value[SIZE_OF_CONFIG_VALUE];
		TConfigure::GetGlobal("filter", "SESSIONS", value);

		if (!strcmp(value, "0"))
		{
			TSession session;
			msql m("T_EPORT_GUID G JOIN T_SESSION S ON S.F_NO = G.F_NO AND S.F_YEAR = G.F_YEAR "
				"AND S.F_TYPE = G.F_TYPE AND S.F_KIND = G.F_KIND AND S.F_DATE = G.F_DATE", "S.*, G.F_WHAT");

			long what;
			session.Add(new TLong("F_WHAT", &what));

			m.AddChars("F_KIND", KIND_SESSIONS);
			m.AddLong("(F_WHAT", WHAT_PUBLIC_PROTOCOL_FILE);
			m.NextOr();
			m.AddLong("F_WHAT", WHAT_PUBLIC_ACT_FILE);
			m.cat(")");

			TQuery q(str(m));
			while (q.Read())
			{
				session << q;

				if (eportal_delete_public_file(&session, what))
					count++;
				else
					wrong++;
			}
		}
	}

	dprintf("\n"
		"Актуализирани обекти: %d\n"
		"Грешки при запис: %d\n",
		count, wrong);

	UpdateFinal();
	UpdateShow();
}

void update_eportals(TWindow *parent)
{
	if (XFER_EPORTAL != 1 || warn("Стойността на [transfer] EPORTAL е 1. "
		"Ако към %s са били изпратени дела с ограничен достъп, те няма да бъдат изтрити.\n%s",
		EPORTAL_NAME, Q_CONTINUE))
	{
		TUpdateEPortals(parent).Execute();
	}
}
