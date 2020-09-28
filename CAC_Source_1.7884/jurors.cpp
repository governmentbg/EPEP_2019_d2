#include "cac.h"

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
// ----- TJurorHistory -------------------------------------------------------
TJurorHistory::TJurorHistory(const TJurorDistrib *distrib)
{
	key = distrib->key;
	excess = distrib->excess;
}

void TJurorHistory::Execute()
{
	msql m;

	memset(jurors, '\0', sizeof jurors);
	FloatCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		*this << q;
		CheckMD5();
		Event();

		if (index != JUROR_DISTRIB_NONE)
			strcpy(jurors[index], juror);
	}
}

enum
{
	NO_PREFIX = 0,
	OLD_JUROR = 1,
	NEW_JUROR = 2
};

void print_juror(mstr &m, const char *ucn, int prefix)
{
	TKnownPerson person;

	person.ucnType = UCN_CITIZEN_UCN;
	strcpy(person.ucn, ucn);
	person.involvement = INVOLVEMENT_JUROR;

	static constant prefixes[] = { "", "Стар: ", "Нов: " };

	m.cat(prefixes[prefix]);

	if (person.Get())
		person.PedName(m, false);
	else
		m.cat(ucn);
}

#define jurorhis OFFICE_GROUP_CAST(TJurorHistory)

static void jurorhis_action(mstr &m, TGroup *group)
{
	const char *name;

	switch (jurorhis->oper)
	{
		case JUROR_DISTRIB_AUTO : name = "Автоматично разпределение"; break;
		case JUROR_DISTRIB_DISTRIB : name = "Автоматично разпределение"; break; // "Разпределение"
		case JUROR_DISTRIB_OLDISTRIB : name = "Преразпределение"; break; // "(Преразпределение)";
		case JUROR_DISTRIB_REDISTRIB : name = "Преразпределение"; break;
		case JUROR_DISTRIB_CLEAR : name = "Изключване"; break;
		case JUROR_DISTRIB_MANUAL : name = "Ръчно разпределение"; break;
		case JUROR_DISTRIB_BEGIN : name = "Начало"; break;
		case JUROR_DISTRIB_OK : name = "Потвърждение"; break;
		case JUROR_DISTRIB_CANCEL : name = "Прекъсване"; break;
		case JUROR_DISTRIB_DELETE : name = "Изтриване"; break;
		case JUROR_DISTRIB_IMPORTED : name = "Текущо разпределение"; break;
		default : name = "Невалидна операция";
	}

	m.cpy(name);
}

static void jurorhis_juror(mstr &m, TGroup *group)
{
	if (jurorhis->index != JUROR_DISTRIB_NONE)
	{
		const char *oldJuror = jurorhis->Jurors(jurorhis->index);

		if (atob(oldJuror) && atob(jurorhis->juror) && strcmp(oldJuror, jurorhis->juror))
		{
			print_juror(m, oldJuror, OLD_JUROR);
			m.cat("\v");
			print_juror(m, jurorhis->juror, NEW_JUROR);
		}
		else if (atob(oldJuror))
			print_juror(m, oldJuror, NO_PREFIX);
		else if (jurorhis->oper == JUROR_DISTRIB_REDISTRIB || jurorhis->oper == JUROR_DISTRIB_MANUAL)
			print_juror(m, jurorhis->juror, NEW_JUROR);
		else if (atob(jurorhis->juror))
			print_juror(m, jurorhis->juror, NO_PREFIX);
	}
	else
	{
		if (jurorhis->oper == JUROR_DISTRIB_OLDISTRIB)
			print_juror(m, jurorhis->juror, OLD_JUROR);
		else if (jurorhis->oper == JUROR_DISTRIB_REDISTRIB || jurorhis->oper == JUROR_DISTRIB_MANUAL)
			print_juror(m, jurorhis->juror, NEW_JUROR);
		else if (atob(jurorhis->juror))
			print_juror(m, jurorhis->juror, NO_PREFIX);
	}
}

static void jurorhis_user(mstr &m, TGroup *group) { print_user(m, jurorhis->ucn); }
static void jurorhis_date(mstr &m, TGroup *group) { m.printf("%Dг.", &(CDate &) jurorhis->stamp); }
static void jurorhis_time(mstr &m, TGroup *group) { m.printf("%lTч.", &(CTime &) jurorhis->stamp); }
static void jurorhis_motives(mstr &m, TGroup *group) { m.cat(jurorhis->text); }

static void jurorhis_user_post(mstr &m, TGroup *group)
{
	if (atob(jurorhis->ucn) && User->Seek(jurorhis->ucn))
		m.cpy(Post->Seek(strchr(POST_JUDGES, User->post) ? POST_JUDGE : User->post));
}

static void jurorhis_law_kind(mstr &m, TGroup *group)
{
	TCharAlias *lawKind = (TCharAlias *) group->Find("F_LAW_KIND");

	if (lawKind)  // 2017:241 for TJurorFloatHistory
	{
		Kind->Seek(lawKind->C());
		m.cat(Kind->alias);
	}
	else if (jurorhis->lawsuit)
	{
		Kind->Seek(jurorhis->lawsuit->kind);
		m.cat(Kind->alias);
	}
	else
	{
		Kind->Seek(jurorhis->key.type);
		m.cat(Type->alias);
	}
}

static void jurorhis_no(mstr &m, TGroup *group)
{
	m.printf("%s%ld", TBaseSession::ExcessText(jurorhis->excess), jurorhis->key.no);
}

static void jurorhis_year(mstr &m, TGroup *group) { itom(jurorhis->key.year, m); }
static void jurorhis_title(mstr &m, TGroup *group) { m.cat(jurorhis->title); }

#undef jurorhis

const offield TJurorHistory::fields[] =
{
	{ "F_ACTION",    jurorhis_action },
	{ "F_JUROR",     jurorhis_juror },
	{ "F_USER",      jurorhis_user },
	{ "F_DATE",      jurorhis_date },
	{ "F_TIME",      jurorhis_time },
	{ "F_MOTIVES",   jurorhis_motives },
	{ "F_LAW_KIND",  jurorhis_law_kind },
	{ "F_NO",        jurorhis_no },
	{ "F_YEAR",      jurorhis_year },
	{ "F_USER_POST", jurorhis_user_post },
	{ "F_USER_NAME", jurorhis_user },
	{ "F_TITLE",     jurorhis_title },
	{ NULL, NULL }
};

// ----- TDistribJuror --------------------------------------------------------------
enum
{
	DELTA_FAR  = 2000,
	DELTA_HUGE = 200000,
	DELTA_MOD  = 1000,
	DELTA_RND  = 950  // 2011:313 LPR: 20 -> 950
};

// 2010:279 LPR: TKnownPerson -> TKnownAliasGroup
class TDistribJuror : public TKnownAliasGroup
{
public:
	TDistribJuror();

	bool Present(const TBaseSession *session) const;
	bool operator<=(const TDistribJuror &that) const;

	long delta;     // minutes to the closest same day session or 2K
	long reserved;  // 2012:269 SGS: juror reserving +related
};

TDistribJuror::TDistribJuror()
{
	Add(
		new TLong("F_DELTA", &delta),
		new TLong("F_RESERVED", &reserved),
		NULL
	);

	involvement = INVOLVEMENT_JUROR;
}

bool TDistribJuror::Present(const TBaseSession *session) const
{
	constant jurors[JUROR_COUNT] = { session->juror, session->reJuror, session->jurorX, session->reserve };

	for (int i = 0; i < JUROR_COUNT; i++)
		if (!strcmp(jurors[i], ucn))
			return true;
			
	return false;
}

bool TDistribJuror::operator<=(const TDistribJuror &that) const
{
	// '>' is not a mistake, we want reserved jurors sorted at start of the list
	return reserved != that.reserved ? reserved > that.reserved : delta <= that.delta;
}

// ----- TJurorDistribution --------------------------------------------------
class TJurorDistribution : public TOutput
{
public:
	TJurorDistribution(TWindow *parent, TJurorDistrib *tDistrib, TBaseSession *session, bool tEdit);
	~TJurorDistribution();

	TDistribJuror *Exclude(const char *ucn);
	const char *FindFirst(bool pedagog);
	bool Filter(TGArray<TDistribJuror> *jurors, bool pedagog);
	const TGSArray<TDistribJuror> *Jurors() { return &jurors; }
	const TGArray<TDistribJuror> *Avails() { return &avails; }
	void PrintJuror(const TDistribJuror *juror, const char *format, ...);

	TJurorDistrib *distrib;
	const CDate prevDate;
	bool edit;

protected:
	TGArray<TDistribJuror> avails;
	TGSArray<TDistribJuror> jurors;

	virtual void HandleRelated(const TConnect &connect);

	CDate dates[JUROR_DATES_MAX];
	int predates;

	bool AddDates(const TBaseSession *session);
	void AddIfNeq(TGArray<TBaseSession> &sessions, const TBaseSession *session, TBaseSession *tSession);
	void Load(TGArray<TBaseSession> &sessions, const TBaseSession *session, const TBaseSession &stemplate);
	void LoadCloseds(TGArray<TBaseSession> &sessions, const TBaseSession *session);
	void LoadJurors(const TGArray<TBaseSession> &sessions, const TBaseSession *session, int sesdates);
	void Exclude(const TBaseSession *session);
};

class TJurorExcludeHistory : public TJurorHistory
{
public:
	TJurorExcludeHistory(TJurorDistribution *tDistribution);

protected:
	TJurorDistribution *distribution;

	virtual void Event();
};

TJurorExcludeHistory::TJurorExcludeHistory(TJurorDistribution *tDistribution)
	: TJurorHistory(tDistribution->distrib), distribution(tDistribution)
{
}

void TJurorExcludeHistory::Event()
{
	if (oper == JUROR_DISTRIB_OLDISTRIB)
	{
		TDistribJuror *tJuror = distribution->Exclude(juror);

		if (tJuror)
			distribution->PrintJuror(tJuror, "преразпределение");
	}
	else if (index != JUROR_DISTRIB_NONE)
	{
		if (strchr(JUROR_DISTRIB_PHASE_OUTS, oper))
		{
			TDistribJuror *tJuror = distribution->Exclude(jurors[index]);

			if (tJuror)
				distribution->PrintJuror(tJuror, oper == JUROR_DISTRIB_REDISTRIB ? "преразпределение" : "изключване");
		}

		if (!distribution->edit)
			strcpy(distribution->distrib->jurors[index], juror);
	}
}

static void advance(CTime &entime, int minutes)
{
	if (TIME_LMAX - entime > minutes)
		entime += minutes;
	else
		entime = TIME_LMAX;
}

static void fix_end(TBaseSession *session)
{
	if (session->end.Empty())
		session->end = session->date;

	CTime &entime = session->entime;

	if (entime.Empty())
	{
		if (!DISTRIB_SESSION_LENGTH)
		{
			entime = TIME_LMAX;
			return;
		}

		entime = session->time;
		advance(entime, DISTRIB_SESSION_LENGTH);
	}

	if (!entime.Empty() && entime.minute % 5 == 0)	// ends "14:30" or "10:25"
		entime += -1;

	advance(entime, DISTRIB_SESSION_PAUSE);
}

static bool lesseq(const CDate &date1, const CTime &time1, const CDate &date2, const CTime &time2)
{
	return date1 < date2 || (date1 == date2 && time1 <= time2);
}

static bool inrange(const CDate &date, const CTime &time, const TBaseSession *session)
{
	return lesseq(session->date, session->time, date, time) && lesseq(date, time, session->end, session->entime);
}

static bool inrange(const CDate &date, const TBaseSession *session)
{
	return session->date <= date && date <= session->end;
}

TJurorDistribution::TJurorDistribution(TWindow *parent, TJurorDistrib *tDistrib, TBaseSession *session, bool tEdit)
	: TOutput(parent, "разпределението", WAIT), distrib(tDistrib), predates(0), prevDate(tDistrib->date), edit(tEdit)
{
	TGArray<TBaseSession> sessions;

	fix_end(session);
	if (!AddDates(session))
		fatal("Заседанието трае повече от %d дни! Не може да се извърши разпределение.", JUROR_DATES_MAX);
	int sesdates = predates;

	wait->SetText("Зареждане на заседания...");
	Load(sessions, session, TSession());
	Load(sessions, session, TExternSession());
	LoadCloseds(sessions, session);

	wait->SetText("Зареждане на заседатели...");
	dprintf("Изцяло изключени заседатели:\n\n");
	LoadJurors(sessions, session, sesdates);

	wait->SetText("Проверка за свързани дела...");
	TraceRelated(session->key, CONNECT_TYPE_RETURNED_LAWSUIT);
	TraceRelated(session->key, CONNECT_TYPE_RERETURNED_LAWSUIT);
	TraceRelated(session->key, CONNECT_TYPE_RENEWED_LAWSUIT);

	dprintf("\n\nИзключени от автоматичното предлагане:\n\n");
	Exclude(session);

	dprintf("\n\nОставащи за автоматично предлагане:\n\n");
	for (int i = 0; i < jurors.Count(); i++)
	{
		TDistribJuror *juror = jurors[i];
		PrintJuror(juror, "%s приоритет %ld", juror->delta < DELTA_FAR ? "фиксиран" : "променлив", juror->delta);
	}
}

TJurorDistribution::~TJurorDistribution()
{
	jurors.Clear();
}

bool TJurorDistribution::AddDates(const TBaseSession *session)
{
	for (CDate date = session->date; date <= session->end; date += 1)
	{
		int i;

		for (i = 0; i < predates; i++)
			if (dates[i] == date)
				break;

		if (i == predates)
		{
			if (predates == JUROR_DATES_MAX)
			{
			#if TESTVER
				dprintf("Участие по дати, включително с текущото заседание:\n");
				for (int i = 0; i < predates; i++)
					dprintf("%0D%s", &dates[i], i == predates - 1 ? "...\n" : i % 5 == 4 ? "\n" : ", ");
				dprintf("Препълване: %0D\n", &date);
				// already max dates, abort
			#endif  // TESTVER
				// already max dates, abort
				return false;
			}
			dates[predates++] = date;
		}
	}

	return true;
}

void TJurorDistribution::AddIfNeq(TGArray<TBaseSession> &sessions, const TBaseSession *session, TBaseSession *tSession)
{
	if (conteq(tSession, session) && tSession->Excess() == session->Excess())
		delete tSession;
	else
	{
		fix_end(tSession);
		sessions.Add(tSession);
		wait->TickCount(sessions.Count(), "заредени заседания");
	}
}

void TJurorDistribution::Load(TGArray<TBaseSession> &sessions, const TBaseSession *session, const TBaseSession &stemplate)
{
	// lots of sessions, so pinpoint the fields
	// 2010:075 FIX: "F_YEAR = " -> "F_DATE BETWEEN "
	// 2012:269 SGS: F_COMPOSITION for juror reserving
	msql m(stemplate.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_TIME, F_COMPOSITION, F_END, F_ENTIME, "
		JUROR_NAMES_UNQUOTED ", F_END, F_ENTIME");

	m.AddRange("F_DATE", CDate(1, 1, session->date.year).Value(), CDate(31, 12, session->date.year).Value());
	m.AddChars("NOT F_RESULT", RESULT_UNLINKEDS);  // 2017:087 TRQ
	TQuery q(str(m));
	while (q.Read())
	{
		TBaseSession *tSession = (TBaseSession *) stemplate.clone();
		*tSession << q;
		AddIfNeq(sessions, session, tSession);
	}
}

void TJurorDistribution::LoadCloseds(TGArray<TBaseSession> &sessions, const TBaseSession *session)
{
	TJurorDistrib distrib;
	msql m(distrib.Name);

	m.AddRange("F_DATE", CDate(1, 1, session->date.year).Value(), CDate(31, 12, session->date.year).Value());
	m.AddZLong("F_CLOSED", TRUE);
	TQuery q(str(m));
	while (q.Read())
	{
		distrib << q;
		TBaseSession *tSession = TBaseSession::NewGroup(distrib.excess);

		*tSession << distrib;
		tSession->kind = KIND_CLOSED_SESSION;
		AddIfNeq(sessions, session, tSession);
	}
}

void TJurorDistribution::LoadJurors(const TGArray<TBaseSession> &sessions, const TBaseSession *session, int sesdates)
{
	TGroupArray knowns;
	TDistribJuror juror;
	// 2010:279 LPR: FloatCriteria() -> load
	KnownAliasGroup->LoadData(&knowns, INVOLVEMENT_JURORS, FLAG_NULL);

	for (int known = 0; known < knowns.Count(); known++)
	{
		juror << knowns[known];
		juror.delta = DELTA_FAR;

		if (!(juror.flags & FLAG_ACTIVE))
		{
			PrintJuror(&juror, "неактивен");
			continue;
		}

		predates = sesdates;
		int index;

		for (index = 0; index < sessions.Count(); index++)
		{
			const TBaseSession *tSession = sessions[index];
			bool reserved = DISTRIB_RESERVE_JURORS &&
				// 2016:021 FIX: full overlapping check
				(inrange(session->date, tSession) || inrange(session->end, tSession) ||
				inrange(tSession->date, session) || inrange(tSession->end, session));

			if (!juror.Present(tSession))
				continue;

			if (!AddDates(tSession))
			{
				PrintJuror(&juror, "участва на %d дати", JUROR_DATES_MAX);
				break;
			}

			if (inrange(session->date, session->time, tSession) ||
				inrange(session->end, session->entime, tSession) ||
				inrange(tSession->date, tSession->time, session) ||
				inrange(tSession->end, tSession->entime, session))
			{
				PrintJuror(&juror, "застъпва %s от %D", Kind->Seek(tSession->kind), &tSession->date);
				break;
			}
			else if (reserved && session->composition != tSession->composition)
			{
				PrintJuror(&juror, "резервиран за %s състав", Composition->Seek(tSession->composition));
				break;
			}
			else
			{
				int delta = juror.delta;

				if (session->end == tSession->date)
				{
					if (session->date < tSession->date || session->entime < tSession->time)
						delta = tSession->time - session->entime;
					else
						delta = session->time - tSession->entime;
				}
				else if (session->date == tSession->end)
					delta = session->time - tSession->entime;

				if (delta < juror.delta)
					juror.delta = delta;

				if (reserved)
					juror.reserved = true;
			}
		}

		if (index < sessions.Count())
			continue;

		if (juror.delta == DELTA_FAR && DISTRIB_JURORS_LOAD)
			juror.delta = DELTA_HUGE + predates * DELTA_MOD;

		TDistribJuror *tJuror = new TDistribJuror;
		*tJuror << juror;
		avails.Add(tJuror);
		wait->TickCount(avails.Count(), "заредени заседатели");
	}
}

void TJurorDistribution::Exclude(const TBaseSession *session)
{
	//srand(time(NULL) + GetTickCount());
	long value = session->key.no % 1024;

	value = (value << 5) + (session->key.type - 'a');
	value = (value << 8) + session->kind;
	value = (value << 5) + session->date.day;
	value = (value << 4) + session->date.month;
	srand(value);

	for (int i = 0; i < avails.Count(); i++)
	{
		TDistribJuror *juror = avails[i];

		if (juror->delta >= DELTA_FAR)
			juror->delta = juror->delta - juror->delta % DELTA_MOD + (rand() % DELTA_RND);

		jurors.Add(juror);
	}

	// 2017:149 only if edit
	if (edit)
		TJurorExcludeHistory(this).Execute();

	for (int index = 0; index < JUROR_COUNT; index++)
	{
		if (!edit)
		{
			int i;

			for (i = 0; i < avails.Count(); i++)
				if (!strcmp(avails[i]->ucn, distrib->jurors[index]))
					break;

			if (i == avails.Count())
			{
				*distrib->jurors[index] = '\0';
				continue;
			}
		}

		TDistribJuror *juror = Exclude(distrib->jurors[index]);

		if (juror)
			PrintJuror(juror, edit ? "участва в момента" : "предложен в момента");
	}
}

void TJurorDistribution::PrintJuror(const TDistribJuror *juror, const char *format, ...)
{
	mstr m;
	va_list ap;

	if (juror->flags & JUROR_PEDAGOG)
		dprintf("п ");
	print_user(m, juror->ucn, juror->ucnType);
	dprintf("%s: ", str(m));

	m.clear();
	va_start(ap, format);
	m.vprintf(format, ap);
	va_end(ap);

	dprintf("%s\n", str(m));
}

TDistribJuror *TJurorDistribution::Exclude(const char *ucn)
{
	for (int i = 0; i < jurors.Count(); i++)
		if (!strcmp(jurors[i]->ucn, ucn))
			return jurors.Detach(i);

	return NULL;
}

const char *TJurorDistribution::FindFirst(bool pedagog)
{
	for (int i = 0; i < jurors.Count(); i++)
	{
		TDistribJuror *juror = jurors[i];

		if (!pedagog || (juror->flags & JUROR_PEDAGOG))
			return juror->ucn;
	}

	return NULL;
}

bool TJurorDistribution::Filter(TGArray<TDistribJuror> *jurors, bool pedagog)
{
	for (int i = 0; i < avails.Count(); i++)
	{
		TDistribJuror *juror = avails[i];

		if (!pedagog || (juror->flags & JUROR_PEDAGOG))
		{
			int index;

			for (index = 0; index < JUROR_COUNT; index++)
				if (!strcmp(distrib->jurors[index], juror->ucn))
					break;

			if (index == JUROR_COUNT)
			{
				if (!jurors)
					return true;

				jurors->Add(juror);
			}
		}
	}

	return jurors && jurors->Count();
}

void TJurorDistribution::HandleRelated(const TConnect &connect)
{
	TJurorDistrib tDistrib;

	tDistrib.key.no = atoi(connect.connectNo);
	tDistrib.key.year = connect.connectYear;
	tDistrib.key.type = connect.key.type;
#if 20170149
	if (!tDistrib.key.no)
		return;

	msql m;

	tDistrib.FloatCriteria(m);
	TQuery q(str(m));
	while (q.Read())
	{
		tDistrib << q;
#else  // 20170149
	if (tDistrib.key.no && tDistrib.Try())
	{
#endif  // 20170149
		for (int juror = 0; juror < JUROR_COUNT; juror++)
		{
			for (int index = 0; index < avails.Count(); index++)
			{
				TDistribJuror *avail = avails[index];

				if (!strcmp(avail->ucn, tDistrib.jurors[juror]))
				{
					PrintJuror(avail, "свързано дело, отхвърлен");
					avails.Remove(index);
					break;
				}
			}
		}
	}
}

// ----- functions -----------------------------------------------------------
#include "JurorMotivesDialog.h"
#include "JurorMotivesDialog.cpp"

#include "SelectJurorListBox.h"
#include "SelectJurorListBox.cpp"

#include "JurorSelectDialog.h"
#include "JurorSelectDialog.cpp"

#include "JurorDistribDialog.h"
#include "JurorDistribDialog.cpp"

void distrib_jurors_ll(TWindow *parent, TJurorDistrib &distrib, TBaseSession *tSession, TBaseSession *session,
	const TLawsuit *lawsuit, bool edit)
{
	log_juror_double(LOG_DISTRIB, 0, session, "на заседатели - начало", JUROR_DISTRIB_BEGIN);

	*tSession << session;
	TJurorDistribution distribution(parent, &distrib, tSession, edit);
	distrib.date = session->date;

	delete distribution.wait;
	distribution.wait = NULL;

	if (!edit && distribution.Avails()->Count() < 2)
	{
		error("За начално разпределение са необходими поне двама заседатели.\nПрегледайте справката.");
		distribution.Display();
	}
	else
	{
		switch (TJurorDistribDialog(parent, &distribution, session, lawsuit).Execute())
		{
			case IDOK :
			{
				log_juror_double(LOG_DISTRIB, 0, session, "на заседатели - потвърждение", JUROR_DISTRIB_OK);

				if (!distrib.closed)
				{
					session->ImportFields(&distrib, JUROR_NAMES_UNQUOTED);
					session->Update(JUROR_NAMES_UNQUOTED);
				}
				break;
			}
			case IDCANCEL :
			{
				log_juror_double(LOG_DISTRIB, 0, session, "на заседатели - прекъсване", JUROR_DISTRIB_CANCEL);
				break;
			}
			case IDDELETE : delete_distrib(session);  // logged by delete_distrib()
		}
	}
}

void new_distrib_jurors(TWindow *parent, TBaseSession *session, const TLawsuit *lawsuit)
{
	TJurorDistrib distrib;
	bool exist = distrib.Find(session, false);
	TBaseSession *const tSession = (TBaseSession *) session->clone();
	TVirtPtr VP(tSession);
	constant jurors[JUROR_COUNT] = { tSession->juror, tSession->reJuror, tSession->jurorX, tSession->reserve };

	if (exist)
	{
		mstr m("По делото вече има разпределени съдебни заседатели");

		if (distrib.closed)
			m.printf("за %s от %Dг", Kind->Seek(KIND_CLOSED_SESSION), &session->date);

		if (!ask("%s.\n\nЖелаете ли да извършите ново разпределение?", str(m)))
			return;

		for (int index = 0; index < JUROR_COUNT; index++)
			*distrib.jurors[index] = '\0';
	}
	else if (!distrib.closed)
	{
		msql m(tSession->Name, JUROR_NAMES_UNQUOTED);

		m.Add(session, "F_NO, F_YEAR, F_TYPE");
		m.AddChars("F_KIND", KIND_OPEN_SESSIONS);
		TQuery q(str(m));

		while (q.Read())
		{
			*tSession << q;

			if (atobc(jurors, JUROR_COUNT))
			{
				if (!ask("По делото има открито заседание със съдебни заседатели. Ако извършите разпределение, "
					"няма да имате възможност да въвеждате ръчно заседатели. %s", Q_CONTINUE))
				{
					return;
				}

				break;
			}
		}
	}

	distrib_jurors_ll(parent, distrib, tSession, session, lawsuit, false);
}

void edit_distrib_jurors(TWindow *parent, TBaseSession *session, const TLawsuit *lawsuit)
{
	TJurorDistrib distrib;

	if (distrib.Find(session, true))
	{
		TBaseSession *const tSession = (TBaseSession *) session->clone();
		TVirtPtr VP(tSession);
		distrib_jurors_ll(parent, distrib, tSession, session, lawsuit, true);
	}
}

// ----- TJurorPrintHistory --------------------------------------------------
class TJurorPrintHistory : public TJurorHistory
{
public:
	TJurorPrintHistory(const TLawsuit *tLawsuit, const char *tTitle, TJurorDistrib *tDistrib);

	void Execute();

protected:
	TJurorDistrib *distrib;

	TOffice office;
	const char *tr1header;
	const char *tr1start;
	const char *tr1final;

	static const offield currents[];
	static const offield headers[];

	void History();
	virtual void Event();
};

TJurorPrintHistory::TJurorPrintHistory(const TLawsuit *tLawsuit, const char *tTitle, TJurorDistrib *tDistrib)
	: TJurorHistory(tDistrib), distrib(tDistrib)
{
	lawsuit = tLawsuit;
	title = tTitle;
}

#define jurorhis OFFICE_GROUP_CAST(TJurorHistory)

static void printjrs_action(mstr &m, TGroup *group)
{
	const char *name;

	switch (jurorhis->index)
	{
		case JUROR_DISTRIB_JUROR : name = "Първи заседател"; break;
		case JUROR_DISTRIB_REJUROR : name = "Втори заседател"; break;
		case JUROR_DISTRIB_JUROR_X : name = "Трети заседател"; break;
		case JUROR_DISTRIB_RESERVE : name = "Резервен заседател"; break;
		default : name = "";
	}

	m.cat(name);
}

static void printjrs_juror(mstr &m, TGroup *group)
{
	if (jurorhis->index != JUROR_DISTRIB_NONE)
		print_juror(m, jurorhis->juror, NO_PREFIX);
}

static void printjrs_session(mstr &m, TGroup *group)
{
	if (jurorhis->kind)
	{
		m.printf("%s %Dг.", strchr(KIND_OPEN_SESSIONS, jurorhis->kind) ? "ОЗС" : "ЗЗ/РЗ", &jurorhis->date);

		TBaseSession *session = TSession::NewGroup(jurorhis->excess);
		TVirtPtr VP(session);

		session->key = jurorhis->key;
		session->kind = jurorhis->kind;
		session->date = jurorhis->date;

		if (session->Try("F_TIME"))
		{
			// 2017:170
			//m.printf(", %Tч.", &session->time);
			session->time.Print(m, ", %Tч.");
		}
	}
	else if (!jurorhis->date.Empty())
		m.printf("%Dг.", &jurorhis->date);
}

#undef jurorhis

const offield TJurorPrintHistory::currents[] =
{
	{ "F_ACTION", printjrs_action },
	{ "F_JUROR",  printjrs_juror },
	{ NULL, NULL }
};

const offield TJurorPrintHistory::headers[] =
{
	{ "F_HEADER", printjrs_session },
	{ NULL, NULL },
};

void TJurorPrintHistory::History()
{
	msql m;
	char lastKind = '\0';
	CDate lastDate;

	memset(jurors, '\0', sizeof jurors);
	FloatCriteria(m);
	m.CutOrder();
	m.AddOrder("F_DATE, F_KIND, F_JUROR_LOG");

	TQuery q(str(m));
	while (q.Read())
	{
		*this << q;
		CheckMD5();

		if (kind != lastKind || date != lastDate)
		{
			office.Emit(tr1header, tr1start, headers, this);
			lastKind = kind;
			lastDate = date;
		}

		Event();

		if (index != JUROR_DISTRIB_NONE)
			strcpy(jurors[index], juror);
	}
}

void TJurorPrintHistory::Execute()
{
	if (office.Open("QueryJuror.htm"))
	{
		tr1header = office.Find("tr1 header");
		tr1start = office.Find("tr1 start");
		tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1header, fields, this);
		History();

		for (index = 0; index < JUROR_COUNT; index++)
			if (atob(distrib->jurors[index]))
				break;

		if (index < JUROR_COUNT)
		{
			index = JUROR_DISTRIB_NONE;
			office.Emit(tr1start, tr1final, currents, this);

			for (index = 0; index < JUROR_COUNT; index++)
			{
				strcpy(juror, distrib->jurors[index]);

				if (atob(juror))
					office.Emit(tr1start, tr1final, currents, this);
			}
		}

		office.Emit(tr1final, NULL, fields, this);
		office.Show(NULL);
	}
}

void TJurorPrintHistory::Event()
{
	office.Emit(tr1start, tr1final, fields, this);
}

// ----- TJurorLoadGroup -----------------------------------------------------
class TJurorLoadGroup : public TFloatGroup
{
public:
	TJurorLoadGroup();

	void ExternCriteria(msql &m);
	virtual void FormCriteria(msql &m);
	void SessionCriteria(msql &m);

	CDate minDate;
	CDate maxDate;
	long explain;
	TGroupArray jurors;
	int jurorMark;

protected:
	void CommonCriteria(msql &m);
};

TJurorLoadGroup::TJurorLoadGroup()
	: TFloatGroup("T_SESSION")
{
	Add(
		new TDate("F_MIN_DATE", &minDate, false),
		new TDate("F_MAX_DATE", &maxDate, false),
		new TCheck("F_EXPLAIN", &explain, TRUE),
		NULL
	);
}

void TJurorLoadGroup::CommonCriteria(msql &m)
{
	if (DISTRIB_SESSION_TIMES == 1)	// 2017:075 include closeds
		m.AddChars("J.F_KIND", KIND_OPEN_SESSIONS);  // 2013:059 URQ/LPR: ignore closeds
	m.AddRange("J.F_START", CDateTime(minDate.day, minDate.month, minDate.year, 0, 0, 0).Value(),
		CDateTime(maxDate.day, maxDate.month, maxDate.year, 23, 59, 0).Value());
	SessionCriteria(m);
}

void TJurorLoadGroup::ExternCriteria(msql &m)
{
	m.Begin("T_SESSION_TIMES J JOIN T_EXTERN_SESSION S ON S.F_NO = J.F_NO AND S.F_YEAR = J.F_YEAR "
		"AND S.F_TYPE = J.F_TYPE AND S.F_KIND = J.F_KIND AND S.F_DATE = J.F_DATE",
		// 2017:177 plain text -> JUROR_NAMES
		//"J.F_START, J.F_ENTIME, J.F_NO, J.F_YEAR, J.F_TYPE, J.F_DATE, J.F_HOURS, S.F_JUROR, S.F_REJUROR, "
		//"S.F_JUROR_X, S.F_RESERVE, S.F_COMPOSITION, S.F_RESULT, J.F_EXCESS, S.F_JUDGE, S.F_SECRETAR");
		"J.F_START, J.F_ENTIME, J.F_NO, J.F_YEAR, J.F_TYPE, J.F_DATE, J.F_HOURS, "
		JUROR_NAMES_QUOTED("S.") ", S.F_COMPOSITION, S.F_RESULT, J.F_EXCESS, S.F_JUDGE, S.F_SECRETAR");
	CommonCriteria(m);
}

void TJurorLoadGroup::FormCriteria(msql &m)
{
	m.Begin("T_SESSION_TIMES J JOIN T_SESSION S ON S.F_NO = J.F_NO AND S.F_YEAR = J.F_YEAR AND S.F_TYPE = J.F_TYPE "
		"AND S.F_KIND = J.F_KIND AND S.F_DATE = J.F_DATE "
		"JOIN T_LAWSUIT L ON L.F_NO = J.F_NO AND L.F_YEAR = J.F_YEAR AND L.F_TYPE = J.F_TYPE",
		// 2017:177 plain text -> JUROR_NAMES
		//"J.F_START, J.F_ENTIME, J.F_NO, J.F_YEAR, J.F_TYPE, J.F_DATE, J.F_HOURS, S.F_JUDGE, S.F_SECRETAR, S.F_JUROR, "
		//"S.F_REJUROR, S.F_JUROR_X, S.F_RESERVE, S.F_COMPOSITION, L.F_KIND AS F_LAW_KIND, S.F_RESULT, J.F_EXCESS");
		"J.F_START, J.F_ENTIME, J.F_NO, J.F_YEAR, J.F_TYPE, J.F_DATE, J.F_HOURS, S.F_JUDGE, S.F_SECRETAR, "
		JUROR_NAMES_QUOTED("S.") ", S.F_COMPOSITION, L.F_KIND AS F_LAW_KIND, S.F_RESULT, J.F_EXCESS");
	CommonCriteria(m);
}

void TJurorLoadGroup::SessionCriteria(msql &m)
{
#if 20170174
	static constant JUROR_NAMES[] = { "S.F_JUROR", "S.F_REJUROR", "S.F_JUROR_X", "S.F_RESERVE", NULL };

	if (jurorMark)
		m.AddJuSet(JUROR_NAMES, jurors, "F_UCN", true);
	else
		m.AddNnSet(JUROR_NAMES);
#else  // 20170174
	if (jurorMark)
		m.AddTriad("S.F_JUROR", "S.F_REJUROR", "S.F_JUROR_X", jurors, "F_UCN", true);
	else
	{
		m.Add("(NOT S.F_JUROR");
		m.cat("IS NULL OR NOT S.F_REJUROR IS NULL OR NOT S.F_JUROR_X IS NULL)");
	}
#endif  // 20170174
	m.AddChars("NOT S.F_RESULT", RESULT_UNCARRIEDS);
}

// ----- TJurorLoadTimes -----------------------------------------------------
class TJurorLoadTimes : public TSession
{
public:
	TJurorLoadTimes();

	const char *ExcessText() const { return TSession::ExcessText(excess); }
	bool operator<=(const TJurorLoadTimes &that) const;

	char lawKind;
	CDateTime start;
	double price;
	long realHours;
	long corrHours;
	long userHours;
	double sum;
	long excess;
};

TJurorLoadTimes::TJurorLoadTimes()
{
	Add(
		new TCharAlias("F_LAW_KIND", &lawKind),
		new TDateTime("F_START", &start),
		new TMoney("F_PRICE", &price),
		new TLong("F_REAL_HOURS", &realHours),
		new TLong("F_CORR_HOURS", &corrHours),
		new TMoney("F_SUM", &sum),
		new TLong("F_HOURS", &userHours),		// 2016:286
		new TZLong("F_EXCESS", &excess),
		NULL
	);
}

bool TJurorLoadTimes::operator<=(const TJurorLoadTimes &that) const
{
	//m.AddOrder("J.F_START, J.F_ENTIME, J.F_TYPE, J.F_YEAR, J.F_NO");
	return start != that.start ? start < that.start : entime != that.entime ? entime < that.entime :
		key.type != that.key.type ? key.type < that.key.type : key.year != that.key.year ? key.year <= that.key.year :
		key.no != that.key.no ? key.no < that.key.no : excess <= that.excess;
}

#define jtimes OFFICE_GROUP_CAST(const TJurorLoadTimes)

static void jtimes_law_kind(mstr &m, TGroup *group)
{
	if (jtimes->lawKind)
		m.printf(Kind->Seek(jtimes->lawKind));
	else
		m.printf(Type->Seek(jtimes->key.type));
}

static void jtimes_key_no(mstr &m,TGroup *group) { m.printf("%s%ld", jtimes->ExcessText(), jtimes->key.no); }
static void jtimes_composition(mstr &m, TGroup *group) { m.cat(Composition->Seek(jtimes->composition)); }
static void jtimes_ses_day(mstr &m, TGroup *group) { ((const CDate &) jtimes->start).Print(m, "%D"); }
static void jtimes_start(mstr &m, TGroup *group) { ((const CTime &) jtimes->start).Print(m, "%T"); }

static void jtimes_secretar(mstr &m, TGroup *group)
{
	if (atob(jtimes->secretar))
		print_name(m, jtimes->secretar, UCN_CITIZEN_UCN);
}

static void jtimes_judge(mstr &m, TGroup *group)
{
	if (atob(jtimes->judge))
		print_name(m, jtimes->judge, UCN_CITIZEN_UCN);
}

static void jtimes_remark(mstr &m, TGroup *group) { m.cat(Result->Seek(jtimes->result)); }

static void jtimes_calc_sum(mstr &m, TGroup *group)
{
	if (jtimes->realHours)
		m.printf("%ldч. * %.2lf лв./ч = %.2lf лв.", jtimes->corrHours, jtimes->price, jtimes->sum);
}

static void jtimes_explain(mstr &m, TGroup *group)
{
	if (jtimes->corrHours != jtimes->realHours)
		m.cat("\v/отложено - фиксирана продължителност/");
}

#undef jtimes

static const offield jtimes_fields[] =
{
	{ "F_LAW_KIND",    jtimes_law_kind },
	{ "F_NO",          jtimes_key_no },
	{ "F_YEAR",        NULL },
	{ "F_COMPOSITION", jtimes_composition },
	{ "F_DATE",        NULL },
	{ "F_SES_DAY",     jtimes_ses_day },
	{ "F_START",       jtimes_start },
	{ "F_ENTIME",      NULL },
	{ "F_REAL_HOURS",  NULL },
	{ "F_SECRETAR",    jtimes_secretar },
	{ "F_JUDGE",       jtimes_judge },
	{ "F_REMARK",      jtimes_remark },
	{ "F_CALC_SUM",    jtimes_calc_sum },
	{ "F_EXPLAIN",     jtimes_explain },
	{ NULL, NULL }
};

class TJurorLoadQuery : public TFloatQuery
{
public:
	TJurorLoadQuery(TWindow *parent, const char *title, TJurorLoadGroup *tGroup);

	bool RegularJudge() { return judges.Count() == 1 && atob(judges[0]); }

	virtual void Execute();

	TKnownPerson *juror;
	CDate sesDay;
	double price;
	long dayHours;
	bool dayMax8h;

	double daySum;
	bool dayMin20l;
	double periodSum;
	TStringArray judges;  // 2016:081 +related
	TJurorLoadGroup *group;

protected:
	TGSArray<TJurorLoadTimes> array;

	bool JurorMatch(const TJurorLoadTimes *jtimes);
	void CheckMissingTimes(TBaseSession &session);
	void LoadJurorTimes(const char *criteria);
	virtual void Count();
};

TJurorLoadQuery::TJurorLoadQuery(TWindow *parent, const char *title, TJurorLoadGroup *tGroup)
	: TFloatQuery(parent, NULL, "JurorLoad.htm", title), group(tGroup)
{
	Add(
		new TDate("F_SES_DAY", &sesDay, false),
		new TMoney("F_PRICE", &price),
		new TMoney("F_DAY_SUM", &daySum),
		new TMoney("F_PERIOD_SUM", &periodSum),
		NULL
	);
}

static void print_name(mstr &m, const char *ucn)
{
	TCitizen citizen;

	strcpy(citizen.ucn, ucn);
	citizen.ucnType = UCN_CITIZEN_UCN;

	if (citizen.Get())
	{
		m.cat(citizen.name);
		if (strlen(citizen.reName) > 2)
			strcpy(citizen.reName + 1, ".");
		if (any(citizen.reName))
			m.printf(" %s", citizen.reName);
		m.printf(" %s", citizen.family);
		if (any(citizen.reFamily))
			m.printf("-%s", citizen.reFamily);
	}
	else
		print_missing(m, UCN_CITIZEN_UCN, ucn);
}

#define loadjur OFFICE_GROUP_CAST(TJurorLoadQuery)

static void loadjur_period(mstr &m, TGroup *group) { m.cat(loadjur->title); }

static void loadjur_global_explain(mstr &m, TGroup *group)
{
	if (loadjur->group->explain)
	{
		m.printf("\vОтложените заседания след %D ", &JUROR_MIN_CORR_DATE);

		if (DISTRIB_SESSION_7302)
		{
			m.printf("се приемат за %ld часа", JUROR_LEFT_SES_HOURS);

			if (DISTRIB_SESSION_7302 == 2)
				m.printf(", освен за \"%s\"", Result->Seek(RESULT_LEFT_BULWARK));
		}
		else
			m.printf("са с реална продължителност");

		if (DISTRIB_SESSION_MAX8H)
			m.printf("\vМаксимум %ld часа на ден", JUROR_MAX_DAY_HOURS);
		else
			m.cat("\vБез максимален брой часове на ден");

		if (DISTRIB_SESSION_MIN20L)
			m.printf("\vМинимум %.2lf лв./ден след %D", JUROR_MIN_DAY_MONEY, &JUROR_MIN_CORR_DATE);
		else
			m.cat("\vБез минимално възнаграждение на ден");
	}
}

static void loadjur_juror(mstr &m, TGroup *group)
{
	constant ucn = loadjur->juror->ucn;

	print_name(m, ucn);

	if (PRINT_UCN && is_real_ucn(UCN_CITIZEN_UCN, ucn))
		m.printf(", ЕГН %s", ucn);
}

static void loadjur_explain(mstr &m, TGroup *group)
{
	if (loadjur->dayMax8h || loadjur->dayMin20l)
	{
		m.cat("\v/");

		if (loadjur->dayMax8h)
		{
			m.cat("маскимален брой часове/ден");

			if (loadjur->dayMin20l)
				m.cat(", ");
		}

		if (loadjur->dayMin20l)
			m.cat("минимално възнаграждение на ден");

		m.cat("/");
	}
}

static void loadjur_user_name(mstr &m, TGroup *) { print_name(m, Default->ucn); }

static void loadjur_president_label(mstr &m, TGroup *group)
{
	if (loadjur->RegularJudge())
		m.cat("СЪДИЯ-ДОКЛАДЧИК");
	else
	{
		m.cat("ПРЕДСЕДАТЕЛ ");
		generic_court_name(m, group);
	}
}

static void loadjur_president(mstr &m, TGroup *group)
{
	if (loadjur->RegularJudge())
		print_name(m, loadjur->judges[0]);
	else
	{
		TGArray<TUserAliasGroup> array;
		UserAliasGroup->LoadData(&array, C2S[POST_CHIEF], FLAG_ACTIVE);

		if (array.Count())
			print_name(m, array[0]->ucn);
	}
}

#undef loadjur

static const offield loadjur_fields[] =
{
	{ "F_PERIOD",          loadjur_period },
	{ "F_GLOBAL_EXPLAIN",  loadjur_global_explain },
	{ "F_JUROR",           loadjur_juror },
	{ "F_SES_DAY",         NULL },
	{ "F_DAY_SUM",         NULL },
	{ "F_EXPLAIN",         loadjur_explain },
	{ "F_PERIOD_SUM",      NULL },
	{ "F_USER_NAME",       loadjur_user_name },
	{ "F_PRESIDENT_LABEL", loadjur_president_label },
	{ "F_PRESIDENT",       loadjur_president },
	{ NULL, NULL }
};

static int CHours(const CTime &startime, const CTime &entime)
{
	int minutes = entime.Value() - startime.Value();
	return minutes <= 0 ? 1 : (minutes + 59) / 60;
}

void TJurorLoadQuery::Execute()
{
	if (!office.Open(name, OFFICE_COPY_HEADER))
		return;

	mstr m;
	m.printf("за заседатели, 7302=%ld MAX8H=%ld MIN20L=%ld", DISTRIB_SESSION_7302, DISTRIB_SESSION_MAX8H,
		DISTRIB_SESSION_MIN20L);
	log(LOG_QUERY, CM_QUERY_JUROR_LOAD, str(m));

	office.Emit(NULL, NULL, loadjur_fields, this);
	office.Close();

	TWindowHideParent hide(parent);

	if (office.Open(name, OFFICE_SKIP_HEADER, ""))
	{
		constant jur_start = office.Find("jur_start");
		constant ses_day = office.Find("ses_day");
		constant reg_line = office.Find("reg_line");
		constant start_line = office.Find("start_line");
		constant end_line = office.Find("end_line");
		constant day_sum = office.Find("day_sum");
		constant per_sum = office.Find("per_sum");
		constant table_end = office.Find("table_end");
		constant jur_end = office.Find("jur_end");

		bool total_start = true;
		TGSArray<TSessionPrice> prices;

		Count();
		wait->SetText(WAIT);
		office.Emit(NULL, jur_start, null_fields, NULL);

		for (int i = 0; i < group->jurors.Count(); i++)
		{
			bool juror_start = true;

			juror = (TKnownPerson *) (group->jurors)[i];
			periodSum = 0;

			// a bit stupid, but wastes ~0 time, and works with single footer
			for (sesDay = group->minDate; sesDay <= group->maxDate; sesDay += 1)
			{
				bool day_start = true;
				bool corr_date = sesDay >= JUROR_MIN_CORR_DATE;
				CTime start;

				dayHours = 0;
				daySum = 0.0;

				for (int n = 0; n < array.Count(); n++)
				{
					TJurorLoadTimes *jtimes = array[n];

					if ((const CDate &) jtimes->start != sesDay || !JurorMatch(jtimes))
						continue;

					// matching jtimes found

					if (day_start)
					{
						if (juror_start)
						{
							if (total_start)
								total_start = false;
							else
								office.EmitBreak();

							office.Emit(jur_start, ses_day, loadjur_fields, this);
							juror_start = false;
						}

						office.Emit(ses_day, reg_line, loadjur_fields, this);
						day_start = false;

						// get day price

						SessionPrice->date = sesDay;
						int index = prices.Find(SessionPrice);

						if (index != INT_MAX)
							price = prices[index]->price;
						else
						{
							msql m(SessionPrice->Name, "F_PRICE");
							TSessionPrice *tPrice = new TSessionPrice;
							tPrice->date = sesDay;

							m.Add("F_DATE");
							m.printf("<= %d", sesDay.Value());
							m.AddOrder("F_DATE DESC");
							TQuery q(str(m));

							if (q.Read())
								*tPrice << q;
							else
								dprintf("Няма часова ставка за %D\n", &sesDay);

							prices.Add(tPrice);
							price = tPrice->price;
						}
					}

					const char *line_start = reg_line;
					const char *line_final = start_line;

					if (corr_date)
					{
						if (jtimes->userHours)
							jtimes->realHours = jtimes->userHours;
						else
						{
							dprintf("%s от %D по %s %s%ld/%ld: няма въведен брой часове за %D\n",
								KindAlias->Seek(jtimes->kind), &jtimes->date, Type->Seek(jtimes->key.type),
								jtimes->ExcessText(), jtimes->key.no, jtimes->key.year, &sesDay);
						}
					}
					else
					{
						TJurorLoadTimes *jnexts;
						bool next_found = false;

						for (int k = n + 1; k < array.Count() && !next_found; k++)
						{
							jnexts = array[k];

							if ((const CDate &) jnexts->start != sesDay)
								break;

							next_found = JurorMatch(jnexts);
						}

						CTime entime = jtimes->entime;

						if (!entime.Empty() && entime.minute % 5 == 0)  // ends "14:30" or "10:25"
							entime += -1;

						if (next_found && ((const CTime &) jnexts->start).Value() - entime.Value() < 60)
						{
							if (start.Empty())  // next but !previous, open interval
							{
								start = jtimes->start;
								line_start = start_line;
								line_final = end_line;
							}
							// else
							//	previous and next, insider
						}
						else
						{
							if (start.Empty())
							{	// no previous, no next, standalone
								jtimes->realHours = CHours(jtimes->start, entime);
							}
							else
							{	// previous but not next, close interval
								line_start = end_line;
								line_final = day_sum;
								jtimes->realHours = CHours(start, entime);
								start.Clear();
							}
						}
					}

					// jtimes
					jtimes->price = price;
					jtimes->corrHours = jtimes->realHours;

					if (corr_date && DISTRIB_SESSION_7302)
					{
						if (jtimes->result != RESULT_LEFT_BULWARK || DISTRIB_SESSION_7302 != 2)
						{
							int outcomes = session_outcomes(jtimes);

							if (outcomes == SESSION_LEFT || outcomes == SESSION_STOPPED)
								jtimes->corrHours = 2;
						}
					}

					jtimes->sum = jtimes->corrHours * price;
					office.Emit(line_start, line_final, jtimes_fields, jtimes);
					dayHours += jtimes->corrHours;

					{	// judges
						int i;

						for (i = 0; i < judges.Count(); i++)
							if (!strcmp(judges[i], jtimes->judge))
								break;

						if (i == judges.Count())
							judges.Add(strnew(jtimes->judge));
					}
				}

				if (day_start)  // no day data
					continue;

				if (DISTRIB_SESSION_MAX8H && dayHours > JUROR_MAX_DAY_HOURS)
				{
					dayHours = JUROR_MAX_DAY_HOURS;
					dayMax8h = true;
				}
				else
					dayMax8h = false;

				daySum = dayHours * price;

				if (corr_date && DISTRIB_SESSION_MIN20L && daySum < JUROR_MIN_DAY_MONEY)
				{
					dayMin20l = true;
					daySum = JUROR_MIN_DAY_MONEY;
				}
				else
					dayMin20l = false;

				office.Emit(day_sum, per_sum, loadjur_fields, this);
				periodSum += daySum;
			}

			if (juror_start)  // no juror data
				continue;

			office.Emit(per_sum, table_end, loadjur_fields, this);
			office.Emit(table_end, jur_end, loadjur_fields, this);
		}

		if (!total_start)
			office.Emit(jur_end, NULL, null_fields, NULL);

		Show();
	}
}

bool TJurorLoadQuery::JurorMatch(const TJurorLoadTimes *jtimes)
{
	constant jurors[JUROR_COUNT] = { jtimes->juror, jtimes->reJuror, jtimes->jurorX, jtimes->reserve };

	for (int i = 0; i < JUROR_COUNT; i++)
		if (!strcmp(jurors[i], juror->ucn))
			return true;

	return false;
}

void TJurorLoadQuery::CheckMissingTimes(TBaseSession &session)
{
	mstr name(session.Name);
	name.cat(" S");
	msql m(str(name), "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_KIND, S.F_DATE");

	m.AddRange("(S.F_DATE", group->minDate, group->maxDate);
	m.NextOr();
	m.AddRange("S.F_END", group->minDate, group->maxDate);
	m.cat(")");
	group->SessionCriteria(m);
	m.AddChars("((S.F_KIND", KIND_OPEN_SESSIONS);
	m.Add("S.F_ENTIME");
	m.printf("IS NULL) OR (SELECT COUNT(T.F_NO) FROM T_SESSION_TIMES T WHERE T.F_NO = S.F_NO AND T.F_YEAR = S.F_YEAR "
		"AND T.F_TYPE = S.F_TYPE AND T.F_KIND = S.F_KIND AND T.F_DATE = S.F_DATE AND T.F_EXCESS = %ld) = 0)",
		session.Excess());
	m.AddOrder("S.F_TYPE, S.F_YEAR, S.F_NO");  // 2014:212 LPR: was plain text

	TQuery r(str(m));
	while (r.Read())
	{
		session << r;
		dprintf("%s от %D по %s %s%ld/%ld: няма въведени данни за продължителност\n", KindAlias->Seek(session.kind),
			&session.date, Type->Seek(session.key.type), session.ExcessText(), session.key.no, session.key.year);
	}
}

void TJurorLoadQuery::LoadJurorTimes(const char *criteria)
{
	TQuery q(criteria);

	while (q.Read())
	{
		TJurorLoadTimes *jtimes = new TJurorLoadTimes;

		*jtimes << q;
		array.Add(jtimes);
		wait->TickCount(++count, "прочетени записа");
	}
}

void TJurorLoadQuery::Count()
{
	msql m;

	group->FormCriteria(m);
	LoadJurorTimes(str(m));
	group->ExternCriteria(m);
	LoadJurorTimes(str(m));

	{
		TSession session;
		CheckMissingTimes(session);
	}
	{
		TExternSession session;
		CheckMissingTimes(session);
	}
}

// ----- functions -----------------------------------------------------------
void query_juror_distrib(TWindow *, const TLawsuit *lawsuit, TJurorDistrib *distrib)
{
	mstr m;

	m.printf("%s %s%ld/%ldг.", lawsuit ? Kind->Seek(lawsuit->kind) : Type->Seek(distrib->key.type),
		TBaseSession::ExcessText(distrib->excess), distrib->key.no, distrib->key.year);
	if (lawsuit)
		m.printf(" %s състав", Composition->Seek(lawsuit->composition));
	TJurorPrintHistory(lawsuit, str(m), distrib).Execute();
}

void query_juror_distrib(TWindow *parent, const TExternSession *suggest, bool forceExcess)
{
	TExternContainer *const lawsuit = get_extern_container(parent, juror_logged, suggest, forceExcess);
	TVirtPtr VP(lawsuit);

	if (lawsuit)
	{
		TJurorDistrib distrib;
		TBaseSession *const session = TBaseSession::NewGroup(lawsuit->Excess());
		TVirtPtr VP(session);

		session->key = lawsuit->key;
		session->kind = *KIND_OPEN_SESSIONS;
		distrib.Find(session, false);
		query_juror_distrib(parent, lawsuit->Excess() ? NULL : (const TLawsuit *) lawsuit, &distrib);
	}
}

static void query_juror_load(TWindow *parent, TJurorLoadGroup *group)
{
	mstr title;
	title.printf("%D", &group->minDate);

	if (group->maxDate != group->minDate)
		title.printf(" - %D", &group->maxDate);

	TJurorLoadQuery(parent, str(title), group).Execute();
}

#include "JurorLoadListBox.h"
#include "JurorLoadListBox.cpp"

#include "JurorLoadDialog.h"
#include "JurorLoadDialog.cpp"

#if TESTVER
void query_juror_test(TWindow *parent)
{
	TJurorLoadGroup group;

	group.minDate = CDate(1, 6, 2016);
	group.maxDate = CDate(3, 10, 2016);
	TJurorLoadDialog(parent, &group, false).Execute();
}
#endif  // TESTVER

void query_juror_load(TWindow *parent)
{
	TJurorLoadGroup group;
	TJurorLoadDialog(parent, &group, true).Execute();
}
#endif  // AREAL || MILITARY || REGIONAL
