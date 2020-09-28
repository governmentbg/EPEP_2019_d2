#include "cac.h"

#define NEWQVER 1  // since 7507

static void back_5days(CDate &start)
{
	int i;
	start = Today;

	for (i = 0; i < 5; i++)
	{
		do
			start += -1;
		while (!start.Work());
	}
}

// ----- Unattached ----------------------------------------------------------
class TQueryUnattachedGroup : public TDummyGroup
{
public:
	TGArray<TUserAliasGroup> judges;
};

class TUnattachedStatGroup : public TDummyGroup
{
public:
	TUnattachedStatGroup();

	mstr judge;
	CDate from;
	long count;
	long remain;
};

TUnattachedStatGroup::TUnattachedStatGroup()
{
	Add(
		new TDate("F_FROM", &from, false),
		new TLong("F_COUNT", &count),
		new TLong("F_REMAIN", &remain),
		NULL
	);
}

#define unattach OFFICE_GROUP_CAST(TUnattachedStatGroup)

static void unattach_judge(mstr &m, TGroup *group) { m.cat(unattach->judge); }

static void unattach_from(mstr &m, TGroup *group)
{
	if (unattach->from.Empty())
		m.cat("Няма данни");
	else
		m.printf("%D", &unattach->from);
}

static void unattach_remain(mstr &m, TGroup *group)
{
	if (unattach->count)
		m.printf("%ld", unattach->remain);
}

static const offield unattach_fields[] =
{
	{ "F_JUDGE",	unattach_judge },
	{ "F_FROM",		unattach_from },
	{ "F_COUNT",	NULL },
	{ "F_REMAIN",	unattach_remain },
	{ NULL, NULL }
};

#undef unattach

static bool has_trouble_attachment(const TRCDKeyContainer *container)
{
	for (int type = 0; type < TROUBLE_COUNT; type++)
		if (electric_exist(container, type))
			return true;

	return false;
}

static bool valid_electrics()
{
	for (int type = 0; type < TYPE_COUNT; type++)
	{
		if (!any(Electrics[type].docketPath))
			return error("Директорията за %s липсва или е недостъпна / недефинирана.", Electrics[type].whats);

		if (!exist(Electrics[type].docketPath, false))
			return error("Директорията за %s липсва или е недостъпна.", Electrics[type].whats);
	}

	return true;
}

static void query_unattached(TWindow *parent, const TQueryUnattachedGroup &group)
{
	TOffice office;
	TAttachStat stat;
	TRCDKeyContainer container("T_CONTAINER", NULL);
	mstr w;

	if (office.Open("Unattached.htm"))
	{
		TWaitWindow wait(parent, WAIT);
		constant tr1start = office.Find("tr1 start");
		constant tr1final = office.Find("tr1 final");

		office.Emit(NULL, tr1start, unattach_fields, NULL);

		for (int i = 0; i < group.judges.Count(); i++)
		{
			const char *ucn = group.judges[i]->ucn;
			TUnattachedStatGroup unattach;
			msql m(stat.Name);

			print_user(unattach.judge, ucn);
			m.AddString("F_UCN", ucn);
			m.AddOrder("F_DATE DESC");

			w.printf("%s...", str(m));
			wait.TickText(str(w));

			TQuery q(str(m));
			while (q.Read())
			{
				stat << q;

				if (!unattach.count)
					unattach.from = stat.from;
				else if (stat.from != unattach.from)
					break;

				unattach.count++;
				container << stat;
				unattach.remain += !has_trouble_attachment(&container);
			}

			office.Emit(tr1start, tr1final, unattach_fields, &unattach);
		}

		office.Emit(tr1final, NULL, null_fields, NULL);
		office.Show(&wait);
	}
}

#include "QueryUnattachedDialog.h"
#include "QueryUnattachedDialog.cpp"

void query_unattached(TWindow *parent)
{
	if (valid_electrics())
	{
		TQueryUnattachedGroup group;

		if (TQueryUnattachedDialog(parent, &group).Execute() == IDOK)
			query_unattached(parent, group);
	}
}

// ----- TUnfilteredActsGroup ------------------------------------------------
class TUnfilteredActsGroup : public TDummyGroup
{
public:
	TUnfilteredActsGroup();

	void FormCriteria(msql &);

	char types[SIZE_OF_TYPES];
	CDate minDate;
	CDate maxDate;
};

TUnfilteredActsGroup::TUnfilteredActsGroup()
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TDate("F_MIN_DATE", &minDate, true),
		new TDate("F_MAX_DATE", &maxDate, true),
		NULL
	);

	back_5days(minDate);
}

void TUnfilteredActsGroup::FormCriteria(msql &)
{
	fatal("TUnfilteredActsGroup::FormCriteria");
}

#define container OFFICE_GROUP_CAST(TRCDKeyContainer)
#define surrouendoc OFFICE_GROUP_CAST(TSurroundment)

static void filter_attached(mstr &m, TGroup *group)
{
	if (electric_exist(container, TYPE_HTML))
		m.cat("О");
}

static void filter_lawsuit(mstr &m, TGroup *group)
{
	TRCDKey &key = strchr(KIND_SURROUNDS, container->kind) ? surrouendoc->lawsuit : container->key;

	Type->Seek(key.type);
	m.printf("%-5s  %5ld/%ld", Type->alias, key.no, key.year);
}

static void filter_decision_kind(mstr &m, TGroup *group)
{
	if (strchr(KIND_SURROUNDS, container->kind))
		m.cat(Kind->Seek(surrouendoc->decisionKind));
	else
		container_kind(m, group);
}

static void filter_decision_date(mstr &m, TGroup *group)
{
	if (strchr(KIND_SURROUNDS, container->kind))
		m.printf("%-D", &surrouendoc->decisionDate);
	else
		container_event_date(m, group);
}

#undef surrouendoc
#undef container

#define regact OFFICE_GROUP_CAST(TRegisterAct)

static void already_attached(mstr &m, TGroup *group)
{
	if (regact->targetKind)
		m.cat("Вп");
}

#undef regact

// ----- TFilterActSortArray -------------------------------------------------
class TFilterActSortArray : public TGSArray<TRCDKeyContainer>
{
public:
	TFilterActSortArray() { lesseql = LessEqlAct; }

protected:
	static bool LessEqlAct(const long value1, const long value2);
};

bool TFilterActSortArray::LessEqlAct(const long value1, const long value2)
{
	TRCDKeyContainer *container1 = (TRCDKeyContainer *) value1;
	TRCDKeyContainer *container2 = (TRCDKeyContainer *) value2;
	const CDate &date1 = container_event_date1(container1);
	const CDate &date2 = container_event_date1(container2);

	return date1 != date2 ? date1 < date2 : *container1 <= *container2;
}

static void filter_act(TWindow *parent, TRCDKeyContainer *container)
{
	char name[SIZE_OF_PATH];

	electric_name(container, name, TYPE_TEXT);
	filter_office(parent, container, container->date, name, true);

	// 2016:132 IRQ: add rider xfer web support
	TDecision *decision = strchr(KIND_ENDOCS, container->kind) ? (TDecision *) container :
		strchr(KIND_ENDOC_RIDERS, container->kind) ? ((TDecisionRider *) container) -> decision :
		NULL;

	if (decision && (decision->flags & DECISION_PUBLIC) && electric_exist(container, TYPE_HTML))
	{
		TSession session;
		TLawsuit lawsuit;

		session.key = lawsuit.key = decision->key;
		session.kind = decision->sessionKind;
		session.date = decision->date;

		if (session.Get() && lawsuit.Get())
		{
			xfer_web(decision, &session, &lawsuit);
			eportal_send(parent, decision, &lawsuit, LOG_WRITE);  // 2018-10-19
		}
	}
}

#include "FilterActListWindow.h"
#include "FilterActListWindow.cpp"

#include "FilterActListDialog.h"
#include "FilterActListDialog.cpp"

#include "UnfilteredActsDialog.h"
#include "UnfilteredActsDialog.cpp"

void unfiltered_acts(TWindow *parent)
{
	TUnfilteredActsGroup group;

	if (TUnfilteredActsDialog(parent, &group).Execute() == IDOK)
		TFilterActListDialog(parent, &group).Execute();
}

// ----- TRegisterActDialog --------------------------------------------------
#include "RegisterActDialog.h"
#include "RegisterActListWindow.h"

static bool special_match(const TRCDKeyContainer *container)
{
	if (!strchr(TYPE_LAWSUITS, container->key.type))
		return false;	// no rules defined

	TLawsuit *lawsuit = (TLawsuit *) container;

	if (!strcmp(lawsuit->judge, Default->ucn))
		return true;

	TSession session;
	msql m(session.Name, "F_KIND, F_DATE, F_TIME, F_JUDGE, F_RESULT");

	session.key = lawsuit->key;
	session.date = Today;
	session.time = Now();
	nearest_event_crit(m, &session, false, false);

	TSession *const tSession = TYPECHECK_PTR_CAST(TSession, nearest_event_find(m, &session, false));
	TVirtPtr VP_S(tSession);
	return tSession && (!strcmp(tSession->judge, Default->ucn) || tSession->result == RESULT_NON_JOIN_SETTL);
}

#include "RegisterActDialog.cpp"

static bool register_act(TRegisterActListWindow *parent, TRegisterAct *act, bool edit, const char *format)
{
	if (TRegisterActDialog(parent, act, edit).Execute() == IDOK)
	{
		char text[SIZE_OF_LOG_TEXT];

		sprintf(text, format, act->actNo);
		log(edit ? LOG_EDIT : LOG_NEW_DOC, CM_JUDGE_REGISTER_ACT, act, text);

		for (int type = 0; type < TROUBLE_COUNT; type++)
			mbk_clear(act->fileData + type);

		if (edit || !parent->DiffDate(act))
			return true;
	}

	return false;
}

bool attach_write_lock(const TRCDKeyContainer *container, char *name, int type, const memblock_t &data, long mtime)
{
	TUserFile uf(Electrics[type].what);

	electric_temp(container, name, type);

	if (exist(name))
	{
		struct stat statbuf;	// 2017:019 try to avoid rewrite

		if (stat(name, &statbuf) == 0 && statbuf.st_size == data.size && stat2utime(statbuf.st_ctime) == mtime &&
			stat2utime(statbuf.st_mtime) == mtime)
		{
			return true;
		}

		if (!unlock_delete_file(name, true))
			return false;
	}

	if (!uf.WriteAll(name, data.data, data.size, mtime))
		return false;

	if (!SetFileAttributes(name, FILE_ATTRIBUTE_READONLY))
		return error("%s: грешка %lu при смяна на правата за достъп.", name, GETLASTERROR);

	return true;
}

void attach_view(TWindow *parent, const char *name, int type)
{
	if (parent && matches_exts(name, "doc", "docx", NULL))
	{
		TWaitWindow wait(parent, IDD_WAIT_MUCH_MORE);

		if (filter_display(&wait, name, NULL, NULL))
		{
			wait.ResetForeground();
			return;
		}
	}

	electric_action(name, type, ACTION_VIEW);
}

#include "RegisterActListWindow.cpp"

#include "RegisterActListDialog.h"
#include "RegisterActListDialog.cpp"

static void print_select_act(mstr &m, const TRegisterAct *act)
{
	m.printf("%s от %Dг. на ", Kind->Seek(act->kind), &act->date);
	print_user(m, act->judge);
}

void register_acts(TWindow *parent)
{
	TRegisterActListDialog(parent).Execute();
}

#include "SelectRegisterActListWindow.h"
#include "SelectRegisterActListWindow.cpp"

#include "SelectRegisterActDialog.h"
#include "SelectRegisterActDialog.cpp"

void attach_right(TWindow *parent, const TRCDKey &key, TElectricTrouble &trouble, char indexKind, const CDate &date,
	TGArray<TUser> &judges)
{
	TRegisterAct act;
	TGArray<TRegisterAct> acts;
	msql m(act.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_JUDGE, F_ACT_NO, F_TEXT_MTIME, F_IMAGE_MTIME, "
		"F_TARGET_KIND, F_TEXT");
	mstr e;

	act.key = key;
	m.Add(&act, "F_NO, F_YEAR, F_TYPE");
	//m.AddChar("F_KIND", indexKind);
	//m.AddDate("F_DATE", date);
	//m.AddArray("F_UCN", judges, "F_JUDGE");
	// TODO? near dates instead of target
	m.AddIsNull("F_TARGET_KIND", true);

	TQuery q(str(m), act.DataBase);
	while (q.Read())
	{
		const char *s = "Неподходящ съдия";
		act << q;

		if (act.kind != indexKind)
			s = "Неподходящ вид на акта";
		else if (act.date != date)
			s = "Неподходяща дата";
		else if (act.targetKind)
			s = "Вече е присъединен";
		else
		{
			for (int i = 0; i < judges.Count(); i++)
			{
				if (!strcmp(act.judge, judges[i]->ucn))
				{
					s = NULL;
					break;
				}
			}
		}

		if (s)
		{
			e.sep(".\n");
			print_select_act(e, &act);
			e.printf(" - %s", s);
		}
		else
			acts.AddCopy(&act);
	}

	if (acts.Count())
		TSelectRegisterActDialog(parent, &trouble, &acts, e).Execute();
	else if (any(e))
	{
		error("Няма регистрирани съдебни актове, отговарящи на необходимите критерии.\n\n%s.", str(e));
		trouble.ClearAct();
	}
	else
		error("Няма регистрирани съдебни актове по %s.", key.type == TYPE_REQUEST ? "документа" : "делото");

	trouble.ActChanged();
}

void attach_right(TWindow *parent, TElectricTrouble &trouble, char indexKind, const CDate &date, const TSession *session)
{
	TGArray<TUser> judges;
	constant fields[4] = { session->judge, session->president, session->judgeX, session->reJudgeX };

	for (int i = 0; i < 4; i++)
	{
		if (atob(fields[i]))
		{
			TUser *user = new TUser;
			strcpy(user->ucn, fields[i]);
			judges.Add(user);
		}
	}

	attach_right(parent, session->key, trouble, indexKind, date, judges);
}

void attach_right(TWindow *parent, const TRCDKey &key, TElectricTrouble &trouble, char indexKind, const CDate &date,
	const char *judge)
{
	TGArray<TUser> judges;
	TUser *user = new TUser;

	strcpy(user->ucn, judge);
	judges.Add(user);
	attach_right(parent, key, trouble, indexKind, date, judges);
}

// ----- TQueryRegisterActGroup ----------------------------------------------
class TQueryRegisterActGroup : public TFloatGroup
{
public:
	TQueryRegisterActGroup();

	void ActCriteria(msql &m);
	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	long minNo;
	long maxNo;
	long minYear;
	long maxYear;
	TGArray<TUserAliasGroup> judges;
	CDate minDate;
	CDate maxDate;
};

TQueryRegisterActGroup::TQueryRegisterActGroup()
	: TFloatGroup("T_LOG")
{
	Add(
		new TCharList("F_TYPES", types, sizeof types),
		new TLong("F_MIN_NO", &minNo),
		new TLong("F_MAX_NO", &maxNo),
		new TLong("F_MIN_YEAR", &minYear),
		new TLong("F_MAX_YEAR", &maxYear),
		new TDate("F_MIN_DATE", &minDate, true),
		new TDate("F_MAX_DATE", &maxDate, true),
		NULL
	);

	back_5days(minDate);
	order = ORDER_BY_NULL;
}

void TQueryRegisterActGroup::ActCriteria(msql &m)
{
	m.AddChars("F_TYPE", types, TYPE_ORIGINS);
	m.AddRange("F_NO", minNo, maxNo);
	m.AddRange("F_YEAR", minYear, maxYear);
}

void TQueryRegisterActGroup::FormCriteria(msql &m)
{
	CDateTime minStamp(minDate.day, minDate.month, minDate.year, 0, 0, 0);
	CDateTime maxStamp(maxDate.day, maxDate.month, maxDate.year, 23, 59, 59);

	BeginCriteria(m);
	m.AddRange("F_STAMP", minStamp, maxStamp);
	ActCriteria(m);
	m.AddArray("F_UCN", judges);
#if NEWQVER
	m.AddChars("F_GOP", GOP_ATTACHES);
	m.AddLong("F_OPC", CM_JUDGE_REGISTER_ACT);
	m.AddLike("F_TEXT", " акт __", true);
#else  // NEWQVER
	m.AddChar("F_GOP", LOG_NEW_DOC);
	m.AddLong("F_OPC", CM_JUDGE_REGISTER_ACT);
	m.AddLike("F_TEXT", OF_JUDICIAL_ACT "__ +", false);
#endif  // NEWQVER
	m.AddOrder("F_STAMP");
}

// ----- TRegisterActLog -----------------------------------------------------
class TRegisterActLog : public TLog
{
public:
#if NEWQVER
	TRegisterActLog(const TLog &log, long tActNo);

	bool Matches(const TLog &that, long actNo) const;
	void Collect(const TLog &log);

	long actNo;
	TGArray<TLog> actions;
	CDateTime minStamp, maxStamp;
	bool finished, deleted;
#else  // NEWQVER
	TRegisterActLog();

	long subcount;
#endif  // NEWQVER
};

#if NEWQVER
TRegisterActLog::TRegisterActLog(const TLog &log, long tActNo)
	: actNo(tActNo), minStamp(log.stamp), maxStamp(DATETIME_MAX), finished(false), deleted(false)
{
	*this << log;
}

bool TRegisterActLog::Matches(const TLog &that, long thatActNo) const
{
	if (finished)
		return false;

	return key == that.key && kind == that.kind && date == that.date && !strcmp(ucn, that.ucn) && actNo == thatActNo;
}

void TRegisterActLog::Collect(const TLog &log)
{
	if (log.gop == LOG_NEW_DOC)
	{
		maxStamp = log.stamp;
		finished = true;
		return;
	}

	actions.AddCopy(&log);

	if (log.gop == LOG_DELETE)
	{
		deleted = finished = true;
		return;
	}

	minStamp = log.stamp;
}

#define reglog OFFICE_GROUP_CAST(TRegisterActLog)

static void regactlog_subcount(mstr &m, TGroup *group) { subcount(m, reglog->actions.Count()); }

#undef reglog

#else  // NEWQVER
TRegisterActLog::TRegisterActLog()
{
	Add(new TLong("F_SUBCOUNT", &subcount));	// for printing
}
#endif  // NEWQVER

// ----- TRegisterActQuery ---------------------------------------------------
class TRegisterActQuery : public TFloatQuery
{
public:
	TRegisterActQuery(TWindow *parent, const char *criteria);

	bool Header(TQueryRegisterActGroup *group);

protected:
	const char *tr1break;

	virtual void GetMarkers();
	virtual void Count();
};

TRegisterActQuery::TRegisterActQuery(TWindow *parent, const char *criteria)
	: TFloatQuery(parent, criteria, "QueryRActs.htm")
{
	copy = OFFICE_SKIP_HEADER;
}

void TRegisterActQuery::GetMarkers()
{
	tr1start = office.Find("tr1 start");
	tr1break = office.Find("tr1 break");
	tr1final = office.Find("tr1 final");
}

#define actgroup OFFICE_GROUP_CAST(TQueryRegisterActGroup)

static void queryact_types(mstr &m, TGroup *group) { print_types_list(m, actgroup->types, false); }
static void queryact_period(mstr &m, TGroup *group) { m.printf("%D-%Dг.", &actgroup->minDate, &actgroup->maxDate); }

#undef actgroup

static const offield queryact_header_fields[] =
{
	{ "F_TYPES",	queryact_types },
	{ "F_PERIOD",	queryact_period },
	{ NULL, NULL }
};

bool TRegisterActQuery::Header(TQueryRegisterActGroup *group)
{
	if (!office.Open(name, OFFICE_COPY_HEADER))
		return false;

	office.Emit(NULL, NULL, queryact_header_fields, group);
	office.Close();
	return true;
}

// ----- TQueryActLog --------------------------------------------------------
class TQueryActLog : public TLog
{
public:
	TQueryActLog();
	static TGroup *Clone();

	long starter;
	CDate actDate;
	char targetKind;
	CDate targetDate;
};

TQueryActLog::TQueryActLog()
	: TLog(TQueryActLog::Clone)
{
	Add(
		new TLong("F_STARTER", &starter),
		new TDate("F_ACT_DATE", &actDate, false),
		new TChar("F_TARGET_KIND", &targetKind),
		new TDate("F_TARGET_DATE", &targetDate, false),
		NULL
	);
}

TGroup *TQueryActLog::Clone()
{
	return new TQueryActLog();
}

#define querylog OFFICE_GROUP_CAST(TLog)

static void queryact_key_type(mstr &m, TGroup *group) { querylog->TypeName(m); }
static void queryact_kind_date(mstr &m, TGroup *group) { m.printf("%s %Dг.", Kind->Seek(querylog->kind), &querylog->date); }
static void queryact_stamp_date(mstr &m, TGroup *group) { querylog->stamp.Date().Print(m); }
static void queryact_user(mstr &m, TGroup *group) { print_user(m, querylog->ucn, UCN_CITIZEN_UCN, false); }

#undef querylog

static const offield queryact_newdoc_fields[] =
{
	{ "F_NO",		NULL },
	{ "F_TYPE",		queryact_key_type },
	{ "F_YEAR",		NULL },
	{ "F_KIND",		queryact_kind_date },
	{ "F_DATE",		queryact_stamp_date },
	{ "F_JUDGE",	queryact_user },
#if NEWQVER
	{ "F_SUBCOUNT",	regactlog_subcount },
#else  // NEWQVER
	{ "F_SUBCOUNT",	NULL },
#endif  // NEWQVER	
	{ NULL, NULL }
};

#define querylog OFFICE_GROUP_CAST(TQueryActLog)

static void queryact_action(mstr &m, TGroup *group)
{
	if (querylog->gop == LOG_CHANGE)
	{
		m.cat(querylog->text);
		querylog->kind = querylog->targetKind;
		querylog->date = querylog->targetDate;
	}
	else
	{
		m.cat(GOP->Seek(querylog->gop));

		if (querylog->gop == LOG_ATTACH)
		{
			m.printf(querylog->starter ? " на %s" : " \ab\bна друг %s\a/b\b",
				strstr(querylog->text, "регистриран акт:") ? "регистриран акт" : "файл");

			if (strchr(KIND_ENDOC_RIDERS, querylog->kind))
				querylog->date = querylog->actDate;
		}
	}

	m.printf("\v%s от %Dг.", Kind->Seek(querylog->kind), &querylog->date);

	//if (any(querylog->text))
	//	m.printf("\vтестова версия: текст: %s", querylog->text);
}

#undef querylog

static const offield queryact_action_fields[] =
{
	{ "F_ACTION",	queryact_action },
	{ "F_DATE",		queryact_stamp_date },
	{ "F_USER",		queryact_user },
	{ NULL, NULL }
};

#if NEWQVER
void TRegisterActQuery::Count()
{
	TQueryActLog log;
	TQuery q(criteria);
	TGArray<TRegisterActLog> array;

	while (q.Read() && Increment())
	{
		log << q;

		constant s = strstr(log.text, " акт ") + 5;

		if (!isdigit(s[0]) || !isdigit(s[1]) || isdigit(s[2]))
			continue;

		long actNo = strtol(s, NULL, 10);

		if (!actNo)
			continue;

		TRegisterActLog *regLog = NULL;

		bool starter = log.gop == LOG_NEW_DOC && log.opc == CM_JUDGE_REGISTER_ACT &&
			!strncmp(log.text, OF_JUDICIAL_ACT, strlen(OF_JUDICIAL_ACT)) && s[2] == ' ' && s[3] == '+';

		for (int i = 0; i < array.Count(); i++)
		{
			if (array[i]->Matches(log, actNo))
			{
				regLog = array[i];
				break;
			}
		}

		if (regLog)
			regLog->Collect(log);

		if (starter)
			array.Add(new TRegisterActLog(log, actNo));
	}

	for (int i = 0; i < array.Count(); i++)
	{
		TRegisterActLog *regLog = array[i];
		TGArray<TLog> &actions = regLog->actions;

		if (!regLog->deleted)
		{
			TRegisterAct act;

			act << regLog;
			strcpy(act.judge, regLog->ucn);
			act.actNo = regLog->actNo;

			if (act.Get("F_DATE, F_TARGET_KIND, F_TARGET_NO, F_TARGET_YEAR, F_TARGET_TYPE, F_TARGET_DATE, "
				"F_TARGET_STAMP") && act.targetKind)
			{
				TQueryActLog log;
				char targetKinds[SIZE_OF_KINDS];
				bool rider = strchr(KIND_ENDOC_RIDERS, regLog->kind);

				if (!act.targetStamp.Empty())
					regLog->minStamp = act.targetStamp;

				strcpy(targetKinds, C2S[act.targetKind]);

				if (rider)
					strcach(targetKinds, regLog->kind);

				log.targetKind = act.targetKind;
				log.targetDate = act.targetDate;
				log.actDate = act.date;

				msql m(log.Name);
				char text[SIZE_OF_LOG_TEXT];

				m.AddLong("F_NO", act.target.no);
				m.AddLong("F_YEAR", act.target.year);
				m.AddChar("F_TYPE", act.target.type);
				m.AddChars("F_KIND", targetKinds);
				m.AddDate("F_DATE", act.targetDate);
				m.AddRange("F_STAMP", regLog->minStamp, regLog->maxStamp);
				sprintf(text, FORMAT_FROM_ACT, regLog->actNo, regLog->kind);
				// 2016:173 LPR/IRQ: surrouendocs lead to duplicate log "endocs"
				m.AddChar("F_GOP", LOG_ATTACH);
				m.AddString("F_TEXT", text);
				m.AddOrder("F_LOG");	// F_STAMP

				TQuery r(str(m));
				bool first = true;

				while (r.Read())
				{
					log << r;

					if (log.stamp > regLog->maxStamp || (log.gop == LOG_ENTER && !first))
						break;

					// for riders we want the rider atachments only
					if (rider && log.gop == LOG_ATTACH && log.kind != regLog->kind)
						continue;

					// 2016:173 LPR/IRQ: surrouendocs lead to duplicate log "endocs"
					TQueryActLog *tLog = new TQueryActLog;

					*tLog << log;
					tLog->starter = true;
					actions.Add(tLog);
				}
			}
		}

		office.Emit(tr1start, tr1break, queryact_newdoc_fields, regLog);

		if (actions.Count() == 0)
			office.Emit(tr1break, tr1final, null_fields, NULL);
		else
		{
			for (int i = 0; i < actions.Count(); i++)
				office.Emit(tr1break, tr1final, queryact_action_fields, actions[i]);
		}
	}
}
#else  // NEWQVER
void TRegisterActQuery::Count()
{
	TRegisterActLog regLog;
	TQuery q(criteria);

	while (q.Read() && Increment())
	{
		msql m(regLog.Name);
		char text[SIZE_OF_LOG_TEXT];
		TGArray<TLog> actions;
		long actNo;

		regLog << q;
		m.Add(&regLog, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_UCN");
		m.Add("F_STAMP");
		m.cat("> ");
		regLog.Find("F_STAMP")->Quote(m);
		m.AddLong("F_OPC", CM_JUDGE_REGISTER_ACT);
		sscanf(regLog.text, OF_JUDICIAL_ACT "%02ld", &actNo);
		sprintf(text, " акт %02ld", actNo);
		m.AddLike("F_TEXT", text, true);
		m.AddOrder("F_STAMP");	// 2016:126 URQ: F_LOG is slow

		CDateTime minStamp = regLog.stamp;
		CDateTime maxStamp = DATETIME_MAX;
		bool deleted = false;

		TQuery r(str(m));
		while (r.Read())
		{
			TQueryActLog *log = new TQueryActLog;
			*log << r;

			if (log->gop == LOG_NEW_DOC)
			{
				maxStamp = log->stamp;
				delete log;
				break;
			}

			actions.Add(log);

			if (log->gop == LOG_DELETE)
			{
				deleted = true;
				break;
			}

			minStamp = log->stamp;
		}

		if (!deleted)
		{
			TRegisterAct act;

			act << regLog;
			strcpy(act.judge, regLog.ucn);
			act.actNo = actNo;

			if (act.Get("F_DATE, F_TARGET_KIND, F_TARGET_NO, F_TARGET_YEAR, F_TARGET_TYPE, F_TARGET_DATE, "
				"F_TARGET_STAMP") && act.targetKind)
			{
				TQueryActLog log;
				char targetKinds[SIZE_OF_KINDS];
				bool rider = strchr(KIND_ENDOC_RIDERS, regLog.kind);

				if (!act.targetStamp.Empty())
					minStamp = act.targetStamp;

				strcpy(targetKinds, C2S[act.targetKind]);

				if (rider)
					strcach(targetKinds, regLog.kind);

				log.targetKind = act.targetKind;
				log.targetDate = act.targetDate;
				log.actDate = act.date;

				m.Begin(log.Name);
				m.AddLong("F_NO", act.target.no);
				m.AddLong("F_YEAR", act.target.year);
				m.AddChar("F_TYPE", act.target.type);
				m.AddChars("F_KIND", targetKinds);
				m.AddDate("F_DATE", act.targetDate);
				m.AddRange("F_STAMP", minStamp, maxStamp);
				sprintf(text, FORMAT_FROM_ACT, actNo, regLog.kind);
			#if 20160173
				// 2016:173 LPR/IRQ: surrouendocs lead to duplicate log "endocs"
				m.AddChar("F_GOP", LOG_ATTACH);
				m.AddString("F_TEXT", text);
				m.AddOrder("F_LOG");	// F_STAMP

				TQuery r(str(m));
			#else
				m.AddOrder("F_LOG");	// F_STAMP

				TQuery r(str(m));
				bool start = false;
			#endif  // 20160173
				bool first = true;

				while (r.Read())
				{
					log << r;

					if (log.stamp > maxStamp || (log.gop == LOG_ENTER && !first))
						break;

					// for riders we want the rider atachments only
					if (rider && log.gop == LOG_ATTACH && log.kind != regLog.kind)
						continue;

				#if 20160173
					// 2016:173 LPR/IRQ: surrouendocs lead to duplicate log "endocs"
					TQueryActLog *tLog = new TQueryActLog;

					*tLog << log;
					tLog->starter = true;
					actions.Add(tLog);
				#else  // 20160173
					if (!start)
					{
						// skip all container operations before our act attach
						start = log.gop == LOG_ATTACH && !strcmp(log.text, text);

						if (start && rider)
						{
							TQueryActLog *tLog = new TQueryActLog;

							*tLog << log;
							tLog->starter = true;
							actions.Add(tLog);
						}
					}
					else if (log.gop == LOG_ATTACH)
					{
						if (!first && !strstr(log.text, "на htm файл"))
						{
							actions.AddCopy(&log);
							break;
						}
					}
					else
					{
						actions.AddCopy(&log);
						first = false;

						if (log.gop == LOG_DELETE || (log.gop == LOG_CHANGE && strstr(log.text, "изтрита")))
							break;
					}
				#endif  // 20160173
				}
			}
		}

		regLog.subcount = actions.Count() ? actions.Count() : 1;
		office.Emit(tr1start, tr1break, queryact_newdoc_fields, &regLog);

		if (actions.Count() == 0)
			office.Emit(tr1break, tr1final, null_fields, NULL);
		else
		{
			for (int i = 0; i < actions.Count(); i++)
				office.Emit(tr1break, tr1final, queryact_action_fields, actions[i]);
		}
	}
}
#endif  // NEWQVER

static void query_register_act(TWindow *parent, const char *criteria, TQueryRegisterActGroup *group)
{
	TRegisterActQuery act(parent, criteria);

	if (act.Header(group))
		act.Execute();
}

#include "QueryRegisterActDialog.h"
#include "QueryRegisterActDialog.cpp"

void query_register_act(TWindow *parent)
{
	// 2016:126 LPR: Windows/OWL starts accelerators without menu functions directly!
	if (command_enabled(CM_QUERY_REGISTER_ACT, MENU_AUTOMATIC))
	{
		TQueryRegisterActGroup group;
		TQueryRegisterActDialog(parent, &group, IDD_QUERY_REGISTER_ACT).Execute();
	}
}

// ----- TFloatRegisterActGroup ----------------------------------------------
class TFloatRegisterActGroup : public TQueryRegisterActGroup
{
public:
	TFloatRegisterActGroup();

	virtual void FormCriteria(msql &m);

	char kinds[SIZE_OF_KINDS];
};

TFloatRegisterActGroup::TFloatRegisterActGroup()
{
	Add(new TCharList("F_KINDS", kinds, sizeof kinds));
}

void TFloatRegisterActGroup::FormCriteria(msql &m)
{
	m.Begin("T_REGISTER_ACT", "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_JUDGE, "
		"octet_length(F_TEXT_DATA) AS F_TEXT_SIZE, octet_length(F_IMAGE_DATA) AS F_IMAGE_SIZE, F_TARGET_KIND, "
		"F_TARGET_DATE");
	ActCriteria(m);
	m.AddArray("F_UCN", judges, "F_JUDGE");
	m.AddRange("F_DATE", minDate, maxDate);
	m.AddChars("F_KIND", kinds, KIND_VISIBLE_ENDOCS);
	m.AddOrder("F_DATE");
}

// ----- TFloatRegisterAct ---------------------------------------------------
class TFloatRegisterAct : public TRegisterAct
{
public:
	TFloatRegisterAct();

	long length[TROUBLE_COUNT];
};

TFloatRegisterAct::TFloatRegisterAct()
{
	Add(
		new TLong("F_TEXT_SIZE", length + TYPE_TEXT),
		new TLong("F_IMAGE_SIZE", length + TYPE_IMAGE),
		NULL
	);
}

// ----- TFloatRegisterActQuery ----------------------------------------------
class TFloatRegisterActQuery : public TFloatQuery
{
public:
	TFloatRegisterActQuery(TWindow *parent, const char *criteria);

protected:
	virtual void Count();
};

TFloatRegisterActQuery::TFloatRegisterActQuery(TWindow *parent, const char *criteria)
	: TFloatQuery(parent, criteria, "FloatRAct.htm")
{
}

#define floatact OFFICE_GROUP_CAST(TFloatRegisterAct)

static void floatact_judge(mstr &m, TGroup *group) { print_user(m, floatact->judge); }

static void floatact_attached(mstr &m, TGroup *group)
{
	for (int type = 0; type < TROUBLE_COUNT; type++)
	{
		if (floatact->length[type])
		{
			m.sep(", ");
			m.cat(Electrics[type].what);
		}
	}
}

static void floatact_entered(mstr &m, TGroup *group)
{
	if (floatact->targetKind)
		m.printf("Вписване към %s от %Dг.", Kind->Seek(floatact->targetKind), &floatact->targetDate);
}

#undef floatact

static const offield floatact_fields[] =
{
	{ "F_TYPE",		keygroup_key_type },
	{ "F_NO",		NULL },
	{ "F_YEAR",		NULL },
	{ "F_KIND",		container_kind },
	{ "F_DATE",		NULL },
	{ "F_JUDGE",	floatact_judge },
	{ "F_ATTACHED",	floatact_attached },
	{ "F_ENTERED",	floatact_entered },
	{ NULL, NULL }
};

void TFloatRegisterActQuery::Count()
{
	TFloatRegisterAct act;
	TQuery q(criteria, ExtraDataBase);

	while (q.Read() && Increment())
	{
		act << q;
		office.Emit(tr1start, tr1final, floatact_fields, &act);
	}
}

#include "FloatRegisterActDialog.h"
#include "FloatRegisterActDialog.cpp"

void float_register_act(TWindow *parent)
{
	TFloatRegisterActGroup group;
	TFloatRegisterActDialog(parent, &group, IDD_FLOAT_REGISTER_ACT).Execute();
}
