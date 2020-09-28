class TTimeBookThread : public TWaitThread
{
	public:
	TTimeBookThread(TCountRecsQuery* const pQuery,
			 TQuery* const pAbPrQuery,
			 timeBookOutFile& of, const char* const formatFName,
			 const collCompBegDEndDBegN& rCCBDEDBN);
	public:  // Methods
    virtual void Execute();
	protected:  // Methods
    bool ArgsOK() const;
    void ChangeCurrentItemText(const TSession& sess, const int current,
					 const int total);
    void WriteOneSession(TSession& sess, TLawsuit& lawsuit,
	unsigned& sessSerialNo, TSession& abolishingSess, timeBookHeap& tBH,
	accusedsDistribution& accDistrHeap, CDate& prevDate, CDate& prevEnd);  // 02.feb.2005
    void WriteHeader();  // 02.feb.2005
    void WriteTail();  // 02.feb.2005
    void ChangePageIfNecessary(timeBookColumns& tBC, bool dateRowNeed);  // 02.feb.2005
	protected:  // Data
    TCountRecsQuery* const pQ;
    TQuery* const pAbPrQ;
    timeBookOutFile& outF;
    const char* const formatFN;
    const collCompBegDEndDBegN& cCBDEDBN;
    const char* const description;
};

enum SessionJudgesNumber  // 08.may.2000
 {
  UndefinedNumberOfJudges = -2,
  JudgeOnly = -1,
  OneJudge = 1,
  TwoJudges = 2,
  ThreeJudges = 3
 } ;  // SessionJudgesNumber

// 17.apr.2000 - do not forget the overriddenResultsSessions class
class overriddenResultsSessionsBS;  // 31.oct.2002

class TJudgeCheckupThread : public judgeCheckupEnum,
			   public TCheckupThread
 {
  private :
    static const collCompBegDEndDBegN cCBDEDBN_Empty ;
    void putJudgeCheckupDataInArr (const judgeCheckupData * jCDPtr) ;
  protected :
    const begMYEndMYTypeCIU bMYEMYTCIU ;  // Attention !!! There
	// is <const_cast> to bMYEMYTCIU FirmJudgeCheckup
    char formatFN [128] ;
    TISArray <judgeCheckupData> * pArr ;
    const bool trueJudgeCheckup ;
    const collCompBegDEndDBegN cCBDEDBN ;
    TWaitWindow * pWW ;
    const unsigned int cipherIfNotTrueGudgeCheckup ;
    overriddenResultsSessionsBS * pASRArr ;
    bool sayAlmostDone ;  // 18.may.2002
    int v_mainMenuItem ;  // 20061207
    void incrementValues_Law (TLawsuit & l, unsigned int dataCiph,
# if INSTANCE
                              unsigned int dataCiph2nd,
# endif
                              const long int iV = 1
                             ) ;
    void addOverriddenToLefts () ;
# if INSTANCE
#   if APPEAL
		void specificAplCourtNewsDistribute (const bookAreas * bkAreas,
																				 int areasCnt,
																				 const CDate * pHighRange,
																				const CDate * pLowRange = NULL,
											const char * const defaultRequestKindsString = NULL
																				) ;
		void specificAplCourtDecDistribute (const bookAreas * bkAreas,
																				int areasCnt,
                                        const CDate * pHighRange,
                                        const CDate * pLowRange = NULL,
                      const char * const defaultRequestKindsString = NULL
                                       ) ;
#   endif  // APPEAL
# endif  // INSTANCE
    virtual  // 04.dec.2000
    void makeBookFormatFN () ;
    void makeArr () ;
    bool columnRequired (unsigned int dataCiph,
				 const bookAreas * bkAreas,
                         int areasCnt) const ;
    void addJCDToArrayIfNecessary (const char * const judge) ;
    bool skipThisLaw (const char lK) ;
    bool skipThisLaw (const long int subj) ;
    void incrementValues (TQuery & q, unsigned int dataCiph
# if INSTANCE
                          , unsigned int dataCiph2nd
# endif
                                                    ,
                          const long int incrementWith
                         ) ;
    void lawsSessDistribute (unsigned int dataCiph,
# if INSTANCE
                         unsigned int dataCiph2nd,
# endif
                         const bookAreas * bkAreas,
				 int areasCnt,
				 const CDate * pHighRange,
				 const CDate * pLowRange = NULL,
                         const char * const resultsString = NULL,
                         const char * const requestKindsString = NULL,
                         const char * const ageString = NULL,
                         const bool finishedTest = true,
                         const char * const sourceTypesString = NULL,
                         const char * const grantedsString = NULL,
                         const char * const sessKindsString = NULL,
				 const bool sKMatch = false,
                         const bool excludeCreatedsLater = false,
                         const bool rSMatch = true,
                         const bool sessDateTest = false,
                         const bool bindSessDateAndFinished = true,
                         const char * const decisResultsString = NULL,
                       const bool withRequestKindsString = true,
          const char * const lawKindsString = NULL,  // 08.apr.2002 - begin
                         const bool lKMatch = true,  // 08.apr.2002 - end
                         const long int incrementWith = 1
                         ) ;
  // If You pass dataCipher2nd different from dataCipher, it must
  // mean the same thing as dataCipher (but for 2nd instance laws)

  // 20061212 to get the same data set as reportBook :: lawsDistribute -->
  //          as reportBook lawDistribute
    void lawsDistribute_RJ (unsigned int dataCiph,
# if INSTANCE
				 unsigned int dataCiph2nd,                  //  1
# endif
                         const bookAreas * bkAreas,                 //  2
                         int areasCnt,                              //  3
                         const CDate * pHighRange,                //  4
				 const CDate * pLowRange = NULL,          //  5
                         const char * resultsString = NULL,         //  6
                         const char * requestKindsString = NULL,    //  7
                         const char * ageString = NULL,             //  8
                         const bool finishedTest = true,            //  9
                         const char * sourceTypesString = NULL,     // 10
				 const char * grantedsString = NULL,        // 11
                         const char * sessKindsString = NULL,       // 12
                         const bool sKMatch = false,                // 13
                         const bool excludeCreatedsLater = false,   // 14
                         const bool rSMatch = true,                 // 15

                         const char * const decResultsString = NULL,
				 const char * const lawKindsString = NULL,
				 const bool lKSMatch = true,
				 const senderChecksActions aboutTheSender = IgnoreSender,
                         const bool requestCurrentlyQuick = false,
                         const long int incrementWith = 1
                         , // 20070119 -->
                         const char * connectTypesString = NULL,   // 22
                         const bool lConnSMatch = true            // 23

                         ) ;
    void sessDistribute_RJ (unsigned int dataCiph,                  //  1
# if INSTANCE
                         unsigned int dataCiph2nd,                  //  1
# endif
                         const bookAreas * bkAreas,                 //  2
                         int areasCnt,                              //  3
                         const CDate * pHighRange,                //  4
                         const CDate * pLowRange = NULL,          //  5
                         const char * resultsString = NULL,         //  6
                         const char * sessKindsString = NULL,       //  7
                         const bool sKSMatch = false,               //  8
               const bool excludeClosedSess = true, // 05.may.2001  //  9
               const bool beforeLawFinish = true  // 25.may.2001    // 10
                        ) ;

//  20070202
# if INSTANCE
# else       // REGIONAL COURT only  -->
    void appealedLawsDistribute_RJ
					  (const unsigned int dataCiph,
                                 const bookAreas * bkAreas,
                                 const int areasCnt,
                                 const CDate highRange,
					   const CDate lowRange) ;

# endif                    
    void incrementValuesFixedSess (TQuery & q, unsigned int dataCiph) ;
    void fixedSessDistribute (unsigned int dataCiph,
                              const bookAreas * bkAreas,
                              const int areasCnt,
                              const CDate highRange,
                              const CDate lowRange,
                              const char * const sessKindsString = NULL,
                              const bool sKMatch = false,
                             const char * const sessResultsString = NULL
                             ) ;
    void incrementAfterFinishValues (TQuery & q, unsigned int dataCiph,
# if INSTANCE
						 unsigned int dataCiph2nd,
# endif
                                     const long int incrementWith) ;
    void afterFinishDistribute (unsigned int dataCiph,
# if INSTANCE
                                unsigned int dataCiph2nd,
# endif
                                const bookAreas * bkAreas,
                                int areasCnt,
                                const CDate highRange,
                                const CDate lowRange,
                                const char * const sessKindsString = NULL,
                                const bool sKMatch = false,
                    const bool conformWithSessDate = true,  // 25.jul.2000
                    const bool exscludeResDecree = true,  // 25.jul.2000
                               const char * const decisKindsString = NULL,
                                const long int incrementWith = 1,
                    const char * const sessResStr = NULL,  // 01.nov.2000
                    const bool sRSMatch = false  // 01.nov.2000
                                ) ;
#if USE_JUDGE_CHECKUP_OTHERS
    void incrementReqResolutionValues (  // 07.jun.2004 - begin
                                       TRequestResolution& rReqResol,
                                       unsigned int dataCiph,
# if INSTANCE
                                       unsigned int dataCiph2nd,
# endif
						   TRequest& rReq, TLawsuit& rLaw,
                                       const long int incrementWith = 1) ;
    void reqResolutionsDistribute (unsigned int dataCiph,
# if INSTANCE
					     unsigned int dataCiph2nd,
# endif
                                   const bookAreas * bkAreas,
                                   int areasCnt,
                                   const CDate highRange,
                                   const CDate lowRange,
                                   const long int incrementWith = 1
                                  ) ;
    void incrementInRegEventValues (TInRegEvent & rIRE,
                                    unsigned int dataCiph,
# if INSTANCE
                                    unsigned int dataCiph2nd,
# endif
						TInReg& rIR, TLawsuit& rLaw, TRequest& rReq,
						const long int incrementWith = 1) ;
    void inRegEventSDistribute (unsigned int dataCiph,
# if INSTANCE
                                unsigned int dataCiph2nd,
# endif
                                const bookAreas * bkAreas,
                                int areasCnt,
                                const CDate highRange,
                                const CDate lowRange,
                                const long int incrementWith = 1
                                ) ;  // 07.jun.2004 - end
#endif  // of USE_JUDGE_CHECKUP_OTHERS
    void fillArr (const CDate & bP, const CDate & eP,
                  const bookAreas * bkAreas, int areasCnt) ;
    void makeGeneralRow (judgeCheckupData & rJCD) ;
    void writeOneRow (const judgeCheckupData * pJCD,
                      judgeCheckupOutFile & outF,
                      const CDate * const dateForMonthColumn = NULL,
                      const int serialNo = 0  // 08.jan.2001
                     ) ;
    void prtJdgChkup () ;
    const char * reqKindsString () ;
    virtual judgeCheckupData * newDataItem () const ;  // 04.dec.2000
    virtual judgeCheckupOutFile *
              newOutFile () const ;  // 04.dec.2000
    virtual judgeCheckupColumns *
              newJudgeCheckupColumns (const char * const formatFN,
						  const judgeCheckupData & jCD,
				 const CDate * const pDateForMonthColumn = NULL,
						const int serialNo = 0  // 08.jan.2001
						 ) const ;  // 04.dec.2000
    virtual void Execute();
  public :
    TJudgeCheckupThread (const begMYEndMYTypeCIU & rBMYEMYTCIU,
			int mainMenuItem = 0 // 1=TimeBook; 2=Judge Report ; 3=RCourt Report+judge's report // 20061206
		     ) ;
    TJudgeCheckupThread (const collCompBegDEndDBegN & rCCBDEDBN,
    const unsigned int ciphIfNotTrueGudgeCheckup = OFFSET_TIME_BOOK_CIPHER
		     ) ;
    virtual ~TJudgeCheckupThread () ;
    virtual void printJudgeCheckup () ;
    const TISArray <judgeCheckupData> * arrPtr () const { return pArr; }
    virtual void prepare () ;
 } ;

class plainJudgeCheckup
 {
  protected :
    TJudgeArray judgesSet ;
    TISArray <subject_With_LongIntArray> * pArr ;
    TWindow * const pParent ;
    const begMYEndMYTypeCIU bMYEMYTCIU ;
    void makeJudgesSet (TQuery & const q) ;
    void makeArr (TQuery & const q, const bool first = true, const bool second = false) ;
    void incCell (const long subject, const char * const ucn) ;
    virtual bool MakeCriteria (char * const crit, const size_t critSz,
                               const CDate & bP,
                               const CDate & eP) ;  // 12.apr.2001
    virtual void fillArr (const CDate & bP, const CDate & eP, const bool first=true, const bool second=false) ;
    void makeGeneralRow (subject_With_LongIntArray & rSWJHA) ;
  public :
    plainJudgeCheckup (TWindow * parent,
                       const begMYEndMYTypeCIU & rBMYEMYTCIU) ;
    virtual ~plainJudgeCheckup () ;
    void prepare ( const bool first = true, const bool second = false) ;  // 12.apr.2001
    void printPlainJudgeCheckup () ;
    static void makeFmtFN (const begMYEndMYTypeCIU &
                             bMYEMYTCIU,  // 14.aug.2002
                           char * const fmtFN, const size_t fmtFNSz) ;
      // 'static' and 'public' --> on 14.aug.2002
 } ;  // class plainJudgeCheckup

class DecidedLawsPlainJudgeCheckup : public plainJudgeCheckup
 {  // 12.apr.2001
  protected :
    bool MakeNonFirmCriteria (char * const crit, const size_t critSz,
                              const CDate & bP, const CDate & eP) ;
    virtual bool MakeCriteria (char * const crit, const size_t critSz,
                               const CDate & bP, const CDate & eP) ;
  public :
    DecidedLawsPlainJudgeCheckup (TWindow * parent,
                                  const begMYEndMYTypeCIU & rBMYEMYTCIU) ;
 } ;  // class DecidedLawsPlainJudgeCheckup

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
class DecidedFirmLawsPlainJudgeCheckup : public plainJudgeCheckup
 {  // 12.apr.2001
  protected :
    bool MakeFinishedLawsCriteria (char * const crit,
					     const size_t critSz,
                                   const CDate & bP,
                                   const CDate & eP) ;
    bool MakeRegisteredFirmsCriteria (char * const crit,
                                      const size_t critSz,
                                      const CDate & bP,
                                      const CDate & eP) ;
    void FillArr_ (const CDate & bP, const CDate & eP,
                   const int which) ;
    virtual void fillArr (const CDate & bP, const CDate & eP, const bool first=true, const bool second=false) ;
  public :
    DecidedFirmLawsPlainJudgeCheckup (TWindow * parent,
                                  const begMYEndMYTypeCIU & rBMYEMYTCIU) ;
 } ;  // class DecidedFirmLawsPlainJudgeCheckup
#   endif  // of APPEAL
# endif  // of INSTANCE

class AppealedLawsInputDiary
// Look for an old version of this class in _notinus.bak (28.sep.2000)
 {
  protected :
    begNEndNYearTypesAC bnenyt ;
    TWindow * pParent ;
    char formatFN [128] ;
    char criteria [1024] ;
    TQuery * pQ ;
    void MakeBookFormatFN () ;
    bool MakeCriteria () ;
    void MakeQuery () ;
    void GetQueryMaxInRegNo () ;
    virtual void PrintOneItem (TInReg & rInReg,
					 AppealedLawsInputDiaryOutFile & of,
                               const int serialNo,
                               TInRegEvent * const pInRegEvent = NULL) ;
  public :
    AppealedLawsInputDiary (TWindow * parent,
                            const begNEndNYearTypesAC & const rBNENYT
                           ) ;
    virtual ~AppealedLawsInputDiary () ;
    virtual void PrintAppealedLawsInputDiary () ;
    static bool
    InregsByLawTypesPartialCriteria (ostream & os,
                                     char * const types) ;  // 19.apr.2001
 } ;  // class AppealedLawsInputDiary

# if INSTANCE
#   if APPEAL
class TIlievaSummary
	: public TCheckupThread
 {
  protected :
    const begMYEndMYTypeCIU & rBMYEMYTCIU ;
    char formatFN [128] ;
    CodeValueArray codeValueArr ;
    const CDate begD ;
    const CDate endD ;
    void MakeFormatFN () ;
    void RegimeStr(CharPtr& dest, const char* const regime); // 14.jun.2002
		static void ResolutionPartialCriteria (ostream & os,
							 const CDate begDate,
							 const CDate endDate) ;
    void NotFinishedBegLawsDistribute (TLawsuit & law,
						   long int & vNotFinTotal,
						   long int & v2ndNotFin,
						   long int & vPrvNotFin,
																			 long int & v2ndNotFinBankruptcy,
																			 long int & vPrvNotFinBankruptcy
																			) const ;
		void ComeInLawsDistribute (TLawsuit & law,
															 long int & v2ndComeIn,
															 long int & vPrvComeIn,
															 long int & vSlowdownComeIn,
															 long int & vTotalComeInLaws,
													     long int & v2ndComeInBankruptcy,
															 long int & vPrvComeInBankruptcy,
													     long int & vPrvComeInPetition  //14.jun.2002
					) const ;
    void FinishedLawsDistribute (TLawsuit & law, TSession & sess,
					   long int & vFinTotal,
					   long int & v2ndDecTotal,
					   long int & v2ndDecUpTo3,
					   long int & v2ndDecOver3,
					   long int & v2ndCancelOpenSess,
					   long int & v2ndCancelClosedSess,
					   long int & vPrvDefin,
					   long int & vSlowdownDefin,
																 long int & v2ndDecidedOpenSessBankruptcy,
															 long int & v2ndDecidedClosedSessBankruptcy,
																 long int & v2ndCanceledBankruptcy,
																 long int & vPrvDecidedBankruptcy,
																 long int & vPrvCanceledBankruptcy,
							 long int & vPrvDecidedBankruptcyClosedS,   // 20.sep.2001
							 long int & vPrvCanceledBankruptcyClosedS,  // 20.sep.2001
														 long int & vPrvDefinClosedS,  // 20.sep.2001
														 long int & vPrv37DefinClosedS,  // 14.jun.2002
														 long int & vPrv37DefinOpenS   // 14.jun.2002
					  ) const ;
    void OpenSessionsDistribute (TSession & sess,
					   long int & vFixed,
					   long int & v2ndLeftTotal,
					   long int & v2ndLeftForNewEvidences,
					   long int & v2ndStopped,
					   long int & v2ndUnmoveable,
					   long int & v2ndAbolishProgr,
					   long int & v2ndLeftInvalidCall,
					   long int & v2ndLeft107,
					   long int & v2ndLeft65,
					   long int & v2ndLeft157,
					   long int & v2ndLeftOtherCauses,
					   long int & v2ndFixed,  // 19.feb.2001
				   long int & v2ndFixedBankruptcy,  // 19.feb.2001
					   long int & vPrvFixed,  // 20.sep.2001
				   long int & vPrvFixedBankruptcy   // 20.sep.2001
					  ) const ;
    void RequestsDistribute (TRequest & req,
				     long int & vReqWithoutLaw,
				     long int & vResolutions) const ;
    void OtherActsDistribute (TSession & sess,
					long int & v2ndStopped_OtherActs,
					long int & v2ndUnmoveable_OtherActs,
					long int & v2ndAbolishProgr_OtherActs,
					long int & vDec192_193,
					long int & vDefin205ClosedSess,
					long int & vDefin192p4ClosedSess,
					long int & vTotalSessOtherActs,
					long int & vDefin192p5ClosedSess,
					long int & vDefin37,
					long int & vDecis194p1OpenSess,
					long int & vDecreesClosedSess,
															long int & vDefin,
															long int & vStoppedUnmoveableCS,//17.jun.2002
					long int & vDefin192p5OpenSess  //24.jun.2002
														 ) const ;
    void CountNotFinishedBegLaws () ;
    void CountComeInLaws (const long int vReqWithoutLaw) ;
    void CountFinishedLaws () ;
    void CountOpenSessions (const long int v2ndStopped_OtherActs,
				    const long int v2ndUnmoveable_OtherActs,
				    const long int v2ndAbolishProgr_OtherActs) ;
    void CountOtherActs (const long int vResolutions,
				 long int & v2ndStopped_OtherActs,
				 long int & v2ndUnmoveable_OtherActs,
				 long int & v2ndAbolishProgr_OtherActs
				) ;
    void CountRequests (long int & vReqWithoutLaw,
				long int & vResolutions) ;
    virtual void Execute () ;
  public :
    TIlievaSummary (const begMYEndMYTypeCIU & bMYEMYTCIU);
    virtual ~TIlievaSummary () ;
    void PrintIlievaSummary () ;
    static bool OtherActsCriteria (char * const dest,
					     const size_t destSz,
					     const char * const types,
					     const CDate begDate,
					     const CDate endDate) ;
    static bool RequestsCriteria (char * const dest,
					    const size_t destSz,
					    const char * const types,
					    const CDate begDate,
					    const CDate endDate) ;
    static bool OpenSessionsCriteria (char * const dest,
						  const size_t destSz,
						  const char * const types,
						  const CDate begDate,
						  const CDate endDate) ;
};

class TIlievaJudgeCheckupThread : public TJudgeCheckupThread
 {
	private :
	  bool useInheritedGossips ;
  protected :
    virtual void makeBookFormatFN () ;
    virtual judgeCheckupData * newDataItem () const ;
    virtual
	judgeCheckupOutFile * newOutFile () const ;
    virtual judgeCheckupColumns *
		  newJudgeCheckupColumns (const char * const formatFN,
						  const judgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
					    const int serialNo = 0
						 ) const ;
    void IncrementValues_Judge (const char * const judgeUcn,
					  unsigned int dataCiph) ;
    void FinishedLawsDistribute (TLawsuit & law, TSession & sess) ;
		void OtherSessActsDistribute (const TLawsuit & law, TSession & sess) ;
		void RequestsDistribute (TRequest & req,
														 const CDate begDate,
														 const CDate endDate) ;
		void OpenSessionsDistribute (TSession & sess, const CDate endD) ;
		void CountFinishedLaws (const CDate begDate,
														const CDate endDate) ;
		void CountOtherActs (const CDate begDate, const CDate endDate) ;
		void CountRequests (const CDate begDate, const CDate endDate) ;

    // 20050804 ->
    void incrementInRegEventValues (TInRegEvent & rIRE,
						TInReg& rIR, TLawsuit& rLaw, TRequest& rReq);

		void CountInRegResolutions (const CDate begDate, const CDate endDate) ;
    // 20050804 <-

		void CountOpenSessions (const CDate begDate,
														const CDate endDate) ;
		bool PendingCondition (const TLawsuit & law, const TSession & sess) ;
		virtual void Execute() ;  // void MakeArray () ;
		void RegimeStr (CharPtr& dest, const char* const regime) ;
  public :
    TIlievaJudgeCheckupThread (const begMYEndMYTypeCIU & rBMYEMYTCIU) ;
    TIlievaJudgeCheckupThread (const collCompBegDEndDBegN & rCCBDEDBN,
   const unsigned int ciphIfNotTrueGudgeCheckup = OFFSET_TIME_BOOK_CIPHER
			     ) ;
    virtual ~TIlievaJudgeCheckupThread() ;
    virtual void prepare () ;
 } ;
#   else  // of APPEAL
class TradeLaw634Book
 {  // An old variant in VersionArchives\962\_appname.j
  protected :
    char bookFormatFN [128] ;
    TWindow * const pParent ;
    void MakeBookFormatFN () ;
    bool LawsuitApproach (const TLawsuit & law) const ;
    void MarkSurroundChains
	    (TGSArray <LinkedXKXKDatedContainerSK> & evArr) ;
    void CopyArray (TGSArray <LinkedXKXKDatedContainerSK> & src,
			  TGSArray <LinkedXKXKDatedContainerSK> & dest) ;
    void MoveArray (TGSArray <LinkedXKXKDatedContainerSK> & src,
			  TGSArray <LinkedXKXKDatedContainerSK> & dest) ;
    void RebuildEventsArray
          (TGSArray <LinkedXKXKDatedContainerSK> & evArray) ;
    void EventsHeapToChain
           (TGSArray <TL634AloneDatedContainer> & eventsHeap,
		TGSArray <LinkedXKXKDatedContainerSK> & chain,
		const TLawsuit & law) ;  // 01.oct.2001
    void HeadsAndExtensionsOfChains
	    (TGSArray <TL634AloneDatedContainer> & heads,
	     TGArray <TL634AloneDatedContainer> & extensions,
	     TLawsuit & law) ;  // 01.oct.2001
    bool SurroundChainMatch (const TDatedContainer & leader,
				     TDatedContainer & extItem) ;
    bool SessionChainMatch (const TDatedContainer & leader,
				    TDatedContainer & extItem) ;
    bool ChainMatch (const TDatedContainer & leader,
			   TDatedContainer & extItem) ;
    bool CompleteAChain (TGSArray <TL634AloneDatedContainer> & heads,
				 TGArray <TL634AloneDatedContainer> & extensions,
				 const int hInd,
				 TGSArray <TL634AloneDatedContainer> & chain) ;
  public :
    TradeLaw634Book  (TWindow * const parent) ;
    virtual ~TradeLaw634Book () ;
    void WriteOneLaw (const Trade634DlgData & rT634DD,
			    TradeLaw634OutFile & outF) ;
    void PrintTradeLaw634Book (Trade634DlgData rT634DD) ;
 } ;  // class TradeLaw634Book

class TKracholovJudgeCheckupThread : public TJudgeCheckupThread
 {
	private :
	  bool useInheritedGossips ;
	protected :
		const bool includingUnwrittens ;

    virtual void makeBookFormatFN () ;
    virtual judgeCheckupData * newDataItem () const ;
    virtual
	judgeCheckupOutFile * newOutFile () const ;
    virtual judgeCheckupColumns *
		  newJudgeCheckupColumns (const char * const formatFN,
						  const judgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
					    const int serialNo = 0
						 ) const ;
    void IncrementValues_Judge (const char * const judgeUcn,
					  unsigned int dataCiph) ;
    void FinishedLawsDistribute (const TLawsuit & law,
					   const TSession & sess) ;
    void SessionsDistribute (const TLawsuit & law,
				     const TSession & sess) ;
    void UnwrittensDistribute (const TLawsuit & law,
					 const TSession & sess,
					 const CDate endDate) ;
    void CountFinishedLaws (const CDate begDate,
				    const CDate endDate) ;
    void CountSessions (const CDate begDate, const CDate endDate) ;
    void CountUnwrittens (const CDate begDate, const CDate endDate) ;
		virtual void Execute();
		void RegimeStr (CharPtr& dest, const char* const regime) ;
		void PrepareNextStep(const int setEndingValTo);
	public :
    TKracholovJudgeCheckupThread(const begMYEndMYTypeCIU & rBMYEMYTCIU) ;
    virtual ~TKracholovJudgeCheckupThread() ;
    virtual void prepare () ;
 } ;

class TPdCitJudgeCheckupThread : public TJudgeCheckupThread
 {
  protected :
    int whichPart ;  // Boza data member, used to generate the format
			   // file's name.
    virtual void makeBookFormatFN () ;
    virtual judgeCheckupData * newDataItem () const ;
    virtual
	judgeCheckupOutFile * newOutFile () const ;
    virtual judgeCheckupColumns *
		  newJudgeCheckupColumns (const char * const formatFN,
						  const judgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
					    const int serialNo = 0
						 ) const ;
    void IncrementValues_Judge (const char * const judgeUcn,
					  unsigned int dataCiph) ;
    void FinishedLawsDistribute (const TLawsuit & law,
					   const TSession & sess) ;
    void ForExamineDistribute (const TLawsuit & law) ;
    void SessionsDistribute (const TLawsuit & law,
				     const TSession & sess) ;
    void UnwrittensDistribute (const TLawsuit & law,
					 const TSession & sess,
					 const CDate endDate) ;
    void CountFinishedLaws (const CDate begDate,
				    const CDate endDate) ;
    void CountForExamine (const CDate begD, const CDate endD) ;
    void CountSessions (const CDate begD, const CDate endD) ;
    void CountUnwrittens (const CDate begDate, const CDate endDate) ;
    void MakeArray (const bool includingUnwrittens) ;
  public :
    TPdCitJudgeCheckupThread (const begMYEndMYTypeCIU & rBMYEMYTCIU) ;
    virtual ~TPdCitJudgeCheckupThread() ;
    virtual void prepare () ;
    virtual void printJudgeCheckup () ;
 } ;

#if COURT_TYPE == COURT_AREAL
class TFirmJudgeCheckupThread : public TJudgeCheckupThread
 {
  protected :
    virtual void makeBookFormatFN () ;
    virtual judgeCheckupData * newDataItem () const ;
    virtual
	judgeCheckupOutFile * newOutFile () const ;
    virtual judgeCheckupColumns *
		  newJudgeCheckupColumns (const char * const formatFN,
						  const judgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
					    const int serialNo = 0
						 ) const ;
    void IncrementValues_Judge (const char * const judgeUcn,
					  unsigned int dataCiph) ;
    void LawsDistribute (const TLawsuit & law) ;
    void DecisionsDistribute (const TDecision & dec,
					const TSession & sess) ;
    void CountLaws (const CDate begDate, const CDate endDate) ;
    void CountDecisions (const CDate begD, const CDate endD) ;

    void CountSession( const CDate begD, const CDate endD);  // 20060322 search in T_Session

    long int CountThisCheckupContextFinisheds () ;  // 03.may.2001
    void MakeArray () ;
  public :
    TFirmJudgeCheckupThread(const begMYEndMYTypeCIU & rBMYEMYTCIU) ;
    virtual ~TFirmJudgeCheckupThread();
    virtual void prepare () ;
    static long int CountFinishedFirmLaws (const CDate begD,
							 const CDate endD,
							 const long int composition) ;
    static long int CountNonFinishedFirmLaws (const CDate rangeDate,
							 const long int composition) ;
 } ;
#endif  // AREAL
#   endif  // of APPEAL
# endif  // of INSTANCE

#if COURT_TYPE == COURT_AREAL
void print_zdravkova_distribution(TWindow* parent, begMYEndMYTypeCIU& rBMYEMYTCIU);
#endif  // AREAL

#if INSTANCE
#  if APPEAL
#  else  // of APPEAL
class InstancePlainJudgeCheckup : public plainJudgeCheckup
{  // 27.aug.2002
	public:
    InstancePlainJudgeCheckup(TWindow* parent,
	const begMYEndMYTypeCIU& rBMYEMYTCIU);
  protected:
    virtual bool MakeCriteria(char* const crit, const size_t critSz,
	const CDate& bP, const CDate& eP);
};  // class InstancePlainJudgeCheckup
#  endif  // of APPEAL
#endif   // of INSTANCE
