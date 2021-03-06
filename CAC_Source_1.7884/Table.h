// ----- TTable --------------------------------------------------------------
enum
{
	TABLE_ACCOMPLY,
	TABLE_ACCOUNT,
	TABLE_ACCOUNT_TYPE,
	TABLE_ACCUSATION,
	TABLE_ADDRESS,
	TABLE_AGE,
	TABLE_ANNOUNCE,
	TABLE_ANNOUNCE_VISIT,
	TABLE_APPEAL_RESOLUTION,
	TABLE_ARCHIVE,
	TABLE_ARCHIVE_INDEX,
	TABLE_ARCHIVE_SUBJECT,
	TABLE_AREA,
	TABLE_ARTICLE,
	TABLE_ARTICLE_SUBJECT,
	TABLE_ATTACH_STAT,
	TABLE_BANCONSIST,
	TABLE_BANKRUPT,
	TABLE_CALENDAR,
	TABLE_CAL_INTERVAL,
	TABLE_CITIZEN,
	TABLE_COLLEGE,
	TABLE_COMPANY_TYPE,
	TABLE_COMPOSITION,
	TABLE_CONFIG_DATA,
	TABLE_CONFIG_NAME,
	TABLE_CONNECT,
	TABLE_CONNECT_KIND,
	TABLE_CONNECT_TYPE,
	TABLE_COUNTRY,
	TABLE_COURT,
	TABLE_CRIME_FORM,
	TABLE_CRIME_STAGE,
	TABLE_CURRENCY,
	TABLE_DAMN,
	TABLE_DANGER,
	TABLE_DANGER_SUBJECT,
	TABLE_DEBTOR_STATUS,
	TABLE_DECISION,
	TABLE_EISPP_CRIME,
	TABLE_EISPP_DEPART,
	TABLE_EISPP_PENDING,
	TABLE_EISPP_PLACE,
	TABLE_EISPP_TRANSL,
	TABLE_EISPP_VAL,
	TABLE_ELECTRIC,
	TABLE_EPORT_ASSIGN,
	TABLE_EPORT_SEND,
	TABLE_EPORT_GUID,	
	TABLE_EVENT_INDEX,
	TABLE_EXEC_LIST,
	TABLE_EXEC_LIST_ADDRESS,
	TABLE_EXEC_LIST_COLLECT,
	TABLE_EXEC_LIST_SUM,
	TABLE_EXEC_LIST_PERSON,
	TABLE_EXPENSE,
	TABLE_EXTERN_LAWSUIT,
	TABLE_EXTERN_SIDE_WIND,
	TABLE_EXTERN_SESSION,
	TABLE_FIRM,
	TABLE_FOREIGN_ADDRESS,
	TABLE_FZL,
	TABLE_GOP,
	TABLE_GRANT,
	TABLE_HALL,
	TABLE_INDEX,
	TABLE_INFORMED,
	TABLE_INREG,
	TABLE_INREG_EVENT,
	TABLE_INTERVAL,
	TABLE_INVOLVEMENT,
	TABLE_JAIL_MODE,
	TABLE_JUDGED,
	TABLE_JUDGEMENT,
	TABLE_JUROR_DISTRIB,
	TABLE_JUROR_LOG,
	TABLE_KIND,
	TABLE_KNOWN_PERSON,
	TABLE_LAWSUIT,
	TABLE_LAW_INTERVAL,
	TABLE_LEGAL_AID_TYPE,
	TABLE_LINK_WIND,
	TABLE_LOAD_DATE,
	TABLE_LOAD_WEEK,
	TABLE_LOG,
	TABLE_MAIL_TYPE,
	TABLE_MONTH,
	TABLE_MOVEMENT,
	TABLE_NIA_PROTOCOL,
	TABLE_NPR,
	TABLE_NPRFZLPNE,
	TABLE_OLD_CITIZEN,
	TABLE_OLD_FIRM,
	TABLE_OLD_SUBJECT,
	TABLE_OUTREG,
	TABLE_PAYMENT,
	TABLE_PENALITY_TYPE,
	TABLE_PNE,
	TABLE_POST,
	TABLE_PROVE,
	TABLE_PROVE_ACTION,
	TABLE_PUNISHMENT,
	TABLE_RANK,
	TABLE_REASON_TYPE,
	TABLE_RECIDIVE,
	TABLE_REGION,
	TABLE_REGISTER_ACT,
	TABLE_REGISTERED_MAIL,
	TABLE_REPLACE_PAIRS,
	TABLE_REPORT_RANGE,
	TABLE_REQUEST,
	TABLE_REQUEST_RESOLUTION,
	TABLE_RESOLUTION,
	TABLE_RESULT,
	TABLE_RIGHT,
	TABLE_SBE_DESC,
	TABLE_SCHED,
	TABLE_SECTION,
	TABLE_SENDER,
	TABLE_SESSION,
	TABLE_SESSION_PRICE,
	TABLE_SESSION_TIMES,
	TABLE_SIDE_WIND,
	TABLE_SLC_REASON,
	TABLE_SLC_TYPE,
	TABLE_SNPR,
	TABLE_STREET,
	TABLE_STREET_MAP,
	TABLE_SUBJECT,
	TABLE_SUBJECT_RANGE,
	TABLE_SUBPOENA,
	TABLE_SUBPOENA_KIND,
	TABLE_RECEIVED_STATUS,
	TABLE_SUBPOENA_VISIT,
	TABLE_SUM_STATE,
	TABLE_SUM_TYPE,
	TABLE_SURROUND,
	TABLE_SURROUNDMENT,
	TABLE_TYPE,
	TABLE_UCLP,
	TABLE_UCN_TYPE,
	TABLE_USER,
	TABLE_UXN_TYPE,
	TABLE_VSS_INDEXES,
	TABLE_WEEKDAY,
	TABLE_WHAT,
	TABLE_XFER_APPEAL,
	TABLE_XFER_CONNECT,
	TABLE_XFER_DECISION,
	TABLE_XFER_DOCUMENT,
	TABLE_XFER_EMAIL,
	TABLE_XFER_EXPERT,
	TABLE_XFER_INVEST,
	TABLE_XFER_KIND,
	TABLE_XFER_SIDE_CITIZEN,
	TABLE_XFER_SIDE_FIRM,
	TABLE_XFER_WEB,
	TABLE_COUNT
};

class TTable : public TVObject
{
public:
	TTable(const char *name);

	TCACDataBase *&DataBase;

protected:
	static TIArray<FLDDesc> Descs[TABLE_COUNT];	// szName, iFldType, iLen and iUnUsed[0]

	TIArray<FLDDesc> *descs;
	TCACDataBase *dataBase;

	void GetDescs() const;
};
