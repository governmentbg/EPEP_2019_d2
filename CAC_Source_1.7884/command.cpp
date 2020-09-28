#include "cac.h"

// ----- commands ------------------------------------------------------------
struct TMenuCommand
{
	char function;
	int command;
	bool (*special)();
};

// ----- REG -----------------------------------------------------------------
#define COLLEGE_FUNCTION(type, COLLEGE) \
	static bool can_college_##type() { return strchr(Default->colleges, TYPE_##COLLEGE##_LAW); }

#if COURT_TYPE == COURT_APPEAL
	COLLEGE_FUNCTION(punishment, PUNISHMENT)
	COLLEGE_FUNCTION(citizen, CITIZEN)
	COLLEGE_FUNCTION(recitizen, RECITIZEN)
	COLLEGE_FUNCTION(trade, TRADE)
	COLLEGE_FUNCTION(retrade, RETRADE)
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	COLLEGE_FUNCTION(admin, ADMIN)
	COLLEGE_FUNCTION(tricky, TRICKY)
	COLLEGE_FUNCTION(citizen, CITIZEN)
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	COLLEGE_FUNCTION(punishment, PUNISHMENT)
	COLLEGE_FUNCTION(citizen, CITIZEN)
	COLLEGE_FUNCTION(trade, TRADE)
	COLLEGE_FUNCTION(admin, ADMIN)
	COLLEGE_FUNCTION(firm, FIRM)
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	COLLEGE_FUNCTION(punishment, PUNISHMENT)
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	COLLEGE_FUNCTION(marriage, MARRIAGE)
	COLLEGE_FUNCTION(citizen, CITIZEN)
	COLLEGE_FUNCTION(punishment, PUNISHMENT)
	COLLEGE_FUNCTION(admin, ADMIN)
#endif  // REGIONAL

#if COURT_TYPE == COURT_MILITARY
static bool is_military_appeal() { return CourtType == COURT_MILITARY_APPEAL; }
#endif  // MILITARY

static bool can_payments() { return TPosTerm::Active(POSTERM_BUDGET) || TPosTerm::Active(POSTERM_DEPOSITS); }
static bool xfer_eportal() { return XFER_EPORTAL; }

static const TMenuCommand command_REG[] =
{
#if COURT_TYPE == COURT_APPEAL
	{ RIGHT_NEW_DOC,	CM_REG_NEW_PUNISHMENT,	can_college_punishment },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_CITIZEN,	can_college_citizen },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_RECITIZEN,	can_college_recitizen },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_TRADE,		can_college_trade },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_RETRADE,	can_college_retrade },
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ RIGHT_NEW_DOC,	CM_REG_NEW_ADMIN,		can_college_admin },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_TRICKY,	can_college_tricky },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_CITIZEN,	can_college_citizen },
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	{ RIGHT_NEW_DOC,	CM_REG_NEW_PUNISHMENT,	can_college_punishment },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_CITIZEN,	can_college_citizen },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_TRADE,		can_college_trade },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_ADMIN,		can_college_admin },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_FIRM,		can_college_firm },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ RIGHT_NEW_DOC,	CM_REG_NEW_PUNISHMENT, 	can_college_punishment },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ RIGHT_NEW_DOC,	CM_REG_NEW_MARRIAGE,	can_college_marriage },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_CITIZEN,	can_college_citizen },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_PUNISHMENT,	can_college_punishment },
	{ RIGHT_NEW_DOC,	CM_REG_NEW_ADMIN,		can_college_admin },
#endif  // REGIONAL
	{ RIGHT_EDIT_DOC,	CM_REG_EDIT,		NULL },
	{ RIGHT_EDIT_DOC,	CM_REG_EDIT_REQUEST,	NULL },
	{ RIGHT_NEW_SUR,	CM_REG_NEW_SURROUND,	NULL },
	{ RIGHT_EDIT_SUR, CM_REG_EDIT_SURROUND,	NULL },
	{ RIGHT_INREG,	CM_REG_IN_NEW,		NULL },
	{ RIGHT_INREG,	CM_REG_IN_NEW_X,		NULL },
	{ RIGHT_INREG,	CM_REG_IN_NEW_EPORTAL,	xfer_eportal },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	{ RIGHT_INREG,	CM_REG_IN_SURROUENDOCS,	NULL },
#endif  // APPEAL || AREAL
	{ RIGHT_INREG,	CM_REG_IN_EDIT,		NULL },
	{ RIGHT_INREG,	CM_REG_NEW_INREG_EVENT,	NULL },
	{ RIGHT_INREG,	CM_REG_EDIT_INREG_EVENT,NULL },
	{ RIGHT_OUTREG,	CM_REG_OUT_APPEAL,	NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	{ RIGHT_OUTREG,	CM_REG_OUT_JURI_CONF,	NULL },
#endif  // APPEAL || AREAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_OUTREG,	CM_REG_OUT_JURI_CONF,	NULL },
#endif  // ADMIN || MILITARY || REGIONAL
	{ RIGHT_OUTREG,	CM_REG_OUT_CONF_HIGH,	NULL },
	{ RIGHT_OUTREG,	CM_REG_OUT_SEND_OTHER,	NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	{ RIGHT_OUTREG,	CM_REG_OUT_RETURN_LOWER,NULL },
#endif  // APPEAL || ADMIN
#if COURT_TYPE == COURT_MILITARY
	{ RIGHT_OUTREG,	CM_REG_OUT_RETURN_LOWER,is_military_appeal },
#endif  // MILITRY
#if COURT_TYPE == COURT_AREAL
	{ RIGHT_OUTREG,	CM_REG_OUT_RETURN_LOWER,NULL },
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_OUTREG,	CM_REG_OUT_RETURN_PROSEC,NULL },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	{ RIGHT_OUTREG,	CM_REG_OUT_EDIT,		NULL },
	{ RIGHT_OUTREG,	CM_REG_OUT_RECEIVE,	NULL },
	{ RIGHT_PAYMENTS,	CM_REG_PAYMENTS,		can_payments },
	{ '\0',		0,				NULL }
};

// ----- CLERK ---------------------------------------------------------------
// 2007:261 LPR: some types may not support some connect types
static bool can_connect_types(const char *predefined)
{
	char lawKinds[SIZE_OF_KINDS];
	adjust_lawsuit_kinds(predefined, lawKinds);
	return any(lawKinds);
}

static bool can_returneds() { return can_connect_types(CONNECT_TYPE_RETURNEDS); }

#if COURT_TYPE == COURT_MILITARY
static bool is_military_military() { return CourtType == COURT_MILITARY; }
#endif  // MILITARY

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static bool can_jurisdictions() { return can_connect_types(CONNECT_TYPE_JURISDICTIONS); }
#endif  // ADMIN || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static bool can_rereturneds() { return can_connect_types(CONNECT_TYPE_RERETURNEDS); }
static bool can_reneweds() { return can_connect_types(CONNECT_TYPE_RENEWEDS); }
#endif  // AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static bool can_force_endoc() { return true; }
#endif  // APPEAL || ADMIN || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
static bool can_force_endoc() { return strcmp(Default->colleges, TYPE_FIRM_LAWS); }
#endif  // AREAL

static bool can_legal_aid() { return XFER_LEGAL_AID; }
static bool can_unfiltered_acts() { return FILTER_SESSIONS || FILTER_DECISIONS; }

static const TMenuCommand command_CLERK[] =
{
	// NEWLAW
#if COURT_TYPE == COURT_APPEAL
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW,			NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_LAWSUIT,		NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RETURNED,		can_returneds },
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_1ST,			NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_2ND,			NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_LAWSUIT,		NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_JURISD,		can_jurisdictions },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RETURNED,		can_returneds },
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_1ST,			NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_2ND,			NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_LAWSUIT,		NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_JURISD,		can_jurisdictions },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RETURNED,		can_returneds },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RERETURNED,	can_rereturneds },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RENEWED,		can_reneweds },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_1ST,			is_military_military },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_2ND,			is_military_appeal },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_LAWSUIT,		NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_JURISD,		can_jurisdictions },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RETURNED,		can_returneds },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RERETURNED,	can_rereturneds },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RENEWED,		can_reneweds },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW,			NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_LAWSUIT,		NULL },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RETURNED,		can_returneds },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_JURISD,		can_jurisdictions },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RERETURNED,	can_rereturneds },
	{ RIGHT_NEW_LAW,	CM_CLERK_NEW_RENEWED,		can_reneweds },
#endif  // REGIONAL
	// NEWLAW
	{ RIGHT_EDIT_LAW,	CM_CLERK_EDIT_LAWSUIT,		NULL },
#if COURT_TYPE == COURT_AREAL
	{ RIGHT_EDIT_LAW,	CM_CLERK_ENTER_REREGISTER_DATE,	NULL },
#endif  // AREAL
	{ RIGHT_RES_DOC,	CM_CLERK_NEW_RESOLUTION_UCN,		NULL },
	{ RIGHT_RES_DOC,	CM_CLERK_NEW_REQUEST_RESOLUTION,	NULL },
	{ RIGHT_RES_DOC,	CM_CLERK_EDIT_RESOLUTION_UCN,		NULL },
	{ RIGHT_RES_DOC,	CM_CLERK_EDIT_REQUEST_RESOLUTION,	NULL },
	{ RIGHT_CLOSED,	CM_CLERK_CLOSED_RESULT,			NULL },
	{ RIGHT_CLOSEDIT,	CM_CLERK_EDIT_CLOSED,			NULL },
	{ RIGHT_FIX_LAW,	CM_CLERK_SET,				NULL },
	{ RIGHT_FIX_LAW,	CM_CLERK_SET_LOAD,			NULL },
	{ RIGHT_FIX_LAW,	CM_CLERK_EDIT_SET,			NULL },
	{ RIGHT_FIX_LAW,	CM_CLERK_RESET,				NULL },
	{ RIGHT_FIX_LAW,	CM_CLERK_UNSET,				NULL },
	{ RIGHT_LIST_LAW,	CM_CLERK_FIXED_LIST,			NULL },
	{ RIGHT_MESSAGES,	CM_CLERK_PRINT_MESSAGES,		NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DECISION,	CM_CLERK_NEW_JUDICIAL_ACT,	NULL },
	{ RIGHT_EDIT_ACT,	CM_CLERK_EDIT_JUDICIAL_ACT,	NULL },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	{ RIGHT_DECISION,	CM_CLERK_NEW_JUDICIAL_ACT,	NULL },
	{ RIGHT_EDIT_ACT,	CM_CLERK_EDIT_JUDICIAL_ACT,	NULL },
#endif  // ADMIN
	{ RIGHT_EDIT_LAW,	CM_CLERK_FORCE_DATE,		can_force_endoc },
	{ RIGHT_MOVEMENT,	CM_CLERK_IN_OUT_MOVEMENT,	NULL },
	{ RIGHT_SCHEDULE,	CM_CLERK_EDIT_SCHEDS,		NULL },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DISTRIB,	CM_CLERK_NEW_DISTRIB_JURORS,	NULL },
	{ RIGHT_DISTRIB,	CM_CLERK_EDIT_DISTRIB_JURORS,	NULL },
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	{ '\0',		CM_CLERK_EXTERN_SESSIONS,	can_extern_sess },
	{ RIGHT_IMPORT,	CM_CLERK_EXPORT_ARCHIVES,	NULL },
	{ RIGHT_IMPORT,	CM_CLERK_IMPORT_ARCHIVED,	NULL },
#endif  // AREAL
	{ '\0',		CM_CLERK_LEGAL_AID,		can_legal_aid },
	{ '\0',		CM_CLERK_NIA_PROTOCOL,		NULL },
	{ '\0',		CM_CLERK_NIA_COLLECT,		NULL },
	{ '\0',		CM_CLERK_BARCODE_SCAN,		NULL },
	{ RIGHT_OUTMAIL,	CM_CLERK_OUT_MAIL,		NULL },
	{ '\0',		CM_CLERK_UNFILTERED_ACTS,	can_unfiltered_acts },
	{ '\0',		0,					NULL }
};

// ----- SECR ----------------------------------------------------------------
static const TMenuCommand command_SECR[] =
{
	{ RIGHT_OPEN,	CM_SECR_OPEN_RESULT,		NULL },
	{ RIGHT_CLOSED,	CM_SECR_CLOSED_RESULT,		NULL },
	{ RIGHT_CLOSEDIT,	CM_SECR_EDIT_CLOSED,		NULL },
	{ RIGHT_FIX_LAW,	CM_SECR_SET,			NULL },
	{ RIGHT_FIX_LAW,	CM_SECR_SET_LOAD,			NULL },
	{ RIGHT_FIX_LAW,	CM_SECR_EDIT_SET,			NULL },
	{ RIGHT_FIX_LAW,	CM_SECR_RESET,			NULL },
	{ RIGHT_FIX_LAW,	CM_SECR_UNSET,			NULL },
	{ RIGHT_LIST_LAW,	CM_SECR_FIXED_LIST,		NULL },
	{ RIGHT_MESSAGES,	CM_SECR_PRINT_MESSAGES,		NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DECISION,	CM_SECR_NEW_JUDICIAL_ACT,	NULL },
	{ RIGHT_EDIT_ACT,	CM_SECR_EDIT_JUDICIAL_ACT,	NULL },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	{ RIGHT_DECISION,	CM_SECR_NEW_JUDICIAL_ACT,	NULL },
	{ RIGHT_EDIT_ACT,	CM_SECR_EDIT_JUDICIAL_ACT,	NULL },
#endif  // ADMIN
	{ RIGHT_EDIT_LAW,	CM_SECR_FORCE_DATE,		can_force_endoc },
	{ RIGHT_MOVEMENT,	CM_SECR_IN_OUT_MOVEMENT,	NULL },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DISTRIB,	CM_SECR_NEW_DISTRIB_JURORS,	NULL },
	{ RIGHT_DISTRIB,	CM_SECR_EDIT_DISTRIB_JURORS,	NULL },
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	{ '\0',		CM_SECR_EXTERN_SESSIONS,	can_extern_sess },
	{ RIGHT_IMPORT,	CM_SECR_EXPORT_ARCHIVES,	NULL },
	{ RIGHT_IMPORT,	CM_SECR_IMPORT_ARCHIVED,	NULL },
#endif  // AREAL
	{ '\0',		CM_SECR_LEGAL_AID,		can_legal_aid },
	{ '\0',		CM_SECR_NIA_PROTOCOL,		NULL },
	{ '\0',		CM_SECR_NIA_COLLECT,		NULL },
	{ '\0',		CM_SECR_UNFILTERED_ACTS,	can_unfiltered_acts },
	{ '\0',		0,					NULL }
};

// ----- JUDGE ---------------------------------------------------------------
static bool can_register_acts() { return can_extra_db() && strchr(POST_JUDGES, Default->post); }
static bool can_query_activity() { return true; }

static const TMenuCommand command_JUDGE[] =
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DECISION,	CM_JUDGE_NEW_JUDICIAL_ACT,	NULL },
	{ RIGHT_EDIT_ACT,	CM_JUDGE_EDIT_JUDICIAL_ACT,	NULL },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	{ RIGHT_DECISION,	CM_JUDGE_NEW_JUDICIAL_ACT,	NULL },
	{ RIGHT_EDIT_ACT,	CM_JUDGE_EDIT_JUDICIAL_ACT,	NULL },
#endif  // ADMIN
	{ RIGHT_CLOSED,	CM_JUDGE_CLOSED_RESULT,		NULL },
	{ RIGHT_CLOSEDIT,	CM_JUDGE_EDIT_CLOSED,		NULL },
	{ RIGHT_FIX_LAW,	CM_JUDGE_SET_LOAD,		NULL },
	{ RIGHT_LIST_LAW,	CM_JUDGE_FIXED_LIST,		NULL },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DISTRIB,	CM_JUDGE_NEW_DISTRIB_JURORS,	NULL },
	{ RIGHT_DISTRIB,	CM_JUDGE_EDIT_DISTRIB_JURORS,	NULL },
#endif  // AREAL || MILITARY || REGIONAL
	{ RIGHT_ATTACH,	CM_JUDGE_REGISTER_ACT,		can_register_acts },
	{ '\0',		0,					NULL }
};

// ----- ARCH ----------------------------------------------------------------
static const TMenuCommand command_ARCH[] =
{
	{ RIGHT_ARCH_LAW,	CM_ARCH_NEW_ARCHIVE,		NULL },
	{ RIGHT_ARCH_LAW,	CM_ARCH_EDIT_ARCHIVE,		NULL },
#if COURT_TYPE == COURT_AREAL
	{ RIGHT_ARCH_LAW,	CM_ARCH_IMPORT_ARCHIVES,	NULL },
	{ RIGHT_ARCH_LAW,	CM_ARCH_EXPORT_ARCHIVED,	NULL },
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_PROVE,	CM_ARCH_NEW_PROVE,		NULL },
	{ RIGHT_PROVE,	CM_ARCH_EDIT_PROVE,		NULL },
	{ RIGHT_PROVE,	CM_ARCH_NEW_PROVE_ACTION,	NULL },
	{ RIGHT_PROVE,	CM_ARCH_EDIT_PROVE_ACTION,	NULL },
#endif  // APPEAL || AREA || MILITARY || REGIONAL
	{ RIGHT_MOVEMENT,	CM_ARCH_IN_OUT_MOVEMENT,	NULL },
	{ RIGHT_KILL_LAW,	CM_ARCH_NEW_DESTROY,		NULL },
	{ RIGHT_KILL_LAW,	CM_ARCH_EDIT_DESTROY,		NULL },
	{ '\0',		0,					NULL }
};

// ----- DELIV ---------------------------------------------------------------
static bool can_deliver() { return COURT_WRIT; }

static const TMenuCommand command_DELIV[] =
{
	{ '\0',		CM_DELIV_EXTERNAL,	can_deliver },
	{ '\0',		CM_DELIV_INTERNAL,	can_deliver },
	{ '\0',		CM_DELIV_BARCODE_SCAN,	can_deliver },
	{ '\0',		0,				NULL },
};

// ----- QUERY ---------------------------------------------------------------
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
static bool can_query_exchange()
{
	char kinds[SIZE_OF_LAW_KINDS];
	return build_ckinds(kinds, KIND_EXCHANGE_LAWS, false);
}
#endif  // AREAL || REGIONAL

static bool can_not_locked() { return !COURT_LOCK; }
static bool can_query_punishment() { return can_access_college(TYPE_PUNISHMENT_LAW, false); }
#if COURT_TYPE == COURT_REGIONAL
static bool can_query_citizen() { return can_access_college(TYPE_CITIZEN_LAW, false); }
#endif  // REGIONAL

static const TMenuCommand command_QUERY[] =
{
	{ '\0',		CM_QUERY_UCN,		NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_QUERY_GROUP,		NULL },
#endif  // APPEAL || AREA || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	{ '\0',		CM_QUERY_GROUP,		NULL },
#endif  // ADMIN
	{ '\0',		CM_ELECTRIC_BLUE,		NULL },
#if COURT_TYPE == COURT_AREAL
	{ '\0',		CM_QUERY_FIRM,		NULL },
#endif  // AREAL
	{ '\0',		CM_QUERY_CONNECT,		NULL },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_QUERY_JUROR_DISTRIB,	NULL },
#endif  // AREAL || MILITARY || REGIONAL
	{ '\0',		CM_QUERY_ARCHIVE,		NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_QUERY_EISPP_LAWS,	NULL },
#endif  // APPEAL || AREA || MILITARY || REGIONAL
	{ '\0',		CM_FLOAT_COMBO,		NULL },
	{ '\0',		CM_FLOAT_REQUEST,		NULL },
	{ '\0',		CM_FLOAT_CONNECT,		NULL },
	{ '\0',		CM_FLOAT_INDEX,		NULL },
	{ '\0',		CM_FLOAT_SUBPOENA,	NULL },
	{ '\0',		CM_FLOAT_ANNOUNCE,	can_deliver },
	{ '\0',		CM_FLOAT_SURROUND,	NULL },
	{ '\0',		CM_FLOAT_SENDERS,		NULL },
	{ '\0',		CM_FLOAT_OUTREG,		NULL },
	{ '\0',		CM_FLOAT_UNAPPEALED,	NULL },
	{ '\0',		CM_FLOAT_MOVEMENT,	NULL },
	{ '\0',		CM_FLOAT_SCHED,		NULL },
	{ '\0',		CM_FLOAT_PENALITY,	NULL },
	{ '\0',		CM_FLOAT_REWARD,		NULL },
	{ '\0',		CM_FLOAT_TAX,		NULL },
	{ '\0',		CM_FLOAT_JUROR,		NULL },
	{ '\0',		CM_FLOAT_EXEC_LIST,	NULL },
#if COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_FLOAT_HERITAGE,	can_college_citizen },
#endif  // REGIONAL
	{ '\0',		CM_FLOAT_PAYMENTS,	NULL },
	{ '\0',		CM_FLOAT_REGISTER_ACT,	can_extra_db },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_QUERY_MOVEMENT,	NULL },
#endif  // APPEAL || AREA || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	{ '\0',		CM_QUERY_MOVEMENT,	NULL },
#endif  // ADMIN
	{ '\0',		CM_EXPIRE_SCHED,		NULL },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_QUERY_EXCHANGE,	can_query_exchange },
#endif  // AREAL || REGIONAL
	{ RIGHT_SPORADIC,	CM_TIMED_SESSION, 	NULL },
	{ RIGHT_SPORADIC,	CM_TIMED_LAWSUIT, 	can_not_locked },
	{ '\0',		CM_UNFINISHED_LAW,	NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_QUERY_JUROR_LOAD,	can_session_times },
#endif  // APPEAL || AREA || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	{ '\0',		CM_QUERY_BANKRUPTCY,	NULL },
	{ '\0',		CM_QUERY_INCONSIDE,	NULL },
#endif  // AREAL
	{ RIGHT_IMPORTANT,CM_QUERY_IMPORTANT,	NULL },
	{ '\0',		CM_QUERY_CORRUPT,		can_query_punishment },
	{ '\0',		CM_PERIOD_SESSION,	NULL },
	{ '\0',		CM_YOUNG_PERIOD,		NULL },
	{ RIGHT_UNWRITTEN,CM_UNWRITTEN_ACT,		NULL },
	{ RIGHT_UNWRITTEN,CM_UNWRITTEN_SES,		NULL },
	{ '\0',		CM_PERIOD_OUTREG,		NULL },
	{ '\0',		CM_FLOAT_ARCHIVE,		NULL },
	{ '\0',		CM_FLOAT_DESTROY,		NULL },
	{ '\0',		CM_QUERY_SUBPOENAS,	NULL },
	{ '\0',		CM_QUERY_PAYMENTS,	NULL },
	{ '\0',		CM_QUERY_NIA_PROTOCOLS,	NULL },
	{ '\0',		CM_QUERY_REGISTER_ACT,	can_extra_db },
#if COURT_TYPE == COURT_APPEAL
	{ RIGHT_ACTIVITY,	CM_QUERY_JUDGES,		NULL },
	{ RIGHT_ACTIVITY,	CM_QUERY_ACTIVITY,	can_query_activity },
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ RIGHT_ACTIVITY,	CM_QUERY_JUDGES,		NULL },
	{ RIGHT_ACTIVITY,	CM_QUERY_ACTIVITY,	can_query_activity },
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	{ RIGHT_ACTIVITY,	CM_QUERY_JUDGES,		NULL },
	{ RIGHT_ACTIVITY,	CM_QUERY_ACTIVITY,	can_query_activity },
	{ '\0',		CM_QUERY_P24A1,		NULL },
	{ '\0',		CM_QUERY_BANCONSIST,	NULL },
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ RIGHT_ACTIVITY,	CM_QUERY_JUDGES,		NULL },
	{ RIGHT_ACTIVITY,	CM_QUERY_ACTIVITY,	can_query_activity },
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ RIGHT_ACTIVITY,	CM_QUERY_JUDGES,		NULL },
	{ RIGHT_ACTIVITY,	CM_QUERY_ACTIVITY,	can_query_activity },
#endif  // REGIONAL
	{ '\0',		0,				NULL }
};

// ----- BOOK ----------------------------------------------------------------
static bool can_book_decision()
{
	char types[SIZE_OF_TYPES];
	return build_ctypes(types, TYPE_NON_PUNISHMENT_LAWS, false);
}

#if COURT_TYPE == COURT_AREAL
static bool can_book_slowdown() { return intersect(Default->colleges, TYPE_TRADEALIKE_LAWS); }
#endif  // AREAL

static const TMenuCommand command_BOOK[] =
{
	{ '\0',		CM_BOOK_DESC,		NULL },
	{ '\0',		CM_BOOK_DESC_HTML,	NULL },
	{ '\0',		CM_BOOK_DESC_DELIV,	can_deliver },
#if COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_BOOK_ZZDN,		can_query_citizen },
	{ '\0',		CM_BOOK_SUCCESS,		NULL },
#endif  // REGIONAL
	{ '\0',		CM_BOOK_ABC,		NULL },
	{ '\0',		CM_BOOK_ABC_HTML,		can_not_locked },
	{ '\0',		CM_BOOK_TIME,		NULL },
	{ '\0',		CM_BOOK_TIME_HTML,	can_not_locked },
	{ '\0',		CM_BOOK_CLOSED,		NULL },
	{ '\0',		CM_BOOK_CLOSED_HTML,	can_not_locked },
	{ '\0',		CM_BOOK_INDOC,		NULL },
	{ '\0',		CM_BOOK_REQUEST,		NULL },
	{ '\0',		CM_BOOK_INREG,		NULL },
	{ '\0',		CM_BOOK_OUTREG,		NULL },
	{ '\0',		CM_BOOK_DECISION,		can_book_decision },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
	{ '\0',		CM_BOOK_39,			NULL },
#endif  // APPEAL || ADMIN || AREAL
	{ '\0',		CM_BOOK_RECUSAL,		NULL },
#if COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_BOOK_DIVORCE,		can_not_locked },
#endif  // REGIONAL	
#if COURT_TYPE == COURT_ADMIN
	{ '\0',		CM_BOOK_251,		NULL },
#endif  // ADMIN
	{ '\0',		CM_BOOK_EXEC_LIST,	NULL },
	{ '\0',		CM_BOOK_APPEAL,		NULL },
#if COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_BOOK_EUHER,		can_query_citizen },
#endif  // REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_BOOK_EXECUTION,	NULL },
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	{ '\0',		CM_BOOK_634,		NULL },
	{ '\0',		CM_BOOK_SLOWDOWN,		can_book_slowdown },
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_BOOK_PROVE,		NULL },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	{ '\0',		CM_BOOK_PENALITIES,	NULL },
	{ '\0',		CM_BOOK_ARCHIVE,		NULL },
	{ '\0',		CM_BOOK_EXTERNAL,		can_deliver },
	{ '\0',		CM_BOOK_INTERNAL,		can_deliver },
	{ RIGHT_PRINTER,	CM_BOOK_PRINT,		NULL },
	{ '\0',		0,				NULL }
};

// ----- ADMIN ---------------------------------------------------------------
static bool is_admin_post() { return Default->post == POST_ADMIN; }
static bool can_admin_query() { return is_sysadmin() || can_query(); }
#if COURT_TYPE == COURT_AREAL
static bool can_admin_unlocked_query() { return  !COURT_LOCK && can_admin_query(); }
#endif  // AREAL
static bool can_float_electric() { return is_sysadmin() || (can_query() && is_admin_post()); }
static bool eispp_active() { return EISPP_ACTIVE; }
static bool can_update_archives() { return is_sysadmin() && ArchiveSubject->Any(); }
static bool can_disable() { return false; }
static bool can_log_kinds() { return is_sysadmin() && COURT_TYPE == COURT_AREAL; }
#if COURT_TYPE == COURT_ADMIN
//static bool can_export_csv() { return is_sysadmin() && COURT_CSVE; }
static bool can_export_csv() { return false; }
#endif  // ADMIN
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
static bool can_stat_upper() { return true; }
#endif  // APPEAL || ADMIN || AREAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
static bool can_stat_lower() { return true; }
#endif  // APPEAL || ADMIN || AREAL || REGIONAL
#if COURT_TYPE == COURT_MILITARY
static bool can_stat_upper() { return CourtType == COURT_MILITARY_APPEAL; }
static bool can_stat_lower() { return CourtType == COURT_MILITARY; }
#endif  // MILITARY
static bool xfer_eportal_admin() { return XFER_EPORTAL && is_admin_post(); }
static bool xfer_eportal_sysadmin() { return XFER_EPORTAL && is_sysadmin(); } 

static const TMenuCommand command_ADMIN[] =
{
	{ RIGHT_NOM,	CM_ADMIN_USERS,		NULL },
	{ RIGHT_KNOWN,	CM_ADMIN_KNOWN_PERSONS,	NULL },
	{ RIGHT_NOM,	CM_ADMIN_SUBJECTS,	NULL },
	{ RIGHT_NOM,	CM_ADMIN_COMPOSITIONS,	NULL },
	{ RIGHT_NOM,	CM_ADMIN_PENALITIES,	NULL },
	{ RIGHT_NOM,	CM_ADMIN_REASONS,		NULL },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_NOM,	CM_ADMIN_DANGERS,		NULL },
	{ RIGHT_NOM,	CM_ADMIN_ARTICLES,	NULL },
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	{ RIGHT_NOM,	CM_ADMIN_BANCONSISTS,	can_not_locked },
#endif  // AREAL
	{ RIGHT_NOM,	CM_ADMIN_SLC_REASONS,	NULL },
	{ RIGHT_NOM,	CM_ADMIN_ARCHIVE_INDEXES, NULL },
	{ RIGHT_NOM,	CM_ADMIN_BANK_ACCOUNTS,	NULL },
	{ RIGHT_NOM,	CM_ADMIN_CURRENCES,	NULL },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_NOM,	CM_ADMIN_SESSION_PRICES, can_session_times },
#endif  // AREAL || MILITARY || REGIONAL
	{ RIGHT_NOM,	CM_ADMIN_SENDERS,		NULL },
	{ RIGHT_NOM,	CM_ADMIN_CALENDAR,	is_admin_post },
	{ RIGHT_NOM,	CM_ADMIN_HALLS,		NULL },
	{ RIGHT_NOM,	CM_ADMIN_CONFIG,		NULL },	// 2010:147 LPR: FIX: is_admin_post() -> NULL, as <= 4266
	{ RIGHT_NOM,	CM_ADMIN_INFORMEDS,	can_not_locked },
	{ '\0',		CM_ADMIN_EPORT_QUERY,	NULL },
	{ '\0',		CM_ADMIN_EPORT_LAWSUIT_ASSIGN, NULL },
	{ '\0',		CM_ADMIN_EPORT_PERSON_ASSIGN, NULL },
	{ '\0',		CM_ADMIN_EPORT_SEND,	xfer_eportal_admin },
	{ RIGHT_DELETE,	CM_ADMIN_EPORT_DELETE,	xfer_eportal_admin },
	{ RIGHT_NOM,	CM_ADMIN_COUNTRIES,	NULL },
	{ RIGHT_NOM,	CM_ADMIN_AREAS,		NULL },
	{ RIGHT_NOM,	CM_ADMIN_REGIONS,		NULL },
	{ RIGHT_NOM,	CM_ADMIN_UCLPS,		NULL },
	{ RIGHT_NOM,	CM_ADMIN_STREETS,		NULL },
	{ RIGHT_LOG,	CM_ADMIN_LOG,		NULL },	// 2012:016 can_admin_query -> RIGHT_LOG
	// DELETE
	{ RIGHT_DEL_IO,	CM_ADMIN_DELETE_INDOC,	NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_REQUEST_RESOLUTION, NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_LAWSUIT,	is_admin_post },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_EISPP_NMR,	NULL },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_LAWSUIT_MOVEMENT, NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_LAWSUIT_EVENT, NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_INREG_EVENT,	NULL },
	{ RIGHT_DEL_IO,	CM_ADMIN_DELETE_OUTREG,		NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_OUTREG_RETURN, NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_NIA_PROTOCOL,	NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_FOREIGN_ADDRESS, NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_OLD_CITIZEN,	NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_MOVEMENT,	NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_ARCHIVE,	NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_PROVE,		NULL },
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_PROVE_ACTION,	NULL },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_XFER,		NULL },
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ RIGHT_DELETE,	CM_ADMIN_DELETE_JUROR_DISTRIB, NULL },
#endif  // AREAL || MILITARY || REGIONAL
	{ '\0',		CM_ADMIN_DISCARD_CITIZENS,	is_sysadmin },
	// DELETE
	{ RIGHT_RENUMBER,	CM_ADMIN_RENUMBER_REQUEST,	NULL },
	{ RIGHT_RENUMBER, CM_ADMIN_RENUMBER_LAWSUIT,	NULL },
	{ RIGHT_RENUMBER,	CM_ADMIN_RENUMBER_UXN,		NULL },
	{ RIGHT_RENUMBER,	CM_ADMIN_RENUMBER_SURROUND,	NULL },
	{ RIGHT_RENUMBER, CM_ADMIN_RENUMBER_INREG,	NULL },
	{ RIGHT_RENUMBER,	CM_ADMIN_RENUMBER_STREET,	NULL },
	{ RIGHT_RENUMBER,	CM_ADMIN_RENUMBER_SUBJECTS,	is_admin_post },
	{ RIGHT_RENUMBER,	CM_ADMIN_JOIN_UXN,		NULL },
	{ RIGHT_RENUMBER, CM_ADMIN_JOIN_STREET,		NULL },
	{ RIGHT_RENUMBER, CM_ADMIN_JOIN_EQUAL_STREETS,	can_not_locked },
	{ RIGHT_OTHER,	CM_ADMIN_CHECKOUT_CITIZENS,	NULL },
	{ RIGHT_OTHER,	CM_ADMIN_CHECKOUT_FIRMS,	NULL },
	{ RIGHT_OTHER,	CM_ADMIN_CHECKOUT_STREET_MAPS, NULL },
	{ RIGHT_FIX_LAW,	CM_ADMIN_RESET_SESSION,		NULL },
	{ RIGHT_EDIT_LAW,	CM_ADMIN_REKIND_LAWSUIT,	NULL },
	{ RIGHT_EDIT_LAW,	CM_ADMIN_RECOMPOSE_LAWSUIT,	NULL },
	{ RIGHT_OTHER,	CM_ADMIN_COPY_SIDE_WINDS,	NULL },
	{ RIGHT_OTHER,	CM_ADMIN_MOVE_JUDICIAL_ACT,	NULL },
	{ RIGHT_RENUMBER,	CM_ADMIN_RENUMBER_SIDE_WIND,	NULL },
	{ RIGHT_OTHER,	CM_ADMIN_EISPP_EDIT_LAWSUIT,	eispp_active },
	{ RIGHT_OTHER,	CM_ADMIN_EISPP_EDIT_FZLPNE,	eispp_active },
	{ '\0',		CM_ADMIN_UPDATE_ARCHIVES,	can_update_archives },	// 2016:025 obsolete
	//{ '\0',		CM_ADMIN_UPDATE_REQUESTS,	is_sysadmin },
	{ '\0',		CM_ADMIN_UPDATE_REQUESTS,	can_disable },
	{ '\0',		CM_ADMIN_UPDATE_LONG_TEXTS,	is_sysadmin },
	{ '\0',		CM_ADMIN_UPDATE_LOG_KINDS,	can_log_kinds },
	{ '\0',		CM_ADMIN_UPDATE_EPORTALS,	xfer_eportal_sysadmin },
#if COURT_TYPE == COURT_AREAL
	{ RIGHT_IMPORT,	CM_ADMIN_IMPORT_REREGISTERED,	NULL },
#endif  // AREAL
	{ RIGHT_IMPORT,	CM_ADMIN_IMPORT_PERSONAL_NO,	NULL },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	{ RIGHT_IMPORT,	CM_ADMIN_EXPORT_BANKRUPT,	NULL },
#endif  // APPEAL || AREAL
#if COURT_TYPE == COURT_ADMIN
	{ RIGHT_IMPORT,	CM_ADMIN_EXPORT_CSV,		can_export_csv },
#endif  // ADMIN
	{ '\0',		CM_ADMIN_QUERY_WEB,		can_admin_query },
#if COURT_TYPE == COURT_AREAL
	{ '\0',		CM_ADMIN_QUERY_BANKRUPT,	can_admin_query },
	{ '\0',		CM_ADMIN_BANKRUPTS,		can_admin_unlocked_query },
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_ADMIN_QUERY_POLICE,		can_admin_query },
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_ADMIN_QUERY_PROSEC,		can_admin_query },
#endif  // AREAL || REGIONAL
	{ '\0',		CM_ADMIN_FLOAT_PUBLICATION,	can_admin_query },
	{ '\0',		CM_ADMIN_FLOAT_ELECTRIC,	can_float_electric },
	{ '\0',		CM_ADMIN_QUERY_XFER_WEB,	can_admin_query },
	{ '\0',		CM_ADMIN_QUERY_YOUNG,		can_admin_query },
	{ '\0',		CM_ADMIN_RANDOM_ACT,		can_admin_query },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	{ '\0',		CM_QUERY_STAT_FIRST,		can_stat_upper },
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ '\0',		CM_QUERY_STAT_SECOND,		can_stat_lower },
#endif  // ADMIN || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	{ '\0',		CM_QUERY_STAT_THIRD,		can_stat_upper },
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	{ '\0',		CM_COMBINE_CORRUPT,		NULL },
#endif  // APPEAL || AREAL
	{ '\0',		CM_ADMIN_OBJECT_COUNT,		can_admin_query },
	{ '\0',		0,					NULL }
};

// ----- HELP ----------------------------------------------------------------
static const TMenuCommand command_HELP[] =
{
	{ '\0',		CM_HELP_CONTENTS,		NULL },
	{ '\0',		CM_HELP_INDEX,		NULL },
	{ '\0',		CM_HELP_KEYBOARD,		NULL },
	{ '\0',		CM_HELP_ABOUT,		NULL },
	{ '\0',		CM_HELP_REFRESH,		NULL },
	{ '\0',		CM_HELP_LOGIN,		NULL },
	{ '\0',		CM_HELP_TESTING,		NULL },
	{ '\0',		0,				NULL }
};

// ----- matchers ------------------------------------------------------------
// 2006:069; 2009:280 former bloat check_request_lawality()
const char *request_lawable(TRCDKeyGroup *request, char status)
{
	TLawsuit lawsuit;
	msql m(lawsuit.Name, "F_NO, F_YEAR, F_TYPE");

	lawsuit.source = request->key;
	m.Add(&lawsuit, "F_SOURCE_NO, F_SOURCE_YEAR, F_SOURCE_TYPE");
	TQuery q(str(m));

	if (q.Read())
	{
		static char s[0x100];

		lawsuit << q;
		if (q.Read())
			sprintf(s, "Документът вече е образуван в две или повече дела");
		else
		{
			sprintf(s, "Документът вече е образуван в %s %ld/%ldг", Type->Seek(lawsuit.key.type),
				lawsuit.key.no, lawsuit.key.year);
		}

		return s;
	}

	if (!status)
		status = request->Status();

	return strchr(RESOLUTION_LAWABLES, status) ? NULL : wrong_status;
}

const char *request_editable(TRCDKeyGroup *, char status)
{
	return strchr(RESULT_CLEANS, status) || can_func(RIGHT_OVERDOC) ? NULL : wrong_status;
}

bool lawsuit_settable_ll(const TLawsuit *lawsuit)
{
	// 2018-10-03: IRQ/LPR: check NEWGPK
	if (!strchr(KIND_FIRST_SET_LAWS, lawsuit->kind))
		return true;

	if (strchr(TYPE_NEWGPK_LAWS, lawsuit->key.type) && !(lawsuit->flags & LAWSUIT_NEWGPK))
		return true;

	TSession session;
	msql m;

	session.key = lawsuit->key;
	session.FloatCriteria(m, "F_NO");
	m.AddFlags("F_FLAGS_X", SESSION_X_FIRSET, SESSION_X_FIRSET, SESSION_X_ALL);

	TQuery q(str(m));
	return q.Read();
}

const char *lawsuit_settable(TRCDKeyGroup *group, char)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	return lawsuit_settable_ll((const TLawsuit *) group) ? NULL :
		"По делото няма въведено ЗЗ/РЗ с включено поле Насрочване за I-во заседание";
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	// 2018-05-08: checked later, KIND_NO1STSET_SESSION is always allowed
	(void) group;
	return NULL;
#endif  // REGIONAL
}

const char *not_aborted(TRCDKeyGroup *group, char status)
{
	if (status == RESULT_ABORTED && strchr(KIND_SESSIONS, TYPECHECK_PTR_CAST(TRCDKeyContainer, group)->kind))
		return wrong_status;

	return NULL;
}

const char *not_departed(TRCDKeyGroup *group, char status)
{
	if (!strchr(RESULT_UNDEPARTEDS, status) && group->key.type == TYPE_INREG)
		return wrong_status;

	return NULL;
}

const char *not_aborted_departed(TRCDKeyGroup *group, char status)
{
	const char *s = not_aborted(group, status);
	return s ? s : not_departed(group, status);
}

const char *lawsuit_editable(TRCDKeyGroup *, char status)
{
	return strchr(RESULT_CLEANS, status) || can_func(RIGHT_OVERLAW) ? NULL : wrong_status;
}

const char *surroundable(TRCDKeyGroup *group, char status)
{
	if (!*adjust_surround_kinds((TRCDKeyContainer *) group))
		return "Не поддържа съпровождащи документи";

	if (group->key.type == TYPE_REQUEST && status == RESOLUTION_LAWED)
		return wrong_status;

	return NULL;
}

const char *not_aborted_present(TRCDKeyGroup *group, char status)
{
	const char *s = not_aborted(group, status);

	if (s == NULL && TYPECHECK_PTR_CAST(TSession, group)->date > Today)
		s = "Датата на заседанието е по-голяма от текущата дата";

	return s;
}

const char *schedulable(TRCDKeyGroup *group, char)
{
	mstr scheds;
	compute_scheds(scheds, TYPECHECK_PTR_CAST(TRCDKeyContainer, group));
	return any(scheds) ? NULL : "Характеристиките на документа/събитието не отговарят на нито един вид срок";
}

const char *indoc_deletable(TRCDKeyGroup *group, char status)
{
	if (strchr(KIND_SURROUENDOCS, ((const TRCDKeyContainer *) group)->kind))
	{
		TSurroundment surroundment;
		msql m;

		surroundment << group;
		surroundment.BasicCriteria(m, "F_NO");
		m.AddIsNull("F_DECISION_KIND", false);

		TQuery q(str(m));
		if (q.Read())
			return "По документа има вписан съдебен акт";
	}

	if (group->key.type == TYPE_SURROUND)
	{
		const TSurround *surround = (const TSurround *) group;

		if (surround->lawsuit.type == TYPE_REQUEST)
		{
			TRequest request;
			request.key = surround->lawsuit;

			if (request.Status() == RESOLUTION_LAWED)
				return "Преписката, по която е регистриран съпровождащия документ, е образувана в дело";
		}
	}

	return strchr(group->key.type == TYPE_INREG ? RESULT_UNDEPARTEDS : RESULT_CLEANS, status) ? NULL : wrong_status;
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
const char *jurorable(const TLawsuit *lawsuit, char sessionKind)
{
	Subject->Seek(lawsuit->key.type, lawsuit->subject);

	if (strchr(KIND_OPEN_SESSIONS, sessionKind))
	{
		return strchr(Subject->Attrib(lawsuit->kind), POST_JUROR) ? NULL :
			"Предметът на делото не предполага наличие на заседатели в съдебния състав. Ако данните за "
			"вид, номер и година на делото са въведени вярно се обърнете към системния администратор";
	}
	else
	{
		return strchr(Subject->Attrib(lawsuit->kind), POST_CLOSE_JUROR) ? NULL :
			"Предметът на делото не предполага наличие заседатели в закрито заседание. Ако данните за "
			"вид, номер и година на делото са въведени вярно се обърнете към системния администратор";
	}
}

const char *jurorable(char status)
{
	switch (status)
	{
		case RESULT_ABORTED : return "Заседанието е отменено";
		case RESULT_UNFIXED : return "Заседанието е отложено без ново насрочване";
		case RESULT_FIXED : return NULL;
		default : return "Заседанието е проведено";
	}
}

const char *jurorable(TRCDKeyGroup *group, char status)
{
	const char *s = jurorable(status);
	return s ? s : jurorable(PrimaryGroup(TLawsuit), ((const TSession *) group)->kind);
}
#endif  // AREAL || MILITARY || REGIONAL

const char *forcable(TRCDKeyGroup *group, char status)
{
	if (strchr(KIND_SESSIONS, TYPECHECK_PTR_CAST(TRCDKeyContainer, group)->kind))
	{
		if (strchr(RESULT_UNCARRIEDS, status))
			return "Заседанието не е проведено";

		if (assumes_decision(status))
			return "Резултатът предполага вписване на датата на влизане в законна сила към отделен съдебен акт";
	}

	return NULL;
}

const char *has_eispp_nmr(TRCDKeyGroup *group, char)
{
	TString *eispp_nmr = (TString *) group->Find("F_EISPP_NMR");
	return eispp_nmr && any(eispp_nmr->S()) ? NULL : "Делото/документа няма вписан ЕИСПП номер на НП";
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
const char *eispp_nmr_deletable(TRCDKeyGroup *group, char status)
{
	const char *result = has_eispp_nmr(group, status);

	if (!result && group->key.type == TYPE_REQUEST)
	{
		TLawsuit lawsuit;

		if (lawsuit.TryBy(group->key, "F_NO"))
			result = wrong_status;
	}

	return result;
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

const char *carried(TRCDKeyGroup *, char status)
{
	return strchr(RESULT_UNCARRIEDS, status) ? "Заседанието не е проведено" : NULL;
}

const char *event_editable(TRCDKeyGroup *, char status)
{
	return strchr(RESULT_INREG_RESOLUTIONS, status) && status != RESULT_EPORTAL_ALLOWED ? NULL : wrong_status;
}

const char *inreg_editable(TRCDKeyGroup *, char status)
{
	return status != RESULT_EPORTAL_ALLOWED ? NULL : wrong_status;
}

const char *not_appealed_act(TRCDKeyGroup *group, char)
{
	const TDecision *decision = (const TDecision *) group;
	TInReg inReg;
	msql m(inReg.Name, "F_NO");

	m.AddLong("F_LAWSUIT_NO", decision->key.no);
	m.AddLong("F_LAWSUIT_YEAR", decision->key.year);
	m.AddChar("F_LAWSUIT_TYPE", decision->key.type);
	m.AddChar("F_SESSION_KIND", decision->sessionKind);
	m.AddChar("F_DECISION_KIND", decision->kind);
	m.AddDate("F_DECISION_DATE", decision->date);

	TQuery q(str(m));
	return q.Read() ? "Съдебния акт е обжалван." : NULL;
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
const char *juror_logged(TRCDKeyGroup *group, char)
{
	TJurorLog jurorLog;
	msql m;

	jurorLog.key = group->key;
	jurorLog.excess = TYPECHECK_PTR_CAST(TExternContainer, group)->Excess();
	jurorLog.FloatCriteria(m, "F_NO");
	TQuery q(str(m));
	return q.Read() ? NULL : "По делото няма журнал за разпределение на заседатели";
}
#endif  // AREAL || MILITARY || REGIONAL

const char *not_archived_killed(TRCDKeyGroup *group, char)
{
	TArchive archive;

	if (archive.TryBy(TYPECHECK_PTR_CAST(TExternContainer, group), "F_KILL_DATE, F_ARCHIVE_NO, F_ARCHIVE_YEAR"))
	{
		if (archive.killDate.Empty())
		{
			static char s[0x100];  // URQ: required
			sprintf(s, "Делото вече е архивирано с архивен номер %ld/%ldг", archive.archiveNo, archive.archiveYear);
			return s;
		}

		return "Делото е унищожено";
	}

	return NULL;
}

const char *extern_archable(TRCDKeyGroup *group, char status)
{
	const char *s = not_archived_killed(group, status);
	TExternContainer *lawsuit = TYPECHECK_PTR_CAST(TExternContainer, group);

	// 2011:158 LPR/LRQ: hack SOFGS + trade 890 -> subject attrib
	if (s == NULL && lawsuit->key.type != TYPE_FIRM_LAW && lawsuit->ForceDate().Empty())
	{
		Subject->Seek(lawsuit->key.type, lawsuit->subject);

		if (!strchr(Subject->Attrib(lawsuit->kind), POST_ALWAYS_ARCH))
			s = "Липсва дата на влизане в законна сила на делото в цялост";
	}

	return s;
}

const char *resolution_deletable(TRCDKeyGroup *group, char status)
{
#if TESTVER
	TLawsuit lawsuit;
	// 2017:212 LPR
	if (status == RESOLUTION_LAWED && !lawsuit.TryBy(group->key, "F_NO"))
		return NULL;
#else  // TESTVER
	(void) group;		
#endif  // TESTVER
	return strchr(RESOLUTION_STANDALONES, status) ? NULL : wrong_status;
}

// ----- menus ---------------------------------------------------------------
struct TMinMaxMenu
{
	int min;
	int max;
	char menu;	// N.B. OPTIONAL
	const TMenuCommand *commands;
};

static const TMinMaxMenu minMaxMenus[] =
{
	{ 10000, 10999, RIGHT_REG,	command_REG },
	{ 11000, 11999, RIGHT_CLERK,	command_CLERK },
	{ 12000, 12999, RIGHT_SECR,	command_SECR },
	{ 14000, 14999, RIGHT_JUDGE,	command_JUDGE },
	{ 15000, 15499, RIGHT_ARCH,	command_ARCH },
	{ 15500, 15999, RIGHT_DELIV,	command_DELIV },
	{ 16000, 16999, RIGHT_QUERY,	command_QUERY },
	{ 17000, 17999, RIGHT_BOOK,	command_BOOK },
	{ 18000, 18999, RIGHT_ADMIN,	command_ADMIN },
	{ 19000, 19999, '\0',		command_HELP },
	{ 0,     0,     '\0',		NULL }
};

// ----- command -------------------------------------------------------------
char MENU_AUTOMATIC[2];

bool command_enabled(int command, const char *menus)
{
	if (COURT_MINI)
	{
		return command == CM_QUERY_GROUP ||
			(COURT_FLAT && command == CM_QUERY_UCN) ||
			(COURT_WRIT && command == CM_ELECTRIC_BLUE) ||
			(COURT_EXAR && command == CM_QUERY_CONNECT);
	}

	if (command == CM_HELP_ABOUT)
		return true;

	// 2010:020 LPR: special override to avoid deadlock
	if (command == CM_ADMIN_USERS && is_sysadmin())
		return true;

	const TMinMaxMenu *menu;

	for (menu = minMaxMenus; menu->min; menu++)
		if (command >= menu->min && command <= menu->max)
			break;

	if (!menu->min)
		fatal("%d: lacks menu mapping", command);

	if (menus && menu->menu && command != CM_BOOK_PRINT)
	{
		MENU_AUTOMATIC[0] = menu->menu;
		MENU_AUTOMATIC[1] = '\0';

		if (!intersect(Default->menus, menus))
			return false;
	}

	const TMenuCommand *cmd;

	for (cmd = menu->commands; cmd->command; cmd++)
		if (cmd->command == command)
			break;

	if (!cmd->command)
		fatal("%d: lacks command mapping", command);

	if (!strchr(Default->functions, cmd->function))
		return false;

	if (cmd->special && !cmd->special())
		return false;

	return true;
}
