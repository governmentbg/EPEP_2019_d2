struct begNEndNYear
 {
  begNEndNYear () { clear (); }
  begNEndNYear (const begNEndNYear & source) ;  // 01.nov.2001
  void clear () ;
  char wantXInstLawsToChar () const ;

  long int begN, endN, year ;

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
  bool want1stInstLaws ;
  bool want2ndInstLaws ;
#   endif  // of APPEAL
# endif  // of INSTANCE
  bool jan2005Format ;  // 10.feb.2005
  bool splitIn2 ;  // 10.feb.2005

  bool selectCrit1;  // 20080619 ->
  CDate begDate ;
  CDate endDate ;  // 20080619 <-

 } ;  // struct begNEndNYear

struct begNLEndNLYear : public begNEndNYear  //begNEndNYear_DB // 20080605 public begNEndNYear
 {
  begNLEndNLYear () ;
  begNLEndNLYear (const begNLEndNLYear & source) ;  // 01.nov.2001
  void clear () ;  // 01.nov.2001
  char begL, endL ;
  bool wantCheckup ;
  bool wantBrokenPrint ;
  bool wantPrintByPages ;
 } ;  // struct begNLEndNLYear

struct begNEndNYearTypes : public begNEndNYear
 {
  char types [SIZE_OF_TYPES] ;
  begNEndNYearTypes () ;
  begNEndNYearTypes (const begNEndNYearTypes & source) ;  // 01.nov.2001
  void clear () ;
  virtual void method();
 } ;  // struct begNEndNYearTypes

struct begNEndNYearTypesAC : public begNEndNYearTypes
 {
  bool allColumns ;
  CDate begDate ;  // 28.sep.2000
  CDate endDate ;  // 28.sep.2000
  long int begSerialNo ;  // 28.sep.2000
  char kinds [SIZE_OF_KINDS] ;  // 08.feb.2003
  bool automaticDistribution ;  // 05.mar.2003
  bool wantCheckup ;  // 27.may.2003
  begNEndNYearTypesAC () ;
  begNEndNYearTypesAC (const begNEndNYearTypesAC & source) ; // 01.nov.2001
  void clear () ;
 } ;  // struct begNEndNYearTypesAC

const begNEndNYearTypes & operator << (begNEndNYearTypes & const dest,
                                       const begNEndNYear & const src) ;

typedef TIntegerArray arrayOfLong;

ostream & operator << (ostream & os, const arrayOfLong & lA) ;

struct collCompBegDEndDBegN;  // 31.oct.2002

// ----- 2008:183 LPR: JUDGE_REPORT_ -----------------------------------------
const
	JUDGE_REPORT_INTERNAL	= 1,
	JUDGE_REPORT_MINISTRY	= 2,
	JUDGE_REPORT_COMBINED	= 3,
	JUDGE_REPORT_INCOMING_X	= 4,
	JUDGE_REPORT_DECIDED	= 5,
	JUDGE_REPORT_DISTRIB	= 6,
	JUDGE_REPORT_VSS	= 7,
	JUDGE_REPORT_VSS_APPEAL	= 8,

	JUDGE_REPORT_NULL	= 0
;

enum WhatPartOfDay
 {
  WholeDay,
  Morning,
  Afternoon
 } ;  // enum WhatPartOfDay

struct begMYEndMYTypeCIU
 {
  void clearData () ;
  begMYEndMYTypeCIU () ;
  begMYEndMYTypeCIU (const begMYEndMYTypeCIU & const source) ;
  ~begMYEndMYTypeCIU () ;
  const begMYEndMYTypeCIU &
    operator = (const begMYEndMYTypeCIU & const source) ;
  begMYEndMYTypeCIU (const collCompBegDEndDBegN & rCCBDEDBN) ;
  void operator >> (collCompBegDEndDBegN & rCCBDEDBN) const ;
  CDate BegDate () const ;
  CDate EndDate () const ;
  void addToArr (const long int l) ;
  const arrayOfLong * lArrPtr () const ;
// -------------- for collCompBegDEndDBegN date fields --------
	int begDay ;
	int endDay ;
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	bool puniAdmConcernLaws ;
	WhatPartOfDay whichPartOfDay ;  // 04.jun.2002
	CTime noon ;                  // 04.jun.2002
#endif  // APPEAL || ADMIN || AREAL || MILITARY
					    // 30.jan.2002 - begin
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	bool all2ndPuniLaws ;
  bool incomingExtend ;           // 20060607
#endif  // ADMIN || AREAL || MILITARY       // 30.jan.2002 - end

#if COURT_TYPE == COURT_REGIONAL
	bool internalUseCombined ;
#endif  // REGIONAL
// ----------------- ALL courts -------------------------------------------
	int begMonth ;
	long begYear ;
	int endMonth ;
	long endYear ;
	char types [SIZE_OF_TYPES] ;
	bool internalUse ;
	long composition ;
#if COURT_TYPE == COURT_REGIONAL
	long admLowSubjRange ;
    long admHighSubjRange ;
  bool forCorrupt ;               // 20070604
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	bool firstInstLaws ;
	bool secondInstLaws ;
	bool forBoss ;
	bool decidedLawsJudgeCheckup ;
	bool sessionsDistribution ;     // 04.jun.2002
  bool forCorrupt ;               // 20070604
#endif  // AREAL || MILITARY

#if COURT_TYPE == COURT_ADMIN
	bool firstInstLaws ;
	bool secondInstLaws ;
	bool forBoss ;
  bool forVSS ;                   // 20070809
	bool decidedLawsJudgeCheckup ;
	bool sessionsDistribution ;     // 04.jun.2002
#endif  // ADMIN

#if COURT_TYPE == COURT_APPEAL
	bool firstInstLaws ;
	bool secondInstLaws ;
	bool forBoss ;
	bool decidedLawsJudgeCheckup ;
	bool sessionsDistribution ;     // 04.jun.2002
	bool keepingMethodTimeBook ;
#endif  // APPEAL

// ---------------------- ALL courts -------------------------------------------
  bool nFBFromLaws ;
  bool finLawsCheckTerminated ;   // 01.nov.2002 - begin
  bool finLawsInconsistent ;
  bool sessCheckTerminated ;
  bool sessInconsistent ;         // 01.nov.2002 - end
  protected :
    arrayOfLong * pArrOfLong ;
  // 2008:183 LPR: JUDGE_REPORT_
  public:
  long variant;
 } ;
 // struct begMYEndMYTypeCIU

struct collBegDEndDBegN
 {
  collBegDEndDBegN () ;
  const collBegDEndDBegN & operator << (const collCompBegDEndDBegN t) ;
  char college ;
  CDate begD, endD ;
  long int begN ;
  bool wantAllResults ;
  bool wantSortByDateReturned ;
  bool wantShortFormat ;  // 23.jun.2000
  long int composition ;  // 24.jul.2000
  bool wantResultsCheckup ;  // 21.sep.2000
  bool jan2005Format ;  // 26.jan.2005
 } ;  // struct collBegDEndDBegN

struct collCompBegDEndDBegN
 {
  const collCompBegDEndDBegN & operator << (const begMYEndMYTypeCIU r) ;
  const collCompBegDEndDBegN & operator << (const collBegDEndDBegN c) ;
# if FOLLOW_LAWSUIT_TYPES
  char colleges [SIZE_OF_TYPES] ;
# else
  char colleges [SIZE_OF_COLLEGES] ;
# endif
  long int composit ;
  CDate begD, endD ;
  long int begN ;
# if INSTANCE
  bool firstInstLaws ;
  bool forBoss ;
  bool puniAdmConcernLaws ;
#   if APPEAL
  bool keepingMethodTimeBook ;
#   else  // of APPEAL  // 02.jan.2002
  bool all2ndPuniLaws ;  // 02.jan.2002
#   endif  // of APPEAL
# endif  // of INSTANCE
  bool wantClosedSessions ;
  bool wantClSessAllResults ;  // For communication with collBegDEndDBegN
                               // only
  bool nFBFromLaws ;  // For communication with begMYEndMYTypeCIU only
  long int extraVal0 ;
  long int extraVal1 ;
  long int extraVal2 ;
  long int extraVal3 ;
  long int extraVal4 ;
  long int extraVal5 ;
  long int extraVal6 ;
  long int extraVal7 ;  // 04.may.2000
  CDate heapingStartDate ;
  bool wantBook ;
  bool wantSummary ;
  bool wantJudgesSummary ;
  bool wantResultsCheckup ;  // 20.sep.2000
  bool wantSortByDateReturned ;  // For communication with
                                 // collBegDEndDBegN only
# if INSTANCE
  WhatPartOfDay whichPartOfDay ;
  CTime noon ;
# endif  // of INSTANCE
  long int begNo ;  // 11.sep.2000
  long int endNo ;  // 11.sep.2000
  long int year ;   // 11.sep.2000
  bool finLawsCheckTerminated ;  // 01.nov.2002 - begin
  bool finLawsInconsistent ;
  bool sessCheckTerminated ;
  bool sessInconsistent ;  // 01.nov.2002 - end
  bool jan2005Format ;  // 08.feb.2005
  bool splitIn2 ;  // 08.feb.2005
  bool orderBy1 ;  // 20051206
  bool orderBy2 ;  // 2015:198

  void clear () ;
  collCompBegDEndDBegN () ;
  collCompBegDEndDBegN (const collCompBegDEndDBegN & src) ;
  ~collCompBegDEndDBegN () ;
  const collCompBegDEndDBegN & operator =
    (const collCompBegDEndDBegN & source) ;
  void addToArr (const long int l) ;
  const arrayOfLong * lArrPtr () const ;
  arrayOfLong * lArrPtr_NC () const ;

  protected :
    arrayOfLong * pArrOfLong ;
 } ;  // struct collCompBegDEndDBegN

struct clgDate
 {
# if INSTANCE
#   if APPEAL  // 27.mar.2001
  char college ;
#   else  // of APPEAL  // 27.mar.2001 - begin
  char lawKinds [SIZE_OF_KINDS] ;
#   endif  // of APPEAL  // 27.mar.2001 - end
# else  // of INSTANCE
#   if FOLLOW_LAWSUIT_TYPES
  char colleges [SIZE_OF_TYPES] ;
#   else  // FOLLOW_LAWSUIT_TYPES
  char colleges [SIZE_OF_COLLEGES] ;
#   endif  // FOLLOW_LAWSUIT_TYPES
# endif  // of INSTANCE
  CDate date ;
  bool wantTimeSort ;
  bool headerWithLawType ;
  long int composition ;
  WhatPartOfDay whichPartOfDay ;
  CTime noon ;
  bool variant0 ;  // 27.mar.2002 - begin
  bool variant1 ;
  bool variant2 ;
  bool variant4 ;
  long int hall ;
  char city [SIZE_OF_UCLP_NAME] ;  // 27.mar.2002 - end
  int emptyRowsAfterEachTimeBlock ;  // 31.may.2001

  bool reOrder;        // 20060302

  clgDate () ;
 } ;  // struct clgDate

class TCountRecsQuery : public TQuery
 {
  public :
    TCountRecsQuery (const char *expr) : TQuery(expr) { }
    unsigned getRecsCount () { return _Count(); }
 } ;  // class TCountRecsQuery

//const int  // Moved to _const_.h on 13.feb.2003
//  SIZE_OF_NAME_FOR_BOOK = 63 + 1,
//  SIZE_OF_ADDRESS_FOR_BOOK = 63 + 1 ;

struct TClaimant
 {
  TClaimant () ;
  void clearData () ;
  char cName [SIZE_OF_NAME_FOR_BOOK] ;
  char cSettlement [SIZE_OF_NAME_FOR_BOOK] ;
  char cAddress [SIZE_OF_ADDRESS_FOR_BOOK] ;
 } ;  // struct TClaimant

struct TDefendant
 {
  TDefendant () ;
  void clearData () ;
  char dName [SIZE_OF_NAME_FOR_BOOK] ;
  char dSettlement [SIZE_OF_NAME_FOR_BOOK] ;
  char dAddress [SIZE_OF_ADDRESS_FOR_BOOK] ;
 } ;  // struct TDefendant

class lawRCDKeyToSidePair : public TRCDKeyToSidePair <TLawsuit, TSideWind>
 {
  public :
    lawRCDKeyToSidePair (TLawsuit & lS,
                         const bool includingAnonymous,  // 13.mar.2001
                         const char * turnInvolvs = NULL,
                         const char * const defInvsForExclude = NULL) ;
 } ;  // class lawRCDKeyToSidePair

class reqRCDKeyToSidePair : public TRCDKeyToSidePair <TRequest, TSideWind>
 {
  public :
    reqRCDKeyToSidePair (TRequest & lS,
                         const bool includingAnonymous,  // 13.mar.2001
                         const char * turnInvolvs = NULL,
                         const char * const defInvsForExclude = NULL) ;
 } ;  // class reqRCDKeyToSidePair

// 18.jan.2000 - end
// 13.mar.2001 - begin
class SessRCDKeyToSidePair : public TRCDKeyToSidePair <TSession, TSideWind>
 {
  public :
    SessRCDKeyToSidePair (TSession & lS,
                          const CDate * const pDate,  // 13.mar.2001
                          const bool includingAnonymous,  // 13.mar.2001
                          const char * turnInvolvs = NULL,
                          const char * const defInvsForExclude = NULL) ;
 } ;  // class sessRCDKeyToSidePair

class DecRCDKeyToSidePair : public TRCDKeyToSidePair <TDecision, TSideWind>
 {
  public :
    DecRCDKeyToSidePair (TDecision & lS,
                         const CDate * const pDate,  // 13.mar.2001
                         const bool includingAnonymous,  // 13.mar.2001
                         const char * turnInvolvs = NULL,
                         const char * const defInvsForExclude = NULL) ;
 } ;  // class DecRCDKeyToSidePair

// 13.mar.2001 - end
class ReqResolRCDKeyToSidePair :
        public TRCDKeyToSidePair <TRequestResolution, TSideWind>
 {  // 12.nov.2001
  public :
    ReqResolRCDKeyToSidePair (TRequestResolution & lS,
                              const CDate * const pDate,
                              const bool includingAnonymous,
                              const char * turnInvolvs = NULL,
                              const char * const defInvsForExclude = NULL);
 } ;  // class ReqResolRCDKeyToSidePair

struct bookAreas
 {
  bookAreas () ;
  void load (istream & is) ;
  unsigned int dataCipher ;
  unsigned int areaLen ;
  unsigned char align ;
  unsigned int from ;
 } ;  // struct bookAreas

class bookFormats
 {
  protected :
    bookAreas * pBA ;
    bookAreas * pBA1 ;
    int areasCnt, areas1Cnt ;
    bool loadOK ;
    void readAreas (ifstream & ifs, bookAreas * bAPtr, int arCnt) ;
    virtual bool loadFormat (const char * formatFN, TWindow * ) ;
  public :
    bookFormats (const char * formatFN, TWindow * parent) ;
    virtual ~bookFormats () ;
    bool formatLoadIsOK () const ;
    virtual void output (ostream & os, bool first) const = 0 ;
    virtual void notConstOutput (ostream & os, bool first) ;
    const bookAreas * getBkAreas () const ;
    int getBkAreasCnt () const ;
    const bookAreas * getBkAreas1 () const ;
    int getBkAreas1Cnt () const ;
 } ;  // class bookFormats

class TBookLawsuit : public TLawsuit, public bookFormats
 {
  public :
    TClaimant claimant ;
    TDefendant defendant ;
    TBookLawsuit (const char * formatFN, TWindow * parent) ;
    virtual void output (ostream & os, bool first) const ;
    virtual void clearData () ;
 } ;  // class TBookLawsuit

class bkFormatsPseudoOutput : public bookFormats
 {
  public :
    bkFormatsPseudoOutput (const char * formatFN, TWindow * parent) ;
    virtual void output (ostream & os, bool first) const ;
 } ; // class bkFormatsPseudoOutput

struct timeBookHeap
 {
  long int lWithDec ;
  long int lReturneds ;
  long int lSends ;
  long int lCancOthReasons ;
  long int lUpTo3Months ;
  long int lOver3Months ;
  long int lUpTo1MonthLong ;       //20050318
  long int lBetw1And3MonthsLong ;  //20050318
  long int lNotMajorDefendants ;
# if INSTANCE
  long int lByComplaints ;
  long int lByObjections ;
  long int lJudgemConfirmed ;
  long int lCndJdgApply ;
  long int lCndJdgRepeal ;  // 18.feb.2005
  long int lJdgChg_PunismDecr ;  // 18.feb.2005
  long int lJdgChg_PunismIncr ;  // 18.feb.2005
  long int lOtherChangesPunishmPart ;  // 18.feb.2005
  long int lOtherChangesCitizenPart ;  // 18.feb.2005
  long int lJdgRepealPartReconsider ;  // 18.feb.2005
  long int lJudgAbolishedPart ;
  long int lJdgFullyAboli_Ret ;
  long int lJdgFullyAboli_NewJ ;
  long int lCanceleds ;
# endif
  long int lNewlyCreateds ;
  long int lReceiveds ;
  long int lRetFrom2ndInstance ;
  long int lClaimFullyGranted ;
  long int lClaimPartialGranted ;
  long int lClaimNonsuit ;
  long int lCancelByAgreement ;
  long int lCancel_ExactByOtherCauses ;
# if INSTANCE
  long int lDecisionValidityConfirmed ;
  long int lDecisionRepealedPartilally ;
  long int lDecisRepealFully_Returned ;
  long int lDecisRepealFully_NewDecis ;
  long int lDecisRepeal_Reconsideration ;  // 18.feb.2005
  long int s_IIinst_Decision ;  // 20051208
  long int s_IIinst_Cease ;     // 20051208
# endif
  long int sFixeds ;
  long int sLefts ;
  long int sStoppeds ;
  long int sOtherSess ;
  long int sClosedSess ;

  long int lcAccusedsNum ;

  // Do not forget to correct clear () and +=
  // when Yoy add new data members !

  void clearData () ;
  timeBookHeap () ;
  void operator += (const timeBookHeap & oth) ;
 } ;  // struct timeBookHeap

// 14.feb.2000 - begin
//void markDecGlJResInTBHeap (const char result, timeBookHeap & tBH) ;
void markDecGlJResInTBHeap (const char decResult, const char sessResult,
                            timeBookHeap & tBH) ;
// 14.feb.2000 - end

struct accusedsDistribution
 {
  long int cJudgedsTotal ;
  long int cJudgedsCond ;
  long int cAccquits ;
  long int cFromCancLaws ;
  long int cFreeds ;
  long int cGenRecidive ;
  long int cSpecRecidive ;
  long int cNewCrimeDuringTestTime ;
  long int cDangerousRecidive ;  // 14.feb.2005

  // Do not forget to correct clear ()  and +=
  // when Yoy add new data members !

  void clearData () ;
  accusedsDistribution () ;
  void operator += (const accusedsDistribution & other) ;
 } ;  // struct accusedsDistribution

void markInDistribution (const TJudgement & lJ,
                         accusedsDistribution & accDistr) ;

class mergedKinds : public TDummyGroup
 {
  public :
    long int order ;
    char kinds [SIZE_OF_KINDS] ;
    char alias [SIZE_OF_NAME] ;
    mergedKinds () ;

    bool operator==(const mergedKinds &that) const { return order == that.order; }
    bool operator<(const mergedKinds &that) const { return order < that.order; }
    bool operator<=(const mergedKinds &that) const { return order <= that.order; }
 } ;  // struct mergedKinds

class subjectHeap : public nomHeap <TSubject>
 {
  public :
    subjectHeap () ;
 } ;  // class subjectHeap

class kindHeap : public nomHeap <TKind>
 {
  public :
    kindHeap () ;
 } ;  // class kindHeap

bool operator<=(const TKind &s1, const TKind &s2);

class subjectRangeHeap : public nomHeap <TSubjectRange>
 {
  public :
    subjectRangeHeap () ;
 } ;  // class subjectRangeHeap

bool operator<=(const TSubjectRange &sR1, const TSubjectRange &sR2);

class mergedKindsHeap : public nomHeap <mergedKinds>
 {
  public :
    mergedKindsHeap () ;
 } ;  // class mergedKindsHeap

class orderedDamnType : public TGroup
 {
  public :
    char companyType ;
    char name [SIZE_OF_ALIAS] ;
    long int order ;
    long int subject ;

    orderedDamnType () ;

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }
 } ;  // class orderedDamnType

class orderedDamnTypeHeap : public nomHeap <orderedDamnType>
 {
  public :
    orderedDamnTypeHeap () ;
 } ;  // class orderedDamnTypeHeap

bool operator<=(const orderedDamnType &s1, const orderedDamnType &s2);

class subjectHeapArray : public heapArray <subjectHeap>
 {
  protected :
    void load (const char * const fmtN, const char type) ;
  public :
    subjectHeapArray () ;
    subjectHeapArray (const char * const fmtN, const char type) ;
 } ;  // class subjectHeapArray

class kindHeapArray : public heapArray <kindHeap>
 {
  protected :
    void load (const char type) ;
    void load (const char * const fmtN, const char type) ;
  public :
    kindHeapArray () ;
    kindHeapArray (const char type) ;
    kindHeapArray (const char * const fmtN, const char type) ;
 } ;  // class kindHeapArray

class subjectRangeHeapArray : public heapArray <subjectRangeHeap>
 {
  protected :
    void load (const char * const fmtN, const char type,
               TWindow * ) ;
  public :
    subjectRangeHeapArray () ;
    subjectRangeHeapArray (const char * const fmtN, const char type,
                           TWindow * pParent = NULL) ;
    void incSRItems (const TSubject & subj) ;                       
 } ;  // class subjectRangeHeapArray

class mergedKindsHeapArray : public heapArray <mergedKindsHeap>
 {
  protected :
    void load (const char * const fmtN, TWindow * ) ;
  public :
    mergedKindsHeapArray () ;
    mergedKindsHeapArray (const char * const fmtN,
                          TWindow * pParent = NULL) ;
    void incMKItems (const char kind) ;
 } ;  // class subjectRangeHeapArray

# if INSTANCE
class orderedDamnTypeHeapArray : public heapArray <orderedDamnTypeHeap>
 {
  protected :
    void load () ;
    void load (const char * const fmtN, const char type) ;
  public :
    orderedDamnTypeHeapArray (const char * const fmtN, const char type) ;
 } ;  // class orderedDamnTypeHeapArray

# endif 

struct summaryData
 {
  char name [SIZE_OF_SUBJECT_NAME] ;
  long int heap ;

  void clear () ;
  summaryData () ;
 } ;  // struct summaryData

struct summaryFmtHeadData
 {
  begNEndNYear bNENY ;
  collCompBegDEndDBegN cCBDEDBN ;
  void clear () ;
  summaryFmtHeadData () ;
 } ;  // struct summaryFmtHeadData

// 17.apr.2000 - begin
struct BriefSessionData : public TDummyGroup
 {
  TRCDKey key ;
  CDate date ;
  CTime time ;
  char kind ;
  char result ;
  char judge [SIZE_OF_UCN] ;
  char judgeX [SIZE_OF_UCN] ;
  char reJudgeX [SIZE_OF_UCN] ;
  char president [SIZE_OF_UCN] ;
  BriefSessionData () ;
  static void
    BrSessData_to_TDatedCont (const BriefSessionData & bsd,
                              TDatedContainer & container) ;//18.sep.2000
  bool operator<=(const BriefSessionData &that) const { return operator<(that) || operator==(that); }
private:
  bool operator==(const BriefSessionData & other) const; //18.sep.2000
  bool operator<(const BriefSessionData & other) const; // 18.sep.2000
 } ;  // struct BriefSessionData

struct CodeValue  // 19.jun.2000
 {
  unsigned int code ;
  unsigned long int value ;
  void Clear () ;
  CodeValue (const unsigned int aCode = 0,
		 const unsigned long int aValue = 0) ;
  void operator = (const CodeValue & src) ;
  bool operator<=(const CodeValue &that) const { return code <= that.code; }
 } ;  // struct CodeValue

typedef TISArray<CodeValue> CodeValueArray;

struct SimpleStrings
 {  // 04.mar.2003
  const char * code ;
  const char * str ;
  void Clear () ;
  SimpleStrings (const char* const aCode = NULL,
    const char* const aStr = NULL) ;
  const SimpleStrings & operator = (const SimpleStrings & src) ;
  bool operator<=(const SimpleStrings &that) const { return operator<(that) || operator==(that); }
private:
  bool operator<(const SimpleStrings &other) const;
  bool operator==(const SimpleStrings &other) const;
 } ;  // struct SimpleStrings

class SimpleStringsArray : public TIArray <SimpleStrings>
 {  // 04.mar.2003
  public :
    SimpleStringsArray () { }
 } ;  // class SimpleStringsArray

bool operator<=(const TNTResult &res0, const TNTResult &res1);

struct ResultsSet
 {
  char sessRes ;
  char decRes ;
  char lawGrant ;
  bool afterFinish ;
  bool dateRetAfterFinish ;
  bool abolished ;
  long int count ;
  void Clear () ;
  ResultsSet () ;
  ResultsSet (const char sessR, const long int cnt = 0,
		  const char decR = '\0',
		  const char lawG = GRANT_NULL,
		  const bool afterFin = false,
		  const bool dateRetAfterFin = false,
		  const bool abolish = false) ;
  const ResultsSet & operator = (const ResultsSet & src) ;
  bool operator<=(const ResultsSet &that) const { return operator<(that) || operator==(that); }
private:
  bool operator<(const ResultsSet &other) const;
  bool operator==(const ResultsSet &other) const;
 } ;  // struct ResultsSet

class ResultsSetArray : public TISArray <ResultsSet>
 {
  public :
    ResultsSetArray () { }
    void operator += (const ResultsSet & rS) ;
 } ;  // class ResultsSetArray

struct TRCDKey_ : public TRCDKey  // 13.nov.2000
 {
  void Clear () ;
  TRCDKey_ () ;
  TRCDKey_ (const TRCDKey & source) ;
  TRCDKey_ (const TRCDKey_ & source) ;
  TRCDKey_ (char type_, long int no_, long int year_) ;
  const TRCDKey_ & operator = (const TRCDKey_ & source) ;
  bool IsEmpty () const ;
  bool FilledOK () const ;
  char * Print (char * const s, const size_t sSz) const ;  // returns s
  char * PrintShort (char * const s, const size_t sSz) const ; // returns s
  char * PrintShortest (char * const s, const size_t sSz) const ; // returns s
  char * PrintAlias (char * const s, const size_t sSz) const ; // returns s
  bool operator == (const TRCDKey_ & other) const ;
 } ;  // struct TRCDKey_

ostream & operator << (ostream & os, const TRCDKey_ & k) ;

class TRCDKey_Array : public TIArray<TRCDKey_>
{
public:
	TRCDKey_Array() { }
	bool Contains(const TRCDKey_ &key) const;
};

struct TRCDKeyContainer_ : public TRCDKey_  // 10.apr.2001
 {
  char kind ;

  void Clear () ;
  TRCDKeyContainer_ () ;
  TRCDKeyContainer_ (const TRCDKey & key, const char kind_) ;
  TRCDKeyContainer_ (const TRCDKeyContainer_ & source) ;
  TRCDKeyContainer_ (const TRCDKeyContainer & source) ;
  TRCDKeyContainer_ (char type_, long int no_, long int year_,
                     char kind_) ;
  const TRCDKeyContainer_ & operator = (const TRCDKeyContainer_ & source) ;
  bool IsEmpty () const ;
  bool FilledOK () const ;
  char * Print (char * const s, const size_t sSz) const ;  // returns s
  char * PrintAlias (char * const s, const size_t sSz) const ; // returns s
  bool operator == (const TRCDKeyContainer_ & other) const ;
 } ;  // struct TRCDKeyContainer_

ostream & operator << (ostream & os, const TRCDKeyContainer_ & k) ;

struct Trade634DlgData
 {
  CDate begD ;
  CDate endD ;
  TRCDKey_ lawKey ;
  long int begSerialNo ;
  bool truncChainAtEndD ;
  void Clear () ;
  Trade634DlgData () ;
 } ;  // struct Trade634DlgData

enum PersLstWhatToPrint
 {  // 19.dec.2001
  NotifiedsOnly = 0,
  NotifiedsOrFuture = 1,
  WholeHeap = 2
 } ;  // enum PersLstWhatToPrint

struct PersonListDlgData
 {
  PersLstWhatToPrint whatToPrint ;
  long int emptyRowsAtEnd ;
  bool sentList ;
  bool mergeCitizenNotifications ;
  bool followColumnNames ;
  bool suppressDates ;  // 07.nov.2003
  bool addLetter ;  // 20070726     
  void Clear () ;
  PersonListDlgData () ;
 } ;  // struct PersonListDlgData

class AloneDatedContainer_xK : public TDatedContainer
 {  // xK means that xCrit == & kind
  public :
    AloneDatedContainer_xK (const char * name = NULL) ;
    void Clear () ;
 } ;  // class AloneDatedContainer_xK

class LinkedXKXKDatedContainer : public AloneDatedContainer_xK
 {
  public :
    AloneDatedContainer_xK link ;
    LinkedXKXKDatedContainer (const char * name = NULL) ;
    void Clear () ;
 } ;  // class LinkedXKXKDatedContainer

class LinkedXKXKDatedContainerSK : public LinkedXKXKDatedContainer
 {  // Declared because of the ugly
    // "more than one session for a day is allowed"
  public :
    char sessKind ;
    CDate linkImportantDate ;  // 19.dec.2000
    AloneDatedContainer_xK surround ;
    LinkedXKXKDatedContainerSK (const char * name = NULL) ;
    void Clear () ;
    CDate ChainDate () const ;
    bool operator<=(const LinkedXKXKDatedContainerSK &that) const { return operator<(that) || operator==(that); }
private:
    bool operator==(const LinkedXKXKDatedContainerSK &other) const;
    bool operator<(const LinkedXKXKDatedContainerSK &other) const;
 } ;  // LinkedXKXKDatedContainerSK

class TL634AloneDatedContainer : public AloneDatedContainer_xK
 {  // 08.oct.2001
  public :
    TL634AloneDatedContainer (const char * name = NULL) ;
    bool operator<=(const TL634AloneDatedContainer &that) const { return operator<(that) || operator==(that); }
private:
    bool operator==(const TL634AloneDatedContainer &other) const;
    bool operator<(const TL634AloneDatedContainer &other) const;
 } ;  // class TL634AloneDatedContainer

class IORegDatedContainer : public AloneDatedContainer_xK
 {
  public :
    IORegDatedContainer (const char * name = NULL) ;
    bool operator<=(const IORegDatedContainer &that) const { return operator<(that) || operator==(that); }
private:
    bool operator==(const IORegDatedContainer & other) const;
    bool operator<(const IORegDatedContainer & other) const;
 } ;  // class IORegDatedContainer

struct DatedContainer_ : public TRCDKey_  // 31.jan.2001
 {
  char kind ;
  CDate date ;
  void Clear () ;
  DatedContainer_ () ;
  DatedContainer_ (const TRCDKeyContainer & source) ;
  DatedContainer_ (const DatedContainer_ & source) ;
  DatedContainer_ (char type_, long int no_, long int year_,
                   char kind_, CDate date_) ;
  DatedContainer_ (const TRCDKey key_, char kind_, CDate date_) ;//19.jun.2003
  const DatedContainer_ & operator = (const DatedContainer_ & source) ;
  bool IsEmpty () const ;
  bool FilledOK () const ;
  char * Print (char * const s, const size_t sSz) const ;  // returns s
  bool operator == (const DatedContainer_ & other) const ;
 } ;  // struct DatedContainer_

ostream & operator << (ostream & os, const DatedContainer_ & dC) ;

struct DatedSide_ : public DatedContainer_  // 30.nov.2001
 {
  char ucnType ;
  char ucn [SIZE_OF_UCN] ;
  char involvement ;
  void Clear () ;
  DatedSide_ () ;
  DatedSide_ (const TSideWind & source) ;
  DatedSide_ (const DatedSide_ & source) ;
  DatedSide_ (char type_, long int no_, long int year_, char kind_,
              CDate date_, char ucnType_, const char * ucn_,
              char involvement_) ;
  const DatedSide_ & operator = (const DatedSide_ & source) ;
  bool IsEmpty () const ;
  bool FilledOK () const ;
  char * Print (char * const s, const size_t sSz) const ;  // returns s
  bool operator == (const DatedSide_ & other) const ;
  bool operator != (const DatedSide_ & other) const ;
 } ;  // struct DatedSide_

ostream & operator << (ostream & os, const DatedSide_ & dS) ;

struct WEB_CheckupDlgData
 {
  char lawKinds [SIZE_OF_KINDS] ;
  CDate begDate ;
  CDate endDate ;
  bool decisionsNeed ;
  char outFileName [SIZE_OF_PATH] ;
  bool police ;  // 29.jan.2002
  bool socialInterest ;  // 01.mar.2004
  bool withJudgeNames;	// 2007:306 LPR: +related
  bool orderByDate1st;	// 2007:313 LPR: +related

  void Clear () ;
  WEB_CheckupDlgData () ;
 } ;  // struct WEB_CheckupDlgData

struct TRCDKeyContainerD_ : public TRCDKeyContainer_  // 23.may.2001
 {
  CDate date ;

  void Clear () ;
  TRCDKeyContainerD_ () ;
  TRCDKeyContainerD_ (const TRCDKey & key, const char kind_,
                      const CDate date_) ;
  TRCDKeyContainerD_ (const TRCDKeyContainerD_ & source) ;
  TRCDKeyContainerD_ (char type_, long int no_, long int year_,
                      char kind_, const CDate date_) ;
  const TRCDKeyContainerD_ &
    operator = (const TRCDKeyContainerD_ & source) ;
  bool IsEmpty () const ;
  bool FilledOK () const ;
  char * Print (char * const s, const size_t sSz) const ;  // returns s
  bool operator == (const TRCDKeyContainerD_ & other) const ;
 } ;  // struct TRCDKeyContainerD_

ostream & operator << (ostream & os, const TRCDKeyContainerD_ & k) ;

class sessLstData : public AloneDatedContainer_xK  // TSession 31.may.2001
 {
  public :
    int number, row ;
    CTime time ;  // 31.may.2001

    void clearData () ;
    sessLstData () ;
    sessLstData (TSession & s, int num, int r) ;
    bool operator<=(const sessLstData &that) const { return row != that.row ? row < that.row : number <= that.number; }
  private :
    void addNewItemsToGroup () ;  // 31.may.2001
 } ;  // class sessLstData

class sessLstTimeSortData : public  sessLstData  // TSession  31.may.2001
 {
  public :
    sessLstTimeSortData () ;
    bool operator<=(const sessLstTimeSortData &that) const { return operator<(that) || operator==(that); }
private:
    bool operator==(const sessLstTimeSortData &that) const;
    bool operator<(const sessLstTimeSortData &that) const;
 } ;  // class sessLstTimeSortData

struct DoubleNos
 {
  int no0 ;
  int no1 ;
  void Clear () ;
  DoubleNos () ;
  const DoubleNos & operator = (const DoubleNos & src) ;
  bool operator<=(const DoubleNos &that) const { return no0 != that.no0 ? no0 < that.no0 : no1 <= that.no1; }
 } ;  // struct DoubleNos

class OutRegDatedContainer : public IORegDatedContainer
 {  // 11.jan.2002
  public :
    char receiver [SIZE_OF_UCN] ;
    long int inNo ;
    long int inYear ;
    char result ;
    CDate returned ;
    char resultI ;
    char resultII ;
    char resultSuper ;
    char resultMJ ;

    OutRegDatedContainer (const char * name = NULL) ;
    void Clear () ;
 } ;  // class OutRegDatedContainer

struct Kind_
 {
  char k ;
  void Clear () ;
  Kind_ (const char k_) ;
 } ;  // struct Kind_

ostream & operator << (ostream & os, const Kind_ & k) ;

struct Type_
 {
  char t ;
  void Clear () ;
  Type_ (const char t_) ;
 } ;  // struct Type_

ostream & operator << (ostream & os, const Type_ & t) ;

struct Result_
 {
  char r ;
  void Clear () ;
  Result_ (const char r_) ;
 } ;  // struct Result_

ostream & operator << (ostream & os, const Result_ & r) ;

struct Resolution_
 {
  char r ;
  void Clear () ;
  Resolution_ (const char r_) ;
 } ;  // struct Resolution_

ostream & operator << (ostream & os, const Resolution_ & r) ;

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
struct BankruptcyWideDlgData
 {
  bool wantNoncompleted ;
  bool wantCompleted ;
  CDate lawBegDate ;
  CDate lawEndDate ;
  CDate completedBegDate ;
  CDate completedEndDate ;
  bool wantExtraTitle;
  char yourOutNo [32] ;
  bool wantNoncompletedWeb ;  // 10.jun.2003 - begin
  bool wantCompletedWeb ;
  char noncompletedWebPath [SIZE_OF_PATH] ;
  char completedWebPath [SIZE_OF_PATH] ;  // 10.jun.2003 - end

  void Clear () ;
  BankruptcyWideDlgData () ;
 } ;  // struct BankruptcyWideDlgData

#   endif  // of APPEAL
# endif  // of INSTANCE

struct UcnLng
{  // 14.aug.2002
	UcnLng();
	UcnLng(const char* const u, const long l, const CDate d_);
	UcnLng& operator = (const UcnLng& src);
  bool operator == (const UcnLng& src) const;
	void Clear();
	char ucn[SIZE_OF_UCN];
	long lng;
	CDate d;
};  // struct UcnLng

class TMaxLongGroup : public TDummyGroup
{
public:
	TMaxLongGroup();

	long max;
};

class TCountLongGroup : public TDummyGroup
{
public:
	TCountLongGroup();

	long count;
};

// 20051010 --> new structure for ZZDN book
# if ! INSTANCE
// Regional court only
struct ZZDN_Dlg_data : public begNEndNYearTypes
 {
  ZZDN_Dlg_data () { clear (); }

  const ZZDN_Dlg_data & operator =
    (const ZZDN_Dlg_data & source) ;

  bool selectCrit1;

  long int FromNo ;
  long int ToNo ;
  long int Year ;

  CDate begD ;
  CDate endD ;
  long int begN ;

  void clear () ;
 } ;  // struct ZZDN_Dlg_data
# endif
// 20051010 <-- new structure for ZZDN book
