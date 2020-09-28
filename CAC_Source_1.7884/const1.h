enum
{
	BOOK_1ST_INST	= 0x01,
	BOOK_2ND_INST	= 0x02,
	BOOK_BOTH_INST	= 0x03,
	BOOK_VZ_INST	= 0x06,
	BOOK_KS_INST	= 0x0A,
	BOOK_ALL_2ND_INST	= 0x0E,
	BOOK_INSTANCES	= 0x0F
};

// ---------- DAY ------------------------------------------------------------
enum
{
	DAY_WHOLE		= 1,
	DAY_MORNING		= 2,
	DAY_AFTERNOON	= 3
};

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL || COURT_TYPE == COURT_MILITARY
extern constant
	KIND_EXCHANGE_LAWS,
	KIND_EXCHANGE_INDOCS;
#endif  // AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
extern constant
	KIND_VZ_LAWSUITS,
	KIND_KS_LAWSUITS;
#endif  // APPEAL || ADMIN || AREAL || MILITARY

extern constant CONNECT_TYPE_PRELIMITARY_ACTS;

// ----- SESSION_ ------------------------------------------------------------
enum
{
	SESSION_OTHER	= 0,
	SESSION_DECIDE	= 1,	// including PUBLIC without act
	SESSION_CANCEL	= 2,
	SESSION_LEFT	= 3,
	SESSION_STOPPED	= 4
};

#if COURT_TYPE == COURT_REGIONAL
const char KIND_COMPL_ELEC_CARD = 'P';
#endif  // REGIONAL