class outText;

class FinalizedSessions
 {
  protected :
    const CDate begD ;
    const CDate endD ;
    const size_t crtLawSz ;
    char * const crtLaw ;
    TCountRecsQuery * pQLaw ;  // TQuery * pQLaw ;  // 29.may.2002
    bool badIntegrity ;
    bool CheckLaw (TLawsuit & law, TSession & sess, TQuery & qS,
			 outText * pOfs) ;
  public :
    static bool FinishedLawsCriteria (char * const dest,
						  const size_t destSz,
						  const char * const types,
						  const CDate begDate,
						  const CDate endDate,
						  const char * const kinds = NULL) ;
    static bool FinalizedSessionsCriteria (char * const dest,
							 const size_t destSz,
							 const TLawsuit & law
							) ;
    FinalizedSessions (const char * const types,
			     const CDate begDate,
			     const CDate endDate,
			     const char * const kinds = NULL) ;
    ~FinalizedSessions () ;
    bool NextSession (TLawsuit & law, TSession & sess,
			    outText * pOfs = NULL) ;
	// returns true if an correctly finalized lawsuit is extracted
	// if law.key is empty, this means that there are no more
	// finished laws.
    bool StillMayContinue () const ;  // ask if badIntegrity detected
    bool BadIntegrity () const ;
    unsigned FinishedLawsNumber () const ;  // 29.may.2002
 } ;  // class FinalizedSessions  // 07.dec.2000

class SessionEndDocs
 {
  protected :
    const size_t crtSz ;
    char * crt ;
    TCountRecsQuery * pQ ;  // TQuery * pQ ;  // 01.aug.2001
    unsigned int count ;
    const TRCDKey sessKey ;  // 05.sep.2001 - begin
    const CDate sessDate ;
    const char sessKind ;  // 05.sep.2001 - end
  public :
    SessionEndDocs (const TRCDKey key, const CDate date,
                    const char sessionKind) ;
    ~SessionEndDocs () ;
    static bool EndDocsCriteria (const TRCDKey key, const CDate date,
                                 const char sessionKind,
                                 char * const criteria,
                                 const size_t criteriaSz) ;
    bool NextDoc (TDecision & dec) ;
    unsigned int DocsFound () const ;
    void Rewind () ;  // 01.aug.2001
    bool NextKey (TDecision & dec) ;  // 01.aug.2001
    bool ChooseOne (TWindow * pParent, TDecision & dec,
                    const char * forWhat) ;  // 05.sep.2001
      // Attention! Rewinds the set.
    bool GeneralCancelingResult () ;  // 10.sep.2001  
      // Attention! Rewinds the set.
 } ;  // class SessionEndDocs

class BriefSessionsToCheckGroup : public AloneDatedContainer_xK
 {  // 17.apr.2001
  public :
    BriefSessionsToCheckGroup (char * name = NULL) ;
    void Clear () ;
    // ----------------------------------------------
    // I'm interested in will be these faster
    // than "operator << (TGrooup..."
    void MoveDataFromBSCG (const BriefSessionsToCheckGroup & bSCG) ;
    void MoveDataFromSession (const TSession & sess) ;
    void MoveDataToSession (TSession & sess) const ;
    // ----------------------------------------------

    CDate returned ;
    char result ;
    long int composition ;
    char judge [SIZE_OF_UCN] ;
    char president [SIZE_OF_UCN] ;
    char judgeX [SIZE_OF_UCN] ;
    char reJudgeX [SIZE_OF_UCN] ;
 } ;  // class BriefSessionsToCheckGroup

class SessionsToCheck
 {
  protected :
    const size_t critSz ;
    char * const crit ;
    TCountRecsQuery * pQ ;  // 28.feb.2001 (from TQuery)
    unsigned long int count ;  // 28.feb.2001
  public :
    SessionsToCheck (const char * const types, const CDate begDate,
                     const CDate endDate,
                     const long int composition = 0,
                     const char * const lawKinds = NULL,
                     const char * const sessKinds = NULL,
                     const bool excludeResFixed = true
                    ) ;
    ~SessionsToCheck () ;
    static bool MakeSessionsToCheckCriteria (char * const dest,
                                             const size_t destSz,
                                             const char * const types,
                                             const CDate begDate,
                                             const CDate endDate,
                             const long int composition = 0,
                             const char * const lawKinds = NULL,
                             const char * const sessKinds = NULL,
                             const bool excludeResFixed = true
                                            ) ;
    bool NextSession (TSession & sess) ;
    bool NextSession (BriefSessionsToCheckGroup & bSCG) ;  // 17.apr.2001
    unsigned long int SessionsFound () const ;  // 28.feb.2001
 } ;  // class SessionsToCheck

class LawsuitAllJudgements
 {
  protected :
    const size_t critSz ;
    char * const crit ;
    TCountRecsQuery * pQ ;  // TQuery * pQ ;  // 01.aug.2001
    unsigned int count ;  // 01.aug.2001
  public :
    LawsuitAllJudgements (const TRCDKey key) ;
    ~LawsuitAllJudgements () ;
    static bool MakeLawsuitAllJudgementsCriteria (char * const dest,
                                                  const size_t destSz,
                                                  const TRCDKey key) ;
    bool NextJudgement (TJudgement & lJ) ;
    unsigned int JudgementsFound () const ;  // 01.aug.2001
    void Rewind () ;  // 01.aug.2001
    bool NextKey (TJudgement & lJ) ;  // 01.aug.2001
 } ;  // class LawsuitAllJudgements

class EndDocJudgements
 {
  protected :
    TGArray <TSideWind> keys ;
    int currIndex ;
  public :
    EndDocJudgements (const TRCDKey key, const CDate date,
			    const char decisionKind) ;
    ~EndDocJudgements () ;
    bool NextJudgement (TJudgement & judg) ;
    bool NextKey (TJudgement & judg) ;
    unsigned int JudgementsFound () const ;
    void Rewind () ;
 } ;  // class EndDocJudgements

class TRCDKeyAllDatedContainers
 {  // Basic class. Do not use it directly.
  protected :
    const size_t critSz ;
    char * const crit ;
    TCountRecsQuery * pQ ;
    unsigned int count ;
    const TRCDKey key ;
    bool initCalled ;
    bool MakeSimpleCriteria (const char * const tableName) ;
    virtual bool MakeCriteria () ;
  public :
    TRCDKeyAllDatedContainers (const TRCDKey aKey) ;
    ~TRCDKeyAllDatedContainers () ;
    bool Init () ;
    bool NextWholeRecord (TDatedContainer & dC) ;
    unsigned int DatedContainersFound () ;
    void Rewind () ;
    bool NextKey (TDatedContainer & dC) ;
 } ;  // class TRCDKeyAllDatedContainers

class RequestAllResolutions : public  TRCDKeyAllDatedContainers
 {
  protected :
    virtual bool MakeCriteria () ;
  public :
    RequestAllResolutions (const TRCDKey aKey) ;
 } ;  // class LawsuitAllJudgements

// 11.jan.2002 - begin 
class LawsuitAllSessions : public TRCDKeyAllDatedContainers
 {
  protected :
    virtual bool MakeCriteria () ;
  public :
    LawsuitAllSessions (const TRCDKey aKey) ;
 } ;  // class LawsuitAllSessions

class LawsuitAllDecisions : public TRCDKeyAllDatedContainers
 {
  protected :
    virtual bool MakeCriteria () ;
  public :
    LawsuitAllDecisions (const TRCDKey aKey) ;
 } ;  // class LawsuitAllDecisions

class BankruptcyLawAllDecisions : public LawsuitAllDecisions
 {
  protected :
    virtual bool MakeCriteria () ;
  public :
    BankruptcyLawAllDecisions (const TRCDKey aKey) ;
 } ;  // class BankruptcyLawAllDecisions

class LawsuitAllInRegs : public TRCDKeyAllDatedContainers
 {
  protected :
    virtual bool MakeCriteria () ;
  public :
    LawsuitAllInRegs (const TRCDKey aKey) ;
 } ;  // class LawsuitAllInRegs

class InRegAllEvents : public TRCDKeyAllDatedContainers
 {
  protected :
    virtual bool MakeCriteria () ;
  public :
    InRegAllEvents (const TRCDKey aKey) ;
 } ;  // class InRegAllEvents
// 11.jan.2002 - end
