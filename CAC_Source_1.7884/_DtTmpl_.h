struct TClaimant ;
struct TDefendant ;

template <class T, class S> class TRCDKeyToSidePair
 {
  protected :
    TGArray <S> * pClaimant ;
    TGArray <S> * pDefendant ;
    unsigned pairsCount, clReadIndex, defReadIndex ;
    bool loadOK ;
    virtual bool citizenToStr (const char * ucn, const char ucnType,
                               char * destN, size_t destNSz) ;
    virtual bool firmToStr (const char * ucn, const char ucnType,
                            char * destN, size_t destNSz) ;
    virtual bool senderToStr (const char * ucn, const char ucnType,
                              char * destN, size_t destNSz) ;
    virtual bool codeToStr (const char * ucn, const char ucnType,
                    char * dest, size_t destSz) ;
  public :
    TRCDKeyToSidePair (T & lS,
                       const bool includingAnonymous,  // 13.mar.2001
                       const char * turnInvolvs = NULL,
                       const char * const defInvsForExclude = NULL,
               const bool includingOthersFromSide = true,  // 17.apr.2000
                       const CDate * const pDate = NULL  // 13.mar.2001
                      ) ;
    ~TRCDKeyToSidePair () ;
    virtual bool nextPair (TClaimant * pCl, TDefendant * pDef,
                           S * pClSide = NULL,
                           S * pDefSide = NULL,
                           const bool wantClAddress = false,
                           const bool wantDefAddress = false,
                           const bool wantClStreet = false,
                           const bool wantDefStreet = false) ;
      // It is allowed to pass pCl == NULL and/or pDef == NULL
    virtual void nextPair (S * pClSide, S * pDefSide,
                           bool & clReturned, bool & defReturned) ;
    bool statusIsOK () ;
    unsigned getPairsCount () ;
    unsigned getDefendantsCount () ;
    unsigned getClaimantsCount () ;
    void readIndexesToZero () ;
 } ;  // TRCDKeyToSidePair
 

template <class T> class nomHeap
 {
  public :
    T nom ;
    unsigned long int heap ;
    nomHeap () ;
    void inc (const T & n) ;
    bool operator==(const nomHeap &other) const;  // required
    bool operator<=(const nomHeap &that) const { return operator<(that) || operator==(that); }
private:
    bool operator<(const nomHeap &other) const ;  // required
 } ;  // class nomHeap

template <class T> class heapArray : public TISArray<T>
 {
  public :
    heapArray () { }
    void putHeapItem (T * pT, const bool putIfNotPresent = true,
                      const bool heap = true) ;
    void putItem (T * pT) ;
    void heapItem (T * pT) ;
    void incItem (T * pT) ;
  // (const T * pT) is not used because of
  // TGroup :: operator << (TGroup &)
 } ;  // class heapArray

template <class T> class GroupPtr
 {
  protected :
    T * pT ;
  public :
    GroupPtr () ;
    ~GroupPtr () ;
    GroupPtr & operator = (GroupPtr & source) ;
    operator T * () ;
    T * Ptr () ;
    T & Ref () ;
 } ;  // GroupPtr

typedef GroupPtr <TSession> SessPtr ;
typedef GroupPtr <TDecision> DecPtr ;
typedef GroupPtr <TJudgement> JudgPtr ;
typedef GroupPtr <TSurroundment> SurroundmentPtr ;
typedef GroupPtr <TRequestResolution> RequestResolutionPtr ;
typedef GroupPtr <TInRegEvent> InRegEventPtr ;
typedef GroupPtr <TProve> ProvePtr ;
typedef GroupPtr <TLawsuit> LawPtr ;
