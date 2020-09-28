#include "cac.h"  // 18.may.2001
//# include "all.h"

// 20051010 -->
#if COURT_TYPE == COURT_REGIONAL
bool composeZZDNCriteria (char * dest,
                          size_t destSz,
                          const char * what,
                          const char * resultsString,
                          const ZZDN_Dlg_data * cCBDEDBN_ZZDNPtr
                         )
 {
  if (dest && destSz && what &&
      cCBDEDBN_ZZDNPtr
      )
   {
    stringToSQLCharSet rTBS (resultsString) ;
    ostrstream oss (dest, destSz - 1) ;
    oss
      << "SELECT " << what << " FROM T_REQUEST R WHERE ";//, T_LAWSUIT L " ;
    if (cCBDEDBN_ZZDNPtr -> selectCrit1)
    {
       int FromNo;
       int ToNo;
       if (cCBDEDBN_ZZDNPtr -> FromNo <= 0) FromNo = 1 ;
	 else FromNo = cCBDEDBN_ZZDNPtr -> FromNo;
	 if (cCBDEDBN_ZZDNPtr -> ToNo <= 0) ToNo = 999999;
       else ToNo = cCBDEDBN_ZZDNPtr -> ToNo;

       oss
          << "R.F_NO BETWEEN " <<
               "'" << FromNo << "' AND '"
                   << ToNo << "' "
                   << " AND R.F_YEAR=" << cCBDEDBN_ZZDNPtr -> Year ;
    }
    else
    {  oss
          << "R.F_DATE BETWEEN "
		 << cCBDEDBN_ZZDNPtr -> begD.Value() << " AND "
		 << cCBDEDBN_ZZDNPtr -> endD.Value() << " " ;
    }
    oss
        << " AND R.F_TYPE = '" << TYPE_REQUEST << "'"
        << " AND R.F_KIND = '" << KIND_REQUEST_ZZDN << "'"
        ;
    oss <<
	   " ORDER BY R.F_DATE, R.F_NO " << ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeZZDNCriteria") ;
   }
  else
    return false ;
 }  // composeZZDNCriteria
#endif  // REGIONAL
// 20051010 <--

bool composeTBTSCriteria (char * dest, size_t destSz, const char * what,
				  const char * resultsString,
				  const collCompBegDEndDBegN * cCBDEDBNPtr,
				  const char * ageString,
				  const char * pGrantChar,
				  const char * sessKindStr,
                          bool sksEq,
                          const char * lawKindStr,
                          const char * sourceKindStr,
                          const bool followSessDate,
                          const char * const decisResultsStr,
                          const bool resultsStringResultsMatch,
                          const bool excudeAbolishedResults
                         )
 {
  if (dest && destSz && what &&
      // resultsString &&
	cCBDEDBNPtr
	&& (strlen (cCBDEDBNPtr -> colleges) == 1)
	)
   {
    stringToSQLCharSet rTBS (resultsString) ;
    ostrstream oss (dest, destSz - 1) ;
    oss
	<< "SELECT " << what << " FROM T_SESSION S, T_LAWSUIT L "
	<< (decisResultsStr ? ", T_DECISION D " : "") <<
	   "WHERE "
	<< (followSessDate ? "S.F_DATE" : "L.F_FINISHED") << " BETWEEN "
		 << cCBDEDBNPtr -> begD.Value() << " AND "
		 << cCBDEDBNPtr -> endD.Value() << " "  // 15.apr.2003 - end
	   "AND "
	     "S.F_NO = L.F_NO "
	   "AND "
	     "S.F_YEAR = L.F_YEAR "
	   "AND "
	     "S.F_TYPE = L.F_TYPE " ;
    if (excudeAbolishedResults)
	oss <<
 "AND (L.F_DEC_MADE IS NULL OR L.F_DEC_MADE <= S.F_DATE) " ; //02.feb.2001
# if INSTANCE
#   if APPEAL
    if (cCBDEDBNPtr -> colleges [0] == TYPE_PUNISHMENT_LAW &&
	  cCBDEDBNPtr -> lArrPtr () -> Count () // 29.may.2000
       )
      oss <<
         "AND L.F_SUBJECT IN (" << (* (cCBDEDBNPtr -> lArrPtr ()))
                                << ") " ;
#   endif
# endif
    if (decisResultsStr)
     {  // 16.may.2000
      oss <<
         "AND "
           "D.F_NO = L.F_NO "
         "AND "
           "D.F_YEAR = L.F_YEAR "
         "AND "
           "D.F_TYPE = L.F_TYPE " ;
      if (excudeAbolishedResults)
        oss <<
 "AND (L.F_DEC_MADE IS NULL OR L.F_DEC_MADE <= D.F_DATE) " ; //02.feb.2001
     }
    oss << ""
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
         "AND L.F_KIND IN ("
           << stringToSQLCharSet (cCBDEDBNPtr -> firstInstLaws ?
                                  KIND_1ST_LAWSUITS :
                                  KIND_2ND_LAWSUITS).getTheSet ()
           << ") "
#   endif  // of APPEAL
# endif
                   ;
      oss <<
         "AND ("
           "("
            "S.F_KIND NOT IN (" <<
               stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                         << ") "  // 12.jan.2001
           ")" ;
      if (cCBDEDBNPtr -> wantClosedSessions)
       {
        oss <<
           " OR ("
                 "S.F_KIND IN (" <<
                   stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                          << ") "  // 12.jan.2001
                 "AND "
                 "S.F_RESULT IN ("
                 << stringToSQLCharSet (RESULT_FINALIZEDS).getTheSet ()
                 << ")) " ;
       }
      oss << ") "
         "AND "
           "L."
# if FOLLOW_LAWSUIT_TYPES
               "F_TYPE"
# else
               "F_COLLEGE"
# endif
                          " IN ("
             << stringToSQLCharSet (cCBDEDBNPtr -> colleges).getTheSet ()
                       << ") " ;
      if (cCBDEDBNPtr -> composit)
        oss <<
         "AND "
           "S.F_COMPOSITION = " << cCBDEDBNPtr -> composit << " " ;
      oss <<
         "AND "
           "S.F_KIND != \'" << KIND_RESOLUTION_SESSION << "\' " ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
      oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_DECREE << "\' " ;
#   endif  // APPEAL
# endif  // INSTANCE
# if ! INSTANCE
      oss <<
         "AND "
           "S.F_KIND != \'" << KIND_LISTENING_SESSION << "\' " ;
# endif
      oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_UNFIXED << "\' " ;
//20050322
      oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_ABORTED << "\' " ;
      if (resultsString || ageString || pGrantChar || decisResultsStr)
       {
        oss <<
         "AND ("
                ;
        if (ageString || pGrantChar)  // The law must be finished
          oss << "L.F_FINISHED IS NOT NULL AND " ;
        else
          oss << "L.F_FINISHED IS NULL OR " ;
	  oss <<   "(S.F_DATE <= L.F_FINISHED"
		<< (decisResultsStr ?
			 " AND D.F_DATE <= L.F_FINISHED" : "")
            <<   ")"
             ") " ;
       }
      if (resultsString && resultsString [0])
        oss <<
         "AND "
           "S.F_RESULT " << (resultsStringResultsMatch ? "" : "NOT ") <<
                        "IN ("
            << rTBS.getTheSet () << ") " ;
      if (decisResultsStr && decisResultsStr [0])
        oss <<
         "AND D.F_RESULT IN ("
            << stringToSQLCharSet (decisResultsStr).getTheSet ()
            <<             ") " ;
      if (ageString && ageString [0])
        oss <<
         "AND "
           "L.F_AGE IN ("
                   << stringToSQLCharSet (ageString).getTheSet ()
                   << ") " ;
      if (pGrantChar)
        oss <<
         "AND "
           "L.F_GRANTED = \'" << (* pGrantChar) << "\' " ;
      if (sessKindStr && sessKindStr [0])
        oss <<
         "AND "
           "S.F_KIND " << (sksEq ? "" : "NOT ") << "IN ("
                       << stringToSQLCharSet (sessKindStr).getTheSet ()
                       << ") " ;
      if (lawKindStr && lawKindStr [0])
        oss <<
         "AND "
           "L.F_KIND " << "IN ("
                       << stringToSQLCharSet (lawKindStr).getTheSet ()
                       << ") " ;
# if INSTANCE
#   if APPEAL  //    15.may.2000
#   else  // of APPEAL  15.may.2000
      if (strchr (cCBDEDBNPtr -> colleges, TYPE_PUNISHMENT_LAW))
        if (cCBDEDBNPtr -> all2ndPuniLaws == false)  // 02.jan.2002
          oss <<
           "AND " << (cCBDEDBNPtr -> puniAdmConcernLaws ? "" : "NOT ") <<
             "(L.F_KIND = \'" << KIND_2ND_ADMIN_PUNISHMENT_LAW << "\' ) " ;
#   endif  // of APPEAL  15.may.2000
# else  // of INSTANCE
# endif  // of INSTANCE
      if (sourceKindStr && sourceKindStr [0])
        oss <<
         "AND "
           "L.F_SOURCE_KIND " << "IN ("
                       << stringToSQLCharSet (sourceKindStr).getTheSet ()
                       << ") " ;
// 20.jul.2004 - begin
#if COURT_TYPE == COURT_APPEAL
	if(cCBDEDBNPtr -> colleges [0] == TYPE_PUNISHMENT_LAW)
	 {
	  oss << "AND (SELECT KEEPING FROM SESSION_KEEPING(S.F_TYPE, S.F_YEAR, "
	    "S.F_NO, S.F_KIND, S.F_DATE)) = "
		<< (cCBDEDBNPtr -> keepingMethodTimeBook ? 1 : 0)
		<< " " ;
	  //info(cCBDEDBNPtr -> keepingMethodTimeBook ? "Keeping" : "Non-keeping");
	 }
#endif  // APPEAL	 
// 20.jul.2004 - end

	if (cCBDEDBNPtr -> orderBy1) // 20051206 ->
	{
	  oss <<
	     "ORDER BY S.F_DATE, " ;
	  if (strchr (cCBDEDBNPtr -> colleges, TYPE_PUNISHMENT_LAW))
	    oss <<
			"S.F_END, " ;
// 20060411 in comm  # if INSTANCE  // 01.feb.2000
	  oss <<      "S.F_TIME, " ;  // 09.nov.2000
// 20060411 in comm  # endif  // of INSTANCE  // 01.feb.2000
	  oss <<
			"S.F_YEAR, S.F_NO " << ";" ;
	}
	// 2015:198 LPR: FIX: don't sort summaries
	else
	{
	  if (cCBDEDBNPtr->orderBy2)
	  {
	    oss <<
		 "ORDER BY S.F_DATE, S.F_YEAR, S.F_NO  " ;

	    if (strchr (cCBDEDBNPtr -> colleges, TYPE_PUNISHMENT_LAW))
		  oss <<
			  ", S.F_END " ;
// 20060411 in comm  # if INSTANCE  // 01.feb.2000
	    oss <<      ", S.F_TIME " ;  // 09.nov.2000
// 20060411 in comm  # endif  // of INSTANCE  // 01.feb.2000
	  }
	  oss << ";" ;
      }

    oss << ends ;
    dest [destSz - 1] = '\0' ;

    // 20070208 ofstream("000000q.txt", ios::out | ios::ate) << dest << endl;

    return criteria_finished_ok (dest, "composeTBTSCriteria") ;
   }
  else
    return false ;
 }  // composeTBTSCriteria

bool composeClosedSessBSCriteria (char * criteria,
					    const size_t criteriaSz,
					    const char * what,
					    const collBegDEndDBegN & cBegDEndDBegN,
					    const char * resultsString,
                                  const char * decisKindsString
# if INSTANCE
                                  , int instns
# endif
                                                     ,
					    const bool followSessDate,
                                  const char * const lawSourceKindsString
                                  , const bool resStrMatch // 24.apr.2000
                                 )
 {
  if (! (criteria && criteriaSz && what))
    return false ;
   {
    ostrstream oss (criteria, criteriaSz) ;
    const char * const desJudgTable =
                                      "T_DECISION"
                                     ;
    const bool othTableReq =
# if INSTANCE
      enterDateNeed (cBegDEndDBegN) ||
# endif
      (decisKindsString && decisKindsString [0]) ;
    const char * const dateField =
# if INSTANCE
      cBegDEndDBegN.wantSortByDateReturned ? // for Areal and Appeal courts
                                             // wantSortByDateReturned is always TRUE
// 20060412 after 1.2552 50.1. to change sort order
// 20060412 no comm (cBegDEndDBegN.college != TYPE_FIRM_LAW ? "S.F_RETURNED" :
// 20060412 no comm                                           "D.F_ENTER_DATE")
					 "S.F_RETURNED"   // 20060412
			     : "S.F_DATE" ;
# else
	cBegDEndDBegN.wantSortByDateReturned ? "S.F_RETURNED" : "S.F_DATE" ;
# endif
    const char * const moreWhat =
# if INSTANCE
      enterDateNeed (cBegDEndDBegN) ? ", D.F_ENTER_DATE" : "" ;
# else
      "" ;
# endif

    oss
      << "SELECT " << what
				   << moreWhat
    // 2015:187 LPR/JRQ: DISTRINCT -> order by must contain selected fields
	<< ", K.F_NAME"
	<< " FROM T_SESSION S, T_LAWSUIT L " ;
    oss << ", T_KIND K " ;  // 13.feb.2001
    if (othTableReq)
      oss << ", " << desJudgTable << " D " ;
    oss
      << "WHERE "
                  ;
# if INSTANCE
    if (cBegDEndDBegN.college != TYPE_FIRM_LAW)
      oss <<
         "S.F_KIND IN ("
                <<  stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                << ") " ;  // 12.jan.2001
    else
     {
      oss <<
         "S.F_KIND NOT IN ("
          << stringToSQLCharSet (KIND_PSEUDO_SESSIONS).getTheSet ()
          << ") " ;
// 20060412  in comment ->
// 20060412 #   if APPEAL  // 22.jan.2001 - begin
// 20060412 #   else  // of APPEAL
// 20060412       oss <<    "AND " <<
// 20060412         "S.F_RESULT NOT IN ("
// 20060412          << stringToSQLCharSet(RESULT_FIRM_CL_S_BK_EXCLUDEDS).getTheSet()
// 20060412          << ") " ;
// 20060412 #   endif  // of APPEAL  // 22.jan.2001 - end
     }
# else
    oss <<
         "S.F_KIND IN (" <<
               stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                  << ") " ;  // 12.jan.2001
# endif  // INSTANCE
// 24.jul.2000 - begin
    if (cBegDEndDBegN.composition)
      oss <<
         "AND S.F_COMPOSITION = " << cBegDEndDBegN.composition << " "  ;
// 24.jul.2000 - end
    oss <<
         "AND S.F_TYPE = \'" << cBegDEndDBegN.college << "\' " ;
# if INSTANCE  // 22.jan.2001 - begin
#   if APPEAL
    oss <<
	   "AND " << (followSessDate ? dateField : "L.F_FINISHED")
		    << " BETWEEN " << cBegDEndDBegN.begD.Value() << " AND "
                << cBegDEndDBegN.endD.Value() << " " ;  // 15.apr.2003 - end
#   else  // of APPEAL
    if (followSessDate)
     {
      if (cBegDEndDBegN.college == TYPE_FIRM_LAW)
       {
        oss <<
         "AND ((S.F_RESULT IN (" <<
         stringToSQLCharSet (RESULT_FIRM_CLSBK_MUST_INCLUDE).getTheSet ()
// 20060412 after 1.2552 50.1. add result types
                                 << ", "                      // 20060412
                                 <<                           // 20060412
         stringToSQLCharSet (RESULT_FIRM_CL_S_BK_EXCLUDEDS).getTheSet () // 20060412
                                 << ") AND "
               "((S.F_KIND IN (" <<  // 02.feb.2001 - begin
         stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
					   << ") AND "  // 02.feb.2001 - end
// 15.apr.2003 - begin
		     "S.F_RETURNED BETWEEN " << cBegDEndDBegN.begD.Value() << " AND "
			 << cBegDEndDBegN.endD.Value() <<   // 15.apr.2003 - end
		    ") OR "  // 02.feb.2001 - begin
		    "(S.F_KIND NOT IN (" <<
	   stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
					   << ") AND "
// 15.apr.2003 - begin
		     "S.F_DATE BETWEEN " << cBegDEndDBegN.begD.Value() << " AND "
			 << cBegDEndDBegN.endD.Value() <<   // 15.apr.2003 - end
		    ")"
               ")"  // 02.feb.2001 - end
              ") OR "
              "(S.F_RESULT NOT IN (" <<
         stringToSQLCharSet (RESULT_FIRM_CLSBK_MUST_INCLUDE).getTheSet ()
// 20060412 after 1.2552 50.1.
                                 << ", "                      // 20060412
                                 <<                           // 20060412
         stringToSQLCharSet (RESULT_FIRM_CL_S_BK_EXCLUDEDS).getTheSet () // 20060412

                                     << ") AND "
// 15.apr.2003 - begin
		   "D.F_ENTER_DATE BETWEEN " << cBegDEndDBegN.begD.Value() << " AND "
                 << cBegDEndDBegN.endD.Value() <<   // 15.apr.2003 - end
		  ")"
             ") " ;
       }
	else
	  oss <<
	   "AND " << dateField
		    << " BETWEEN " << cBegDEndDBegN.begD.Value() << " AND "
			   << cBegDEndDBegN.endD.Value() << " " ;  // 15.apr.2003 - end
     }
    else
	oss <<
// 15.apr.2003 - begin
	   "AND L.F_FINISHED BETWEEN " << cBegDEndDBegN.begD.Value() << " AND "
           << cBegDEndDBegN.endD.Value() << " " ;  // 15.apr.2003 - end
#   endif  // of APPEAL
# else  // of INSTANCE  // 22.jan.2001 - end
    oss <<
	   "AND " << (followSessDate ? dateField : "L.F_FINISHED")
		    << " BETWEEN " << cBegDEndDBegN.begD.Value() << " AND "
		    << cBegDEndDBegN.endD.Value() << " " ;  // 15.apr.2003 - end
# endif  // of INSTANCE  // 22.jan.2001
    oss <<  // 22.feb.2000 - begin
       "AND "
         "S.F_RESULT != \'" << RESULT_UNFIXED << "\' "  //22.feb.2000-end
       "AND "
         "S.F_RESULT != \'" << RESULT_ABORTED << "\' " ; //20050322

    if (! cBegDEndDBegN.wantAllResults)
      oss <<
         "AND S.F_RESULT IN ("
          << stringToSQLCharSet (RESULT_FINALIZEDS).getTheSet ()
          << ") " ;
    oss
      << "AND L.F_NO = S.F_NO "
         "AND L.F_YEAR = S.F_YEAR "
         "AND L.F_TYPE = S.F_TYPE "
         "AND L."
# if FOLLOW_LAWSUIT_TYPES
                 "F_TYPE"
# else
                 "F_COLLEGE"
# endif
                            " = \'" << cBegDEndDBegN.college << "\' "
          ;
	if (resultsString)
        oss <<
         "AND (L.F_FINISHED IS NULL OR "
            "S.F_DATE <= L.F_FINISHED) " ;
    if (resultsString && resultsString [0])
      oss <<
         "AND S.F_RESULT "
          << (resStrMatch ? "" : "NOT ") <<  // 24.apr.2000
              "IN ("
          << stringToSQLCharSet (resultsString).getTheSet () << ") " ;
    if (othTableReq)
      oss <<
         "AND D.F_NO = S.F_NO "
         "AND D.F_YEAR = S.F_YEAR "
         "AND D.F_TYPE = S.F_TYPE "
         "AND D.F_DATE = S.F_DATE "
         "AND D.F_SESSION_KIND = S.F_KIND "
# if INSTANCE
                                    ;
    if (decisKindsString && decisKindsString [0])
      oss <<
# endif
         "AND D.F_KIND IN ("
           << stringToSQLCharSet (decisKindsString).getTheSet ()
           << ") " ;
# if INSTANCE
    if (instns)
     {
#   if APPEAL  // 31.mar.2000
#   else  // of APPEAL  // 31.mar.2000
      const char * kS = instns == 1 ?
        KIND_1ST_LAWSUITS : KIND_2ND_LAWSUITS ;

      oss <<
         "AND L.F_KIND IN ("
          << stringToSQLCharSet (kS).getTheSet () << ") " ;
#   endif  // APPEAL  // 31.mar.2000
     }
# endif
    if (lawSourceKindsString && lawSourceKindsString [0])
      oss <<
         "AND L.F_SOURCE_KIND IN ("
          << stringToSQLCharSet (lawSourceKindsString).getTheSet ()
          << ") " ;
    oss <<
	   "AND S.F_KIND = K.F_KIND " ;  // 13.feb.2001
    oss
        << "ORDER BY " << dateField << ", S.F_YEAR, S.F_NO"
	   ", K.F_NAME"  // 13.feb.2001
         " "";" ;
    oss << ends ;
    criteria [criteriaSz - 1] = '\0' ;

//    ofstream("cscrt.txt") << criteria << endl;  // DEBUG!

    return criteria_finished_ok (criteria,
                                 "composeClosedSessBSCriteria") ;
   }
 }  // composeClosedSessBSCriteria


 
bool composeFinishWithDecJudgDef_Crit (char * const dest,
                                       const size_t destSz,
                                const collCompBegDEndDBegN * cCBEDBNPtr,
//                                       const char * const decGJkindsStr,
                                       const char * const,
                                       const char * const lawKindsStr,
                                       const char * const sourceKindStr)
 {
  if (dest && destSz && cCBEDBNPtr)
   {
    const char * const what =
//      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE" ;  // DISTINCT is important
      "L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_FINISHED"
      ", L.F_SOURCE_KIND"  // because of new function is_judgementable
      ", L.F_QUICK" // 20060324
       ;
    ostrstream oss (dest, destSz) ;

    oss
      << "SELECT " << what
      << " FROM T_LAWSUIT L "
         "WHERE " ;
    if (cCBEDBNPtr -> colleges && cCBEDBNPtr -> colleges [0])
      oss
        << "L.F_TYPE IN ("
             << stringToSQLCharSet (cCBEDBNPtr -> colleges).getTheSet ()
             << ") " ;
    if (lawKindsStr && lawKindsStr [0])
      oss
        << "AND "
             "L.F_KIND IN ("
             << stringToSQLCharSet (lawKindsStr).getTheSet () << ") " ;
    if (cCBEDBNPtr -> composit)
      oss <<
           "AND "
             "L.F_COMPOSITION = " << cCBEDBNPtr -> composit << " " ;
# if INSTANCE
// 31.mar.2000 - begin
#   if APPEAL
#   else  // of APPEAL
// 31.mar.2000 - end
      oss <<
         "AND L.F_KIND IN ("
           << stringToSQLCharSet (cCBEDBNPtr -> firstInstLaws ?
                                  KIND_1ST_LAWSUITS :
                                  KIND_2ND_LAWSUITS).getTheSet ()
           << ") " ;
      if (strchr (cCBEDBNPtr -> colleges, TYPE_PUNISHMENT_LAW))
        if (cCBEDBNPtr -> all2ndPuniLaws == false)  // 02.jan.2002
          oss <<
           "AND " << (cCBEDBNPtr -> puniAdmConcernLaws ? "" : "NOT ") <<
             "(L.F_KIND = \'" << KIND_2ND_ADMIN_PUNISHMENT_LAW << "\' ) " ;
             // 12.feb.2001  instead of f...d private
             // complaints - concern (private) complaints
#   endif  // of APPEAL  // 31.mar.2000
# endif
    if (sourceKindStr && sourceKindStr [0])
      oss <<
         "AND L.F_SOURCE_KIND IN ("
             << stringToSQLCharSet (sourceKindStr).getTheSet () << ") " ;
    oss <<
         "AND L.F_FINISHED IS NOT NULL "
// ----------------------- like report ... ----------------------------
	   "AND L.F_FINISHED BETWEEN " << cCBEDBNPtr -> begD.Value() << " AND "
           << cCBEDBNPtr -> endD.Value() << " " ;  // 15.apr.2003 - end
// --------------------------------------------------------------------
    oss
      << ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest,
                                 "composeFinishWithDecJudgDef_Crit") ;
   }
  else
    return false ;
 }  // composeFinishWithDecJudgDef_Crit

bool composeSessionsByResultsCriteria (char * const crit,
                                       const size_t critSz,
                                   const collCompBegDEndDBegN & cCBDEDBN,
                                       const char * const resultsStr,
                                       const char * const what)
 {
  if (! (crit && critSz && cCBDEDBN.colleges [0] &&
         resultsStr && resultsStr [0] && what && what [0]))
    return false ;

  const char * const dateField = cCBDEDBN.wantSortByDateReturned ?
					     "F_RETURNED" : "F_DATE" ;

  ostrstream (crit, critSz)
    << "SELECT " << what << " FROM T_SESSION WHERE "
	   "F_TYPE IN (" << SQLCharSet (cCBDEDBN.colleges) << ") AND "
	   "F_RESULT IN (" << SQLCharSet (resultsStr) << ") AND "
    <<   dateField << " BETWEEN " << cCBDEDBN.begD.Value() << " AND "
					     << cCBDEDBN.endD.Value() << " "  // 15.apr.2003-end
         "ORDER BY F_DATE, F_TIME ;" << ends ;
  crit [critSz - 1] = '\0' ;
  return criteria_finished_ok (crit,
                               "composeSessionsByResultsCriteria") ;
 }  // composeSessionsByResultsCriteria

bool composeAbolishedProgressCriteria (char * const crit,
                                       const size_t critSz,
                                 const collCompBegDEndDBegN & cCBDEDBN,
                                 const char * const what  // 05.jun.2001
                                      )
 {
  if (! (crit && critSz && cCBDEDBN.colleges [0] && what && what [0]))
    return false ;

  return composeSessionsByResultsCriteria (crit, critSz, cCBDEDBN, RESULT_UNDOS,
                                           what) ;
 }  // composeAbolishedProgressCriteria


bool compose_desk_book_criteria (char * const crit,
                                 const size_t critSz,
                                 const char college,
                                 const begNEndNYear & bneny)
 {
  if (! (crit && critSz && college))
    return false ;
  crit [0] = '\0' ;
   {
    ostrstream oss (crit, critSz) ;

    if (bneny.selectCrit1)  // 20080623
    {
      oss
	  		<< "SELECT "
		  	   "F_TYPE, F_YEAR, F_NO "  // "* "  // 20.jun.2002
			  	 "FROM T_LAWSUIT "
           "WHERE "
             "F_NO " ;
      if (bneny.endN > 0)
        oss
          << "BETWEEN " << bneny.begN << " AND " << bneny.endN ;
      else
        oss << ">= " << bneny.begN ;
      oss
        << " "
           "AND "
             "F_YEAR = " << bneny.year << " "
             "AND "
# if FOLLOW_LAWSUIT_TYPES
             "F_TYPE"
# else
             "F_COLLEGE"
# endif
                      " = \'" << college << "\' " ;

    } else { // 20080623 ->

      oss << "SELECT F_TYPE, F_YEAR, F_NO FROM T_LAWSUIT WHERE "
             "F_DATE " ;
      if (bneny.endDate.day > 0)
        oss
	    << "BETWEEN " << bneny.begDate.Value() << " AND "
                        << bneny.endDate.Value() ;
      else
        oss << ">= " << bneny.begDate ;
      oss
        << " AND "
# if FOLLOW_LAWSUIT_TYPES
             "F_TYPE"
# else
             "F_COLLEGE"
# endif
                      " = \'" << college << "\' " ;
    }        // 20080623 <-

// 16.oct.2000 - begin
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    if (bneny.want1stInstLaws != bneny.want2ndInstLaws)
     {
      const char * const lawKinds =
        bneny.want1stInstLaws ? KIND_1ST_LAWSUITS : KIND_2ND_LAWSUITS ;

      oss
      <<
         "AND F_KIND IN ("
           << stringToSQLCharSet (lawKinds).getTheSet () <<
                       ") " ;
     }
#   endif  // APPEAL
# endif  // INSTANCE
// 16.oct.2000 - end
    oss
      << " ORDER BY F_NO ;" << ends ;
    }
  crit [critSz - 1] = '\0' ;
  return criteria_finished_ok (crit, "compose_desk_book_criteria") ;
 }  // compose_desk_book_criteria

bool appealed_laws_basic_criteria (ostrstream & oss,
                                   const char * const types,
                                   const CDate begDate,
                                   const CDate endDate,
                                   const char * const what,
                  const bool includingAppealedInRegEvents  // 10.apr.2001
                                  )
 {  // 02.dec.2000
  if (! (types && types [0] && 
         begDate.Empty () == false && endDate.Empty () == false) &&
         what && what [0])
    return false ;

  const stringToSQLCharSet typesSet (types) ;
  const char * const tS = typesSet.getTheSet () ;

  oss
    << "SELECT " << what << " "
       "FROM T_INREG I, T_INREG_EVENT E, T_OUTREG O"
       " WHERE "
	   "E.F_DATE IS NOT NULL AND "  // F_FROM_DATE replaced with F_DATE  20051118
	   "E.F_DATE BETWEEN " << begDate.Value() << " " // F_FROM_DATE replaced with F_DATE  20051118
                           "AND " << endDate.Value() << " AND "  // 15.apr.2003 - end
	   "E.F_RESULT IS NOT NULL AND "
         "E.F_RESULT IN (" <<
		stringToSQLCharSet (C2S[RESULT_IOREG_SEND]).getTheSet ()
                           << ") AND "
         "I.F_NO = E.F_NO AND "
         "I.F_YEAR = E.F_YEAR AND "
         "I.F_TYPE = E.F_TYPE AND "
         "O.F_NO = E.F_OUT_NO AND "
         "O.F_YEAR = E.F_OUT_YEAR AND "
         "O.F_IN_NO = E.F_NO AND "     // <==> "O.F_IN_NO = I.F_NO"
         "O.F_IN_YEAR = E.F_YEAR AND " // <==> "O.F_IN_YEAR = I.F_YEAR"
         "I.F_LAWSUIT_TYPE IS NOT NULL AND "
         "I.F_LAWSUIT_TYPE IN (" << tS << ") " ;
  if (includingAppealedInRegEvents == false)  // 10.apr.2001 - begin
    oss <<
         "AND (NOT (I.F_DECISION_TYPE IN ("
          << stringToSQLCharSet (TYPE_INREGS).getTheSet ()
                                     << "))) " ;  // 10.apr.2001 - end
  return oss.good () ;
 }  // appealed_laws_basic_criteria

// 20070208
bool appealed_laws_basic_with_composition
                                 (ostrstream & oss,
                                   const char * const types,
                                   const CDate begDate,
                                   const CDate endDate,
                                   const long int composition,
                                   const char * const what,
                  const bool includingAppealedInRegEvents
                                  )
 {
  if (! (types && types [0] && 
         begDate.Empty () == false && endDate.Empty () == false) &&
         what && what [0])
    return false ;

  const stringToSQLCharSet typesSet (types) ;
  const char * const tS = typesSet.getTheSet () ;

  oss
    << "SELECT " << what << " "
       "FROM T_INREG I, T_INREG_EVENT E, T_OUTREG O";
  if ( composition > 0 )
  {
     oss
        << ", T_LAWSUIT L ";
  }
  oss
    << " WHERE "
	   "E.F_DATE IS NOT NULL AND "  // F_FROM_DATE replaced with F_DATE  20051118
	   "E.F_DATE BETWEEN " << begDate.Value() << " " // F_FROM_DATE replaced with F_DATE  20051118
                           "AND " << endDate.Value() << " AND "  // 15.apr.2003 - end
	   "E.F_RESULT IS NOT NULL AND "
         "E.F_RESULT IN (" <<
            stringToSQLCharSet (C2S[RESULT_IOREG_SEND]).getTheSet ()
                           << ") AND "
         "I.F_NO = E.F_NO AND "
         "I.F_YEAR = E.F_YEAR AND "
         "I.F_TYPE = E.F_TYPE AND "
         "O.F_NO = E.F_OUT_NO AND "
         "O.F_YEAR = E.F_OUT_YEAR AND "
         "O.F_IN_NO = E.F_NO AND "     // <==> "O.F_IN_NO = I.F_NO"
         "O.F_IN_YEAR = E.F_YEAR AND " // <==> "O.F_IN_YEAR = I.F_YEAR"
         "I.F_LAWSUIT_TYPE IS NOT NULL AND "
         "I.F_LAWSUIT_TYPE IN (" << tS << ") " ;
  if ( composition > 0 )
  {
     oss <<
         "AND "
         "L.F_NO = I.F_LAWSUIT_NO AND "
         "L.F_YEAR = I.F_LAWSUIT_YEAR AND "
         "L.F_TYPE = I.F_LAWSUIT_TYPE AND "
         "L.F_COMPOSITION = " << composition << " ";

  }
  if (includingAppealedInRegEvents == false)  // 10.apr.2001 - begin
    oss <<
         "AND (NOT (I.F_DECISION_TYPE IN ("
          << stringToSQLCharSet (TYPE_INREGS).getTheSet ()
                                     << "))) " ;  // 10.apr.2001 - end
  return oss.good () ;
 }  // appealed_laws_basic_with_composition



bool request_by_lawKinds_part_crit (ostream & os,
						const char * const lawKinds,
						const char * const pref)
 {
  const char * const prefix = pref ? pref : "" ;

  os << prefix << "F_TYPE = \'" << TYPE_REQUEST << "\' " ;
  if (lawKinds && lawKinds [0])
  {
    char kinds[SIZE_OF_KINDS];
    char crit[1024];

    ostrstream(crit, sizeof(crit))
      << "SELECT F_KIND FROM T_KIND WHERE F_LAW_KINDS IN ("
      << SQLCharSet(lawKinds)
      << ")" << " ;" << ends;
    if(criteria_finished_ok(crit, "request_by_lawKinds_part_crit"))
    {
      kinds[0] = '\0';

      {
        TQuery q(crit);
        TKind k;

        while(q.Read())
        {
          k << q;
          strConcatCh(kinds, k.kind, sizeof(kinds));
        }
      }
      if(kinds[0])
      {
        os << "AND " << prefix << "F_KIND IN (" << SQLCharSet(kinds) << ")";
        return os.good () ;
      }
      else
        return error("%s:\n""No corresponding request kinds are found.",
          "request_by_lawKinds_part_crit");
    }
    else
      return false;
  }
  return os.good () ;
 }  // request_by_lawKinds_part_crit

//

