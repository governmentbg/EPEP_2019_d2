// ----- TRequestResolution --------------------------------------------------
class TRequestResolution : public TIndexContainer
{
public:
	TRequestResolution();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);

	virtual void Involvements(mstr &m) const;
	virtual char IndexType() const;
	virtual char IndexKind() const;
	virtual char Status() const { return resolution; }

	char resolution;
	mstr text;
	char judge[SIZE_OF_UCN];
	char assign[SIZE_OF_UCN];	// 2007:180 LPR: was in TAssignment
	long lawChoiceNo;			// 2014:232 +related
};

// ----- TRequest ------------------------------------------------------------
class TXferDocument;

class TRequest : public TOriginContainer
{
public:
	TRequest();
	static TGroup *Clone();

	virtual char LawKind() const;
	virtual bool Generate();	// 2015:278
	virtual bool Insert();
	virtual char Status() const;
	TRequestResolution *LastResolution(const char *fields) const;

	char text[SIZE_OF_ORIGIN_TEXT];

	CTime time;
	char direction[SIZE_OF_DIRECTION];	// 2010:048
	char lawKind;

	TXferDocument *xfer;

protected:
	virtual bool Latest(TRCDKeyContainer *);	// false
};

// ----- TWindData -----------------------------------------------------------
class TWindData : public TUCNGroup
{
public:
	TWindData(const char *name, TGroup *(*const tClone)());

	void CopyTo(const TRCDKeyContainer *target, bool override);

	TRCDKey key;
	char kind;
	CDate date;
	char involvement;
};

// ----- TBaseWind -----------------------------------------------------------
struct TWindIndex
{
	const char *name;
	int table;
};

extern const TWindIndex WIND_INDEXES[];

class TBaseWind : public TWindData
{
public:
	TBaseWind(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	void Transfer(const TRCDKeyContainer *source, const TRCDKeyContainer *target, const char *involvements,
		TBaseWind *baseWind = NULL);
	void PrintGroup(mstr &m) { PrintGroup(m, false); }
	void PrintNames(mstr &m) { PrintGroup(m, true); }
	void PrintIdent(mstr &m);	// UCN | EIK/BULSTAT
	bool SameWind(const TBaseWind *that) const;	// ucn, ucnType, involvement
	bool SameWind(const TBaseWind &that) const { return SameWind(&that); }
	virtual long Excess() const;	// 2017:167

protected:
	void PrintGroup(char *s, bool cut);		// cut leading UCN <UCN>
	void PrintGroup(mstr &m, bool cut);		// cut leading UCN <UCN>
};

// ----- TSideWind -----------------------------------------------------------
class TSideWind : public TBaseWind
{
public:
	TSideWind();
	TSideWind(const char *name, TGroup *(*const tClone)());
	static TGroup *Clone();

	bool Appealer();
	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// sorted by order
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Print(mstr &m);
	virtual void PrintState(mstr &m);	// 2011:308 LPR: method
	unsigned Key() const;
	bool Represents(const TLinkWind *linkWind) const;
	bool Represents(const TSideWind *sideWind, TGArray<TLinkWind> *links) const;
	int LoadSides(TRCDKeyContainer *container, TGArray<TSideWind> *left, TGArray<TSideWind> *right,
		TGArray<TSideWind> *other = NULL);
	// 2016:349 LPR: minor/infant/... support
	static constant ChildText(long flags);
	constant ChildText() { return ChildText(flags); }

	virtual void Generate();
	virtual bool Insert();

	char state;
	long order;
	long flags;
	char fromSide;
	char unwanted[SIZE_OF_UNWANTED];	// 2008:162 LRQ: +related

	long investId;
	long invExpId;

	char eventStatus;		// 2010:082 LPR: memory only, lists only

private:
	void Init();
};

// 2010:074 LPR: explicit array, for compatilibity
class TSideWindArray : public TGArray<TSideWind>
{
public:
	TSideWindArray() { lesseql = LessEqlT; }

	bool Contains(const TSideWind *sideWind) const { return TGroupArray::Contains(sideWind); }
	int Find(const TSideWind *sideWind) const { return TGroupArray::Find(sideWind); }

protected:
	static bool LessEqlT(const long t1, const long t2) { return ((TSideWind *) t1)->Key() <= ((TSideWind *) t2)->Key(); }
};

// ----- TMovement -----------------------------------------------------------
class TMovement : public TDatedContainer
{
public:
	TMovement();
	static TGroup *Clone();

	virtual char Status() const { return result; }
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual bool Insert();

	bool operator<=(const TMovement &that) const;

	char result;
	char text[SIZE_OF_MOVEMENT_TEXT];
	CTime time;
	char ucn[SIZE_OF_UCN];
};

// ----- TLawsuit ------------------------------------------------------------
class TLawsuit : public TOriginContainer
{
public:
	TLawsuit();
	static TGroup *Clone();

	virtual void Involvements(mstr &m) const;
	virtual bool Suitor(char involvement);
	virtual char Status() const;
	bool PostPersonal() const;	// 2017:305

	virtual char LawKind() const { return kind; }
	virtual bool Generate();	// 2015:278
	virtual bool Insert();
	virtual bool Update(const char *fields = NULL);

	bool TryBy(const TRCDKey &source, const char *fields);
	bool TryBy(const TRCDKeyGroup *request, const char *fields) { return TryBy(request->key, fields); }
	static TExternContainer *NewGroup(long excess);
	bool LimitedNotAdmin() const { return (flags & LAWSUIT_LIMITED) && Default->post != POST_ADMIN; } 

	TRCDKey source;
	char sourceKind;
	long composition;
	char judge[SIZE_OF_UCN];
	char text[SIZE_OF_ORIGIN_TEXT];

	// Finalizing data
	char age;
	CDate finished;
	char grant;
	CDate decMade;

	CDate returnedDate;
	CDate previousDate;
	CDate receivedDate;
	CDate forceDate;
	CDate completed;

	long flags;
	long flagsX;
	mstr important;

	char rejudge[SIZE_OF_UCN];
	long danger;			// 2008:262
	CDate firstSet;			// 2009:201
	long cutDown;			// 2014:163 was LAWSUIT_CUTDOWN +related
	bool autoNumbering;		// 2015:278 LPR: memory only, create only
	long flagsY;			// 2018-05-28

protected:
	virtual bool Latest(TRCDKeyContainer *container);
};

// ----- TBaseSession --------------------------------------------------------
class TBaseSession : public TIndexContainer
{
public:
	TBaseSession(const char *name, TGroup *(*const tClone)());

	virtual char Status() const { return result; }
	virtual long Excess() const = 0;
	static const char *ExcessText(long excess);
	const char *ExcessText() const { return ExcessText(Excess()); }
	void GetLast(const char *fields);
	static TBaseSession *NewGroup(long excess);

	CTime time;
	CDate end;
	CTime entime;
	long composition;
	char result;

	char judge[SIZE_OF_UCN];
	char secretar[SIZE_OF_UCN];
	char juror[SIZE_OF_UCN];
	char reJuror[SIZE_OF_UCN];
	char jurorX[SIZE_OF_UCN];
	char reserve[SIZE_OF_UCN];
};

// ----- TSession ------------------------------------------------------------
class TSession : public TBaseSession
{
public:
	TSession(TGroup *(*const tClone)() = TSession::Clone);
	static TGroup *Clone();

	// 2009:224 returns s; 2011:314 LRQ: lawsuit (kind)
	const char *DecisionKinds(mstr &m, const TLawsuit *lawsuit);
	// 2012:011 special use only, subject only; 2017:304 pre-check possible attributes
	bool PersonalData();
	const char *Accomplys() const;	// 2016:165
	bool DefaultPublic() const;		// 2016:168

	// 2018-10-05; 2018-10-19: IRQ: rewrite
	bool IsEPortSessEndoc() const;
	bool IsEPortSession() const;

	virtual char IndexType() const;
	virtual char IndexKind() const;
	virtual long Excess() const;

	virtual bool Insert();
	virtual bool Update(const char *fields = NULL);

	char prosecutor[SIZE_OF_UCN];
	mstr text;

	char judgeX[SIZE_OF_UCN];
	char reJudgeX[SIZE_OF_UCN];

	char president[SIZE_OF_UCN];
	CDate returned;
	long hall;

	long flags;
	long flagsX;
	CDate final;		// 2008:197
	CDate forceDate;		// 2011:342
	long slcReason;		// 2015:055
	long flagsY;

	long jurors;		// 2009:217 memory only
	TGroupArray xfer;
	CTime startime;		// 2012:248 memory only
};

// ----- TCitizenNames -------------------------------------------------------
struct TCitizenNames
{
	char name[SIZE_OF_CITIZEN_NAME];
	char family[SIZE_OF_CITIZEN_FAMILY];
	char reName[SIZE_OF_CITIZEN_RENAME];
	char reFamily[SIZE_OF_CITIZEN_REFAMILY];
	char rank[SIZE_OF_CITIZEN_RANK];		// 2008:353

	const char *Rank() const;			// 2008:353 LPR: rank + ' '
	bool CheckLiterate();				// 2013:071

protected:
	TCitizenNames(TGroup *self);

	mutable char rank_[SIZE_OF_CITIZEN_RANK + 1];	// 2008:353 Rank() buffer
};

// ----- TCitizen ------------------------------------------------------------
class TCitizen : public TNamedGroup, public TCitizenNames
{
public:
	TCitizen();
	TCitizen(const char *tName, TGroup *(*const tClone)());
	static TGroup *Clone();

	virtual void Print(mstr &m);
	virtual bool Delete(bool exact);				// Rejects high priority
	virtual bool Update(const char *fields = NULL);		// Rejects high priority
	virtual bool ValidCriteria();
	virtual void NamedCriteria(msql &m, const char *fields, const char *extra, char involvement, bool active);

	char country[SIZE_OF_COUNTRY];
	char reCountry[SIZE_OF_COUNTRY];
	char lnc[SIZE_OF_UCN];

private:
	void Init();
};

// ----- TFirm ---------------------------------------------------------------
class TFirm : public TNamedGroup
{
public:
	TFirm();
	TFirm(const char *tName, TGroup *(*const tClone)());
	static TGroup *Clone();

	virtual void Print(mstr &m);
	virtual bool ValidCriteria();
	virtual void NamedCriteria(msql &m, const char *fields, const char *extra, char involvement, bool active);

	char name[SIZE_OF_FIRM_NAME];
	char ufn[SIZE_OF_UCN];
	char bulstat[SIZE_OF_BULSTAT];
	char country[SIZE_OF_COUNTRY];

private:
	void Init();
};

// ----- TDamn ---------------------------------------------------------------
class TDamn : public TNamedGroup
{
public:
	TDamn();
	static TGroup *Clone();

	virtual void Print(mstr &m);
	virtual bool ValidCriteria();
	virtual void NamedCriteria(msql &m, const char *fields, const char *extra, char, bool);

	char name[SIZE_OF_DAMN_NAME];
	char companyType;
	char taxNo[SIZE_OF_UCN];
	char bulstat[SIZE_OF_BULSTAT];

	TRCDKey key;
	CDate reRegister;		// 2008:039
};

// ----- TSender -------------------------------------------------------------
class TSender : public TNamedGroup
{
public:
	TSender();
	TSender(TKnownPerson *person);
	static TGroup *Clone();

	virtual void Print(mstr &m);
	virtual bool ValidCriteria();
	virtual void NamedCriteria(msql &m, const char *fields, const char *extra, char, bool);

	char name[SIZE_OF_NAME];
	long court;
	long eispp;		// 2012:277 LRQ: rewrite: +related: instead of re#
	long flags;		// 2015:166 LPR: must be visible for update

private:
	void Init();
};

// ----- TReceiver -----------------------------------------------------------
class TReceiver : public TStringAliasGroup	// == T_SENDER for combo boxes
{
public:
	TReceiver();
	static TGroup *Clone();

	virtual void AliasCriteria(msql &m, const char *xCrit, long flags);

	char ucnType;
	char ucn[SIZE_OF_UCN];
	char name[SIZE_OF_NAME];
};

extern TReceiver *Receiver;

// ----- TFixeiver -----------------------------------------------------------
class TFixeiver : public TReceiver
{
public:
	TFixeiver(char tUCNType);

	virtual void AliasCriteria(msql &m, const char *xCrit, long flags);

	const char *ucns[FIXEIVER_UCNS_MAX];
};

// ----- TAnyAddress ---------------------------------------------------------
class TAnyAddress : public TUCNGroup
{
public:
	TAnyAddress(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);	// 2011:260
	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// 2011:270 FIX: was missing
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void MakeLines(long flags = 0) = 0;
	virtual void MakeExtra(long flags);

	void Print(mstr &m, long flags);
	virtual void Print(mstr &m);				// flags ADDRESS_EXTRAS
	void PrintExtra(long flags = ADDRESS_EXTRAS);	// print into ExtraText

	virtual void Generate();
	void Generate(const char *minMax);

	static TAnyAddress *NewGroup(bool foreign);
	static void LoadAddresses(TUCNGroup *ucnGroup, TGSArray<TAnyAddress> *addresses, bool foreign);
	static void LoadAddresses(TUCNGroup *ucnGroup, TGSArray<TAnyAddress> *addresses);

	bool operator<=(const TAnyAddress &that) { return Order() <= that.Order(); }

	long address;	// positive buglarian, negative foreign

	char line1[SIZE_OF_ADDRESS_LINE];
	char line2[SIZE_OF_ADDRESS_LINE];
	char line3[SIZE_OF_ADDRESS_LINE];
	char extra[SIZE_OF_ADDRESS_LINE];	// 2011:259 LPR: size _EXTRA -> LINE

protected:
	long Order() const { return address >= 0 ? address : 100 - address; }
};

// ----- TAddressData --------------------------------------------------------
struct TAddressData
{
	void AddFields(TGroup *self);

	long uclp;
	long street;
	char adrNo[SIZE_OF_ADR_NO];
	char subNo[SIZE_OF_SUB_NO];
	char entrance[SIZE_OF_ENTRANCE];
	char floor[SIZE_OF_FLOOR];
	char apartment[SIZE_OF_APARTMENT];
	char blockNo[SIZE_OF_BLOCK_NO];

	void GetUCLP();
};

// ----- TAddress ------------------------------------------------------------
class TAddress : public TAnyAddress, public TAddressData
{
public:
	TAddress();
	static TGroup *Clone();

	virtual bool Insert();
	void MakeLines(long flags = 0);
	void MakeExtra(long flags);
	const char *Match(int addrMode) const;

	char phone[SIZE_OF_PHONE];
	char telex[SIZE_OF_TELEX];
	char fax[SIZE_OF_FAX];
	char email[SIZE_OF_EMAIL];
	char remark[SIZE_OF_ADDRESS_REMARK];
};

// ----- TForeignAddress -----------------------------------------------------
class TForeignAddress : public TAnyAddress
{
public:
	TForeignAddress();
	static TGroup *Clone();

	virtual bool Insert();
	void MakeLines(long = 0);

	char country[SIZE_OF_COUNTRY];
	char foreign[SIZE_OF_FOREIGN_ADDRESS];
};

// ----- TTryAddress ---------------------------------------------------------
class TTryAddress
{
public:
	TTryAddress(TAnyAddress *tAddress);
	virtual ~TTryAddress() { delete address; }

	TAnyAddress *address;

protected:
	bool triedAddress;

	void TryAddress(const char *ucn, char ucnType);
};

// ----- TLegalSide ----------------------------------------------------------
class TLegalSide : public TSideWind, public TTryAddress
{
public:
	TLegalSide();
	static TGroup *Clone();

	void TryAddress() { TTryAddress::TryAddress(ucn, ucnType); }
	virtual void Print(mstr &m);	// involvement - names - address

	char specifiedLawer[SIZE_OF_LEGAL_AID_LAWER];
};

// ----- TDecision -----------------------------------------------------------
struct TDecResData
{
	TDecResData() : appealedKindLoaded(false), subjectAttribLoaded(false) { }

	char appealedKind;
	bool appealedKindLoaded;
	char subjectAttrib[SIZE_OF_SUBJECT_ATTRIB];
	bool subjectAttribLoaded;
};

class TSurroundment;
class TInReg;

class TDecision : public TIndexContainer
{
public:
	TDecision(TGroup *(*const tClone)() = TDecision::Clone);
	static TGroup *Clone();

	// 2017:300 [session], const; 2017:304 pre-check possible attributes, inline PersonalKind
	bool PersonalData(const TSession *session = NULL) const;
	bool PreferMotives(const TLawsuit *lawsuit) const;		// 2017:241
	const char *Punishments(TLawsuit *lawsuit);
	virtual char Status() const { return result; }
	virtual char IndexType() const;
	virtual char IndexKind() const;

	// 2009:072 +lawsuit +session, internal optimization
#if COURT_TYPE == COURT_AREAL
	const char *Accomplys(const TLawsuit *lawsuit, const TSession *session) const;
#endif  // AREAL
	char JudicialActType(TLawsuit *lawsuit, TSession *session);
	static char StandardResult(char kind);
	char StandardResult() { return StandardResult(kind); }
	const char *SpecificResults(TLawsuit *lawsuit, TSession *session, TDecResData *data);
	const char *ExtraPunishments(TLawsuit *lawsuit);
	bool SeekSubject(TLawsuit *lawsuit);
	bool Motivable();		// 2010:096 For initial check
	bool Motivated();

	virtual bool Insert();
	virtual bool Update(const char *fields = NULL);

	virtual bool Combine(TRCDKeyContainer *source);
	CDate MinReturned(bool exself) const;	// 2012:082
	// 2017:051 cp from utility, may be used by export csv
	int GetPriority(const TSession *session, const char *priorityResults = NULL) const;
	// 2018-11-14: LPR: moved here from index container
	void OutRegIndexes(mstr &m, const char *sep = NULL) const;  // default ", "

	CDate fromDate;
	char result;
	CDate returned;
	mstr text;

	CDate enterDate;
	char sessionKind;

	long registry;		// register is a reserved word
	long volume;
	long page;
	long partition;
	long level;

	char accomplys[SIZE_OF_ACCOMPLYS];
	mstr syndicate;

	long flags;
	long flagsX;		// 2016:166
	mstr motives;
	CDate forceDate;		// 2008:038
	long execKind;		// 2008:074 kind-compatible
	double damages;		// 2008:274 +related
	mstr opinion;		// 2009:128

	CDate completed;		// 2009:183 memory, querydoc only
	char debtorStatus;	// 2010:340
	long slcReason;		// 2015:063
	// 2016:119 memory, xfer only
	const TSurroundment *surrouendoc;
	void Init(const TSurroundment *tSurrouendoc);	// 2016:176

	char keyWords[SIZE_OF_KEYWORDS];	// 2017:081
	bool CopyTo(const CDate &tDate);	// 2017:089
	// 2017:300+
	CDate publicDate;
	sign Appealed() const;  // 2017:342 LPR: -1 = 2+ times
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void P64Check(mstr &m, const TGroupArray *judgements, char lawsuitKind);
	bool P64Wrong(const TLawsuit *lawsuit, const TInReg *inReg, bool appealedCheck);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	bool P64Connect(char lawsuitKind) const;
	bool P64Direct(bool appealedCheck = true, char tKind = '\0') const;
	bool virtualAct;		// 2018-03-28 xfer uuid
};

void CreateEndocResults();

// ----- TDecisionRider ------------------------------------------------------
class TDecisionRider : public TDecision	// motives, opinion
{
public:
	TDecisionRider(TDecision *tDecision, char tKind);
	TDecision *decision;	// original
};

// ----- TJudgement ----------------------------------------------------------
class TJudgement : public TSideWind
{
public:
	TJudgement();
	static TGroup *Clone();

	virtual void Print(mstr &m);
	virtual void PrintState(mstr &m);	// 2011:308 LPR: method
	virtual char Status();

	char judged;
	char punishment;
	char recidive;

	mstr text;

	CDate forceDate;
	CDate sendDate;
	char sendTo[SIZE_OF_JUDGEMENT_SEND_TO];
	CDate execDate;
	char execBind[SIZE_OF_JUDGEMENT_EXEC_BIND];
	char execPlace[SIZE_OF_JUDGEMENT_EXEC_PLACE];

	long flagsX;	// to avoid collision with side wind flags
	long flagsY;	// 2017:122 +related

	char forgive[SIZE_OF_JUDGEMENT_FORGIVE];
	char orderFrom[SIZE_OF_JUDGEMENT_ORDER_FROM];
	CDate givenDate;
	char sendText[SIZE_OF_JUDGEMENT_SEND_TEXT];
	CDate countDate;

	char interval;
	char execRemark[SIZE_OF_JUDGEMENT_REMARK];
	mstr execText;
	char punishmentsExtra[SIZE_OF_PUNISHMENTS];
	char jailMode;
	long article;		// 2007:288
	CDate notifyDate;		// 2017:300
};

// ----- TSurround -----------------------------------------------------------
class TSurround : public TPrimeContainer
{
public:
	TSurround();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Involvements(mstr &m) const;
	TSurroundment *LastSurroundment(const char *fields) const;

	virtual bool Generate();
	virtual bool Insert();
	virtual char Status() const;
	virtual bool Update(const char *fields = NULL);

	char college;
	char text[SIZE_OF_ORIGIN_TEXT];
	char judge[SIZE_OF_UCN];
	CTime time;
	char decisionKind;			// 2009:134
	CDate decisionDate;			// 2009:134
	char direction[SIZE_OF_DIRECTION];	// 2010:050

protected:
	bool xfer;					// 2009:195 true -> don't copy sides
	friend class TXferDocument;

	virtual bool Latest(TRCDKeyContainer *container);
};

// ----- TSurroundment -------------------------------------------------------
class TSurroundment : public TIndexContainer
{
public:
	TSurroundment();
	static TGroup *Clone();

	void BasicCriteria(msql &m, const char *fields = NULL);
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Print(mstr &m);
	virtual char Status() const { return result; }

	void Init(const TSurround *surround, const TSession *session);
	void SessionCriteria(msql &m);
	void SessionCriteria(msql &m, const TRCDKey &key, char kind, const CDate &date);
	void SessionCriteria(msql &m, const TSession *session);
	bool ClearSurrouendoc();
	const char *SurrouendocResults();

	virtual bool Delete(bool exact);	// 2016:130 FIX: no signal
	virtual char IndexType() const;
	virtual char IndexKind() const;
	virtual bool Insert();
	virtual bool Update(const char *fields = NULL);	// 2016:173 FIX: no signal

	CDate &sessionDate;
	char decisionKind;
	CDate decisionDate;
	long extraNo;

	TRCDKey lawsuit;
	char sessionKind;
	char result;
	mstr text;

	char accomplys[SIZE_OF_ACCOMPLYS];	// 2016:165
	char debtorStatus;			// 2016:165
	long flags;					// 2016:167
	long flagsX;				// 2016:167
};

// ----- TLog ----------------------------------------------------------------
class TLog : public TGroup
{
public:
	TLog(TGroup *(*const tClone)() = TLog::Clone);
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual bool Insert();
	virtual void Print(mstr &m);
	virtual bool Update(const char *fields = NULL);

	void CheckMD5();
	bool ResBlock();
	// 2018-07-04: for extern announces
	bool IsAnnounce() const;
	bool TypeName(mstr &m) const;  // no Seek

	long log;
	char ucn[SIZE_OF_UCN];
	char gop;
	long opc;
	CDateTime stamp;
	TRCDKey key;
	char kind;
	CDate date;

	char text[SIZE_OF_LOG_TEXT];

protected:
	long resBlock[4];

	void CalcMD5(long *tResBlock);
};

// ----- TIOReg --------------------------------------------------------------
class TIOReg : public TPrimeContainer
{
public:
	TIOReg(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual bool Generate();
	virtual bool Insert();

	char college;
	// 2016:083 from TOutReg, common w/ TInReg
	char receiver[SIZE_OF_UCN];
	char receiverType;	// 2009:161 +related; replaces retBack
};

// ----- TOutReg -------------------------------------------------------------
class TInRegEvent;
class TConnect;

class TOutReg : public TIOReg
{
public:
	TOutReg();
	static TGroup *Clone();

	virtual void Involvements(mstr &m) const;
	virtual char Status() const { return result; }	// no basic status
	void LoadInRegs(TGArray<TInRegEvent> *events, const char *eventFields, const char *inRegFields);
	bool operator<=(const TOutReg &that) const;	// lawsuit, date
	bool TryLower(TConnect &connect) const;
	bool EPortalSendable() const; 

	long inNo;
	long inYear;

	char result;
	mstr remark;
	mstr remarkVZ;	// 2011:355
	mstr remarkKS;	// 2011:355
	CDate returned;

	char resultI;
	char resultII;
	char resultKS;
	char resultMJ;

	long flags;
	char vssIndex[SIZE_OF_VSS_INDEX];	// 2008:190 +related; 2015:160 char -> string

	long flagsX;			// 2009:161 memory; 2007:167 -> disk
	char decisionKind;		// 2009:173
	CDate decisionDate;		// 2009:173 LPR/LRQ: session date as usual
	mstr text;				// 2013:067 replaces TIOReg text

	TXferDocument *xfer;
	bool aret;				// 2010:158 LPR: electric: return date
	long deliveryNotice;		// 2017:243 memory only
	bool resend;			// 2018-03-29 LPR: edit only

protected:
	virtual bool Latest(TRCDKeyContainer *);	// false, skip prime container
};

// ----- TInRegEvent ---------------------------------------------------------
class TInRegEvent : public TIndexContainer
{
public:
	TInRegEvent();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	void OutCriteria(const TOutReg *outReg);
	virtual void Print(mstr &m);
	virtual char Status() const { return result; }
	virtual char IndexType() const;
	virtual char IndexKind() const;

	char result;
	mstr text;

	long outNo;
	long outYear;
	char judge[SIZE_OF_UCN];
	CDate returned;
};

// ----- TInReg --------------------------------------------------------------
class TInReg : public TIOReg
{
public:
	TInReg();
	static TGroup *Clone();

	virtual void Involvements(mstr &m) const;
	TInRegEvent *LastEvent(const char *fields, bool sendRecv = false) const;
	bool LastReceiver(char *receiver, char *receiverType) const;	// 2016:084
	virtual char Status() const;
	// 2011:173 mandatory -> reKind, reDate, fields
	TIndexContainer *AppealedAct(bool reKind, bool reDate, const char *fields = NULL) const;
	// 2016:084 rewrite to check all data
	const char *SameComplect(const TInReg *leadInReg, char leadStatus, char status) const;
	// 2016:242 LPR: split from AppealedAct, used by EPortalSendable
	TDecision *AppealedDecision(bool reKind, bool reDate, const char *fields = NULL) const;
	sign EPortalSendable() const;		// 2018-10-03: -1 for session
	const char *EPortalModeText() const;

	virtual bool Insert();

	TRCDKey decision;
	char decisionKind;
	CDate decisionDate;

	char judge[SIZE_OF_UCN];
	char sessionKind;

	char surroundKind;
	long surroundNo;
	long surroundYear;
	CTime time;
	char direction[SIZE_OF_DIRECTION];	// 2010:048
	char text[SIZE_OF_ORIGIN_TEXT];	// 2013:076 moved from TIOReg
	long extraNo;				// 2016:175
	char eportalMail[SIZE_OF_EMAIL];	// 2016:251
	long eportalMode;				// 2016:258 was memory

protected:
	virtual bool Latest(TRCDKeyContainer *container);
};

// ----- TOldCitizen ---------------------------------------------------------
class TOldCitizen : public TCitizen
{
public:
	TOldCitizen();
	static TGroup *Clone();
};

// ----- TAccusation ---------------------------------------------------------
class TAccusation : public TUCNGroup
{
public:
	TAccusation();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	TRCDKey key;

	mstr accusation;
	char crimeForm;
	char crimeStage;
	CDate crimeDate;
	char recidive;
	char punishment;
	char resolution;
	long accusationNo;
	long flags;
	CDate arrestDate;
};

// ----- TArchive ------------------------------------------------------------
class TArchive : public TRCDKeyGroup
{
public:
	TArchive();
	static TGroup *Clone();

	// 2017:153 +F_EXCESS
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	virtual char Status() const;	// killDate ? destroyed : archived
	virtual bool Generate();
	virtual bool Insert();
	bool InsertImported() { return TRCDKeyGroup::Insert(); }  // 2017:159

	void Signal(int sigtype, const TExternContainer *lawsuit);
	const char *ExcessText() const { return excess ? "Ñ" : ""; }
	bool TryBy(const TExternContainer *lawsuit, const char *fields);

	long linkNo;
	long archiveNo;
	long archiveYear;
	CDate archiveDate;
	mstr archiveRemark;

	CDate killDate;
	mstr killRemark;
	long keep;

	long killerNo;
	CDate killerDate;
	mstr remainder;
	long remainderNo;
	long remainderYear;
	char archiveIndex[SIZE_OF_ALIAS];

	long excess;	// 2017:153
};

// ----- TProveAction --------------------------------------------------------
class TProveAction : public TDatedContainer
{
public:
	TProveAction();
	static TGroup *Clone();

	virtual void Involvements(mstr &m) const;
	virtual char Status() const { return resolution; }

	char resolution;
	long flags;
	char sessionKind;
	CDate sessionDate;
	char decisionKind;
	long outNo;
	long outYear;
	mstr text;
};

// ----- TProve --------------------------------------------------------------
class TProve : public TPrimeContainer
{
public:
	TProve();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual char Status() const;
	virtual bool Insert();

	bool TryBy(const TRCDKeyGroup *group, const char *fields = NULL);
	TProveAction *LastAction(const char *fields, const CDate *date = NULL) const;
	TProveAction *LastAction(const char *fields, const CDate &date) const { return LastAction(fields, &date); }

	mstr description;
	char remark[SIZE_OF_PROVE_REMARK];
};

// ----- TSched --------------------------------------------------------------
class TSched : public TDatedContainer
{
public:
	TSched();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	virtual bool Insert();
	// 2010:168 skip signals
	virtual bool Update(const char *fields = NULL);
	virtual bool Delete(bool exact);
	virtual void Print(mstr &m);

	char sched;
	CDate start;
	CDate final;
	char judge[SIZE_OF_UCN];

	TRCDKey lawsuit;
};

// ----- TExpense ------------------------------------------------------------
class TExpense : public TWindData
{
public:
	TExpense();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Generate();
	virtual bool Insert();
	virtual void Print(mstr &m);

	long expense;

	char sumType;
	char sumState;
	char accountType;

	char text[SIZE_OF_EXPENSE_TEXT];
	long penalityType;

	char rejectKind;
	CDate rejectDate;
	long flags;
	char rejectCourt[SIZE_OF_UCN];

	long reasonType;
	double sum1;		// reward/penality
	double sum2;		// reward only: travel etc.

	// 2014:211 LPR: were paid, note
	CDate paidDate;
	TRCDKey paid;
	char paidTransId[SIZE_OF_TRANS_ID];
	char paidNote[SIZE_OF_EXPENSE_NOTE];

	double Sum() { return sum1 + sum2; }
};

// ----- TIndex --------------------------------------------------------------
class TIndex : public TGroup
{
public:
	TIndex(char type, TGroup *(*const tClone)());

	long no;
	long year;
	char index;

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual bool Generate();
	virtual bool Insert();
};

// ----- TRequestIndex -------------------------------------------------------
class TRequestIndex : public TIndex
{
public:
	TRequestIndex();
	static TGroup *Clone();
};

extern TRequestIndex *RequestIndex;

// ----- TAnnounceIndex ------------------------------------------------------
class TAnnounceIndex : public TIndex
{
public:
	TAnnounceIndex();
	static TGroup *Clone();
};

extern TAnnounceIndex *AnnounceIndex;

// ----- TEventIndex ---------------------------------------------------------
class TEventIndex : public TGroup
{
public:
	TEventIndex();
	static TGroup *Clone();

	long eventNo;
	long year;
	char type;
	char kind;

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual bool Generate();
	virtual bool Insert();
};

extern TEventIndex *DecisionIndex;

// ----- TConnect ------------------------------------------------------------
enum TConnectReturnable
{
	RETURNABLE_LOWER	= 1,
	RETURNABLE_EISPP	= 2,
	RETURNABLE_FULL	= 3
};

class TConnect : public TGroup	// maybe TRCDKeyGroup?
{
public:
	TConnect();
	static TGroup *Clone();

	const char *CompositionName();
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	// 2010:152 LPR: for desc book; 2013:240 bool -> sign; 2017:230 separate alias and cyrno
	// 2818-02-14: separate compos
	void Print(mstr &m, bool alias, bool cyrno, constant compos);
	virtual void Print(mstr &m);

	bool TryBy(const TRCDKey &tKey, char tConnectType, const char *fields = NULL);
	bool TryBy(const TRCDKeyGroup *group, char connectType, const char *fields = NULL);
	bool TryBy(const TRCDKeyGroup *group, const char *connectTypes, const char *fields = NULL);

	bool TryLower(const TRCDKey &tKey, const char *fields = NULL);
	bool TryLower(const TRCDKeyGroup *group, const char *fields = NULL) { return TryLower(group->key, fields); }
	const char *Returnable(TConnectReturnable returnable) const;
	bool ReturnedTo() const;

	TRCDKey key;
	char ucnType;
	char ucn[SIZE_OF_UCN];
	char connectNo[SIZE_OF_CONNECT_NO];
	long connectYear;
	char connectType;
	char connectKind;
	long composition;
	char decisionKind;
	CDate decisionDate;
	char outKind;
	long outNo;
	long outYear;
	char text[SIZE_OF_CONNECT_TEXT];
	long xferKind;
	long dirty;			// 2009:047 LPR: log only edited connects
};

// ----- TReplacePair --------------------------------------------------------
struct TReplacePair
{
	TReplacePair(const char *tFind, const char *tReplace);

	char find[SIZE_OF_FIND_REPLACE_TEXT];
	char replace[SIZE_OF_FIND_REPLACE_TEXT];

	void FindReplace(char *text, IDispatch *iActive) const;
	void Print(mstr &m) const;	// overwrite, not append
};

// ----- TReplacePairs -------------------------------------------------------
class TReplacePairs : public TRCDKeyGroup
{
public:
	TReplacePairs();

	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// == exact criteria
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual bool Try(const char *fields = NULL);	// try & split to pairs

	mstr text;

	TIArray<TReplacePair> pairs;		// memory only
};

// ----- TLawInterval --------------------------------------------------------
class TLawInterval : public TRCDKeyGroup
{
public:
	TLawInterval();
	TLawInterval(const TRCDKey &tKey, const CDate &tDate);
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual bool Update(const char *fields);
	// 2009:202; 2018-04-23: +silent
	bool Read(const char *fields, bool silent);
	// 2017:038 LPR: the preffered way to check for finished
	static bool Finished(const TLawsuit *lawsuit, const CDate &date);

	bool operator<=(const TLawInterval &that) const;	// date only

	CDate date;
	long flags;
	CDate firstSet;
	char age;
	CDate finished;
	char grant;

private:
	void Init();
};

// ----- TJurorDistrib -------------------------------------------------------
class TJurorDistrib : public TRCDKeyGroup
{
public:
	TJurorDistrib();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	bool operator<=(const TJurorDistrib &that) const { return key <= that.key; }

	char juror[SIZE_OF_UCN];
	char reJuror[SIZE_OF_UCN];
	char jurorX[SIZE_OF_UCN];
	char reserve[SIZE_OF_UCN];

	mstr motives;
	CDate date;
	char md5sum[SIZE_OF_MD5SUM];
	long excess;			// 2017:067
	long closed;			// 2017:142
	CTime time;				// 2017:149

	char *jurors[JUROR_COUNT];	// memory only
	char lawsuitKind;			// memory only

	virtual bool Insert();
	virtual bool Update(const char *fields = NULL);

	void Regenerate();
	bool Find(const TBaseSession *session, bool complain);

protected:
	virtual bool Try(const char *fields = NULL);	// verify checksum; 2017:149 hidden
	void CalcMD5(char *tMD5Sum);
};

// ----- TSubServeData -------------------------------------------------------
struct TSubServeData
{
	CDate subpoenaReceived;
	CTime receivedTime;
	char receivedStatus;
	char receivedText[SIZE_OF_RECEIVED_TEXT];
	CDate subpoenaReturned;

	bool operator==(const TSubServeData &that) const;

protected:
	TSubServeData(TGroup *self);
};

// ----- TAnnounceKey --------------------------------------------------------
struct TAnnounceKey
{
	long no;
	long year;
	char kind;

	int Kompare(const TAnnounceKey &that) const;	// kind, no, year
	bool operator<=(const TAnnounceKey &that) const { return Kompare(that) <= 0; }
};

// ----- TAnnounce -----------------------------------------------------------
class TAnnounce : public TGroup, public TAddressData, public TSubServeData
{
public:
	TAnnounce();
	virtual ~TAnnounce();
	static TGroup *Clone();

	char SenderType() const { return court == COURT_CODE ? UCN_INSTANCE_SELF : UCN_INSTANCE_EXTERNAL; }
	void GetSenderName(mstr &m) const;
	void SetSubpoena(TSubpoena *tSubpoena);

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual bool Insert();
	virtual void Print(mstr &m);
	// 2011:319 LPR: former ankey()
	bool Less1(const TAnnounce *that) const;
	bool Less2(const TAnnounce *that) const;

	TAnnounceKey key;
	long court;
	long announce;
	long requestId;		// 2015:085 for barcoding, optional

	CDate subpoenaDate;
	char subpoenaKind;
	CDate sessionDate;
	long sideNo;
	char subpoenaText[SIZE_OF_SUBPOENA_TEXT];

	char delivererUcn[SIZE_OF_UCN];
	long composition;
	char barcode[SIZE_OF_BARCODE_TEXT];
	double coordN;
	double coordE;

	TSubpoena *subpoena;	// 2010:004 LPR: memory, if represents subpoena

protected:
	bool Less0(const TAnnounce *that) const;
};

// ----- TBankrupt -----------------------------------------------------------
class TBankrupt : public TUCNGroup
{
public:
	TBankrupt();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Print(mstr &m);

	char creditor[SIZE_OF_UCN];
	char bulstat[SIZE_OF_BULSTAT];	// memory only
};

// ----- TInformed -----------------------------------------------------------
class TInformed : public TRCDKeyGroup
{
public:
	TInformed();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Print(mstr &m);

	bool operator<=(const TInformed &that) const { return key <= that.key; }

	char ucn[SIZE_OF_UCN];
	char email[SIZE_OF_EMAIL];	// 2013:065
};

// ----- TSessionTimes -------------------------------------------------------
class TSessionTimes : public TRCDKeyGroup
{
public:
	TSessionTimes();
	static TGroup *Clone();

	void operator<<(const TQuery &query) { ImportFromQuery(&query); }
	void operator<<(const TBaseSession *session) { ImportFromSession(session); }
	void operator<<(const TSessionTimes *sTimes) { ImportFromGroup(sTimes); }

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	const char *Invalid(bool internal) const;

	bool operator<=(const TSessionTimes &that) const { return (CDate &) start <= (CDate &) that.start; }

	char kind;
	CDate date;
	CDateTime start;
	CTime entime;
	long hours;
	long excess;	// 2017:074

protected:
	void ImportFromSession(const TBaseSession *session);
};

// ----- TEisPending ---------------------------------------------------------
class TEisPending : public TGroup
{
public:
	TEisPending();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);

	char fileName[SIZE_OF_NAME];
	long requestId;
};

// ----- TPayment ------------------------------------------------------------
class TPayment : public TGroup
{
public:
	TPayment();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Generate();
	virtual bool Insert();

	bool operator<=(const TPayment &that) const;	// DESC;
	int PosTerm() const { return accountType == ACCOUNT_TYPE_BUDGET ? POSTERM_BUDGET : POSTERM_DEPOSITS; }
	bool SameTransact(const TPayment &next) const;	// 2018-10-24: LPR: higher level, for transId 0

	char transId[SIZE_OF_TRANS_ID];
	long termId;
	long ecr;
	long payment;
	double sum;
	CDate date;
	CTime time;
	char sumType;
	char ucn[SIZE_OF_UCN];
	char ucnType;
	char obl[SIZE_OF_UCN];
	char oblType;
	long court;
	TAnnounceKey key;
	char accountType;
	char text[SIZE_OF_PAYMENT_TEXT];
	char authId[SIZE_OF_PAYMENT_AUTH_ID];
	char cardId[SIZE_OF_PAYMENT_CARD_ID];	// 2013:345
};

// ----- TOldFirm ------------------------------------------------------------
class TOldFirm : public TFirm
{
public:
	TOldFirm();
	static TGroup *Clone();
};

// ----- TExecListBase -------------------------------------------------------
class TExecListBase : public TGroup
{
public:
	TExecListBase(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);

	void Criteria(TSubpoena *subpoena, msql &m, const char *fields = NULL);

	long execListNo;
	long execListYear;
	char type;
	long execListKind;
};

// ----- TExecListPerson -----------------------------------------------------
class TExecListAddress;

class TExecListPerson : public TExecListBase, public TTryAddress
{
public:
	TExecListPerson();
	static TGroup *Clone();

	void TryAddress() { TTryAddress::TryAddress(ucn, ucnType); }
	void SetAddress(const TGroup *tAddress, bool foreign);
	void SetAddress(TGArray<TExecListAddress> &addresses);

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual void Print(mstr &m);

	long sumNo;
	char ucn[SIZE_OF_UCN];
	char ucnType;
	char involvement;		// 2015:075, optional
};

// ---- TExecListSumBase -----------------------------------------------------
class TExecListSumBase : public TExecListBase
{
public:
	TExecListSumBase(const char *name, TGroup *(*const tClone)());

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual bool Generate();
	virtual bool Insert();

	long sumNo;
	double sum;
	char currency[SIZE_OF_CURRENCY];
	char sumText[SIZE_OF_PAYMENT_TEXT];
};

// ---- TExecListSum ---------------------------------------------------------
class TExecListSum : public TExecListSumBase
{
public:
	TExecListSum();
	static TGroup *Clone();

	bool Save(bool edit);

	bool SameList(const TExecListSum *sum);
	bool SamePersons(const TExecListSum *sum);

	char sumType;
	char sender[SIZE_OF_UCN];

	TGArray<TExecListPerson> persons;
};

// ----- TExecList -----------------------------------------------------------
class TExecList : public TExecListBase
{
public:
	TExecList();
	static TGroup *Clone();

	virtual bool Delete(bool exact);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual bool Generate();
	virtual bool Insert();

	bool Save(bool edit);
	void AddSumPerson(TBaseWind *baseWind) { InsertSumPerson(baseWind, allSumPersons.Count()); }
	bool InsertSumPerson(TBaseWind *baseWind, int index);
	bool CheckSend(TOutReg *outReg);

	bool operator<=(const TExecListBase &that);

	CDate execListDate;
	long no;
	long year;
	TRCDKey nia;	// 2015:041
	// 2015:042
	char tdep[SIZE_OF_UCN];
	char tdepInNo[SIZE_OF_NIA_TDEP_IN_NO];
	CDate tdepDate;
	mstr tdepText;
	char tdepUCNType;		// 2016:070

	TGArray<TExecListPerson> allSumPersons;
	TGArray<TExecListSum> sums;
	long recipient;

protected:
	void DeleteSumsPersons();
};

// ----- TNIAProtocol --------------------------------------------------------
class TNIAProtocol : public TGroup
{
public:
	TNIAProtocol();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual bool Generate();
	virtual bool Insert();

	TRCDKey nia;
	char niaTDep[SIZE_OF_UCN];
	long outNo;
	long outYear;

	TGArray<TExecList> execLists;
};

// ----- TExecListCollect ----------------------------------------------------
class TExecListCollect : public TExecListSumBase
{
public:
	TExecListCollect();
	static TGroup *Clone();

	CDate date;
	long flags;
	char ucn[SIZE_OF_UCN];
	char ucnType;
};

// ----- TExecListAddress ----------------------------------------------------
class TExecListAddress : public TExecListBase
{
public:
	TExecListAddress();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	char ucn[SIZE_OF_UCN];
	char ucnType;

	long uclp;
	long street;
	char adrNo[SIZE_OF_ADR_NO];
	char subNo[SIZE_OF_SUB_NO];
	char entrance[SIZE_OF_ENTRANCE];
	char floor[SIZE_OF_FLOOR];
	char apartment[SIZE_OF_APARTMENT];
	char blockNo[SIZE_OF_BLOCK_NO];
	char phone[SIZE_OF_PHONE];
	char telex[SIZE_OF_TELEX];
	char fax[SIZE_OF_FAX];
	char email[SIZE_OF_EMAIL];
	char remark[SIZE_OF_ADDRESS_REMARK];

	char country[SIZE_OF_COUNTRY];
	char foreign[SIZE_OF_FOREIGN_ADDRESS];
};

// ----- TJurorLog -----------------------------------------------------------
class TJurorLog : public TGroup
{
public:
	TJurorLog();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual bool Insert();

	long jurorLog;
	TRCDKey key;
	char kind;
	CDate date;

	char ucn[SIZE_OF_UCN];
	char oper;
	char juror[SIZE_OF_UCN];
	long index;
	CDateTime stamp;
	char text[SIZE_OF_JUROR_MOTIVES];
	long excess;	// 2017:069

	void CheckMD5();

protected:
	long resBlock[4];

	void CalcMD5(long *tResBlock);
};

// ----- TAttachStat ---------------------------------------------------------
class TAttachStat : public TRCDKeyGroup
{
public:
	TAttachStat();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	char ucn[SIZE_OF_UCN];
	CDate from;
	char kind;
	CDate date;

	bool found;	// memory only
};

// ----- TRegisterAct --------------------------------------------------------
struct TElectricTrouble;

class TRegisterAct : public TRCDKeyContainer
{
public:
	TRegisterAct();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual bool Insert();
	virtual void Print(mstr &m);

	void ViewFiles(TWindow *parent);
	bool FetchFiles(TElectricTrouble *trouble);
	bool ReWriteFiles();

	char judge[SIZE_OF_UCN];
	long actNo;
	long mtime[TROUBLE_COUNT];
	TMemBlock fileData[TROUBLE_COUNT];
	// 2015:329 LPR/LRQ: +related
	char targetKind;
	TRCDKey target;
	CDate targetDate;
	CDateTime targetStamp;
	char text[SIZE_OF_REGISTER_ACT_TEXT];	// 2016:135 +related
};

// ----- TDeliverVisit -------------------------------------------------------
class TDeliverVisit : public TGroup
{
public:
	TDeliverVisit(const char *name, TGroup *(*const tClone)());

	virtual void Print(mstr &m);
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	void PrintVisits(mstr &m, TGroup *parent);

	CDateTime stamp;
	double coordN;
	double coordE;
};

// ----- TSubpoenaVisit ------------------------------------------------------
class TSubpoenaVisit : public TDeliverVisit
{
public:
	TSubpoenaVisit();
	static TGroup *Clone();

	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	TRCDKey key;
	char kind;
	CDate date;
	long subpoenaNo;
};

// ----- TAnnounceVisit ------------------------------------------------------
class TAnnounceVisit : public TDeliverVisit
{
public:
	TAnnounceVisit();
	static TGroup *Clone();

	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	TAnnounceKey key;
	long court;
	long announce;
};

// ----- TElectric -----------------------------------------------------------
class TElectric : public TRCDKeyContainer
{
public:
	TElectric();
	static TGroup *Clone();

	virtual bool Delete(bool exact);
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual bool Insert();
	virtual void Print(mstr &m);
	virtual bool Update(const char *fields = NULL);

	bool operator<=(const TElectric &that) const;
	bool ExtraSubpoena() const { return extraNo && extraNo <= SUBPOENA_NO_MAX; }
	static bool ExtraUser(long extraNo) { return extraNo > SUBPOENA_NO_MAX && extraNo <= SUBPOENA_NO_MAX + EXTRA_NO_MAX; }
	bool ExtraUser() const { return ExtraUser(extraNo); }
	static bool ExtraSurrouend(long extraNo);
	bool ElectricDelete(const TLawsuit *lawsuit, const TRCDKeyContainer *container);

	long extraNo;
	char descript[SIZE_OF_ELECTRIC_DESCRIPT];

	// memory only
	char extraName[SIZE_OF_PATH];
	long extraType;
	// 2016:319 memory only, for e-portal delete
	char ucn[SIZE_OF_UCN];
	char ucnType;
	char involvement;
	char subpoenaKind;
	char eventStatus;
};

// ----- TKnownNamesPerson ---------------------------------------------------
class TKnownNamesPerson : public TKnownPerson, public TCitizenNames
{
public:
	TKnownNamesPerson();

	virtual void NamesCriteria(msql &m);
	virtual void Print(mstr &m);
	// 2017:116 for known person edit
	void ClearCache() { cache.clear(); }

protected:
	mstr cache;
};

class TEPortAssign : public TRCDKeyGroup
{
public:
	TEPortAssign();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	TRCDKey lawsuit;
	char involvement;
	long flags;
};

// ----- TExternSession ------------------------------------------------------
class TExternSession : public TBaseSession
{
public:
	TExternSession();
	static TGroup *Clone();

	virtual bool Delete(bool exact);	// delete session times
	virtual char IndexType() const;
	virtual char IndexKind() const;
	virtual bool Insert();			// skip find/copy sides
	virtual long Excess() const;
	virtual void Signal(int, const void *data);
};

// ----- TExternLawsuit ------------------------------------------------------
class TExternLawsuit : public TExternContainer
{
public:
	TExternLawsuit();
	static TGroup *Clone();

	virtual char LawKind() const;
	virtual bool Insert();
	virtual long Excess() const;
	virtual void Signal(int, const void *);
	virtual void Involvements(mstr &m) const;

	CDate forceDate;
	long sourceKind;
};

// ----- TExternSideWind -----------------------------------------------------
class TExternSideWind : public TBaseWind
{
public:
	TExternSideWind();
	static TGroup *Clone();

	virtual long Excess() const;

	char externId[SIZE_OF_EISPP_NMR];
};
