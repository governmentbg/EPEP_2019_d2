#include "cac.h"  // 18.may.2001
//# include "all.h"
                                                  
bool composeJudgeCheckupCriteria (char * const dest, size_t destSz,
                                  const char * const what,
                                  const char * const typesString,
                                  const long int composition,
                                  const CDate * const pHighRange,
                                  const CDate * const pLowRange,
                                  const char * const resultsString,
                                  const char * const requestKindsString,
                                  const char * const ageString,
                                  const bool finishedTest,
                                  const char * const sourceTypesString,
                                  const char * const grantedsString,
                                  const char * const sessKindsString,
                                  const bool sKMatch,
                                  const bool excludeCreatedsLater,
                                  const bool rSMatch,
                                  const bool sessDateTest,
                                  const bool bindSessDateAndFinished,
                                  const char * const decisResultsString,
				     const bool withRequestKindsString,
                const char * const lawKindsString,  // 08.apr.2002 - begin
                                 const bool lKMatch,  // 08.apr.2002 - end
                     const begMYEndMYTypeCIU* pBMYEMYTCIU   // 20.jul.2004
                                 )
 {
  if (dest && destSz && what && pHighRange &&
      typesString && typesString [0]
     )
   {
    stringToSQLCharSet rTBS (resultsString) ;
    stringToSQLCharSet kRS (requestKindsString) ;
    stringToSQLCharSet sTS (sourceTypesString) ;
    stringToSQLCharSet gS (grantedsString) ;
    stringToSQLCharSet sKS (sessKindsString) ;
    ostrstream oss (dest, destSz - 1) ;
    const bool lookInDecisions =
      decisResultsString && decisResultsString [0] ;
    const bool lookInSessions = resultsString || sessKindsString ||
                                strstr (what, "S.") ||
				strstr (what, "s.") || lookInDecisions
      // Will work slowly, but
      // else I have to copy the date dependencies and checks  :-(
                                  ;

    SUPPRESS_UNUSED(pBMYEMYTCIU);
    oss
      << "SELECT " << what << " FROM T_LAWSUIT L " ;
    if (lookInSessions)
      oss << ", T_SESSION S " ;
    if (lookInDecisions)
      oss << ", T_DECISION D " ;
    oss <<
           "WHERE "
             << "L.F_TYPE IN ("
             << stringToSQLCharSet (typesString).getTheSet () << ") " ;
# if INSTANCE
    oss <<      "AND L.F_TYPE != \'" << TYPE_FIRM_LAW << "\' " ;
# endif
    if (sourceTypesString && sourceTypesString [0])
      oss    << "AND L.F_SOURCE_TYPE IN (" << sTS.getTheSet ()
             << ") " ;
    if (grantedsString && grantedsString [0])
      oss    << "AND L.F_GRANTED IN (" << gS.getTheSet () << ") " ;
    if (composition)
      oss    << "AND L.F_COMPOSITION = " << composition << " " ;
    if (finishedTest)
	if (pLowRange)
        oss <<
		   "AND L.F_FINISHED "
		     "BETWEEN " << (* pLowRange).Value()
			  << " AND " << (* pHighRange).Value() << " " ;
	else
	  oss << "AND (L.F_FINISHED IS NULL OR "
			<< "L.F_FINISHED" << " >= "
			<< (* pHighRange).Value() << ") " ;  // 15.apr.2003 - end
    else
	if (! lookInSessions)
	  if (pLowRange)
	    oss <<
		     "AND ""L.F_DATE "
			 "BETWEEN " << (* pLowRange).Value()
			    << " AND " << (* pHighRange).Value() << " " ;
	  else
	    oss <<  "AND (L.F_DATE" << " >= "
                    << (* pHighRange).Value() << ") " ;  // 15.apr.2003 - end
    if (excludeCreatedsLater)
      oss <<
           "AND "
             << "L.F_DATE < " << (* pHighRange).Value() << " " ;  // 15.apr.2003
    if (ageString && ageString [0])
      oss <<
           "AND "
             << "L.F_AGE IN ("
             << stringToSQLCharSet (ageString).getTheSet ()
                        << ") " ;
    if (lawKindsString &&
        lawKindsString [0]  // lawKindsString  // 09.jan.2003
       )  // 08.apr.2002 - begin
     {
      oss <<
           "AND "
             << "L.F_KIND " << (lKMatch ? "" : "NOT ") << "IN ("
             << SQLCharSet (lawKindsString) << ") " ;
     }  // 08.apr.2002 - end
    if (requestKindsString && requestKindsString [0])
     {
      if (withRequestKindsString)
        oss <<
           "AND " <<
             "L.F_SOURCE_TYPE = \'" << TYPE_REQUEST << "\' " ;
      oss <<
           "AND " <<
             "L.F_SOURCE_KIND IN (" << kRS.getTheSet () << ") " ;
     }
    if (lookInSessions)
     {  // -- 0 --
      oss <<
           "AND "
             "S.F_NO = L.F_NO "
           "AND "
             "S.F_YEAR = L.F_YEAR "
           "AND "
             "S.F_TYPE = L.F_TYPE " ;
      oss <<
         "AND "
           "S.F_KIND != \'" << KIND_RESOLUTION_SESSION << "\' " ;
// 08.jun.2000 - begin
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    oss <<
          "AND "
            "S.F_RESULT != \'" << RESULT_DECREE << "\' " ;
#   endif  // APPEAL
# endif  // INSTANCE
// 08.jun.2000 - end
// 22.feb.2000 - begin
      oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_UNFIXED << "\' " ;
// 22.feb.2000 - end
//20050322
      oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_ABORTED << "\' " ;

# if JUDGE_CHKUP_EXCLUDE_RES_FIXED  // 25.jan.2001 - begin
      oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_FIXED << "\' " ;
# endif  // of JUDGE_CHKUP_EXCLUDE_RES_FIXED  // 25.jan.2001 - end
# if ! INSTANCE
      oss <<
         "AND "
	   "S.F_KIND != \'" << KIND_LISTENING_SESSION << "\' " ;
# endif
      if (bindSessDateAndFinished)
       {
	oss <<
	     "AND (" ;
	if (sessDateTest)
	  oss << "L.F_FINISHED IS NULL OR " ;
	else
	  oss << "L.F_FINISHED IS NOT NULL AND " ;
	oss <<   "S.F_DATE <= L.F_FINISHED) " ;
       }
// 17.apr.2000 - begin
      if (finishedTest)
	oss <<
	       "AND (L.F_DEC_MADE IS NULL OR "
		    "L.F_DEC_MADE <= S.F_DATE) " ; //02.mar.2001 '<'->'<='
// 17.apr.2000 - end
      // 2008:299
      if (finishedTest)
      	oss << "AND NOT S.F_FINAL IS NULL ";
      if (resultsString)
	if (resultsString [0])
	  oss <<
	     "AND "
	       "S.F_RESULT " << (rSMatch ? "" : "NOT ")
			     << "IN (" << rTBS.getTheSet () << ") " ;
	if (sessDateTest)
	    if (pLowRange)
            oss <<
			 "AND S.F_DATE "
			   "BETWEEN " << (* pLowRange).Value()
                        << " AND " << (* pHighRange).Value() << " " ;
          else
		oss <<  "AND S.F_DATE" << " >= "
                      << (* pHighRange).Value() << " " ;  // 15.apr.2003 - end
	if (sessKindsString && sessKindsString [0])
        oss <<
           "AND "
             "S.F_KIND " << (sKMatch ? "" : "NOT ")
                         << "IN (" << sKS.getTheSet () << ") " ;
      if (lookInDecisions)
       {  // -- 1 --
        oss <<
           "AND "
             "D.F_NO = S.F_NO "
           "AND "
             "D.F_YEAR = S.F_YEAR "
           "AND "
             "D.F_TYPE = S.F_TYPE "
           "AND "
             "D.F_DATE = S.F_DATE "
           "AND "
             "D.F_SESSION_KIND = S.F_KIND "
             ;
        if (decisResultsString && decisResultsString [0])
          oss <<
           "AND "
             "D.F_RESULT IN ("
             << stringToSQLCharSet (decisResultsString).getTheSet ()
             << ") " ;
       }  // -- 1 --
// 20.jul.2004 - begin
#if COURT_TYPE == COURT_APPEAL
	  if (pBMYEMYTCIU)
	   {
	    if (typesString [0] == TYPE_PUNISHMENT_LAW)
	     {
		oss << "AND (SELECT KEEPING FROM SESSION_KEEPING(S.F_TYPE, "
		  "S.F_YEAR, S.F_NO, S.F_KIND, S.F_DATE)) = "
		    << (pBMYEMYTCIU -> keepingMethodTimeBook ? 1 : 0)
		    << " " ;
	     }
	   }
#endif  // APPEAL
// 20.jul.2004 - end
     }  // -- 0 --  if (lookInSessions)
    oss
	<< ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeJudgeCheckupCriteria") ;
   }
  else
    return false ;
 }  // composeJudgeCheckupCriteria

// 08.may.2000 - begin  (moved here)
const char * const notPresidentEmpty =
  "NOT (F_PRESIDENT IS NULL OR F_PRESIDENT = '0000000000')" ;  // 15.apr.2003
const char * const presidentEmpty = notPresidentEmpty + 4 ;
const char * const notJudgeXEmpty =
  "NOT (F_JUDGE_X IS NULL OR F_JUDGE_X = '0000000000')" ;  // 15.apr.2003
const char * const judgeXEmpty = notJudgeXEmpty  + 4 ;
const char * const notReJudgeXEmpty =
  "NOT (F_REJUDGE_X IS NULL OR F_REJUDGE_X = '0000000000')" ;  // 15.apr.2003
const char * const reJudgeXEmpty = notReJudgeXEmpty  + 4 ;
// 08.may.2000 - end

bool composeJdgChkupFixedsCriteria (char * const dest, size_t destSz,
                                    const
// 08.may.2000 - begin
//                                          int
                                          SessionJudgesNumber
// 08.may.2000 - end                                          
                                              howManyJudges,
# if INSTANCE
// 31.mar.2000 - begin
#   if APPEAL
                                    const bool,
#   else  // APPEAL
// 31.mar.2000 - end
                                    const bool firstInstLaws,
#   endif  // APPEAL  // 31.mar.2000 - end
# endif
                                    const char * const typesString,
                                    const long int composition,
                                    const CDate highRange,
                                    const CDate lowRange,
                                    const char * const sessKindsString,
                                    const bool sKMatch,
                                    const char * const sessResultsString,
                          const begMYEndMYTypeCIU* pBMYEMYTCIU   // 20.jul.2004
                                    )
 {
  if (dest && destSz && typesString && typesString [0] &&
      (howManyJudges == JudgeOnly || howManyJudges == OneJudge ||
       howManyJudges == TwoJudges || howManyJudges == ThreeJudges
      )
     )
   {
    stringToSQLCharSet sKS (sessKindsString) ;
    ostrstream oss (dest, destSz - 1) ;
    const char * const what0 = howManyJudges == JudgeOnly
                                                   ? "S.F_JUDGE, " : "" ;
    const char * what = "S.F_PRESIDENT, S.F_JUDGE_X, S.F_REJUDGE_X"
                        ", L.F_KIND"
# if INSTANCE
#   if APPEAL
                                    ", L.F_SUBJECT"
#   endif  // of APPEAL
# endif  // of INSTANCE
      ;

    SUPPRESS_UNUSED(pBMYEMYTCIU);
    oss
	<< "SELECT " << what0 << what
				    << " FROM T_LAWSUIT L, T_SESSION S " ;
    oss <<
	   "WHERE "
	     << "L.F_TYPE IN ("
	     << stringToSQLCharSet (typesString).getTheSet () << ") " ;
    if (composition)
	oss
	  << "AND L.F_COMPOSITION = " << composition << " " ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    oss << "AND L.F_KIND IN ("
	  << stringToSQLCharSet (firstInstLaws ?
					 KIND_1ST_LAWSUITS :
					 KIND_2ND_LAWSUITS).getTheSet () << ") " ;
#   endif  // of APPEAL
# endif  // of INSTANCE
    oss <<
           "AND "
             "S.F_NO = L.F_NO "
           "AND "
             "S.F_YEAR = L.F_YEAR "
           "AND "
             "S.F_TYPE = L.F_TYPE " ;
    oss <<
          "AND "
            "S.F_KIND != \'" << KIND_RESOLUTION_SESSION << "\' " ;
// 08.jun.2000 - begin
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    oss <<
          "AND "
            "S.F_RESULT != \'" << RESULT_DECREE << "\' " ;
#   endif  // APPEAL
# endif  // INSTANCE
// 08.jun.2000 - end
# if JUDGE_CHKUP_EXCLUDE_RES_FIXED  // 25.jan.2001 - begin
    oss <<
          "AND "
            "S.F_RESULT != \'" << RESULT_FIXED << "\' " ;
# endif  // of JUDGE_CHKUP_EXCLUDE_RES_FIXED  // 25.jan.2001 - end
# if ! INSTANCE
    oss <<
          "AND "
            "S.F_KIND != \'" << KIND_LISTENING_SESSION << "\' " ;
# endif
// 22.feb.2000 - begin
    oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_UNFIXED << "\' " ;
// 22.feb.2000 - end
//20050322
    oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_ABORTED << "\' " ;

    oss <<
          "AND (L.F_FINISHED IS NULL OR "
               "S.F_DATE <= L.F_FINISHED) " ;
    oss <<
	     "AND S.F_DATE BETWEEN " << lowRange.Value()
                << " AND " << highRange.Value() << " " ;  // 15.apr.2003 - end
    if (sessKindsString && sessKindsString [0])
      oss <<
         "AND S.F_KIND " << (sKMatch ? "" : "NOT ")
                       << "IN (" << sKS.getTheSet () << ") " ;
    if (sessResultsString && sessResultsString [0])
      oss <<
         "AND S.F_RESULT IN ("
          << stringToSQLCharSet (sessResultsString).getTheSet ()
          << ") " ;
    oss <<
         "AND " ;
    switch (howManyJudges)
     {  // -- 0 --
// 08.may.2000 - begin
//      case 1 :
      case OneJudge :
// 08.may.2000 - end
        oss <<
          "("
           "(" << notPresidentEmpty << " AND "
               <<    judgeXEmpty    << " AND "
               <<    reJudgeXEmpty  << ") "
          "OR "
           "(" <<    presidentEmpty << " AND "
               << notJudgeXEmpty    << " AND "
               <<    reJudgeXEmpty  << ") "
          "OR "
           "(" <<    presidentEmpty << " AND "
               <<    judgeXEmpty    << " AND "
               << notReJudgeXEmpty  << ")"
          ")" ;
        break ;
// 08.may.2000 - begin
//      case 2 :
      case TwoJudges :
// 08.may.2000 - end
        oss <<
          "("
           "(" << notPresidentEmpty << " AND "
               << notJudgeXEmpty    << " AND "
               <<    reJudgeXEmpty  << ") "
          "OR "
           "(" << notPresidentEmpty << " AND "
               <<    judgeXEmpty    << " AND "
               << notReJudgeXEmpty  << ") "
          "OR "
           "(" <<    presidentEmpty << " AND "
               << notJudgeXEmpty    << " AND "
               << notReJudgeXEmpty  << ")"
          ")" ;
        break ;
// 08.may.2000 - begin
//      case 3 :
      case ThreeJudges :
// 08.may.2000 - end
        oss <<
          "("
           "(" << notPresidentEmpty << " AND "
               << notJudgeXEmpty    << " AND "
               << notReJudgeXEmpty  << ")"
          ")" ;
        break ;
// 08.may.2000 - begin
//      case -1 :
      case JudgeOnly :
// 08.may.2000 - end
        oss <<
          "("
           "(" << notPresidentEmpty << " OR "
               << notJudgeXEmpty    << " OR "
               << notReJudgeXEmpty  << ")"
          ")" ;
        break ;
     }  // -- 0 --
// 20.jul.2004 - begin
#if COURT_TYPE == COURT_APPEAL
    if (pBMYEMYTCIU)
     {
	if (typesString [0] == TYPE_PUNISHMENT_LAW)
	 {
	  oss << "AND (SELECT KEEPING FROM SESSION_KEEPING(S.F_TYPE, "
	    "S.F_YEAR, S.F_NO, S.F_KIND, S.F_DATE)) = "
		<< (pBMYEMYTCIU -> keepingMethodTimeBook ? 1 : 0)
		<< " " ;
	 }
     }
#endif  // APPEAL
// 20.jul.2004 - end
    oss
	<< ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeJdgChkupFixedsCriteria") ;
   }
  else
    return false ;
 }  // composeJdgChkupFixedsCriteria

// 08.may.2000 - begin
bool composeJdgChkupAfterFinCriteria (char * const dest,
				      const size_t destSz,
				      const SessionJudgesNumber
					howManyJudges,
				      const char * const typesString,
				      const long int composition,
				      const CDate highRange,
				      const CDate lowRange,
				      const char * const sessKindsString,
				      const bool sKMatch,
				      const bool conformWithSessDate,
# if INSTANCE
#   if APPEAL
                                      const bool,
#   else  // of APPEAL
                                      const bool exscludeResDecree,
#   endif  // of APPEAL
# else  // INSTANCE
				      const bool,
# endif  // INSTANCE
				      const char * const decisKindsString,
                           const char * const sessResStr,  // 01.nov.2000
                           const bool sRSMatch  // 01.nov.2000
				     )
 {
  if (dest && destSz && typesString && typesString [0] &&
      (
       howManyJudges == JudgeOnly || howManyJudges == OneJudge ||
       howManyJudges == TwoJudges || howManyJudges == ThreeJudges
      )
     )
   {
    stringToSQLCharSet sKS (sessKindsString) ;
    ostrstream oss (dest, destSz - 1) ;
    const char * const what0 =
      howManyJudges == JudgeOnly ? "S.F_JUDGE, " : "" ;
    const char * what = "S.F_PRESIDENT, S.F_JUDGE_X, S.F_REJUDGE_X"
			", L.F_KIND"
	;

    oss
      << "SELECT " << what0 <<
		      what << " FROM T_LAWSUIT L, T_SESSION S " ;
    if (decisKindsString)
      oss <<                         ", T_DECISION D " ;
    oss <<
	 "WHERE "
	   << "L.F_TYPE IN ("
	   << stringToSQLCharSet (typesString).getTheSet () << ") " ;
    if (composition)
      oss
	<< "AND L.F_COMPOSITION = " << composition << " " ;
    oss <<
	   "AND "
	     "S.F_NO = L.F_NO "
	   "AND "
	     "S.F_YEAR = L.F_YEAR "
	   "AND "
	     "S.F_TYPE = L.F_TYPE " ;
    oss <<
	  "AND "
	    "S.F_KIND != \'" << KIND_RESOLUTION_SESSION << "\' " ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    if (exscludeResDecree)  // 25.jul.2000
      oss <<
	  "AND "
	    "S.F_RESULT != \'" << RESULT_DECREE << "\' " ;
#   endif  // of APPEAL
# endif  // of INSTANCE
# if ! INSTANCE
    oss <<
	  "AND "
	    "S.F_KIND != \'" << KIND_LISTENING_SESSION << "\' " ;
# endif  // of ! INSTANCE
      oss <<
	 "AND "
	   "S.F_RESULT != \'" << RESULT_UNFIXED << "\' " ;
//20050322
      oss <<
	 "AND "
	   "S.F_RESULT != \'" << RESULT_ABORTED << "\' " ;
# if JUDGE_CHKUP_EXCLUDE_RES_FIXED  // 25.jan.2001 - begin
      oss <<
	 "AND "
	   "S.F_RESULT != \'" << RESULT_FIXED << "\' " ;
# endif  // of JUDGE_CHKUP_EXCLUDE_RES_FIXED  // 25.jan.2001 - end
    if (sessResStr && sessResStr [0])
     {
      oss <<
	 "AND "
	   "S.F_RESULT " << (sRSMatch ? "" : "NOT")
	    << " IN (" << stringToSQLCharSet (sessResStr).getTheSet ()
	    << ") " ;
     }
    oss <<
	  "AND (L.F_FINISHED IS NULL OR "
	       "(L.F_FINISHED IS NOT NULL AND "
		   << (conformWithSessDate ? "S.F_DATE" : "S.F_RETURNED")
	      << "!= L.F_FINISHED"
	       ")"
	      ") " ;
    oss <<
	   "AND S.F_DATE BETWEEN " << lowRange.Value()
		<< " AND " << highRange.Value() << " " ;  // 15.apr.2003 - end
    if (sessKindsString && sessKindsString [0])
      oss <<
	 "AND S.F_KIND " << (sKMatch ? "" : "NOT ")
		       << "IN (" << sKS.getTheSet () << ") " ;
//20070507    oss <<
//20070507         "AND " ;
    switch (howManyJudges)
     {  // -- 0 --
      case OneJudge :
	oss <<
	  "AND "  //20070507
	  "("
	   "(" << notPresidentEmpty << " AND "
	       <<    judgeXEmpty    << " AND "
	       <<    reJudgeXEmpty  << ") "
	  "OR "
	   "(" <<    presidentEmpty << " AND "
	       << notJudgeXEmpty    << " AND "
	       <<    reJudgeXEmpty  << ") "
	  "OR "
	   "(" <<    presidentEmpty << " AND "
	       <<    judgeXEmpty    << " AND "
	       << notReJudgeXEmpty  << ")"
	  ")" ;
	break ;
      case TwoJudges :
	oss <<
	  "AND "  //20070507
	  "("
	   "(" << notPresidentEmpty << " AND "
	       << notJudgeXEmpty    << " AND "
	       <<    reJudgeXEmpty  << ") "
	  "OR "
	   "(" << notPresidentEmpty << " AND "
	       <<    judgeXEmpty    << " AND "
	       << notReJudgeXEmpty  << ") "
	  "OR "
	   "(" <<    presidentEmpty << " AND "
	       << notJudgeXEmpty    << " AND "
	       << notReJudgeXEmpty  << ")"
	  ")" ;
	break ;
      case ThreeJudges :
	oss <<
          "AND "  //20070507
          "("
           "(" << notPresidentEmpty << " AND "
               << notJudgeXEmpty    << " AND "
	       << notReJudgeXEmpty  << ")"
          ")" ;
        break ;
      case JudgeOnly :   //"AND "
      // 20070507  oss <<
      // 20070507    "("
      // 20070507     "(" << notPresidentEmpty << " OR "
      // 20070507         << notJudgeXEmpty    << " OR "
      // 20070507         << notReJudgeXEmpty  << ")"
      // 20070507    ")" ;
        break ;
     }  // -- 0 --
    if (decisKindsString)
     {
      oss <<
        "AND "
          "S.F_NO = D.F_NO "
        "AND "
	  "S.F_YEAR = D.F_YEAR "
	"AND "
	  "S.F_TYPE = D.F_TYPE "
	"AND "
	  "S.F_DATE = D.F_DATE "
	"AND "
	  "S.F_KIND = D.F_SESIION_KIND " ;
      if (decisKindsString [0])
	oss <<
	"AND D.F_KIND IN ("
	    << stringToSQLCharSet (decisKindsString).getTheSet ()
	    <<            ") " ;
     }
    // 2008:287; 2008:288; 2008:297 LRQ: newest
    oss << " AND S.F_DATE > L.F_FINISHED";
    oss
      << ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest,
				 "composeJdgChkupAfterFinCriteria") ;
   }
  else
    return false ;
 }  // composeJdgChkupAfterFinCriteria

// 08.may.2000 - end

//
