#include "cac.h"

#if COURT_TYPE == COURT_AREAL
static bool kind_not_processed (const char k, const char * const where)
 {
  char kBuff [32] ;

  kBuff [0] = '\0' ;
   {
    ostrstream oss (kBuff, sizeof (kBuff)) ;

    oss << "\'" ;
    if (k)
      oss << k ;
    else
      oss << "\\0" ;  
    oss << "\'" << ends ;
   }
  kBuff [sizeof (kBuff) - 1] = '\0' ; 
  return error ("%s:\n"
                "Видът събитие %s остава необработен!",
                where ? where : "", kBuff) ;
 }  // kind_not_processed

TradeLaw634BookEnum :: TradeLaw634BookEnum ()
 {
 }  // TradeLaw634BookEnum :: TradeLaw634BookEnum

bool TradeLaw634BookColumns :: IsKindForTradeLaw634Book (const char k)
 {
  return (is_surround_kind (k) ||
          is_session_kind (k) ||
          is_decision_kind (k) ||
          is_inreg_kind (k)
         )  ;
 }  // TradeLaw634BookColumns :: IsKindForTradeLaw634Book

bool TradeLaw634BookColumns :: IndexAndDateNotTooBig
       (TGSArray <LinkedXKXKDatedContainerSK> & eventsArr, const int ind,
        const Trade634DlgData & )
 {
  return (ind < eventsArr.Count ()) ;
 }  // TradeLaw634BookColumns :: IndexAndDateNotTooBig

bool TradeLaw634BookColumns :: PreliminaryChecks
      (TGSArray <LinkedXKXKDatedContainerSK> & eventsArr,
       int & startIndex, const Trade634DlgData & t634dd)
 {
  const int items = eventsArr.Count () ;

  if (items <= 0 || startIndex >= items)
    return false ;
  if ((IndexAndDateNotTooBig (eventsArr, startIndex, t634dd)) == false ||
      (eventsArr [0] -> ChainDate () < t634dd.begD) ||
      (eventsArr [0] -> ChainDate () > t634dd.endD)
     )
   {
    startIndex = items ;
    return false ;
   }
  return true ;
 }  // TradeLaw634BookColumns :: PreliminaryChecks

bool TradeLaw634BookColumns :: IsSurround (const int ind) const
 {
  if (ind >= items)
    return false ;

  return is_surround_kind (events [ind] -> link.kind) ;
 }  // TradeLaw634BookColumns :: IsSurround

bool TradeLaw634BookColumns :: IsSession (const int ind) const
 {
  if (ind >= items)
    return false ;

  return is_session_kind (events [ind] -> link.kind) ;
 }  // TradeLaw634BookColumns :: IsSession

bool TradeLaw634BookColumns :: DateInRange (const CDate & d)
 {
  return (t634DD.begD <= d) ;
 }  // TradeLaw634BookColumns :: DateInRange

void TradeLaw634BookColumns :: AddSurroundSidesToColumn (
                            column * const pColumn, char * const buff,
				    const size_t buffL, TSurround & rSurround)
 {  // by idea of AppealedLawsInRegColumns :: AddAppealersToColumn
  if (! (pColumn && buff && buffL))
    return ;

  TSideWind waln ;

  waln << rSurround ;
  waln.FloatCriteria (buff) ;
  if (strlen (buff) < buffL - 2)
   {  // -- ~1~ --
    char * const partArea = buff + strlen (buff) + 1 ;
    const size_t partLen = buffL - 1 - strlen (buff) ;
    TQuery q (buff) ;

    while (q.Read ())
     {  // -- ~2~ --
      waln << q ;
      if (surr_kind_and_waln_state_match (rSurround.kind, waln.state))
       {
        ucnAndUcnTypeToName (waln.ucn, waln.ucnType,
                             partArea, partLen) ;
        pColumn -> addText (partArea) ;
       }
      else
        if (waln.involvement == INVOLVEMENT_SENDER)
         {
          ucnAndUcnTypeToName (waln.ucn, waln.ucnType,
                               partArea, partLen) ;
          pColumn -> addText (partArea) ;
         }
     }  // -- ~2~ --
   }  // -- ~1~ --
 }  // TradeLaw634BookColumns :: AddSurroundSidesToColumn

void TradeLaw634BookColumns :: MakeSurroundColumns (const int ind)
 {
  if (ind < 0 || ind >= items)
    return ;

  TDatedContainer & surroundDC = (events [ind]) -> link ;

  if (! DateInRange (//surroundDC.date
                     (events [ind]) -> linkImportantDate  // 18.dec.2000
                     ))
    return ;

   {  // -- 0 --
    TSurround * pSurround = new TSurround ;

    (* pSurround) << surroundDC ;
    if (pSurround -> Get ())
     {  // -- 1 --
      column * pColumn ;
      char * line = new char [SIZE_OF_LINE] ;
      const size_t lineL = SIZE_OF_LINE - 1 ;

      line [lineL] = '\0' ;
      if ((pColumn = columns [iC_ComeInDate]) != NULL)
       {  // -- iC_ComeInDate --
        add_date_to_column (pColumn, pSurround -> date) ;
       }  // -- iC_ComeInDate --
      if ((pColumn = columns [iC_SurroundAct]) != NULL)
       {  // -- iC_SurroundAct --
        surround_to_string (* pSurround, line, lineL) ;
        pColumn -> addText (line) ;
       }  // -- iC_SurroundAct --
      if ((pColumn = columns [iC_Involvement]) != NULL)
       {  // -- iC_Involvement --
        AddSurroundSidesToColumn (pColumn, line, lineL, * pSurround) ;
       }  // -- iC_Involvement --
      delete [] line ;
     }  // -- 1 --
    delete pSurround ;
   }  // -- 0 --
   {
    columnArray forAdjusting ;

    forAdjusting.Add (columns [iC_ComeInDate]) ;
    forAdjusting.Add (columns [iC_SurroundAct]) ;
    forAdjusting.Add (columns [iC_Involvement]) ;
    adjust_column_rows (forAdjusting) ;
   }
 }  // TradeLaw634BookColumns :: MakeSurroundColumns

void TradeLaw634BookColumns :: MakeSessionColumns (const int ind)
 {
  if (ind < 0 || ind >= items)
    return ;

  TDatedContainer & sessionDC = (events [ind]) -> link ;
  TSession * pSess = new TSession ;

  (* pSess) << sessionDC ;
  if (pSess -> Get () &&
      DateInRange (//pSess -> date
                   (events [ind]) -> linkImportantDate  // 18.dec.2000
                   )
     )
   {  // -- 0 --
    column * pColumn ;
    char * line = new char [SIZE_OF_LINE] ;
    const size_t lineL = SIZE_OF_LINE - 1 ;

    line [lineL] = '\0' ;
    if ((pColumn = columns [iC_Involvement]) != NULL)
     {  // -- iC_Involvement --
	// Nothing from the sesion.
     }  // -- iC_Involvement --
    if ((pColumn = columns [iC_ArealCourtAct]) != NULL)
     {  // -- iC_ArealCourtAct --
       {
        ostrstream oss (line, lineL) ;

        if (strchr (KIND_CLOSED_SESSIONS, pSess -> kind))
          oss << "ЗЗ/РЗ" ;
        else
          if (pSess -> kind == KIND_CREDITORS_MEETING)
           {
            TKind k ;

            k.kind = KIND_CREDITORS_MEETING ;
            if (k.Get ())
              oss << k.name ;
           }
          else
            oss << "Открито заседание" ;
        oss << " от" << ends ;
       }
	 {
	  char tmp [sizeof Result->name + 16] ;

	  tmp [0] = '\0' ;
	  Result->result = pSess -> result ;
	  if ((! strchr (KIND_CLOSED_SESSIONS, pSess -> kind)) && Result->Get ())
	    scSprintf (sizeof tmp, tmp, "- %s", Result->name) ;
	  add_date_to_column (pColumn, pSess -> date, line, tmp) ;
       }
	if (any(pSess -> text))
        pColumn -> addText (pSess -> text) ;
      if (DatedContainer_ (leadSurround).FilledOK ())
       {
        SurroundmentPtr pMent ;

        if (surroundment_key_data (* pMent, leadSurround.key,
                                   leadSurround.kind, pSess -> key,
                                   pSess -> date, pSess -> kind))
          if (pMent.Ref ().Get ())
            if (surr_ment_res_and_act_kind_match (pMent.Ref ().result,
                                                  pSess -> kind))
             {
		  if (any(pMent.Ref ().text))
                pColumn -> addText (pMent.Ref ().text) ;
//              else
//                addResultToColumn (pMent.Ref ().result, pColumn) ;
             }
       }
     }  // -- iC_ArealCourtAct --
    delete [] line ;
   }  // -- 0 --
  delete pSess ;
 }  // TradeLaw634BookColumns :: MakeSessionColumns

void TradeLaw634BookColumns :: MakeDecisionColumns (const int ind)
 {
  if (ind < 0 || ind >= items)
    return ;

  TDatedContainer & decisionDC = (events [ind]) -> link ;
  TDecision * pDec = new TDecision ;
  const CDate & importantDate =
    (events [ind]) -> linkImportantDate ; //pDec->fromDate;//18.dec.2000

  (* pDec) << decisionDC ;
  if (pDec -> Get () && DateInRange (importantDate))
   {  // -- 0 --
    column * pColumn ;
    char * line = new char [SIZE_OF_LINE] ;
    const size_t lineL = SIZE_OF_LINE - 1 ;

    line [lineL] = '\0' ;
    if ((pColumn = columns [iC_ArealCourtAct]) != NULL)
     {  // -- iC_ArealCourtAct --
      bool DecIdentificationPrinted = false ;

      line [0] = '\0' ;
      if (pDec -> kind)
       {
        kindToString (pDec -> kind, line, lineL) ;
        strConcat (line, " от", lineL) ;
       }
      // Attention! Do no change the text in line, unless you use
      // add_date_to_column  --> it is used at 2 places
	if (any(pDec -> text))
       {
        add_date_to_column (pColumn, importantDate, line) ;
        pColumn -> addText (pDec -> text) ;
        DecIdentificationPrinted = true ;
       }
      if (DatedContainer_ (leadSurround).FilledOK ())
       {
        SurroundmentPtr pMent ;

        if (surroundment_key_data (* pMent, leadSurround.key,
                                   leadSurround.kind, pDec -> key,
                                   pDec -> date, pDec -> sessionKind))
          if (pMent.Ref ().Get ())
            if (surr_ment_res_and_act_kind_match (pMent.Ref ().result,
                                                  pDec -> kind))
             {
              if (DecIdentificationPrinted == false)
                add_date_to_column (pColumn, importantDate, line) ;
              if (any(pMent.Ref ().text))
                pColumn -> addText (pMent.Ref ().text) ;
              else
                addResultToColumn (pMent.Ref ().result, pColumn) ;
             }
       }
     }  // -- iC_ArealCourtAct --
    delete [] line ;
   }  // -- 0 --
  delete pDec ;
 }  // TradeLaw634BookColumns :: MakeDecisionColumns

void TradeLaw634BookColumns :: MakeInRegEventColumns (TInReg & rInReg,
                                                      char * const line,
                                                      const size_t lineL)
 {  // 31.jan.2001
  if (! (line && lineL > 1))
    return ;
  const size_t crtIRESz = 512 ;
  char * crtIRE = new char [crtIRESz] ;

  if (AppealedLawsInRegColumns ::
        MakeInRegEventsCriteria (rInReg, crtIRE, crtIRESz))
   {  // -- 0 --
    TQuery ireQ (crtIRE) ;
    TInRegEvent * pIRE = new TInRegEvent ;

    while (ireQ.Read ())
     {  // -- 1 --
      line [0] = '\0' ;
      (* pIRE) << ireQ ;
      if (DateInRange (pIRE -> date) == false)
        continue ;

      column * pColumn ;

      if ((pColumn = columns [iC_ArealCourtAct]) != NULL)
       {  // -- iC_ArealCourtAct --
        if (pIRE -> result)
          resultToString (pIRE -> result, line, lineL) ;
        else
          ostrstream (line, lineL) << "?" << ends ;  
        ostrstream (line + strlen (line), lineL - strlen (line))
          << " на " << pIRE -> date << ends ;
        line [lineL - 1] = '\0' ;  
        pColumn -> addText (line) ; 
       }  // -- iC_ArealCourtAct --
      if (strchr (RESULT_IOREG_RETURNEDS, pIRE -> result))
       {
        TOutReg * pOutReg = new TOutReg ;

        if (no_year_to_outreg (pIRE -> outNo, pIRE -> outYear,
                               * pOutReg))
          MakeOutRegColumns (* pOutReg) ;
        delete pOutReg ;
       }
      else
       {
	  columnArray forAdjusting ;

        forAdjusting.Add (columns [iC_ArealCourtAct]) ;
        forAdjusting.Add (columns [iC_AppealCourtAct]) ;
        forAdjusting.Add (columns [iC_SupremeConcernCourtAct]) ;
        adjust_column_rows (forAdjusting) ;
       }
     }  // -- 1 --
    delete pIRE ;
   }  // -- 0 --
  delete [] crtIRE ;
 }  // TradeLaw634BookColumns :: MakeInRegEventColumns

void TradeLaw634BookColumns :: MakeInRegColumns (const int ind)
 {
  if (ind < 0 || ind >= items)
    return ;

  TDatedContainer & inRegDC = (events [ind]) -> link ;
  TInReg * pInReg = new TInReg ;
  const CDate & importantDate =
    (events [ind]) -> linkImportantDate ; // pInReg -> date ;//18.dec.2000

  (* pInReg) << inRegDC ;
  if (pInReg -> Get () && DateInRange (importantDate))
   {  // -- 0 --
     {
	columnArray forAdjusting ;

      forAdjusting.Add (columns [iC_ComeInDate]) ;
      forAdjusting.Add (columns [iC_SurroundAct]) ;
      forAdjusting.Add (columns [iC_Involvement]) ;
      forAdjusting.Add (columns [iC_ArealCourtAct]) ;
      adjust_column_rows (forAdjusting) ;  // , 1) ;  // 09.oct.2001
      forAdjusting.Add (columns [iC_AppealCourtAct]) ;
      forAdjusting.Add (columns [iC_SupremeConcernCourtAct]) ;
      adjust_column_rows (forAdjusting) ;
     }
    column * pColumn ;
    char * line = new char [SIZE_OF_LINE] ;
    const size_t lineL = SIZE_OF_LINE - 1 ;

    line [lineL] = '\0' ;
    if ((pColumn = columns [iC_ComeInDate]) != NULL)
     {  // -- iC_ComeInDate --
      add_date_to_column (pColumn, importantDate) ;
     }  // -- iC_ComeInDate --
    if ((pColumn = columns [iC_Involvement]) != NULL)
     {  // -- iC_Involvement --
      AppealedLawsInRegColumns ::
        AddAppealersToColumn (pColumn, line, lineL, * pInReg) ;
     }  // -- iC_Involvement --
    if ((pColumn = columns [iC_SurroundAct]) != NULL)
     {  // -- iC_SurroundAct --
      kindToString (pInReg -> kind, line, lineL) ;
      pColumn -> addText (line) ;
     }  // -- iC_SurroundAct --
    MakeInRegEventColumns (* pInReg, line, lineL) ; 
    delete [] line ;
   }  // -- 0 --
  delete pInReg ;
 }  // TradeLaw634BookColumns :: MakeInRegColumns

void TradeLaw634BookColumns :: MakeOutRegColumns (TOutReg & rOutReg)
 {
  const CDate importantDate = rOutReg.returned ;

  if (DateInRange (importantDate) == false)
    return ;

  column * pColumn ;

  if ((pColumn = columns [iC_AppealCourtAct]) != NULL)
   {  // -- iC_AppealCourtAct --
    const char reso1 = rOutReg.resultI ;
    const char reso2 = rOutReg.resultII ;

    if ((reso1 && reso1 != RESOLUTION_FAKE) ||
        (reso2 && reso2 != RESOLUTION_FAKE)
        )
     {
      add_date_to_column (pColumn, importantDate) ;
      add_resolution_name_to_column (reso1, pColumn) ;
      if (reso2 && reso2 != RESOLUTION_FAKE)
       {
        pColumn -> addText (HIGHER_INST_2) ;
        add_resolution_name_to_column (reso2, pColumn) ;
       }
     }
   }  // -- iC_AppealCourtAct --
  if ((pColumn = columns [iC_SupremeConcernCourtAct]) != NULL)
   {  // -- iC_SupremeConcernCourtAct --
    const char resoS = rOutReg.resultKS ;
    const char resoMJ = rOutReg.resultMJ ;

    if ((resoS && resoS != RESOLUTION_FAKE) ||
        (resoMJ && resoMJ != RESOLUTION_FAKE)
        )
     {
      add_date_to_column (pColumn, importantDate) ;
      add_resolution_name_to_column (resoS, pColumn) ;
      if (resoMJ && resoMJ != RESOLUTION_FAKE)
       {
        pColumn -> addText (AppNameMinistryShort) ;
        add_resolution_name_to_column (resoMJ, pColumn) ;
       }
     }
   }  // -- iC_SupremeConcernCourtAct --
 }  // TradeLaw634BookColumns :: MakeOutRegColumns

void TradeLaw634BookColumns ::
       MakeColumns (int & startIndex, const int begSerialNo)
 {
  if (! PreliminaryChecks (events, startIndex, t634DD))
    return ;

  const int start = startIndex ;
  const int end = items ;  // DetectASequence (startIndex) ; // 03.oct.2001

  startIndex = end ;  // Do not change startIndex more !
  {
    column * pColumn ;

    if ((pColumn = columns [iC_SerialNo]) != NULL)
     {  // -- iC_SerialNo --
      addLongToColumn (pColumn, begSerialNo) ;
     }  // -- iC_SerialNo --

    if ((pColumn = columns [iC_LawsuitNoYear]) != NULL)      // 20050221
     {  // -- iC_LawsuitNoYear --
      char tmp[64];

      t634DD.lawKey.PrintShortest(tmp, sizeof(tmp));
      pColumn->addText(tmp);
     }  // -- iC_LawsuitNoYear --

    if ((pColumn = columns [iC_Judge]) != NULL)              // 20050221
     {  // -- iC_Judge --  get from T_Lawsuit  F_JUDGE, F_COMPOSITION
        LawPtr pLaw;
        pLaw.Ref().key = t634DD.lawKey;
        if(pLaw.Ref().Get())
        {
           char tmp[128];

           if(ucnPresent(pLaw.Ref().judge) &&
             citizenUCNAndUCNTypeToName(pLaw.Ref().judge,
               UCN_CITIZEN_UCN, tmp, sizeof(tmp), true, true))
           {
             pColumn->addText(tmp);
           }
           ZERO_LEN(tmp);
           if ( use_area_compositions () )  // 20070806
           {
	     compositionToString(pLaw.Ref().composition, tmp, sizeof(tmp));
             strConcat(tmp, " състав", sizeof(tmp));
             pColumn->addText(tmp);
           }
        }
     }  // -- iC_Judge --


    if ((pColumn = columns [iC_FirmAndAddress]) != NULL)      // 20050221
    {  // -- iC_FirmAndAddress --
      LawPtr pLaw;
      pLaw.Ref().key = t634DD.lawKey;
      if(pLaw.Ref().Get())
      {
	 TSideWind s ;
         CharPtr crit (1024) ;

         s.key = pLaw.Ref().key ;   //   t634DD.lawKey;
	   s.kind = pLaw.Ref().kind ;
	   s.date = pLaw.Ref().date ;
         s.FloatCriteria (crit) ;
	 {
            TQuery q (crit) ;

            while (q.Read ())
	    {
               s << q ;
               if (s.involvement == INVOLVEMENT_DEBTOR ||
                   s.involvement == INVOLVEMENT_DEFENDANT)
	       {
		  char tmp [128] ;

                  if (ucnAndUcnTypeToName (s.ucn, s.ucnType, tmp, sizeof (tmp)))
		  {
		     pColumn->addText(tmp);
		     mstr buff;
		     TLinkWind link;
		     link.SetOrdinary(s);
		     print_link_address(buff, link);
		     pColumn->addText(buff);
		  }
	       }
	    }
	 }
      }
    }  // -- iC_FirmAndAddress --

  }

  for (int i = start ;
       i < end &&
                  i < items ;  // This is an insurance
       i ++)
   {  // -- 0 --
    const char k = events [i] -> link.kind ;

    if (is_surround_kind (k))
      MakeSurroundColumns (i) ;
    else
     {
      if (is_session_kind (k))
        MakeSessionColumns (i) ;
      else
        if (is_decision_kind (k))
           MakeDecisionColumns (i) ;
         else
           if (is_inreg_kind (k))
             MakeInRegColumns (i) ;
           else
             kind_not_processed (k, "TradeLaw634BookColumns::MakeColumns");
                 // The format string appears below, too.
      if (i < end - 1)
       {
        if (columns [iC_ArealCourtAct])
         {
          columns [iC_ArealCourtAct] -> addEmptyRow () ;

	    columnArray forAdjusting ;

          forAdjusting.Add (columns [iC_ComeInDate]) ;
          forAdjusting.Add (columns [iC_SurroundAct]) ;
          forAdjusting.Add (columns [iC_Involvement]) ;
          forAdjusting.Add (columns [iC_SerialNo]) ;
          forAdjusting.Add (columns [iC_ArealCourtAct]) ;
          forAdjusting.Add (columns [iC_AppealCourtAct]) ;
          forAdjusting.Add (columns [iC_AppealCourtAct]) ;
          adjust_column_rows (forAdjusting) ;
         }
       }
     }
   }  // -- 0 --
 }  // TradeLaw634BookColumns :: MakeColumns

TradeLaw634BookColumns ::
  TradeLaw634BookColumns (const char * formatFN, TWindow * parent,
                          const Trade634DlgData & rT634DD,
                       TGSArray <LinkedXKXKDatedContainerSK> & eventsArr,
                          int & startIndex, const int begSerialNo,
                          const size_t minRows) :
    TradeLaw634BookEnum (),
    columnsArr (formatFN, parent, i_columns,
                1 + OFFSET_TRADE_634_BOOK_CIPHER),
    t634DD (rT634DD), events (eventsArr),
    items (events.Count ())
 {
  if (items && DatedContainer_ (events [0] -> surround).FilledOK ())
    leadSurround << (events [0] -> surround) ;
  MakeColumns (startIndex, begSerialNo) ;
  defineMaxRows () ;
  if (maxRows < minRows)
    maxRows = minRows ;
 }  // TradeLaw634BookColumns :: TradeLaw634BookColumns

void TradeLaw634Book :: MakeBookFormatFN ()
 {
  scSprintf (sizeof (bookFormatFN), bookFormatFN, "text\\634tlaw%s",
             DEFAULT_FORMAT_FILE_EXTENSION) ;
 }  // TradeLaw634Book :: MakeBookFormatFN

TradeLaw634Book :: TradeLaw634Book (TWindow * const parent) :
                     pParent (parent)
 {
  MakeBookFormatFN () ;
 }  // TradeLaw634Book :: TradeLaw634Book

TradeLaw634Book :: ~TradeLaw634Book ()
 {
 }  // TradeLaw634Book :: ~TradeLaw634Book

void TradeLaw634Book :: MarkSurroundChains
          (TGSArray <LinkedXKXKDatedContainerSK> & evArr)
 {
  const int items = evArr.Count () ;

  for (int i = 0 ; i < items ; i ++)
   {
    LinkedXKXKDatedContainerSK & ev = * (evArr [i]) ;

    if (is_surround_kind (ev.link.kind))
     {
      for (int j = 0 ; j < items ; j ++)
       {
        LinkedXKXKDatedContainerSK & target = * (evArr [j]) ;

        if (ev.key == target.key &&
            ev.date == target.date &&
            ev.sessKind == target.sessKind)
         {
          target.surround << ev.link ;
//          target.date = ev.link.date ;  // because the array is sorted
// by surround date. But, this is not a good idea to fake the "key's"
// data. 13.dec.2000 commented. This reflects to
// LinkedXKXKDatedContainerSK :: operator <
         }
       }
     }
   }
 }  // TradeLaw634Book :: MarkSurroundChains

void TradeLaw634Book ::
	 CopyArray (TGSArray <LinkedXKXKDatedContainerSK> & src,
                  TGSArray <LinkedXKXKDatedContainerSK> & dest)
 {
  const int sItems = src.Count () ;

  for (int i = 0 ; i < sItems ; i ++)
   {
    LinkedXKXKDatedContainerSK * pSrcItem = src [i] ;
    LinkedXKXKDatedContainerSK * pNewItem =
      new LinkedXKXKDatedContainerSK ;

    (* pNewItem) << (* pSrcItem) ;
    (pNewItem -> link) << (pSrcItem -> link) ;
    pNewItem -> date = pSrcItem -> date ;
    pNewItem -> sessKind = pSrcItem -> sessKind ;
    (pNewItem -> surround) << (pSrcItem -> surround) ;
    pNewItem -> linkImportantDate =
      pSrcItem -> linkImportantDate ;  // 18.dec.2000
    dest.Add (pNewItem) ;
   }
 }  // TradeLaw634Book :: CopyArray

void TradeLaw634Book ::
	  MoveArray (TGSArray <LinkedXKXKDatedContainerSK> & src,
                   TGSArray <LinkedXKXKDatedContainerSK> & dest)
 {
  dest.Flush () ;
  CopyArray (src, dest) ;
  src.Flush () ;
 }  // TradeLaw634Book :: MoveArray
                     
void TradeLaw634Book :: RebuildEventsArray
          (TGSArray <LinkedXKXKDatedContainerSK> & evArray)
 {
  // At the beginning the evArray is sorted by session data.
  // But, it must be sorted by surround data, and then, if there is a
  // session(s) without surround - by session data.

  TGSArray <LinkedXKXKDatedContainerSK> tmp ;

  MoveArray (evArray, tmp) ;
  MarkSurroundChains (tmp) ;  // Marking of surrounds.
  MoveArray (tmp, evArray) ;  // During this process, the array is
                              // re-sorted.
 }  // TradeLaw634Book :: RebuildEventsArray

bool TradeLaw634Book :: LawsuitApproach (const TLawsuit & law) const
 {
  char tmp [128] ;

  TRCDKey_ (law.key).Print (tmp, sizeof (tmp)) ;
  // 2015:027 BANKRUPTCY_REQUESTS -> KIND_INCONSISTENCE_INDOCS
  if (! strchr (KIND_INCONSISTENCE_INDOCS, law.sourceKind))
   {
    TKind kRequired ;

    kRequired.kind = KIND_INCONSISTENCE_INDOCS [0] ;
    if (! kRequired.Get ())
      clearGroupData (kRequired) ;

    TKind kUsed ;

    kUsed.kind = law.sourceKind ;
    if (! kUsed.Get ())
	clearGroupData (kUsed) ;
    if (ask ("%s е образувано по\n"
		 "%s,\n"
		 "вместо по %s.\n"
		 "%s",
		 tmp, kUsed.name, kRequired.name, WANT_CONTINUE_QUESTION
		) == false)
	return false ;
   }
  return true ;
 }  // TradeLaw634Book :: LawsuitApproach

void TradeLaw634Book ::
  EventsHeapToChain (TGSArray <TL634AloneDatedContainer> & eventsHeap,
			   TGSArray <LinkedXKXKDatedContainerSK> & chain,
                     const TLawsuit & law)
 {  // 01.oct.2001
  const int allItems = eventsHeap.Count () ;

  for (int i = 0 ; i < allItems ; i ++)
   {  // -- 1 --
    if (TradeLaw634BookColumns ::
          IsKindForTradeLaw634Book (eventsHeap [i] -> kind))
     {  // -- 2 --
      LinkedXKXKDatedContainerSK *
        pNewLinked = new LinkedXKXKDatedContainerSK ;

      (pNewLinked -> link) << (* (eventsHeap [i])) ;
      (* pNewLinked) << (pNewLinked -> link) ;
      pNewLinked -> key = law.key ;  // !!!

       {  // -- 3 --
        bool found = false ;
        TDatedContainer * pDC =
          kind2DatedContainer (pNewLinked -> kind) ;

        if (pDC)
         {  // -- 4 --
          (* pDC) << (pNewLinked -> link) ;
          if (is_surround_kind (pNewLinked -> kind))
           {  // -- is_surround_kind --
            TSurround * pSurround = dynamic_cast <TSurround *> (pDC) ;

            if ((found = (pSurround && pSurround -> Get ())) == true)
             {
              pNewLinked -> date = DATE_MIN ;
              pNewLinked -> sessKind = STUB_CHAR ; 
              pNewLinked -> linkImportantDate =
                pSurround -> date ;  // 18.dec.2000
             }
           }  // -- is_surround_kind --
          else
            if (is_session_kind (pNewLinked -> kind))
             {  // -- is_session_kind --
              TSession * pSess = dynamic_cast <TSession *> (pDC) ;

              if ((found = pSess) == true)
               {
                pNewLinked -> sessKind = pNewLinked -> kind ;
                pNewLinked -> linkImportantDate =
                  pSess -> date ;  // 18.dec.2000
               }
             }  // -- is_session_kind --
            else
              if (is_decision_kind (pNewLinked -> kind))
               {  // -- is_decision_kind --
                TDecision * pDec = dynamic_cast <TDecision *> (pDC) ;

                if ((found = pDec && pDec -> Get ()) == true)
                 {
                  pNewLinked -> sessKind = pDec -> sessionKind ;
                  pNewLinked -> linkImportantDate =
                    pDec -> fromDate ;  // 18.dec.2000
                 }
               }  // -- is_decision_kind --
              else
                if (is_inreg_kind (pNewLinked -> kind))
                 {  // -- is_inreg_kind --
                  TInReg * pInReg = dynamic_cast <TInReg *> (pDC) ;

                  if ((found = (pInReg && pInReg -> Get ())) == true)
                   {
//   pNewLinked -> date = pInReg -> decisionDate ;  // 03.oct.2001 - begin
//                    pNewLinked -> sessKind = pInReg -> sessionKind ;
                    AloneDatedContainer_xK versusSess ;

                    if (inreg_to_law_type (* pInReg, NULL, & versusSess) &&
                        DatedContainer_ (versusSess).FilledOK ())
                     {
                      pNewLinked -> date = versusSess.date ;
                      pNewLinked -> sessKind = versusSess.kind ;
                     }
                    else
                     {
                      pNewLinked -> date = DATE_MAX ;
                      pNewLinked -> sessKind = STUB_CHAR ;
                      
                      char tmp [128] ;

                      TRCDKeyContainer_ (pInReg -> key,
                                         pInReg -> kind).Print (tmp,
                                                           sizeof (tmp)) ;
                      error ("%s\n""няма да може да бъде коректно "
                             "позициониран(а) във веригата на събитията, "
                             "тъй като не може да се определи с кое "
                             "заседание е свързан(а).", tmp) ;  // If the
                             // program will be alive ... :-(
                     }  //  03.oct.2001 - end
                    pNewLinked -> linkImportantDate =
                      pInReg -> date ;  // 18.dec.2000
                   }
                 }  // -- is_inreg_kind --
                else
                  kind_not_processed (pNewLinked -> kind,
                                 "TradeLaw634Book::EventsHeapToChain") ;
         }  // -- 4 --
        if (! found)
          pNewLinked -> Clear () ;
        delete pDC ;
       }  // -- 3 --
      chain.Add (pNewLinked) ;
     }  // -- 2 --
   }  // -- 1 --
 }  // TradeLaw634Book :: EventsHeapToChain

void TradeLaw634Book ::
  HeadsAndExtensionsOfChains (TGSArray <TL634AloneDatedContainer> & heads,
				  TGArray <TL634AloneDatedContainer> & extensions,
					TLawsuit & law)
 {  // 01.oct.2001
  heads.Flush () ;
  extensions.Flush () ;
   {  // -- 0 --
    TGSArray <TRCDKeyContainer> allLawEvents ;

    query_events (& law, & allLawEvents) ;

    const int allEventsNum = allLawEvents.Count () ;

    for (int i = 0 ; i < allEventsNum ; i ++)
     {  // -- 1 --
      TRCDKeyContainer * const pEvent = allLawEvents [i] ;

      if (TradeLaw634BookColumns ::
            IsKindForTradeLaw634Book (pEvent -> kind))
       {  // -- 2 --
        TL634AloneDatedContainer * pNewDC = new TL634AloneDatedContainer ;

        if (pNewDC)
         {  // -- 3 --
          (* pNewDC) << (* pEvent) ;
          if (is_surround_kind (pNewDC -> kind))
            heads.Add (pNewDC) ;
          else
            if (is_session_kind (pNewDC -> kind))
             {
              SessPtr pSess ;

              (* pSess) << (* pNewDC) ;
              if (session_by_surrounds (pSess))
                extensions.Add (pNewDC) ;
              else
                heads.Add (pNewDC) ;
             }
            else
              if (is_decision_kind (pNewDC -> kind))
                extensions.Add (pNewDC) ;
              else
                if (is_inreg_kind (pNewDC -> kind))
                  extensions.Add (pNewDC) ;
                else
                 {
                  delete pNewDC ;
                  kind_not_processed (pEvent -> kind,
                          "TradeLaw634Book::HeadsAndExtensionsOfChains") ;
                 }
         }  // -- 3 --
       }  // -- 2 --
     }  // -- 1 --
   }  // -- 0 --
 }  // TradeLaw634Book :: HeadsAndExtensionsOfChains

bool TradeLaw634Book :: SurroundChainMatch (const TDatedContainer & leader,
                                            TDatedContainer & extItem)
 {
  bool res = false ;

  if (leader.kind)
   {  // -- 0 --
    TSurround * pSurround =
      dynamic_cast <TSurround *> (kind2DatedContainer (leader.kind)) ;

    if (pSurround)
     {  // -- 1 --
      pSurround -> key = leader.key ;
      pSurround -> date = leader.date ;
      pSurround -> kind = leader.kind ;
      if (is_surround_kind (extItem.kind))
        res = false ;
      else
        if (is_session_kind (extItem.kind))
         {
          SessPtr pSess ;

          (* pSess) << extItem ;
          res = session_by_surround (pSurround, pSess) ;
         }
        else
          if (is_decision_kind (extItem.kind))
           {
            DecPtr pDec ;

            (* pDec) << extItem ;
            res = pDec.Ref ().Get () &&
                  decision_by_surround (pSurround, pDec) ;
           }
          else
            if (is_inreg_kind (extItem.kind))
             {
              TInReg inReg ;

              inReg << extItem ;
              res = inReg.Get () &&
                    inreg_by_surround (pSurround, & inReg, true) ;
             }
      delete pSurround ;
     }  // -- 1 --
    else
      res = false ;
   }  // -- 0 --
  return res ;
 }  // TradeLaw634Book :: SurroundChainMatch

bool TradeLaw634Book :: SessionChainMatch (const TDatedContainer & leader,
                                           TDatedContainer & extItem)
 {
  bool res = false ;

  if (leader.kind)
   {  // -- 0 --
    TSession * pSess =
      dynamic_cast <TSession *> (kind2DatedContainer (leader.kind)) ;

    if (pSess)
     {  // -- 1 --
      pSess -> key = leader.key ;
      pSess -> date = leader.date ;
      pSess -> kind = leader.kind ;
      if (is_decision_kind (extItem.kind))
       {
        DecPtr pDec ;

        (* pDec) << extItem ;
        if (pDec.Ref ().Get ())
          if (pDec.Ref ().key == pSess -> key &&
              pDec.Ref ().date == pSess -> date &&
              pDec.Ref ().sessionKind == pSess -> kind)
            res = true ;
       }
      else
        if (is_inreg_kind (extItem.kind))
         {
          TInReg inReg ;

          inReg << extItem ;
          res = inReg.Get () &&
                inreg_by_session (pSess, & inReg) ;
         }
      delete pSess ;
     }  // -- 1 --
    else
      res = false ;
   }  // -- 0 --
  return res ;
 }  // TradeLaw634Book :: SessionChainMatch

bool TradeLaw634Book :: ChainMatch (const TDatedContainer & leader,
                                    TDatedContainer & extItem)
 {
  bool res ;

  if (is_surround_kind (leader.kind))
    res = SurroundChainMatch (leader, extItem) ;
  else
    if (is_session_kind (leader.kind))
      res = SessionChainMatch (leader, extItem) ;
    else
      res = kind_not_processed (leader.kind,
                                "TradeLaw634Book::ChainMatch") ;
  return res ;
 }  // TradeLaw634Book :: ChainMatch

bool TradeLaw634Book ::
       CompleteAChain (TGSArray <TL634AloneDatedContainer> & heads,
                       TGArray <TL634AloneDatedContainer> & extensions,
                       const int hInd,
                       TGSArray <TL634AloneDatedContainer> & chain)
 {
  bool res = false ;

  chain.Flush () ;
  if (hInd >= 0 && hInd < heads.Count ())
   {  // -- 0 --
    TDatedContainer * const pLeader = heads [hInd] ;

     {
	TL634AloneDatedContainer * pNewLead = new TL634AloneDatedContainer ;
//        kind2DatedContainer (heads [hInd] -> kind) ;

      (* pNewLead) << (* pLeader) ;
      chain.Add (pNewLead) ;
     }
    if (chain.Count () > 0)
	for (int i = 0 ; i < extensions.Count () ; i ++)
       {  // -- 1 --
        TDatedContainer * const pExtItem = extensions [i] ;

        if (ChainMatch (* pLeader, * pExtItem))
         {
          TL634AloneDatedContainer * pNewChainItem =
            new TL634AloneDatedContainer ;
//            kind2DatedContainer (pExtItem -> kind) ;

          (* pNewChainItem) << (* pExtItem) ;
          chain.Add (pNewChainItem) ;
         }
       }  // -- 1 --
    if (chain.Count () > 0)
      res = true ;
   }  // -- 0 --
  return res ;
 }  // TradeLaw634Book :: CompleteAChain

void TradeLaw634Book :: WriteOneLaw (const Trade634DlgData & rT634DD,
                                     TradeLaw634OutFile & outF)
 {
  TLawsuit law ;

  law.key = rT634DD.lawKey ;
  if (law.Get () && LawsuitApproach (law))
   {  // -- 0 --
    TGSArray <TL634AloneDatedContainer> heads ;
    TGArray <TL634AloneDatedContainer> extensions ;

    HeadsAndExtensionsOfChains (heads, extensions, law) ;

    const int chainsNum = heads.Count () ;
    long int sNo = rT634DD.begSerialNo ;

    if (chainsNum >= 1)
     {  // -- 1 --
      for (int hInd = 0 ; hInd < chainsNum ; hInd ++)
       {  // -- 2 --
        TGSArray <TL634AloneDatedContainer> eventsHeap ;

        if ((heads [hInd] -> date >= rT634DD.begD) &&
            (heads [hInd] -> date <= rT634DD.endD))
          if (CompleteAChain (heads, extensions, hInd, eventsHeap))
           {  // -- 3 --
            TGSArray <LinkedXKXKDatedContainerSK> sortableChain ;

            EventsHeapToChain (eventsHeap, sortableChain, law) ;
		RebuildEventsArray (sortableChain) ;

            const int items = sortableChain.Count () ;
            int startIndex = 0 ;

            // Seek the correct date first. -------------------
            while (startIndex < items &&
                   sortableChain [startIndex] -> ChainDate () <
                     rT634DD.begD
                  )
              startIndex ++ ;
            // ------------------------------------------------

            int forSyntaxUse ;

            if (TradeLaw634BookColumns ::
                     PreliminaryChecks (sortableChain,
                                        (forSyntaxUse = startIndex),
                                        rT634DD)
                  )
             {  // -- 4 --
              if (sNo == rT634DD.begSerialNo
                  && rT634DD.truncChainAtEndD
                 )
               {  // Title need.
                outF.title () ;
                outF.FormattedHead (& rT634DD) ;
                outF.head () ;
               }

              TradeLaw634BookColumns tL634BC (bookFormatFN, pParent, rT634DD,
                                              sortableChain, startIndex, sNo,
                                              outF.MinRowsTL634Chain ()) ;
              const int rows = tL634BC.getMaxRows () ;

              if (sNo != rT634DD.begSerialNo)
                outF.NextChainToPrint (rows + 1, rT634DD.truncChainAtEndD) ;
              if (rT634DD.truncChainAtEndD == false)
                outF.OneChainPrtStartFinalChar (true) ;
              outF.horzLine () ;
              for (int i = 0 ; i < rows ; i ++)
                outF.WriteItem (tL634BC, true) ;
              if (rT634DD.truncChainAtEndD == false)
                outF.OneChainPrtStartFinalChar (false) ;
              sNo ++ ;
             }  // -- 4 --
           }  // -- 3 --
       }  // -- 2 --
      outF.footherAndTail () ;
     }  // -- 1 --
    else                                                   // 30.may.2002
      info ("Липсват събития за отразяване в книгата.") ;  // 30.may.2002
   }  // -- 0 --
 }  // TradeLaw634Book :: WriteOneLaw

void TradeLaw634Book::PrintTradeLaw634Book(Trade634DlgData rT634DD)
{
	if (rT634DD.truncChainAtEndD == false)
		rT634DD.endD = rT634DD.begD;

	TradeLaw634OutFile of(pParent, bookFormatFN);

	{
		TWaitWindow ww(pParent, PLEASE_WAIT);
		WriteOneLaw(rT634DD, of);
	}

	if (of.pcount())
		show_text_stream_use_settings(NULL, of, bookFormatFN, FLOAT_ALIGNMENT);
}
#endif  // AREAL
