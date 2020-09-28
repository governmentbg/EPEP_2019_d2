#include "cac.h"  // 18.may.2001
//# include "all.h"

void addEmptyRowsIfDiffer (column * pC1, column * pC2)
 {
  int i1, i2 ;

  if (! (pC1 && pC2))
    return ;
  while ((i1 = pC1 -> getRowsCount ()) !=
         (i2 = pC2 -> getRowsCount ()))
    if (i1 < i2)
      pC1 -> addEmptyRow () ;
    else
      pC2 -> addEmptyRow () ;
 }  // addEmptyRowsIfDiffer

void adjust_column_rows (columnArray &cA, const unsigned int additionalRows)
 {
  size_t maxR = 0 ;
  int i ;
  const int items = cA.Count () ;

  for (i = 0 ; i < items ; i ++)
   {
    const column * pC = cA [i] ;

    if (pC)
     {
      const size_t cRCnt = pC -> getRowsCount () ;

      if (cRCnt > maxR)
        maxR = cRCnt ;
     }
   }
  maxR += additionalRows ;
  for (i = 0 ; i < items ; i ++)
   {
    column * pC = cA [i] ;

    if (pC)
      while (pC -> getRowsCount () < maxR)
        pC -> addEmptyRow () ;
   }
 }  // adjust_column_rows

void addKindToColumn (char k, column * pC)
 {
  size_t sSz ;
  char * s ;

  if (! (k && pC))
    return ;
   {
    TKind tK ;

    sSz = sizeof (tK.name) ;
   }
  if ((s = new char [sSz]) != NULL)
   {
    kindToString (k, s, sSz) ;
    pC -> addText (s) ;
    delete [] s ;
   }
 }  // addKindToColumn

static constant
	// 2010:090 LRQ: +a for APPEAL, ADMIN, AREAL
#if COURT_TYPE == COURT_APPEAL
	CAT_DEC_RESULT_TYPES	= "cta",
	CAT_DEC_RESULT_KINDS	= KIND_LAWSUITS;
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	CAT_DEC_RESULT_TYPES	= "ca",
	CAT_DEC_RESULT_KINDS	= KIND_2ND_LAWSUITS;
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	CAT_DEC_RESULT_TYPES	= "cta",
	CAT_DEC_RESULT_KINDS	= KIND_2ND_LAWSUITS;
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	CAT_DEC_RESULT_TYPES	= "c",
	CAT_DEC_RESULT_KINDS	= KIND_2ND_LAWSUITS;
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	CAT_DEC_RESULT_TYPES	= "",
	CAT_DEC_RESULT_KINDS	= "";
#endif  // REGIONAL

// 2010:027 LPR: +related, replaces print date calls
void cat_date_result(char *s, TDecision & dec, size_t size)
{
	mstr m;
	dec.fromDate.Print(m, " от %D");
	strConcat(s, str(m), size);

	if (strchr(CAT_DEC_RESULT_TYPES, dec.key.type))
	{
		TLawsuit lawsuit;
		lawsuit.key = dec.key;

		if (lawsuit.Get("F_KIND") && strchr(CAT_DEC_RESULT_KINDS, lawsuit.kind))
		{
			strConcat(s, " - ", size);
			strConcat(s, Result->Seek(dec.result), size);
		}
	}
}

void addDecisionKindToColumn (column * pColumn, TDecision & dec,
					const bool addKind)
 {
  if (pColumn == NULL)
    return ;
  if (dec.PersonalData ())     //Ако има индидидуални части на присъдата 20050401
   {
   if (timeBookColumns :: showDecisionNumber ())
     {
	if (dec.eventNo)  // dec.decisionNo  // 27.dec.2004
	 {
	  const size_t sSz = sizeof (Kind -> name) + 128 ;	// 2010:014 LPR: 64 -> 128
	  char * s = new char [sSz] ;

	  kindToString (dec.kind, s, sSz) ;

	  const size_t sL = strlen (s) ;

	  if (sSz > (sL + 1))
	    scSprintf (sSz - sL, s + sL, " No %ld",
			   dec.eventNo  // dec.decisionNo  // 27.dec.2004
			   ) ;
	  cat_date_result(s, dec, sSz);
	  pColumn -> addText (s) ;
	  delete [] s ;
	 }
	else
	    if (addKind)
	    {
		  // 2010:014 LPR: altered
		  char s[128];
		  kindToString(dec.kind, s, sizeof(s));
		  strcpy(s, Kind->Seek(dec.kind));
		  cat_date_result(s, dec, sizeof s);
	    }
     }
   else
     {
	    // 2010:014 LPR: altered
	    char tmp[128];
	    *tmp = '\0';

	    if (addKind)
		kindToString(dec.kind, tmp, sizeof(tmp));
	    if((dec.eventNo > 0)&& // 20060321 ->
		 PRINT_JUDGEMENTS) //20050401
	    {
		 strConcat(tmp, " No ", sizeof(tmp));
		 strConcatLong(tmp, dec.eventNo, sizeof(tmp));
	    }                      // 20060321 <-
	    cat_date_result(tmp, dec, sizeof tmp);
	    pColumn->addText(tmp);
     }
   }
  else
    if (addKind)
    {
	    //20050401 -->

	    // addKindToColumn (dec.kind, pColumn) ;

	    char tmp[128];	// 2010:014 LPR: 64 -> 128

	    kind_to_alias(dec.kind, tmp, sizeof(tmp));
	    if((dec.eventNo > 0)&&
		 PRINT_JUDGEMENTS) //20050401
	    {
		 strConcat(tmp, " No ", sizeof(tmp));
		 strConcatLong(tmp, dec.eventNo, sizeof(tmp));
	    }
	    cat_date_result(tmp, dec, sizeof tmp);
	    pColumn->addText(tmp);
	    //20050401 <--
    }

 }  // addDecisionKindToColumn

void addResultToColumn (char r, column * pC)
 {
  size_t sSz ;
  char * s ;

  if (! (r && pC))
    return ;
   {
    sSz = sizeof Result->name ;
   }
  if ((s = new char [sSz]) != NULL)
   {
    if (resultToString (r, s, sSz))
      pC -> addText (s) ;
    delete [] s ;
   }
 }  // addResultToColumn

void addDefNameAndSettlToColumn (const TDefendant & defendant,
                                 column * const pColumn)
 {
  if (! pColumn)
    return ;

  char tmp [sizeof (defendant.dName) + 2 +
            sizeof (defendant.dSettlement)] ;

  ostrstream (tmp, sizeof (tmp))
    << defendant.dName << ", "
    << defendant.dSettlement << ends ;
  tmp [sizeof (tmp) - 1] = '\0' ;
  pColumn -> addText (tmp) ;
 }  // addDefNameAndSettlToColumn

void add_decision_to_column (TDecision & dec,
                             column * pColumn,
                             timeBookHeap * pTBH,
                             const bool addKind, const bool addResult
# if INSTANCE
#   if APPEAL
                                                                     ,
                             const char, const char
#   else  // of APPEAL
                                                                     ,
                             const char lawKind, const char sessKind
#   endif  // of APPEAL
# endif  // of INSTANCE
                             , const char sessResult,   // To be printed?
                             const char sessResToCount  // To be accumulated ?
                            )
 {
  if (! pColumn)
    return ;
//  if (addKind)  // 21.feb.2001 - begin
//    addKindToColumn (dec.kind, pColumn) ;

  addDecisionKindToColumn (pColumn, dec, addKind) ;  // 21.feb.2001 - end



  if (sessResult &&
      (dec.key.type == TYPE_PUNISHMENT_LAW ||
       (strchr (KIND_DECISIONS_BY_NAME, dec.kind) == NULL &&
        strchr (KIND_DEFINITIONS_BY_NAME, dec.kind) == NULL  // 24.sep.2001
       )
      )
     )
    addResultToColumn (sessResult, pColumn) ;
  if (addResult)
# if INSTANCE
#   if APPEAL  // APPEAL ->
    addResultToColumn (dec.result, pColumn) ;
               // APPEAL <-
#   else       // AREAL  ->
    if (lawKind && sessKind &&
        (strchr (KIND_2ND_LAWSUITS, lawKind) ||
//         sessKind == KIND_CLOSED_SESSION  // 12.jan.2001
         strchr (KIND_CLOSED_SESSIONS, sessKind)  // 12.jan.2001
        )
       )
     {
	Result->result = dec.result ;
	if (Result->Get () && strncmp (Result->name, str(dec.text), strlen (Result->name)))
	  pColumn -> addText (Result->name) ;
     }
    else
      addResultToColumn (dec.result, pColumn) ;
#   endif      // AREAL  <-
# else         // REGIONAL  ->
      addResultToColumn (dec.result, pColumn) ;
# endif        // REGIONAL  <-
  if (! strIsBlank (dec.text))
    pColumn -> addText (dec.text) ;
  if (pTBH)
    markDecGlJResInTBHeap (dec.result, sessResToCount, (* pTBH)) ;

// pColumn -> addText ("CHECK_THIS") ;

 }

bool addDecisionToColumn (const TRCDKey & key, const CDate & date,
                          const char sessKind,  // 15.jan.2001
				  column * pColumn, timeBookHeap * pTBH,
                          const char sessResult)
 {
  if (! pColumn)
    return false ;
   {
    DecPtr pDec ;
    SessionEndDocs endDocs (key, date, sessKind) ;

    const bool res = (endDocs.DocsFound () > 0) ;

    if (res)
     {
      while (endDocs.NextDoc (* pDec))
       {
# if INSTANCE
        if (key.type != TYPE_FIRM_LAW)
          add_decision_to_column (* pDec, pColumn, pTBH, true, false,
                                  '\0', '\0', sessResult) ;
        else
          add_decision_to_column (* pDec, pColumn, pTBH, false, true) ;
# else
          add_decision_to_column (* pDec, pColumn, pTBH, true, false,
                                  sessResult) ;
# endif
       }
     }
    else
      if (sessResult)
# if INSTANCE
# else
        if (sessResult != RESULT_BOZA_LIKE_0)   // 20070212
# endif
          addResultToColumn (sessResult, pColumn) ;
    return res ;
   }
 }  // addDecisionToColumn

bool addLocalJudgemToColumn (const TRCDKey & key, const CDate & date,
                             const char ucnType, const char * ucn,
                             const char involvement,  // 20060413
                             column * pColumn, const char decisKind,
                             accusedsDistribution * pAccDistr,
                             bool * pNotMajor
                            )
 {
  if (! pColumn)
    return false ;
   {
    JudgPtr pJudg ;  // 13.jan.2004
    TJudgement & lJ = pJudg.Ref () ;
    const bool res =
      extractLocalJudgement (key, date, ucnType, ucn,
                             involvement,  // 20060413
                             lJ,
                             decisKind  // 15.jan.2001
                            ) ;

    if (res)
     {
      if (! strIsBlank (lJ.text))
        pColumn -> addText (lJ.text) ;
      if (pAccDistr)
        markInDistribution (lJ, (* pAccDistr)) ;
      {
        //if (pNotMajor && lJ.old == OLD_NOT_MAJOR)
        if (pNotMajor && (lJ.flags & SIDE_WIND_CHILD))
          * pNotMajor = true ;
      }
     }
    return res ;
   }
 }  // addLocalJudgemToColumn

void addNextSessionToColumn (column * pColumn,
                             char * buff, const size_t buffL,
                             const TSession & nextSess,
                             const bool addComposition)
 {
  if (pColumn && buff && buffL)
   {
    char d [16] ;
    char t [16] ;
    char c [16] ;
    ostrstream oss (buff, buffL) ;

    CDateToString (nextSess.date, d, sizeof (d)) ;
    CTimeHMToString (nextSess.time, t, sizeof (t)) ;
    compositionToString (nextSess.composition, c, sizeof (c)) ;
    oss
      << "Наср. за " << d << "г. от " << t << "ч." ;

// 20070806    if (addComposition)
// 20070806      oss << ", " << c << " състав" ;
    if ((addComposition) && (use_area_compositions ()))
        oss << ", " << c << " състав" ; // 20070806

    oss << ends ;
    buff [buffL - 1] = '\0' ;
    pColumn -> addText (buff) ;
   }
 }  // addNextSessionToColumn

void addCancLeftStopReasonToColumn (column * pColumn,
                                    const TSession & sess,
                                    TLawsuit &
# if INSTANCE  // 17.jan.2002 - begin
#   if APPEAL
                                              ,
#   else  // of APPEAL
                                               lawsuit,
#   endif  // of APPEAL
# else  // of INSTANCE  // 17.jan.2002 - end
                                               lawsuit,
# endif  // of INSTANCE
                                    bool catchOtherEndingResults,
                                    const TSession * pNextSess
# if INSTANCE
# else
                                    , const bool addComposit
# endif
                                   )
 {
  bool match ;

  if (! pColumn)
    return ;
  if (! (match = (strchr (RESULT_RETURNEDS, sess.result) != NULL)))
    if (! (match = (sess.result == RESULT_SEND)))
      if (! (match = (strchr (RESULT_CANCELEDS, sess.result) != NULL)))
	  if (! (match = (strchr (RESULT_LEFTS, sess.result) != NULL)))
          if (! (match = (strchr (RESULT_UNMOVEABLES, sess.result)
                 != NULL)))
            if (! (match = (strchr (RESULT_STOPPEDS, sess.result)
                   != NULL)))
              if (! (match = (sess.result == RESULT_DEFINITION)))
                match = match ;
// 23.jun.2000 - begin
  if (match && sess.result == RESULT_WITH_BULWARK)
    match = false ;
// 23.jun.2000 - end
  if (match)
   {  // -- 0 --
     {  // -- 1 --
      bool decKindAdded = false ;
      bool sessResAdded = false ;
      bool sessTextAdded = false ;
      SessionEndDocs endDocs (sess.key, sess.date, sess.kind) ;
      DecPtr pDec ;
      bool decFound = endDocs.NextDoc (* pDec) ;

      do
       {  // -- 2 --
        if (catchOtherEndingResults)
          if (decKindAdded == false)
            if (decFound)
             {
# if INSTANCE
#   if APPEAL
              if (sess.key.type != TYPE_PUNISHMENT_LAW)
#   else  // of APPEAL
              if ( (! strchr (KIND_2ND_PUNISHMENT_LAWS, lawsuit.kind))
                   && // 20060405 added
                   (! (sess.key.type == TYPE_PUNISHMENT_LAW && strchr(RESULT_REENTERABLES, sess.result)))
                 )
#   endif  // of APPEAL
# else  // of INSTANCE
          //   if (sess.key.type != TYPE_PUNISHMENT_LAW)    // 20060330 in comment replaced with :
               if (! (sess.key.type == TYPE_PUNISHMENT_LAW && strchr(RESULT_REENTERABLES, sess.result)))
# endif  // of INSTANCE
          //    addKindToColumn (pDec.Ref ().kind, pColumn) ; // 20060405 in comment
          //    20060405  replaced with ->
               {
                char tmp[64];
                kind_to_alias(pDec.Ref ().kind, tmp, sizeof(tmp));
                if((pDec.Ref ().eventNo > 0)&&
			 PRINT_JUDGEMENTS)
                {
			strConcat(tmp, " No ", sizeof(tmp));
                  strConcatLong(tmp, pDec.Ref ().eventNo, sizeof(tmp));
                }
                pColumn -> addText (tmp) ;
          //    20060405  replaced with <-
                decKindAdded = true ;
               } 
             }

        const bool judgNeed = (catchOtherEndingResults &&
                               resMustBeTreatAsDecision (sess.result))
# if INSTANCE
                             || RESULT_LEFT_1_PHASE_DECISION == sess.result
# endif
                             ;
        const bool resNeed = catchOtherEndingResults ||
                             ! resMustBeTreatAsDecision (sess.result) ;

        if (sessResAdded == false)
          if (resNeed
// 20060405 in comment -->
// 20060405 in comment                    && (! strchr (RESULT_FINALIZEDS, sess.result) ||
// 20060405 in comment                        strchr (KIND_CLOSED_SESSIONS, sess.kind) ||
// 20060405 in comment                        sess.key.type != TYPE_PUNISHMENT_LAW
                           ||
# if INSTANCE
#   if APPEAL
                      false
#   else  // of APPEAL
				strchr (KIND_1ST_PUNISHMENT_LAWS, lawsuit.kind) ||
				lawsuit.kind == KIND_1ST_ADMIN_PUNISHMENT_LAW ||
				lawsuit.kind == KIND_2ND_ADMIN_PUNISHMENT_LAW
#   endif  // of APPEAL
# else
				lawsuit.kind == KIND_ADMIN_PUNISHMENT_LAW
# endif
// 20060405 in comment                       )
		 )
	   {
		addResultToColumn (sess.result, pColumn) ;
		sessResAdded = true ;
	   }
	  if (sessTextAdded == false)
	    if (! strIsBlank (sess.text))
	     {
		pColumn -> addText (sess.text) ;
		sessTextAdded = true ;
	     }
	  if (decFound)
	  {
	    if (judgNeed)
		add_decision_to_column (pDec.Ref (), pColumn, NULL, false) ;
	    // 2015:119 TRQ: HOLD with ENDOC hack; 2015:120 reverted
	    //else if (sess.result == RESULT_HOLD && ! strIsBlank (pDec.Ref ().text))
	    //{
	    //pColumn -> addText (pDec.Ref ().text) ;
	    //sessTextAdded = true;
	    //}
	  }
	  // 2015:119
	 }  // -- 2 --
	while ((decFound = endDocs.NextDoc (* pDec)) == true) ;
     }  // -- 1 --
    if (pNextSess)
     {
      char b [80] ;

      addNextSessionToColumn (pColumn, b, sizeof (b), * pNextSess
# if INSTANCE
# else
                                , addComposit
# endif
                                ) ;
     }
   }  // -- 0 --
 }  // addCancLeftStopReasonToColumn

void addDateFinishedToColumn (column * pColumn,
                              char * buff, const size_t buffL,
                              const TLawsuit & lawsuit,
                              const bool addEmptyR)
 {
  if (! (buff && buffL))
    return ;
    
  ostrstream oss (buff, buffL) ;

  oss << "Дата на свършване: " ;
  if (lawsuit.finished.year && lawsuit.finished.month &&
      lawsuit.finished.day)
    oss << lawsuit.finished ;
  else
    oss << "необявена" ;
  oss << ends ;
  if (addEmptyR)
    pColumn -> addEmptyRow () ;
  pColumn -> addText (buff) ;
 }  // addDateFinishedToColumn

void addLongToColumn (column * pColumn, const long int n,
			    const bool includingZero)
 {
  if (pColumn)
    if (n || includingZero)
     {
	char s [32] ;

	scSprintf (sizeof (s), s, "%ld", n) ;
	pColumn -> addText (s) ;
     }
 }  // addLongToColumn

void addSubjCodeToColumn(column *pColumn, long subject, char lawKind)
{
	if (pColumn)
		pColumn->addText(TSubject::Code(subject, lawKind));
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
void addRelatedLawsToColumn (column * const pColumn,
                             const TLawsuit & rLawsuit,
                             char * const buff,
                             const size_t buffSz,
                             const bool useOnPrefix  // 15.nov.2001
                            )
 // Adds info for DECISIV - laws
 {
  if (! (buff && buffSz && pColumn))
    return ;

  const size_t buffL = buffSz - 1 ;

  buff [buffL] = '\0' ;
  if (buffL == 0)
    return ;

  ostrstream oss (buff, buffL) ;

  // 20070103  -->
  {
          char decisiv_acts[8] = {
            	CONNECT_TYPE_JURISDICTION, //	= 'J',        // Дело по подсъдност
            	CONNECT_TYPE_RENEWED_LAWSUIT, //	= 'N',    // Възобновено дело
            	CONNECT_TYPE_RERETURNED_LAWSUIT, //	= 'U',  // Внесено след доразследване
            	CONNECT_TYPE_RETURNED_LAWSUIT, //	= 'R',    // Върнато дело
            	CONNECT_TYPE_LOWER_INSTANCE, //	= 'L',      // Първоинстанционно дело
            	CONNECT_TYPE_HIGHER_INSTANCE, //	= 'H',    // Дело на висша инстанция
              CONNECT_TYPE_RECALL_LAWSUIT, //	= 'B'       // Въззивно дело  // 20070926
		  '\0'};

	TGArray<TConnect> connects;
	    TConnect *connect = new TConnect;
      TLawsuit * pLawsuit = new TLawsuit ;
      pLawsuit -> key = rLawsuit.key;

      TSender s ;
      char tmp_law_kind [SIZE_OF_NAME + SIZE_OF_REQUEST_TEXT ] ;

	    if (load_connects(pLawsuit, &connects))
    	{
		    for (int i = 0; i < connects.Count(); i++)
		    {
          tmp_law_kind [0] = '\0' ;
          clearGroupData (s) ;
          *connect << connects[i];

          ConnectType->Seek(connect -> connectType);

			    if (strchr ( decisiv_acts, connect -> connectType) )
			    {
             if (( connect -> ucnType )&&( connect -> ucn ))
             { // get sender name
                senderUCNAndUCNTypeToName (connect -> ucn , connect -> ucnType,
                                           s.name, sizeof (s.name) );
             }
             if (   ( connect -> connectNo && connect -> connectNo[0] )
                  ||  connect -> connectYear || s.name [0])
             {  // connected law kind
		    connectKindToString (connect -> connectKind , tmp_law_kind, sizeof (tmp_law_kind)) ;
		 }

             // compose output
             if(  (connect -> connectType == CONNECT_TYPE_LOWER_INSTANCE)//Първоинстанционно дело
                ||(connect -> connectType == CONNECT_TYPE_RECALL_LAWSUIT)//Въззивно дело // 20070926
               )
             {
                oss << (useOnPrefix ? " По " : " ");
                if (tmp_law_kind[0])
		    {
                   if (connect -> connectType == CONNECT_TYPE_RECALL_LAWSUIT )// 20070926
                      oss << "Въззивно ";
                   oss << tmp_law_kind ;
                }
                else
                   oss << "дело";
             }
             if(  (connect -> connectType == CONNECT_TYPE_HIGHER_INSTANCE)//Дело на висша инстанция
                ||(connect -> connectType == CONNECT_TYPE_RERETURNED_LAWSUIT)//Внесено след доразследване
               )
             {
                if (useOnPrefix) oss << ConnectType -> name << " ";
                if (tmp_law_kind[0]) oss << tmp_law_kind ;
                else                 oss << " дело";
             }

             if(  (connect -> connectType == CONNECT_TYPE_JURISDICTION)//Дело по подсъдност
                ||(connect -> connectType == CONNECT_TYPE_RENEWED_LAWSUIT)//Възобновено дело
                ||(connect -> connectType == CONNECT_TYPE_RETURNED_LAWSUIT)// Върнато дело
		   )
             {
                if (useOnPrefix) oss << ConnectType -> alias << " ";
                if (tmp_law_kind[0]) oss << tmp_law_kind ;
                else                 oss << "дело";
             }

             if ( connect -> connectNo && connect -> connectNo[0] )
                 oss << " No " << connect -> connectNo ;
             if (connect -> connectYear)
                     oss << "/" << connect -> connectYear <<  " г." ;
		 if (connect -> composition)
		 {
		     char comp_t [SIZE_OF_NAME + SIZE_OF_REQUEST_TEXT ] ;
		     comp_t [0] = '\0' ;
		     if (use_area_compositions ()) // 20070806
		     {
			  // 2012:009
			  //compositionToString (connect -> composition, comp_t, sizeof (comp_t)) ;
			  strcpy(comp_t, connect->CompositionName());
                    oss << " " << comp_t << " състав " ;
                 }
		 }
             if (s.name [0])
                    oss << " " << s.name ;

             if (connect-> text[0])
                    oss << ", " << connect-> text ;

             oss << ends ;

             if (buff [0])
                     pColumn -> addText (buff) ;

             oss.clear () ;
             oss.seekp (0) ;
      } } }
      delete connect;
      delete pLawsuit;
  }
  // 20070103 <--

  oss.clear () ;
  oss.seekp (0) ;

#if 20160060
// 9000 was the compiled punishment subject?
#else  // 20160060
// Add subject  -->
#if COURT_TYPE == COURT_APPEAL
  if (rLawsuit.key.type == TYPE_PUNISHMENT_LAW &&
	rLawsuit.subject != SUBJECT_9000)
   {
    TSubject subj ;

    subj.subject = rLawsuit.subject ;
    subj.type = rLawsuit.key.type ;
    if (subj.Get ()
 //                      && subj.kind != TYPE_PUNISHMENT_LAW
	 )
	oss << subj.name ;
   }
#endif  // APPEAL
#endif  // 20160060

  // Add law text -->
  if (rLawsuit.text [0])
    oss << " " << rLawsuit.text ;
  oss << ends ;
  if (buff [0])
    pColumn -> addText (buff) ;

}  // addRelatedLawsToColumn


void addRelated_1st_Inst_LawsToColumn (column * const pColumn,
                             const TLawsuit & rLawsuit,
                             char * const buff,
                             const size_t buffSz,
                             const bool useOnPrefix  // 15.nov.2001
                            )
 {
  if (! (buff && buffSz && pColumn))
    return ;

  const size_t buffL = buffSz - 1 ;

  //buff [buffL] = '\0' ;
  buff [0] = '\0' ;
  if (buffL == 0)
    return ;

  ostrstream oss (buff, buffL) ;
  oss.clear () ;
  oss.seekp (0) ;

#if COURT_TYPE == COURT_AREAL
  if (strchr (KIND_2ND_LAWSUITS, rLawsuit.kind))
   {
    if (lawIsCocern3rdInst (rLawsuit))
	oss << "К" ;                  // касационно
    else
	oss << "В" ;                  // въззивно
   }
  if (law_is_pre_court (rLawsuit))
    oss << "Д" ;                    // досъдебно производство
  {
     TLawsuit * pLawsuit = new TLawsuit ;
     pLawsuit -> key = rLawsuit.key;
     // AREA    *CONNECT_TYPE_DECISIVES		= "JNUHL",
     TConnect *c = main_connect( pLawsuit );
     if ( c != NULL )
     {
	  if ( c -> connectType == CONNECT_TYPE_JURISDICTION )       //'J'
	    oss << "П" ;          // Изпратено по посъдност
	  if ( c -> connectType == CONNECT_TYPE_RENEWED_LAWSUIT )    //'N'
	    oss << "Възобн" ;     // възобновено
	  if ( c -> connectType == CONNECT_TYPE_RERETURNED_LAWSUIT ) //'U'
	    oss << "ВСДР" ;       // внесено след доразследване
	  if ( c -> connectType == CONNECT_TYPE_HIGHER_INSTANCE )    //'H'
	    oss << "НР" ;         // Върнато за ново разглеждане == Върнато дело
	  //Първоинстанционно дело
	  //if ( c -> connect.type == CONNECT_TYPE_LOWER_INSTANCE )   //'L'
     }
     delete c;
     delete pLawsuit;
  }
#endif  // AREAL  <--

// add source kind -  20070428 --> APP
  if (oss.tellp ())  // > 0
    oss << " " ;
   {
    TKind k ;

    k.kind = rLawsuit.sourceKind ;
    if (k.Get ())  // Here it (the indoc) is !!!
	oss << k.name ;
   }
  oss << ends ;
// add source kind -  20070428 <-- APP
  if (buff [0])
    pColumn -> addText (buff) ;

  oss.clear () ;
  oss.seekp (0) ;

  // 20070215  --> // само Първоинстанционно дело
  {
    	TGArray<TConnect> connects;
	    TConnect *connect = new TConnect;
      TLawsuit * pLawsuit = new TLawsuit ;
      pLawsuit -> key = rLawsuit.key;

      TSender s ;
      char tmp_law_kind [SIZE_OF_NAME + SIZE_OF_REQUEST_TEXT ] ;

	    if (load_connects(pLawsuit, &connects))
    	{
		    for (int i = 0; i < connects.Count(); i++)
		    {
          tmp_law_kind [0] = '\0' ;
          clearGroupData (s) ;
          *connect << connects[i];

          ConnectType->Seek(connect -> connectType);

			    if (  (connect -> connectType == CONNECT_TYPE_LOWER_INSTANCE ) //	= 'L',
              ||(connect -> connectType == CONNECT_TYPE_RECALL_LAWSUIT ) //	= 'B'  // Въззивно дело  // 20070926
             )
			    {
             if (( connect -> ucnType )&&( connect -> ucn ))
             { // get sender name
                senderUCNAndUCNTypeToName (connect -> ucn , connect -> ucnType,
                                           s.name, sizeof (s.name) );
             }
             if (   ( connect -> connectNo && connect -> connectNo[0] )
                  ||  connect -> connectYear || s.name [0])
             {  // connected law kind
                connectKindToString (connect -> connectKind , tmp_law_kind, sizeof (tmp_law_kind)) ;
             }

             // compose output
             if(  (connect -> connectType == CONNECT_TYPE_LOWER_INSTANCE)//Първоинстанционно дело
                ||(connect -> connectType == CONNECT_TYPE_RECALL_LAWSUIT)
                )
             {
		    oss << (useOnPrefix ? " По " : " ");
                if (tmp_law_kind[0])
                {
                   if (connect -> connectType == CONNECT_TYPE_RECALL_LAWSUIT )
                      oss << "Въззивно ";
                   oss << tmp_law_kind ;
                }
                else  oss << "дело";
             }

             if ( connect -> connectNo && connect -> connectNo[0] )
                 oss << " No " << connect -> connectNo ;
             if (connect -> connectYear)
                     oss << "/" << connect -> connectYear <<  " г." ;
             if (connect -> composition)
             {
		     char comp_t [SIZE_OF_NAME + SIZE_OF_REQUEST_TEXT ] ;
		     comp_t [0] = '\0' ;
		     if (use_area_compositions ())  // 20070806
		     {
			  // 2012:009
			  //compositionToString (connect -> composition, comp_t, sizeof (comp_t)) ;
			  strcpy(comp_t, connect->CompositionName());
			  oss << " " << comp_t << " състав " ;
		     }
             }
             if (s.name [0])
                    oss << " " << s.name ;

             // if (connect-> text[0])
             //       oss << ", " << connect-> text ;

             oss << ends ;

             if (buff [0])
                     pColumn -> addText (buff) ;

             oss.clear () ;
             oss.seekp (0) ;
      } } }
      delete connect;
      delete pLawsuit;
  }
  // 20070215 <--

  oss.clear () ;
  oss.seekp (0) ;

// Add subject  -->
#if COURT_TYPE == COURT_APPEAL
  if (rLawsuit.key.type == TYPE_PUNISHMENT_LAW &&
	rLawsuit.subject != SUBJECT_9000)
   {
    TSubject subj ;

    subj.subject = rLawsuit.subject ;
    subj.type = rLawsuit.key.type ;
    if (subj.Get ()
 //                      && subj.kind != TYPE_PUNISHMENT_LAW
	 )
	oss << subj.name ;
    if (buff [0])
	 pColumn -> addText (buff) ;
   }
#endif  // APPEAL

// Add law text -->
//  if (rLawsuit.text [0])
//   oss << " " << rLawsuit.text ;
//  oss << ends ;

}  // addRelated_1st_Inst_LawsToColumn

void addInDocAndRelatedLawsToColumn (column * pColumn,
                                     const TLawsuit & rLawsuit,
                                     char * const buff,
                                     const size_t buffSz)
{
  if (! (buff && buffSz && pColumn))
    return ;

  const size_t buffL = buffSz - 1 ;

  buff [buffL] = '\0' ;
  if (buffL == 0)
    return ;

  ostrstream oss (buff, buffL) ;

#if COURT_TYPE == COURT_AREAL
  if (strchr (KIND_2ND_LAWSUITS, rLawsuit.kind))
   {
    if (lawIsCocern3rdInst (rLawsuit))
      oss << "К" ;                  // касационно
    else
      oss << "В" ;                  // въззивно
   }
  if (law_is_pre_court (rLawsuit))
    oss << "Д" ;                    // досъдебно производство

  // 20070116 -->
  {
     TLawsuit * pLawsuit = new TLawsuit ;
     pLawsuit -> key = rLawsuit.key;
     // AREA    *CONNECT_TYPE_DECISIVES		= "JNUHL",
     // APPEAL	*CONNECT_TYPE_DECISIVES		= "NUHL",
     //       	*CONNECT_TYPE_RERETURNEDS	= "UH",      APPEAL_+ AREA + REGION
     TConnect *c = main_connect( pLawsuit );
     if ( c != NULL )
     {
        if ( c -> connectType == CONNECT_TYPE_JURISDICTION )       //'J'
          oss << "П" ;          // Изпратено по посъдност
        if ( c -> connectType == CONNECT_TYPE_RENEWED_LAWSUIT )    //'N'
          oss << "Възобн" ;     // възобновено
        if ( c -> connectType == CONNECT_TYPE_RERETURNED_LAWSUIT ) //'U'
          oss << "ВСДР" ;       // внесено след доразследване
        if ( c -> connectType == CONNECT_TYPE_HIGHER_INSTANCE )    //'H'
          oss << "НР" ;         // Върнато за ново разглеждане == Върнато дело
        //Първоинстанционно дело
	  //if ( c -> connect.type == CONNECT_TYPE_LOWER_INSTANCE )   //'L'
     }
     delete c;
     delete pLawsuit;
  }
  // 20070116 <--

#endif  // AREAL  <--

// add source kind -
  if (oss.tellp ())  // > 0
    oss << " " ;
   {
    TKind k ;

    k.kind = rLawsuit.sourceKind ;
    if (k.Get ())  // Here it (the indoc) is !!!
      oss << k.name ;
   }
  oss << ends ;

  if (buff [0])
    pColumn -> addText (buff) ;

  oss.clear () ;
  oss.seekp (0) ;

  addRelatedLawsToColumn (pColumn, rLawsuit, buff, buffSz) ;

}  // addInDocAndRelatedLawsToColumn

#endif  // APPEAL || ADMIN || AREAL || MILITARY


void addCompositionToColumn (const long int c, column * const pColumn)
{
  // 20060203 ->
  if ( ( c ) &&
	 ( use_area_compositions () )
     )   // 20060203 <-
  {
     char buff [32] ;
     const size_t buffL = sizeof (buff) - 1 ;

     if (pColumn)
     {
	 buff [buffL] = '\0' ;
	 compositionToString (c, buff, buffL) ;
       ostrstream (buff + strlen (buff), buffL - strlen (buff))
         << " състав" << ends ;
       pColumn -> addText (buff) ;
     }
  }  // addCompositionToColumn
}
void addAgeToColumn (const char age, column * const pColumn)
 {
  if (pColumn)
   {
    const char * t = STAR_STRING ;
    switch (age)
     {
      case AGE_THREE_OR_LESS :
        t = "до 3м." ;
        break ;
      case AGE_MORE_THAN_THREE :
        t = "над 3м." ;
        break ;
// 31.jul.2000 - begin
      case AGE_ONE_OR_LESS :
        t = "до 1м." ;
        break ;
      case AGE_ONE_TO_THREE :
        t = "от 1 до 3м." ;
        break ;
// 31.jul.2000 - end
// 20050318 - begin
      case AGE_THREE_TO_SIX	:
        t = "от 3 до 6м." ;
      break ;
      case AGE_SIX_TO_TWELVE :
        t = "от 6 до 1год." ;
      break ;
      case AGE_MORE_THAN_TWELVE	:
        t = "над 1год." ;
      break ;
// 20050318 - end
     }
    pColumn -> addText (t) ;
   }
 }  // addAgeToColumn

# if INSTANCE
void add_dec_res_damn_name_to_column (column * pColumn,
                                      TLawsuit & lawsuit,
                                      char * const buff,
                                      const size_t buffSz,
						  const TDecision * const pDec,
						  const bool decFound,
						  const TSession & sess,
						  const bool addDesTxt
						 ) // 23.jun.2000
 {
  if (! (pColumn && buff && buffSz > 1))
    return ;

  const size_t buffL = buffSz - 1 ;
  TDamn firm ;
  TCompanyType firmType ;
  TNTResult decRes ;
  TNTResult sessRes ;
  const bool firmFound = lawsuitToDamn (& lawsuit, & firm) ;
  bool firmTypeFound = false ;
  bool decResFound = false ;
  bool sessResFound = false ;

  if (firmFound)
   {
    firmType.companyType = firm.companyType ;
    firmTypeFound = firmType.Get () ;
   }
  if ((pDec -> result == RESULT_DEFINITION_READY))
   {
    sessRes.result = sess.result ;
    sessResFound = sessRes.Get () ;
   }
  else
    if (pDec && decFound)
     {
	decRes.result = pDec -> result ;
	decResFound = decRes.Get () ;
     }
  ostrstream (buff, buffL)
//20050322 ->
    << ((pDec -> result == RESULT_DEFINITION_READY) ?
	    (sessResFound ? sessRes.name : "") :
	    (decResFound ? decRes.name : "")
	 )
//20050322 <-
							  << " "
    << (firmTypeFound ? firmType.name : "") << " \n"
    << (firmFound ? firm.name : "") << ends ;
  pColumn -> addText (buff) ;

//20050322 ->
//  addKindToColumn (pDec -> kind,  pColumn);
//20050322 <-

  if (addDesTxt)
    if (any(pDec -> text))
	pColumn -> addText (pDec -> text) ;

 }  // add_dec_res_damn_name_to_column

# endif

void add_resolution_name_to_column (const char reso, column * pColumn)
 {
  if (pColumn && reso && reso != RESOLUTION_FAKE)
   {
    TResolution resol ;

    resol.resolution = reso ;
    if (resol.Get ())
	pColumn -> addText (resol.name) ;
   }
 }  // add_resolution_name_to_column (const char reso, column * pColumn)

void add_resolution_name_to_column (char * const buff,
                                    const size_t buffSz,
                                    const char * const prefix,
                                    const char reso, column * pColumn)
 {
//  if (! pColumn && reso && reso != RESOLUTION_FAKE)  // 09.jan.2002-begin
//    return ;
  if (pColumn && reso && reso != RESOLUTION_FAKE)
   {  // 09.jan.2002 - end
    if (buff && buffSz > 1 && prefix && prefix [0])
     {
      ostrstream oss (buff, buffSz) ;

      oss << prefix << ": " ;
       {
        TResolution resol ;

        resol.resolution = reso ;
        if (resol.Get ())
          oss << resol.name ;
       }
      oss << ends ;
      buff [buffSz - 1] = '\0' ;
      pColumn -> addText (buff) ;
     }
    else
      add_resolution_name_to_column (reso, pColumn) ;
   }  // 09.jan.2002
 }  // add_resolution_name_to_column (char * const buff,
    //                                const size_t buffSz,
    //                                const char * const prefix,
    //                                const char reso, column * pColumn)

void add_preliminaryActions_to_column (column * pColumn,
                                       char * const buff,
						   const size_t buffSz,
                                       const TLawsuit & law)
 {  // Attention! Keep the logic of this function in
    // conformity with the code of preliminary_actions_to_ostream.

  if (! (pColumn && buff && buffSz))
    return ;

  buff[0] = '\0';

  // 20070103 -->
	TGArray<TConnect> connects;
  TConnect *connect = new TConnect;
  TLawsuit *pLaw = new TLawsuit;
  pLaw -> key = law.key;

 	if (load_connects(pLaw, &connects))
  {
		  for (int i = 0; i < connects.Count(); i++)
		  {
			   *connect << connects[i];

	   if ( strchr ( CONNECT_TYPE_PRELIMITARY_ACTS, connect -> connectType ))
	   {
		buff [0] = '\0' ;
		// 20070201
		if (connect -> connectType == CONNECT_TYPE_PENALTY_ENACTMENT)
		{ // alias txt
		  connect_to_string( connect, buff, buffSz, true, NULL, false, true );
										// withSender = true;
										// senderPrefix = NULL
                                                            // labelOnly = false
            }
            else
            { // connection text
              connect_to_string( connect, buff, buffSz  );  // withSender = true;
                                                            // senderPrefix = NULL
                                                            // labelOnly = false
            }
            if (buff [0]) pColumn -> addText (buff) ;
	}		}  }
	delete connect;
  delete pLaw;
  // 20070103 <--                       
} // add_preliminaryActions_to_column

void add_req_resolution_to_column (const TRequestResolution & fr,
                                   column * pColumn, char * const buff,
                                   const size_t buffSz)
// By idea of querydoc.cpp, query_resolution ()
 {
  if (! (pColumn && buff && buffSz && fr.resolution
        ))
    return ;

  TResolution resol ;

  resol.resolution = fr.resolution ;
  if (! resol.Get ())
    clearGroupData (resol) ;
  ostrstream (buff, buffSz)
    << resol.name << " от " << fr.date << " " << fr.text << ends ;
  buff [buffSz - 1] = '\0' ;
  pColumn -> addText (buff) ;
 }  // add_req_resolution_to_column

void add_date_to_column (column * pColumn, const CDate & d,
                         const char * const prefix,
                         const char * const suffix,
                         const bool emptyAllowed)
 {
  if (! pColumn)
    return ;
  if (emptyAllowed == false && d.Empty ())
    return ;

  const size_t pL = prefix ? strlen (prefix) : 0 ;
  const size_t sL = suffix ? strlen (suffix) : 0 ;
  const size_t tmpSz = pL + sL + 32 ;
  char * tmp = new char [tmpSz] ;
   {
    ostrstream oss (tmp, tmpSz) ;

    if (prefix)
      oss << prefix << " " ;
    if (d.Empty () == false)
      oss << d ;
    if (suffix)
      oss << " " << suffix ;
    oss << ends ;
    tmp [tmpSz - 1] = '\0' ;
   }
  if (tmp [0])
    pColumn -> addText (tmp) ;
  delete [] tmp ;
 }  // add_date_to_column

void add_local_surr_ments_to_column (column & decisionsColumn,
                            const TGArray <TSurroundment> & surroundments,
                                     TDecision & dec,
                                     const TLawsuit &  //law
                                    )
 {
  const int mentItems = surroundments.Count () ;

  if (mentItems > 0)  // 05.oct.2001
    for (int i = 0 ; i < mentItems ; i++)
     {  // -- 3 --
      const TSurroundment * pMent = surroundments [i] ;

      if (surr_ment_res_and_act_kind_match (pMent -> result, dec.kind))
       {  // -- 4 --
         {  // -- 5 --
          CharPtr buff (1024) ;
          const size_t buffL = buff.Size () - 1 ;

          buff [0] = '\0' ;
           {  // -- 6 --
            ostrstream oss (buff, buff.Size ()) ;

             {
              TKind k ;

              k.kind = dec.kind ;
              if (k.kind && k.Get ())
                oss << k.name ;
             }
            oss << " по " ;
             {  // -- 7 --
              TSurround surr ;

              surr.key = pMent -> key ;
              surr.kind = pMent -> kind ;
              if (surr.Get ())
               {  // -- 8 --
                 {
			char tmpTxt [SIZE_OF_SURROUNDMENT_TEXT + 64] ;

                  surround_to_string (surr, tmpTxt, sizeof (tmpTxt)) ;
                  oss << tmpTxt ;
                 }
                oss << " на " ;
                oss << ends ;
                buff [buff.Size () - 1] = '\0' ;
                if (buff [0])
                  decisionsColumn.addText (buff) ;
                 {  // -- 9 --
			TSideWind waln ;

			waln << surr ;
                  waln.FloatCriteria (buff) ;
                  if (strlen (buff) < buffL - 2)
                   {  // -- 10 --
                    char * const partArea = buff + strlen (buff) + 1 ;
                    const size_t partLen = buffL - 1 - strlen (buff) ;
                    TCountRecsQuery q (buff) ;
                    int surrAutors = 0 ;

                    while (q.Read ())
                     {  // -- 11 --
                      waln << q ;
                      if (surr_kind_and_waln_state_match (surr.kind,
                                                          waln.state))
                        surrAutors ++ ;
                     }
                    if (surrAutors > 0)
                     {
                      int printeds = 0 ;

                      q.Restart () ;
                      while (q.Read () && printeds < surrAutors)
                       {  // -- 11 --
                        waln << q ;
                        partArea [0] = '\0' ;
                        if (surr_kind_and_waln_state_match (surr.kind,
                                                            waln.state))
                         {
                          printeds ++ ;
                          ucnAndUcnTypeToName (waln.ucn, waln.ucnType,
                                               partArea, partLen) ;
                          if (printeds < surrAutors)
                            strConcat (partArea, ",", partLen) ;
                          buff [buff.Size () - 1] = '\0' ;
                          if (partArea [0])
                            decisionsColumn.addText (partArea) ;
                         }
                       }
                     }
                   }  // -- 10 --
                 }  // -- 9 --
               }  // -- 8 --
              else
               {
                oss << ends ;
                buff [buff.Size () - 1] = '\0' ;
                if (buff [0])
                  decisionsColumn.addText (buff) ;
               }
             }  // -- 7 --
           }  // -- 6 --
         }  // -- 5 --
	  if (any(pMent -> text))
          decisionsColumn.addText (pMent -> text) ;
        else
          addResultToColumn (pMent -> result, & decisionsColumn) ;
       }  // -- 4 --
     }  // -- 3 --  // 05.oct.2001 - end
 }  // add_local_surr_ments_to_column

void sess_end_docs_to_columns (column * const decisionsColumn,
                               column * const retDatesColumn,
                               TSession & sess, const TLawsuit & law,
                            const TGArray <TSurroundment> & surroundments,
                               const char * const endDocKindsToSkip)
 {
  SessionEndDocs endDocs (sess.key, sess.date, sess.kind) ;
  DecPtr pDec ;

  while (endDocs.NextDoc (* pDec))
   {  // -- 1 --
    if (endDocKindsToSkip)
      if (strchr (endDocKindsToSkip, pDec.Ref ().kind))
        continue ;
     {
      columnArray cA ;

      cA.Add (decisionsColumn) ;
      cA.Add (retDatesColumn) ;
      adjust_column_rows (cA, 1) ;
     }
    if (decisionsColumn)
     {  // -- 2 --
      addKindToColumn (pDec.Ref ().kind, decisionsColumn) ;
      if (! strIsBlank (pDec.Ref ().text))
        decisionsColumn -> addText (pDec.Ref ().text) ;
      add_local_surr_ments_to_column (* decisionsColumn, surroundments,
                                      * pDec, law) ; // 05.oct.2001
     }  // -- 2 --
    if (retDatesColumn)
      add_date_to_column (retDatesColumn, pDec.Ref ().returned) ;
   }  // -- 1 --
 }  // sess_end_docs_to_columns

void many_end_docs_and_ret_dates (column * const decisionsColumn,
                                  column * const retDatesColumn,
                                  TSession & sess, const TLawsuit & law,
                                  const int additionalRowsAfterSess)
 {
  if (decisionsColumn == NULL && retDatesColumn == NULL)
    return ;

  TGArray <TSurroundment> surroundments ;  // 05.oct.2001 съпровождащи документи

  if (decisionsColumn)
   {
    addResultToColumn (sess.result, decisionsColumn) ;
    if (any(sess.text))  // 05.oct.2001 - begin
      decisionsColumn -> addText (sess.text) ;
    for (int i = 0 ; i < additionalRowsAfterSess ; i ++)
      decisionsColumn -> addEmptyRow () ;  // 10.oct.2001
    if (try2surround (& sess, & surroundments) == false)
      surroundments.Flush () ;  // 05.oct.2001-end
   }
  if (retDatesColumn)
    add_date_to_column (retDatesColumn, sess.returned) ;
  sess_end_docs_to_columns (decisionsColumn, retDatesColumn, sess,
                            law, surroundments) ;
 }  // many_end_docs_and_ret_dates

// 08.mar.2001 - begin
void add_abolishing_to_column (column * const pColumn,
                               const CDate abolishingDate,
                          const TSession * pAbolishingSess  // 13.dec.2001
                              )
 {
  if (pColumn)
   {
    CDate abD = abolishingDate ;

    if (abD.Empty ())
      if (pAbolishingSess)
        abD = pAbolishingSess -> date ;
    if (abD.Empty () == false)
     {
       {
        char tmp [80] ;

        ostrstream (tmp, sizeof (tmp))
          << ABOLISHED_MOVE_STR
//          << abolishingDate  // 13.dec.2001я
          << abD  // 13.dec.2001
          << ends ;
	  tmp [sizeof (tmp) - 1] = '\0' ;
        pColumn -> addEmptyRow () ;
        pColumn -> addText (tmp) ;
       }
      if (pAbolishingSess)
        pColumn -> addText (pAbolishingSess -> text) ;
     }
   }
 }  // add_abolishing_to_column

// 08.mar.2001 - end
//

