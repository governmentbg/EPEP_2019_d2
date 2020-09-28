// ", class S" added to the TRCDKeyToSidePair class at 13.mar.2001
// (insterad of TSide)
template <class T, class S>
TRCDKeyToSidePair <T, S> :: TRCDKeyToSidePair (T & lS,
			     const bool includingAnonymous,  // 13.mar.2001
						    const char * turnInvolvs,
					    const char * const defInvsForExclude,
					    const bool includingOthersFromSide,
					const CDate * const pDate  // 13.mar.2001
							    ) :
			     pClaimant (NULL), pDefendant (NULL),
			     pairsCount (0), loadOK (true),
			     clReadIndex (0), defReadIndex (0)
 {
  char criteria [256] ;
//  size_t cLen ;  // 13.mar.2001

  pClaimant =
    new TGArray <S> ;
  pDefendant =
    new TGArray <S> ;
   {
    S aSide ;
    ostrstream oss (criteria, sizeof (criteria)) ;

    oss << "SELECT * FROM " << aSide.Name << " WHERE " ;
    oss << "F_DATE = "
	  << ((pDate != NULL && pDate -> Empty () == false) ?
	    (* pDate) : CDate (31, 12, 1900)).Value() << " AND " ;
    oss << "F_KIND = '" << (lS.kind ? lS.kind : STUB_CHAR) << "' AND " ;
    oss << "F_TYPE = '" << (lS.key.type ? lS.key.type : STUB_CHAR) << "' AND "
	     "F_YEAR = "  << lS.key.year << " AND "
	     "F_NO = " << lS.key.no
	  << " ;" << ends ;
    criteria [sizeof (criteria) - 1] = '\0'  ;
   }
  if (criteria_finished_ok (criteria, typeid (* this).name (),
    typeid (* this).name ()))
   {  // -- -1 --
    try
     {  // -- 0 --
	TCountRecsQuery q (criteria) ;
	const unsigned itCnt = q.getRecsCount () ;

	 {  // -- 1 --
	  unsigned i ;

	  for (i = 0 ; i < itCnt ; i ++)
	   {
	    S * pS = new S ;

	    if (! q.Read ())
	     {
		loadOK = false ;
		delete pS ;
		return ;
	     }
	    (* pS) << q ;
  # if INSTANCE
  #   if APPEAL
	    if (ucn2uxn (pS -> ucnType) == UXN_UNKNOWN)
	     {
		delete pS ;
		continue ;
	     }
  #   endif  // APPEAL
  # endif  // INSTANCE
	  if (includingAnonymous == false)  // 13.mar.2001 - begin
		if (ucn2uxn (pS -> ucnType) == UXN_ANONYMOUS)
		 {
		  delete pS ;
		  continue ;
		 }  // 13.mar.2001 - end
	    if (lS.Suitor (pS -> involvement)
  // ------------- complement - begin ---------------------------
		  &&
		  (! (turnInvolvs && strchr (turnInvolvs, pS -> involvement)))
  // -------------- complement - end ----------------------------
		  )
		pClaimant -> Add (pS) ;
	    else
	    if (lS.Defendant (pS -> involvement)
  // ------------- complement - begin ---------------------------
		||
		(turnInvolvs && strchr (turnInvolvs, pS -> involvement))
  // -------------- complement - end ----------------------------
		   )
		  if (defInvsForExclude &&
			strchr (defInvsForExclude, pS -> involvement))
		    delete pS ;
		  else
		    pDefendant -> Add (pS) ;
		else
		  if (includingOthersFromSide)
		if (lS.Suitor(pS -> involvement))
		  pClaimant -> Add (pS) ;
		else
		  if (lS.Defendant (pS -> involvement))
		    pDefendant -> Add (pS) ;
		  else
		    delete pS ;
		else
		delete pS ;
         }
       }  // -- 1 --
	pairsCount = pClaimant -> Count () ;
       {
	  unsigned c1 = pDefendant -> Count () ;

        if (c1 > pairsCount)
          pairsCount = c1 ;
       }
     }  // -- 0 --
    CATCH_ANYTHING
   }  // -- -1 --
 }  // TRCDKeyToSidePair :: TRCDKeyToSidePair

template <class T, class S>
TRCDKeyToSidePair <T, S> :: ~TRCDKeyToSidePair ()
 {
  delete pClaimant ;
  delete pDefendant ;
 }  // TRCDKeyToSidePair :: ~TRCDKeyToSidePair

template <class T, class S>
bool TRCDKeyToSidePair <T, S> :: statusIsOK ()
 {
  return (pClaimant && pDefendant && loadOK) ;
 }  // TRCDKeyToSidePair :: statusIsOK

template <class T, class S>
unsigned TRCDKeyToSidePair <T, S> :: getPairsCount ()
 {
  return pairsCount ;
 }  // TRCDKeyToSidePair :: getPairsCount

template <class T, class S>
bool TRCDKeyToSidePair <T, S> :: citizenToStr (const char * ucn,
                                               const char ucnType,
                                            char * destN, size_t destNSz)
 {
  bool res = false ;

  if (statusIsOK () && destN && destNSz > 1)
    if (! (res = citizenUCNAndUCNTypeToName (ucn, ucnType,
                                             destN, destNSz)))
      destN [0] = '\0' ;
  return res ;
 }  // TRCDKeyToSidePair :: citizenToStr

template <class T, class S>
bool TRCDKeyToSidePair <T, S> :: firmToStr (const char * ucn,
                                            const char ucnType,
                                            char * destN, size_t destNSz)
 {
  bool res = false ;

  if (statusIsOK () && destN && destNSz > 1)
    if (! (res = firmUCNAndUCNTypeToName (ucn, ucnType,
                                          destN, destNSz)))
      destN [0] = '\0' ;
  return res ;
 }  // TRCDKeyToSidePair :: firmToStr

template <class T, class S>
bool TRCDKeyToSidePair <T, S> :: senderToStr (const char * ucn,
                                              const char ucnType,
                                           char * destN, size_t destNSz)
 {
  bool res = false ;

  if (statusIsOK () && destN && destNSz > 1)
    if (! (res = senderUCNAndUCNTypeToName (ucn, ucnType,
                                            destN, destNSz)))
      destN [0] = '\0' ;
  return res ;
 }  // TRCDKeyToSidePair :: senderToStr

template <class T, class S>
bool TRCDKeyToSidePair <T, S> :: codeToStr (const char * ucn,
                                            const char ucnType,
                                            char * destN, size_t destNSz)
 {
  bool res = false ;

  if (! statusIsOK ())
    return false ;
  switch (ucn2uxn (ucnType))
   {
    case UXN_CITIZEN :
      res = citizenToStr (ucn, ucnType, destN, destNSz) ;
      break;
    case UXN_FIRM :
      res = firmToStr (ucn, ucnType, destN, destNSz) ;
      break;
    case UXN_SENDER :
    case UXN_UNKNOWN :  // 22.may.2000
    case UXN_ANONYMOUS :  // 22.may.2000
      res = senderToStr (ucn, ucnType, destN, destNSz) ;
      break;
    default :
      break ;
   }
  return res ;
 }  // TRCDKeyToSidePair :: codeToStr

template <class T, class S> bool TRCDKeyToSidePair <T, S> ::
  nextPair (TClaimant * pCl, TDefendant * pDef,
            S * pClSide, S * pDefSide,
            const bool wantClAddress, const bool wantDefAddress,
            const bool wantClStreet, const bool wantDefStreet)
 {
  bool cRes, cRes1, dRes, dRes1 ;

  cRes1 = cRes = dRes = dRes1 = false ;
  if (pCl)
    pCl -> clearData () ;
  if (pDef)
    pDef -> clearData () ;
  if (! statusIsOK ())
    return false ;
  if (clReadIndex < pClaimant -> Count ())
   {  // -- 0 --
    S * pSide = (* pClaimant) [clReadIndex ++] ;

    if (pClSide)
      (* pClSide) << (* pSide) ;
    cRes = (pCl != NULL) ;
    cRes1 = (pCl && codeToStr (pSide -> ucn, pSide -> ucnType,
                               pCl -> cName, sizeof (pCl -> cName))) ;
    if (cRes1 && wantClAddress
        && ucn2uxn (pSide -> ucnType) != UXN_UNKNOWN  // 22.may.2000
        && ucn2uxn (pSide -> ucnType) != UXN_ANONYMOUS  // 22.may.2000
       )
     {  // -- 0a --
      bool frgnAddrFound = false ;

       {  // -- 6 --
        TForeignAddress fAddr ;

        if (ucnAndUcnTypeToFrgnAddr (pSide -> ucn, pSide -> ucnType,
                                     & fAddr))
         {  // -- 7 --
          TCountry cntry ;

          frgnAddrFound = true ;
          cntry << fAddr ;
          if (cntry.Try ())
           {  // -- 8 --
            ostrstream (pCl -> cSettlement, sizeof (pCl -> cSettlement))
              << cntry.name << ends ;
            (pCl -> cSettlement) [sizeof (pCl -> cSettlement) - 1] =
              '\0' ;
            if (wantClStreet)
             {
              ostrstream (pCl -> cAddress, sizeof (pCl -> cAddress))
                << fAddr.address << ends ;
              (pCl -> cAddress) [sizeof (pCl -> cAddress) - 1] = '\0' ;
             }
           }  // -- 8 --
          else
            pCl -> cSettlement [0] = '\0' ;
	   }  // -- 7 --
       }  // -- 6 --
      if (! frgnAddrFound)
       {  // -- 1 --
        TAddress addr ;

        if (ucnAndUcnTypeToAddress (pSide -> ucn, pSide -> ucnType,
                                    & addr))
         {  // -- 2 --
          TUCLP settlement ;

          settlement.uclp = addr.uclp ;
          if (settlement.Try ())
           {
            ostrstream (pCl -> cSettlement, sizeof (pCl -> cSettlement))
              << settlement.name << ends ;
            (pCl -> cSettlement) [sizeof (pCl -> cSettlement) - 1] = '\0' ;
           }
          else
            (pCl -> cSettlement) [0] = '\0' ;
          if (wantClStreet)
            addressWithStr_SB_ToString (addr, pCl -> cAddress,
                                        sizeof (pCl -> cAddress)) ;
         }  // -- 2 --
       }  // -- 1 --
     }  // -- 0a --
   }  // -- 0 --
  else
    cRes = false ;
  if (defReadIndex < pDefendant -> Count ())
   {  // -- 3 --
    S * pSide = (* pDefendant) [defReadIndex ++] ;

    if (pDefSide)
      (* pDefSide) << (* pSide) ;
    dRes = (pDef != NULL) ;
    dRes1 = (pDef && codeToStr (pSide -> ucn, pSide -> ucnType,
                                pDef -> dName, sizeof (pDef -> dName))) ;
    if (dRes1 && wantDefAddress
        && ucn2uxn (pSide -> ucnType) != UXN_UNKNOWN  // 22.may.2000
        && ucn2uxn (pSide -> ucnType) != UXN_ANONYMOUS  // 22.may.2000
       )
     {  // -- 3a --
      bool frgnAddrFound = false ;

       {  // -- 9 --
        TForeignAddress fAddr ;

        if (ucnAndUcnTypeToFrgnAddr (pSide -> ucn, pSide -> ucnType,
                                     & fAddr))
         {  // -- 10 --
          TCountry cntry ;

          frgnAddrFound = true ;
          cntry << fAddr ;
          if (cntry.Try ())
	     {  // -- 11 --
            ostrstream (pDef -> dSettlement,
                        sizeof (pDef -> dSettlement))
              << cntry.name << ends ;
            (pDef -> dSettlement) [sizeof (pDef -> dSettlement) - 1] =
              '\0' ;
            if (wantDefStreet)
             {
              ostrstream (pDef -> dAddress, sizeof (pDef -> dAddress))
                << fAddr.address << ends ;
              (pDef -> dAddress) [sizeof (pDef -> dAddress) - 1] = '\0' ;
             }
           }  // -- 11 --
          else
            pCl -> cSettlement [0] = '\0' ;
         }  // -- 10 --
       }  // -- 9 --
      if (! frgnAddrFound)
       {  // -- 4 --
        TAddress addr ;

        if (ucnAndUcnTypeToAddress (pSide -> ucn, pSide -> ucnType,
                                    & addr))
         {  // -- 5 --
          TUCLP settlement ;

          settlement.uclp = addr.uclp ;
          if (settlement.Try ())
	     {
            ostrstream (pDef -> dSettlement, sizeof (pDef -> dSettlement))
              << settlement.name << ends ;
            (pDef -> dSettlement) [sizeof (pDef -> dSettlement) - 1] =
                                                                 '\0' ;
           }
          else
            (pDef -> dSettlement) [0] = '\0' ;
          if (wantDefStreet)
            addressWithStr_SB_ToString (addr, pDef -> dAddress,
                                        sizeof (pDef -> dAddress)) ;
         }  // -- 5 --
       }  // -- 4 --
     }  // -- 3a --
   }  // -- 3 --
  else
    dRes = false ;
  return (cRes == cRes1 && dRes == dRes1 && (cRes || dRes)) ;
 }  // TRCDKeyToSidePair :: nextPair

template <class T, class S>
void TRCDKeyToSidePair <T, S> :: nextPair (S * pClSide, S * pDefSide,
                                           bool & clReturned,
                                           bool & defReturned)
 {
  clReturned = defReturned = false ;
  if (! statusIsOK ())
    return ;
  if (pClSide && clReadIndex < pClaimant -> Count ())
   {  // -- 0 --
    (* pClSide) << (* pClaimant) [clReadIndex ++] ;
    clReturned = true ;
   }
  if (pDefSide && defReadIndex < pDefendant -> Count ())
   {  // -- 3 --
    (* pDefSide) << (* pDefendant) [defReadIndex ++] ;
    defReturned = true ;
   }
 }  // TRCDKeyToSidePair :: nextPair

template <class T, class S>
unsigned TRCDKeyToSidePair <T, S> :: getDefendantsCount ()
 {
  return statusIsOK () ? pDefendant -> Count () : 0 ;
 }  // TRCDKeyToSidePair :: getDefendantsCount

template <class T, class S>
unsigned TRCDKeyToSidePair <T, S> :: getClaimantsCount ()
 {
  return statusIsOK () ? pClaimant -> Count () : 0 ;
 }  // TRCDKeyToSidePair :: getClaimantsCount

template <class T, class S>
void TRCDKeyToSidePair <T, S> :: readIndexesToZero ()
 {
  clReadIndex = defReadIndex = 0 ;
 }  // TRCDKeyToSidePair :: readIndexesToZero

template <class T> nomHeap <T> :: nomHeap ()
 {
  heap = 0 ;
 }  // nomHeap :: nomHeap

template <class T> void nomHeap <T> :: inc (const T & n)
 {
  if (nom == n)
    heap ++ ;
 }  // nomHeap :: inc

template <class T> bool nomHeap <T> :: operator==(const nomHeap &other) const
 {
  return nom == other.nom ;
 }  // template <class T>
    //  int nomHeap <T> :: operator == (const nomHeap & other) const

template <class T> bool nomHeap <T> :: operator<(const nomHeap & other) const
 {
  return nom < other.nom ;
 }  // template <class T>
    //  int nomHeap <T> :: operator < (const nomHeap & other) const

template <class T>
  void heapArray <T> :: putHeapItem (T * pT,
                                     const bool putIfNotPresent,
                                     const bool heap)
 {
  TGroup *const pG = dynamic_cast <TGroup *> (& (pT -> nom)) ;

  if (! (pT && pG))
    return ;
    
  const int index = Find (pT) ;

  if (index == INT_MAX)
   {  // not found
    if (putIfNotPresent)
     {
      T * tPtr = new T ;
      TGroup *const pGroup = dynamic_cast <TGroup *> (& (tPtr -> nom)) ;

      if (pGroup)
       {
        (* pGroup) << (* pG) ;
        if (heap)
          tPtr -> heap = pT -> heap ;
        Add (tPtr) ;
       }
     }
   }
  else
    if (heap)
      ((* this) [index]) -> heap += pT -> heap ; 
 }  // heapArray <T> putHeapItem

template <class T> void heapArray <T> :: putItem (T * pT)
 {
  if (pT)
    putHeapItem (pT, true, false) ;
 }  // heapArray :: putItem

template <class T> void heapArray <T> :: heapItem (T * pT)
 {
  if (pT)
    putHeapItem (pT, false) ;
 }  // heapArray :: heapItem

template <class T> void heapArray <T> :: incItem (T * pT)
 {
  TGroup *const pG = dynamic_cast <TGroup *> (& (pT -> nom)) ;

  if (pT && pG)
   {
    T * tTmp = new T ;
    TGroup *const pGroup = dynamic_cast <TGroup *> (& (tTmp -> nom)) ;

    if (pGroup)
		 {
      (* pGroup) << (* pG) ;
	tTmp -> heap = 1 ;
	heapItem (tTmp) ;
		 }
		delete tTmp ;
	 }
 }  // heapArray :: incItem

template <class T> GroupPtr <T> :: GroupPtr ()
 {
  pT = new T ;
 }  // GroupPtr :: GroupPtr

template <class T> GroupPtr <T> :: ~GroupPtr ()
 {
  delete pT ;
 }  // GroupPtr :: ~GroupPtr

template <class T>
GroupPtr & GroupPtr <T> :: operator = (GroupPtr & source)
 {
  (* pT) << source.Ref () ;
  return (* this) ;
 }  // GroupPtr :: operator =

template <class T> GroupPtr <T> :: operator T * ()
 {
  return pT ;
 }  // GroupPtr :: operator T *

template <class T> T * GroupPtr <T> :: Ptr ()
 {
  return pT ;
 }  // GroupPtr :: Ptr

template <class T> T & GroupPtr <T> :: Ref ()
 {
  return (* pT) ;
 }  // GroupPtr :: Ref
