#include "cac.h"

long get_log_id(TCACDataBase *dataBase)
{
	return dataBase->GetNext("G_LOG_LOG", "F_LOG");
}

static void log(char gop, long opc, const TRCDKey *key, char kind, const CDate *date, const char *text, const char *ucn,
	const long *log_id)
{
	TLog log;

	if (log_id)
		log.log = *log_id;
	else
		log.log = get_log_id(log.DataBase);

	strcpy(log.ucn, ucn ? ucn : Default->ucn);
	log.gop = gop;
	log.opc = opc ? opc : PrimaryCommand ? PrimaryCommand : CM_UNKNOWN;
	CDateTime::SyncServer(SYNC_PRECISE);
	log.stamp = _CurrentDateTime;
	if (key)
		log.key = *key;
	log.kind = kind;
	if (date)
		log.date = *date;
	strzcpy(log.text, text ? text : "", sizeof log.text);
	log.Insert();
}

void log(char gop, long opc, const TRCDKey *key, char kind, const CDate *date, const char *text)
{
	log(gop, opc, key, kind, date, text, NULL, NULL);
}

void log(char gop, long opc, const TRCDKey *key, const char *text)
{
	log(gop, opc, key, '\0', NULL, text);
}

void log(char gop, long opc, long no, long year, char type, char kind, const CDate *date, const char *text)
{
	TRCDKey key;

	key.no = no;
	key.year = year;
	key.type = type;
	log(gop, opc, &key, kind, date, text);
}

void log(char gop, long opc, const char *text)
{
	log(gop, opc, 0, 0, '\0', '\0', NULL, text);
}

void log(char gop, long opc, const TRCDKeyContainer *container, const char *text)
{
	log(gop, opc, &container->key, container->kind, &container->date, text);
}

void log(char gop, long opc, const TRCDKeyContainer *container, TSideWind *sideWind)
{
	mstr m;

	sideWind->PrintGroup(m);
	log(gop, opc, container, str(m));
}

void log(char gop, long opc, const TRCDKeyContainer *container, const TRCDKey *key)
{
	log(gop, opc, container);

	if (key->type)
	{
		mstr m;

		m.printf("%s %ld/%ldг.", Type->Seek(container->key.type), container->key.no, container->key.year);
		log(gop, opc, key, str(m));
	}
}

void log_event(char gop, long opc, TIndexContainer *container, sign publinet)
{
	mstr m;

	if (container->eventNo)
	{
		Kind->Seek(container->IndexKind());
		m.printf("%s %ld/%ld", Kind->alias, container->eventNo, container->IndexYear());
	}

	if (publinet >= 0)
	{
		m.sep(": ");
		m.printf("ППИ-%s.", publinet ? "вкл" : "изкл");
	}

	log(gop, opc, container, str(m));
}

void log_lawsuit(char gop, long opc, const TLawsuit *lawsuit)
{
	mstr m;

	if (gop == LOG_NEW_LAW)
	{
		if (lawsuit->source.type == TYPE_REQUEST)
		{
			Type->Seek(TYPE_REQUEST);
			m.printf("по %s%ld/%ld", Type->alias, lawsuit->source.no, lawsuit->source.year);
		}

		m.sep(" ");
		m.printf("%s#", lawsuit->autoNumbering ? "автом" : "ръчен");
	}

	m.sep(" ");
	m.printf("%s дост.", (lawsuit->flags & LAWSUIT_LIMITED) ? "огран." : "норм.");
	log(gop, opc, lawsuit, str(m));
}

void register_usage(int regime, int varianth, const char* types, const char* remark)
{
	mstr m;

	if (varianth || any(types) || any(remark))
		m.printf("в=%ld, т=%s, з=%s", varianth, types, remark);

	log(LOG_USAGE, regime, NULL, '\0', NULL, str(m));
}

void register_usage(int regime, const char type, int varianth, const char* remark)
{
	register_usage(regime, varianth, C2S[type], remark);
}

// 2013:169 IRQ/TRQ: extented log with proper ucn and login
void log_inout(const char *inout, const char *ucn)
{
	mstr m;

	m.printf("%s - %s", inout, CONFIG_NAME_LOCAL);
	log(LOG_INOUT, CM_ENTER, NULL, '\0', NULL, str(m), ucn, NULL);
}

static void log_juror(TJurorLog &jurorLog, const TBaseSession *session, char oper, const char *juror, int index,
	const char *text)
{
	jurorLog.key = session->key;
	jurorLog.kind = session->kind;
	jurorLog.date = session->date;
	strcpy(jurorLog.ucn, Default->ucn);
	jurorLog.oper = oper;
	strcpy(jurorLog.juror, juror);
	jurorLog.index = index;
	CDateTime::SyncServer(SYNC_PRECISE);
	jurorLog.stamp = _CurrentDateTime;
	strcpy(jurorLog.text, text);
	jurorLog.excess = session->Excess();
	jurorLog.Insert();
}

void log_juror(const TBaseSession *session, char oper, const char *juror, int index, const char *text)
{
	TJurorLog jurorLog;

	jurorLog.jurorLog = get_log_id(jurorLog.DataBase);
	log_juror(jurorLog, session, oper, juror, index, text);
}

void log_juror_double(char gop, long opc, const TBaseSession *session, const char *text, char oper)
{
	TJurorLog jurorLog;

	jurorLog.jurorLog = get_log_id(jurorLog.DataBase);
	log_juror(jurorLog, session, oper, "", -1, "");
	log(gop, opc, &session->key, session->kind, &session->date, text, Default->ucn, &jurorLog.jurorLog);
}

void log_surrouendoc(char gop, long opc, const TSurroundment *surrouendoc)
{
	mstr m;

	m.printf("по съпровождащ документ %ld/%ldг.", surrouendoc->key.no, surrouendoc->key.year);
	log(gop, opc, &surrouendoc->lawsuit, surrouendoc->decisionKind, &surrouendoc->decisionDate, str(m));
}

void log_subpoena(char gop, const TSubpoena *subpoena, const char *text)
{
	log(gop, 0, &subpoena->key, subpoena->kind, &subpoena->date, text);
}

void log_announce(char gop, const TAnnounce *announce, const char *suffix)
{
	mstr m;
	char type = connectKind2Type(announce->key.kind);

	m.printf(LOG_ANNOUNCE_FORMAT, announce->court, announce->announce);
	m.sep_cat(" - ", suffix);
	// 2018-07-04: LPR: 0 -> CM_DELIV_EXTERNAL for log
	log(gop, CM_DELIV_EXTERNAL, announce->key.no, announce->key.year, type, '\0', &announce->sessionDate, str(m));
}
