// ----- TType ---------------------------------------------------------------
class TType : public TCharAliasGroup
{
public:
	TType();
	static TGroup *Clone();

	char type;
	char name[SIZE_OF_NAME];
	char alias[SIZE_OF_ALIAS];
	long ecode;

protected:
#if CUTCITILAWS
	virtual void CacheCriteria(msql &m);
#endif  // CUTCITILAWS
};

extern TType *Type;

// ----- TKind ---------------------------------------------------------------
class TKind : public TXferAliasGroup
{
public:
	TKind();
	static TGroup *Clone();

	void XferCriteria(msql &m, long xferKind, char type);
	long Order() { return order; }

	virtual void InitCache();	// fill INVOLVEMENT_ALLS

	char kind;
	char name[SIZE_OF_NAME];
	char alias[SIZE_OF_ALIAS];
	char type;
	char lawKinds[SIZE_OF_LAW_KINDS];
	char suitors[SIZE_OF_KIND_INVOLVEMENTS];
	char defendants[SIZE_OF_KIND_INVOLVEMENTS];
	char others[SIZE_OF_KIND_INVOLVEMENTS];
	long eispp;

protected:
#if CUTCITILAWS
	virtual void CacheCriteria(msql &m);
#endif  // CUTCITILAWS
};

extern TKind *Kind;

// ----- TKindAlias ----------------------------------------------------------
class TKindAlias : public TCharAliasGroup
{
public:
	TKindAlias();

	static TGroup *Clone();

	void InitCache();

	char kind;
	char alias[SIZE_OF_ALIAS];
};

extern TKindAlias *KindAlias;

// ----- TSubjectData --------------------------------------------------------
struct TSubjectData
{
	void AddTo(TGroup *self, long si);

	char kind;
	long flags;
	char attrib[SIZE_OF_SUBJECT_ATTRIB];
	char results[SIZE_OF_SUBJECT_RESULTS];
	char closeds[SIZE_OF_SUBJECT_RESULTS];	// 2009:082 +related
	char penalities[SIZE_OF_REALITIES];		// 2003:142
	char reasons[SIZE_OF_REALITIES];		// 2005:357
	long judgeReport;					// 2008:184
	long quickReport;					// 2011:174 +related
	long vid;
	char sbes[SIZE_OF_SUBJECT_SBES];
};

// ----- TSubject ------------------------------------------------------------
class TSubject : public TLongAliasGroup
{
public:
	TSubject();
	static TGroup *Clone();

	virtual bool Delete(bool exact);		// delete sub-noms
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void Print(mstr &m);			// 0, false
	const char *Seek(char type, long subject);

	virtual bool Insert();

//	bool operator==(const TSubject &that) const;
	bool operator<=(const TSubject &that) const;
//	bool operator<(const TSubject &that) const;	// for _ nomHeap only

	long subject;
	char type;
	char name[SIZE_OF_SUBJECT_NAME];
	char alias[SIZE_OF_NAME];
	long flagsX;			// 2016:055 LPQ: hidden
	long saas;				// memory only: subject active as second

	static long SI(char lawKind);
	static const char *FNI(const char *name, long si);
	const char *FNI(const char *name) { return FNI(name, saas); }
	static bool Twin(char type) { return strchr(TYPE_TWIN_SUBJ_LAWS, type); }

	static const char *Code(long subject, char type, long si, bool saas = true);
	static const char *Code(long subject, char lawKind);
	const char *Code(long si, bool saas = true) { return Code(subject, type, si, saas); }
	static void ClearCodes() { CodeNames.Flush(); }

	TSubjectData sd[2];
	TSubjectData &SD(char lawKind) { return sd[SI(lawKind)]; }
	TSubjectData &SaasD() { return sd[saas]; }
	static void AddVisible(msql &m);

	char Kind(char lawKind) { return SD(lawKind).kind; }
	char *Attrib(char lawKind) { return SD(lawKind).attrib; }
	char *Results(char lawKind) { return SD(lawKind).results; }
	char *Closeds(char lawKind) { return SD(lawKind).closeds; }
	char *Penalities(char lawKind) { return SD(lawKind).penalities; }
	char *Reasons(char lawKind) { return SD(lawKind).reasons; }
	long JudgeReport(char lawKind) { return SD(lawKind).judgeReport; }
	long QuickReport(char lawKind) { return SD(lawKind).quickReport; }
	long Vid(char lawKind) { return SD(lawKind).vid; }
	char *Sbes(char lawKind) { return SD(lawKind).sbes; }

private:
	struct TCodeName
	{
		long subject;
		char type;
		char name[SIZE_OF_SUBJECT_CODE_NAME];
		char name1[SIZE_OF_SUBJECT_CODE_NAME];
		char name2[SIZE_OF_SUBJECT_CODE_NAME];

		bool operator<=(const TSubject::TCodeName &that);
	};

	static TISArray<TCodeName> CodeNames;
};

extern TSubject *Subject;

// ----- TOldSubject ---------------------------------------------------------
class TOldSubject : public TLongAliasGroup
{
public:
	TOldSubject();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void Print(mstr &m);
	const char *Seek(char type, long subject);

	bool operator<=(const TOldSubject &that) const;

	long subject;
	char name[SIZE_OF_SUBJECT_NAME];
	char type;
	char attrib[SIZE_OF_SUBJECT_ATTRIB];
	char results[SIZE_OF_SUBJECT_RESULTS];
	char kind;
	char alias[SIZE_OF_NAME];
	char college;
	char posts[SIZE_OF_POSTS];			// obsolete, firm distribution
	char penalities[SIZE_OF_REALITIES];		// 2003:142
	long flags;						// 2005:332
	char reasons[SIZE_OF_REALITIES];		// 2005:357
	long judgeReport;					// 2008:184
	long quickReport;					// 2011:174 +related
	char closeds[SIZE_OF_SUBJECT_RESULTS];	// 2009:082 +related
	long vid;
	char sbes[SIZE_OF_SUBJECT_SBES];
};

extern TOldSubject *OldSubject;

// ----- TResolution ---------------------------------------------------------
class TResolution : public TCharAliasGroup
{
public:
	TResolution();
	static TGroup *Clone();

	char resolution;
	char name[SIZE_OF_LONGER_NAME];
};

extern TResolution *Resolution;

// ----- TNTResult -----------------------------------------------------------
class TNTResult : public TXferAliasGroup
{
public:
	TNTResult();
	static TGroup *Clone();

	char result;
	char name[SIZE_OF_LONGER_NAME];
};

extern TNTResult *Result;

// ----- TUXNType ------------------------------------------------------------
class TUXNType : public TCharAliasGroup
{
public:
	TUXNType();
	TUXNType(const char *tName, TGroup *(*tClone)());

	static TGroup *Clone();

	char ucnType;
	char name[SIZE_OF_ALIAS];
};

extern TUXNType *UXNType;

// ----- TUCNType ------------------------------------------------------------
class TUCNType : public TUXNType
{
public:
	TUCNType();
	static TGroup *Clone();

	char uxnType;
};

extern TUCNType *UCNType;

// ----- TInvolvement --------------------------------------------------------
class TInvolvement : public TXferAliasGroup
{
public:
	TInvolvement();
	static TGroup *Clone();

	long Order() { return order; }

	char involvement;
	char name[SIZE_OF_NAME];
};

extern TInvolvement *Involvement;

// ----- TPost ---------------------------------------------------------------
class TPost : public TCharAliasGroup
{
public:
	TPost();
	static TGroup *Clone();

	char post;
	char name[SIZE_OF_NAME];
};

extern TPost *Post;

// ----- TCollege ------------------------------------------------------------
class TCollege : public TCharAliasGroup
{
public:
	TCollege();
	static TGroup *Clone();

	virtual void Fixup(bool);
	static void FixupDepartName(char college, char *name);

	char college;
	char name[SIZE_OF_NAME];

protected:
#if CUTCITILAWS
	virtual void CacheCriteria(msql &m);
#endif  // CUTCITILAWS
};

extern TCollege *College;

// ----- TCollegeRight -------------------------------------------------------
class TCollegeRight : public TCharAliasGroup
{
public:
	TCollegeRight();
	static TGroup *Clone();

	virtual void Fixup(bool);

	char college;
	char rightName[SIZE_OF_NAME];

protected:
#if CUTCITILAWS
	virtual void CacheCriteria(msql &m);
#endif  // CUTCITILAWS
};

extern TCollegeRight *CollegeRight;

// ----- TUser ---------------------------------------------------------------
class TUser : public TUserAliasGroup
{
public:
	TUser();
	static TGroup *Clone();

	virtual bool Delete(bool exact);				// userSubjects, streetMaps
	virtual bool Update(const char *fields = NULL);		// assure rights to sysadmin

	char md5sum[SIZE_OF_MD5SUM];			// 2005:034
	char menus[SIZE_OF_MENUS];
	CDate date;						// 2003:049
	long rawload;					// memory only

	// 2007:184 LPR: new or fixed
	double FullLoad();				// raw adjusted by % + pre
	long RawLoadA();					// rounded raw adjusted by %
	void PrintLoad(mstr &m, bool twoDec);

	// 2010:245 LPR: back from TUserBase
	char login[SIZE_OF_NAME];
	char kind;
	long percent;	// 2007:183 LPR: new distribution
	long preload;	// 2007:183 LPR: new distribution
	long failogs;	// 2013:064 LRQ: +related
	long compos;	// 2013:034 PRQ: default
	long actype;	// 2015:030 +related

protected:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

extern TUser *User;

// ----- TDefault ------------------------------------------------------------
// 2008:043 LPR: +related: extended (common) college support
class TDefault : public TUser
{
public:
	TDefault();

	char xColleges[SIZE_OF_COLLEGES];
	char xKinds[SIZE_OF_LAW_KINDS];
};

extern TDefault *Default;

// ----- TFineComposition ----------------------------------------------------
class TFineComposition : public TLongAliasGroup
{
public:
	TFineComposition();
	static TGroup *Clone();

	long composition;
	char name[SIZE_OF_ALIAS];
	char judge[SIZE_OF_UCN];
	char secretar[SIZE_OF_UCN];
	// 2005:102 LPR: prosecutor/jurors comout; 2005:318 Removed
	char judgeX[SIZE_OF_UCN];
	char reJudgeX[SIZE_OF_UCN];
	char president[SIZE_OF_UCN];
	char college;			// 2003:048
	long flags;				// 2008:254

protected:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

extern TFineComposition *FineComposition;

// ----- TBaseComposition ----------------------------------------------------
class TCompositionCrit
{
public:
	TCompositionCrit() { }

	long composition;
	char name[SIZE_OF_ALIAS];

protected:
	void _ExactCriteria(msql &m, const char *fields);	// college = *COLLEGE_ALLS
	void _FloatCriteria(msql &m, const char *fields);

	// chars xCrit -> long F_COMPOSITION set
	void _AliasCriteria(msql &m, const char *xCrit, long);

private:
	const char *_Name() { return "T_COMPOSITION"; }
};

// ----- TComposition --------------------------------------------------------
class TComposition : public TLongAliasGroup, public TCompositionCrit
{
public:
	TComposition();	// xField = new F_CHAR_COMPOSITION
	static TGroup *Clone();

	virtual void AliasCriteria(msql &m, const char *xCrit, long flags);

protected:
	// from TCompositionCrit
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

extern TComposition *Composition;

// ----- TCharComposition ----------------------------------------------------
class TCharComposition : public TCharAliasGroup, public TCompositionCrit
{
public:
	TCharComposition();
	static TGroup *Clone();

	virtual void InitCache();
	virtual void AliasCriteria(msql &m, const char *xCrit, long flags);

protected:
	// from TCompositionCrit
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

extern TCharComposition *CharComposition;

// ----- TRight --------------------------------------------------------------
class TRight : public TCharAliasGroup
{
public:
	TRight();
	static TGroup *Clone();

	char right;
	char name[SIZE_OF_LONGER_NAME];	// 2015:034 LPR: LONGER
};

extern TRight *Right;

// ----- TAge ----------------------------------------------------------------
class TAge : public TCharAliasGroup
{
public:
	TAge();
	static TGroup *Clone();

	char age;
	char name[SIZE_OF_ALIAS];
};

extern TAge *Age;

// ----- TGrant --------------------------------------------------------------
class TGrant : public TCharAliasGroup
{
public:
	TGrant();
	static TGroup *Clone();

	char grant;
	char name[SIZE_OF_NAME];
};

extern TGrant *Grant;

// ----- TJudged -------------------------------------------------------------
class TJudged : public TCharAliasGroup
{
public:
	TJudged();
	static TGroup *Clone();

	char judged;
	char name[SIZE_OF_NAME];
};

extern TJudged *Judged;

// ----- TPunishment ---------------------------------------------------------
class TPunishment : public TCharAliasGroup
{
public:
	TPunishment();
	static TGroup *Clone();

	char punishment;
	char name[SIZE_OF_NAME];
};

extern TPunishment *Punishment;

// ----- TRecidive -----------------------------------------------------------
class TRecidive : public TCharAliasGroup
{
public:
	TRecidive();
	static TGroup *Clone();

	char recidive;
	char name[SIZE_OF_NAME];
};

extern TRecidive *Recidive;

// ----- TCountry ------------------------------------------------------------
class TCountry : public TStringAliasGroup
{
public:
	TCountry();
	static TGroup *Clone();

	virtual bool Delete(bool exact);				// reject required
	virtual bool Update(const char *fields = NULL);		// reject required

	char country[SIZE_OF_COUNTRY];
	char name[SIZE_OF_UCLP_NAME];
	long eispp;
};

extern TCountry *Country;

// ----- TAddrAliasGroup -----------------------------------------------------
class TAddrAliasGroup : public TLongAliasGroup
{
public:
	TAddrAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData,
		const char *nameFieldName, char *nameFieldData, int nameFieldSize);

	virtual bool Insert() = 0;					// generate -value
	virtual bool Delete(bool exact);				// reject required
	virtual bool Update(const char *fields = NULL);		// reject required

	virtual bool Required() = 0;
};

// ----- TArea ---------------------------------------------------------------
class TArea : public TAddrAliasGroup
{
public:
	TArea();
	static TGroup *Clone();

	virtual bool Insert();
	virtual bool Required();

	long area;
	char name[SIZE_OF_UCLP_NAME];
};

extern TArea *Area;

// ----- TRegion -------------------------------------------------------------
#define TRegion TNTRegion
class TRegion : public TAddrAliasGroup
{
public:
	TRegion();
	static TGroup *Clone();

	virtual bool Insert();
	virtual bool Required();
	const char *Seek(long tArea, long tRegion);

	long area;
	long region;
	char name[SIZE_OF_UCLP_NAME];

protected:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual void CacheCriteria(msql &m);
	virtual bool CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags);
};

extern TRegion *Region;

// ----- TFindAddrAliasGroup -------------------------------------------------
class TFindAddrAliasGroup : public TAddrAliasGroup
{
public:
	TFindAddrAliasGroup(const char *name, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData,
		const char *nameFieldName, char *nameFieldData, int nameFieldSize);

	virtual void AliasCriteria(msql &m, const char *, long flags);
};

// ----- TUCLP ---------------------------------------------------------------
class TUCLP : public TFindAddrAliasGroup
{
public:
	TUCLP();
	static TGroup *Clone();

	virtual bool Insert();
	virtual bool Required();

	long area;
	long region;
	long uclp;
	char name[SIZE_OF_UCLP_NAME];

protected:
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

extern TUCLP *UCLP;

// ----- TStreet -------------------------------------------------------------
class TStreet : public TFindAddrAliasGroup
{
public:
	TStreet();
	static TGroup *Clone();

	virtual bool Insert();
	virtual bool Required();
	const char *Seek(long tUCLP, long tStreet);

	bool operator<=(const TStreet &that) const { return uclp != that.uclp ? uclp < that.uclp : street <= that.street; }

	long uclp;
	long street;
	char name[SIZE_OF_STREET_NAME];

protected:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

extern TStreet *Street;

// ----- TCompanyType -----------------------------------------------------------
class TCompanyType : public TCharAliasGroup
{
public:
	TCompanyType();
	static TGroup *Clone();

	char companyType;
	long subject;
	char name[SIZE_OF_ALIAS];
	long registry;
	char reName[SIZE_OF_NAME];

	bool DamnName(const char *s);
};

extern TCompanyType *CompanyType;

// ----- THall ---------------------------------------------------------------
class THall : public TLongAliasGroup
{
public:
	THall();
	static TGroup *Clone();

	virtual bool Delete(bool exact);	// delete load

	bool operator<=(const THall &that) const { return strcmp(name, that.name) <= 0; }

	long hall;
	char name[SIZE_OF_NAME];		// 2005:060 HALL_NAME -> NAME
	char alias[SIZE_OF_HALL_NAME];	// 2005:060
	char address[SIZE_OF_HALL_ADDRESS];	// 2006:040 +related
	long flags;					// 2007:213 +related
};

extern THall *Hall;

// ----- TInterval -----------------------------------------------------------
class TInterval : public TCharAliasGroup
{
public:
	TInterval();
	static TGroup *Clone();

	char interval;
	char name[SIZE_OF_NAME];
};

extern TInterval *Interval;

// ----- TAppealResolution ---------------------------------------------------
class TAppealResolution : public TCharAliasGroup
{
public:
	TAppealResolution();
	static TGroup *Clone();

	char resolution;
	char resolutions[SIZE_OF_APPEAL_RESOLUTION_RESOLUTIONS];
	char name[SIZE_OF_NAME];
};

extern TAppealResolution *AppealResolution;

// ----- TCrimeForm ----------------------------------------------------------
class TCrimeForm : public TCharAliasGroup
{
public:
	TCrimeForm();
	static TGroup *Clone();

	char crimeForm;
	char name[SIZE_OF_NAME];
};

extern TCrimeForm *CrimeForm;

// ----- TCrimeStage ---------------------------------------------------------
class TCrimeStage : public TCharAliasGroup
{
public:
	TCrimeStage();
	static TGroup *Clone();

	char crimeStage;
	char name[SIZE_OF_NAME];
};

extern TCrimeStage *CrimeStage;

// ----- TJailMode -----------------------------------------------------------
class TJailMode : public TCharAliasGroup
{
public:
	TJailMode();
	static TGroup *Clone();

	char jailMode;
	char name[SIZE_OF_NAME];
};

extern TJailMode *JailMode;

// ----- TAccomply -----------------------------------------------------------
class TAccomply : public TXferAliasGroup
{
public:
	TAccomply();
	static TGroup *Clone();

	char accomply;
	char name[SIZE_OF_ACCOMPLY_TEXT];
	long flags;			// 2012:004 (in)active
	char debtorStatus;	// 2012:010 +related
};

extern TAccomply *Accomply;

// ----- TCalendar -----------------------------------------------------------
class TCalendar : public TGroup
{
public:
	TCalendar();
	static TGroup *Clone();

	virtual bool Insert();

	bool Work(long tYear, int month, int day);
	void SetWork(int month, int day, bool work);

	long year;
	char jan[31 + 1], feb[29 + 1], mar[31 + 1], apr[30 + 1];
	char may[31 + 1], jun[30 + 1], jul[31 + 1], aug[31 + 1];
	char sep[30 + 1], oct[31 + 1], nov[30 + 1], dec[31 + 1];

protected:
	char *months[12];

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	void Initialize();	// current year
};

extern TCalendar *Calendar;

// ----- TCalInterval --------------------------------------------------------
class TCalInterval : public TCharAliasGroup
{
public:
	TCalInterval();
	static TGroup *Clone();

	virtual void InitCache();		// fill SCHED_ALLS
	virtual void Fixup(bool);

	char calInterval;

	char interval;
	long count;
	long delta;
	char post;		// 2011:105 DAT:

	char name[SIZE_OF_NAME];
};

extern TCalInterval *CalInterval;

// ----- TMonth --------------------------------------------------------------
// 2002:080 from _; 2011:101 LPR: rewritten
class TMonth : public TLongAliasGroup
{
public:
	TMonth();
	static TGroup *Clone();

	long month;
	char name[SIZE_OF_MONTH_NAME];
	char alias[SIZE_OF_MONTH_ALIAS];
};

extern TMonth *Month;

// ----- TSumType ------------------------------------------------------------
class TSumType : public TCharAliasGroup
{
public:
	TSumType();
	static TGroup *Clone();

	virtual void InitCache();		// fill SUM_PAYMENTS

	char sumType;
	char name[SIZE_OF_NAME];
	char alias[SIZE_OF_ALIAS];
	char accountType;
	char execListTemplate[SIZE_OF_EXEC_LIST_TEMPLATE];
	long flags;

protected:
	void CatchFlag(char *s, long flag) { if (flags & flag) strcach(s, sumType); }
};

extern TSumType *SumType;

// ----- TSumState -----------------------------------------------------------
class TSumState : public TCharAliasGroup
{
public:
	TSumState();
	static TGroup *Clone();

	char sumState;
	char name[SIZE_OF_ALIAS];
};

extern TSumState *SumState;

// ----- TAccountType --------------------------------------------------------
class TAccountType : public TCharAliasGroup
{
public:
	TAccountType();
	static TGroup *Clone();

	char accountType;
	char name[SIZE_OF_ALIAS];
};

extern TAccountType *AccountType;

// ----- TStreetMap ----------------------------------------------------------
class TStreetMap : public TGroup
{
public:
	TStreetMap();
	static TGroup *Clone();

	void PrintMap(mstr &m);
	virtual void Print(mstr &m);

	virtual bool Active(int no);
	virtual void Adjust();

	virtual bool Insert();
	virtual bool Update(const char *fields = NULL);

	bool operator<=(const TStreetMap &that) const { return uclp != that.uclp ? uclp < that.uclp : street <= that.street; }

	char ucn[SIZE_OF_UCN];
	long uclp;
	long street;
	mstr map;

	// 2004:016 BRW: cache, used for sorting/printing/passing
	char *StreetName();
	char streetName[SIZE_OF_STREET_NAME];
	long lastUCLP;
	long lastStreet;

protected:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

extern TStreetMap *StreetMap;

// ----- TKnownPerson --------------------------------------------------------
class TKnownPerson : public TUCNGroup
{
public:
	TKnownPerson();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Print(mstr &m);

	void PedName(mstr &m, bool full);

	char involvement;
	long flags;		// 2005:083 LPR: (in)active
	long csjid;		// 2006:279 executor id
	long expertId;	// 2009:201
	char personalNo[SIZE_OF_LAWER_PERSONAL_NO];	// 2014:238
	CDateTime birthDate;	// 2016:259 CDate -> CDateTime stamp
};

// ----- TCourt --------------------------------------------------------------
class TCourt : public TLongAliasGroup
{
public:
	TCourt();
	static TGroup *Clone();

	long court;
	char courtType;
	long uclp;
	long courtArea;		// 2008:267 LPR: +related: EIK hierarhy broke
	char name[SIZE_OF_COURT_NAME];	// 2010:188 loaded from the table
	long eispp;
	long flags;			// 2014:206
};

extern TCourt *Court;

// ----- TWeekday ------------------------------------------------------------
class TWeekday : public TCharAliasGroup
{
public:
	TWeekday();
	static TGroup *Clone();

	char weekday;
	char name[SIZE_OF_WEEKDAY_NAME];
	char alias[SIZE_OF_WEEKDAY_ALIAS];
};

extern TWeekday *Weekday;

// ----- TLoadHall -----------------------------------------------------------
struct TLoadData
{
	char college;
	long composition;
	long flags;
	char text[SIZE_OF_LOAD_HALL_TEXT];
};

class TLoadHall : public TGroup
{
public:
	TLoadHall(const char *name, TGroup *(*const tClone)());

	bool Empty();
	bool Save(bool edit);

	long hall;

	TLoadData bef;
	TLoadData aft;
};

// ----- TLoadWeek -----------------------------------------------------------
class TLoadWeek : public TLoadHall
{
public:
	TLoadWeek();
	static TGroup *Clone();

	char weekday;

protected:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

// ----- TLoadDate -----------------------------------------------------------
class TLoadDate : public TLoadHall
{
public:
	TLoadDate();
	static TGroup *Clone();

	CDate date;

protected:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

// ----- TSection ------------------------------------------------------------
class TSection : public TCharAliasGroup
{
public:
	TSection();

	static TGroup *Clone();

	char section;
	char name[SIZE_OF_ALIAS];
};

extern TSection *Section;

// ----- TConnectType --------------------------------------------------------
class TConnectType : public TXferAliasGroup
{
public:
	TConnectType();
	static TGroup *Clone();

	long Order() { return order; }

	char connectType;
	char name[SIZE_OF_NAME];
	char alias[SIZE_OF_ALIAS];
	// 2013:046 LPR: reverted SIZE_OF_CONNECT_KINDS -> SIZE_OF_TYPES, breaks update
	// 2013:336 LPR: SIZE_OF_TYPES -> SIZE_OF_CONNECT_KINDS
	char connectKinds[SIZE_OF_CONNECT_KINDS];
	char senderType;
	long eispp;
};

extern TConnectType *ConnectType;

// ----- TArticle ------------------------------------------------------------
class TArticle : public TLongAliasGroup
{
public:
	TArticle();
	static TGroup *Clone();

	virtual bool Delete(bool exact);	// delete ArticleSubjects

	long article;
	char name[SIZE_OF_NAME];
	long flags;
};

extern TArticle *Article;

// ----- TArticleSubject -----------------------------------------------------
class TArticleSubject : public TLongAliasGroup
{
public:
	TArticleSubject();
	static TGroup *Clone();

	virtual void Fixup(bool force);
	virtual void Print(mstr &m);

	long article;
	long subject;

protected:
	char aname[SIZE_OF_NAME];
	long aflags;

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual void CacheCriteria(msql &m) { m.Begin(Name); }
	virtual bool Match(const char *xCrit, long flags);	// subject == ArticleSubject->subject
};

extern TArticleSubject *ArticleSubject;

// ----- TRealityType --------------------------------------------------------
class TRealityType : public TLongAliasGroup
{
public:
	TRealityType(const char *tName, TGroup *(*tClone)(), const char *valueName, const char *tLongValueName);

	virtual bool Insert();

	long realityType;
	char name[SIZE_OF_REALITY_NAME];
	long flags;

	const char *longValueName;
};

// ----- TCharRealityType ----------------------------------------------------
class TCharRealityType : public TCharAliasGroup
{
public:
	TCharRealityType(TRealityType *realityType, TGroup *(*tClone)());

	static TGroup *ClonePenality();
	static TGroup *CloneReason();

	long realityType;
	char name[SIZE_OF_REALITY_NAME];
	long flags;
};

// ----- TPenalityType -------------------------------------------------------
class TPenalityType : public TRealityType
{
public:
	TPenalityType();
	static TGroup *Clone();

	long &penalityType;
};

extern TPenalityType *PenalityType;

// ----- TReasonType ---------------------------------------------------------
class TReasonType : public TRealityType
{
public:
	TReasonType();
	static TGroup *Clone();

	long &reasonType;
};

extern TReasonType *ReasonType;

// ----- TDanger -------------------------------------------------------------
class TDanger : public TLongAliasGroup
{
public:
	TDanger(TGroup *(*const tClone)() = TDanger::Clone);
	static TGroup *Clone();

	virtual bool Delete(bool exact);		// delete DangerSubjects
	virtual void FloatCriteria(msql &m, const char *fields = NULL);	// public
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	bool operator<=(const TDanger &that) const { return danger <= that.danger; }

	long danger;
	char name[SIZE_OF_DANGER_NAME];
	long flags;
};

extern TDanger *Danger;

// ----- TDangerSubject ------------------------------------------------------
class TDangerSubject : public TLongAliasGroup
{
public:
	TDangerSubject();
	static TGroup *Clone();

	virtual void Fixup(bool force);

	long danger;
	long subject;

protected:
	char dname[SIZE_OF_DANGER_NAME];
	long dflags;

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual void CacheCriteria(msql &m) { m.Begin(Name); }
	virtual bool Match(const char *xCrit, long flags);	// subject == DangerSubject->subject
};

extern TDangerSubject *DangerSubject;

// ----- TRank ---------------------------------------------------------------
class TRank : public TLongAliasGroup
{
public:
	TRank();
	static TGroup *Clone();

	bool Seek(const char *name);
	const char *Seek(long l) { return TLongAliasGroup::Seek(l); }

	long rank;
	char name[SIZE_OF_NAME];
	char alias[SIZE_OF_ALIAS];
};

extern TRank *Rank;

// ----- TGOP ----------------------------------------------------------------
class TGOP : public TCharAliasGroup
{
public:
	TGOP();
	static TGroup *Clone();

	char gop;
	char name[SIZE_OF_ALIAS];
};

extern TGOP *GOP;

// ----- TConnectKind --------------------------------------------------------
class TConnectKind : public TXferAliasGroup
{
public:
	TConnectKind();
	static TGroup *Clone();

	char connectKind;
	char name[SIZE_OF_NAME];
	char alias[SIZE_OF_ALIAS];
};

extern TConnectKind *ConnectKind;

// ----- TConfigBase ---------------------------------------------------------
class TConfigBase : public TGroup
{
public:
	TConfigBase(const char *name, const char *tConfig, int tIndesc);

	char config[SIZE_OF_ALIAS];

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	bool Global() const;	// config == CONFIG_NAME_GLOBAL
};

// ----- TConfigName ---------------------------------------------------------
class TConfigName : public TConfigBase
{
public:
	TConfigName(const char *config);

	long no;
	CDate date;
	char version[SIZE_OF_VERSION];

	bool Check();
	virtual bool Insert();			// invoke generate
	virtual bool Delete(bool exact);	// delete ConfigData

protected:
	bool Generate(const char *what);
	void CreateVersion();
};

// ----- TConfigData ---------------------------------------------------------
struct TSettingData;

class TConfigData : public TConfigBase
{
public:
	TConfigData(const char *config);

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void Print(mstr &m);
#if 20180328
	// 2017:292 LPR: check for absolute names; 2018-03-28: FIX: check flags
	bool AskEdit(const char *value, const TSettingData *setting) const;
#endif  // 20180328
	bool AskEdit(long flags) const;	// 2010:236 LPR: now method

	bool IsEPortal(const char *tName) const { return !strcmp(section, "transfer") && !strcmp(name, "EPORTAL"); }
	bool IsEPortal() const { return IsEPortal(name); }
	const char *Scope() const { return Global() ? "ÃËÎÁÀËÍÀ" : "ëîêàëíà"; }

	char section[SIZE_OF_ALIAS];
	char name[SIZE_OF_NAME];
	char value[SIZE_OF_CONFIG_VALUE];
};

// ----- TSubpoenaKind -------------------------------------------------------
class TSubpoenaKind : public TCharAliasGroup
{
public:
	TSubpoenaKind();
	static TGroup *Clone();

	bool AddressRequired() const { return HasAttribute(SK_ATTR_ADDRESS_REQUIRED); }
	bool IsNotification() const { return HasAttribute(SK_ATTR_NOTIFICATION); }
	bool PrintingRequired() const { return HasAttribute(SK_ATTR_PRINTING_REQUIRED); }
	bool MayServe() const { return HasAttribute(SK_ATTR_MAY_SERVE); }
	bool ShowSubpoenaDate() const { return HasAttribute(SK_ATTR_SHOW_DATE); }
	bool LetterAllowed() const { return HasAttribute(SK_ATTR_LETTER_ALLOWED); }
	bool PostalAllowed() const { return HasAttribute(SK_ATTR_POSTAL_ALLOWED); }
	bool MaySchedule() const { return HasAttribute(SK_ATTR_MAY_SCHEDULE); }
	bool LinksAllowed() const { return HasAttribute(SK_ATTR_LINKS_ALLOWED); }
	bool OutRegAllowed() const { return HasAttribute(SK_ATTR_OUTREG_ALLOWED); }
	bool AppealAllowed() const { return HasAttribute(SK_ATTR_APPEAL_ALLOWED); }
	bool ForceDate() const { return HasAttribute(SK_ATTR_FORCE_DATE); }
	bool AdditionalText() const { return HasAttribute(SK_ATTR_ADDITIONAL_TEXT); }
	bool ExpertTransfer() const { return HasAttribute(SK_ATTR_EXPERT_TRANSFER); }
	bool IndirectEvent() const { return HasAttribute(SK_ATTR_INDIRECT_EVENT); }
	bool LawerGroup() const { return HasAttribute(SK_ATTR_LAWER_GROUP); }
	bool HasAccomply() const { return HasAttribute(SK_ATTR_HAS_ACCOMPLY); }
	bool EnterValidDate() const { return HasAttribute(SK_ATTR_ENTER_VALID_DATE); }
	bool IsSticker() const { return HasAttribute(SK_ATTR_ENTER_VALID_DATE);}

	char InitialStatus() const { return proc == SK_PROC_NULL || proc == SK_PROC_NOTIFYING ? SRS_NONE : SRS_NULL; }

	char subpoenaKind;
	char name[SIZE_OF_SUBPOENA_NAME];	// 2008:058 LPR: was SIZE_OF_NAME
	char alias[SIZE_OF_ALIAS];
	char kinds[SIZE_OF_SUBPOENA_KIND_KINDS];
	char statuses[SIZE_OF_SUBPOENA_KIND_STATUSES];
	char involvements[SIZE_OF_SUBPOENA_KIND_INVOLVEMENTS];
	char lawsuitKinds[SIZE_OF_SUBPOENA_KIND_ANOTHER_KINDS];
	char proc;
	char htemplate[SIZE_OF_HTEMPLATE_NAME];
	char genericKind;					// 2009:344 LRQ: for announces
	char info[SIZE_OF_SUBPOENA_INFO];
	mstr additionalText;
	char surroundKinds[SIZE_OF_SUBPOENA_KIND_ANOTHER_KINDS];

protected:
	char attributes[SIZE_OF_SUBPOENA_ATTRIBUTES];
	bool HasAttribute(char attr) const;
	friend void update_mini(TWindow *);		// 2016:340 !!TMP!! until fixed
};

extern TSubpoenaKind *SubpoenaKind;

// ----- TReceivedStatus -----------------------------------------------------
class TReceivedStatus: public TCharAliasGroup
{
public:
	TReceivedStatus();
	static TGroup *Clone();

	bool Clerk() const { return clerk == COMMON_YES; }
	bool Deliverer() const { return deliverer == COMMON_YES; }
	bool AllowDateReceived() const { return allowDateReceived == COMMON_YES; }
	bool AllowDateReturned() const { return allowDateReturned == COMMON_YES; }
	bool AllowSchedule() const { return allowSchedule == COMMON_YES; }
	bool ShowSubpoenaDate() const { return showSubpoenaDate == COMMON_YES; }

	char receivedStatus;
	char name[SIZE_OF_NAME];
	char alias[SIZE_OF_ALIAS];

protected:
	char clerk;
	char deliverer;
	char allowDateReceived;
	char allowDateReturned;
	char allowSchedule;
	char showSubpoenaDate;
};

extern TReceivedStatus *ReceivedStatus;

// ----- TDebtorStatus -------------------------------------------------------
class TDebtorStatus : public TXferAliasGroup	// 2012:018 Char -> Xfer
{
public:
	TDebtorStatus();
	static TGroup *Clone();

	char debtorStatus;
	char name[SIZE_OF_NAME];
};

extern TDebtorStatus *DebtorStatus;

// ----- TBanConSist ---------------------------------------------------------
class TBanConSist : public TLongAliasGroup
{
public:
	TBanConSist();
	static TGroup *Clone();

	int Count() const { return cache->Count(); }
	bool operator<=(const TBanConSist &that) const { return banConSist <= that.banConSist; }

	long banConSist;
	mstr text;
	long flags;

	// memory only
	char fake[1];
	char text1[SIZE_OF_BANCONSIST_TEXT + 8];
	char *text2;
};

extern TBanConSist *BanConSist;

// ----- TEisAliasGroup ------------------------------------------------------
class TEisAliasGroup : public TLongAliasGroup
{
public:
	TEisAliasGroup(const char *tName, TGroup *(*tClone)(), const char *valueFieldName, long *valueFieldData);

	char name[SIZE_OF_EISPP_NAME];
	long flags;

	TIntegerArray *lset;

	bool MatchCode(long code, long flags) const;
};

// ----- TEisVal -------------------------------------------------------------
class TEisVal : public TEisAliasGroup
{
public:
	TEisVal();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	bool Contains(long tNom, long tCode);
	const char *Seek(long tNom, long tCode);
	bool operator<=(const TEisVal &that) const;

	long nom;
	long code;
	char alias[SIZE_OF_EISPP_ALIAS];

protected:
	virtual void CacheCriteria(msql &m);
	virtual bool Match(const char *xCrit, long flags);	// lset, nom
	virtual bool CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags);
};

extern TEisVal *EisVal;

// ----- TSbeDesc ------------------------------------------------------------
class TSbeDesc : public TGroup
{
public:
	TSbeDesc();
	TSbeDesc(const char *tName, long tFlags, const char *tValue = NULL);
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);	// ignore .names
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	long vid;
	char name[SIZE_OF_SBEDESC_NAME];
	long flags;
	char value[SIZE_OF_SBEDESC_VALUE];

protected:
	void Init();
};

extern TSbeDesc *SbeDesc;

// ----- TSessionPrice -------------------------------------------------------
class TSessionPrice : public TGroup
{
public:
	TSessionPrice();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);

	bool operator<=(const TSessionPrice &that) const { return date <= that.date; }

	CDate date;
	double price;
};

extern TSessionPrice *SessionPrice;

// ----- TEisDepart ----------------------------------------------------------
class TEisDepart : public TEisAliasGroup
{
public:
	TEisDepart();
	static TGroup *Clone();

	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void AliasCriteria(msql &m, const char *, long);	// clear

	long nom;
	long code;
	char tlacr[SIZE_OF_ALIAS];
	char alias[SIZE_OF_EISPP_ALIAS];

protected:
	char uname[SIZE_OF_EISPP_NAME];

	virtual void CacheCriteria(msql &m);
	virtual bool Match(const char *xCrit, long flags);
	virtual bool CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags);
	virtual void Fixup(bool);
};

extern TEisDepart *EisDepart;

// ----- TEisCrime -----------------------------------------------------------
class TEisCrime : public TEisAliasGroup
{
public:
	TEisCrime();
	static TGroup *Clone();

	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

	long code;
	char alias[SIZE_OF_EISPP_ALIAS];

protected:
	char uname[SIZE_OF_EISPP_NAME];

	virtual bool Match(const char *xCrit, long flags);
	virtual bool CacheFill(TGroupList *glist, TGroupArray *array, const char *xCrit, long flags);
	virtual void Fixup(bool);
};

extern TEisCrime *EisCrime;

// ----- TEisTransl ----------------------------------------------------------
class TEisTransl : public TStringAliasGroup
{
public:
	TEisTransl();
	static TGroup *Clone();

	const char *Seek(const char *name);		// name if !found or DIAG

	char name[SIZE_OF_ALIAS];
	char text[SIZE_OF_NAME];
};

extern TEisTransl *EisTransl;

// ----- TEisPlace -----------------------------------------------------------
class TEisPlace : public TEisAliasGroup
{
public:
	TEisPlace();
	static TGroup *Clone();

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
	virtual void AliasCriteria(msql &m, const char *, long flags);

	long nom;
	long code;
	long uclp;
	char alias[SIZE_OF_EISPP_ALIAS];
};

extern TEisPlace *EisPlace;

// ----- TAccount ------------------------------------------------------------
class TAccount : public TStringAliasGroup
{
public:
	TAccount();
	static TGroup *Clone();

	virtual bool Insert();
	virtual void Print(mstr &m);
	virtual void AliasCriteria(msql &m, const char *xCrit, long);

	bool operator<=(const TAccount &that) const { return strcmp(name, that.name); }

	char ucn[SIZE_OF_UCN];
	char ucnType;
	char accountType;
	char bankAccount[SIZE_OF_BANK_ACCOUNT];
	char bic[SIZE_OF_BIC];
	char bankName[SIZE_OF_EXEC_TEXT];
	char name[SIZE_OF_NAME];
	long flags;

protected:
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual void CacheCriteria(msql &m) { m.Begin(Name); }
	void Generate();
};

extern TAccount *Account;

// ----- TLegalAidType -------------------------------------------------------
class TLegalAidType : public TLongAliasGroup
{
public:
	TLegalAidType();
	static TGroup *Clone();

	long legalAidType;
	char name[SIZE_OF_NAME];
};

extern TLegalAidType *LegalAidType;

// ----- TArchiveIndex -------------------------------------------------------
class TArchiveIndex : public TStringAliasGroup
{
public:
	TArchiveIndex();
	static TGroup *Clone();

	virtual bool Delete(bool exact);

	bool operator<=(const TArchiveIndex &that) const { return strcmp(archiveIndex, that.archiveIndex) <= 0; }

	char archiveIndex[SIZE_OF_ALIAS];
	char name[SIZE_OF_ARCHIVE_INDEX_NAME];
	long keep;
	long flags;
};

extern TArchiveIndex *ArchiveIndex;

// ----- TArchiveSubject -----------------------------------------------------
class TArchiveSubject : public TStringAliasGroup
{
public:
	TArchiveSubject();
	static TGroup *Clone();

	virtual void Fixup(bool force);
	bool Any() { return cache->Count(); }

	char archiveIndex[SIZE_OF_ALIAS];
	char kind;
	long subject;

protected:
	long aflags;

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	virtual void CacheCriteria(msql &m) { m.Begin(Name); }
	virtual bool Match(const char *xCrit, long flags);
};

extern TArchiveSubject *ArchiveSubject;

// ----- TCurrency -----------------------------------------------------------
class TCurrency : public TStringAliasGroup
{
public:
	TCurrency();
	static TGroup *Clone();

	virtual void Fixup(bool);

	const char *Integer(mstr &sum) { return Suffix(str(sum), true); }
	const char *Fraction(const char *fracp) { return Suffix(fracp, false); }

	char currency[SIZE_OF_CURRENCY];
	long order;
	char name[SIZE_OF_NAME];
	char numeric[SIZE_OF_CURRENCY_PART];
	char integer[3][SIZE_OF_CURRENCY_PART];
	long integerMfo;
	char fraction[3][SIZE_OF_CURRENCY_PART];
	long fractionMfo;

protected:
	virtual void CacheCriteria(msql &m);
	const char *Suffix(const char *sum, bool intPart);
};

extern TCurrency *Currency;

// ----- TSLCType ------------------------------------------------------------
class TSLCType : public TCharAliasGroup
{
public:
	TSLCType();
	static TGroup *Clone();

	char slcType;
	char name[SIZE_OF_NAME];
};

extern TSLCType *SLCType;

// ----- TSLCReason ----------------------------------------------------------
class TSLCReason : public TLongAliasGroup
{
public:
	TSLCReason();
	static TGroup *Clone();

	virtual bool Insert();
	void PrintPrefixed(mstr &m, const char *slcTypes);

	long slcReason;
	char slcType;
	char name[SIZE_OF_SLC_REASON_NAME];
	char lawKinds[SIZE_OF_LAW_KINDS];
	long flags;
	long order;

protected:
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
};

extern TSLCReason *SLCReason;

// ----- TVSSIndex -----------------------------------------------------------
class TVSSIndex : public TStringAliasGroup
{
public:
	TVSSIndex();
	static TGroup *Clone();

	char index[SIZE_OF_ALIAS];
};

extern TVSSIndex *VSSIndex;

// ----- TVSSIndexSet --------------------------------------------------------
class TVSSIndexSet : public TCharAliasGroup
{
public:
	TVSSIndexSet();
	static TGroup *Clone();

	void ExactCriteria(msql &m, const char *fields);
	virtual void InitCache();	// fill iAccepts, iRejects
	static TVSSIndexSet *Seek(char kind, CDate &date);

	void AddIndexes(TGroupArray &array) const;
	bool Matches(char tCourtType, const char *tTypes, char kind, const CDate &tStart, const CDate &tFinal) const;
	// NULL tTypes -> !punishment
	static void AddMatching(TGroupArray &array, char tCourtType, const char *tTypes, char kind, const CDate &tStart,
		const CDate &tFinal, bool entireSet);

	bool operator<=(const TVSSIndexSet &that) const;

	char courtType;
	CDate start;
	CDate final;
	char types[SIZE_OF_LAW_TYPES];
	char kinds[SIZE_OF_LAW_KINDS];
	char indexes[SIZE_OF_VSS_INDEXES];
	char accepts[SIZE_OF_VSS_ACCREJS];
	char rejects[SIZE_OF_VSS_ACCREJS];

	TIntegerArray iAccepts;
	TIntegerArray iRejects;
};

extern TVSSIndexSet *VSSIndexSet;

// ----- TReportRange --------------------------------------------------------
class TRangeReportGroup;

class TReportRange : public TGroup
{
public:
	TReportRange();
	static TGroup *Clone();

	// 2017:058 altered to combine subjects
	static void Load(TGroupArray &reportRanges, const TRangeReportGroup &group);

	bool operator<=(const TReportRange &that) const;
	bool InsideRange(char kind, long subject) const;
#if SPLITREPSUBJ
	static void Load(TGroupArray &reportRanges, const TRangeReportGroup &group, long si, bool instance);
	bool Consumes(long subject, long si) const;
#else  // SPLITREPSUBJ
	bool InsideRange(const TSubject *subject) const;
	bool Consumes(char type, long subject) const;
#endif  // SPLITREPSUBJ

	char types[SIZE_OF_LAW_TYPES];
	long instances;
	long rangeId;
	long singleSubject;	// memory only
	TIntegerArray includes;

	char subjects[SIZE_OF_REPORT_RANGE_SUBJECTS];
	char attrib[SIZE_OF_REPORT_RANGE_ATTRIB];
	char name[SIZE_OF_SUBJECT_NAME];
	char alias[SIZE_OF_NAME];
	//char letter;	// 2017:198 LPR: comout, unused

	long values[RANGE_COLUMNS_MAX];	// memory only
	size_t Positive() const;	// 1-based, 0 = none
	bool Visible() const;

protected:
	TIntegerArray ranges;

	// 2017:180 for update mini
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
};

extern TReportRange *ReportRange;

// ----- TWhat ---------------------------------------------------------------
class TWhat : public TLongAliasGroup
{
public:
	TWhat();
	static TGroup *Clone();

	bool operator<=(const TWhat &that) const { return what <= that.what; }

	long what;
	char name[SIZE_OF_NAME];
};

extern TWhat *What;
