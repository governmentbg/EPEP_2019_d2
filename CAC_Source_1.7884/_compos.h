# if ! defined (_COMPOS_H)
# define _COMPOS_H

// -------------------------- Compose ----------------------------------

// 20051010 -->
# if ! INSTANCE
// Regional court only
bool composeZZDNCriteria (char * dest,
                          size_t destSz,
                          const char * what,
                          const char * resultsString,
                          const ZZDN_Dlg_data * cCBDEDBN_ZZDNPtr
                         ) ;
# endif
// 20051010 <--

bool composeTBTSCriteria (char * dest, size_t destSz, const char * what,
                          const char * resultsString,
                          const collCompBegDEndDBegN * cCBDEDBNPtr,
                          const char * ageString = NULL,
                          const char * pGrantChar = NULL,
                          const char * sessKindStr = NULL,
                          bool sksEq = true,
                          const char * lawKindStr = NULL,
                          const char * sourceKindStr = NULL,
                          const bool followSessDate = true,
				  const char * const decisResultsStr = NULL,
                          const bool resultsStringResultsMatch = true,
                          const bool excudeAbolishedResults = false
                         ) ;
bool composeReportLawsCriteria (char * dest, size_t destSz,
                                const char * what,
                                const char * typesString,
                                const long int composition,
                                const CDate * pHighRange,
                                const CDate * pLowRange = NULL,
                                const char * resultsString = NULL,
                                const char * requestKindsString = NULL,
                                const char * ageString = NULL,
                                const bool finishedTest = true,
                                const char * sourceTypesString = NULL,
                                const char * grantedsString = NULL,
                                const char * sessKindsString = NULL,
                                const bool sKMatch = false,
                                const bool excludeCreatedsLater = false,
                                const bool rSMatch = true,
                                const char * const decResultsString = NULL,
                                const char * const lawKindsString = NULL,
                                const bool lKSMatch = true,
                       const senderChecksActions aboutTheSender = IgnoreSender,
                       const bool requestCurrentlyQuick = false, // 24.sep.2003
                       const bool corr_crit = false,     // 23    // 20070607
                       const bool decResMatch = true,    // 24    // 20070810
                       const bool sessDate = false,      // 25    // 20070829
                       const bool sessBeforeFin = true   // 26    // 20070829
                               ) ;
// 20070119
bool composeReportLawsConnectCriteria (char * dest, size_t destSz,
                                const char * what,
                                const char * typesString,
                                const long int composition,
                                const CDate * pHighRange,
                                const CDate * pLowRange = NULL,
                                const char * sourceTypesString = NULL,
                                const char * const lawKindsString = NULL,
                                const bool lKSMatch = true,
                                const char * connectTypesString = NULL,
                                const bool lConnSMatch  = true
                               ) ;

// 20080625 ->
bool composeReportJudgementCriteria (char * dest, size_t destSz,
                                 const char * what,
                                 const char * lawTypesString,
                                 const long int composition,
                                 const CDate * pLowRange,
                                 const CDate * pHighRange,

                                 const char * judgementKinds,
                                 const char JUDGEMENT_X
                                ) ;

bool composeReportDecisionCriteria (char * dest, size_t destSz,
                                 const char * what,
                                 const char * lawTypesString,
                                 const long int composition,
                                 const CDate * pLowRange,
                                 const CDate * pHighRange,

                                 const char * decisionKinds
                                ) ;

// 20080625 <-

bool composeReportSidesCriteria (char * dest, size_t destSz,
                                 const char * what,
                                 const char * typesString,
                                 long int composition,
                                 const CDate * pHighRange,
                                 const CDate * pLowRange = NULL,
                                 const char * resultsString = NULL,
                                 const char * judgedString = NULL,
                                 const char * judgementKinds = NULL,
                                 const char * oldString = NULL,
                                 const char * punishmentString = NULL,
                                 const char * recidiveString = NULL,
                                 const char * lawKindString = NULL,
                                 const char * const freedStr = NULL,
                              const arrayOfLong * const pFlagVArr = NULL,
                    const char * const lawPS2Str = NULL,  // 11.jan.2001
                    const bool corr_crit = false          // 20070611
                                ) ;
bool composeReportDefendantsCriteria (char * dest, size_t destSz,
                                      const char * what,
                                      const char * typesString,
                                      const long int composition,
                                      const CDate * pHighRange,
                                      const CDate * pLowRange,
                                      const bool corr_crit = false ) ;  // 20070611
bool composeReportSessionsCriteria (char * dest, size_t destSz,
                                    const char * what,
                                    const char * typesString,
                                    const long int composition,
                                    const CDate * pHighRange,
                                    const CDate * pLowRange = NULL,
                                    const char * resultsString = NULL,
                                    const char * sessKindsString = NULL,
                                    const bool sKSMatch = false,
                      const bool excludeClosedSess = true,  // 05.may.2001
                      const bool beforeLawFinish = true  // 25.may.2001
                                   ) ;
bool composeRSCriteria (char * dest, size_t destSz, const char * what,
                        const char * resultsString,
                        const collCompBegDEndDBegN * cCBDEDBNPtr,
                        const char * ageString = NULL,
                        const char * pGrantChar = NULL,
                        const char * sessKindStr = NULL,
                        bool sksEq = true,
                        const char * lawKindStr = NULL,
                        const bool followSessDate = true,
                        const bool includingClosedSessions = false
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
				, const bool observeInstLaws = true     // 20070907
#endif  // ADMIN || AREAL || MILITARY
			     ) ;
# if INSTANCE
bool composeReportLawsCriteria2 (char * dest, size_t destSz,
                                 const bool forMinistry,  // 21.jun.2000
                                 const char * what,
                                 const char * typesString,
                                 const long int composition,
                                 const CDate * pHighRange,
                                 const CDate * pLowRange = NULL,
                                 const char * requestKindsString = NULL,
                                 const char * globJudgemResultString =
                                                                   NULL,
                                 const char * decisionResultString =
                                                                   NULL,
                                 const char * sessResultString = NULL,
                                 const bool finishedTest = true,
                                 const bool excludeCreatedsLater = false,
                                 const bool rKSMatch = true, //25.feb.2000
//, const long int subjectLo = 0,  // 31.oct.2000, 08.nov.2000 back again
//  const long int subjectHi = 0,  // 31.oct.2000, 08.nov.2000 back again
//  const bool subjInRange = false  // 31.oct.2000, 08.nov.2000 back again
              const char * const regionActKindsStr = NULL,  // 12.sep.2001
                                 const bool rgnAKSMatch = true,//12.sep.2001
                                 const bool dKSMatch = true,            // 20060414

                                 const char * sessKindsString = NULL,   // 20070829
                                 const bool sKMatch = false,            // 20070829
                                 const bool sessDate = false,           // 20070829
                                 const bool sessBeforeFin = true,       // 20070829
                                 const char * ageString = NULL          // 20070914
                                ) ;
# else
bool composePuniAdmLawsCriteria (char * dest, size_t destSz,
                                 const char * what,
                                 const long int composition,
                                 const CDate highRange,
                                 const CDate lowRange,
                           const char * const decisResultString = NULL) ;
# endif
bool composeClosedSessBSCriteria (char * criteria,
                                  const size_t criteriaSz,
                                  const char * what,
                                const collBegDEndDBegN & cBegDEndDBegN,
                                  const char * resultsString = NULL,
                                  const char * decisKindsString = NULL
# if INSTANCE
                                  , int instns = 0
# endif
                                                     ,
                                  const bool followSessDate = true,
                                  const char * const lawSourceKindsString
                                                         = NULL,
                                  const bool resStrMatch = true
                                 ) ;
bool composeJudgeCheckupCriteria (char * const dest, size_t destSz,
                           const char * const what,
                           const char * const typesString,
                           const long int composition,
                           const CDate * const pHighRange,
                           const CDate * const pLowRange = NULL,
                           const char * const resultsString = NULL,
                           const char * const requestKindsString = NULL,
                           const char * const ageString = NULL,
                           const bool finishedTest = true,
                           const char * const sourceTypesString = NULL,
                           const char * const grantedsString = NULL,
                           const char * const sessKindsString = NULL,
                           const bool sKMatch = false,
                           const bool excludeCreatedsLater = false,
                           const bool rSMatch = true,
                           const bool sessDateTest = false,
                           const bool bindSessDateAndFinished = true,
                           const char * const decisResultsString = NULL,
                     const bool withRequestKindsString = true,
          const char * const lawKindsString = NULL,  // 08.apr.2002 - begin
                           const bool lKMatch = true,  // 08.apr.2002 - end
                    const begMYEndMYTypeCIU* pBMYEMYTCIU = NULL   // 20.jul.2004
                                 ) ;
bool composeJdgChkupFixedsCriteria (char * const dest, size_t destSz,
                                    const SessionJudgesNumber
                                              howManyJudges,
# if INSTANCE
                                    const bool firstInstLaws,
# endif
                                    const char * const typesString,
                                    const long int composition,
                                    const CDate highRange,
                                    const CDate lowRange,
                               const char * const sessKindsString = NULL,
                                    const bool sKMatch = false,
                             const char * const sessResultsString = NULL,
                    const begMYEndMYTypeCIU* pBMYEMYTCIU = NULL   // 20.jul.2004
                                   ) ;
bool composeJdgChkupAfterFinCriteria (char * const dest,
                                      const size_t destSz,
                                      const SessionJudgesNumber
                                        howManyJudges,
                                      const char * const typesString,
                                      const long int composition,
                                      const CDate highRange,
                                      const CDate lowRange,
                               const char * const sessKindsString = NULL,
                                      const bool sKMatch = false,
                    const bool conformWithSessDate = true,  // 25.jul.2000
                    const bool exscludeResDecree = true,  // 25.jul.2000
                              const char * const decisKindsString = NULL,
                    const char * const sessResStr = NULL,  // 01.nov.2000
                    const bool sRSMatch = false  // 01.nov.2000
                                      ) ;
bool composeFinishWithDecJudgDef_Crit (char * const dest,
                                       const size_t destSz,
                               const collCompBegDEndDBegN * cCBEDBNPtr,
                               const char * const decGJkindsStr = NULL,
                               const char * const lawKindsStr = NULL,
                               const char * const sourceKindStr = NULL) ;


bool composeSessionsByResultsCriteria (char * const crit,
                                       const size_t critSz,
                                   const collCompBegDEndDBegN & cCBDEDBN,
                                       const char * const resultsStr,
 const char * const what = DEFAULT_BRIEF_SESSION_FIELDS  // 05.jun.2001
                                      ) ;
bool composeAbolishedProgressCriteria (char * const crit,
                                       const size_t critSz,
                                  const collCompBegDEndDBegN & cCBDEDBN,
 const char * const what = DEFAULT_BRIEF_SESSION_FIELDS  // 05.jun.2001
                                      ) ;
bool compose_desk_book_criteria (char * const crit,
                                 const size_t critSz,
                                 const char college,
                                 const begNEndNYear & bneny) ;
bool appealed_laws_basic_criteria (ostrstream & oss,
                                   const char * const types,
                                   const CDate begDate,
                                   const CDate endDate,
                                   const char * const what,
                  const bool includingAppealedInRegEvents  // 10.apr.2001
                                  ) ;  // 02.dec.2000
// 20070208 ->
bool appealed_laws_basic_with_composition
                                 (ostrstream & oss,
                                   const char * const types,
                                   const CDate begDate,
                                   const CDate endDate,
                                   const long int composition,
                                   const char * const what,
                  const bool includingAppealedInRegEvents
                                  ) ;  // 20070208


bool composeABCCriteria (const begNLEndNLYear & bnlenly,
                         const char college,
                         char * criteria, const size_t critSz,
                         const char * //lastName
                         , const char * table) ;
bool request_by_lawKinds_part_crit (ostream & os,
                         const char * const lawKinds,
                         const char * const pref) ;  // 10.apr.2001

# endif  // _COMPOS_H
//

