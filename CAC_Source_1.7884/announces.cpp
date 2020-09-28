#include "cac.h"

// 2014:213 FB2: nested selects -> join

// ----- TAnnounceGroup ------------------------------------------------------
class TAnnounceGroup : public TFloatGroup
{
public:
	TAnnounceGroup(const char *name);

	virtual void FormCriteria(msql &m);

	CDate date;
	long court;
	char sender[SIZE_OF_UCN];
	TGArray<TUserAliasGroup> deliverers;
	char type;
	long delivered;
};

TAnnounceGroup::TAnnounceGroup(const char *name)
	: TFloatGroup(name, "T.*")
{
	Add(
		new TDate("F_DATE", &date, true),
		new TLongAlias("F_COURT", &court),
		new TStringAlias("F_SENDER", sender, sizeof sender),
		new TCharAlias("F_TYPE", &type),
		new TCheck("F_DELIVERED", &delivered, TRUE),
		NULL
	);

	delivered = -1;
}

void TAnnounceGroup::FormCriteria(msql &m)
{
	BeginCriteria(m);

	m.AddDate("T.F_SUBPOENA_DATE", date);
	m.AddArray("F_UCN", deliverers, "T.F_DELIVERER_UCN");
	// 2011:084 +related
	if (delivered != -1)
	{
		m.Add(delivered ? "T.F_RECEIVED_STATUS" : "NOT T.F_RECEIVED_STATUS");
		m.catset(SRS_DELIVEREDS);
	}
}

// ----- TExternalAnnounceGroup ----------------------------------------------
class TExternalAnnounceGroup : public TAnnounceGroup
{
public:
	TExternalAnnounceGroup();

	virtual void FormCriteria(msql &m);
};

TExternalAnnounceGroup::TExternalAnnounceGroup()
	: TAnnounceGroup("T_ANNOUNCE T")
{
}

void TExternalAnnounceGroup::FormCriteria(msql &m)
{
	TAnnounceGroup::FormCriteria(m);

	m.AddLong("T.F_COURT", court);

	if (type)
		m.AddChar("T.F_KIND", type2ConnectKind(type));
}

// ----- TInternalAnnounceGroup ----------------------------------------------
class TInternalAnnounceGroup : public TAnnounceGroup
{
public:
	TInternalAnnounceGroup();

	virtual void FormCriteria(msql &m);
};

TInternalAnnounceGroup::TInternalAnnounceGroup()
	: TAnnounceGroup("T_SUBPOENA T JOIN T_SUBPOENA_KIND K ON K.F_SUBPOENA_KIND = T.F_SUBPOENA_KIND LEFT JOIN T_INREG I "
		"ON I.F_NO = T.F_NO AND I.F_YEAR = T.F_YEAR AND I.F_TYPE = T.F_TYPE LEFT JOIN T_OUTREG O ON O.F_NO = T.F_NO "
		"AND O.F_YEAR = T.F_YEAR AND O.F_TYPE = T.F_TYPE")
{
	court = COURT_CODE;
	sprintf(sender, "%010d", court);
	type = kind2type(Default->kind);
}

void TInternalAnnounceGroup::FormCriteria(msql &m)
{
	TAnnounceGroup::FormCriteria(m);
	internal_subpoena_uclp_lead(m);

	m.AddChar("(T.F_TYPE", type);
	m.NextOr();
	m.AddChar("I.F_LAWSUIT_TYPE", type);
	m.NextOr();
	m.AddChar("O.F_LAWSUIT_TYPE", type);
	m.cat(")");
	m.AddIsNull("K.F_GENERIC_KIND", false);
	m.AddNonElectron("T.");
}

// ----- functions -----------------------------------------------------------
static int add_announce_sorted(TGroupArray *array, TAnnounce *announce, bool date)
{
	int i;

	for (i = 0; i < array->Count(); i++)
	{
		TAnnounce *tAnnounce = (TAnnounce *) (*array)[i];

		if (date)
		{
			if (
				announce->subpoenaDate < tAnnounce->subpoenaDate ||
				(announce->subpoenaDate == tAnnounce->subpoenaDate &&
				announce->Less2(tAnnounce))
			)
				break;
		}
		else if (announce->Less1(tAnnounce))
			break;
	}

	array->Insert(announce, i);

	return i;
}

int add_announce_subpoena(TGroupArray *array, TQuery &q, TAnnounce *announce, bool date)
{
	TSubpoena *subpoena = new TSubpoena;
	*subpoena << q;

	if (subpoena->HasRegards())
	{
		delete subpoena;
		delete announce;
		return INT_MAX;		// 2013:107 LPR: was -1
	}
	else if (!announce)
		announce = new TAnnounce;
		
	announce->SetSubpoena(subpoena);
	return add_announce_sorted(array, announce, date);
}

int add_announce_announce(TGroupArray *array, TQuery &q, TAnnounce *announce, bool date)
{
	if (!announce)
		announce = new TAnnounce();

	*announce << q;
	return add_announce_sorted(array, announce, date);
}

void internal_subpoena_uclp_lead(msql &m)
{
	TStreetMap *streetMap = new TStreetMap;
	TGroupArray uclps;
	msql w;

	streetMap->uclp = COURT_UCLP;
	uclps.Add(streetMap);
	w.Begin(streetMap->Name, "DISTINCT(F_UCLP)");
	w.AddLong("NOT F_UCLP", COURT_UCLP);

	TQuery q(str(w));
	while (q.Read())
	{
		streetMap = new TStreetMap;
		*streetMap << q;
		uclps.Add(streetMap);
	}

	m.AddArray("T.F_UCLP", uclps);
	m.AddChar("NOT T.F_MULTI", SUBPOENA_MULTI);
}

static void get_barcode_text(TAnnounce *announce, TStringFace *barcodeText)
{
	mstr m;

	if (any(announce->barcode))
		m.cpy(announce->barcode);
	else if (announce->requestId)
		barcode_generate(m, announce->requestId, false, announce->court);
	else if (announce->subpoena && announce->subpoena->requestId)
		barcode_generate(m, announce->subpoena->requestId, false);

	barcodeText->SetS(str(m));
}

#include "DelivererListBox.h"
#include "DelivererListBox.cpp"

#include "EditAnnounceDialog.h"
#include "EditAnnounceDialog.cpp"

#include "ServeAnnounceDialog.h"
#include "ServeAnnounceDialog.cpp"

enum
{
	SCHED_NO	= 0,
	SCHED_YES	= 1,
	SCHED_7	= 2,
	SCHED_15	= 3
};

class TStaticAnnounce : public TAnnounce
{
public:
	virtual ~TStaticAnnounce();	// 2018-03-19: LPR: don't delete sbpoena, was TAnnounce::dynamic
};

TStaticAnnounce::~TStaticAnnounce()
{
	subpoena = NULL;
}

class TSubServeHolder : public TDummyGroup, public TSubServeData
{
public:
	TSubServeHolder() : TSubServeData(this) { }
};

#define STICKER_UPDATES "F_SUBPOENA_RECEIVED, F_RECEIVED_TIME, F_RECEIVED_STATUS, F_RECEIVED_TEXT, F_SUBPOENA_RETURNED"
static constant STICKER_SELECTS = "F_SUBPOENA_NO, F_BY_POST, F_BY_POST_X, " STICKER_UPDATES;

bool serve_sticker(const TSubpoena *subpoena, TSubpoena *sticker)
{
	TSubServeHolder subserve;
	subserve << sticker;

	if (subpoena->receivedStatus == SRS_FORTY_SEVEN)
	{
		(TSubServeData &) *sticker = (TSubServeData &) *subpoena;
		sticker->receivedStatus = SRS_RECEIVED;
	}
	else
	{
		*sticker << TSubServeHolder();
		sticker->receivedStatus = sticker->InitialStatus();
	}

	if ((TSubServeData &) *sticker == (TSubServeData &) subserve)
		return false;

	if (sticker->Update(STICKER_UPDATES))
		return true;

	*sticker << subserve;
	return false;
}

void serve_stickers(TSubpoena *subpoena)
{
	if (!strchr(SRS_DELIV_BASICS, subpoena->receivedStatus))
	{
		msql m;

		subpoena->RegardCriteria(m, STICKER_SELECTS, true);
		TQuery q(str(m));

		while (q.Read())
		{
			TSubpoena sticker;

			sticker << subpoena;
			sticker << q;
			serve_sticker(subpoena, &sticker);
		}
	}
}

bool serve_subpoena(TWindow *parent, TSubpoena *subpoena, TSideWind *sideWind, TRCDKeyContainer *container, char originKind,
	char secondKind, char status, sign already)
{
	TSubpoena tSubpoena;
	long sched;
	mstr scheds;

	tSubpoena << subpoena;
	TStaticAnnounce announce;
	announce.SetSubpoena(&tSubpoena);

	if (subpoena->eventStatus == status)
		compute_scheds(scheds, container, originKind, secondKind);

	announce.Add(
		new TRadio("F_SCHED_NO", &sched, SCHED_NO),
		new TRadio("F_SCHED_YES", &sched, SCHED_YES),
		new TRadio("F_SCHED_7", &sched, SCHED_7),
		new TRadio("F_SCHED_15", &sched, SCHED_15),
		NULL
	);

	// 2010:049 LPR: regression: specific statuses
	mstr statuses;
	SubpoenaKind->Seek(subpoena->subpoenaKind);

	if (SubpoenaKind->proc == SK_PROC_STATE_PAPER)
		statuses.cat(SRS_STATE_PAPERS);
	// 2011:158; maybe by proc if more kinds
	else if (strchr(SUBPOENA_KIND_REWARDS, subpoena->kind))
		statuses.cat(SRS_REWARDS);
	else
	{
		if (SubpoenaKind->proc == SK_PROC_TELEGRAM)
			statuses.cat(SRS_TELEGRAMS);
		else
			get_received_statuses(statuses, subpoena->receivedStatus, subpoena);
		// 2010:281 LPR/LRQ: forty-fifty also for non-punishment requests
		get_forty_fifty_stats(statuses, kind2type(lawsuit_kind_from(originKind, secondKind)));
	}

	if (TServeAnnounceDialog(parent, &announce, container, str(scheds), str(statuses), already).Execute() == IDOK)
	{
		msql m(tSubpoena.Name, MQ_UPDATE);

		tSubpoena.receivedStatus = announce.receivedStatus;
		tSubpoena.subpoenaReceived = announce.subpoenaReceived;
		tSubpoena.receivedTime = announce.receivedTime;
		strcpy(tSubpoena.receivedText, announce.receivedText);
		tSubpoena.subpoenaReturned = announce.subpoenaReturned;

		tSubpoena.Quote(m, "F_SUBPOENA_RECEIVED, F_SUBPOENA_RETURNED, F_RECEIVED_STATUS, F_RECEIVED_TEXT, "
			"F_OFFICIALLY_NOTIFIED");
		m.Add(&tSubpoena, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_SUBPOENA_NO, F_EVENT_STATUS");

		if (tSubpoena.execListNo ? tSubpoena.DataBase->Execute(str(m), ET_ERROR) : tSubpoena.Update())
		{
			// 2008:178 LPR: update multi subpoena elements
			// 2014:255 LPR: shared with exec lists update
			if (tSubpoena.multi == SUBPOENA_LEADER)
			{
				m.AddChar("F_MULTI", SUBPOENA_MULTI);
				tSubpoena.DataBase->Execute(str(m), ET_ERROR);
			}

			m.clear();
			ReceivedStatus->Seek(tSubpoena.receivedStatus);
			m.printf("Призовка %ld - %s", tSubpoena.subpoenaNo, ReceivedStatus->alias);
			log_subpoena(LOG_DELIVER, &tSubpoena, str(m));

			*subpoena << tSubpoena;
			const CDate *received = &tSubpoena.subpoenaReceived;

			if (!received->Empty())
			{
				if (sched)
				{
					if (sched == SCHED_7)
						schedule(SCHED_DECIDE_7, container, received);
					else if (sched == SCHED_15)
						schedule(SCHED_SETTLE_15, container, received);
					else if (scheds.chr(SCHED_SETTLE_3))
						schedule(SCHED_SETTLE_3, container, received);
					else
						schedule(Container2Sched(container), container, received);

					if (container->key.type != TYPE_PUNISHMENT_LAW && status == RESULT_STOPPED)
						schedule(SCHED_STOPPED_M, container, received);
				}

				// closed session sched is for 131G only
				if (container->key.type != TYPE_REQUEST &&
					(!strchr(KIND_CLOSED_SESSIONS, container->kind) ||
					tSubpoena.subpoenaKind == SUBPOENA_KIND_MESSAGE_131G))
				{
					schedule(Container2Sched(container), container, received);
				}

				if (strchr(INVOLVEMENT_EXPERTS, sideWind->involvement))
					if (SubpoenaKind->ExpertTransfer() && sideWind->investId)
						xfer_send(&tSubpoena, container, sideWind, originKind);

				// 2013:235
			#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
				if (tSubpoena.subpoenaKind == SUBPOENA_KIND_RET_PROS_1)
					schedule(SCHED_RET_PROS_1, container, received);
				//else if (tSubpoena.subpoenaKind == SUBPOENA_KIND_RET_PROS_3)
				//	schedule(SCHED_RET_PROS_3, container, received);
			#endif  // AREAL || MILITARY || REGIONAL
			#if COURT_TYPE == COURT_REGIONAL
				// 2014:002; 2017:236 rewritten w/o SUBPOENA_KIND_MESSAGE_4XXS
				if (strchr(SUBPOENA_KIND_MESSAGE_415S, tSubpoena.subpoenaKind))
					schedule(SCHED_FILE_CLAIM, container, received);
				else if (tSubpoena.subpoenaKind == SUBPOENA_KIND_MESSAGE_411 ||
					tSubpoena.subpoenaKind == SUBPOENA_KIND_MESSAGE_418)
				{
					schedule(SCHED_APP_EXECOR, container, received);
				}
			#endif  // REGIONAL
			}

			return true;
		}
	}

	return false;
}

bool serve_subpoena(TWindow *parent, TSubpoena *subpoena, sign already)
{
	TRCDKeyContainer *const container = kind2RCDKeyContainer(subpoena->kind);
	TVirtPtr VP(container);
	bool result = false;

	container->key = subpoena->key;
	container->date = subpoena->date;

	if (container->Get())
	{
		bool personal = strchr(KIND_ENDOCS, container->kind) && ((TDecision *) container)->PersonalData();
		TSideWind *sideWind = personal ? (TSideWind *) new TJudgement : new TSideWind;

		*sideWind << subpoena;
		if (sideWind->Get())
		{
			char originKind, secondKind;

			get_container_kinds(container, originKind, secondKind);
			result = serve_subpoena(parent, subpoena, sideWind, container, originKind, secondKind, container->Status(),
				already);
			delete sideWind;
		}
	}

	return result;
}

bool serve_announce(TWindow *parent, TAnnounce *announce)
{
	if (announce->subpoena)
	{
		if (serve_subpoena(parent, announce->subpoena, -1))
		{
			announce->receivedStatus = announce->subpoena->receivedStatus;
			return true;
		}
	}
	else
	{
		TAnnounce tAnnounce;
		mstr statuses;

		tAnnounce << announce;

		get_received_statuses(statuses, announce->receivedStatus, announce->subpoena);
		get_forty_fifty_stats(statuses, connectKind2Type(announce->key.kind));

		if (TServeAnnounceDialog(parent, &tAnnounce, NULL, "", str(statuses), -1).Execute() == IDOK)
		{
			ReceivedStatus->Seek(tAnnounce.receivedStatus);
			log_announce(LOG_DELIVER, &tAnnounce, ReceivedStatus->alias);
			*announce << tAnnounce;
			return true;
		}
	}

	return false;
}

static bool operator<=(const TAnnounce &announce1, const TAnnounce &announce2) { return announce1.key <= announce2.key; }

bool edit_announce(TWindow* parent, TAnnounce *announce, bool edit, TGroupArray *deliverers, const char *kinds, bool scan)
{
	return TEditAnnounceDialog(parent, announce, edit, deliverers, kinds, scan).Execute() == IDOK;
}

#include "AnnounceListWindow.h"
#include "AnnounceListWindow.cpp"

#include "AnnounceListDialog.h"
#include "AnnounceListDialog.cpp"

void announce_external(TWindow *parent)
{
	TExternalAnnounceGroup group;
	TAnnounceListDialog(parent, &group).Execute();
}

void announce_internal(TWindow *parent)
{
	TInternalAnnounceGroup group;
	TAnnounceListDialog(parent, &group).Execute();
}