#include "cac.h"  // 18.may.2001
//# include "all.h"

class TimeBookEndDocColumnsHolder
 {
  protected :
    column * const pDefendantsColumn ;
    column * const pDecisionsJudgementsColumn ;
    column * const pDateReturnedColumn ;
    columnArray * const pColumns ;
    void AddToColumn (column * const pColumn, const char * const txt) ;
    void AddEmptyRowToColumn (column * const pColumn) ;
  public :
    TimeBookEndDocColumnsHolder (column * defendantsColumn = NULL,
                                 column * decisionsJudgementsColumn = NULL,
                                 column * dateReturnedColumn = NULL) ;
    ~TimeBookEndDocColumnsHolder () ;
    void AddToDefsColumn (const char * const txt) ;
    void AddToDecisColumn (const char * const txt) ;
    void AddToDecisColumn (const mstr &m) { AddToDecisColumn(str(m)); }
    void AddToDateRetColumn (const char * const txt) ;
    void AdjustColumnsRows (const int emptyRows = 0) ;
    void AddEmptyRowToDecisColumn () ;
    void AddDecisionToDecisColumn (TDecision & dec,
                                   timeBookHeap * pTBH = NULL,
                                   bool addKind = true,
                                   bool addResult = false,
# if INSTANCE
                                   const char lawKind = '\0',
                                   const char sessKind = '\0',
# endif
					     const char sessResult = '\0',
                                   const char sessResultToCount = '\0'
                                  ) ;
    bool AddJudgemToDecisColumn (const TRCDKey & key,
                                 const CDate & date,
                                 const char ucnType,
                                 const char * const ucn,
                                 const char involvement,  // 20060413
                                 const char decisKind = '\0',
                                 accusedsDistribution * pAccDistr = NULL,
                                 bool * pNotMajor = NULL
                                ) ;
    void AddResultToDecisColumn (const char result) ;
    void AddDefNameToDefsColumn (const TDefendant & defendant,
                                 const bool wantDefSettl = false) ;
    void AddDateRetToDateRetColumn (const CDate dateRet) ;
    void AddAbolishingToDecisColumn (const CDate * pDateAbolish,
                   const TSession * pAbolishingSess = NULL  // 13.dec.2001
                                    ) ;
 } ;  // class TimeBookEndDocColumnsHolder

TimeBookEndDocColumnsHolder :: TimeBookEndDocColumnsHolder(
					 column * defendantsColumn,
					 column * decisionsJudgementsColumn,
					 column * dateReturnedColumn) :
    pDefendantsColumn (defendantsColumn),
    pDecisionsJudgementsColumn (decisionsJudgementsColumn),
    pDateReturnedColumn (dateReturnedColumn),
    pColumns (new columnArray)
 {
  pColumns -> Add (pDefendantsColumn) ;
  pColumns -> Add (pDecisionsJudgementsColumn) ;
  pColumns -> Add (pDateReturnedColumn) ;
 }  // TimeBookEndDocColumnsHolder :: TimeBookEndDocColumnsHolder

TimeBookEndDocColumnsHolder :: ~TimeBookEndDocColumnsHolder ()
 {
  if (pColumns)
    delete pColumns ;
 }  // TimeBookEndDocColumnsHolder :: ~TimeBookEndDocColumnsHolder

void TimeBookEndDocColumnsHolder :: AddToColumn (column * const pColumn,
                                                 const char * const txt)
 {
  if (pColumn && txt)
    pColumn -> addText (txt) ;
 } // TimeBookEndDocColumnsHolder :: AddToColumn

void
TimeBookEndDocColumnsHolder :: AddToDefsColumn (const char * const txt)
 {
  AddToColumn (pDefendantsColumn, txt) ;
 }  // TimeBookEndDocColumnsHolder :: AddToDefsColumn

void
  TimeBookEndDocColumnsHolder :: AddToDecisColumn (const char * const txt)
 {
  AddToColumn (pDecisionsJudgementsColumn, txt) ;
 }  // TimeBookEndDocColumnsHolder :: AddToDecisColumn

void
TimeBookEndDocColumnsHolder :: AddToDateRetColumn (const char * const txt)
 {
  AddToColumn (pDateReturnedColumn, txt) ;
 }  // TimeBookEndDocColumnsHolder :: AddToDateRetColumn

void
TimeBookEndDocColumnsHolder :: AdjustColumnsRows (const int emptyRows)
 {
  adjust_column_rows (* pColumns, emptyRows) ;
 }  // TimeBookEndDocColumnsHolder :: AdjustColumnsRows

void
TimeBookEndDocColumnsHolder :: AddEmptyRowToColumn (column * const pColumn)
 {
  if (pColumn)
    pColumn -> addEmptyRow () ;
 }  // TimeBookEndDocColumnsHolder :: AddEmptyRowToColumn

void TimeBookEndDocColumnsHolder :: AddEmptyRowToDecisColumn ()
 {
  AddEmptyRowToColumn (pDecisionsJudgementsColumn) ;
 }  // TimeBookEndDocColumnsHolder :: AddEmptyRowToDecisColumn

void TimeBookEndDocColumnsHolder ::
       AddDecisionToDecisColumn (TDecision & dec, timeBookHeap * pTBH,
                                 bool addKind, bool addResult,
# if INSTANCE
                                 const char lawKind, const char sessKind,
# endif
                                 const char sessResult,
                                 const char sessResultToCount
                                )
 {
  if (pDecisionsJudgementsColumn)

  // 20060719  if (! strchr(TB_SUPPRESS_SESS_RESULTS, sessResult) )  // 20060713 added  - 13117

        add_decision_to_column (dec, pDecisionsJudgementsColumn, pTBH,
                            addKind, addResult,
# if INSTANCE
                            lawKind, sessKind,
# endif
                            sessResult, sessResultToCount
                           ) ;
 }  // TimeBookEndDocColumnsHolder :: AddDecisionToDecisColumn

bool TimeBookEndDocColumnsHolder ::
       AddJudgemToDecisColumn (const TRCDKey & key, const CDate & date,
                               const char ucnType, const char * const ucn,
                               const char involvement,  // 20060413
                               const char decisKind,
                               accusedsDistribution * pAccDistr,
                               bool * pNotMajor)
 {
  column tmp (70) ;
  column * const pColumn = pDecisionsJudgementsColumn ?
                              pDecisionsJudgementsColumn : & tmp ;

  return addLocalJudgemToColumn (key, date, ucnType, ucn,
                                 involvement,  // 20060413
                                 pColumn,
                                 decisKind, pAccDistr, pNotMajor) ;
 }  // TimeBookEndDocColumnsHolder :: AddJudgemToDecisColumn

void TimeBookEndDocColumnsHolder ::
       AddDefNameToDefsColumn (const TDefendant & defendant,
                               const bool wantDefSettl)
 {
  if (pDefendantsColumn)
    if (wantDefSettl)
      addDefNameAndSettlToColumn (defendant,
                                  pDefendantsColumn) ;
    else
      pDefendantsColumn -> addText (defendant.dName) ;
 }  // TimeBookEndDocColumnsHolder :: AddDefNameToDefsColumn

void TimeBookEndDocColumnsHolder ::
       AddDateRetToDateRetColumn (const CDate dateRet)
 {
  if (pDateReturnedColumn)
    add_date_to_column (pDateReturnedColumn, dateRet) ;
 }  // TimeBookEndDocColumnsHolder :: AddDateRetToDateRetColumn

void TimeBookEndDocColumnsHolder ::
       AddResultToDecisColumn (const char result)
 {
  if (pDecisionsJudgementsColumn)
    addResultToColumn (result, pDecisionsJudgementsColumn) ;
 }  // TimeBookEndDocColumnsHolder :: AddResultToDecisColumn

void TimeBookEndDocColumnsHolder ::
       AddAbolishingToDecisColumn (const CDate * pDateAbolish,
                                   const TSession * pAbolishingSess)
 {
  if (pDateAbolish && pDecisionsJudgementsColumn)
    add_abolishing_to_column (pDecisionsJudgementsColumn,
                              * pDateAbolish,  pAbolishingSess) ;
 }  // TimeBookEndDocColumnsHolder :: AddAbolishingToDecisColumn

class TimeBookEndDocColumns : public TimeBookEndDocColumnsHolder
 {
  protected :
    TSession & rSess ;
    TLawsuit & rLawsuit ;
    timeBookHeap * const pTBH ;
    accusedsDistribution & rAccDistr ;
    long int & rNotMajorD ;
    const bool catchOtherEndingResults ;
    const bool wantDefendantsSettl ;
    const CDate * const pDateAbolish ;
    SessRCDKeyToSidePair * const pPairs ;
    SessionEndDocs * const pEndDocs ;
    TISArray <DoubleNos> printingList ;
    const TSession * const pAbolishingSession ;

    void FillPrintingList () ;
    void WriteDefendants (const int no0) ;
    void ProcessSession () ;
    void ProcessOneEndDoc (TDecision & dec, const int no0) ;
    void ProcessEndDocs () ;
    void ProcessEndDocJudgements (const TDecision & dec, const int no0) ;
    void FinalizingActions () ;
    void SessionStatistics () ;
  public :
    TimeBookEndDocColumns (TSession & sess,
                           TLawsuit & lawsuit,
                           timeBookHeap * tBHPtr,
                           accusedsDistribution & accDistr,
                           long int & notMajorD,
                           column * defendantsColumn = NULL,
                           column * decisionsJudgementsColumn = NULL,
                           column * dateReturnedColumn = NULL,
                           bool catchOthEndingResults = false,
                           const bool wantDefSettl = false,
                           const CDate * dateAbolishPtr = NULL,
                   const TSession * pAbolishingSess = NULL  // 13.dec.2001
                          ) ;
    ~TimeBookEndDocColumns () ;
    void FillColumns () ;
 } ;  // class TimeBookEndDocColumns

TimeBookEndDocColumns :: TimeBookEndDocColumns (
				 TSession & sess,
				 TLawsuit & lawsuit,
				 timeBookHeap * tBHPtr,
                         accusedsDistribution & accDistr,
                         long int & notMajorD,
                         column * defendantsColumn,
                         column * decisionsJudgementsColumn,
                         column * dateReturnedColumn,
                         bool catchOthEndingResults,
                         const bool wantDefSettl,
                         const CDate * dateAbolishPtr,
                         const TSession * pAbolishingSess
                         ) :
// CHECK_THIS
    TimeBookEndDocColumnsHolder (defendantsColumn,
                                 decisionsJudgementsColumn,
                                 dateReturnedColumn),
    rSess (sess), rLawsuit (lawsuit), pTBH (tBHPtr), rAccDistr (accDistr),
    rNotMajorD (notMajorD),
    catchOtherEndingResults (catchOthEndingResults),
    wantDefendantsSettl (wantDefSettl), pDateAbolish (dateAbolishPtr),
# if INSTANCE
#   if APPEAL
    pPairs (new SessRCDKeyToSidePair (rSess, & (rSess.date), true,
                                      INVOLV_2ND_T_BK_SUIT_TO_DEFENDS)),
#   else  // of APPEAL
    pPairs (new SessRCDKeyToSidePair (rSess, & (rSess.date), true,
                             strchr (KIND_1ST_LAWSUITS, rLawsuit.kind) ?
                             INVOLV_TIME_BK_SUIT_TO_DEFENDS :
                             INVOLV_2ND_T_BK_SUIT_TO_DEFENDS)),
#   endif  // of APPEAL
# else  // of INSTANCE
    pPairs (new SessRCDKeyToSidePair (rSess, & (rSess.date), true,
                                      INVOLV_TIME_BK_SUIT_TO_DEFENDS,
                             rLawsuit.kind == KIND_ADMIN_PUNISHMENT_LAW ?
                             INVOLV_DEFENDS_TO_EXCLUDE_P_ADM : NULL)),
# endif  // of INSTANCE
    pEndDocs (new SessionEndDocs (rSess.key, rSess.date, rSess.kind)),
    pAbolishingSession (pAbolishingSess)
 {
 }  // TimeBookEndDocColumns :: TimeBookEndDocColumns

TimeBookEndDocColumns :: ~TimeBookEndDocColumns ()
 {
  if (pEndDocs)
    delete pEndDocs ;
  if (pPairs)
    delete pPairs ;
 }  // TimeBookEndDocColumns :: ~TimeBookEndDocColumns

void TimeBookEndDocColumns :: FillPrintingList ()
 {
  // With this method I attempt to create an order of printing of
  // defendants.
  // The order is based on two numbers:
  //   - first -- the serial number of the decision (for the given
  //     session) - this is dependent on SessionEndDocs sort.
  //   - second -- the serial number of the defendant (for the given
  //     session) - this is dependent on SessRCDKeyToSidePair sort (if
  //     there is a sort...).

  printingList.Flush () ;
   {  // -- 1 --  // Collect the defendants who have judgement(s) entered.
    DecPtr pDec ;
    TDecision & dec = pDec.Ref () ;

    for (unsigned int i = 0 ; i < pEndDocs -> DocsFound () ; i ++)
     {  // -- 2 --  // Loop through end docs (I will use the serial
			  // number (i) below in the body of the loop and
			  // later, in the columns filling time).
	if (pEndDocs -> NextKey (dec))
	 {  // -- 3 --
	  EndDocJudgements edj (dec.key, dec.date, dec.kind) ;
	  JudgPtr pJudg ;
	  TJudgement & judg = pJudg.Ref () ;
	  TSideWind wDef ;

	  for (unsigned int j = 0 ; j < edj.JudgementsFound () ; j ++)
	   {  // -- 4 --  // Loop through personal data (because there are
				// no personal data in the decisions).
	    if (edj.NextKey (judg))
	     {  // -- 5 --
		bool wFound = false ;
		pPairs -> readIndexesToZero () ;
		for (unsigned int k = 0 ;
		     wFound == false && k < pPairs -> getDefendantsCount () ;
		     k ++
		    )
		 {  // -- 6 --  // Loop through defendants - to detect for
				    // which of them is the current
				    // pesonal judgement entered.
		  bool clRet ;        // for syntax only
		  bool defReturned ;

		  pPairs -> nextPair (NULL, & wDef, clRet, defReturned) ;
		  if (defReturned)
		    if (wDef.ucnType == judg.ucnType &&
			  strcmp (wDef.ucn, judg.ucn) == 0 &&
			  (judg.involvement == '\0' ||
// 20060413          judg.involvement == INVOLVEMENT_JUDGEMENT  // 09.jan.2004
			   wDef.involvement == judg.involvement))
			 {  // -- 7 --
			  DoubleNos newDn ;

			  newDn.no0 = i ;  // ATTENTION! (i) // In the end...
			  newDn.no1 = k ;
			  printingList.Add (new DoubleNos(newDn));
			  wFound = true ;
			 }  // -- 7 --
		 }  // -- 6 --
		 // 2009:168 LPR/TRQ: removed wFound error message
	     }  // -- 5 --
	   }  // -- 4 --
	 }  // -- 3 --
     }  // -- 2 --
   }  // -- 1 --
  for (int l = 0 ; l < pPairs -> getDefendantsCount () ; l ++)
   {  // Add the defendants who have not any judgement entered.
    bool hasJudgement = false ;

    for (int m = 0 ;
         hasJudgement == false && m < printingList.Count () ;
         m ++)
	if (printingList [m]->no1 == l)
	  hasJudgement = true ;
    if (hasJudgement == false)
     {
      DoubleNos newDn ;

      newDn.no0 = MAXINT ;  // this means "No judgement"
      newDn.no1 = l ;
      printingList.Add (new DoubleNos(newDn)) ;
     }
   }
  pEndDocs -> Rewind () ;
  pPairs -> readIndexesToZero () ;
 }  // TimeBookEndDocColumns :: FillPrintingList

void TimeBookEndDocColumns :: SessionStatistics ()
 {
  if (strchr (RESULT_FINALIZEDS, rSess.result))
    if (countThisSession (rSess, rLawsuit, true))
     {
      LawsuitAllJudgements lAJ (rLawsuit.key) ;
      JudgPtr pJudg ;
      TJudgement & judg = pJudg.Ref () ;

      for (unsigned int i = 0 ; i < lAJ.JudgementsFound () ; i ++)
        if (lAJ.NextJudgement (judg))
          markInDistribution (judg, rAccDistr) ;
     }
 }  // TimeBookEndDocColumns :: SessionStatistics

void TimeBookEndDocColumns :: ProcessSession ()
 {
  SessionStatistics () ;
//  AddDateRetToDateRetColumn (rSess.returned) ;  // It should be added.
# if INSTANCE
#   if APPEAL
  if (rSess.result == RESULT_CIT_192_193
      || rSess.result == RESULT_CIT_DECIS_194_1  // 07.feb.2001
      || rSess.result == RESULT_CIT_37  // 07.feb.2001
			|| rSess.result == RESULT_CIT_192_5  // 25.jun.2002
		 )
    AddResultToDecisColumn (rSess.result) ;
#   endif  // of APPEAL
# endif  // of INSTANCE
  if (strchr (KIND_CLOSED_SESSIONS, rSess.kind) &&
      ! strIsBlank (rSess.text))
    AddToDecisColumn (rSess.text) ;
  if (pEndDocs -> DocsFound () == 0)
   {  // -- 6 --
    if (strchr (RESULT_WITH_DECISIONS, rSess.result) ||
        (catchOtherEndingResults &&
         resMustBeTreatAsDecision (rSess.result)))
    AddResultToDecisColumn (rSess.result) ;
# if ! INSTANCE
// ------------------------ BOZA - like !!! -------------------------
    // 2010:111 TRQ: also for PUBLIC DECISION
    if (rSess.result == RESULT_BOZA_LIKE_0 || rSess.result == RESULT_PUBLIC_DECISION)
	if (! strIsBlank (rSess.text))
        AddToDecisColumn (rSess.text) ;
# endif
# if INSTANCE
#   if APPEAL
    if (rSess.result == RESULT_CIT_192_193)
      AddResultToDecisColumn (rSess.result) ;
#   else  // of APPEAL
    if (strchr (KIND_2ND_LAWSUITS, rLawsuit.kind))
      if (rSess.result == RESULT_CANCEL_BY_AGREEMENT)
        AddResultToDecisColumn (rSess.result) ;
#   endif  // of APPEAL
# endif  // of INSTANCE
    if (rLawsuit.key.type == TYPE_PUNISHMENT_LAW &&
        rSess.result == RESULT_WITH_PROTOCOL)
      if (! strIsBlank (rSess.text))
        AddToDecisColumn (rSess.text) ;
// ------------------------------------------------------------------
   }  // -- 6 --
  if (pEndDocs -> DocsFound () == 0)
   {
    WriteDefendants (MAXINT) ;
    AdjustColumnsRows () ;
   }
 }  // TimeBookEndDocColumns :: ProcessSession

void TimeBookEndDocColumns ::
       ProcessEndDocJudgements (const TDecision & dec, const int no0)
 {
  for (int i = 0 ; i < printingList.Count () ; i ++)
   {  // -- 0 --
    const DoubleNos dn = *printingList [i] ;

    if (dn.no0 == no0)
     {  // -- 1 --
	TSideWind wDef ;
	TDefendant defendant ;

      pPairs -> readIndexesToZero () ;
      for (unsigned int j = 0 ; j < dn.no1 ; j ++)
        pPairs -> nextPair (NULL, NULL) ;
      if (pPairs -> nextPair (NULL, & defendant, NULL, & wDef,
                              false, wantDefendantsSettl))
       {  // -- 2 --
        bool notMaj = false ;

        if (AddJudgemToDecisColumn (rLawsuit.key, rSess.date, wDef.ucnType,
                                    wDef.ucn,
                                    wDef.involvement, // 20060413
                                    dec.kind, NULL, & notMaj))
         {
          AddEmptyRowToDecisColumn () ;
          if (notMaj && strchr (RESULT_FINALIZEDS, rSess.result))
            rNotMajorD ++ ;
         }
        AddDefNameToDefsColumn (defendant, wantDefendantsSettl) ;
       }  // -- 2 --
     }  // -- 1 --
    AdjustColumnsRows () ;
   }  // -- 0 --
  pPairs -> readIndexesToZero () ;
 }  // TimeBookEndDocColumns :: ProcessEndDocJudgements

void TimeBookEndDocColumns :: WriteDefendants (const int no0)
 {
  TDefendant defendant ;

  pPairs -> readIndexesToZero () ;
  for (unsigned int i = 0 ; i < pPairs -> getDefendantsCount () ; i ++)
    if (pPairs -> nextPair (NULL, & defendant, NULL, NULL,
                            false, wantDefendantsSettl))
      for (int j = 0 ; j < printingList.Count () ; j ++)
	  if (printingList [j]->no0 == no0 && printingList [j]->no1 == i)
          AddDefNameToDefsColumn (defendant, wantDefendantsSettl) ;
  pPairs -> readIndexesToZero () ;
 }  // TimeBookEndDocColumns :: WriteDefendants

void TimeBookEndDocColumns ::
       ProcessOneEndDoc (TDecision & dec, const int no0)
 {
  AdjustColumnsRows () ;
//  AddDateRetToDateRetColumn (dec.returned) ;  // 21.sep.2001
  if (dec.PersonalData ())
   {  // -- 0a --
    if (strchr (RESULT_WITH_DECISIONS, rSess.result)
        || (rSess.key.type == TYPE_PUNISHMENT_LAW &&  // 04.feb.2000
            rSess.result == RESULT_LEFT_BULWARK)  // 04.feb.2000
        || (catchOtherEndingResults &&
            resMustBeTreatAsDecision (rSess.result))
        || rSess.result == RESULT_WITH_BULWARK  // 22.jun.2000
       )
     {  // -- 1 --
      AddDecisionToDecisColumn (dec, pTBH, true
# if INSTANCE
                                , false, rLawsuit.kind, rSess.kind,
#  if APPEAL
                                '\0',
#  else  // of APPEAL
                                ((strchr (KIND_2ND_LAWSUITS,
                                          rLawsuit.kind) &&
                                  rSess.result ==
                                  RESULT_CANCEL_BY_AGREEMENT) ?
                                    rSess.result : '\0'
                                ),
#  endif  // of APPEAL
                                rSess.result
# endif
                               ) ;
      AddEmptyRowToDecisColumn () ;
      AdjustColumnsRows () ;
      ProcessEndDocJudgements (dec, no0) ;
     }  // -- 1 --
    else
     {
      WriteDefendants (no0) ;
      AdjustColumnsRows () ;
     }
   }  // -- 0a --
  else
   {  // -- 5 --
    if (strchr (RESULT_WITH_DECISIONS, rSess.result) ||
        (catchOtherEndingResults &&
         resMustBeTreatAsDecision (rSess.result)))
      AddDecisionToDecisColumn (dec, pTBH, true
# if INSTANCE
                                , false, rLawsuit.kind, rSess.kind,
#  if APPEAL
                                '\0',
#  else  // of APPEAL
                                ((strchr (KIND_2ND_LAWSUITS,
                                          rLawsuit.kind) &&
                                  rSess.result ==
                                  RESULT_CANCEL_BY_AGREEMENT) ?
                                    rSess.result : '\0'
                                ),
#  endif  // of APPEAL
                                rSess.result
# endif
                               ) ;
    else
     {
# if INSTANCE
#   if APPEAL
      if (rSess.result == RESULT_CIT_192_193
          || rSess.result == RESULT_CIT_DECIS_194_1  // 07.feb.2001
					|| rSess.result == RESULT_CIT_37  // 07.feb.2001
					|| rSess.result == RESULT_CIT_192_5  // 25.jun.2002
         )
        if (! strIsBlank (dec.text))
          AddToDecisColumn (dec.text) ;
#   endif  // of APPEAL
# endif  // of INSTANCE
     }
    WriteDefendants (no0) ;
    AdjustColumnsRows () ;
   }  // -- 5 --
 }  // TimeBookEndDocColumns :: ProcessOneEndDoc

void TimeBookEndDocColumns :: ProcessEndDocs ()
 {
  DecPtr pDec ;
  TDecision & dec = pDec.Ref () ;

  for (unsigned int i = 0 ; i < pEndDocs -> DocsFound () ; i ++)
    if (pEndDocs -> NextDoc (dec))
      ProcessOneEndDoc (dec, i) ;
 }  // TimeBookEndDocColumns :: ProcessEndDocs

void TimeBookEndDocColumns :: FinalizingActions ()
 {
  AdjustColumnsRows () ;
  if (pEndDocs -> DocsFound () != 0)
    WriteDefendants (MAXINT) ;
  AdjustColumnsRows () ;
# if ! INSTANCE
  if (rSess.key.type == TYPE_PUNISHMENT_LAW &&
      pPairs -> getDefendantsCount () == 0)
    AddToDefsColumn (UNKNOWN_CRIMINAL) ;
  AdjustColumnsRows () ;
# endif
  AddAbolishingToDecisColumn (pDateAbolish, pAbolishingSession
                             ) ;  // 06.nov.2001
  AdjustColumnsRows () ;  // 06.nov.2001
 }  // TimeBookEndDocColumns :: FinalizingActions

void TimeBookEndDocColumns :: FillColumns ()
 {
  FillPrintingList () ;
  ProcessSession () ;
  ProcessEndDocs () ;
  FinalizingActions () ;
 }  // TimeBookEndDocColumns :: FillColumns

void defendantsAndJudgements (TSession & sess,
                              TLawsuit & lawsuit,
                              timeBookHeap * pTBH,
                              accusedsDistribution & accDistr,
                              long int & notMajorD,
                              column * defendantsColumn,
                              column * decisionsJudgementsColumn,
                              column * dateReturnedColumn,
                              bool catchOtherEndingResults,
                              const bool wantDefSettl,
                              const CDate * pDateAbolish,
                          const TSession * pAbolishingSess,  // 13.dec.2001
                          const showJudgements               // 20060719
                             )
 {
 // CHECK_THIS
  if(! showJudgements) decisionsJudgementsColumn = NULL;     // 20060719
  TimeBookEndDocColumns (sess, lawsuit, pTBH, accDistr, notMajorD,
                         defendantsColumn, decisionsJudgementsColumn,
                         dateReturnedColumn, catchOtherEndingResults,
                         wantDefSettl, pDateAbolish,
                         pAbolishingSess).FillColumns () ;
 }  // defendantsAndJudgements

//

