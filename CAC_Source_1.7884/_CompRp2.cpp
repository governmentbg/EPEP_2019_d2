#include "cac.h"  // 18.may.2001
//# include "all.h"

# if INSTANCE
bool composeReportLawsCriteria2 (char * dest, size_t destSz,
#   if APPEAL
                                 const bool,
#   else  // of APPEAL
                                 const bool forMinistry,
#   endif  // of APPEAL
                                 const char * what,
                                 const char * typesString,
                                 const long int composition,
                                 const CDate * pHighRange,
                                 const CDate * pLowRange,
                                 const char * requestKindsString,
                                 const char * globJudgemResultString,
                                 const char * decisionResultString,
                                 const char * sessResultString,
                                 const bool finishedTest,
                                 const bool excludeCreatedsLater,
                                 const bool rKSMatch,  // 25.feb.2000
//    , const long int subjectLo,  // 31.oct.2000, 08.nov.2000 back again
//    const long int subjectHi,  // 31.oct.2000, 08.nov.2000 back again
//    const bool subjInRange  // 31.oct.2000, 08.nov.2000 back again
#   if PUNISHM_2ND_REPORT_12_SEP_2001
                                 const char * const regionActKindsStr,
                                 const bool rgnAKSMatch,
#   else  // of PUNISHM_2ND_REPORT_12_SEP_2001
                                 const char * const, const bool,
#   endif  // of PUNISHM_2ND_REPORT_12_SEP_2001

                                 const bool dKSMatch,   // 20060414

                                 const char * sessKindsString,// NULL   // 20070829
                                 const bool sKMatch,          // false  // 20070829
                                 const bool sessDate,         // false  // 20070829
                                 const bool sessBeforeFin,    // = true // 20070829
                                 const char * ageString       // = NULL // 20070914
                                )
 {
  if (dest && destSz && what && pHighRange &&
      typesString && typesString [0])
   {
    stringToSQLCharSet kRS (requestKindsString) ;
    stringToSQLCharSet gJRS (globJudgemResultString) ;
    stringToSQLCharSet dRS (decisionResultString) ;
    stringToSQLCharSet sRS (sessResultString) ;
    stringToSQLCharSet sKS (sessKindsString) ;

#   if APPEAL
#   else  // APPEAL
    stringToSQLCharSet lK2nd (KIND_2ND_LAWSUITS) ;
#   endif  // APPEAL  // 31.mar.2000
    ostrstream oss (dest, destSz - 1) ;
    bool res ;
//    char eD [16] ;  // 15.apr.2003
    const char * const dateField = finishedTest ? "F_FINISHED" :
                                                  "F_DATE" ;

    const bool lookInGlobJudgem =
                  globJudgemResultString && globJudgemResultString [0] ;
    const bool lookInDecision =
                      decisionResultString && decisionResultString [0] ;
    const bool lookInSess = (sessResultString && sessResultString [0])
                            ||
                            (sessKindsString && sessKindsString [0])  ;
#   if PUNISHM_2ND_REPORT_12_SEP_2001
    const bool
      lookInRequests = regionActKindsStr && regionActKindsStr [0] ;
    const bool lookInOtherTables =
      lookInGlobJudgem || lookInDecision || lookInSess || lookInRequests ;
#   else  // of PUNISHM_2ND_REPORT_12_SEP_2001
    const bool lookInOtherTables =
                      lookInGlobJudgem || lookInDecision || lookInSess ;
#   endif  // of PUNISHM_2ND_REPORT_12_SEP_2001
    const char * const tlp = "L." ;
    const char * const tLPref = lookInOtherTables ? tlp : "" ;
    const char * const tSPref = (lookInOtherTables || sessDate) ? "S." : "" ;  // 20070829

//    ostrstream (eD, sizeof (eD))  // 15.apr.2003 - begin
//      << CDateToSQLDateString (* pHighRange) << ends ;
//    eD [sizeof (eD) - 1] = '\0' ;  // 15.apr.2003 - end
    oss
      << "SELECT " << what << " FROM T_LAWSUIT " ;
    if (lookInOtherTables)
      oss << "L " ;
    if (lookInDecision || lookInGlobJudgem)
      oss << ", T_DECISION D " ;
    if (lookInSess)
      oss << ", T_SESSION S " ;
    oss <<
           "WHERE "
             << tLPref
             << "F_TYPE IN ("
             << stringToSQLCharSet (typesString).getTheSet () << ") "
           "AND " ;
    oss << tLPref << "F_TYPE != \'" << TYPE_FIRM_LAW << "\' "
           "AND " ;
//// 31.oct.2000 - begin, 08.nov.2000 back again
//    if (subjectLo && subjectHi && subjectLo <= subjectHi)
//      oss << "(" << (subjInRange ? "" : "NOT") << " "
//          <<   tLPref << "F_SUBJECT "
//          <<  "BETWEEN " << subjectLo << " AND " << subjectHi
//          << ") AND " ;
//// 31.oct.2000 - end, 08.nov.2000 back again
#   if APPEAL
#   else  // of APPEAL
    if (! strchr (typesString, TYPE_ADMIN_LAW))
      oss <<
             "NOT (" << tLPref << "F_KIND = \'"
               << KIND_2ND_ADMIN_PUNISHMENT_LAW << "\') "  // 12.feb.2001,
   // instead of f...d private complaints - concern (private) complaints
           "AND " ;
    oss << tLPref << "F_KIND IN (" << lK2nd.getTheSet () << ") "
           "AND " ;
#   endif  // of APPEAL  // 31.mar.2000
    if (composition)
      oss    << tLPref << "F_COMPOSITION = " << composition << " "
           "AND " ;
    if (pLowRange)
     {
             // 20070829
	oss << ( ( (!finishedTest) && sessDate )  ? tSPref : tLPref) << dateField << " BETWEEN "
	    << (* pLowRange).Value() << " AND "
	    << (* pHighRange).Value() << " " ;  // 15.apr.2003 - end
     }
    else
     {
//20070829      oss <<  "(" << tLPref << dateField << " IS NULL ""OR "
	    oss <<  "(" <<( ( (!finishedTest) && sessDate )  ? tSPref : tLPref) << dateField << " IS NULL ""OR "
	    << ( ( (!finishedTest) && sessDate )  ? tSPref : tLPref) << dateField << " >= "
	    << (* pHighRange).Value() << ") " ;  // 15.apr.2003 - end
     }
    if (excludeCreatedsLater)
      oss <<
           "AND "
             << tLPref << "F_DATE < " << (* pHighRange).Value() << " " ;//15.apr.2003
    if (requestKindsString && requestKindsString [0])
      oss <<
           "AND " << tLPref <<
             "F_SOURCE_KIND "
             << (rKSMatch ? "" : "NOT ")
             << "IN (" << kRS.getTheSet () << ") " ;
    else
     {  // (boza - like) - begin
        // при промяна на бозата да се коригира и пресмятането на код 10302 - Справка II - repo2sae.fmd
        // CountAdminLaws_Confirm_Abolish(...) - (report2.cpp) 
      const size_t cOSZ = strlen (KIND_PRIVATE_COMPLAINTS) +
                      strlen (KIND_SLOWDOWN_COMPLAINTS) +  // 24.oct.2000
                          strlen (KIND_PRIVATE_OBJECTIONS) + 1 ;
      char * tmpComplObj = new char [cOSZ] ;

      if (tmpComplObj)
       {
        ostrstream (tmpComplObj, cOSZ)
          << KIND_PRIVATE_COMPLAINTS
          << KIND_SLOWDOWN_COMPLAINTS  // 24.oct.2000
          << KIND_PRIVATE_OBJECTIONS << ends ;
        tmpComplObj [cOSZ - 1] = '\0' ;
        oss <<
           "AND NOT ("
                       << tLPref << "F_SOURCE_KIND IN ("
               << stringToSQLCharSet (tmpComplObj).getTheSet ()
               <<  ")) " ;
        delete [] tmpComplObj ;
       }
     }  // (boza - like) - end
#   if APPEAL
#   else  // of APPEAL

   if (forMinistry) {}
#   endif  // of APPEAL
    if (lookInDecision || lookInGlobJudgem)
     {
      oss <<
           "AND ("
                 "("
                    "D.F_NO = L.F_NO "
                  "AND "
                    "D.F_YEAR = L.F_YEAR "
                  "AND "
                    "D.F_TYPE = L.F_TYPE "
                  "AND "
                    "D.F_RESULT "
                           << (dKSMatch ? "" : "NOT ")

                            <<" IN (" <<
                                         (lookInDecision ?
                                            dRS.getTheSet ()
                                            : gJRS.getTheSet ()
						     )
                                                          << ") "
                 ") "
               ") " ;
      oss <<
           "AND L.F_FINISHED IS NOT NULL AND "
               "D.F_DATE <= L.F_FINISHED " ;
      oss <<
           "AND (" << tLPref << "F_DEC_MADE IS NULL OR "
                   << tLPref << "F_DEC_MADE <= D.F_DATE) " ;//02.mar.2001
                                                            // '<' to '<='
     }
    if (lookInSess
//         && sessResultString && sessResultString [0]
       )
     {
      oss <<
           "AND S.F_NO = L.F_NO "
           "AND S.F_YEAR = L.F_YEAR "
           "AND S.F_TYPE = L.F_TYPE " ;

	// 2015:189 TRQ/FIX: link session to decision
	if (lookInDecision || lookInGlobJudgem)
		oss << "AND S.F_DATE = D.F_DATE AND S.F_KIND = D.F_SESSION_KIND ";

	if (sessResultString && sessResultString [0])   //20070829
         oss <<
           "AND S.F_RESULT IN (" << sRS.getTheSet () << ") " ;

      if (sessKindsString && sessKindsString [0])   //20070829 -->
         oss <<
           "AND "
             "S.F_KIND " << (sKMatch ? "" : "NOT ")
                         << "IN (" << sKS.getTheSet () << ") " ;   // <--

      if (sessBeforeFin)  // 20070829
         oss <<
           "AND "
                  "L.F_FINISHED IS NOT NULL AND "
                  "S.F_DATE <= L.F_FINISHED " ;
      oss <<
           "AND (" << tLPref << "F_DEC_MADE IS NULL OR "
                   << tLPref << "F_DEC_MADE <= S.F_DATE) " ; //02.mar.2001
                                                           //  '<' to '<='
      oss <<
         "AND S.F_RESULT != \'" << RESULT_UNFIXED << "\' " ;
      oss <<
         "AND S.F_RESULT != \'" << RESULT_ABORTED << "\' " ; //20050322
     }
#   if PUNISHM_2ND_REPORT_12_SEP_2001
    if (lookInRequests)
     {
      oss <<
         "AND (L.F_SOURCE_TYPE " << (rgnAKSMatch ? "=" : "!=")
	    << " \'" << TYPE_REQUEST << "\' " << (rgnAKSMatch ? "AND" : "OR")
	    << " ((SELECT COUNT (R.F_NO) FROM T_CONNECT R "
		    "WHERE " //L.F_SOURCE_TYPE = \'" << TYPE_REQUEST << "\' AND "
		    "L.F_TYPE = R.F_TYPE AND "
		    "L.F_YEAR = R.F_YEAR AND "
		    "L.F_NO = R.F_NO AND "
		    "R.F_CONNECT_TYPE = \'" << CONNECT_TYPE_LOWER_INSTANCE << "\' AND "
		    "R.F_DECISION_KIND IN (" << SQLCharSet (regionActKindsStr)
					    << ")"
		   ") " << (rgnAKSMatch ? "!=" : "=") << " 0"
		  ")"


		 ")" ;
     }
#   else  // of PUNISHM_2ND_REPORT_12_SEP_2001
#   endif  // of PUNISHM_2ND_REPORT_12_SEP_2001

    // 20070914 -->
    if (ageString && ageString [0])
      oss <<
           "AND "
             << tLPref << "F_AGE IN ("
                            << stringToSQLCharSet (ageString).getTheSet ()
                                << ") " ;
    oss
      << ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    res = criteria_finished_ok (dest, "composeReportLawsCriteria2") ;
    return res ;
   }
  else
    return false ;
 }  // composeReportLawsCriteria2

# endif  // of INSTANCE

//
