enum
{
	XFER_OUT_LETTER_EQUAL		= 7004,
	XFER_OUT_LETTER_COMPETENT	= 7001,
	XFER_CONNECT_TYPE_CONFLICT	= 3005,
	XFER_CONNECT_TYPE_STRIFE	= 3006,
	XFER_CONNECT_TYPE_LOWER		= 3002,
	XFER_INDOC_REJECT_JUDGES	= 8061
};

enum
{
	XFER_SEND		= 0,
	XFER_RECEIVE	= 1,
	XFER_WEB_DELETE	= 1,
	XFER_INVALID	= -1
};

enum
{
	XFER_TYPES_PUNISHMENT	= 0x01,
	XFER_TYPES_CITIZEN	= 0x02,
	XFER_TYPES_FIRM		= 0x04,
	XFER_TYPES_ALL		= 0x07
};

constant
	XFER_COURT_TYPES		= "3457269",	// 2009:230 LRQ: +4 (SPEAL); 2014:206 +2; 2014:290 +69
	XFER_TYPE_INCOMINGS	= "JKks";

#if COURT_TYPE == COURT_AREAL
constant
	XFER_FROM_INVOLVEMENTS	= "cZVd",
	XFER_TO_INVOLVEMENTS	= "лпфц";
#endif  // COURT_AREAL

enum
{
	XFER_TYPE_RECEIVED	= 'L',
	XFER_TYPE_RESIGN		= 'c',
	XFER_TYPE_ASSIGN		= 'n',
	XFER_TYPE_ACCEPT		= 'r',
	XFER_TYPE_NOTIFY		= 'u'
};

constant XFER_CCEXP_DEFAULT	= "https://ams.justice.bg/Investigation/AssignmentFromCourt/";

const
	// 2011:160; 2011:196 LRQ: 100 -> 7800
	LENGTH_OF_XFER_EMAIL_TEXT	= 8100,
	SIZE_OF_XFER_EMAIL_TEXT		= LENGTH_OF_XFER_EMAIL_TEXT + 1;

const
	XFER_FIRM_NAME	= 50,	// 2009:348 LPR: cut down on export
	XFER_EMAIL_VOID	= 1,
	XFER_EMAIL_TEXT	= 2,
	XFER_EMAIL_HTML	= 3;

// ----- XferDate ------------------------------------------------------------
class CXferDate : public CDateTime
{
public:
	CXferDate() { }
	void operator=(const CDate &date) { ((CDate &) *this) = date; }
};

class TXferDate : public TTimeStamp
{
public:
	TXferDate(const char *name, CXferDate *date) : TTimeStamp(name, date) { }

protected:
	virtual void ImportFromControl(TControl *control);
};

void TXferDate::ImportFromControl(TControl *control)
{
	SetDate(((const TDateFace *) control)->Date());
}

// ----- TXferKey ------------------------------------------------------------
struct TXferKey
{
	long xferKind;
	long no;
	long year;
};

#define NEW_XFERKEY(prefix_, id) \
	new TLong(prefix_"KIND", &(id)->xferKind), \
	new TLong(prefix_"NO", &(id)->no), \
	new TLong(prefix_"YEAR", &(id)->year)

// ----- TXferCitizen --------------------------------------------------------
struct TXferCitizen
{
	char name[SIZE_OF_CITIZEN_NAME];
	char rename[SIZE_OF_CITIZEN_RENAME];
	char family1[SIZE_OF_CITIZEN_FAMILY];
	char family2[SIZE_OF_CITIZEN_FAMILY];
	char egn[SIZE_OF_UCN];

	bool Init(const char *ucn, char ucnType);
};

#define NEW_XFER_CITIZEN(prefix_, citizen) \
	new TString(prefix_"NAME", (citizen)->name, sizeof (citizen)->name), \
	new TString(prefix_"RENAME", (citizen)->rename, sizeof (citizen)->rename), \
	new TString(prefix_"FAMILY_1", (citizen)->family1, sizeof (citizen)->family1), \
	new TString(prefix_"FAMILY_2", (citizen)->family2, sizeof (citizen)->family1), \
	new TString(prefix_"EGN", (citizen)->egn, sizeof (citizen)->egn)

// ----- TXferTable ----------------------------------------------------------
struct TXferTKey : public TXferKey
{
	long courtSender;
};

class TXferTable : public TGroup
{
public:
	TXferTable(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// == TXferTable exact
	void FloatCriteria(char *s) { TGroup::FloatCriteria(s); }

	TXferTKey key;
};

// ----- TXferSide -----------------------------------------------------------
class TXferSide : public TXferTable
{
public:
	TXferSide(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void Print(mstr &m);
	virtual void Load(const char *involvements);

	void Init(const char *involvements);
	bool operator<=(const TXferSide &that) const { return sideId <= that.sideId; }

	TXferKey appeal;
	long sideInvolvement;

	long sideId;
	char ucn[SIZE_OF_UCN];
	char ucnType;

	char involvement;
};

// ----- TXferSideCitizen ----------------------------------------------------
class TXferSideCitizen : public TXferSide
{
public:
	TXferSideCitizen();
	static TGroup *Clone();

	virtual void Print(mstr &m);
	virtual void Load(const char *involvements);

	TXferCitizen side;
};

// ----- TXferFirm -----------------------------------------------------------
class TXferSideFirm : public TXferSide
{
public:
	TXferSideFirm();
	static TGroup *Clone();

	virtual void Print(mstr &m);
	virtual void Load(const char *involvements);

	char sideName[SIZE_OF_FIRM_NAME];
	char sideBulStat[SIZE_OF_BULSTAT];
};

// ----- TXferConnect --------------------------------------------------------
class TXferDocument;

#define NEW_XFER_CONNECT(infix, connect) \
	new TLong("F_"infix"_TYPE", &(connect)->connectType), \
	new TLong("F_"infix"_COURT", &(connect)->connectCourt), \
	new TLong("F_"infix"_KIND", &(connect)->connectKind), \
	new TLong("F_"infix"_NO", &(connect)->connectNo), \
	new TLong("F_"infix"_YEAR", &(connect)->connectYear)

struct TXferConnectData
{
	bool Init(const TConnect &connect);

	long connectType;
	long connectCourt;
	long connectKind;
	long connectNo;
	long connectYear;
};

class TXferConnect : public TXferTable, public TXferConnectData
{
public:
	TXferConnect();
	static TGroup *Clone();

	bool Init(const TXferDocument *document);
	bool Init(const TXferDocument *document, const TOutReg *outReg);

	TConnect connect;
};

// ----- matchers ------------------------------------------------------------
static bool loadMatch(int type, const char *name)
{
	return electric_matches(name, type, true, true) || (type == TYPE_TEXT && html_name(name));
}

static bool NonMatchingFile(const char *name)
{
	if (!any(name))  // no file
		return false;

	for (int type = 0; type < TROUBLE_COUNT; type++)
		if (loadMatch(type, name))
			return false;  // matching file

	return true;  // non-matching file
}

// ----- TXferAppeal ---------------------------------------------------------
class TXferAppeal : public TXferTable
{
public:
	TXferAppeal();
	static TGroup *Clone();

	bool Init(TInReg *inReg);
	bool NonMatchingFile() { return ::NonMatchingFile(appealFile); }

	TXferKey appeal;
	char appealFile[SIZE_OF_XFER_PATH];
};

// ----- TXferDocument -------------------------------------------------------
class TXferDocument : public TXferTable
{
public:
	TXferDocument();
	~TXferDocument();

	static TGroup *Clone();

	virtual bool Delete(bool exact);
	bool Delete();
	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// all received
	void FloatCriteria(char *s) { TGroup::FloatCriteria(s); }
	virtual void Print(mstr &m);

	bool Init(const TOutReg *outReg);
	void Load(TRequest *request, TGArray<TConnect> *connects, TElectricTrouble *trouble);
	void LogAny(const char *what, const char *text, char gop = LOG_TRANSFER);

	char eisppNo[SIZE_OF_EISPP_NMR];
	TXferKey indoc;
	char indocFile[SIZE_OF_XFER_PATH];
	TXferKey outreg;
	char returnResult[SIZE_OF_VSS_INDEX];
	long courtReceiver;
	char xferType;
	long io;

	TRCDKeyContainer *container;
	char requestKind;
	TXferConnect connect;

	TGArray<TXferAppeal> appeals;
	TConnect primary;
	void Accept(const TRequest *request);
	bool CreateSurround(const TRequest *request, TSurround *surround, const char *what);
	void AutoAppealSides(TXferSide *side, TXferAppeal *appeal, TSurround *surround, const char *involvements);
};

// ----- TXferDecision -------------------------------------------------------
class TXferDecision : public TXferTable
{
public:
	TXferDecision();
	static TGroup *Clone();

	bool Init(const TInReg *inReg, bool motivable);
	bool Init(const TOutReg *outReg);

	long decisionKind;
	long decisionNo;
	CXferDate decisionDate;
	char decisionFile[SIZE_OF_XFER_PATH];
	TXferCitizen judge[3];
	char motivesFile[SIZE_OF_XFER_PATH];
};

// ----- functions -----------------------------------------------------------
static bool xfer_name(const TRCDKeyContainer *container, char *file, int type, bool shorten = false)
{
	char name[SIZE_OF_PATH];

	if (strchr(KIND_ENDOCS, container->kind) && ((const TDecision *) container)->surrouendoc)
		container = ((const TDecision *) container)->surrouendoc;

	electric_name(container, name, type);

	if (exist(name))
	{
		char tName[SIZE_OF_PATH];
		strcpy(file, name_shortened(name, tName, shorten) ? tName : name);
		return true;
	}

	return false;
}

static void xfer_name(const TRCDKeyContainer *container, char *file, bool shorten = false)
{
	for (int i = 0; i < TROUBLE_COUNT; i++)
		if (xfer_name(container, file, i, shorten))
			break;
}

static char xferKind2Kind(long xferKind, char type = TYPE_NULL)
{
	msql m;

	Kind->XferCriteria(m, xferKind, type);
	TQuery q(str(m));
	if (q.Read())
	{
		*Kind << q;
		return Kind->kind;
	}

	return '\0';
}

static constant MULTI_DOC = "Вида на входящия документ не може да бъде определен. "
	"Необходимо е да изберете вид на входящия документ";

static char xferKind2indocKind(long xferKind, char college)
{
	msql m;
	char kind = '\0';
	int count = 0;
	TKind tKind;

	tKind.XferCriteria(m, xferKind, TYPE_REQUEST);
	TQuery q(str(m));
	while (q.Read())
	{
		tKind << q;

		for (const char *lawKind = tKind.lawKinds; *lawKind; lawKind++)
		{
			if (kind2type(*lawKind) != college)
				continue;

			if (++count >= 2)
				return message("%s.", MULTI_DOC);

			kind = tKind.kind;
		}
	}

	return kind;
}

static long sender2Court(const char *ucn, char ucnType)
{
	TSender sender;

	sender.ucnType = ucnType;
	strcpy(sender.ucn, ucn);
	if (sender.Get() && sender.court)
		return sender.court;

	mstr m;
	m.printf("%c %s липсва или няма код на съда", ucnType, ucn);
	log(LOG_TRANSFER, 0, str(m));
	return 0;
}

static void load_names(TElectricTrouble *trouble, const char *name)
{
	if (any(name))
	{
		for (int type = 0; type < TROUBLE_COUNT; type++)
		{
			char tName[SIZE_OF_PATH];
			strcpy(tName, trouble->name[type]->S());

			if ((!any(tName) || !strncmpi(tName, XFER_PATH, XFER_LEN)) && loadMatch(type, name))
			{
				trouble->name[type]->SetS(name);
				break;
			}
		}
	}
}

static bool xfer_file(const TRCDKeyContainer *container, const char *name, int extraNo = -1)
{
	if (any(name))
	{
		int type;
		bool matches = false;

		for (type = 0; type < TROUBLE_COUNT; type++)
		{
			if (electric_matches(name, type, true, false))
			{
				matches = true;
				break;
			}
		}

		if (matches)	// keep secondary type exts
			matches = electric_matches(name, type, true, true);
		else
			type = TYPE_IMAGE;

		const char *dir = Electrics[type].docketPath;

		if (exist(dir, false))
		{
			char tName[SIZE_OF_PATH];
			const char *ext = file_ext(name);

			electric_name(container, dir, tName, type, extraNo);

			if (!matches && any(ext))		// restore original ext
				strcpy(file_ext(tName), ext);

			return copy_overead_file(name, tName, false, true);
		}

		mstr m;
		m.printf("грешка %s:%d достъп", dir + (strlen(dir) > 20 ? strlen(dir) - 20 : 0), errno);
		log(LOG_TRANSFER, CM_REG_NEW, container, str(m));
	}

	return false;
}

static void xfer2sender(char *ucn, const char ucnType, const long court)
{
	TSender sender;
	msql m(sender.Name, "F_UCN");

	m.AddChar("F_UCN_TYPE", ucnType);
	m.AddLong("F_COURT", court);

	TQuery q(str(m));
	if (q.Read())
	{
		sender << q;
		strcpy(ucn, sender.ucn);
	}
	else
		fatal("%d: липсващ адресант %s", court, UCNType->Seek(ucnType));
}

static void session2Judges(const TSession *session, const char *judges[])
{
	constant source[4] = { session->judge, session->judgeX, session->reJudgeX, session->president };
	int i, k;

	judges[0] = judges[1] = judges[2] = EMPTY;
	for (i = k = 0; k < 4; k++)
	{
		if (!atob(source[k]))
			continue;

		int j;

		for (j = 0; j < i; j++)
			if (!strcmp(judges[j], source[k]))
				break;

		if (j == i)
		{
			judges[i++] = source[k];
			if (i == 3)
				break;
		}
	}
}

static unsigned alnum_to_6bit(char c)
{
	int index = c ? strindex(" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", c) : 0;

#if RANGECHECK
	if (index == INT_MAX)
		fatal("xfer_uuid: invalid (ucn)type %d", c);
#endif  // RANGECHECK

	return index;
}

const
	LENGTH_OF_HALF_UCN = (LENGTH_OF_UCN / 2),
	SIZE_OF_HALF_UCN = LENGTH_OF_HALF_UCN + 1;

static const char xfer_safe_chars[SURROUNDMENTS_MAX + 1] =
{
	'\0', '\1', '\2', '\3', '\4', '\5', '\6', '\7', '\10', '\11', '\12',
	'\13', '\14', '\15', '\16', '\17', '\20', '\21', '\22', '\23', '\24',
	'\25', '\26', '\27', '\30', '\31', '\32', '\33', '\34', '\35', '\36',
	'\37', '\40', '\41', '\42', '\43', '\44', '\45', '\46', '\50',
	'\51', '\52', '\53', '\54', '\55', '\56', '\57', '\60', '\61', '\62',
	'\63', '\64', '\65', '\66', '\67', '\70', '\71', '\72', '\73', '\74',
	'\75', '\76', '\77', '\100', '\101', '\102', '\103', '\104', '\105',
	'\106', '\107', '\110', '\111', '\112', '\113', '\114', '\115',
	'\116', '\117', '\120', '\121', '\122', '\123', '\124', '\125',
	'\126', '\127', '\130', '\131', '\132', '\133', '\134', '\135',
	'\136', '\137', '\140', '\141', '\142', '\143', '\144', '\145',
	'\146', '\147', '\150', '\151', '\152', '\153', '\154', '\155',
	'\156', '\157', '\160', '\161', '\162', '\163', '\164', '\165',
	'\166', '\167', '\170', '\171', '\172', '\173', '\174', '\175',
	'\176', '\200', '\201', '\202', '\203', '\204', '\205',
	'\206', '\207', '\210', '\211', '\212', '\213', '\214', '\215',
	'\216', '\217', '\220', '\221', '\222', '\223', '\224', '\225',
	'\226', '\227', '\231', '\232', '\233', '\234', '\235',
	'\236', '\237', '\240', '\241', '\242', '\243', '\244', '\245',
	'\246', '\247', '\250', '\251', '\252', '\253', '\254', '\255',
	'\256', '\257', '\260', '\261', '\262', '\263', '\264', '\265',
	'\266', '\267', '\270', '\271', '\272', '\273', '\274', '\275',
	'\276', '\277', '\300', '\301', '\302', '\303', '\304', '\305',
	'\306', '\307', '\310', '\311', '\312', '\313', '\314', '\315',
	'\316', '\317', '\320', '\321', '\322', '\323', '\324', '\325',
	'\326', '\327', '\330', '\331', '\332', '\333', '\334', '\335',
	'\336', '\337', '\340', '\341', '\342', '\343', '\344', '\345',
	'\346', '\347', '\350', '\351', '\352', '\353', '\354', '\355',
	'\356', '\357', '\360', '\361', '\362', '\363', '\364', '\365',
	'\366', '\367', '\370', '\371', '\372', '\373', '\374', '\375'//,
	//'\376', '\377'
};

static void xfer_uuid(char *id, long no, long year, char type, char kind, const CDate *date, const CTime *time,
	char involvement, char ucnType, const char *ucn)
{
	unsigned typeIndex = alnum_to_6bit(type);
	unsigned v1 = ((unsigned) no << 12) + (year ? ((unsigned) (year - YEAR_MIN) << 5) + 1 : 0) + (typeIndex >> 3);

	if (!date)
		date = &DATE_MIN;
	if (!time)
		time = &TIME_MIN;

	unsigned v2 = ((typeIndex & 0x07) << 29) + (kind << 21) + ((unsigned) date->day << 16) +
		((unsigned) date->month << 12) + ((unsigned) (date->year - YEAR_MIN) << 5) + (unsigned) time->hour;

	char ucn1[SIZE_OF_HALF_UCN], ucn2[SIZE_OF_HALF_UCN];

	if (ucn)
	{
		memcpy(ucn1, ucn, LENGTH_OF_HALF_UCN);
		ucn1[LENGTH_OF_HALF_UCN] = '\0';
		memcpy(ucn2, ucn + LENGTH_OF_HALF_UCN, LENGTH_OF_HALF_UCN);
		ucn2[LENGTH_OF_HALF_UCN] = '\0';
	}
	else
	{
		strcpy(ucn1, "00000");
		strcpy(ucn2, "00000");
	}

	unsigned v3 = ((unsigned) time->minute << 26) + (involvement << 18) + (alnum_to_6bit(ucnType) << 12) +
		((unsigned) atol(ucn1) >> 5);
	unsigned v4 = ((unsigned) (atol(ucn1) & 0x1F) << 27) + ((unsigned) atol(ucn2) << 10) + (unsigned) COURT_CODE;

	sprintf(id, "%08X%08X%08X%08X", v1, v2, v3, v4);
}

static void xfer_uuid(char *id, TRCDKeyContainer *container, char involvement, char ucnType, const char *ucn)
{
	xfer_uuid(id, container->key.no, container->key.year, container->key.type, container->kind, &container->date,
		NULL, involvement, ucnType, ucn);
}

#if TESTVER
void xfer_uuid(char *id, TRCDKeyContainer *container) { xfer_uuid(id, container, '\0', '\0', NULL); }
#else  // TESTVER
static inline void xfer_uuid(char *id, TRCDKeyContainer *container) { xfer_uuid(id, container, '\0', '\0', NULL); }
#endif  // TESTVER

static void xfer_uuid(char *id, TOutReg *outReg, TInReg *inReg)
{
	char inreg_ucn[SIZE_OF_UCN];
	sprintf(inreg_ucn, "%06d%04d", inReg->key.no, inReg->key.year);
	xfer_uuid(id, outReg, '\0', '\0', inreg_ucn);
}

static const char *xfer_type_name(char xferType)
{
	struct TXferTypeName
	{
		char xferType;
		const char *name;
	};

	static const TXferTypeName XferTypeNames[] =
	{
		{ UCN_INSTANCE_HIGHER, "За обжалване" },
		{ UCN_INSTANCE_EQUAL, "По подсъдност" },
		{ UCN_INSTANCE_LOWER, "Връщане" },
		{ UCN_INSTANCE_CONFLICT, "За определяне" },
		{ UCN_INSTANCE_COMPETENT, "По компетентност" },
		{ '\0', "" }
	};

	const TXferTypeName *xtn;

	for (xtn = XferTypeNames; xtn->xferType; xtn++)
		if (xtn->xferType == xferType)
			break;

	return xtn->name;
}
