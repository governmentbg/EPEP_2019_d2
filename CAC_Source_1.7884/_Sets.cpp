#include "cac.h"  // 18.may.2001
//# include "all.h"

FinalizedSessions :: FinalizedSessions (const char * const types,
                                        const CDate begDate,
                                        const CDate endDate,
                                        const char * const kinds) :
                       begD (begDate), endD (endDate), crtLawSz (1024),
                       crtLaw (new char [crtLawSz]), pQLaw (NULL),
                       badIntegrity (false)
 {
  if (types && types [0])
   {
    if (FinishedLawsCriteria (crtLaw, crtLawSz, types, begD, endD,
                              kinds))
      pQLaw = new
                  TCountRecsQuery  // TQuery  // 29.may.2002
                         (crtLaw) ;
   }
 }  // FinalizedSessions :: FinalizedSessions

FinalizedSessions :: ~FinalizedSessions ()
 {
  if (pQLaw)
    delete pQLaw ;
  if (crtLaw)
    delete[] crtLaw ;
 }  // FinalizedSessions :: ~FinalizedSessions

bool FinalizedSessions ::
FinishedLawsCriteria (char * const dest, const size_t destSz,
                      const char * const types, const CDate begDate,
                      const CDate endDate, const char * const kinds)
 {
  if (! (dest && destSz && types && types [0] &&
         (begDate.Empty () == false && endDate.Empty () == false)
        ))
    return false ;
  dest [0] = dest [destSz - 1] = '\0' ;

  ostrstream oss (dest, destSz - 1) ;

  oss
    << "SELECT "
       "F_NO, F_YEAR, F_TYPE, F_AGE, F_KIND, F_SOURCE_KIND, "
       "F_FINISHED, F_DEC_MADE"
       ", F_SUBJECT, F_DATE"  // 08.jan.2000
       ", F_COMPOSITION"  // 16.feb.2001
       ", F_GRANTED"  // 30.jan.2002
       " "
       "FROM T_LAWSUIT WHERE "
         "F_TYPE IN ("
           << stringToSQLCharSet (types).getTheSet () <<
                   ") AND "
	    "F_FINISHED BETWEEN " << begDate.Value() << " AND "
                                 << endDate.Value()   ;  // 15.apr.2003 - end
  if (kinds && kinds [0])
    oss
    << " AND F_KIND IN ("
           << stringToSQLCharSet (kinds).getTheSet () <<
                      ") " ;
  oss
    << " ;" << ends ;
  return criteria_finished_ok (dest,
                               "FinalizedSessions::FinishedLawsCriteria") ;
 }  // FinalizedSessions :: FinishedLawsCriteria

bool FinalizedSessions ::
FinalizedSessionsCriteria (char * const dest, const size_t destSz,
                           const TLawsuit & law)
 {
  if (! (dest && destSz && law.finished.Empty () == false))
    return false ;

  dest [destSz - 1] = '\0' ;
   {
    ostrstream oss (dest, destSz - 1) ;

    oss <<
      "SELECT F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_RESULT, "
	"F_RETURNED, F_END, F_JUDGE, F_COMPOSITION, F_FINAL "  // 2008:255 F_FINAL
      "FROM T_SESSION WHERE "
        "F_NO = " << law.key.no << " AND "
        "F_YEAR = " << law.key.year << " AND "
        "F_TYPE = \'" << law.key.type << "\' AND "
        "F_DATE <= " << law.finished.Value() << " AND " ;  // 15.apr.2003
    if (law.decMade.Empty () == false)
      oss <<
	"F_DATE >= " << law.decMade.Value() << " AND " ;  // 15.apr.2003
    // 2008:255 LPR: final date means finalizing session now, not result
    //oss <<
    //	"F_RESULT IN (" <<
    //          stringToSQLCharSet (RESULT_FINALIZEDS).getTheSet ()
    //                         << ") " ;
    oss << "NOT F_FINAL IS NULL" ;
    oss << " ;" << ends ;
   }
  return criteria_finished_ok (dest,
                        "FinalizedSessions::FinalizedSessionsCriteria") ;
 }  // FinalizedSessions :: FinalizedSessionsCriteria

bool FinalizedSessions :: CheckLaw (TLawsuit & law, TSession & sess,
				    TQuery & qS, outText * pOfs)
 {
  int sFound = 0 ;
  int sProper = 0 ; 
  
  while (qS.Read ())
   {  // -- 2 --

    sFound ++ ;
    (sess) << qS ;

    // 2008:255 LPR: check sess final against finished, not date/returned/end
    if (law.key.type == TYPE_PUNISHMENT_LAW)
     {  // -- 3 --
      if (strchr (KIND_CLOSED_SESSIONS, sess.kind))
       {
	const CDate startOfCheck ((char) 1, (char) 1, 2001) ;

	if (sess.date >= startOfCheck)
	 {
	  //if (sess.returned == law.finished)
	  if (sess.final == law.finished)
	    sProper ++ ;
	 }
	else
	  sProper ++ ;
       }
      else
       {
	  //if (sess.date == law.finished || sess.end == law.finished)
	  if (sess.final == law.finished)
	  sProper ++ ;
       }
     }  // -- 3 --
    else
     {  // -- 4 --
      if (((! strchr (KIND_CLOSED_SESSIONS, sess.kind))
	     &&
	    //sess.date == law.finished
	    sess.final == law.finished
	    ) ||
	  (strchr (KIND_CLOSED_SESSIONS, sess.kind)
	   &&
	   //sess.returned == law.finished
	   sess.final == law.finished
	  )
	 )
	sProper ++ ;
     }  // -- 4 --
   }  // -- 2 --

  const char sessKind = sess.kind ;
  const char sessResult = sess.result ;  // 13.jul.2004

  clearGroupData (sess) ;

  // ============ !!! DO NOT use sess more !!! ===================

  if (sFound == 1 && sProper == 1)
   {  // -- 4 --
     {  // 13.jul.2004
       if((sessResult &&
	   (strchr(RESULT_WITH_DECISIONS, sessResult) ||
	    resMustBeTreatAsDecision (sessResult)
	    //|| sess.result == RESULT_DEFINITION_TERMINATING  // No need,
	    // it is included in RESULT_WITH_DECISIONS
	   )
	  ) == false)
	{
	  bad_finished_law (law, FLE_BadFinalizedClSessionResult, pOfs) ;
	}
     }
    qS.Restart () ;
    return true ;
   }  // -- 4 --
  else
   {
    badIntegrity = true ;
    if (sFound > 1)
      bad_finished_law (law, FLE_TooManyFinalizedSessions, pOfs) ;
    else
      if (sFound == 1)
       {
	if (strchr (KIND_CLOSED_SESSIONS, sessKind))
	  bad_finished_law (law, FLE_BadFinalizedClSessionDate, pOfs) ;
	else
	  bad_finished_law (law, FLE_BadFinalizedOpenSessionDate, pOfs) ;
       }
      else
	bad_finished_law (law, FLE_NoFinalizedSession, pOfs) ;
    return false ;
   }
 }  // FinalizedSessions :: CheckLaw

bool FinalizedSessions :: NextSession (TLawsuit & law, TSession & sess,
				       outText * pOfs)
// returns true if an correctly finalized lawsuit is extracted.
// if law.key is empty, this means that there are no more finished laws.
 {
  clearGroupData (law) ;
  clearGroupData (sess) ;

  if (! pQLaw)
    return false ;
  if (pQLaw -> Read ())
   {  // -- 0 --
    const size_t crtSessSz = 1024 ;
    char * const crtSess = new char [crtSessSz] ;

    law << (* pQLaw) ;

    if (FinalizedSessionsCriteria (crtSess, crtSessSz, law))
     {  // -- 1 --
      TQuery qSess (crtSess) ;
     
      if (CheckLaw (law, sess, qSess, pOfs))
       {  // -- 4 --
        if (qSess.Read ())
         {
          sess << qSess ;
         }
        else
          error ("Unsuccessful read from restarted non-empty query") ;
       }  // -- 4 --
     }  // -- 1 --
    delete[] crtSess ;
   }  // -- 0 --
  return (TRCDKey_ (law.key).FilledOK () &&
          TRCDKey_ (sess.key).FilledOK ()) ;
 }  // FinalizedSessions :: NextSession

bool FinalizedSessions :: BadIntegrity () const
 {
  return badIntegrity ;
 }  // FinalizedSessions :: BadIntegrity

bool FinalizedSessions :: StillMayContinue () const
 {
  if (BadIntegrity ())
    return confirm_bad_finished_laws () ;
  else
    return true ;
 }  // FinalizedSessions :: StillMayContinue

unsigned FinalizedSessions::FinishedLawsNumber () const
{  // 29.may.2002
	return pQLaw ? pQLaw->getRecsCount() : 0;
}  //FinalizedSessions :: FinishedLawsNumber

SessionEndDocs :: SessionEndDocs (const TRCDKey key, const CDate date,
                                  const char sessionKind) :
                    crtSz (512 + 256), crt (NULL), pQ (NULL), count (0),
                    sessKey (key), sessDate (date), sessKind (sessionKind)
 {
  crt = new char [crtSz] ;
  if (EndDocsCriteria (key, date, sessionKind, crt, crtSz))
   {
    pQ = new TCountRecsQuery (crt) ;
    count = pQ -> getRecsCount () ;
   }
 }  // SessionEndDocs :: SessionEndDocs

SessionEndDocs :: ~SessionEndDocs ()
 {
  if (crt)
    delete[] crt ;
  if (pQ)
    delete pQ ;
 }  // SessionEndDocs :: ~SessionEndDocs

bool SessionEndDocs :: EndDocsCriteria (const TRCDKey key,
                                        const CDate date,
                                        const char sessionKind,
                                        char * const criteria,
                                        const size_t criteriaSz)
 {
  if (! criteriaSz)
    return criteria_finished_ok ("", "SessionEndDocs::EndDocsCriteria") ;
  if (criteria)
   {  // -- 0 --
    criteria [0] = '\0' ;
    if (TRCDKey_ (key).FilledOK () && date.Empty () == false &&
	sessionKind)
     {  // -- 1 --
       {  // -- 2 --
        ostrstream oss (criteria, criteriaSz - 1) ;

        oss << "SELECT "
                 "D.F_NO, D.F_YEAR, D.F_TYPE, D.F_DATE, D.F_KIND "
                 "FROM T_DECISION D, T_KIND K WHERE "
                 "D.F_NO = " << key.no << " AND "
                 "D.F_YEAR = " << key.year << " AND "
                 "D.F_TYPE = \'" << key.type << "\' AND "
                 "D.F_DATE = " << date.Value() << " AND "
                 "D.F_SESSION_KIND = \'" << sessionKind << "\' AND "
                 "D.F_KIND = K.F_KIND "
                 "ORDER BY "
			 "D.F_FROM_DATE, "  // 10.oct.2001
			 "K.F_ORDER "
                 ";"
            << ends ;
       }  // -- 2 --
      criteria [criteriaSz - 1] = '\0' ;
     }  // -- 1 --
   }  // -- 0 --
  return criteria_finished_ok (criteria,
                               "SessionEndDocs::EndDocsCriteria") ;
 }  // SessionEndDocs :: EndDocsCriteria

bool SessionEndDocs :: NextKey (TDecision & dec)  // 01.aug.2001
 {
  clearGroupData (dec) ;
  if (! (pQ && count && pQ -> Read ()))
    return false ;
  dec << (* pQ) ;
  return true ;
 }  // SessionEndDocs :: NextKey

bool SessionEndDocs :: NextDoc (TDecision & dec)
 {
  const bool res = NextKey (dec) && dec.Get () ;

  return res ;
 }  // SessionEndDocs :: NextDoc

unsigned int SessionEndDocs :: DocsFound () const
 {
  return (pQ && count) ? count : 0 ;
 }  // SessionEndDocs :: DocsFound

void SessionEndDocs :: Rewind ()
 {
  if (pQ)
    pQ -> Restart () ;
 }  // SessionEndDocs :: Rewind ()

bool SessionEndDocs :: ChooseOne (TWindow * pParent, TDecision & dec,
                                  const char * forWhat)
      // Attention! Rewinds the set.
 {
  bool res = false ;

  if (forWhat == NULL)
    forWhat = "" ;
  Rewind () ;
  if (pQ && count)
   {
    if (count == 1)
      res = NextDoc (dec) ;
    else
     {
      // 20080321 ->
      bool procException = true;
#if COURT_TYPE == COURT_REGIONAL
	if(sessKey.type == TYPE_CITIZEN_LAW)
	{
	  TLawsuit law ;
	  law.key = sessKey ;
	  if (law.Get () && (law.source.no))
	  {
	    TRequest request;
	    request.key = law.source;
	    if (request.Get() && (request.kind == KIND_EXECUTIVE_REQUEST))
	    {
            TSession session;
            session.key = sessKey;
            session.kind = sessKind;
            session.date = sessDate;
            if (session.Get() && (strchr(RESULT_EXECUTIVABLES, session.result) != NULL))
              procException = false;
          }
        }
      }
#endif  // REGIONAL
      // 20080321 <-
      char tmp [128] ;
      if (procException)   // 20080321
       {
        DatedContainer_ dC_(sessKey.type, sessKey.no, sessKey.year,
                            sessKind, sessDate) ;

        ostrstream (tmp, sizeof (tmp)) << dC_ << ends ;
        tmp [sizeof (tmp) - 1] = '\0' ;
        if (ask ("%s:\n"
                 "ѕо %s\n""има повече от един краен документ.\n"
                 "ћол€, изберете нужни€.\n"
                 "∆елаете ли да прегледате справката за делото\n"
                 "преди това?",
                 forWhat, tmp))
         {
          if (can_query ())
		query_key(pParent, sessKey);
          else
		message ("¬ие н€мате права за тази справка.") ;
         }
       }

      tmp [0] = '\0' ;
      if (procException)   // 20080321
       {
        while (NextKey (dec))
         {
          char a2 [2] ;

          a2 [0] = dec.kind ;
          a2 [1] = '\0' ;
          strConcat (tmp, a2, sizeof (tmp)) ;
         }
       } // 20080321

      if (tmp [0])
       {
          TLawsuit kC ;  // kind container

          if (select_kind (pParent, & kC, tmp) && kC.kind)
           {
            Rewind () ;
            while (res == false && NextKey (dec))
              res = (dec.kind == kC.kind && dec.Get ()) ;
           }
       }
#if COURT_TYPE == COURT_REGIONAL
	else { // 20080321 ->
		Rewind () ;
		while (res == false && NextKey (dec))
		  res = ((dec.kind == KIND_EXECUTIVE) && dec.Get ()) ;
	 } // 20080321  <-
#endif  // REGIONAL
     }
   }
  if (res == false)
    clearGroupData (dec) ;
  Rewind () ;
  return res ;
 }  // SessionEndDocs :: ChooseOne

bool SessionEndDocs :: GeneralCancelingResult ()
	// Attention! Rewinds the set.
 {
# if INSTANCE
  bool res = false ;
  DecPtr pDec ;

  Rewind () ;
  if (pQ && count)
   {
    constant rCancels = RESULT_CANCELAWS ;

    if (count == 1)
	res = NextDoc (* pDec) &&
		strchr (rCancels, pDec.Ref ().result) ;
    else
     {
     #if 20160025
	long flags = 0;

	while (NextDoc (* pDec))
	  flags |= strchr(rCancels, pDec.Ref ().result) ? 1 : 2;

	const bool antinomy = flags == 3;
     #else  // 20160025
	char resultsSet [SIZE_OF_RESULTS] ;
	bool antinomy = false ;

	*resultsSet = '\0';
	while (NextDoc (* pDec))
	  if (strchr(resultsSet, pDec.Ref ().result) == NULL)
	    strcach(resultsSet, pDec.Ref ().result) ;
	if (strchr(resultsSet, rCancel))
	  if (strlen(resultsSet) == 1)
	    res = true ;
	  else
	    if (strlen(resultsSet) > 1)
		antinomy = true ;
     #endif  // 20160025
	if (antinomy)
	 {
	  char tmp [128] ;
	  DatedContainer_ dC_(sessKey.type, sessKey.no, sessKey.year,
				    sessKind, sessDate) ;

	  ostrstream (tmp, sizeof (tmp)) << dC_ << ends ;
	  tmp [sizeof (tmp) - 1] = '\0' ;
	  if (antinomy)
	    if (ask ("%s:\n"
			 " райните документи по това заседание имат "
			 "различни резултати, които си противоречат "
			 "по отношение на това дали делото тр€бва да се "
			 "счита за решено или за прекратено.\n"
			 "∆елаете ли да изберете един от т€х за определ€щ "
			 "в случа€?", tmp))
		if (ChooseOne (NULL, * pDec, ""))
		  res = strchr (rCancels, pDec.Ref ().result) ;
	 }
     }
   }
  Rewind () ;
  return res ;
# else  // of INSTANCE
  return false ;
# endif  // of INSTANCE
 }  // SessionEndDocs :: GeneralCancelingResult

void BriefSessionsToCheckGroup :: Clear ()
 {
  clearGroupData (* this) ;
 }  // BriefSessionsToCheckGroup :: Clear

BriefSessionsToCheckGroup :: BriefSessionsToCheckGroup (char * name) :
  AloneDatedContainer_xK (name ? name : "BRIEF_SESSIONS_TO_CHECK_GROUP")
 {
  Add(
    new TDate ("F_RETURNED", & returned, false),
    new TCharAlias ("F_RESULT", & result),
    new TLongAlias ("F_COMPOSITION", & composition),
    new TPersonAlias ("F_JUDGE", judge, sizeof (judge)),
    new TPersonAlias ("F_PRESIDENT", president, sizeof (president)),
    new TPersonAlias ("F_JUDGE_X", judgeX, sizeof (judgeX)),
    new TPersonAlias ("F_REJUDGE_X", reJudgeX, sizeof (reJudgeX)),
    NULL
   );
  Clear () ;
 }  // BriefSessionsToCheckGroup :: BriefSessionsToCheckGroup

void BriefSessionsToCheckGroup ::
       MoveDataFromBSCG (const BriefSessionsToCheckGroup & bSCG)
 {
  key = bSCG.key ;
  date = bSCG.date ;
  kind = bSCG.kind ;
  returned = bSCG.returned ;
  result = bSCG.result ;
  composition = bSCG.composition ;
  memmove (judge, bSCG.judge, sizeof (judge)) ;
  memmove (president, bSCG.president, sizeof (president)) ;
  memmove (judgeX, bSCG.judgeX, sizeof (judgeX)) ;
  memmove (reJudgeX, bSCG.reJudgeX, sizeof (reJudgeX)) ;
 }  // BriefSessionsToCheckGroup :: MoveDataFromBSCG

void BriefSessionsToCheckGroup ::
       MoveDataFromSession (const TSession & sess)
 {
  key = sess.key ;
  date = sess.date ;
  kind = sess.kind ;
  returned = sess.returned ;
  result = sess.result ;
  composition = sess.composition ;
  memmove (judge, sess.judge, sizeof (judge)) ;
  memmove (president, sess.president, sizeof (president)) ;
  memmove (judgeX, sess.judgeX, sizeof (judgeX)) ;
  memmove (reJudgeX, sess.reJudgeX, sizeof (reJudgeX)) ;
 }  // BriefSessionsToCheckGroup :: MoveDataFromSession

void BriefSessionsToCheckGroup ::
       MoveDataToSession (TSession & sess) const
 {
  sess.key = key ;
  sess.date = date ;
  sess.kind = kind ;
  sess.returned = returned ;
  sess.result = result ;
  sess.composition = composition ;
  memmove (sess.judge, judge, sizeof (sess.judge)) ;
  memmove (sess.president, president, sizeof (sess.president)) ;
  memmove (sess.judgeX, judgeX, sizeof (sess.judgeX)) ;
  memmove (sess.reJudgeX, reJudgeX, sizeof (sess.reJudgeX)) ;
 }  // BriefSessionsToCheckGroup :: MoveDataToSession

bool SessionsToCheck ::
MakeSessionsToCheckCriteria (char * const dest, const size_t destSz,
                             const char * const types,
                             const CDate begDate,
                             const CDate endDate,
                             const long int composition,
                             const char * const lawKinds,
                             const char * const sessKinds,
                             bool excludeResFixed)
 {
  if (! (dest && destSz && types && types [0] &&
         (begDate.Empty () == false && endDate.Empty () == false)
        ))
    return false ;

  const bool useTLawsuit = lawKinds && lawKinds [0] ;  // 31.may.2001
  ostrstream oss (dest, destSz) ;

  oss << "SELECT " ;
//  oss << "DISTINCT " ;  // 31.may.2001
  oss <<
  // ----------------------------------------------------------------
  // You have to add data members to the BriefSessionsToCheckGroup
  // class if you add any fields to extract here.
         "S.F_TYPE, S.F_YEAR, S.F_NO, S.F_KIND, S.F_DATE, "  // These
           // are enough for the "distinct"
         "S.F_RETURNED, S.F_RESULT, S.F_COMPOSITION, "
         "S.F_JUDGE, S.F_PRESIDENT, S.F_JUDGE_X, S.F_REJUDGE_X "
  // ----------------------------------------------------------------
         "FROM T_SESSION S " ;
  if (useTLawsuit)
    oss << ", T_LAWSUIT L " ;
  oss << "WHERE "
         "S.F_TYPE IN (" <<
           stringToSQLCharSet (types).getTheSet ()
                         << ") AND "
         "S.F_RESULT != \'" << RESULT_UNFIXED << "\' AND " 
         "S.F_RESULT != \'" << RESULT_ABORTED << "\' AND " ; //20050322
  if (excludeResFixed)
    oss <<
         "S.F_RESULT != \'" << RESULT_FIXED << "\' AND " ;
  oss << "S.F_KIND != \'" << KIND_RESOLUTION_SESSION << "\' AND "
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
#   endif  // of APPEAL
# else  // of INSTANCE
         "S.F_KIND != \'" << KIND_LISTENING_SESSION << "\' AND "
# endif  // of INSTANCE
         "((S.F_KIND NOT IN (" <<
           stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                         << ") AND "
	     "S.F_DATE BETWEEN " << begDate.Value() << " AND "
                                 << endDate.Value() << 
          ") OR "
          "(S.F_KIND IN (" <<
           stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                         << ") AND "
	     "S.F_RETURNED BETWEEN " << begDate.Value() << " AND "
                                     << endDate.Value() << 
          ")"
         ") " ;
  if (sessKinds && sessKinds [0])
    oss <<
         "AND S.F_KIND IN (" <<
           stringToSQLCharSet (sessKinds).getTheSet ()
                             << ") " ;
  if (composition)
    oss <<
         "AND S.F_COMPOSITION = " << composition << " " ;
  if (useTLawsuit)
    oss <<
         "AND L.F_TYPE = S.F_TYPE "
         "AND L.F_YEAR = S.F_YEAR "
         "AND L.F_NO = S.F_NO "
         "AND L.F_KIND IN (" <<
           stringToSQLCharSet (lawKinds).getTheSet ()
                             << ") " ;
  oss << "ORDER BY S.F_TYPE, S.F_YEAR, S.F_NO, "
                  "S.F_DATE, S.F_TIME ;" ;
  // Important ! Do not change the first three fields of this clause!
  // This reflects to the program speed (where this object is used).                  
  oss << ends ;                
  dest [destSz - 1] = '\0' ;
  return criteria_finished_ok (dest,
                     "SessionsToCheck::MakeSessionsToCheckCriteria") ;
 }  // SessionsToCheck :: MakeSessionsToCheckCriteria

SessionsToCheck :: SessionsToCheck (const char * const types,
                                    const CDate begDate,
                                    const CDate endDate,
                                    const long int composition,
                                    const char * const lawKinds,
                                    const char * const sessKinds,
                                    const bool excludeResFixed
                                   ) :
                     critSz (768), crit (new char [critSz]), pQ (NULL),
                     count (0)
 {
  if (MakeSessionsToCheckCriteria (crit, critSz, types, begDate,
                                   endDate, composition, lawKinds,
                                   sessKinds, excludeResFixed))
   {
    pQ = new TCountRecsQuery (crit) ;
    count = pQ -> getRecsCount () ;
   }
 }  // SessionsToCheck :: SessionsToCheck

SessionsToCheck :: ~SessionsToCheck ()
 {
  if (pQ)
    delete pQ ;
  if (crit)
    delete[] crit ;  
 }  // SessionsToCheck :: ~SessionsToCheck

bool SessionsToCheck :: NextSession (TSession & sess)
 {
  if (pQ)
    if (pQ -> Read ())
     {
      sess << (* pQ) ;
      return true ;
     }
    else
      clearGroupData (sess) ;
  else
    clearGroupData (sess) ;
  return false ;
 }  // SessionsToCheck :: NextSession

bool SessionsToCheck :: NextSession (BriefSessionsToCheckGroup & bSCG)
 {
  if (pQ)
    if (pQ -> Read ())
     {
      bSCG << (* pQ) ;
      return true ;
     }
    else
      bSCG.Clear () ;
  else
    bSCG.Clear () ;
  return false ;
 }  // SessionsToCheck :: NextSession

unsigned long int SessionsToCheck :: SessionsFound () const
 {  // 28.feb.2001
  return (pQ && count) ? count : 0 ;
 }  // SessionsToCheck :: SessionsFound

bool LawsuitAllJudgements ::
MakeLawsuitAllJudgementsCriteria (char * const dest, const size_t destSz,
                                  const TRCDKey key)
 {
  if (! (dest && destSz && TRCDKey_ (key).FilledOK ()))
    return false ;

  ostrstream oss (dest, destSz) ;

  oss << "SELECT "
         "J.F_NO, J.F_YEAR, J.F_TYPE, "
         "J.F_KIND, J.F_DATE, J.F_UCN_TYPE, J.F_UCN, "
         "J.F_INVOLVEMENT "  // 09.jan.2004
         "FROM T_JUDGEMENT J, T_KIND K "
         "WHERE "
         "J.F_TYPE = \'" << key.type << "\' AND "
         "J.F_YEAR = " << key.year << " AND "
         "J.F_NO = " << key.no << " AND "
         "J.F_KIND = K.F_KIND "
         "ORDER BY J.F_DATE, K.F_ORDER " ;
  oss << " ;" ;
  oss << ends ;
  dest [destSz - 1] = '\0' ;
  return criteria_finished_ok (dest,
              "LawsuitAllJudgements :: MakeLawsuitAllJudgementsCriteria") ;
 }  // LawsuitAllJudgements :: MakeLawsuitAllJudgementsCriteria

LawsuitAllJudgements :: LawsuitAllJudgements (const TRCDKey key) :
                     critSz (512), crit (new char [critSz]), pQ (NULL),
                     count (0)
 {
  if (MakeLawsuitAllJudgementsCriteria (crit, critSz, key))
   {
    pQ = new TCountRecsQuery (crit) ;
    count = pQ -> getRecsCount () ;
   }
 }  // LawsuitAllJudgements :: LawsuitAllJudgements

LawsuitAllJudgements :: ~LawsuitAllJudgements ()
 {
  if (pQ)
    delete pQ ;
  if (crit)
    delete[] crit ;
 }  // LawsuitAllJudgements :: ~LawsuitAllJudgements

bool LawsuitAllJudgements :: NextJudgement (TJudgement & lJ)
 {
  return NextKey (lJ) && lJ.Get () ;
 }  // LawsuitAllJudgements :: NextJudgement

bool LawsuitAllJudgements :: NextKey (TJudgement & lJ)  // 01.aug.2001
 {
  bool res = false ;

  clearGroupData (lJ) ;
  if (pQ)
    if (pQ -> Read ())
     {
      lJ << (* pQ) ;
      res = true ;
     }
  return res ;
 }  //  LawsuitAllJudgements :: NextKey

void LawsuitAllJudgements :: Rewind ()
 {
  if (pQ)
    pQ -> Restart () ;
 }  // SessionEndDocs :: Rewind ()

unsigned int LawsuitAllJudgements :: JudgementsFound () const
 {
  return count ;
 }  // LawsuitAllJudgements :: JudgementsFound

EndDocJudgements :: EndDocJudgements (const TRCDKey key,
                                      const CDate date,
                                      const char decisionKind) :
                      currIndex (0)
 {
  LawsuitAllJudgements lAllJ (key) ;  // It is sorted.
  const unsigned int jFound = lAllJ.JudgementsFound () ;
  JudgPtr pJudg ;
  TJudgement & judg = pJudg.Ref () ;

  for (unsigned int i = 0 ; i < jFound ; i ++)
    if (lAllJ.NextKey (judg))
      if (judg.date == date && judg.kind == decisionKind)
	 {
	  TSideWind * pNewDSide = new TSideWind (judg.Name, NULL) ;
	  (* pNewDSide) << judg ;
	  keys.Add (pNewDSide) ;
	 }
  Rewind () ;
 }  // EndDocJudgements :: EndDocJudgements

EndDocJudgements :: ~EndDocJudgements ()
 {
 }  // EndDocJudgements :: ~EndDocJudgements

bool EndDocJudgements :: NextKey (TJudgement & judg)
 {
  bool res = false ;

  clearGroupData (judg) ;
  if (currIndex < JudgementsFound ())
   {
    judg << (* (keys [currIndex ++])) ;
    res = true ;
   }
  return res ;
 }  // EndDocJudgements :: NextKey

bool EndDocJudgements :: NextJudgement (TJudgement & judg)
 {
  return NextKey (judg) && judg.Get () ;
 }  // EndDocJudgements :: NextJudgement

unsigned int EndDocJudgements :: JudgementsFound () const
 {
  return keys.Count () ;
 }  // EndDocJudgements :: JudgementsFound

void EndDocJudgements :: Rewind ()
 {
  currIndex = 0 ;
 }  // EndDocJudgements :: Rewind ()

TRCDKeyAllDatedContainers :: TRCDKeyAllDatedContainers (const TRCDKey aKey) :
    critSz (1024), crit (new char [critSz]), pQ (NULL), count (0),
    key (aKey), initCalled (false)
 {
  crit [0] = '\0' ;
 }  // TRCDKeyAllDatedContainers :: TRCDKeyAllDatedContainers

TRCDKeyAllDatedContainers :: ~TRCDKeyAllDatedContainers ()
 {
  delete pQ ;
  delete[] crit ;
 }  // TRCDKeyAllDatedContainers :: ~TRCDKeyAllDatedContainers

bool TRCDKeyAllDatedContainers :: Init ()
 {
  bool res = false ;
  if (initCalled == false)
   {
    initCalled = true ;
    if (MakeCriteria ())
     {
      pQ = new TCountRecsQuery (crit) ;
      count = pQ -> getRecsCount () ;
     }
    res = (pQ != NULL) ;
   }
  return res ;
 }  // TRCDKeyAllDatedContainers :: Init ()

bool TRCDKeyAllDatedContainers ::
  MakeSimpleCriteria (const char * const tableName)
 {
  bool res = false ;
  const char * const dataMember = "data member" ;
  char methodFullName [128] ;

  ostrstream (methodFullName, sizeof (methodFullName))
    << typeid (* this).name () << "::MakeSimpleCriteria" << ends ;
  methodFullName [sizeof (methodFullName) - 1] = '\0' ;

  if (crit && critSz)
   {
    if (TRCDKey_ (key).FilledOK ())
     {
      if (tableName && tableName [0])
       {
         {
          ostrstream oss (crit, critSz) ;

          oss << "SELECT DC.F_TYPE, DC.F_YEAR, DC.F_NO, DC.F_KIND, "
                        "DC.F_DATE "  // The 'DC.' prefixes added on
                                      // 03.jan.2001
                 "FROM " << tableName << " DC, T_KIND K "
                 "WHERE DC.F_TYPE = \'" << key.type << "\' AND "
                       "DC.F_YEAR = " << key.year << " AND "
                       "DC.F_NO = " << key.no << " AND "
                       "K.F_KIND = DC.F_KIND "
                 "ORDER BY DC.F_DATE, K.F_ORDER, K.F_NAME" ;
          oss << " ;" << ends ;
         }
        crit [critSz - 1] = '\0' ;
        res = criteria_finished_ok (crit, methodFullName) ;
       }
      else
        error ("%s:\n""Wrong value of the 'tableName' %s.",
               methodFullName, "argument") ;
     }
    else
      error ("%s:\n""Badly filled 'key' %s.", methodFullName, dataMember) ;
   }
  else
    error ("%s:\n""Wrong value of 'crit' or 'critSz' %s(s).",
           methodFullName, dataMember) ;
  return res ;
 }  // TRCDKeyAllDatedContainers :: MakeSimpleCriteria

bool TRCDKeyAllDatedContainers :: MakeCriteria ()
 {
  return error ("Inappropriate use of the '%s' class.",
                typeid (* this).name ()) ;
 }  // TRCDKeyAllDatedContainers :: MakeCriteria

unsigned int TRCDKeyAllDatedContainers :: DatedContainersFound ()
 {
  if (initCalled == false)
    Init () ;
  return count ;
 }  // TRCDKeyAllDatedContainers :: DatedContainersFound

void TRCDKeyAllDatedContainers :: Rewind ()
 {
  if (DatedContainersFound ())
    pQ -> Restart () ;
 }  // TRCDKeyAllDatedContainers :: Rewind

bool TRCDKeyAllDatedContainers :: NextKey (TDatedContainer & dC)
 {
  bool res = false ;

  if (DatedContainersFound ())
    res = pQ -> Read () ;
  if (res)
    dC << (* pQ) ;
  else
    clearGroupData (dC) ;
  return res ;
 }  // TRCDKeyAllDatedContainers :: NextKey

bool TRCDKeyAllDatedContainers :: NextWholeRecord (TDatedContainer & dC)
 {
  bool res = false ;

  if (NextKey (dC))
    res = dC.Get () ;
  if (res == false)
    clearGroupData (dC) ;
  return res ;
 }  // TRCDKeyAllDatedContainers :: NextWholeRecord

RequestAllResolutions :: RequestAllResolutions (const TRCDKey key) :
                           TRCDKeyAllDatedContainers (key)
 {
 }  // RequestAllResolutions :: RequestAllResolutions

bool RequestAllResolutions :: MakeCriteria ()
 {
  return MakeSimpleCriteria ("T_REQUEST_RESOLUTION") ;
 }  // RequestAllResolutions :: MakeCriteria

// 11.jan.2002 - begin 
LawsuitAllSessions :: LawsuitAllSessions (const TRCDKey aKey) :
                        TRCDKeyAllDatedContainers (aKey)
 {
 }  // LawsuitAllSessions :: LawsuitAllSessions

bool LawsuitAllSessions :: MakeCriteria ()
 {
  return MakeSimpleCriteria ("T_SESSION") ;
 }  // LawsuitAllSessions :: MakeCriteria

LawsuitAllDecisions :: LawsuitAllDecisions (const TRCDKey aKey) :
                         TRCDKeyAllDatedContainers (aKey)
 {
 }  // LawsuitAllDecisions :: LawsuitAllDecisions

bool LawsuitAllDecisions :: MakeCriteria ()
 {
  return MakeSimpleCriteria ("T_DECISION") ;
 }  // LawsuitAllDecisions :: MakeCriteria


BankruptcyLawAllDecisions ::
  BankruptcyLawAllDecisions (const TRCDKey aKey) :
    LawsuitAllDecisions (aKey)
 {
 }  // BankruptcyLawAllDecisions :: BankruptcyLawAllDecisions

bool BankruptcyLawAllDecisions :: MakeCriteria ()
 {
  bool res = false ;
  const char * const dataMember = "data member" ;
  char methodFullName [128] ;
  const char * const tableName = "T_DECISION" ;

  ostrstream (methodFullName, sizeof (methodFullName))
    << typeid (* this).name () << "::MakeCriteria" << ends ;
  methodFullName [sizeof (methodFullName) - 1] = '\0' ;

  if (crit && critSz)
   {
    if (TRCDKey_ (key).FilledOK ())
     {
      if (tableName && tableName [0])
       {
         {
          ostrstream oss (crit, critSz) ;

          oss << "SELECT DC.F_TYPE, DC.F_YEAR, DC.F_NO, DC.F_KIND, "
                        "DC.F_DATE, DC.F_FROM_DATE "
                 "FROM " << tableName << " DC, T_KIND K "
                 "WHERE DC.F_TYPE = \'" << key.type << "\' AND "
                       "DC.F_YEAR = " << key.year << " AND "
                       "DC.F_NO = " << key.no << " AND "
                       "K.F_KIND = DC.F_KIND "
                 "ORDER BY DC.F_DATE, DC.F_FROM_DATE, "
                          "K.F_ORDER, K.F_NAME" ;
          oss << " ;" << ends ;
         }
        crit [critSz - 1] = '\0' ;
        res = criteria_finished_ok (crit, methodFullName) ;
       }
      else
        error ("%s:\n""Wrong value of the 'tableName' %s.",
               methodFullName, "argument") ;
     }
    else
      error ("%s:\n""Badly filled 'key' %s.", methodFullName, dataMember) ;
   }
  else
    error ("%s:\n""Wrong value of 'crit' or 'critSz' %s(s).",
           methodFullName, dataMember) ;
  return res ;
 }  // BankruptcyLawAllDecisions :: MakeCriteria
 

LawsuitAllInRegs :: LawsuitAllInRegs (const TRCDKey aKey) :
                      TRCDKeyAllDatedContainers (aKey) 
 {
 }  // LawsuitAllInRegs :: LawsuitAllInRegs

bool LawsuitAllInRegs :: MakeCriteria ()
 {
  bool res = false ;
  const char * const dataMember = "data member" ;
  char methodFullName [128] ;
  const char * const tableName = "T_INREG" ;

  ostrstream (methodFullName, sizeof (methodFullName))
    << typeid (* this).name () << "::MakeCriteria" << ends ;
  methodFullName [sizeof (methodFullName) - 1] = '\0' ;

  if (crit && critSz)
   {
    if (TRCDKey_ (key).FilledOK ())
     {
       {
        ostrstream oss (crit, critSz) ;

        oss << "SELECT DC.F_TYPE, DC.F_YEAR, DC.F_NO, DC.F_KIND, "
                      "DC.F_DATE "
               "FROM " << tableName << " DC "
               "WHERE DC.F_LAWSUIT_TYPE = \'" << key.type << "\' AND "
                     "DC.F_LAWSUIT_YEAR = " << key.year << " AND "
                     "DC.F_LAWSUIT_NO = " << key.no << " "
               "ORDER BY DC.F_DATE, DC.F_YEAR, DC.F_NO" ;
        oss << " ;" << ends ;
       }
      crit [critSz - 1] = '\0' ;
      res = criteria_finished_ok (crit, methodFullName) ;
     }
    else
      error ("%s:\n""Badly filled 'key' %s.", methodFullName, dataMember) ;
   }
  else
    error ("%s:\n""Wrong value of 'crit' or 'critSz' %s(s).",
           methodFullName, dataMember) ;
  return res ;
 }  // LawsuitAllInRegs :: MakeCriteria

InRegAllEvents :: InRegAllEvents (const TRCDKey aKey) :
                        TRCDKeyAllDatedContainers (aKey) 
 {
 }  // InRegAllEvents :: InRegAllEvents

bool InRegAllEvents :: MakeCriteria ()
 {
  bool res = false ;
  const char * const dataMember = "data member" ;
  char methodFullName [128] ;
  const char * const tableName = "T_INREG_EVENT" ;

  ostrstream (methodFullName, sizeof (methodFullName))
    << typeid (* this).name () << "::MakeCriteria" << ends ;
  methodFullName [sizeof (methodFullName) - 1] = '\0' ;

  if (crit && critSz)
   {
    if (TRCDKey_ (key).FilledOK ())
     {
      if (tableName && tableName [0])
       {
         {
          ostrstream oss (crit, critSz) ;

          oss << "SELECT DC.F_TYPE, DC.F_YEAR, DC.F_NO, DC.F_KIND, "
				"DC.F_DATE " //, DC.F_FROM_DATE " // F_FROM_DATE replaced by F_DATE
		     "FROM " << tableName << " DC, T_KIND K "
		     "WHERE DC.F_TYPE = \'" << key.type << "\' AND "
			     "DC.F_YEAR = " << key.year << " AND "
			     "DC.F_NO = " << key.no << " AND "
			     "K.F_KIND = DC.F_KIND "
		     "ORDER BY DC.F_DATE, K.F_ORDER, K.F_NAME" ;
          oss << " ;" << ends ;
         }
        crit [critSz - 1] = '\0' ;
        res = criteria_finished_ok (crit, methodFullName) ;
       }
      else
        error ("%s:\n""Wrong value of the 'tableName' %s.",
               methodFullName, "argument") ;
     }
    else
      error ("%s:\n""Badly filled 'key' %s.", methodFullName, dataMember) ;
   }
  else
    error ("%s:\n""Wrong value of 'crit' or 'critSz' %s(s).",
           methodFullName, dataMember) ;
  return res ;
 }  // InRegAllEvents :: MakeCriteria

// 11.jan.2002 - end

//
