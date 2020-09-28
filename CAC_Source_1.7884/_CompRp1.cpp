#include "cac.h"  // 18.may.2001
//# include "all.h"
bool composeReportLawsConnectCriteria (char * dest, size_t destSz,
                                const char * what,
                                const char * typesString,
                                const long int composition,
                                const CDate * pHighRange,
                                const CDate * pLowRange,        // = NULL,
                                const char * sourceTypesString,   // = NULL,
                                const char * const lawKindsString,// = NULL,
                                const bool lKSMatch,              // = true,
                                const char * connectTypesString,  // = NULL,
                                const bool lConnSMatch            // = true
                               )
{
  if (dest && destSz && what && pHighRange &&
      typesString && typesString [0]
     )
   {
    stringToSQLCharSet sTS (sourceTypesString) ;
    stringToSQLCharSet connTS (connectTypesString) ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    stringToSQLCharSet lK1st (KIND_1ST_LAWSUITS) ;
#   endif  // APPEAL  // 31.mar.2000
# endif

    ostrstream oss (dest, destSz - 1) ;
    oss << "SELECT " << what << " FROM T_LAWSUIT L ";
    if (lConnSMatch) oss << ", T_CONNECT C ";
    oss << "WHERE L.F_TYPE IN ("
	  <<  stringToSQLCharSet (typesString).getTheSet () << ") "
           "AND " ;
# if INSTANCE
    oss << "L.F_TYPE != \'" << TYPE_FIRM_LAW << "\' "
         "AND " ;
#   if APPEAL
#   else  // of APPEAL
    oss << "L.F_KIND IN (" << lK1st.getTheSet () << ") "
	     "AND " ;
#       if AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#       else  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
    oss << "NOT (" << "L.F_KIND = \'" << KIND_PRIVATE_PUNISHMENT_LAW
			 << "\' AND "
			 // 2016:060 subject -> source
			 //<< "L.F_SUBJECT = "  << SUBJECT_1905 << ""
			 << "L.F_SOURCE_KIND = '" << KIND_CANCEL_ENACTMENT << "'"
		   ") "
	     "AND " ;
#       endif  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#   endif  // of APPEAL  // 31.mar.2000 - end
# endif  // of INSTANCE
    if (sourceTypesString && sourceTypesString [0])
      oss    << "L.F_SOURCE_TYPE IN (" << sTS.getTheSet ()
             << ") "
           "AND " ;
    if (composition)
      oss    << "L.F_COMPOSITION = " << composition << " "
	     "AND " ;
    if (pLowRange)
     {
      oss << "L.F_DATE "
		 "BETWEEN " << (* pLowRange).Value() << " AND "
				 << (* pHighRange).Value() << " " ;  // 15.apr.2003 - end
     }
    else
     {
      oss <<  "(" ;
      oss <<  "L.F_DATE " ;
      oss <<  " IS NULL "
                "OR "
                << "L.F_DATE >= " << (* pHighRange).Value() << ") " ;
     }
    if (lawKindsString && lawKindsString [0])
      oss <<
           "AND " <<
             "L.F_KIND " << (lKSMatch ? "" : "NOT ") << "IN ("
                         << stringToSQLCharSet (lawKindsString).getTheSet ()
                         << ") " ;

    if (lConnSMatch)
    {
        if ( connectTypesString && connectTypesString [0] )
        {
          oss <<
               "AND "
                 "C.F_NO = L.F_NO "
               "AND "
                 "C.F_YEAR = L.F_YEAR "
               "AND "
                 "C.F_TYPE = L.F_TYPE " ;
          oss <<
               "AND "
                 "C.F_CONNECT_TYPE " << "IN (" << connTS.getTheSet () << ") " ;
        }
    }
    else
    {
      oss <<
           "AND(  SELECT COUNT(C.F_NO) from T_CONNECT C " <<
                " WHERE ( C.F_NO = L.F_NO AND C.F_YEAR = L.F_YEAR AND C.F_TYPE = L.F_TYPE "
                " AND "
                " C.F_CONNECT_TYPE " << "IN (" << connTS.getTheSet () << ") "
                       ")"
              ")  = 0";
    }
    oss
      << ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeReportLawsConnectCriteria") ;
   }
  else
    return false ;

} // composeReportLawsConnectCriteria

bool composeReportLawsCriteria (char * dest, size_t destSz,
                                const char * what,
                                const char * typesString,
                                const long int composition,
					  const CDate * pHighRange,
                                const CDate * pLowRange,
                                const char * resultsString,
                                const char * requestKindsString,
                                const char * ageString,
                                const bool finishedTest,
                                const char * sourceTypesString,
                                const char * grantedsString,
                                const char * sessKindsString,
                                const bool sKMatch,
                                const bool excludeCreatedsLater,
                                const bool rSMatch,
                                const char * const decResultsString,
                                const char * const lawKindsString,
                                const bool lKSMatch,
                                const senderChecksActions aboutTheSender,
                                const bool requestCurrentlyQuick,  // 24.sep.2003
                                const bool corr_crit,  // = false
                                const bool decResMatch,  // = true    // 24    // 20070810
                                const bool sessDate,     // false     // 25    // 20070829
                                const bool sessBeforeFin // = true    // 26    // 20070829
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
/* 20070828 -->
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    stringToSQLCharSet lK1st (KIND_1ST_LAWSUITS) ;
#   endif  // APPEAL  // 31.mar.2000
# endif
20070828 */
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
    stringToSQLCharSet lK1st (KIND_1ST_LAWSUITS) ;
#endif  // ADMIN || AREAL || MILITARY

    ostrstream oss (dest, destSz - 1) ;
//    char eD [16] ;  // 15.apr.2003
    const char * const dateField = finishedTest ? "F_FINISHED" :
								  "F_DATE" ;
    const bool lookInSessions = resultsString || sessKindsString ;
    const bool lookInDecisions = decResultsString != NULL ;
    const bool lookInRequests = requestCurrentlyQuick ;  // 24.sep.2003
    const char * const tlp = "L." ;
    const char * const tLPref =
	(lookInSessions || lookInDecisions || lookInRequests) ? tlp : "" ;
    const char * const tSPref =
	(lookInSessions || sessDate) ? "S." : "" ;

    oss
	<< "SELECT " << what << " FROM T_LAWSUIT " ;
    if (lookInSessions || lookInDecisions || lookInRequests)
	oss << "L " ;
    if (lookInDecisions)
	oss << ", T_DECISION D " ;
    if (lookInSessions)
	oss << ", T_SESSION S " ;
    if (lookInRequests)
	oss << ", T_REQUEST R " ;
    oss <<
	     "WHERE "
		 << tLPref
		 << "F_TYPE IN ("
		 << stringToSQLCharSet (typesString).getTheSet () << ") "
	     "AND " ;

// 20070611  -->
    if (( corr_crit ) && (strcmp(tLPref, "L.")==0))
	 oss << "L.F_CORRUPT > 0 AND "  ;

// 20070611  <--



# if INSTANCE
    oss << tLPref << "F_TYPE != \'" << TYPE_FIRM_LAW << "\' "
           "AND " ;
#   if APPEAL
#   else  // of APPEAL
    oss << tLPref << "F_KIND IN (" << lK1st.getTheSet () << ") "
           "AND " ;
#       if AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#       else  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
    oss << "NOT (" << tLPref
			 << "F_KIND = \'" << KIND_PRIVATE_PUNISHMENT_LAW
					  << "\' AND "
			 << tLPref
			 // 2016:060 subject -> source
			 //<< "F_SUBJECT = "  << SUBJECT_1905 << ""
			 << "F_SOURCE_KIND = '" << KIND_CANCEL_ENACTMENT << "'"
		   ") "
	     "AND " ;
#       endif  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#   endif  // of APPEAL  // 31.mar.2000 - end
# endif  // of INSTANCE
    if (sourceTypesString && sourceTypesString [0])
      oss    << tLPref << "F_SOURCE_TYPE IN (" << sTS.getTheSet ()
             << ") "
           "AND " ;
    if (grantedsString && grantedsString [0])
      oss    << tLPref << "F_GRANTED IN (" << gS.getTheSet () << ") "
	     "AND " ;
    if (composition)
      oss    << tLPref << "F_COMPOSITION = " << composition << " "
           "AND " ;
    if (pLowRange)
     {
      //oss << tLPref << dateField ;
      oss << ( ( (!finishedTest) && sessDate ) ? tSPref : tLPref) << dateField ;     // 20070829
      oss << " "
		   "BETWEEN " << (* pLowRange).Value() << " AND "
				   << (* pHighRange).Value() << " " ;  // 15.apr.2003 - end
     }
    else
     {
	oss <<  "(" ;

	// oss << tLPref << dateField ; // 20070829
	oss << ( ( (!finishedTest) && sessDate ) ? tSPref : tLPref) << dateField ;

	oss << " IS NULL "
		    "OR "
		    //<< tLPref  20070829
		    << ( ( (!finishedTest) && sessDate ) ? tSPref : tLPref)
		    << dateField << " >= " << (* pHighRange).Value()
						   << ") " ;  // 15.apr.2003 - end
     }
    if (excludeCreatedsLater)
	oss <<
           "AND "
		 << tLPref << "F_DATE < " << (* pHighRange).Value() << " " ;//15.apr.2003
    if (ageString && ageString [0])
      oss <<
           "AND "
             << tLPref << "F_AGE IN ("
                            << stringToSQLCharSet (ageString).getTheSet ()
                                << ") " ;
    if (requestKindsString && requestKindsString [0])
      oss <<
           "AND " << tLPref <<
             "F_SOURCE_TYPE = \'" << TYPE_REQUEST << "\' "
           "AND " << tLPref <<
             "F_SOURCE_KIND IN (" << kRS.getTheSet () << ") " ;
    if (lawKindsString && lawKindsString [0])
      oss <<
           "AND " << tLPref <<
             "F_KIND " << (lKSMatch ? "" : "NOT ") << "IN ("
                  << stringToSQLCharSet (lawKindsString).getTheSet ()
                    << ") " ;
// 20070607 -->
    if (corr_crit)
    {
      oss << "AND " << tLPref << "F_CORRUPT > 0 " ;
    }
// 20070607 <--
    if (lookInSessions)
     {
      oss <<
           "AND "
             "S.F_NO = L.F_NO "
           "AND "
             "S.F_YEAR = L.F_YEAR "
           "AND "
             "S.F_TYPE = L.F_TYPE " ;
      if (sessBeforeFin)  // 20070829
         oss <<
              "AND "
                  "L.F_FINISHED IS NOT NULL AND "
                  "S.F_DATE <= L.F_FINISHED " ;
      if (
          resultsString &&  // 29.jun.2000
          resultsString [0])
        oss <<
           "AND "
             "S.F_RESULT " << (rSMatch ? "" : "NOT ")
                           << "IN (" << rTBS.getTheSet () << ") " ;
      if (sessKindsString && sessKindsString [0])
        oss <<
           "AND "
             "S.F_KIND " << (sKMatch ? "" : "NOT ")
                         << "IN (" << sKS.getTheSet () << ") " ;
     }
    if (lookInDecisions)
     {
      oss <<
           "AND "
             "D.F_NO = L.F_NO "
           "AND "
             "D.F_YEAR = L.F_YEAR "
           "AND "
             "D.F_TYPE = L.F_TYPE "
           "AND "
               "L.F_FINISHED IS NOT NULL AND "
               "D.F_DATE <= L.F_FINISHED " ;
      if (decResultsString [0])
        oss <<
           "AND "
             "D.F_RESULT " << (decResMatch ? "" : "NOT ")
            << "IN ("
            << stringToSQLCharSet (decResultsString).getTheSet ()
                        << ") " ;
     }
    if (aboutTheSender != IgnoreSender)
    {
	 // 20061220  oss <<
	 // 20061220      "AND " << tLPref << "F_SENDER IS "
	 // 20061220         << (aboutTheSender == SenderMustNotBeEmpty ? "NOT " : "")
	 // 20061220                                         << "NULL " ;
    }
    if (lookInRequests)
     {
      oss <<
           "AND "
             "R.F_NO = L.F_SOURCE_NO "
           "AND "
             "R.F_YEAR = L.F_SOURCE_YEAR "
           "AND "
             "R.F_TYPE = L.F_SOURCE_TYPE " ;
       {
        arrayOfLong flagVArr ;

        add_set_bit_values (flagVArr, QUICK_QUICK, MAX_QUICK_FLAGS) ;
        oss << "AND R.F_QUICK IN (" << flagVArr << ") " ;
       }
     }
    oss
      << ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeReportLawsCriteria") ;
   }
  else
    return false ;
 }  // composeReportLawsCriteria

// 20080625 ->
void generateFlagSet (ostream & os, const char JUDGEMENT_X
     )//generates set of allowable values for F_FLASG_X, T_JUDGEMENT
 {
  arrayOfLong flagsArr ;

  add_set_bit_values (flagsArr, JUDGEMENT_X,// JUDGEMENT_APPENDIX_25	= 0x08,
                      MAX_JUDGEMENT_FLAGS) ;//const int MAX_JUDGEMENT_FLAGS = 6;
  os << flagsArr ;
 }


bool composeReportJudgementCriteria (char * dest, size_t destSz,
                                 const char * what,
                                 const char * lawTypesString,
                                 const long int composition,
                                 const CDate * pLowRange,
                                 const CDate * pHighRange,

                                 const char * judgementKinds,
                                 const char JUDGEMENT_X
                                )
{
  if (dest && destSz && what
       && lawTypesString && lawTypesString [0]
       && pLowRange && pHighRange)
  {
    stringToSQLCharSet cS (UCN_CITIZENS) ;
    stringToSQLCharSet kS (judgementKinds) ;

    ostrstream oss (dest, destSz - 1) ;

    oss << "SELECT " << what << " FROM T_JUDGEMENT J, T_LAWSUIT L " ;
    oss << "WHERE J.F_TYPE IN ("
             << stringToSQLCharSet (lawTypesString).getTheSet () << ") ";

    if (composition)
    {  oss << "AND L.F_COMPOSITION = " << composition << " " ;
    }

    oss <<    "AND J.F_DATE BETWEEN " << (* pLowRange).Value() << " AND "
                                       << (* pHighRange).Value() << " " ;

    if (judgementKinds && judgementKinds [0])
      oss <<  "AND J.F_KIND IN (" << kS.getTheSet () << ") "
              "AND J.F_UCN_TYPE IN (" << cS.getTheSet () << ") " ;

    if (JUDGEMENT_X)
    {  oss << "AND J.F_FLAGS_X IN (" ;
       generateFlagSet (oss, JUDGEMENT_X );
       oss << ") " ;
    }
    oss <<   "AND L.F_NO = J.F_NO "
             "AND L.F_YEAR = J.F_YEAR "
             "AND L.F_TYPE = J.F_TYPE " ;
    oss << ";" ;
    oss << ends ;

    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeReportJudgementCriteria") ;
   }
  else
    return false ;
 }  // composeReportJudgementCriteria


bool composeReportDecisionCriteria (char * dest, size_t destSz,
                                 const char * what,
                                 const char * lawTypesString,
                                 const long int composition,
                                 const CDate * pLowRange,
                                 const CDate * pHighRange,

                                 const char * decisionKinds
                                )
{
  if (dest && destSz && what
       && lawTypesString && lawTypesString [0]
       && pLowRange && pHighRange)
  {
    stringToSQLCharSet decisionS (decisionKinds) ;
    ostrstream oss (dest, destSz - 1) ;

    oss << "SELECT " << what << " FROM T_DECISION D, T_LAWSUIT L " ;
    oss << "WHERE D.F_TYPE IN ("
             << stringToSQLCharSet (lawTypesString).getTheSet () << ") ";

    if (composition)
    {  oss << "AND L.F_COMPOSITION = " << composition << " " ;
    }
    oss <<    "AND D.F_FROM_DATE BETWEEN " << (* pLowRange).Value() << " AND "
                                            << (* pHighRange).Value()<< " " ;

    if (decisionKinds && decisionKinds [0])
      oss <<  "AND D.F_KIND IN (" << decisionS.getTheSet () << ") " ;

    oss <<    "AND L.F_NO = D.F_NO "
              "AND L.F_YEAR = D.F_YEAR "
              "AND L.F_TYPE = D.F_TYPE " ;

    oss << ";" ;
    oss << ends ;

    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeReportDecisionCriteria") ;
   }
  else
    return false ;
}
// 20080625 <-

bool composeReportSidesCriteria (char * dest, size_t destSz,
                                 const char * what,
                                 const char * typesString,
                                 const long int composition,
                                 const CDate * pHighRange,
                                 const CDate * pLowRange,
                                 const char * resultsString,
                                 const char * judgedString,
                                 const char * judgementKinds,
                                 const char * ,  // oldString,  // 18.dec.2003
                                 const char * punishmentString,
                                 const char * recidiveString,
                                 const char * lawKindString,
                                 const char * const freedStr,
                                 const arrayOfLong * const pFlagVArr,
                             const char * const lawPS2Str,  // 11.jan.2001
                             const bool corr_crit           // 20070611
                                )
 {
  if (dest && destSz && what && pHighRange &&
      typesString && typesString [0])
   {
    stringToSQLCharSet rS (resultsString) ;
    stringToSQLCharSet jS (judgedString) ;
    //stringToSQLCharSet oS (oldString) ;  // 18.dec.2003
    stringToSQLCharSet pS (punishmentString) ;
    stringToSQLCharSet cS (UCN_CITIZENS) ;
    stringToSQLCharSet kS (judgementKinds) ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    stringToSQLCharSet lK1st (KIND_1ST_LAWSUITS) ;
#   endif  // APPEAL  // 31.mar.2000 - end
# endif
    ostrstream oss (dest, destSz - 1) ;
//    char eD [16] ;  // 15.apr.2003

//    ostrstream (eD, sizeof (eD))  // 15.apr.2003 - begin
//      << CDateToSQLDateString (* pHighRange) << ends ;
//    eD [sizeof (eD) - 1] = '\0' ;  // 15.apr.2003 - end
    oss
      << "SELECT " << what <<
                              " FROM T_LAWSUIT L, " ;
    if (resultsString)
      oss <<
                               "T_SESSION S, " ;
    oss <<
                               "T_JUDGEMENT J " ;
    oss <<
           "WHERE "
//             "L.F_TYPE = \'" << type << "\' "
             "L.F_TYPE IN ("
             << stringToSQLCharSet (typesString).getTheSet () << ") "
            ;

// 20070611  -->
    if ( corr_crit )
    { }
// 20070611  <--

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    oss << "AND L.F_KIND IN (" << lK1st.getTheSet () << ") " ;
#       if AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#       else  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
    oss << "AND NOT (L.F_KIND = \'" << KIND_PRIVATE_PUNISHMENT_LAW
					  << "\' AND "
					  // 2016:060 subject -> source
					  //<< "L.F_SUBJECT = "  << SUBJECT_1905 << ""
					  << "L.F_SOURCE_KIND = '" << KIND_CANCEL_ENACTMENT << "'"
		   ") " ;
#       endif  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#   endif  // of APPEAL  // 31.mar.2000
# endif
    if ((! resultsString) && composition)  // Otherwise tests the sessions - see below
	oss <<
           "AND "
             "L.F_COMPOSITION = " << composition << " " ;
    oss <<
	     "AND " ;
    if (pLowRange)
	oss << "L.F_FINISHED "
		   "BETWEEN " << (* pLowRange).Value() << " AND "
                           << (* pHighRange).Value() << " " ;
    else
      oss <<  "(L.F_FINISHED IS NULL "
                "OR L.F_FINISHED >= " << (* pHighRange).Value() << ") " ;
// 15.apr.2003 - end
    if (lawKindString && lawKindString [0])
      oss <<
	     "AND "
             "L.F_KIND IN ("
               << stringToSQLCharSet (lawKindString).getTheSet ()
               << ") " ;


    if (lawPS2Str && lawPS2Str [0])  // 11.jan.2001 - begin
    {
/*  20071203 -->
      oss <<
           "AND "
               "L.F_PS2_KIND IN ("    // NOT_CORRECT
               << stringToSQLCharSet (lawPS2Str).getTheSet ()
               << ") " ;  // 11.jan.2001 - end
*/
      oss <<
           "AND "
                  "( SELECT COUNT(*) FROM T_CONNECT C WHERE "
                  "    (C.F_NO=L.F_NO)AND(C.F_YEAR=L.F_YEAR)AND(C.F_TYPE=L.F_TYPE)AND"
                  "    (C.F_CONNECT_TYPE in ("
                         << stringToSQLCharSet (lawPS2Str).getTheSet ()
                         << ") )"
                         << ")>0 " ;
//  20071203 <--
    }
    if (resultsString)
     {
      oss <<
           "AND "
             "S.F_NO = L.F_NO "
           "AND "
             "S.F_YEAR = L.F_YEAR "
           "AND "
             "S.F_TYPE = L.F_TYPE " ;
      if (composition)
        oss <<
           "AND "
             "S.F_COMPOSITION = " << composition << " " ;
      if (resultsString [0])
        oss <<
           "AND "
             "S.F_RESULT IN (" << rS.getTheSet () << ") " ;
     }
    oss <<
           "AND "
             "J.F_NO = L.F_NO "
           "AND "
             "J.F_YEAR = L.F_YEAR "
           "AND "
             "J.F_TYPE = L.F_TYPE " ;
    if (judgementKinds && judgementKinds [0])
      oss <<
           "AND "
             "J.F_KIND IN (" << kS.getTheSet () << ") " ;
    oss <<
//           "AND "                         //
//             "J.F_DATE = L.F_FINISHED "   // -  Are these necessary ?
           "AND "
             "J.F_UCN_TYPE IN (" << cS.getTheSet () << ") " ;
    //if (oldString && oldString [0])  // 18.dec.2003 - begin
    //  oss <<
    //       "AND "
    //         "J.F_OLD IN (" << oS.getTheSet () << ") " ;  // 18.dec.2003 - end
    if (judgedString && judgedString [0])
      oss <<
           "AND "
             "J.F_JUDGED IN (" << jS.getTheSet () << ") " ;
    if (punishmentString && punishmentString [0])
      oss <<
           "AND "
             "J.F_PUNISHMENT IN (" << pS.getTheSet () << ") " ;
    if (recidiveString && recidiveString [0])
      oss <<
           "AND "
             "J.F_RECIDIVE IN ("
               << stringToSQLCharSet (recidiveString).getTheSet ()
               << ") " ;
    if (freedStr && freedStr [0])
     {
//      oss <<              // 25.jul.2001 - begin
//           "AND "
//             "J.F_FREE IN ("
//               << stringToSQLCharSet (freedStr).getTheSet ()
//               << ") " ;  // 25.jul.2001 - end
     }
    if (pFlagVArr && pFlagVArr -> Count ())
	oss <<
	     "AND "
		 "J.F_FLAGS IN (" << (* pFlagVArr) << ") " ;
    oss << " AND J.F_FLAGS_X IN (" ;  // 18.dec.2003 from F_FLAGS
    {
	arrayOfLong flagsArr ;
	add_set_bit_values (flagsArr, JUDGEMENT_X_PRIORITY, JUDGEMENT_X_ALL);
	oss << flagsArr ;
    }
    oss << ") " ;
    oss
	<< ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeReportSidesCriteria") ;
   }
  else
    return false ;
 }  // composeReportSidesCriteria

bool composeReportDefendantsCriteria (char * dest, size_t destSz,
                                      const char * what,
                                      const char * typesString,
                                      const long int composition,
                                      const CDate * pHighRange,
                                      const CDate * pLowRange,
                                      const bool corr_crit     ) // 20070611
 {
  if (dest && destSz && what && pHighRange && pLowRange &&
      typesString && typesString [0])
   {
    stringToSQLCharSet cS (UCN_CITIZENS) ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    stringToSQLCharSet lK1st (KIND_1ST_LAWSUITS) ;
#   endif  // APPEAL  // 31.mar.2000
# endif
    ostrstream oss (dest, destSz - 1) ;
//    char eD [16] ;  // 15.apr.2003

//    ostrstream (eD, sizeof (eD))  // 15.apr.2003 - begin
//      << CDateToSQLDateString (* pHighRange) << ends ;
//    eD [sizeof (eD) - 1] = '\0' ;  // 15.apr.2003 - end
    oss
      << "SELECT " << what <<
                              " FROM T_LAWSUIT L, " ;
    oss <<
					 "T_SIDE_WIND J"" "
                                ;
    oss <<
           "WHERE "
             "L.F_TYPE IN ("
             << stringToSQLCharSet (typesString).getTheSet () << ") "
            ;

// 20070611  -->
    if ( corr_crit )
       oss << "AND L.F_CORRUPT > 0 "  ;

// 20070611  <--

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    oss << "AND L.F_KIND IN (" << lK1st.getTheSet () << ") " ;
#       if AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#       else  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
    oss << "AND NOT (L.F_KIND = \'" << KIND_PRIVATE_PUNISHMENT_LAW
						<< "\' AND "
						// 2016:060 subject -> source
						//<< "L.F_SUBJECT = " << SUBJECT_1905 << ""
						<< "L.F_SUBJECT = '" << KIND_CANCEL_ENACTMENT << "'"
		   ") " ;
#       endif  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#   endif  // of APPEAL  // 31.mar.2000
# endif
    if (composition)
      oss <<
           "AND "
             "L.F_COMPOSITION = " << composition << " " ;
    oss <<
	     "AND " ;
    if (pLowRange)
	oss << "L.F_FINISHED BETWEEN "
        << (* pLowRange).Value()  << " AND " << (* pHighRange).Value() << " " ;
    else
      oss <<  "(L.F_FINISHED IS NULL "
                "OR L.F_FINISHED >= " << (* pHighRange).Value() << ") " ;
// 15.apr.2003 - end
    oss <<
           "AND "
             "J.F_NO = L.F_NO "
	     "AND "
             "J.F_YEAR = L.F_YEAR "
           "AND "
             "J.F_TYPE = L.F_TYPE "
	     "AND "
		 "J.F_KIND = L.F_KIND "
	     "AND "
		 "J.F_DATE = L.F_DATE "
           "AND "
		 "J.F_UCN_TYPE IN (" << cS.getTheSet () << ") " ;
    oss
	<< ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest,
					   "composeReportDefendantsCriteria") ;
   }
  else
    return false ;
 }  // composeReportDefendantsCriteria

bool composeReportSessionsCriteria (char * dest, size_t destSz,
                                    const char * what,
                                    const char * typesString,
                                    const long int composition,
                                    const CDate * pHighRange,
                                    const CDate * pLowRange,
                                    const char * resultsString,
                                    const char * sessKindsString,
                                    const bool sKSMatch,
                             const bool excludeClosedSess,  // 04.may.2001
                             const bool beforeLawFinish  // 25.may.2001
                                   )
 {
  if (dest && destSz && what && pHighRange &&
      typesString && typesString [0])
   {
    stringToSQLCharSet rS (resultsString) ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    stringToSQLCharSet lK1st (KIND_1ST_LAWSUITS) ;
#   endif  // APPEAL  // 31.mar.2000
# endif
//    char eD [16] ;  // 15.apr.2003
    ostrstream oss (dest, destSz) ;

//    ostrstream (eD, sizeof (eD))  // 15.apr.2003 - begin
//      << CDateToSQLDateString (* pHighRange) << ends ;
//    eD [sizeof (eD) - 1] = '\0' ;  // 15.apr.2003 - end
    oss
      << "SELECT " << what << " FROM "
                               "T_LAWSUIT L, T_SESSION S " ;
    oss <<
           "WHERE "
//             "L.F_TYPE = \'" << type << "\' "
             "L.F_TYPE IN ("
             << stringToSQLCharSet (typesString).getTheSet () << ") "
            ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    oss << "AND L.F_KIND IN (" << lK1st.getTheSet () << ") " ;
#       if AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#       else  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
    oss << "AND NOT (L.F_KIND = \'" << KIND_PRIVATE_PUNISHMENT_LAW
						<< "\' AND "
						// 2016:060 subject -> source
						//<< "L.F_SUBJECT = " << SUBJECT_1905 << ""
						<< "L.F_SUBJECT = '" << KIND_CANCEL_ENACTMENT << "'"
		   ") " ;
#       endif  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#   endif  // of APPEAL  // 31.mar.2000
# endif
    oss <<
           "AND "
             "S.F_NO = L.F_NO "
           "AND "
             "S.F_YEAR = L.F_YEAR "
           "AND "
		 "S.F_TYPE = L.F_TYPE " ;
    if (composition)
      oss <<
           "AND "
             "S.F_COMPOSITION = " << composition << " " ;
    oss << "AND ("
        << (beforeLawFinish ? "" : "NOT ") << // 25.may.2001
		    "(L.F_FINISHED IS NULL OR "
//              "S.F_DATE"  25.may.2001 - begin
		    // 2011:193 don't use final, non-finishing sessions are checked too
		    "((S.F_KIND IN (" << stringToSQLCharSet(KIND_OPEN_SESSIONS).getTheSet() <<
		    ") AND S.F_DATE <= L.F_FINISHED) OR (S.F_KIND IN (" <<
		    stringToSQLCharSet(KIND_CLOSED_SESSIONS).getTheSet() <<
		    ") AND S.F_RETURNED <= L.F_FINISHED)) )) ";
    oss << "AND "
		 "S.F_RESULT != \'" << RESULT_UNFIXED << "\' " ;
    oss << "AND "
		 "S.F_RESULT != \'" << RESULT_ABORTED << "\' " ; //20050322
    if (resultsString && resultsString [0])
	oss <<
	     "AND "
		 "S.F_RESULT IN (" << rS.getTheSet () << ") " ;
    oss <<
           "AND " ;
    if (pLowRange)
	oss << "S.F_DATE BETWEEN "
	  << (* pLowRange).Value() << " AND " << (* pHighRange).Value() << " " ;
    else
      oss << "S.F_DATE >= " << (* pHighRange).Value() << " " ;  // 15.apr.2003 - end
    if (sessKindsString && sessKindsString [0])
	oss <<
       "AND "
         "S.F_KIND " << (sKSMatch ? "" : "NOT ") << "IN ("
			   << stringToSQLCharSet (sessKindsString).getTheSet ()
			   << ") " ;
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
    if (excludeClosedSess)  // 04.may.2001
	oss <<
       "AND "
//     "S.F_KIND != \'" << KIND_CLOSED_SESSION << "\' " ;  // 12.jan.2001
         "S.F_KIND NOT IN (" <<
               stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
			    << ") " ;  // 12.jan.2001
    oss
	<< ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeReportSessionsCriteria") ;
   }
  else
    return false ;
 }  // composeReportSessionsCriteria

bool composeRSCriteria (char * dest, size_t destSz, const char * what,
                        const char * resultsString,
                        const collCompBegDEndDBegN * cCBDEDBNPtr,
                        const char * ageString,
                        const char * pGrantChar,
                        const char * sessKindStr, bool sksEq,
                        const char * lawKindStr,
                        const bool followSessDate,
                        const bool includingClosedSessions
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
				, const bool observeInstLaws   // 20070907
#endif  // ADMIN || AREAL || MILITARY
                        )
 {
  if (dest && destSz && what && cCBDEDBNPtr &&
      strlen (cCBDEDBNPtr -> colleges))
   {
    stringToSQLCharSet rTBS (resultsString) ;
    ostrstream oss (dest, destSz - 1) ;
//    char bD [16] ;  // 15.apr.2003
//    char eD [16] ;  // 15.apr.2003
//# if INSTANCE
//# else
//    const bool closedSessNeed =
//      (cCBDEDBNPtr -> college == TYPE_PUNISHMENT_LAW) ||
//      (sksEq && sessKindStr &&
//       strchr (sessKindStr, KIND_CLOSED_SESSION)) ;
//# endif

//    ostrstream (bD, sizeof (bD))  // 15.apr.2003 - begin
//      << CDateToSQLDateString (cCBDEDBNPtr -> begD) << ends ;
//    bD [sizeof (bD) - 1] = '\0' ;
//    ostrstream (eD, sizeof (eD))
//      << CDateToSQLDateString (cCBDEDBNPtr -> endD) << ends ;
//    eD [sizeof (eD) - 1] = '\0' ;  // 15.apr.2003 - end
    oss
      << "SELECT " << what << " FROM T_SESSION S, T_LAWSUIT L "
         "WHERE " ;
    oss << (followSessDate ? "S.F_DATE" : "L.F_FINISHED") <<
		 " BETWEEN "
		 " " << cCBDEDBNPtr -> begD.Value() << " AND "
                  << cCBDEDBNPtr -> endD.Value() << " "  // 15.apr.2003 - end
	   "AND "
           "S.F_NO = L.F_NO "
         "AND "
           "S.F_YEAR = L.F_YEAR "
         "AND "
           "S.F_TYPE = L.F_TYPE "
	   "AND (L.F_FINISHED IS NULL OR "
	     "S.F_DATE <= L.F_FINISHED ) " ;

//20071002 # if INSTANCE
//20071002        oss <<
//20071002         "AND L.F_TYPE != \'" << TYPE_FIRM_LAW << "\' "  ;
//20071002
//20071002 #   if APPEAL
//20071002 #   else  // of APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY  // 20071002 -->

        if ( observeInstLaws ) // 20070907
	  {
	    oss <<
	     "AND L.F_KIND IN ("
		 << stringToSQLCharSet (cCBDEDBNPtr -> firstInstLaws ?
						KIND_1ST_LAWSUITS :
						KIND_2ND_LAWSUITS).getTheSet ()
		 << ") " ;
	  }
// 24.feb.2001 - begin
//    if (cCBDEDBNPtr -> firstInstLaws ||
//        ! strchr (cCBDEDBNPtr -> colleges, TYPE_ADMIN_LAW))
//      oss <<
//             "AND ""NOT ("
//                  << "L.F_SOURCE_KIND = "
//                    "\'" << KIND_CONCERN_COMPLAINT_PUN_INDOC << "\') "
    oss << "AND L.F_KIND != \'" << KIND_2ND_ADMIN_PUNISHMENT_LAW << "\' "
   //  instead of f...d private complaints - concern (private) complaints

// 24.feb.2001 - end
#       if AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
#       else  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
     ;
    oss << "AND NOT (L.F_KIND = \'" << KIND_PRIVATE_PUNISHMENT_LAW
					  << "\' AND "
					  // 2016:060 subject -> source
					  //<< "L.F_SUBJECT = " << SUBJECT_1905 << ""
					  << "L.F_SUBJECT << '" << KIND_CANCEL_ENACTMENT << "'"
		   ") "
#       endif  // of AREA_CANCEL_ENACTMENT_ON_FACE  // 25.jul.2001
			 ;

//20071002 #   endif  // of APPEAL  // 31.mar.2000 - end
//20071002 # else  // of INSTANCE
//20071002 # endif  // of INSTANCE

#endif  // ADMIN || AREAL || MILITARY   //20071002

//# if INSTANCE
//# else
//      if (! closedSessNeed)
//# endif

//        oss <<
//         "AND "
//           "S.F_KIND != \'" << KIND_CLOSED_SESSION << "\' " ;

      oss <<
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
      oss <<
         "AND "
           "S.F_RESULT != \'" << RESULT_ABORTED << "\' " ; //20050322
      if (resultsString
                        && resultsString [0]  // 22.feb.2000
         )
        oss <<
         "AND "
           "S.F_RESULT IN (" << rTBS.getTheSet () << ") " ;
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

//20071002 why ? -->
//20071002       if ( includingClosedSessions )
//20071002      { }
/*
        oss <<
// "AND S.F_KIND != \'" << KIND_CLOSED_SESSION << "\' " ;  // 12.jan.2001
           "AND S.F_KIND IN (" <<
               stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                            << ") " ;  // 12.jan.2001
*/
//20071002 why ? <--

//20071002 code from Petyo -->

      if (! includingClosedSessions)
        oss << "AND S.F_KIND NOT IN (" <<
               stringToSQLCharSet (KIND_CLOSED_SESSIONS).getTheSet ()
                            << ") " ;  // 12.jan.2001
//20071002 code from Petyo <--


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
      oss <<
         "ORDER BY S.F_DATE, S.F_YEAR, S.F_NO " << ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composeRSCriteria") ;
   }
  else
    return false ;
 }  // composeRSCriteria

# if INSTANCE
# else  // of INSTANCE
bool composePuniAdmLawsCriteria (char * dest, size_t destSz,
                                 const char * what,
                                 const long int composition,
                                 const CDate highRange,
                                 const CDate lowRange,
                                 const char * const decisResultString)
 {
  if (dest && destSz && what)
   {
    stringToSQLCharSet dRS (decisResultString) ;
    ostrstream oss (dest, destSz) ;

    oss
      << "SELECT " << what << " FROM T_LAWSUIT L, T_DECISION D "
           "WHERE L.F_KIND = \'" << KIND_ADMIN_PUNISHMENT_LAW << "\' " ;
    if (composition)
      oss <<
           "AND L.F_COMPOSITION = " << composition << " " ;
    oss <<
	     "AND L.F_FINISHED BETWEEN " << lowRange.Value() << " AND "
						     << highRange.Value() << " "
	     "AND ("
		     "("
                    "D.F_NO = L.F_NO "
                  "AND "
                    "D.F_YEAR = L.F_YEAR "
                  "AND "
                    "D.F_TYPE = L.F_TYPE "
                  "AND "
                    "D.F_RESULT IN (" << dRS.getTheSet () << ") "
                 ") "
               ") "
           "AND "
               "L.F_FINISHED IS NOT NULL AND "
               "D.F_DATE <= L.F_FINISHED " ;
    oss << ";" ;
    oss << ends ;
    dest [destSz - 1] = '\0' ;
    return criteria_finished_ok (dest, "composePuniAdmLawsCriteria") ;
   }
  else
    return false ;
 }  // composePuniAdmLawsCriteria

# endif

//
