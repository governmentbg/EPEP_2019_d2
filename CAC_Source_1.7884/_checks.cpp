#include "cac.h"

#if COURT_TYPE == COURT_ADMIN
static constant KIND_PUNISHMENT_LAWS = "";
#endif  // ADMIN

bool lawIsCreatedByInDoc (const TLawsuit & lawsuit)
{
// 20061228 -->
  bool res = false ;
  TLawsuit * pLawsuit = new TLawsuit ;
  pLawsuit -> key = lawsuit.key;
  TConnect *c = main_connect( pLawsuit );
  if ( c == NULL ) res = true;
  delete c;
  delete pLawsuit;
  return res;
}  // lawIsCreatedByInDoc

bool lawIsReceivedByJurisd (const TLawsuit & lawsuit)
 {
// 20061228 -->
  TLawsuit * pLawsuit = new TLawsuit ;
  pLawsuit -> key = lawsuit.key;
  bool res = connect_type_in( pLawsuit, CONNECT_TYPE_JURISDICTIONS);
  delete pLawsuit;
  return res;

}  // lawIsReceivedByJurisd

bool lawIsReturnedFrom2ndInstance (const TLawsuit & lawsuit)
 {
// 20070124  -->
  bool res = false; ;
  if (strchr (TYPE_RETURNEDS, lawsuit.kind))
  {
     TLawsuit * pLawsuit = new TLawsuit ;
     pLawsuit -> key = lawsuit.key;
     TConnect *c = main_connect( pLawsuit );
     if (( c != NULL ) && ( c -> connectType == CONNECT_TYPE_RETURNED_LAWSUIT ) )
        res = true;
     delete pLawsuit;
     delete c;
  }
// 20070124 <--
  return res;
 }  // lawIsReturnedFrom2ndInstance

bool thereIsNextSession (const TSession & currSess, TSession & nextSess,
                         const char * sessKinds, bool sessKindsMatch)
 {
  bool res = false ;
  char criteria [256 + 128] ;

   {
    stringToSQLCharSet kS (sessKinds) ;
    ostrstream  oss (criteria, sizeof (criteria)) ;

    oss
      << "SELECT * FROM T_SESSION "
         "WHERE "
           "F_NO = " << currSess.key.no << " AND "
           "F_YEAR = " << currSess.key.year << " AND "
           "F_TYPE = \'" << currSess.key.type << "\' AND "
           "F_DATE >= " << currSess.date.Value() << " " ;
    if (sessKinds && sessKinds [0])
      oss << "AND "
           "F_KIND " << (sessKindsMatch ? "" : "NOT ")
                     << "IN (" << kS.getTheSet () << ") " ;
    oss
      << "ORDER BY F_DATE, F_TIME ;" << ends ;
    criteria [sizeof (criteria) - 1] = '\0' ;
   }
  if (! criteria_finished_ok (criteria, "thereIsNextSession"))
    return false ;
   {
    TQuery q (criteria) ;

    while (! res && q.Read ())
     {
      TSession s ;

      s << q ;
      if (currSess.date <= s.date && currSess.date != s.date)  // eq to
                                       // currEss.date < s.date
        res = true ;
      else
        if (currSess.date == s.date && s.time > currSess.time)
          res = true ;
      if (res)
        nextSess << s ;
     }
   }
  return res ;
 }  // thereIsNextSession

bool resMustBeTreatAsDecision (const char result)
  // Created : 10.dec.1998
 {
  return ( strchr (RESULT_CANCELEDS, result) ||
	     strchr (RESULT_RETURNEDS, result) ||
	    (result == RESULT_SEND)) ;
 }  // resMustBeTreatAsDecision

#if COURT_TYPE == COURT_APPEAL
bool lawIsCocern3rdInst(const char)
{
	return false;
}
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
bool lawIsCocern3rdInst(const char kind)
{
	return strchr(KIND_2ND_ADMIN_LAWS, kind);
}
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
bool lawIsCocern3rdInst(const char kind)
{
    return strchr(KIND_2ND_ADMIN_LAWS, kind) || kind == KIND_2ND_ADMIN_PUNISHMENT_LAW;
}
#endif

#if COURT_TYPE == COURT_MILITARY
bool lawIsCocern3rdInst(const char kind)
{
    return kind == KIND_2ND_ADMIN_PUNISHMENT_LAW;
}
#endif

bool countThisSession (const CDate sessDate,
			     const CDate lawsuitFinished,
			     const bool emptyFinishedAllowed)
 {
  return (sessDate <= lawsuitFinished) ||
	   (emptyFinishedAllowed && lawsuitFinished.Empty ()) ;
 }  // bool countThisSession (const CDate sessDate,
    //                        const CDate lawsuitFinished,
    //                        const bool emptyFinishedAllowed)

bool countThisSession (const TSession & sess, const TLawsuit & lawsuit,
			     const bool emptyFinishedAllowed)
 {
  return countThisSession (sess.date, lawsuit.finished,
				   emptyFinishedAllowed) ;
 }  // bool countThisSession (const TSession & sess,
    //                        const TLawsuit & lawsuit,
    //                        const bool emptyFinishedAllowed)

bool thisIsFirstSession (const TSession & sess)
 {
  char crit [256] ;

  ostrstream (crit, sizeof (crit))
    << "SELECT COUNT (*) FROM T_SESSION WHERE "
	 "F_TYPE = \'" << sess.key.type << "\' AND "
	 "F_YEAR = \'" << sess.key.year << "\' AND "
	 "F_NO = " << sess.key.no << " AND "
	 "F_DATE < " << sess.date.Value() << " ;" << ends ;  // 15.apr.2003
  crit [sizeof (crit) - 1] = '\0' ;
   {
    long int howMany = 0 ;

    TGetGroup g(crit, new TLong("COUNT", &howMany));

    return (howMany == 0) ;
   }
 }  // thisIsFirstSession

bool ucnPresent (const char * const ucn)
 {
  return (ucn && ! strIsBlank (ucn) && atof (ucn) > 0.5) ;
 }  // ucnPresent

bool ucn_distributed (const char * const ucn)
 {
  return (ucnPresent(ucn) && cmp_str(ucn, INTER) != 0) ;
 }  // ucn_distributed

# if INSTANCE
bool enterDateNeed (const collBegDEndDBegN & cBegDEndDBegN)
 {
  return (cBegDEndDBegN.wantSortByDateReturned &&
	    cBegDEndDBegN.college == TYPE_FIRM_LAW) ;
 }  // enterDateNeed

# endif

bool timeInRange (const CTime & noon, const WhatPartOfDay part,
			const CTime time)
 {
  if (part == WholeDay)
    return true ;
  else
    if (part == Morning)
	return time < noon ;
    else
	return time >= noon ;
 }  // timeInRange

# if INSTANCE
# define RDT_07_sep_2000_patch
// Attention ! There is # undef below ...

void check_dec_res (char result)
 {
  if (! result)
    error ("sessionCancelsTheLaw/sessionDecidesTheLaw:\n"
	     "The 'result' data member of TDecision is zero!") ;
 }  // check_dec_res

bool sessionCancelsTheLaw (const TSession & sess,
				   const TLawsuit * const pLaw,
				   const bool accordingToSessDate,
				   const TDecision * const pDec,
				   const bool thereIsNotDecision)
 {
  if (! strchr (RESULT_FINALIZEDS, sess.result))
    return false ;
   {  // -- 0 --
    TLawsuit * pLocalLaw = pLaw ? NULL : new TLawsuit ;

    if (pLocalLaw)
     {
	pLocalLaw -> key = sess.key ;
	if (! (pLocalLaw -> Get ()))
	 {
	  delete pLocalLaw ;
	  return false ;
	 }
     }
    const TLawsuit & law = pLaw ? (* pLaw) : (* pLocalLaw) ;
    bool returnBeforeDecCheck = false ;
    bool lawCheckSaysCanceled = false ;

    if (law.finished.Empty () ||
	  ((accordingToSessDate ? sess.date : sess.returned) > law.finished)
	  || (law.decMade.Empty () == false &&  // 13.dec.2000
		sess.date < law.decMade) //13.dec.2000,02.mar.2001 '<=' to '<'
	 )
	returnBeforeDecCheck = true ;
    else
     {  // -- 1 --
#   if APPEAL
#   else  // APPEAL
	if (sess.result == RESULT_DEFINITION_TERMINATING)
	  if (resMustBeTreatAsDecision (RESULT_DEFINITION_TERMINATING))
	    error ("\"sessionCancelsTheLaw\" must be rewritten!") ;
	  else
	    if (strchr (KIND_1ST_LAWSUITS, law.kind))
		if (strchr (KIND_PUNISHMENT_LAWS, law.kind) == NULL)
		 {
#     if PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
		  if (law.grant == GRANT_NONE_YET || law.grant == GRANT_NULL)
		    if ((accordingToSessDate ? sess.date : sess.returned) >=
			  PATCH_04_AUG_2000_RDT_DATE)  // 28.sep.2000
		     {
			lawCheckSaysCanceled = true ;
			returnBeforeDecCheck = true ;
		     }
#     endif  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
		 }
#   if defined (RDT_07_sep_2000_patch)
		else
		 {
		  returnBeforeDecCheck = true ;
		 }
#   endif  // RDT_07_sep_2000_patch
#   endif  // APPEAL
     }  // -- 1 --
    if (pLocalLaw)
	delete pLocalLaw ;
    if (returnBeforeDecCheck)
	return lawCheckSaysCanceled ;
   }  // -- 0 --
  if (resMustBeTreatAsDecision (sess.result))
    return true ;
  if (sess.result != RESULT_DEFINITION_TERMINATING)
    if (strchr (RESULT_WITH_DECISIONS, sess.result))
	return false ;

  if (thereIsNotDecision)
    return false ;
  if (pDec)
   {
    check_dec_res (pDec -> result) ;
    return strchr (RESULT_CANCELAWS, pDec -> result) ;
   }
  else
    return SessionEndDocs (sess.key, sess.date,
				   sess.kind).GeneralCancelingResult () ;
 }  // sessionCancelsTheLaw

bool sessionDecidesTheLaw (const TSession & sess,
				   const TLawsuit * const pLaw,
				   const bool accordingToSessDate,
				   const TDecision * const pDec,
			   const bool thereIsNotDecision)
 {
  // 2008:299 LPR: oh fuck, why don't we just rewrite the reports?!
  if (sess.result == RESULT_PUBLIC_DECISION)
    return true;
  if (! strchr (RESULT_FINALIZEDS, sess.result))
    return false ;
  if (resMustBeTreatAsDecision (sess.result))
    return false ;
   {  // -- 0 --
    TLawsuit * pLocalLaw = pLaw ? NULL : new TLawsuit ;

    if (pLocalLaw)
     {
      pLocalLaw -> key = sess.key ;
      if (! (pLocalLaw -> Get ()))
       {
        delete pLocalLaw ;
        return false ;
       }
     }
    const TLawsuit & law = pLaw ? (* pLaw) : (* pLocalLaw) ;
    bool returnBeforeDecCheck = false ;
#   if defined (RDT_07_sep_2000_patch)
    bool sessCheckSaysDecided = false ;
#   endif  // RDT_07_sep_2000_patch

    if (law.finished.Empty () ||
        ((accordingToSessDate ? sess.date : sess.returned) > law.finished)
        || (law.decMade.Empty () == false &&  // 13.dec.2000
            sess.date < law.decMade) //13.dec.2000,02.mar.2001 '<=' to '<'
       )
      returnBeforeDecCheck = true ;
    else
     {  // -- 1 --
#   if APPEAL
#   else  // APPEAL
      if (sess.result == RESULT_DEFINITION_TERMINATING)
        if (resMustBeTreatAsDecision (RESULT_DEFINITION_TERMINATING))
          error ("\"sessionDecidesTheLaw\" must be rewritten!") ;
        else
          if (strchr (KIND_1ST_LAWSUITS, law.kind))
		if (strchr (KIND_PUNISHMENT_LAWS, law.kind) == NULL)
             {
#     if PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
              if (law.grant == GRANT_NONE_YET || law.grant == GRANT_NULL)
                if ((accordingToSessDate ? sess.date : sess.returned) >=
                    PATCH_04_AUG_2000_RDT_DATE)  // 28.sep.2000
                 {
                  returnBeforeDecCheck = true ;
                 }
#     endif  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
             }
#   if defined (RDT_07_sep_2000_patch)
            else
             {
              sessCheckSaysDecided = true ;  // 13.sep.2000
              returnBeforeDecCheck = true ;
             }
#   endif  // RDT_07_sep_2000_patch
#   endif  // APPEAL
     }  // -- 1 --
    if (pLocalLaw)
      delete pLocalLaw ;
    if (returnBeforeDecCheck)
#   if defined (RDT_07_sep_2000_patch)
      return sessCheckSaysDecided ;
#   else  // RDT_07_sep_2000_patch
      return false ;
#   endif  // RDT_07_sep_2000_patch  // 13.sep.2000
   }  // -- 0 --

// 25.jan.2001 - begin
//  if (RESULT_ANNOUNCE_TO_BE_DECIDED == sess.result)  // 04.dec.2000
//    return true ;  // 04.dec.2000
  if (sess.result != RESULT_DEFINITION_TERMINATING)
    if (strchr (RESULT_WITH_DECISIONS, sess.result))
      return true ;
// 25.jan.2001 - end

  if (thereIsNotDecision)
    return false ;
  if (pDec)
   {
    check_dec_res (pDec -> result) ;
    return ! strchr ( RESULT_CANCELAWS, pDec -> result) ;
   }
  else
    return SessionEndDocs (sess.key, sess.date,
                           sess.kind).GeneralCancelingResult () == false ;
 }  // sessionDecidesTheLaw

# if defined (RDT_07_sep_2000_patch)
# undef RDT_07_sep_2000_patch
# endif
# else  // of INSTANCE
bool sessionCancelsTheLaw (const TSession & sess,
			   const TLawsuit * const pLaw,
			   const bool accordingToSessDate,
			   const TDecision * const, const bool)
 {
  if (! strchr (RESULT_FINALIZEDS, sess.result))
    return false ;
   {  // -- 0 --
    TLawsuit * pLocalLaw = pLaw ? NULL : new TLawsuit ;

    if (pLocalLaw)
     {
      pLocalLaw -> key = sess.key ;
      if (! (pLocalLaw -> Get ()))
       {
        delete pLocalLaw ;
        return false ;
       }
     }
    const TLawsuit & law = pLaw ? (* pLaw) : (* pLocalLaw) ;
    bool returnBeforeDecCheck = false ;
    bool lawCheckSaysCanceled = false ;

    if (law.finished.Empty () ||
        ((accordingToSessDate ? sess.date : sess.returned) > law.finished)
       )
      returnBeforeDecCheck = true ;
    else
     {  // -- 1 --
      if (sess.result == RESULT_DEFINITION_TERMINATING)
        if (resMustBeTreatAsDecision (RESULT_DEFINITION_TERMINATING))
          error ("\"sessionCancelsTheLaw\" must be rewritten!") ;
        else
          if (strchr (KIND_PUNISHMENT_LAWS, law.kind) == NULL)
           {
#     if PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
            if (law.grant == GRANT_NONE_YET || law.grant == GRANT_NULL)
              if ((accordingToSessDate ? sess.date : sess.returned) >=
                  PATCH_04_AUG_2000_RDT_DATE)  // 28.sep.2000
               {
                lawCheckSaysCanceled = true ;
                returnBeforeDecCheck = true ;
               }
#     endif  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000 
           }
     }  // -- 1 --
    if (pLocalLaw)
      delete pLocalLaw ;
    if (returnBeforeDecCheck)
      return lawCheckSaysCanceled ;
   }  // -- 0 --
  return resMustBeTreatAsDecision (sess.result) ;
 }  // sessionCancelsTheLaw

bool sessionDecidesTheLaw (const TSession & sess,
			   const TLawsuit * const pLaw,
			   const bool accordingToSessDate,
			   const TDecision * const, const bool)
 {
  // 2008:299 LPR: oh fuck, why don't we just rewrite the reports?!
  if (sess.result == RESULT_PUBLIC_DECISION)
    return true;
  if (! strchr (RESULT_FINALIZEDS, sess.result))
    return false ;
// 04.aug.2000 - begin
   {  // -- 0 --
    TLawsuit * pLocalLaw = pLaw ? NULL : new TLawsuit ;

    if (pLocalLaw)
     {
      pLocalLaw -> key = sess.key ;
      if (! (pLocalLaw -> Get ()))
       {
        delete pLocalLaw ;
        return false ;
       }
     }
    const TLawsuit & law = pLaw ? (* pLaw) : (* pLocalLaw) ;
    bool returnBeforeDecCheck = false ;

    if (law.finished.Empty () ||
        ((accordingToSessDate ? sess.date : sess.returned) > law.finished)
       )
      returnBeforeDecCheck = true ;
    else
     {  // -- 1 --
      if (sess.result == RESULT_DEFINITION_TERMINATING)
        if (resMustBeTreatAsDecision (RESULT_DEFINITION_TERMINATING))
          error ("\"sessionDecidesTheLaw\" must be rewritten!") ;
        else
          if (strchr (KIND_PUNISHMENT_LAWS, law.kind) == NULL)
           {
#     if PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
            if (law.grant == GRANT_NONE_YET || law.grant == GRANT_NULL)
              if ((accordingToSessDate ? sess.date : sess.returned) >=
                  PATCH_04_AUG_2000_RDT_DATE)  // 28.sep.2000
               {
                returnBeforeDecCheck = true ;
               }
#     endif  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
           }
     }  // -- 1 --
    if (pLocalLaw)
      delete pLocalLaw ;
    if (returnBeforeDecCheck)
      return false ;
   }  // -- 0 --
  if (RESULT_ANNOUNCE_TO_BE_DECIDED == sess.result)  // 04.dec.2000
    return true ;  // 04.dec.2000
  return ! resMustBeTreatAsDecision (sess.result) ;
 }  // sessionDecidesTheLaw
# endif  // of INSTANCE

bool sessResOverridden_AbolishedMove (const TSession & sess,
                                      CDate & abolishingDate,
                           CDate * const pDateReturned  // 11.sep.2000
						 )
 {
  abolishingDate.year =
    abolishingDate.month = abolishingDate.day = '\0' ;
  if (pDateReturned)  // 11.sep.2000
    (* pDateReturned) = abolishingDate ;  // empty  // 11.sep.2000
  if (! strchr (RESULT_FINALIZEDS, sess.result))
    return false ;
//  law.finished.Empty () - this check is not correct enough (because
//  of the following sequence: decision, progress abolishing, decision (sets
//  the law.finished again)).

  char crit [256] ;

  ostrstream (crit, sizeof (crit))
    << "SELECT * FROM T_SESSION WHERE "
         "F_TYPE = \'" << sess.key.type << "\' AND "
         "F_NO = " << sess.key.no << " AND "
         "F_YEAR = " << sess.key.year << " AND "
	   "F_RESULT IN ("  << SQLCharSet(RESULT_UNDOS) << ") AND "
	   "F_DATE > " << sess.date.Value() << " "  // 15.apr.2003
	   "ORDER BY F_DATE, F_TIME ;" << ends ;
  crit [sizeof (crit) - 1] = '\0' ;

  TQuery q (crit) ;

  if (! q.Read ()) 
    return false ;
   {
    TSession * pS = new TSession ;

    (* pS) << q ;

    abolishingDate = pS -> date ;
    if (pDateReturned)  // 11.sep.2000
      (* pDateReturned) = pS -> returned ;  // 11.sep.2000
    delete pS ;  
   }
  return true ;
 }  // sessResOverridden_AbolishedMove

bool sessResOverridden_AbolishProgr (const TSession & s, TQuery * pQ,
                                     CDate * const pAbolishingDate,
                           CDate * const pDateReturned , // 11.sep.2000
                               TSession * pAbolishingSess  // 13.dec.2001
                                    )
 {
  if (pAbolishingSess)  // 13.dec.2001
    clearGroupData (* pAbolishingSess) ;  // 13.dec.2001
  if (pAbolishingDate)
    pAbolishingDate -> year =
      pAbolishingDate -> month = pAbolishingDate -> day = '\0' ;
  if (pDateReturned)  // 11.sep.2000
    (* pDateReturned) = (* pAbolishingDate) ;  // empty  // 11.sep.2000
  if (! pQ)
    return false ;
  if (! strchr (RESULT_FINALIZEDS, s.result))
    return false ;
  pQ -> Restart () ;

  bool found = false ;
   {  // -- new TSession --
    TSession * pLocS = new TSession ;  // locS ;  // 09.nov.2000

    while (! found && pQ -> Read ())
     {
      (* pLocS) << (* pQ) ;
//      if (pLocS -> date < s.date ||  // 02.mar.2001 - begin
//          (pLocS -> date == s.date && pLocS -> time < s.time))
      if (pLocS -> date <= s.date)  // 02.mar.2001 - end
        continue ;
      found = s.key.type == pLocS -> key.type &&
              s.key.year == pLocS -> key.year &&
              s.key.no == pLocS -> key.no ;
     }
    if (found)
     {
      if (pAbolishingDate)
       (* pAbolishingDate) = pLocS -> date ;
	if (pDateReturned)  // 11.sep.2000
        (* pDateReturned) = pLocS -> returned ;  // 11.sep.2000
      if (pAbolishingSess)  // 13.dec.2001 - begin
       {
        (* pAbolishingSess) << (* pLocS) ;
        if (DatedContainer_ (* pLocS).FilledOK ())
          if (pLocS -> Get ())
           (* pAbolishingSess) << (* pLocS) ;
       }  // 13.dec.2001 - end
     }
    delete pLocS ; 
   }  // -- new TSession --
  return found ;
 }  // sessResOverridden_AbolishProgr

bool strIsBlank (const char * s)
  // returns true if s is empty string or s contains only spaces
 {
  bool blank = true ;

  if (s)
   {
    unsigned long int i ;

    for (i = 0 ; blank && s [i] ; i ++)
      if (s [i] != ' ' || (long int) i == -1)
        blank = false ;
   }
  else
    error("%s\n%s", WRONG_ARGS_PASSED_TO, "strIsBlank");
  return blank ;
 }  // strIsBlank

bool strIsBlankOr160 (const char * s)
  // returns true if s is empty string or s contains only spaces
 {
  bool blank = true ;

  if (s)
   {
    unsigned long int i ;

    for (i = 0 ; blank && s [i] ; i ++)
	if ((s [i] != ' ' && s [i] != ((char) 160)) || (long int) i == -1)
	  blank = false ;
   }
  else
    error("%s\n%s%s", WRONG_ARGS_PASSED_TO, "strIsBlank", "Or160");
  return blank ;
 }  // strIsBlankOr160

bool law_is_returned_to_up_look_into (const TLawsuit & law)
 {  // 13.jun.2000
  bool res = false ;

  if (law.finished.Empty () == false)
   {
    char crit [128 + 128 + 16] ;

    ostrstream (crit, sizeof (crit))
      << "SELECT F_RESULT FROM T_SESSION WHERE "
      << "F_NO = " << law.key.no << " AND "
      << "F_YEAR = " << law.key.year << " AND "
      << "F_TYPE = \'" << law.key.type << "\' AND "
      << "F_RESULT IN ("
          << stringToSQLCharSet (RESULT_TO_UP_LOOK_INTO_RETURNEDS).
               getTheSet ()
# if INSTANCE  // 21.jul.2000
                     << ",\'" << RESULT_SEND << "\'" // 21.jul.2000
# endif  // 21.jul.2000
			<< ") AND "
	<< "F_DATE <= " << law.finished.Value() << " ;" << ends ;  // 15.apr.2003
    crit [sizeof (crit) - 1] = '\0' ;
    if (crit [0] && crit [strlen (crit) - 1] == ';')
     {
	TQuery q (crit) ;

	res = q.Read () ;
     }
   }
  return res ;
 }  // law_is_returned_to_up_look_into

# if INSTANCE
#   if APPEAL
bool law_is_pre_court (const TLawsuit & rLawsuit)
 {
  if (rLawsuit.key.type != TYPE_PUNISHMENT_LAW)
    return false ;
  return strchr (KIND_PRIVATE_PUN_C_O_INDOCS, rLawsuit.sourceKind) ;
 }  // law_is_pre_court

#   else  // of APPEAL
static bool preliminary(long subject)
{
	TSubject tSubject;

	tSubject.subject = subject;
	tSubject.type = TYPE_PUNISHMENT_LAW;

	return tSubject.Try("F_ATTRIB_I, F_ATTRIB_II") &&
		(strchr(tSubject.sd[0].attrib, POST_PRECOURT) || strchr(tSubject.sd[1].attrib, POST_PRECOURT));
}

bool law_is_pre_court (const TLawsuit & rLawsuit)
 {
  if (rLawsuit.key.type != TYPE_PUNISHMENT_LAW)
    return false ;
  if (strchr (KIND_1ST_PUNISHMENT_LAWS, rLawsuit.kind))
    return (rLawsuit.kind == KIND_PRIVATE_PUNISHMENT_LAW &&
		preliminary(rLawsuit.subject));
  else
    return (preliminary(rLawsuit.subject));
 }  // law_is_pre_court

#   endif  // of APPEAL
# else  // of INSTANCE
static bool preliminary(long subject)
{
	TSubject tSubject;

	tSubject.subject = subject;
	tSubject.type = TYPE_PUNISHMENT_LAW;
	return tSubject.Try("F_ATTRIB_I") && strchr(tSubject.sd[0].attrib, POST_PRECOURT);
}

bool law_is_pre_court (const TLawsuit & rLawsuit)
 {
  if (rLawsuit.key.type != TYPE_PUNISHMENT_LAW)
    return false ;
  return (rLawsuit.kind == KIND_PRIVATE_PUNISHMENT_LAW &&
	    preliminary(rLawsuit.subject));
 }  // law_is_pre_court

# endif  // of INSTANCE

#if 20160060
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	bool law_is_237_enactment (const TLawsuit & rLawsuit) { return rLawsuit.sourceKind == KIND_CANCEL_ENACTMENT; }
	#endif  // APPEAL || AREA || MILITARY

	#if COURT_TYPE == COURT_ADMIN
	bool law_is_237_enactment (const TLawsuit & ) { return false; }
	#endif  // ADMIN

	#if COURT_TYPE == COURT_REGIONAL
	bool law_is_237_enactment (const TLawsuit & rLawsuit) { return rLawsuit.subject == SUBJECT_CANCEL_ENACTMENT; }
	#endif  // REGIONAL
#else  // 20160060
	# if INSTANCE
	#   if APPEAL
	bool law_is_237_enactment (const TLawsuit & rLawsuit)
	 {
	  return rLawsuit.key.type == TYPE_PUNISHMENT_LAW &&
		   rLawsuit.sourceKind == KIND_CANCEL_ENACTMENT ;
	 }  // law_is_237_enactment

	#   else  // of APPEAL
	bool law_is_237_enactment (const TLawsuit & rLawsuit)
	 {
	  return rLawsuit.kind == KIND_PRIVATE_PUNISHMENT_LAW &&
		   rLawsuit.subject == SUBJECT_1905 ;	// 2016:060 only in AREA?
	 }  // law_is_237_enactment

	#   endif  // of APPEAL
	# else  // of INSTANCE
	bool law_is_237_enactment (const TLawsuit &)
	 {
	  return false ;			// 2016:060 should be 1488?
	 }  // law_is_237_enactment


	# endif  // of INSTANCE
#endif  // 20160060

bool criteria_finished_ok (const char * const criteria,
				   const char * const comment)
 {
  const char * const pref = comment ? " (for the " : "" ;
  const char * const comm = comment ? comment : "" ;
  const char * const suff = comment ? ")" : "" ;
  const char * const checkName = "criteria_finished_ok" ;

  if (! criteria)
    return error ("%s: the criteria parameter is NULL"
                  "%s%s%s.", checkName, pref, comm, suff) ;

  const size_t l = strlen (criteria) ;

  if (l && criteria [l - 1] == ';')
    return true ;
  else
    return error ("%s failed%s%s%s.", checkName, pref, comm, suff) ;
 }  // criteria_finished_ok

bool criteria_finished_ok(const char* const criteria,
	const char* const className, const char* const methodName)
{
	if(methodName == NULL)
		return criteria_finished_ok(criteria, className);
	else
	{
		const char* const cN = className ? className : "?";
		const char * const pref = " (for the ";
		const char * const suff = ")";
		const char * const checkName = "criteria_finished_ok";

		if(! criteria)
			return error("%s: the criteria parameter is NULL"
         	"%s%s::%s%s.", checkName, pref, cN, methodName, suff);

		const size_t l = strlen (criteria);

		if(l && criteria[l - 1] == ';')
			return true ;
		else
			return error("%s failed%s%s::%s%s.",
	    checkName, pref, cN, methodName, suff);
	}
}  // criteria_finished_ok

bool judges_membership_ok (const char lawKind,
                           const BriefSessionsToCheckGroup & sess,
                           const bool closedIsAlwaysOK)
 {
  const int expect = judges_count_expecting (lawKind, sess.kind,
                                             closedIsAlwaysOK) ;

  return expect ? expect == judges_in_session (sess) : true ;
 }  // judges_membership_ok

bool judges_membership_ok_spk (const char lawKind,
                               const BriefSessionsToCheckGroup & sess,
                               const bool closedIsAlwaysOK,
					 outText * pOfs, const bool errMsg
                              )
 {
  const int expect = judges_count_expecting (lawKind, sess.kind,
							   closedIsAlwaysOK) ;
  const bool res = expect ? expect == judges_in_session (sess) : true ;

  if (res == false)
   {
    SessionErrors sE = SE_UndefinedError ;

    switch (expect)
     {
	case 1 :
	  sE = SE_WrongMembership1 ;
	  break ;
	case 2 :
	  sE = SE_WrongMembership2 ;
	  break ;
	case 3 :
	  sE = SE_WrongMembership3 ;
	  break ;
     }
    bad_session (sess, sE, pOfs, errMsg) ;
   }
  return res ;
 }  // judges_membership_ok_spk

// 2015:027 LPR/TRQ/IRQ: rewritten
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
bool bankruptcy_law (const TLawsuit &law)
{
	// 2015:099 LPR/TRQ/IRQ: was KIND_INCONSISTENCE_INDOCS
	return strchr(KIND_INCONSISTENCE_REQUESTS, law.sourceKind);
}
#endif  // APPEAL || AREAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
bool bankruptcy_law (const TLawsuit &)
 {
  return false ;
 }  // bankruptcy_law
#endif  // ADMIN || MILITARY || REGIONAL

bool string_is_upper (const char * const str)
 {
  if (! str)
    return false ;

  bool isUpper = true ;
  const size_t l = strlen (str) ;

  if (l < 1)
    return false ;
  for (size_t i = 0 ; isUpper == true && i < l ; i ++)
    if (str [i] != upCyr (str [i]))
      if (str [i] != toupper (str [i]))
        isUpper = false ;
  return isUpper ;
 }  // string_is_upper

bool reminding_notice(const TSideWind& w, CDate* const pFirstDate,
  const bool errMsg)
{
  bool reminding = false;
  CDate firstSent;

  if(pFirstDate)
  {
    clear_CDate(*pFirstDate);
  }
  {
    TSubpoena subp;

    subp.SetOrdinary(w);
    {
	msql m;

	subp.FloatCriteriaByOrdinary(m);
	{
	  TQuery q(str(m));
	  while(q.Read())
	  {
	    subp << q;
	    SubpoenaKind->Seek(subp.subpoenaKind);

		if(SubpoenaKind->IsNotification())
		{
		  if(Today != subp.subpoenaDate) // Any today's sending can't
		    // be a "previous notification"
		  {
		    reminding = true;
		    if(firstSent.Empty() || subp.subpoenaDate < firstSent)
		    {
			firstSent = subp.subpoenaDate;
		    }
		  }
		}
	  }
	}
    }
  }
  if(reminding == true)
  {
    if(pFirstDate)
    {
      (*pFirstDate) = firstSent;
    }
    if(firstSent > Today && errMsg == true)
    {
      CharPtr tmp(1024);

      ostrstream(tmp, tmp.Size())
        << "Текущата дата е " << Today << ", а има "
           "изпратено СРГ на " << firstSent << ". Съобщението, което сега "
           "изпращате ще бъде НАПОМНИТЕЛНО.";
      tmp.End();
      error(tmp);
    }
  }
  return reminding;
}  // reminding_notice

bool valid_surroundment (const TSurroundment & ment,
                         TSurround * const pSurr,
				 TSession * const pSess,
                         const bool sessIsOK)
 {
  bool surroundOK ;

  if (pSurr)
    surroundOK = surroundment_to_surround (ment, * pSurr) ;
  else
   {
    TSurround tmp ;

    surroundOK = surroundment_to_surround (ment, tmp) ;
   }

  bool sessionOK = sessIsOK ;

  if (sessionOK == false)
   {
    if (pSess)
	sessionOK = surroundment_to_session (ment, * pSess) ;
    else
     {
      SessPtr pS ;

      sessionOK = surroundment_to_session (ment, pS.Ref ()) ;
     }
   }

  const bool res = surroundOK && sessionOK ;

  if (res == false)
   {
    char tmp [256] ;

    surroundment_to_string (ment, tmp, sizeof (tmp)) ;
    error ("Връзката\n""%s""\nе невалидна.", tmp) ;
   }
  return res ;
 }  // valid_surroundment

bool session_by_surrounds (TSession * pSess)
 {
  bool res = false ;

  if (pSess)
   {
    TGArray<TSurround> forSyntaxUse ;

    res = session_to_surrounds (* pSess, forSyntaxUse) ;
   }
  return res ;
 }  // session_by_surrounds

bool session_by_surround (const TSurround * pSurround, TSession * pSess)
 {
  bool res = false ;

  if (pSurround && pSess)
   {  // -- 0 --
    SurroundmentPtr pMent ;

    if (surroundment_key_data (* pMent,
                               pSurround -> key, pSurround -> kind,
                               pSess -> key, pSess -> date, pSess -> kind))
      if (pMent.Ref ().Try ())
        res = true ;
   }  // -- 0 --
  return res ;
 }  // session_by_surround

bool decision_by_surround (const TSurround * pSurround,
                           const TDecision * pDec)
 {
  bool res = false ;

  if (pSurround && pDec)
   {
    SessPtr pSess ;

    pSess.Ref ().key = pDec -> key ;
    pSess.Ref ().kind = pDec -> sessionKind ;
    pSess.Ref ().date = pDec -> date ;
    if (DatedContainer_ (* pSess).FilledOK ())
      if (pSess.Ref ().Get ())
        if (session_by_surround (pSurround, pSess))
          res = true ;
   }
  return res ;
 }  // decision_by_surround

bool inreg_by_surround (const TSurround * pSurround,
                        const TInReg * pInReg,
                        const bool includingNoSpecification)
 {
  bool res = false ;

  if (pSurround && pInReg)
   {
    TRCDKeyContainer_ foundSurr ;
    AloneDatedContainer_xK foundSess ;
    const bool lawTypeOK =
      inreg_to_law_type ((TInReg &) * pInReg, NULL, & foundSess, & foundSurr) ;

    if (lawTypeOK)
      if (foundSurr.FilledOK ())
        if (foundSurr == TRCDKeyContainer_ (pSurround -> key.type,
                                            pSurround -> key.no,
                                            pSurround -> key.year,
                                            pSurround -> kind))
          res = true ;
    if (res == false && includingNoSpecification)
      if (lawTypeOK)
        if (foundSurr.FilledOK () == false)  // No surround specified
          if (DatedContainer_ (foundSess).FilledOK ())  // Session found
	     {
            SessPtr pSess ;

            (* pSess) << foundSess ;
            if (session_by_surround (pSurround, pSess))
             res = true ;
           }
   }
  return res ;
 }  // inreg_by_surround

bool inreg_by_session (const TSession * pSess, const TInReg * pInReg)
 {
  bool res = false ;

  if (pSess && pInReg)
   {
    AloneDatedContainer_xK foundSess ;

    res = inreg_to_law_type ((TInReg &) * pInReg, NULL, & foundSess) &&
          DatedContainer_ (foundSess).FilledOK () &&
          DatedContainer_ (foundSess) == DatedContainer_ (* pSess) ;
   }
  return res ;
 }  // inreg_by_session

bool surr_kind_and_waln_state_match (const char surroundKind,
                                     const char walnState)
 {
  bool res = false ;

  if (surroundKind && walnState)
   {
    char tmpWS = '\0' ;
    const bool convertedOK =
      surround_kind_to_waln_state (surroundKind, tmpWS) ;

    res = convertedOK && tmpWS == walnState ;
   }
  return res ;
 }  // surr_kind_and_waln_state_match

bool surr_ment_res_and_act_kind_match (const char surroundmentResult,
                                       const char actKind)
 {
  bool res = false ;

  if (surroundmentResult && actKind)
   {
    if (is_decision_kind (actKind))
     {
      if (surroundmentResult != RESULT_SURROUNDED)
        if (strchr (KIND_DECISIONS_BY_NAME, actKind) &&
		strchr (RESULT_SURROUNDMENT_DECS, surroundmentResult))
	    res = true ;
	  else
	    if (strchr (KIND_DEFINITIONS_BY_NAME, actKind) &&
		  strchr (RESULT_SURROUNDMENT_SETTS, surroundmentResult))
		res = true ;
# if INSTANCE
	    else
		if (strchr (KIND_DECREES_BY_NAME, actKind) &&
                strchr (RESULT_SURROUNDMENT_ORDERS, surroundmentResult))
              res = true ;
# endif  // of INSTANCE
     }
    else
      if (is_session_kind (actKind))
       {
        if (surroundmentResult == RESULT_SURROUNDED)
          res = true ;
        else
	    if (strchr (RESULT_SURROUNDMENT_DECS,
			    surroundmentResult) == NULL)
		if (strchr (RESULT_SURROUNDMENT_SETTS,
				surroundmentResult) == NULL)
# if INSTANCE
		  if (strchr (RESULT_SURROUNDMENT_ORDERS,
				  surroundmentResult) == NULL)
# endif  // of INSTANCE
                res = true ;
       }
   }
  return res ;
 }  // surr_ment_res_and_act_kind_match

bool outrg_retLawToLowerInst_weakCond(const TOutReg& outReg,
	const bool checkWalns)
{
	const bool kindOk = outReg.kind == KIND_OUT_LETTER;
	const bool noInNoOk = outReg.inNo == 0 && outReg.inYear == 0;
	const bool concernsALawOk =
		outReg.lawsuit.type && strchr(TYPE_LAWSUITS, outReg.lawsuit.type) &&
		TRCDKey_(outReg.lawsuit).FilledOK();
	const bool noReceiverOk = ucnPresent(outReg.receiver) == false;
	bool regionOk = false;
	bool res = false;

	if(concernsALawOk)
	{
	  LawPtr pLaw;

	  pLaw.Ref().key = outReg.lawsuit;
	  if(pLaw.Ref().Get())
    {
	   // 20061228  if(pLaw.Ref().regionKind && pLaw.Ref().regionYear &&
		 //	20061228  pLaw.Ref().regionNo)
     //	20061228  regionOk = true;
     // 20061228 -->  търсим Първоинстанционно дело ?

        regionOk = connect_type_in( pLaw, CONNECT_TYPE_LOWER_INSTANCES);

     // 20061228 <--
    }
	}
	if(kindOk && noInNoOk && concernsALawOk && noReceiverOk && regionOk)
	{
		const bool dcFilled =
			DatedContainer_(outReg.key.type, outReg.key.no, outReg.key.year,
				outReg.kind, outReg.date).FilledOK();
		bool noWalnsOk = dcFilled == false || checkWalns == false ;

		if (noWalnsOk == false)
		{
			if(dcFilled)
			{
				TSideWind w;
				CharPtr crit(1024);

				w.key = outReg.key;
				w.kind = outReg.kind;
				w.date = outReg.date;
				w.FloatCriteria(crit);
				{
					TQuery q(crit);

					if(q.Read() == false)
						noWalnsOk = true;
				}
			}
			else
				noWalnsOk = true;
		}
		if(noWalnsOk == true)
			res = true;
	}
	return res;
}  // outrg_retLawToLowerInst_weakCond

bool is_default_post_admin()
{
  return Default->post == POST_ADMIN;
}

//  Quick / immediate / standard  ->
//  without conversion
bool quick_pure_standard(const long quick)   //new 20060324  not in use
{
  return (quick == QUICK_NULL);
}

bool quick_pure_quick(const long quick)
{
  return (quick == QUICK_QUICK);
}

bool quick_pure_immediate(const long quick)
{
  return (quick == QUICK_IMMEDIATE);
}

// with conwersion
// from standard to ...
bool quick_formerly_standard_to_quick (const long quick)  //new 20060324
{
  return (    (( quick & QUICK_QUICK ) == QUICK_QUICK )
		  &&
		  (( quick & QUICK_CONVERTED ) == QUICK_CONVERTED )
		  &&
		  (( quick & QUICK_IMMEDIATE ) == 0 )
	   );
}

bool quick_formerly_standard_to_immediate (const long quick)  //new 20060324
{
  return (    (( quick & QUICK_QUICK ) == QUICK_QUICK )
              &&
              (( quick & QUICK_CONVERTED ) == QUICK_CONVERTED )
              &&
              (( quick & QUICK_IMMEDIATE ) == QUICK_IMMEDIATE )
         );
}

// from  ... to standard
bool quick_formerly_quick_to_standard (const long quick)  //new 20060324
{
  return (   (((quick & QUICK_QUICK) == 0 )&&
              ((quick & QUICK_IMMEDIATE) == 0))
             &&
             ((quick & QUICK_CONVERTED) == QUICK_CONVERTED)
         );
}

bool quick_formerly_immediate_to_standard (const long quick)  //new 20060324
{
  return (   (((quick & QUICK_QUICK) == 0 )&&
              ((quick & QUICK_IMMEDIATE) == QUICK_IMMEDIATE))
             &&
             ((quick & QUICK_CONVERTED) == QUICK_CONVERTED)
         );
}

// currently
bool quick_currently_standard(const long quick)
{
  return (   (quick == QUICK_NULL)
             ||
             ( ((quick & QUICK_CONVERTED)==QUICK_CONVERTED)&&
               ((quick & QUICK_QUICK) == 0)
             )
         );
}

bool quick_currently_quick(const long quick) // the same as quick_somehow_quick
{
// from Petyo
//  return ((quick & QUICK_QUICK) == QUICK_QUICK);
  return (   ( ((quick & QUICK_QUICK) == QUICK_QUICK )&&
               ((quick & QUICK_IMMEDIATE) == 0)
             )
             &&
             ( ((quick & QUICK_CONVERTED) == 0)||
               ((quick & QUICK_CONVERTED) == QUICK_CONVERTED)
             )
         );
}
bool quick_currently_immediate (const long quick)//new 20060324
{
  return (   ( ((quick & QUICK_QUICK) == QUICK_QUICK )&&
               ((quick & QUICK_IMMEDIATE) == QUICK_IMMEDIATE)
		 )
             &&
             ( ((quick & QUICK_CONVERTED) == 0)||
               ((quick & QUICK_CONVERTED) == QUICK_CONVERTED)
             )
         );
}

bool quick_law_source_currently_quick(const TRCDKey source, TRequest& tmpReq)
{
  bool quickProc = false;

  if(source.type == TYPE_REQUEST && TRCDKey_(source).FilledOK())
  {
    tmpReq.key = source;
    if(tmpReq.Try())
	  if (  ((tmpReq.quick & QUICK_QUICK) == QUICK_QUICK )&&       // there is no conversion
		  ((tmpReq.quick & QUICK_IMMEDIATE) == 0))
		quickProc = true;
  }
  return quickProc;
}

bool quick_law_source_currently_quick(const TRCDKey source)
{
  TRequest tmpReq;

  return quick_law_source_currently_quick(source, tmpReq);
}

//  Quick / immediate / standard <-

int compare_side_winds(const TSideWind& sw0, const TSideWind& sw1)
{
  int res = 0;

  if(sw0.key.type < sw1.key.type)
    res = -1;
  else
    if(sw0.key.type > sw1.key.type)
	res = 1;
    else
	if(sw0.key.year < sw1.key.year)
	  res = -1;
	else
	  if(sw0.key.year > sw1.key.year)
	    res = 1;
	  else
	    if(sw0.key.no < sw1.key.no)
		res = -1;
	    else
		if(sw0.key.no > sw1.key.no)
		  res = 1;
		else
		  if(sw0.kind < sw1.kind)
		    res = -1;
		  else
		    if(sw0.kind > sw1.kind)
			res = 1;
		    else
			if(sw0.date < sw1.date)
			  res = -1;
			else
			  if(sw0.date > sw1.date)
			    res = 1;
			  else
			    if(sw0.ucnType < sw1.ucnType)
				res = -1;
			    else
                        if(sw0.ucnType > sw1.ucnType)
                          res = 1;
                        else
                        {
                          const int ucnCmpRes = strcmp(sw0.ucn, sw1.ucn);

                          if(ucnCmpRes < 0)
                            res = -1;
                          else
                            if(ucnCmpRes > 0)
                              res = 1;
                            else
                              if(sw0.involvement < sw1.involvement)
                                res = -1;
                              else
                                if(sw0.involvement > sw1.involvement)
                                  res = 1;
                        }
  return res;
}  // compare_side_winds

static void fill_notified_mark_text(const char ucnType, const char* const ucn, const char mark, const CDate &subpoenaDate,
	const CDate &subpoenaReceived, mstr &m, char receivedStatus)
{
	size_t preLen = m.len();

	if (strchr(UCN_CITIZENS, ucnType))
		citizenUCNAndUCNTypeToName(ucn, ucnType, m, true);	// MarkTextParams::FatherNameShort()
	else
		ucnAndUcnTypeToName(ucn, ucnType, m);

	m.setlen(preLen + LENGTH_OF_SIDE_MARK_TEXT_NAME);	// MarkTextParams::NameLen()
	m.printf(" %c ", mark);

	// 2017:005 LPR/IRQ: unify
	subpoenaDate.Print(m, "%0D");
	// 2016:358 LPR/IRQ: improve status
	if (strchr(SRS_DELIVEREDS, receivedStatus))
	{
		TReceivedStatus srs;
		srs.receivedStatus = receivedStatus;

		if (srs.Get("F_ALIAS"))
			m.printf(" %s", srs.alias);
	}
	subpoenaReceived.Print(m, " на %0D");
}

static char side_wind_notified_mark_ll(TSideWind *sideWind, char eventStatus, mstr &m, bool fill)
{
	TSubpoena subpoena;
	msql w(subpoena.Name, "F_SUBPOENA_KIND, F_SUBPOENA_DATE, F_SUBPOENA_RECEIVED, F_RECEIVED_STATUS");

	w.Add(sideWind, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
	w.AddChar("F_EVENT_STATUS", eventStatus);

	// 2016:358 LPR/IRQ: improve status
	w.AddOrder("F_SUBPOENA_RECEIVED DESC, F_SUBPOENA_NO DESC");

	char mark = ' ';
	TQuery q(str(w));

	while (q.Read())
	{
		mark = '~';
		subpoena << q;
		SubpoenaKind->Seek(subpoena.subpoenaKind);

		if (SubpoenaKind->IsNotification())
		{
			switch (SubpoenaKind->proc)
			{
				case SK_PROC_ANNOUNCE :
				{
					mark = strchr(KIND_SESSIONS, sideWind->kind) && eventStatus == RESULT_FIXED ? 'П' : 'С';
					break;
				}
				case SK_PROC_NOTIFYING : mark = 'У'; break;
				case SK_PROC_LETTER_TO : mark = 'п'; break;
				case SK_PROC_TELEGRAM : mark = 'Т'; break;
				case SK_PROC_STATE_PAPER : mark = 'Д'; break;
				case SK_PROC_DELIVER_COPY : mark = 'В'; break;
			}

			if (mark != '~')
				break;
		}
	}

	if (fill && !isspace(mark))
	{
		fill_notified_mark_text(sideWind->ucnType, sideWind->ucn, mark, subpoena.subpoenaDate, subpoena.subpoenaReceived,
			m, subpoena.receivedStatus);
	}

	return mark;
}

char side_wind_notified_mark(TSideWind *sideWind, char eventStatus, mstr &m)
{
	return side_wind_notified_mark_ll(sideWind, eventStatus, m, true);
}

char side_wind_notified_mark(TSideWind *sideWind, char eventStatus)
{
	return side_wind_notified_mark_ll(sideWind, eventStatus, mstr(), false);
}

bool sess_result_appealed(DatedContainer_ sessKey,
  TIArray<DatedContainer_>* pInRegKeys)
{  // 31.jan.2005
  bool res = false;
  FUNC_NAME("sess_result_appealed");

  if(sessKey.FilledOK())
  {
    CharPtr crit(512);

    ostrstream(crit, crit.Size())
	<<
	"SELECT F_TYPE, F_YEAR, F_NO, F_KIND, F_DATE "
      "FROM T_INREG WHERE "
      "F_DECISION_TYPE = '" << sessKey.type << "' AND "
      "F_LAWSUIT_TYPE = '" << sessKey.type << "' AND "
      "F_LAWSUIT_YEAR = " << sessKey.year << " AND "
      "F_LAWSUIT_NO = " << sessKey.no << " AND "
	"F_DECISION_DATE = " << sessKey.date.Value() << " AND "
      "F_SESSION_KIND = '" << sessKey.kind << "'" << " ;"
      << ends;
    crit.End();
    if(criteria_finished_ok(crit, funcName))
    {
      TQuery q(crit);

      if(q.Read())
      {
        if(pInRegKeys)
        {
          TInReg inReg;

          // Add the first appealing key to array
          DatedContainer_* pNewDtCnt = new DatedContainer_();

          inReg << q;
	    *pNewDtCnt = DatedContainer_(inReg);
          pInRegKeys->Add(pNewDtCnt);

          // Add next appealing keys to array (if any)
          while(q.Read())
          {
            DatedContainer_* pNewDtCnt = new DatedContainer_();

            inReg << q;
            *pNewDtCnt = DatedContainer_(inReg);
            pInRegKeys->Add(pNewDtCnt);
          }
        }
        res = true;
      }
    }
  }
  else
    REPORT_F_INVALID_ARGS;
  return res;
}  // sess_result_appealed

bool default_split_wide_hyper_docs()
{  // 08.feb.2005
  bool res = false;
  const int defaultSplit = (WINDOWS_PRINT != 0 && WINDOWS_A3 == 0) ? 1 : 0;

  if(defaultSplit)
  {
    res = true;
  }
  return res;
}  // default_split_wide_hyper_docs
