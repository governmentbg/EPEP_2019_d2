#include "cac.h"
                     
void begNEndNYear :: clear ()
 {
  begN = endN = year = 0 ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
  want1stInstLaws = want2ndInstLaws = false ;
#   endif  // of APPEAL
# endif  // of INSTANCE
  jan2005Format  = splitIn2 = false ;  // 10.feb.2005

  selectCrit1 = true; // 2008619 ->
  begDate.Clear() ;
  endDate.Clear() ;
                      // 20080619 <-
 }  // begNEndNYear :: clear

begNEndNYear :: begNEndNYear (const begNEndNYear & source)  // 01.nov.2001
 {
  begN = source.begN ;
  endN = source.endN ;
  year = source.year ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
  want1stInstLaws = source.want1stInstLaws ;
  want2ndInstLaws = source.want2ndInstLaws ;
#   endif  // of APPEAL
# endif  // of INSTANCE
  jan2005Format = source.jan2005Format;
  splitIn2 = source.splitIn2;

  selectCrit1 = source.selectCrit1; // 20080619 ->
  begDate = source.begDate ;
  endDate = source.endDate ;        // 20080619 <-


 }  //begNEndNYear :: begNEndNYear

// 16.oct.2000 - begin
char begNEndNYear :: wantXInstLawsToChar () const
 {
# if INSTANCE
#   if APPEAL
  return 'l' ;
#   else  // APPEAL
  if (want1stInstLaws == want2ndInstLaws)
    return 'l' ;
  else
    if (want1stInstLaws)
      return '1' ;
    else
      return '2' ;
#   endif  // APPEAL
# else // INSTANCE
  return 'l' ;
# endif  // INSTANCE
 } // begNEndNYear :: wantXInstLawsToChar
// 16.oct.2000 - end

void begNLEndNLYear :: clear ()
 {  // 01.nov.2001
  begNEndNYear :: clear () ;// 20080605 begNEndNYear_DB :: clear () ;
  begL = endL = '\0' ;
  wantCheckup = false ;
  wantBrokenPrint = false ;
  wantPrintByPages = false ;  // 01.nov.2001 (was missing)
 }  // begNLEndNLYear :: clear

begNLEndNLYear :: begNLEndNLYear () : begNEndNYear ()  // 20080605  begNEndNYear_DB ()
 {
  clear () ;  // 01.nov.2001 instead of inline text
 }  // begNLEndNLYear :: begNLEndNLYear

begNLEndNLYear :: begNLEndNLYear (const begNLEndNLYear & source) :
                      begNEndNYear (source)  // 20080605 begNEndNYear_DB (source)
 {  // 01.nov.2001
  begL = source.begL ;
  endL = source.endL ;
  wantCheckup = source.wantCheckup ;
  wantBrokenPrint = source.wantBrokenPrint ;
  wantPrintByPages = source.wantPrintByPages ;
 }  //  begNLEndNLYear :: begNLEndNLYear

void begNEndNYearTypes :: clear ()
 {
  begNEndNYear :: clear () ;
  setmem (types, sizeof (types), '\0') ;
 }  // begNEndNYearTypes :: clear

begNEndNYearTypes :: begNEndNYearTypes () : begNEndNYear ()
 {
  clear () ;
 }  // begNEndNYearTypes :: begNEndNYearTypes

begNEndNYearTypes :: begNEndNYearTypes (const begNEndNYearTypes & source) :
			     begNEndNYear (source)
 {  // 01.nov.2001
  memmove (types, source.types, sizeof (types)) ;
 }  // begNEndNYearTypes :: begNEndNYearTypes

void begNEndNYearTypes::method()
{
}

void begNEndNYearTypesAC :: clear ()
 {
  begNEndNYearTypes :: clear () ;
  allColumns = false ;
  begDate.Clear() ;
  endDate.Clear() ;
  kinds [0] = '\0' ;  // 08.feb.2003
  automaticDistribution = false ;  // 05.mar.2003
  wantCheckup = false;  // 23.may.2003
  begSerialNo = 0 ;  // 28.sep.2000
 }  // begNEndNYearTypesAC :: clear

begNEndNYearTypesAC :: begNEndNYearTypesAC () : begNEndNYearTypes ()
 {
  clear () ;
 } ;  // struct begNEndNYearTypes

begNEndNYearTypesAC ::
  begNEndNYearTypesAC (const begNEndNYearTypesAC & source) :
    begNEndNYearTypes (source)
 { // 01.nov.2001
  allColumns = source.allColumns ;
  begDate = source.begDate ;
  endDate = source.endDate ;
  begSerialNo = source.begSerialNo ;
  ostrstream (kinds, sizeof(kinds)) << source.kinds << ends ;  // 08.feb.2003
  ENDING_ZERO (kinds) ;  // 08.feb.2003
  automaticDistribution = source.automaticDistribution ;  // 05.mar.2003
  wantCheckup = source.wantCheckup ;  // 27.may.2003
 }  // begNEndNYearTypesAC :: begNEndNYearTypesAC

const begNEndNYearTypes & operator << (begNEndNYearTypes & const dest,
                                       const begNEndNYear & const src)
 {
  dest.clear () ;
  dest.begN = src.begN ;
  dest.endN = src.endN ;
  dest.year = src.year ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
  dest.want1stInstLaws = src.want1stInstLaws ;
  dest.want2ndInstLaws = src.want2ndInstLaws ;
#   endif  // APPEAL
# endif  // INSTANCE
  return dest ;
 }  // const begNEndNYearTypes &
    //   operator << (begNEndNYearTypes & const dest,
    //                const begNEndNYear & const src)

void collCompBegDEndDBegN :: clear ()
 {
  setmem (colleges, sizeof (colleges), '\0') ;
  composit = begN = 0 ;
  setmem (& begD, sizeof (begD), '\0') ;
  setmem (& endD, sizeof (endD), '\0') ;
# if INSTANCE
  firstInstLaws = false ;
  puniAdmConcernLaws = false ;
  forBoss = false ;
#   if APPEAL
  keepingMethodTimeBook = false ;
#   else  // of APPEAL
  all2ndPuniLaws = false ;  // 02.jan.2002
#   endif  // of APPEAL
# endif  // of INSTANCE
  wantClosedSessions = false ;
  wantClSessAllResults = false ;
  extraVal0 = 0 ;
  extraVal1 = 0 ;
  extraVal2 = 0 ;
  extraVal3 = 0 ;
  extraVal4 = 0 ;
  extraVal5 = 0 ;
  extraVal6 = 0 ;
  extraVal7 = 0 ;  // 04.may.2000
  heapingStartDate.Clear() ;
  wantBook = false ;
  wantSummary = false ;
  wantJudgesSummary = false ;
  wantResultsCheckup = false ;  // 20.sep.2000
  wantSortByDateReturned = false ;
# if INSTANCE
	whichPartOfDay = WholeDay ;
	noon.hour = noon.minute = noon.second = '\0' ;
# endif
  begNo = endNo = year = 0 ;  // 11.sep.2000
  finLawsCheckTerminated = false ;  // 01.nov.2002 - begin
  finLawsInconsistent = false ;
  sessCheckTerminated = false ;
  sessInconsistent = false ;  // 01.nov.2002 - end
  jan2005Format = false ;  // 08.feb.2005
  splitIn2 = false ;  // 08.feb.2005
  orderBy1 = true ; // 20051206
  orderBy2 = true;
  if (pArrOfLong)
    pArrOfLong -> Clear () ;
 }  // collCompBegDEndDBegN :: clear

collCompBegDEndDBegN :: collCompBegDEndDBegN ()
 {
  pArrOfLong = new arrayOfLong ;  // 25.may.2000
  clear () ;
 }  // collCompBegDEndDBegN :: collCompBegDEndDBegN

collCompBegDEndDBegN ::
  collCompBegDEndDBegN (const collCompBegDEndDBegN & src)
 {
  pArrOfLong = new arrayOfLong ;
  (* this) = src ;
 }  // collCompBegDEndDBegN ::
    //   collCompBegDEndDBegN (const collCompBegDEndDBegN src)
    
collCompBegDEndDBegN :: ~collCompBegDEndDBegN ()
 {
  if (pArrOfLong)
    delete pArrOfLong ;
 }  // collCompBegDEndDBegN :: ~collCompBegDEndDBegN

const collCompBegDEndDBegN & collCompBegDEndDBegN :: operator =
    (const collCompBegDEndDBegN & source)
 {
  clear () ;
  scSprintf (sizeof (colleges),colleges,
             "%.*s", sizeof (colleges) - 1, source.colleges) ;
  composit = source.composit ;
  begD = source.begD ;
  endD = source.endD ;
  begN = source.begN ;
# if INSTANCE
  firstInstLaws = source.firstInstLaws ;
  forBoss = source.forBoss ;
  puniAdmConcernLaws = source.puniAdmConcernLaws ;
#   if APPEAL
  keepingMethodTimeBook = source.keepingMethodTimeBook ;
#   else // of APPEAL
  all2ndPuniLaws = source.all2ndPuniLaws ;  // 02.jan.2002
#   endif  // of APPEAL
# endif  // of INSTANCE
  wantClosedSessions = source.wantClosedSessions ;
  wantClSessAllResults = source.wantClSessAllResults ;
  extraVal0 = source.extraVal0 ;
  extraVal1 = source.extraVal1 ;
  extraVal2 = source.extraVal2 ;
  extraVal3 = source.extraVal3 ;
  extraVal4 = source.extraVal4 ;
  extraVal5 = source.extraVal5 ;
  extraVal6 = source.extraVal6 ;
  extraVal7 = source.extraVal7 ;
  heapingStartDate = source.heapingStartDate ;
  wantBook = source.wantBook ;
  wantSummary = source.wantSummary ;
  wantJudgesSummary = source.wantJudgesSummary ;
  wantResultsCheckup = source.wantResultsCheckup ;  // 20.sep.2000
  wantSortByDateReturned = source.wantSortByDateReturned ;
# if INSTANCE
  whichPartOfDay = source.whichPartOfDay ;
  noon = source.noon ;
# endif
  finLawsCheckTerminated = source.finLawsCheckTerminated ; //01.nov.2002-begin
  finLawsInconsistent = source.finLawsInconsistent ;
  sessCheckTerminated = source.sessCheckTerminated ;
  sessInconsistent = source.sessInconsistent ;  // 01.nov.2002 - end
  jan2005Format = source.jan2005Format ;  // 08.feb.2005
  splitIn2 = source.splitIn2 ;  // 08.feb.2005
  orderBy1 = source.orderBy1 ;  // 20051206
  orderBy2 = source.orderBy2;
  if (pArrOfLong)
   {
    pArrOfLong -> Clear () ;

    const arrayOfLong * const pSrcArr = source.lArrPtr () ;

    if (pSrcArr)
	for (int i = 0 ; i < pSrcArr -> Count () ; i ++)
        addToArr ((* pSrcArr) [i]) ;
   }

  return (* this) ;
 }  // const collCompBegDEndDBegN & collCompBegDEndDBegN :: operator =
    //    (const collCompBegDEndDBegN & source)
    
void collCompBegDEndDBegN :: addToArr (const long int l)
 {
  if (pArrOfLong)
    pArrOfLong -> Add (l) ;
 }  // collCompBegDEndDBegN :: addToArr

const arrayOfLong * collCompBegDEndDBegN :: lArrPtr () const
 {
  return  pArrOfLong ;
 }  // collCompBegDEndDBegN :: lArrPtr

arrayOfLong * collCompBegDEndDBegN :: lArrPtr_NC () const
 {
  return  pArrOfLong ;
 }  // collCompBegDEndDBegN :: lArrPtr_NC

const collCompBegDEndDBegN & collCompBegDEndDBegN :: operator <<
                                          (const begMYEndMYTypeCIU r)
 {
  clear () ;
  ostrstream (colleges, sizeof (colleges)) << r.types << ends ;
  colleges [sizeof (colleges) - 1] = '\0' ;
  if (strlen (colleges) < strlen (r.types))
    error ("%s", TYPES_BUFF_TOO_SHORT) ;  // This
    // normally will never be displayed  // message --> error  30.mar.2001
  composit = r.composition ;
   {
    CDate bP, eP ;

    bP.day = eP.day = (char) 1 ;
    bP.month = r.begMonth ;
    bP.year = r.begYear ;
    eP.month = r.endMonth ;
    eP.year = r.endYear ;
    begD = firstDayOfThisMonth (bP) ;
    endD = lastDayOfThisMonth (eP) ;
   }
# if INSTANCE
  firstInstLaws = r.firstInstLaws ;
	forBoss = r.forBoss ;
	whichPartOfDay = r.whichPartOfDay ;  // 04.jun.2002
	noon = r.noon ;                      // 04.jun.2002
#   if APPEAL  // 25.may.2000 - begin
	keepingMethodTimeBook = r.keepingMethodTimeBook ;
#   endif  // 25.may.2000 - end
# endif
  finLawsCheckTerminated = r.finLawsCheckTerminated ;  // 01.nov.2002 - begin
  finLawsInconsistent = r.finLawsInconsistent ;
  sessCheckTerminated = r.sessCheckTerminated ;
  sessInconsistent = r.sessInconsistent ;  // 01.nov.2002 - end
  if (pArrOfLong)
   {
    pArrOfLong -> Clear () ;

    const arrayOfLong * const pSrcArr = r.lArrPtr () ;

    if (pSrcArr)
	for (int i = 0 ; i < pSrcArr -> Count () ; i ++)
        addToArr ((* pSrcArr) [i]) ;
   }
  return (* this) ;
 }  // collCompBegDEndDBegN :: operator <<

const collCompBegDEndDBegN & collCompBegDEndDBegN :: operator <<
                                          (const collBegDEndDBegN c)
 {
  clear () ;
  ostrstream (colleges, sizeof (colleges)) << c.college << ends ;
  colleges [sizeof (colleges) - 1] = '\0' ;
  composit = 0 ;
  begD = c.begD ;
  endD = c.endD ;
  begN = c.begN ;
  wantClSessAllResults = c.wantAllResults ;
// # if INSTANCE  // 19.apr.2000
  wantSortByDateReturned = c.wantSortByDateReturned ;
// # endif  // 19.apr.2000
  composit = c.composition ;  // 24.jul.2000
  wantResultsCheckup = c.wantResultsCheckup ;  // 21.sep.2000
  jan2005Format = c.jan2005Format ;  // 08.feb.2005
  return (* this) ;
 }  // collCompBegDEndDBegN :: operator <<

void begMYEndMYTypeCIU :: clearData ()
 {
  begDay = '\0' ;
  endDay = '\0' ;
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  puniAdmConcernLaws = false ;
	whichPartOfDay = WholeDay ;  // 04.jun.2002
	noon.hour = noon.minute = noon.second = '\0' ; // 04.jun.2002
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  all2ndPuniLaws = false ;
  incomingExtend = false;      // 20060607
#endif  // ADMIN || AREAL || MILITARY   // 30.jan.2002 - end
#if COURT_TYPE == COURT_REGIONAL
  internalUseCombined = false ;
#endif  // REGIONAL
// ---------------------- ALL courts -------------------------------------------
  begMonth = '\0' ;
  begYear = 0 ;
  endMonth = '\0' ;
  endYear = 0 ;
  setmem (types, sizeof (types), '\0') ;
  internalUse = false ;
  composition = 0 ;

# if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  firstInstLaws = false ;
  secondInstLaws = false;            // 20060609
  forBoss = false ;
	decidedLawsJudgeCheckup = false ;  // 12.apr.2001
	sessionsDistribution = false ;     // 04.jun.2002
#endif  // APPEAL || ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_APPEAL
  keepingMethodTimeBook = false ;
#endif  // APPEAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  forCorrupt = false;                // 20070604
#endif  // AREAL || MILITARY
# if COURT_TYPE == COURT_ADMIN
  forVSS = false;                    // 20070809
#endif  // ADMIN
#if COURT_TYPE == COURT_REGIONAL
  admLowSubjRange = 0 ;
  admHighSubjRange = 0 ;
#endif  // REGIONAL
// ---------------------- ALL courts -------------------------------------------
  finLawsCheckTerminated = false ;  // 01.nov.2002 - begin
  finLawsInconsistent = false ;
  sessCheckTerminated = false ;
  sessInconsistent = false ;  // 01.nov.2002 - end
  if (pArrOfLong)
    pArrOfLong -> Clear () ;
  variant = JUDGE_REPORT_NULL;
 }  // begMYEndMYTypeCIU :: clearData



begMYEndMYTypeCIU :: begMYEndMYTypeCIU ()
 {
//# if ! INSTANCE   // 16.feb.2000  // 25.may.2000 commented
  pArrOfLong = new arrayOfLong ;
//# endif  // 25.may.2000 commented
  clearData () ;
 }  // begMYEndMYTypeCIU :: begMYEndMYTypeCIU

begMYEndMYTypeCIU ::
  begMYEndMYTypeCIU (const begMYEndMYTypeCIU & const source)
 {
  pArrOfLong = new arrayOfLong ;
  (* this) = source ;
 }  // begMYEndMYTypeCIU :: begMYEndMYTypeCIU

begMYEndMYTypeCIU :: ~begMYEndMYTypeCIU ()
 {
  if (pArrOfLong)
    delete pArrOfLong ;
 }  // begMYEndMYTypeCIU :: ~begMYEndMYTypeCIU

const begMYEndMYTypeCIU &
  begMYEndMYTypeCIU :: operator = (const begMYEndMYTypeCIU & const source)
 {
  begDay = source.begDay ;
  endDay = source.endDay ;
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  puniAdmConcernLaws  = source.puniAdmConcernLaws ;
	whichPartOfDay = source.whichPartOfDay ;    // 04.jun.2002
	noon = source.noon ;                        // 04.jun.2002
#endif  // APPEAL || ADMIN || AREAL || MILITARY
							    // 30.jan.2002 - begin
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY
  all2ndPuniLaws = source.all2ndPuniLaws ;
  incomingExtend = source.incomingExtend ;    // 20060607
#endif  // ADMIN || AREAL || MILITARY            // 30.jan.2002 - end

#if COURT_TYPE == COURT_REGIONAL
  internalUseCombined = source.internalUseCombined ;
#endif  // REGIONAL

// ----------------- ALL courts -------------------------------------------
  begMonth = source.begMonth ;
  begYear = source.begYear ;
  endMonth = source.endMonth ;
  endYear = source.endYear ;
  scSprintf (sizeof (types), types, "%s", source.types) ;
  internalUse = source.internalUse ;
  composition = source.composition ;

#if COURT_TYPE == COURT_REGIONAL
    admLowSubjRange = source.admLowSubjRange ;
    admHighSubjRange = source.admHighSubjRange ;
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  firstInstLaws = source.firstInstLaws ;
  secondInstLaws = source.secondInstLaws ;  // 20060609
  forBoss = source.forBoss ;
	decidedLawsJudgeCheckup = source.decidedLawsJudgeCheckup ; //12.apr.2001
	sessionsDistribution = source.sessionsDistribution ;  // 04.jun.2002
  forCorrupt = source.forCorrupt;           // 20070615
#endif  // AREAL || MILITARY

#if COURT_TYPE == COURT_ADMIN
  firstInstLaws = source.firstInstLaws ;
  secondInstLaws = source.secondInstLaws ;  // 20060609
  forBoss = source.forBoss ;
  forVSS  = source.forVSS ;                // 20070809
	decidedLawsJudgeCheckup = source.decidedLawsJudgeCheckup ; //12.apr.2001
	sessionsDistribution = source.sessionsDistribution ;  // 04.jun.2002
#endif  // ADMIN

#if COURT_TYPE == COURT_APPEAL
  firstInstLaws = source.firstInstLaws ;
  secondInstLaws = source.secondInstLaws ;  // 20060609
  forBoss = source.forBoss ;
	decidedLawsJudgeCheckup = source.decidedLawsJudgeCheckup ; //12.apr.2001
	sessionsDistribution = source.sessionsDistribution ;  // 04.jun.2002
  keepingMethodTimeBook = source.keepingMethodTimeBook ;
#endif  // APPEAL

  finLawsCheckTerminated = source.finLawsCheckTerminated ; //01.nov.2002-begin
  finLawsInconsistent = source.finLawsInconsistent ;
  sessCheckTerminated = source.sessCheckTerminated ;
  sessInconsistent = source.sessInconsistent ;  // 01.nov.2002 - end
  if (pArrOfLong)
   {
    pArrOfLong -> Clear () ;

    const arrayOfLong * const pSrcArr = source.lArrPtr () ;

    if (pSrcArr)
	for (int i = 0 ; i < pSrcArr -> Count () ; i ++)
        addToArr ((* pSrcArr) [i]) ;
   }
  return (* this) ;
 }  // const begMYEndMYTypeCIU & begMYEndMYTypeCIU ::
    //   operator = (const begMYEndMYTypeCIU & const source)

const arrayOfLong * begMYEndMYTypeCIU :: lArrPtr () const
 {
  return pArrOfLong ;
 }  // begMYEndMYTypeCIU :: lArrPtr

void begMYEndMYTypeCIU :: addToArr (const long int l)
 {
  if (pArrOfLong)
    pArrOfLong -> Add (l) ;
 }  // begMYEndMYTypeCIU :: addToArr

begMYEndMYTypeCIU ::
  begMYEndMYTypeCIU (const collCompBegDEndDBegN & rCCBDEDBN)
 {
  pArrOfLong = new arrayOfLong ;
  clearData () ;
  begDay = rCCBDEDBN.begD.day ;
  begMonth = rCCBDEDBN.begD.month ;
  begYear = rCCBDEDBN.begD.year ;
  endDay = rCCBDEDBN.endD.day ;
  endMonth = rCCBDEDBN.endD.month ;
  endYear = rCCBDEDBN.endD.year ;
  scSprintf (sizeof (types), types, "%s", rCCBDEDBN.colleges) ;
  composition = rCCBDEDBN.composit ;
# if INSTANCE
  puniAdmConcernLaws = rCCBDEDBN.puniAdmConcernLaws ;
  firstInstLaws = rCCBDEDBN.firstInstLaws ;
#   if APPEAL
  keepingMethodTimeBook = rCCBDEDBN.keepingMethodTimeBook ;
#   else  // of APPEAL  // 30.jan.2002 - begin
  all2ndPuniLaws = rCCBDEDBN.all2ndPuniLaws ;  // 30.jan.2002 - end
#   endif  // of APPEAL
  finLawsCheckTerminated =  // 01.nov.2002 - begin
    rCCBDEDBN.finLawsCheckTerminated ;
  finLawsInconsistent = rCCBDEDBN.finLawsInconsistent ;
  sessCheckTerminated = rCCBDEDBN.sessCheckTerminated ;
  sessInconsistent = rCCBDEDBN.sessInconsistent ;  // 01.nov.2002 - end
  if (pArrOfLong)
   {
    pArrOfLong -> Clear () ;

    const arrayOfLong * const pSrcArr = rCCBDEDBN.lArrPtr () ;

    if (pSrcArr)
	for (int i = 0 ; i < pSrcArr -> Count () ; i ++)
        addToArr ((* pSrcArr) [i]) ;
   }
# endif  // of INSTANCE
 }  // begMYEndMYTypeCIU ::
    //   begMYEndMYTypeCIU (const collCompBegDEndDBegN & rCCBDEDBN)

void begMYEndMYTypeCIU ::
       operator >> (collCompBegDEndDBegN & rCCBDEDBN) const
 {
  rCCBDEDBN.clear () ;
  ostrstream (rCCBDEDBN.colleges, sizeof (rCCBDEDBN.colleges))
    << types << ends ;
  rCCBDEDBN.colleges [sizeof (rCCBDEDBN.colleges) - 1] = '\0' ;
  if (strlen (rCCBDEDBN.colleges) < strlen (types))
    error ("%s", TYPES_BUFF_TOO_SHORT) ;  // This
    // normally will never be displayed  // message --> error  30.mar.2001
  rCCBDEDBN.composit = composition ;
# if INSTANCE
  rCCBDEDBN.firstInstLaws = firstInstLaws ;
  rCCBDEDBN.forBoss = forBoss ;
#   if APPEAL
  rCCBDEDBN.keepingMethodTimeBook = keepingMethodTimeBook ;
#   endif
# endif
  rCCBDEDBN.begD.day = begDay ;
  rCCBDEDBN.begD.month = begMonth ;
  rCCBDEDBN.begD.year = begYear ;
  rCCBDEDBN.endD.day = endDay ;
  rCCBDEDBN.endD.month = endMonth ;
  rCCBDEDBN.endD.year = endYear ;
# if INSTANCE
  rCCBDEDBN.puniAdmConcernLaws = puniAdmConcernLaws ;
#   if APPEAL  // 30.jan.2002 - begin
#   else  // of APPEAL
  rCCBDEDBN.all2ndPuniLaws = all2ndPuniLaws ;
#   endif  // of APPEAL  // 30.jan.2002 - end
# endif  // of INSTANCE
  rCCBDEDBN.finLawsCheckTerminated =  // 01.nov.2002 - begin
    finLawsCheckTerminated ;
  rCCBDEDBN.finLawsInconsistent = finLawsInconsistent ;
  rCCBDEDBN.sessCheckTerminated = sessCheckTerminated ;
  rCCBDEDBN.sessInconsistent = sessInconsistent ;  // 01.nov.2002 - end
   {
    arrayOfLong * const pDestArr = rCCBDEDBN.lArrPtr_NC () ;

    if (pDestArr)
     {
	pDestArr -> Clear () ;
      if (pArrOfLong)
	  for (int i = 0 ; i < pArrOfLong -> Count () ; i ++)
          rCCBDEDBN.addToArr ((* pArrOfLong) [i]) ;
     }
   }
 }  // void begMYEndMYTypeCIU ::
    //   operator >> (collCompBegDEndDBegN & rCCBDEDBN)

CDate begMYEndMYTypeCIU :: BegDate () const  // 08.jan.2001
 {
  return CDate (1, begMonth, begYear) ;
 }  // begMYEndMYTypeCIU :: BegDate

CDate begMYEndMYTypeCIU :: EndDate () const  // 08.jan.2001
 {
  return CDate (lastDayOfThisMonth (CDate (1, endMonth,
                                                endYear))) ;
 }  // begMYEndMYTypeCIU :: EndDate
 
collBegDEndDBegN :: collBegDEndDBegN ()
 {
  college = '\0' ;
  setmem (& begD, sizeof (begD), '\0') ;
  endD = begD ;
  begN = 0 ;
  wantAllResults = false ;
// # if INSTANCE  // 19.apr.2000
  wantSortByDateReturned = false ;
// # endif  // 19.apr.2000
  wantShortFormat = false ;  // 23.jun.2000
  composition = 0 ;  // 24.jul.2000
  wantResultsCheckup = false ;  // 21.sep.2000
  jan2005Format =  false ;  // 28.jan.2005
 }  // collBegDEndDBegN :: collBegDEndDBegN

const collBegDEndDBegN & collBegDEndDBegN :: operator <<
                                        (const collCompBegDEndDBegN t)
 {
  college = t.colleges [0] ;
  begD = t.begD ;
  endD = t.endD ;
  begN = t.begN ;
  wantAllResults = t.wantClSessAllResults ;
// # if INSTANCE  // 19.apr.2000
  wantSortByDateReturned = t.wantSortByDateReturned ;
// # endif  // 19.apr.2000
  wantShortFormat = false ;  // 23.jun.2000
  composition = t.composit ;  // 24.jul.2000
  wantResultsCheckup = t.wantResultsCheckup ;  // 21.sep.2000
  jan2005Format = t.jan2005Format ;  // 08.feb.2005
  return (* this) ;
 }  // collBegDEndDBegN :: operator <<

clgDate :: clgDate ()
 {
# if INSTANCE
#   if APPEAL  // 27.mar.2001
  college = '\0' ;
#   else  // of APPEAL  // 27.mar.2001 - begin
  lawKinds [0] = '\0' ;
#   endif  // of APPEAL  // 27.mar.2001 - end
# else  // of INSTANCE
  colleges [0] = '\0' ;
# endif  // of INSTANCE
  setmem (& date, sizeof (date), '\0') ;
  headerWithLawType = false ;
  composition = 0 ;
  wantTimeSort = false ;
  whichPartOfDay = WholeDay ;
  noon.hour = noon.minute = noon.second = '\0' ;
  variant0 = variant1 = variant2 = variant4 = false ;  // 27.mar.2001 - begin
  hall = 0 ;
  city [0] = '\0' ;  // 27.mar.2001 - end
  emptyRowsAfterEachTimeBlock = 0 ;  // 31.may.2001
 }  // clgDate :: clgDate

lawRCDKeyToSidePair::lawRCDKeyToSidePair(TLawsuit& lS,
  const bool includingAnonymous, const char* turnInvolvs,
  const char* const defInvsForExclude) :
  TRCDKeyToSidePair<TLawsuit, TSideWind>(lS, includingAnonymous,
    turnInvolvs, defInvsForExclude, true, &(lS.date))
{
  METHOD_NAME("lawRCDKeyToSidePair");

  if(DatedContainer_(lS.key, lS.kind, lS.date).FilledOK() ==false)
    REPORT_M_INVALID_ARGS;
}  // lawRCDKeyToSidePair :: lawRCDKeyToSidePair

reqRCDKeyToSidePair::reqRCDKeyToSidePair(TRequest& lS,
  const bool includingAnonymous, const char* turnInvolvs,
    const char* const defInvsForExclude) :
  TRCDKeyToSidePair<TRequest, TSideWind>(lS, includingAnonymous,
    turnInvolvs, defInvsForExclude, true,  &(lS.date))
{
  // TRCDKeyToSidePair::TRCDKeyToSidePair is smart enough and won't crash,
  // but it may yield wrong results.
  METHOD_NAME("reqRCDKeyToSidePair");

  if(DatedContainer_(lS.key, lS.kind, lS.date).FilledOK() ==false)
    REPORT_M_INVALID_ARGS;
}  // reqRCDKeyToSidePair :: reqRCDKeyToSidePair

SessRCDKeyToSidePair::SessRCDKeyToSidePair(TSession& lS,
  const CDate* const pDate, const bool includingAnonymous,
  const char* turnInvolvs, const char* const defInvsForExclude):
  TRCDKeyToSidePair<TSession, TSideWind>(lS, includingAnonymous,
    turnInvolvs, defInvsForExclude, true, pDate)
{
  // TRCDKeyToSidePair::TRCDKeyToSidePair is smart enough and won't crash,
  // but it may yield wrong results.
  METHOD_NAME("SessRCDKeyToSidePair");
  CDate dummy;

  if(pDate)
  {
    dummy = (*pDate);
  }
  if(DatedContainer_(lS.key, lS.kind, dummy).FilledOK() ==false)
    REPORT_M_INVALID_ARGS;
}  // SessRCDKeyToSidePair :: SessRCDKeyToSidePair

DecRCDKeyToSidePair::DecRCDKeyToSidePair(TDecision& lS,
  const CDate* const pDate, const bool includingAnonymous,
  const char* turnInvolvs, const char* const defInvsForExclude):
  TRCDKeyToSidePair<TDecision, TSideWind>(lS, includingAnonymous,
    turnInvolvs, defInvsForExclude, true, pDate)
{
  // TRCDKeyToSidePair::TRCDKeyToSidePair is smart enough and won't crash,
  // but it may yield wrong results.
  METHOD_NAME("DecRCDKeyToSidePair");
  CDate dummy;

  if(pDate)
  {
    dummy = (*pDate);
  }
  if(DatedContainer_(lS.key, lS.kind, dummy).FilledOK() ==false)
    REPORT_M_INVALID_ARGS;
}  // DecRCDKeyToSidePair :: DecRCDKeyToSidePair

bookAreas :: bookAreas ()
 {
  dataCipher = areaLen = from = 0 ;
  align = '\0' ;
 }  // bookAreas :: bookAreas

void bookAreas :: load (istream & is)
 {
  char buff [256] ;

  int a ;
  unsigned int dC, aL, f ;
  dC = aL = a = f = 0 ;
  is.getline (buff, sizeof (buff)) ;
  if (! is.good ())
    if (is.rdstate () != ios :: eofbit)
      buff [0] = '\0' ;
  if (endSection (buff))
    buff [0] = '\0' ;
  buff [sizeof (buff) - 1] = '\0' ;
   {
    char * semicolon = strchr (buff, ';') ;

    if (semicolon)
      * semicolon = '\0' ;
   }
   {  // -- 1 --
    istrstream iss (buff, strlen (buff)) ;

    iss >> dC ;
    if (iss.good ())
     {  // -- 2 --
      iss >> aL ;
      if (iss.good ())
       {  // -- 3 --
        iss >> a ;
        if (iss.good ())
         {  // -- 4 --
          iss >> f ;
          if (! iss.good ())
            f = 0 ;
         }  // -- 4 --
        else
          a = 0 ;
       }  // -- 3 --
      else
        aL = 0 ;
     }  // -- 2 --
    else
      dC = 0 ;
    dataCipher = dC ;
    areaLen = aL ;
    align = (unsigned char) a ;
    from = f ;
   }  // -- 1 --
 }  // bookAreas :: load

void TClaimant :: clearData ()
 {
  setmem (cName, sizeof (cName), '\0') ;
  setmem (cSettlement, sizeof (cSettlement), '\0') ;
  setmem (cAddress, sizeof (cAddress), '\0') ;
 }  // TClaimant :: clearData

TClaimant :: TClaimant ()
 {
  clearData () ;
 }  // TClaimant :: TClaimant

void TDefendant :: clearData ()
 {
  setmem (dName, sizeof (dName), '\0') ;
  setmem (dSettlement, sizeof (dSettlement), '\0') ;
  setmem (dAddress, sizeof (dAddress), '\0') ;
 }  // TDefendant :: clearData

TDefendant :: TDefendant ()
 {
  clearData () ;
 }  // TDefendant :: TDefendant

void bookFormats :: readAreas (ifstream & ifs,
                                bookAreas * bAPtr, int arCnt)
 {
  for (int i = 0 ; i < arCnt ; i ++)
    bAPtr [i].load (ifs) ;
 }  // TBookFormats :: readAreas

bool bookFormats :: loadFormat (const char * formatFN,
					  TWindow *
                                )
 {
  char tmpFFN [SIZE_OF_LINE] ;
  bool hasReference = false ;

  if (access (formatFN, 0))
   {
    if (strlen (formatFN) < sizeof (tmpFFN))
     {
      strcpy (tmpFFN, formatFN) ;
      if (! referenceToFmtFile (tmpFFN, sizeof (tmpFFN)))
        return false ;
      else
        hasReference = true ;
     }
    else
      return false ;
   }
   {
//    ifstream ifs (formatFN) ;
    ifstream ifs (hasReference ? tmpFFN : formatFN) ;
    bool err = true ;
    int issGood = 0 ;

    if (ifs)
     {  // -- 0 --
      char buff [256] ;
      bool sectionFound = false ;  // 16.oct.2001

	if (ALT_PATCH_COLUMNS)
	{
	  if (hasReference) sectionFound = findSection (ifs, tmpFFN, ALT_PATCH_COLUMNS_SECTION, false) ;
	  else sectionFound = findSection (ifs, formatFN, ALT_PATCH_COLUMNS_SECTION, false) ;
	}
	else  // 15.nov.2001 - begin
	   if (WINDOWS_PRINT)
	   {
	     if (hasReference) sectionFound = findSection (ifs, tmpFFN, WINDOWS_COLUMNS_SECTION, false) ;
	     else sectionFound = findSection (ifs, formatFN, WINDOWS_COLUMNS_SECTION, false) ;
	   }
	if (sectionFound == false)
	{
	   if (hasReference) sectionFound = findSection (ifs, tmpFFN, COLUMNS_SECTION) ;
         else  sectionFound = findSection (ifs, formatFN, COLUMNS_SECTION) ;
      }
//      if (findSection (parent, ifs, COLUMNS_SECTION))
      if (sectionFound)  // 16.oct.2001 - end
       {  // -- 0a --
        buff [0] = '\0' ;
        ifs.getline (buff, sizeof (buff)) ;
        if (ifs.good () && ! endSection (buff))
         {  // -- 1 --
           {
            istrstream iss (buff, sizeof (buff)) ;

            iss >> areasCnt >> areas1Cnt ;
            issGood = iss.good () ;
           }
          if (issGood)
           {  // -- 2 --
            pBA = new bookAreas [areasCnt] ;
            pBA1 = new bookAreas [areas1Cnt] ;
            if (pBA && pBA1)
             {  // -- 3 --
              readAreas (ifs, pBA, areasCnt) ;
               {  // skip one empty line
                char a [255] ;

                ifs.getline (a, sizeof (a)) ;
               }
              readAreas (ifs, pBA1, areas1Cnt) ;
             }  // -- 3 --
           }  // -- 2 --
         }  // -- 1 --
        err = ! (ifs.good () && issGood && pBA && pBA1) ;
       }  // -- 0a --
     }  // -- 0 --
    if (err)
     {
      areasCnt = areas1Cnt = 0 ;
	error(CANNOT_LOAD_THE_FORMAT_INFO) ;
     }
    return ! err ;
   }
 }  // bookFormats :: loadFormat

bookFormats :: bookFormats (const char * formatFN, TWindow * parent) :
                  pBA (NULL), pBA1 (NULL), areasCnt (0), areas1Cnt (0),
                  loadOK (false)
 {
  if (formatFN)
    loadOK = loadFormat (formatFN, parent) ;
 }  // bookFormats :: bookFormats

bookFormats :: ~bookFormats ()
 {
  if (pBA)
    delete [] pBA ;
  if (pBA1)
    delete [] pBA1 ;
 }  // bookFormats :: ~bookFormats

bool bookFormats :: formatLoadIsOK () const
 {
  return loadOK ;
 }  // bookFormats :: formaLoadIsOK

void bookFormats :: notConstOutput (ostream &, bool)
 {
 }  // bookFormats :: notConstOutput

const bookAreas * bookFormats :: getBkAreas () const
 {
  return (loadOK ? pBA : NULL) ;
 }  // bookFormats :: getBkAreas

int bookFormats :: getBkAreasCnt () const
 {
  return (loadOK ? areasCnt : 0) ;
 }  // bookFormats :: getBkAreasCnt

const bookAreas * bookFormats :: getBkAreas1 () const
 {
  return (loadOK ? pBA1 : NULL) ;
 }  // bookFormats :: getBkAreas1

int bookFormats :: getBkAreas1Cnt () const
 {
  return (loadOK ? areas1Cnt : 0) ;
 }  // bookFormats :: getBkAreas1Cnt

void TBookLawsuit :: clearData ()
 {
  clearLawsuitData (* this) ;
  setmem (& claimant, sizeof (claimant), '\0') ;
  setmem (& defendant, sizeof (defendant), '\0') ;
 }  // TBookLawsuit :: clearData


TBookLawsuit :: TBookLawsuit (const char * formatFN, TWindow * parent) :
                  bookFormats (formatFN, parent),
                  TLawsuit ()//, pBA (NULL), pBA1 (NULL),
//                  areasCnt (0), areas1Cnt (0), loadOK (false)
 {
  Add(
  new TStringAlias ("F_CLAIMANT_NAME",
			  claimant.cName, SIZE_OF_NAME_FOR_BOOK),
  new TStringAlias ("F_CLAIMANT_ADDRESS",
			  claimant.cAddress, SIZE_OF_ADDRESS_FOR_BOOK),
  new TStringAlias ("F_DEFENDANT_NAME",
			  defendant.dName, SIZE_OF_NAME_FOR_BOOK),
  new TStringAlias ("F_DEFENDANT_ADDRESS",
			  defendant.dAddress, SIZE_OF_ADDRESS_FOR_BOOK),
  NULL) ;
//  loadOK = loadFormat (formatFN, parentHWnd) ;
  clearData () ;
 }  // TBookLawsuit :: TBookLawsuit

// ciphers of the TBookLawsuit data members - begin
//   0 : nothing (empty string)     string  S
// 255 : nothing (new line)         string  S
// 254 : nothing (".")              string  S
// 253 : nothing ("|")              string  S
//  27 : claimant.cName             string  S
//  28 : claimant.cAddress          string  S
//  29 : defendant.dName            string  S
//  30 : defendant.dAddress         string  S
// ciphers of the TBookLawsuit data members - end

void TBookLawsuit :: output (ostream & os, bool first) const
 {
  const bookAreas * areasPtr = first ? pBA : pBA1 ;
  const int aCnt = first ? areasCnt : areas1Cnt ; 
  int i ;

  if (! loadOK)
    return ;
  for (i = 0 ; i < aCnt && os.good () ; i ++)
   {  // -- 0 --
    const size_t len = areasPtr [i].areaLen + areasPtr [i].from ;
    char * s = new char [len + 1] ;
    const int algn = areasPtr [i].align ;
    const size_t skip = areasPtr [i].from ;

    if (! (lawsuitOutput (* this, os, s, len, algn, skip,
                          areasPtr [i].dataCipher) ||
           miscOutput (os, s, len, algn, skip,
                       areasPtr [i].dataCipher)))
      switch (((long int) (areasPtr [i].dataCipher)) -
              OFFSET_LAWSUIT_CIPHER)
       {  // -- 1 --
//        case 0 :
//          writeString (os, "", len) ;
//          break ;
//        case 255 :
//          writeString (os, "\n", len, algn) ;
//          break ;
//        case 254 :
//          writeString (os, ".", len, algn) ;
//          break ;
//        case 253 :
//          writeString (os, "|", len, algn) ;
//          break ;
  //      case 1 :
  //        writeLong (os, key.no, len, algn) ;
  //        break ;
  //      case 2 :
  //        writeLong (os, key.year, len, algn) ;
  //        break ;
  //      case 3 :
  //        typeToString (key.type, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 4 :
  //        writeLong (os, source.no, len, algn) ;
  //        break ;
  //      case 5 :
  //        writeLong (os, source.year, len, algn) ;
  //        break ;
  //      case 6 :
  //        typeToString (source.type, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 7 :
  //        writeULong (os, date.day, len, algn) ;
  //        break ;
  //      case 8 :
  //        writeULong (os, date.month, len, algn) ;
  //        break ;
  //      case 9 :
  //        writeLong (os, date.year, len, algn) ;
  //        break ;
  //      case 10 :
  //        subjectToString (key.type, subject, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 11 :
  //        compositionToString (composition, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 12 :
  //        writeULong (os, age, len, algn) ;
  //        break ;
  //      case 13 :
  //        judgeUCNToName (judge, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 14 :
  //        writeULong (os, finished.day, len, algn) ;
  //        break ;
  //      case 15 :
  //        writeULong (os, finished.month, len, algn) ;
  //        break ;
  //      case 16 :
  //        writeLong (os, finished.year, len, algn) ;
  //        break ;
  //      case 17 :
  //        writeString (os, "", len) ;
  //        break ;
  //      case 18 :
  //        senderUCNToName (judge, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 19 :
  //        writeLong (os, returned.no, len, algn) ;
  //        break ;
  //      case 20 :
  //        writeLong (os, returned.year, len, algn) ;
  //        break ;
  //      case 21 :
  //        typeToString (returned.type, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 22 :
  //        writeULong (os, decMade.day, len, algn) ;
  //        break ;
  //      case 23 :
  //        writeULong (os, decMade.month, len, algn) ;
  //        break ;
  //      case 24 :
  //        writeLong (os, decMade.year, len, algn) ;
  //        break ;
  //      case 25 :
  //        kindToString (kind, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 26 :
  //        collegeToString (college, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
        case 27 :
          writeString (os, claimant.cName, len, algn, skip) ;
          break ;
        case 28 :
          writeString (os, claimant.cAddress, len, algn, skip) ;
          break ;
        case 29 :
          writeString (os, defendant.dName, len, algn, skip) ;
          break ;
        case 30 :
          writeString (os, defendant.dAddress, len, algn, skip) ;
          break ;
  //      case 31 :
  //        CDateToString (date, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 32 :
  //        CDateToString (decMade, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
  //      case 33 :
  //        kindToStringAbbr (kind, s, len + 1) ;
  //        writeString (os, s, len, algn, skip) ;
  //        break ;
        default :
          if (len)
            strcpy (s, "?") ;
          s [len] = '\0' ;
          writeString (os, s, len, algn, skip) ;
          break ;
       }  // -- 1 --      (switch)
    delete [] s ;
   }  // -- 0 --   (for)
 }  // TBookLawsuit :: output

bkFormatsPseudoOutput :: bkFormatsPseudoOutput (const char * formatFN,
                                                TWindow * parent) :
                           bookFormats (formatFN, parent)
 {
 }  // bkFormatsPseudoOutput :: bkFormatsPseudoOutput

void bkFormatsPseudoOutput :: output (ostream &, bool) const
 {
 }  // bkFormatsPseudoOutput :: output

# if INSTANCE
// 2011:171 LPR: for _ book/report columns
struct TColumnResult
{
	int column;
	const char *results;
};

// ----- RESULT_ -------------------------------------------------------------
static TColumnResult ColumnResults[] =
{
	// 2016:028 +Ê where Ø
#if COURT_TYPE == COURT_APPEAL
	// report2 p1
	{ 23020, "Cv" },
	{ 23036, "E¾w" },
	{ 23037, "G¿zÎ" },
	{ 23038, "H" },
	{ 23026, "Ø" "Ê" },
	{ 23043, "12" },	// 2011:231 LPR: +12
	{ 23044, "uØ" "Ê" },
	// report2 p2
	//{ 23010, "ÃCv" },	// 2011:234 LPR/LRQ: reverted
	{ 23011, "Ä" },
	{ 23012, "È" },
	{ 23013, "É" },
	{ 23014, "Ë" },
	{ 23015, "ÏEw" },
	{ 23016, "Ó" },
	{ 23017, "Ô¾" },
	{ 23018, "Ö¿¯Î" },
	//{ 23019, "×Gz" },	// 2011:234 LPR/LRQ: reverted
	// 23026 - dup
	// report2 p3
	{ 23047, "Ã" },
	{ 23048, "Cv" },
	{ 23049, "v" },
	{ 23050, "v" },
	// 23011 - dup
	// 23012 - dup
	// 23013 - dup
	// 23014 - dup
	{ 23045, "Ï" },
	{ 23116, "Ó" },
	{ 23051, "Ew" },
	{ 23052, "w" },
	{ 23053, "w" },
	{ 23117, "Ô" },
	{ 23118, "Ö¯" },
	{ 23046, "×" },
	{ 23054, "¿G¾zÎ" },
	{ 23055, "zÎ" },
	{ 23056, "zÎ" },
	{ 23057, "Ê" },	// 2011:231 LPR: +Ê
	// 23026 - dup
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	// descbook
	{ 15054, "Cv" },
	{ 15055, "" },
	{ 15056, "" },
	{ 15057, "" },
	{ 15058, "" },
	{ 15059, "" },
	{ 15060, "" },
	{ 15061, "" },
	{ 15062, "¿¾" },
	{ 15063, "" },
	{ 15040, "Cv" },
	{ 15041, "Ew" },
	{ 15042, "Gz" },
	{ 15043, "¿¾Î" },
	{ 15044, "H" },
	// report2 p1
	{ 23020, "Cv" },
	{ 23036, "E¾¥w" },
	{ 23037, "G¿ÌzÎ" },
	{ 23038, "H" },
	{ 23026, "Ø" "Ê" },
	{ 23043, "78" },
	{ 23044, "uØ" "Ê" },
	// report2 p2
	//{ 23010, "CvÊ" },	// 2011:234 LPR/LRQ: reverted
	{ 23011, "" },
	{ 23012, "" },
	{ 23013, "" },
	{ 23014, "" },
	{ 23015, "Ew" },
	{ 23016, "¥" },
	{ 23017, "¾" },
	{ 23018, "¿Î" },
	//{ 23019, "Gz" },	// 2011:234 LPR/LRQ: reverted
	// 23026 - dup
	// report2 p3
	{ 23047, "" },
	{ 23048, "Cv" },
	{ 23049, "v" },
	{ 23050, "v" },
	// 23011 - dup
	// 23012 - dup
	// 23013 - dup
	// 23014 - dup
	{ 23045, "" },
	{ 23116, "" },
	{ 23051, "¥Ew" },
	{ 23052, "w" },
	{ 23053, "w" },
	{ 23117, "" },
	{ 23118, "" },
	{ 23046, "" },
	{ 23054, "¿G¾zÎ" },
	{ 23055, "zÎ" },
	{ 23056, "zÎ" },
	{ 23057, "Ê" },
	// 23026 - dup
#endif  // ADMIN
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	// descbook
	{ 15054, "ÃCv" },
	{ 15055, "Ä" },
	{ 15056, "È" },
	{ 15057, "É" },
	{ 15058, "Ë" },
	{ 15059, "Ï" },
	{ 15060, "Ó" },
	{ 15061, "Ô" },
	{ 15062, "¿¾Ö¯" },
	{ 15063, "×" },
	{ 15040, "Cv" },
	{ 15041, "Ew" },
	{ 15042, "Gz" },
	{ 15043, "¿¾Î" },
	{ 15044, "H" },
#endif  // APPEAL || AREAL || MILITARY
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	// report2 p1
	{ 23020, "Cv" },
	{ 23036, "E¾¥w" },
	{ 23037, "G¿ÌzÎ" },
	{ 23038, "H" },
	{ 23026, "Ø" "Ê" },
	{ 23043, "78" },
	{ 23044, "uØ" "Ê" },
	// report2 p2
	//{ 23010, "ÃCvÊ" },	// 2011:234 LPR/LRQ: reverted
	{ 23011, "Ä" },
	{ 23012, "È" },
	{ 23013, "É" },
	{ 23014, "Ë" },
	{ 23015, "ÏEw" },
	{ 23016, "Ó¥" },
	{ 23017, "Ô¾" },
	{ 23018, "Ö¿¯Î" },
	//{ 23019, "×Gz" },	// 2011:234 LPR/LRQ: reverted
	// 23026 - dup
	// report2 p3
	{ 23047, "Ã" },
	{ 23048, "Cv" },
	{ 23049, "v" },
	{ 23050, "v" },
	// 23011 - dup
	// 23012 - dup
	// 23013 - dup
	// 23014 - dup
	{ 23045, "Ï" },
	{ 23116, "Ó" },
	{ 23051, "¥Ew" },
	{ 23052, "w" },
	{ 23053, "w" },
	{ 23117, "Ô" },
	{ 23118, "Ö¯" },
	{ 23046, "×" },
	{ 23054, "¿G¾zÎ" },
	{ 23055, "zÎ" },
	{ 23056, "zÎ" },
	{ 23057, "Ê" },
	// 23026 - dup
#endif  // AREAL || MILITARY
	{ 16041, "Ø" "Ê" },
	{ 25011, "Ø" "Ê" },
	{ 0, NULL }
};

static constant _column_results(_full_column column)
{
	for (const TColumnResult *result = ColumnResults; result->column; result++)
		if (result->column == column)
			return result->results;

	fatal("%d: not supported by _column_results", column);
	return NULL;
}
# endif

void timeBookHeap :: clearData ()
 {
  lWithDec = 0 ;
  lReturneds = 0 ;
  lSends = 0 ;
  lCancOthReasons = 0 ;
  lUpTo3Months = 0 ;
  lOver3Months = 0 ;
  lUpTo1MonthLong = 0 ;       //20050318
  lBetw1And3MonthsLong = 0 ;  //20050318

  lNotMajorDefendants = 0 ;
# if INSTANCE
  lByComplaints = 0 ;
  lByObjections = 0 ;
  lJudgemConfirmed = 0 ;
  lCndJdgApply = 0 ;
  lCndJdgRepeal = 0 ;  // 18.feb.2005
  lJdgChg_PunismDecr = 0 ;  // 18.feb.2005
  lJdgChg_PunismIncr = 0 ;  // 18.feb.2005
  lOtherChangesPunishmPart = 0 ;  // 18.feb.2005
  lOtherChangesCitizenPart = 0 ;  // 18.feb.2005
  lJdgRepealPartReconsider = 0 ;  // 18.feb.2005
  lJudgAbolishedPart = 0 ;
  lJdgFullyAboli_Ret = 0 ;
  lJdgFullyAboli_NewJ = 0 ;
  lCanceleds = 0 ;
# endif
  lNewlyCreateds = 0 ;
  lReceiveds = 0 ;
  lRetFrom2ndInstance = 0 ;
  lClaimFullyGranted = 0 ;
  lClaimPartialGranted = 0 ;
  lClaimNonsuit = 0 ;
  lCancelByAgreement = 0 ;
  lCancel_ExactByOtherCauses = 0 ;
# if INSTANCE
  lDecisionValidityConfirmed = 0 ;
  lDecisionRepealedPartilally = 0 ;
  lDecisRepealFully_Returned = 0 ;
  lDecisRepealFully_NewDecis = 0 ;
  lDecisRepeal_Reconsideration = 0 ;  // 18.feb.2005
  s_IIinst_Decision = 0 ;  // 20051208
  s_IIinst_Cease = 0 ;     // 20051208
# endif
  sFixeds = 0 ;
  sLefts = 0 ;
  sStoppeds = 0 ;
  sOtherSess = 0 ;
  sClosedSess = 0 ;
  lcAccusedsNum = 0 ;
 }  // timeBookHeap :: clearData

timeBookHeap :: timeBookHeap ()
 {
  clearData () ;
 }  // timeBookHeap :: timeBookHeap

void timeBookHeap :: operator += (const timeBookHeap & oth)
 {
  lWithDec += oth.lWithDec ;
  lReturneds += oth.lReturneds ;
  lSends += oth.lSends ;
  lCancOthReasons += oth.lCancOthReasons ;
  lUpTo3Months += oth.lUpTo3Months ;
  lOver3Months += oth.lOver3Months ;
  lUpTo1MonthLong += oth.lUpTo1MonthLong ;       //20050318
  lBetw1And3MonthsLong += oth.lBetw1And3MonthsLong;  //20050318
  lNotMajorDefendants += oth.lNotMajorDefendants ;

  sFixeds += oth.sFixeds ;
  sLefts += oth.sLefts ;
  sStoppeds += oth.sStoppeds ;
  sOtherSess += oth.sOtherSess ;
  sClosedSess += oth.sClosedSess ;

  lcAccusedsNum += oth.lcAccusedsNum ;
# if INSTANCE
  s_IIinst_Decision  += oth.s_IIinst_Decision;  // 20051208
  s_IIinst_Cease  += oth.s_IIinst_Cease;  // 20051208
# endif
 }  // void timeBookHeap :: operator += (const timeBookHeap & oth)

// 14.feb.2000 - begin
void markDecGlJResInTBHeap (
# if INSTANCE
				    const char decResult, const char sessResult,
				    timeBookHeap & tBH
# else
				    const char, const char, timeBookHeap &
# endif
				   )
 {
# if INSTANCE

  tBH.lJudgemConfirmed += strchr(_column_results(dB_JudgementUpheld), decResult) != NULL;
  tBH.lCndJdgApply += strchr(_column_results(dB_ConditionalConvictingApplied), decResult) != NULL;
  tBH.lCndJdgRepeal += strchr(_column_results(dB_ConditionalConvictingRepealed), decResult) != NULL;
  tBH.lJdgChg_PunismDecr += strchr(_column_results(dB_PunishmentDecreased), decResult) != NULL;
  tBH.lJdgChg_PunismIncr += strchr(_column_results(dB_PunishmentIncreased), decResult) != NULL;
  tBH.lOtherChangesPunishmPart += strchr(_column_results(dB_AnotherChangesPunishmentPart), decResult) != NULL;
  tBH.lOtherChangesCitizenPart += strchr(_column_results(dB_ChangesCitizenPart), decResult) != NULL;
  tBH.lJdgRepealPartReconsider += strchr(_column_results(dB_JudgemRepealPart_Reconsider), decResult) != NULL;
  tBH.lJdgFullyAboli_Ret += strchr(_column_results(dB_Reconsideration), decResult) != NULL;
  tBH.lJdgFullyAboli_NewJ += strchr(_column_results(dB_NewJudgement), decResult) != NULL;
  tBH.lDecisionValidityConfirmed += strchr(_column_results(dB_DecisionFullyUpheld), decResult) != NULL;
  tBH.lDecisionRepealedPartilally += strchr(_column_results(dB_DecisionPartiallyChanged), decResult) != NULL;
  tBH.lDecisRepealFully_NewDecis += strchr(_column_results(dB_DecisionRepealed_NewDecision), decResult) != NULL;
  tBH.lDecisRepeal_Reconsideration += strchr(_column_results(dB_DecRepealed_Reconsideration), decResult) != NULL;
  tBH.lDecisRepealFully_Returned += strchr(_column_results(dB_DecisionInvalidated), decResult) != NULL;
  if (decResult == RESULT_JUDGEM_REPEAL_PARTILALLY ||  // col.14 of 2nd rep.
	decResult == RESULT_COND_CONVICTING_ABOLISHED || // col. 9 of 2nd rep.
	decResult == RESULT_PUNISHMENT_DECREASED ||      // col.10 of 2nd rep.
	decResult == RESULT_PUNISHMENT_INCREASED ||      // col.11 of 2nd rep.
	decResult == RESULT_JUDG_CHG_PUN_PART ||    // col.12 of 2nd rep. ???
	decResult == RESULT_JUDG_CHG_CIT_PART)    // col.13 of 2nd rep.
    tBH.lJudgAbolishedPart ++ ;
  // 2015:188 LPR/IRQ: ignore session result
  if (strchr(RESULT_CANCELAWS, decResult))// &&
	//sessResult == RESULT_DEFINITION_TERMINATING)
    tBH.lCanceleds ++ ;  // 14.feb.2000
  // 20051208 ->
  if (  (sessResult == RESULT_ANNOUNCE_TO_BE_DECIDED) // = 'p'
	||(sessResult == RESULT_WITH_DECISION)          // = 'Z'
     )
       tBH.s_IIinst_Decision ++ ;
  if (  (sessResult == RESULT_CANCELED)            //  = 'c'
      ||(sessResult == RESULT_CANCEL_BY_AGREEMENT) //  = 'P'
      ||(sessResult == RESULT_SEND_BY_JURIDICTION) //  = 's'
     )
       tBH.s_IIinst_Cease ++ ;
  // 20051208 <-
# endif
 }  // markDecGlJResInTBHeap
// 14.feb.2000 - end

void accusedsDistribution :: clearData ()
 {
  cJudgedsTotal = 0 ;
  cJudgedsCond = 0 ;
  cAccquits = 0 ;
  cFromCancLaws = 0 ;
  cFreeds = 0 ;
  cGenRecidive = 0 ;
  cSpecRecidive = 0 ;
  cNewCrimeDuringTestTime = 0 ;
  cDangerousRecidive = 0 ;  // 14.feb.2005
 }  // accusedsDistribution :: clearData

accusedsDistribution  :: accusedsDistribution ()
 {
  clearData () ;
 }  // accusedsDistribution :: accusedsDistribution

void accusedsDistribution ::
       operator += (const accusedsDistribution & other)
 {
  cJudgedsTotal += other.cJudgedsTotal ;
  cJudgedsCond += other.cJudgedsCond ;
  cAccquits += other.cAccquits ;
  cFromCancLaws += other.cFromCancLaws ;
  cFreeds += other.cFreeds ;
  cGenRecidive += other.cGenRecidive ;
  cSpecRecidive += other.cSpecRecidive ;
  cNewCrimeDuringTestTime += other.cNewCrimeDuringTestTime ;
  cDangerousRecidive += other.cDangerousRecidive ;  // 14.feb.2005
 }  // accusedsDistribution  :: operator +=

void markInDistribution (const TJudgement & lJ,
                         accusedsDistribution & accDistr)
 {
  if ((JUDGEMENT_X_PRIORITY & lJ.flagsX) != JUDGEMENT_X_PRIORITY)
    return ;  // 12.jan.2001
  if (strchr (JUDGED_CONVICTEDS, lJ.judged))
   {
    accDistr.cJudgedsTotal ++ ;
    if (strchr (PUNISHMENT_UP_TO_3_YEARS_CONDS, lJ.punishment)
# if INSTANCE  // 20.apr.2001 - begin
# else // of INSTANCE
        || strchr (PUNISHMENT_OTHER_CONDIT_S, lJ.punishment)
# endif  // of INSTANCE  // 20.apr.2001 - end
       )
      accDistr.cJudgedsCond ++ ;
   }
  if (strchr (JUDGED_ACQUITS, lJ.judged))
    accDistr.cAccquits ++ ;
  if (strchr (JUDGED_FREEDS, lJ.judged))
    accDistr.cFreeds ++ ;
  if (RECIDIVE_GENERAL == lJ.recidive)
    accDistr.cGenRecidive ++ ;
  if (RECIDIVE_SPECIAL == lJ.recidive)
    accDistr.cSpecRecidive ++ ;
  if (RECIDIVE_DANGEROUS == lJ.recidive)
   {
    accDistr.cDangerousRecidive ++ ;  // 14.feb.2004
   }
  if ((JUDGEMENT_X_NEW & lJ.flagsX) == JUDGEMENT_X_NEW)  // 13.jan.2004
    accDistr.cNewCrimeDuringTestTime ++ ;
  if (strchr (JUDGED_CANCELEDS, lJ.judged))
    accDistr.cFromCancLaws ++ ;
 }  // markInDistribution

mergedKinds :: mergedKinds ()
 {
  Add(
    new TLong ("ORDER", & order),
    new TString ("KINDS", kinds, sizeof (kinds)),
    new TString ("ALIAS", alias, sizeof (alias)),
    NULL);
  order = 0 ;
  setmem (kinds, sizeof (kinds), '\0') ;
  setmem (alias, sizeof (alias), '\0') ;
 }  // mergedKinds :: mergedKinds

static bool operator==(const TSubject &subject1, const TSubject &subject2)
{
	return subject1.type == subject2.type && subject1.subject == subject2.subject;
}

static bool operator<(const TSubject &subject1, const TSubject &subject2)
{
	return subject1.type < subject2.type || (subject1.type == subject2.type && subject1.subject < subject2.subject);
}

subjectHeap :: subjectHeap () : nomHeap <TSubject> ()
 {
 }  // subjectHeap :: subjectHeap

kindHeap :: kindHeap () : nomHeap <TKind> ()
 {
 }  // kindHeap :: kindHeap

static bool operator==(const TKind &s1, const TKind &s2)
 {
// return s1.kind == s2.kind ;
  return (! strcmp (s1.name, s2.name) && s1.kind == s2.kind) ;
 }  // bool operator == (const TKind & s1, const TKind & s2)

static bool operator<(const TKind &s1, const TKind &s2)
 {
//  return s1.kind < s2.kind ;
  const int namesCmpRes = strcmp (s1.name, s2.name) ;

  if (namesCmpRes < 0)
    return true ;
  else
    if (namesCmpRes > 0)
	return false ;
    else
     return s1.kind < s2.kind ;
 }  // bool operator < (const TKind & s1, const TKind & s2)

bool operator<=(const TKind &s1, const TKind &s2) { return s1 < s2 || s1 == s2; }

subjectRangeHeap :: subjectRangeHeap () : nomHeap <TSubjectRange> ()
 {
 }  // subjectRangeHeap :: subjectRangeHeap

bool operator==(const TSubjectRange &sR1, const TSubjectRange &sR2)
 {
  return sR1.subject == sR2.subject &&
	    sR1.type == sR2.type ;
 }  // bool operator == (const TSubjectRange & sR1,
    //                   const TSubjectRange & sR2)

bool operator<(const TSubjectRange &sR1, const TSubjectRange & sR2)
 {
  if (sR1.type < sR2.type)
    return true ;
  else
    if (sR1.type > sR2.type)
	return false ;
    else
	return sR1.subject < sR2.subject ;
 }  // bool operator < (const TSubjectRange & sR1,
    //                  const TSubjectRange & sR2)

bool operator<=(const TSubjectRange &sR1, const TSubjectRange &sR2) { return sR1 < sR2 || sR1 == sR2; }

mergedKindsHeap :: mergedKindsHeap () : nomHeap <mergedKinds> ()
 {
 }  // mergedKindsHeap :: mergedKindsHeap

orderedDamnType :: orderedDamnType () : TGroup ("T_COMPANY_TYPE", NULL)
 {
  Add(
    new TChar ("F_COMPANY_TYPE", & companyType),
    new TString ("F_NAME", name, sizeof (name)),
    new TLong ("F_ORDER", & order),
    new TLong ("F_SUBJECT", & subject),
    NULL);
  clearGroupData (* this) ;

  descs = Descs + TABLE_COMPANY_TYPE;
 }  // orderedDamnType :: orderedDamnType

void orderedDamnType::ExactCriteria(msql &m, const char *fields)
{
	orderedDamnType::FloatCriteria(m, fields);
	m.Add(this, "F_COMPANY_TYPE");
}

void orderedDamnType::FloatCriteria (msql &m, const char *fields)
{
	m.Begin(Name, fields);
}

orderedDamnTypeHeap :: orderedDamnTypeHeap () :
				 nomHeap <orderedDamnType> ()
 {
 }  // orderedDamnTypeHeap :: orderedDamnTypeHeap

bool operator==(const orderedDamnType &s1, const orderedDamnType &s2)
 {
  return (s1.order == s2.order && s1.subject == s2.subject &&
	    ! strcmp (s1.name, s2.name) && s1.companyType == s2.companyType) ;
 }  // bool operator == (const orderedDamnType & s1,
    //                   const orderedDamnType & s2)

bool operator<(const orderedDamnType &s1, const orderedDamnType &s2)
 {
  if (s1.order < s2.order)
    return true ;
  else
    if (s1.order > s2.order)
	return false ;
    else
	if (s1.subject < s2.subject)
	  return true ;
	else
	  if (s1.subject > s2.subject)
	    return false ;
	  else
	   {
	    const int nc = strcmp (s1.name, s2.name) ;

	    if (nc < 0)
		return true ;
	    else
		if (nc > 0)
		  return false ;
		else
		  return (s1.companyType < s2.companyType) ;
	   }
 }  // bool operator < (const orderedDamnType & s1,
    //                  const orderedDamnType & s2)

bool operator<=(const orderedDamnType &s1, const orderedDamnType &s2) { return s1 < s2 || s1 == s2; }

void subjectHeapArray :: load (const char * const fmtN, const char type)
 {
  const size_t sZ = strlen (fmtN) + SIZE_OF_LINE ;
  char * const tmpN = new char [sZ] ;

  scSprintf (sZ, tmpN, "%s", fmtN) ;
  if (! access (tmpN, 0) ||
	referenceToFmtFile (tmpN, sZ))
   {  // -- 0 --
    ifstream ifs (tmpN) ;

    if (ifs)
     {  // -- 1 --
      if (findSection (ifs, tmpN, SUBJECTS_SECTION))
       {  // -- 2 --
        char * line = new char [SIZE_OF_LINE] ;
        subjectHeap sH ;

        line [0] = '\0' ;
        while (ifs.good () && ! endSection (line))
         {  // -- 3 --
          line [0] = '\0' ;
          ifs.getline (line, SIZE_OF_LINE) ;
          if (ifs.good () && ! endSection (line))
           {  // -- 4 --
             {
              char * const semicolonPtr = strchr (line, ';') ;

              if (semicolonPtr)
                * semicolonPtr = '\0' ;
             }
            if (! strIsBlank (line))
             {
              istrstream iss (line, strlen (line)) ;

              while (iss.good ())
               {  // -- 5 --
                iss >> sH.nom.subject ;

                if (iss.good () || iss.rdstate () == ios :: eofbit)
                 {  // -- 6 --
                  sH.nom.type = type ;
                  if (sH.nom.Get ())
                   {
                    sH.heap = 0 ;
                    putItem (& sH) ;
                   }
                 }  // -- 6 --
               }  // -- 5 --
             }
           }  // -- 4 --
         }  // -- 3 --
        delete [] line ;
       }  // -- 2 --
     }  // -- 1 --
    else
	cannotOpenFmtFile (tmpN) ;
   }  // -- 0 --
  delete [] tmpN ;
 }  // subjectHeapArray :: load

subjectHeapArray :: subjectHeapArray (const char * const fmtN,
						  const char type) :
			    heapArray <subjectHeap> ()
 {
  if (fmtN)
    load (fmtN, type) ;
 }  // subjectHeapArray :: subjectHeapArray

subjectHeapArray :: subjectHeapArray () : heapArray <subjectHeap> ()
 {
 }  // subjectHeapArray :: subjectHeapArray

void kindHeapArray :: load (const char type)
 {
  if (! type)
    return ;
    
  TQuery q ("SELECT * FROM T_KIND WHERE F_ORDER IS NOT NULL") ;
  kindHeap kH ;

  while (q.Read ())
   {
    kH.nom << q ;
    if (kind2type (kH.nom.kind) == type)
      putItem (& kH) ;
   }
 }  // kindHeapArray :: load

void kindHeapArray :: load (const char * const fmtN, const char type)
 {
  if (! (fmtN && type))
    return ;
  const size_t sZ = strlen (fmtN) + SIZE_OF_LINE ;
  char * const tmpN = new char [sZ] ;

  scSprintf (sZ, tmpN, "%s", fmtN) ;
  if (! access (tmpN, 0) ||
	referenceToFmtFile (tmpN, sZ))
   {  // -- 0 --
    ifstream ifs (tmpN) ;

    if (ifs)
     {  // -- 1 --
      if (findSection (ifs, tmpN, WANT_KINDS_SECTION))
       {  // -- 2 --
        char * line = new char [SIZE_OF_LINE] ;
        bool mustLoad = false ;

        line [0] = '\0' ;
        while (ifs.good () && ! endSection (line))
         {  // -- 3 --
          line [0] = '\0' ;
          ifs.getline (line, SIZE_OF_LINE) ;

          if (ifs.good () && ! endSection (line))
           {  // -- 4 --
             {
              char * const semicolonPtr = strchr (line, ';') ;

              if (semicolonPtr)
                * semicolonPtr = '\0' ;
             }
            strDelLastSpaces (line) ;
            if (! strIsBlank (line))
              mustLoad = ! strcmp (line, "YES") ;
           }  // -- 4 --
         }  // -- 3 --
        if (mustLoad)
          load (type) ; 
        delete [] line ;
       }  // -- 2 --
     }  // -- 1 --
    else
	cannotOpenFmtFile (tmpN) ;
   }  // -- 0 --
  delete [] tmpN ;
 }  // kindHeapArray :: load

kindHeapArray :: kindHeapArray (const char type) :
                   heapArray <kindHeap> ()
 {
  if (type)
    load (type) ;
 }  // kindHeapArray :: kindHeapArray

kindHeapArray :: kindHeapArray () : heapArray <kindHeap> ()
 {
 }  // kindHeapArray :: kindHeapArray

kindHeapArray :: kindHeapArray (const char * const fmtN, const char type)
					     : heapArray <kindHeap> ()
 {
  if (fmtN && type)
    load (fmtN, type) ;
 }  // kindHeapArray :: kindHeapArray
                
void subjectRangeHeapArray :: load (const char * const fmtN,
                                    const char type,
						TWindow * )
 {
  const size_t sZ = strlen (fmtN) + SIZE_OF_LINE ;
  char * const tmpN = new char [sZ] ;

  scSprintf (sZ, tmpN, "%s", fmtN) ;
  if (! access (tmpN, 0) ||
	referenceToFmtFile (tmpN, sZ))
   {  // -- 0 --
    ifstream ifs (tmpN) ;

    if (ifs)
     {  // -- 1 --
      if (findSection (ifs, tmpN, SUBJECT_RANGES_SECTION))
       {  // -- 2 --
        char * line = new char [SIZE_OF_LINE] ;
        subjectRangeHeap sRH ;

        line [0] = '\0' ;
        while (ifs.good () && ! endSection (line))
         {  // -- 3 --
          line [0] = '\0' ;
          ifs.getline (line, SIZE_OF_LINE) ;

          if (ifs.good () && ! endSection (line))
           {  // -- 4 --
             {
              char * const semicolonPtr = strchr (line, ';') ;

              if (semicolonPtr)
                * semicolonPtr = '\0' ;
              strDelLastSpaces (line) ;  
             }
            if (! strIsBlank (line))
             {
              const size_t lL = strlen (line) ;
              istrstream iss (line, lL) ;
              long int s, lSR, hSR ;

              iss >> s >> lSR >> hSR ;

              if (iss.good ())
               {  // -- 5 --
                sRH.nom.type = type ;
		    sRH.nom.subject = s ;
		    sRH.nom.lowSubjectRange = lSR ;
                sRH.nom.highSubjectRange = hSR ;
                 {
                  long int pos ;

                  while ((pos = iss.tellg ()) < (lL - 1) &&
                         line [pos] == ' ')
			  iss.seekg (pos + 1) ;
			strzcpy(sRH.nom.name, line + pos, sizeof sRH.nom.name);
		     }
		    sRH.heap = 0 ;
                putItem (& sRH) ;
               }  // -- 5 --
		  else
		    error(CANNOT_LOAD_THE_FORMAT_INFO) ;
		 }
	     }  // -- 4 --
         }  // -- 3 --
        delete [] line ;
       }  // -- 2 --
     }  // -- 1 --
    else
	cannotOpenFmtFile (tmpN) ;
   }  // -- 0 --
  delete [] tmpN ;
 }  // subjectRangeHeapArray :: load

subjectRangeHeapArray :: subjectRangeHeapArray () :
                           heapArray <subjectRangeHeap> ()
 {
 }  // subjectRangeHeapArray :: subjectRangeHeapArray ()

void subjectRangeHeapArray :: incSRItems (const TSubject & subj)
 {
  const int cnt = Count () ;

  for (int i = 0 ; i < cnt ; i ++)
   {
    subjectRangeHeap & rSRH = * ((* this) [i]) ;

    if (subj.type == rSRH.nom.type && rSRH.nom.InsideRange (subj.subject))
	rSRH.heap += 1 ;
   }
 }  // subjectRangeHeapArray :: incSRItems
 
subjectRangeHeapArray :: subjectRangeHeapArray (const char * const fmtN,
                                                const char type,
                                                TWindow * pParent)
 {
  if (fmtN)
    load (fmtN, type, pParent) ;
 }  // subjectRangeHeapArray ::
    //   subjectRangeHeapArray (const char * const fmtN,
    //                          const char type, TWindow * pParent)

void mergedKindsHeapArray :: load (const char * const fmtN,
					     TWindow * )
 {
  const size_t sZ = strlen (fmtN) + SIZE_OF_LINE ;
  char * const tmpN = new char [sZ] ;

  scSprintf (sZ, tmpN, "%s", fmtN) ;
  if (! access (tmpN, 0) ||
	referenceToFmtFile (tmpN, sZ))
   {  // -- 0 --
    ifstream ifs (tmpN) ;

    if (ifs)
     {  // -- 1 --
      if (findSection (ifs, tmpN, MERGED_KINDS_SECTION))
       {  // -- 2 --
        char * line = new char [SIZE_OF_LINE] ;
        mergedKindsHeap mKH ;

        line [0] = '\0' ;
        while (ifs.good () && ! endSection (line))
         {  // -- 3 --
          line [0] = '\0' ;
          ifs.getline (line, SIZE_OF_LINE) ;

          if (ifs.good () && ! endSection (line))
           {  // -- 4 --
             {
              char * const semicolonPtr = strchr (line, ';') ;

              if (semicolonPtr)
                * semicolonPtr = '\0' ;
              strDelLastSpaces (line) ;  
             }
            if (! strIsBlank (line))
             {
              const size_t lL = strlen (line) ;
              istrstream iss (line, lL) ;
              long int o ;

              iss >> o ;

              if (iss.good ())
               {  // -- 5 --
                mKH.nom.order = o ;
                 {
                  long int pos ;

                  while ((pos = iss.tellg ()) < (lL - 1) &&
                         line [pos] == ' ')
                    iss.seekg (pos + 1) ;
                 }
                iss.getline (mKH.nom.kinds,
                             sizeof (mKH.nom.kinds), ' ') ;
                if (iss.good ())
                 {  // -- 6 --
                   {
                    long int pos ;

                    while ((pos = iss.tellg ()) < (lL - 1) &&
                           line [pos] == ' ')
                      iss.seekg (pos + 1) ;
                    iss.getline (mKH.nom.alias, sizeof (mKH.nom.alias)) ;
                   }
                  if (strlen (mKH.nom.alias))
                   {
                    mKH.heap = 0 ;
                    putItem (& mKH) ;
                   }
                  else
			  error(CANNOT_LOAD_THE_FORMAT_INFO) ;
		     }  // -- 6 --
		    else
			error(CANNOT_LOAD_THE_FORMAT_INFO) ;
		   }  // -- 5 --
		  else
		    error(CANNOT_LOAD_THE_FORMAT_INFO) ;
		 }
	     }  // -- 4 --
         }  // -- 3 --
        delete [] line ;
       }  // -- 2 --
     }  // -- 1 --
    else
	cannotOpenFmtFile (tmpN) ;
   }  // -- 0 --
  delete [] tmpN ;
 }  // mergedKindsHeapArray :: load

mergedKindsHeapArray :: mergedKindsHeapArray () :
                           heapArray <mergedKindsHeap> ()
 {
 }  // mergedKindsHeapArray :: mergedKindsHeapArray ()

void mergedKindsHeapArray :: incMKItems (const char kind)
 {
  const int cnt = Count () ;

  for (int i = 0 ; i < cnt ; i ++)
   {
    mergedKindsHeap & rMKH = * ((* this) [i]) ;

    if (strchr (rMKH.nom.kinds, kind))
      rMKH.heap += 1 ;
   }
 }  // mergedKindsHeapArray :: incMKItems
 
mergedKindsHeapArray :: mergedKindsHeapArray (const char * const fmtN,
                                              TWindow * pParent)
 {
  if (fmtN)
    load (fmtN, pParent) ;
 }  // mergedKindsHeapArray ::
    //   mergedKindsHeapArray (const char * const fmtN,
    //                          const char type, TWindow * pParent)

# if INSTANCE
void orderedDamnTypeHeapArray :: load ()
 {
  TQuery q ("SELECT * FROM T_COMPANY_TYPE ;") ;
  orderedDamnTypeHeap kH ;

  while (q.Read ())
   {
    kH.nom << q ;
    putItem (& kH) ;
   }
 }  // orderedDamnTypeHeapArray :: load

void orderedDamnTypeHeapArray :: load (const char * const fmtN,
						   const char type)
 {
  if (! (fmtN && type == TYPE_FIRM_LAW))
    return ;
  const size_t sZ = strlen (fmtN) + SIZE_OF_LINE ;
  char * const tmpN = new char [sZ] ;

  scSprintf (sZ, tmpN, "%s", fmtN) ;
  if (! access (tmpN, 0) ||
	referenceToFmtFile (tmpN, sZ))
   {  // -- 0 --
    ifstream ifs (tmpN) ;

    if (ifs)
     {  // -- 1 --
      if (findSection (ifs, tmpN, WANT_COMPANY_TYPES_SECTION))
       {  // -- 2 --
        char * line = new char [SIZE_OF_LINE] ;
        bool mustLoad = false ;

        line [0] = '\0' ;
        while (ifs.good () && ! endSection (line))
         {  // -- 3 --
          line [0] = '\0' ;
          ifs.getline (line, SIZE_OF_LINE) ;

          if (ifs.good () && ! endSection (line))
           {  // -- 4 --
             {
              char * const semicolonPtr = strchr (line, ';') ;

              if (semicolonPtr)
                * semicolonPtr = '\0' ;
             }
            strDelLastSpaces (line) ;
            if (! strIsBlank (line))
              mustLoad = ! strcmp (line, "YES") ;
           }  // -- 4 --
         }  // -- 3 --
        if (mustLoad)
          load () ; 
        delete [] line ;
       }  // -- 2 --
     }  // -- 1 --
    else
	cannotOpenFmtFile (tmpN) ;
   }  // -- 0 --
  delete [] tmpN ;
 }  // kindHeapArray :: load

orderedDamnTypeHeapArray ::
  orderedDamnTypeHeapArray (const char * const fmtN, const char type) :
					heapArray <orderedDamnTypeHeap> ()
 {
  if (fmtN && type)
    load (fmtN, type) ;
 }  // orderedDamnTypeHeapArray :: orderedDamnTypeHeapArray

# endif

void summaryData :: clear ()
 {
  setmem (name, sizeof (name), '\0') ;
  heap = 0 ;
 }  // summaryData :: clear

summaryData :: summaryData ()
 {
  clear () ;
 }  // summaryData :: summaryData
 
void summaryFmtHeadData :: clear ()
 {
  bNENY.clear () ;
  cCBDEDBN.clear () ;
 }  // summaryFmtHeadData :: clear

summaryFmtHeadData :: summaryFmtHeadData ()
 {
  clear () ;
 }  // summaryFmtHeadData :: summaryFmtHeadData

ostream & operator << (ostream & os, const arrayOfLong & lA)
 {
  const int items = lA.Count () ;

  for (int i = 0 ; i < items ; i ++)
   {
    os << lA [i] ;
    if (i < items - 1)
      os << "," ;
   }
  return os ;
 }  // ostream & operator << (ostream & os, arrayOfLong & lA)

BriefSessionData :: BriefSessionData ()
 {
  Add(

    NEW_RCDKEY ("F_", & key),
    new TDate ("F_DATE", & date, false),
    new TTime ("F_TIME", & time),
    new TChar ("F_KIND", & kind),
    new TChar ("F_RESULT", & result),
    new TString ("F_JUDGE", judge, sizeof (judge)),
    new TString ("F_JUDGE_X", judgeX, sizeof (judgeX)),
    new TString ("F_REJUDGE_X", reJudgeX, sizeof (reJudgeX)),
    new TString ("F_PRESIDENT", president, sizeof (president)),
    NULL);

  clearGroupData (* this) ;
 }  // BriefSessionData :: BriefSessionData

// 18.sep.2000 - begin
void BriefSessionData ::
BrSessData_to_TDatedCont (const BriefSessionData & bsd,
                          TDatedContainer & container)
 {
  clearGroupData (container) ;
  container.key = bsd.key ;
  container.kind = bsd.kind ;
  container.date = bsd.date ;
 }  // BriefSessionData :: BrSessData_to_TDatedCont

bool
BriefSessionData :: operator==(const BriefSessionData &other) const
 {
  return (
	    key.type == other.key.type &&
	    key.year == other.key.year &&
	    key.no == other.key.no &&
	    date == other.date &&
	    kind == other.kind
	   ) ;
 }  // BriefSessionData :: operator ==

bool BriefSessionData :: operator<(const BriefSessionData &other) const
 {
  if (key.type < other.key.type)
    return true ;
  else
    if (key.type > other.key.type)
	return false ;
    else
	if (key.year < other.key.year)
	  return true ;
	else
	  if (key.year > other.key.year)
	    return false ;
	  else
	    if (key.no < other.key.no)
		return true ;
	    else
		if (key.no > other.key.no)
		  return false ;
		else
		  if (date < other.date)
		    return true ;
		  else
		    if (date > other.date)
			return false ;
		    else
			return kind < other.kind ;
 }  // BriefSessionData :: operator <

// 18.sep.2000 - end

// 19.jun.2000 - begin
void CodeValue :: Clear ()
 {
  value = code = 0 ;
 }  // CodeValue :: Clear

CodeValue :: CodeValue (const unsigned int aCode,
                        const unsigned long int aValue)
 {
  Clear () ;
  code = aCode ;
  value = aValue ;
 }  // CodeValue :: CodeValue

void CodeValue :: operator = (const CodeValue & src)
 {
  Clear () ;
  code = src.code ;
  value = src.value ;
 }  // void CodeValue :: operator = (const CodeValue & src)

// 18.feb.2003 - end
// 04.mar.2003 - begin
void SimpleStrings :: Clear ()
 {
  code = NULL ;
  str = NULL ;
 }  // SimpleStrings :: Clear

SimpleStrings :: SimpleStrings (const char* const aCode,
    const char* const aStr)
 {
  code = aCode ;
  str = aStr ;
 }  // SimpleStrings :: SimpleStrings

bool SimpleStrings :: operator<(const SimpleStrings &other) const
 {
  bool res ;
  int codeCmp = cmp_str(code, other.code) ;

  if (codeCmp < 0)
    res = true ;
  else
    if (codeCmp > 0)
	res = false ;
    else
	res = (cmp_str(str, other.str) < 0) ;
  return res ;
 }  // SimpleStrings :: operator <

bool SimpleStrings :: operator==(const SimpleStrings &other) const
{
  return (cmp_str (code, other.code) == 0 &&
    cmp_str (str, other.str) == 0) ;
}  // SimpleStrings :: operator ==

const SimpleStrings & SimpleStrings :: operator = (const SimpleStrings & src)
 {
  code = src.code ;
  str = src.str ;
  return (* this) ;
 }  // SimpleStrings :: operator =

// 04.mar.2003 - end

bool operator==(const TNTResult &res0, const TNTResult &res1)
 {
  return res0.result == res1.result ;
 }  // bool operator == (const TNTResult & r0, const TNTResult & r1)

bool operator<(const TNTResult &res0, const TNTResult &res1)
 {
  if (res0 == res1)
    return false ;

  const char r0 = res0.result ;
  const char r1 = res1.result ;
  const bool r0Final =
	r0 != RESULT_NULL ? strchr (RESULT_FINALIZEDS, r0) : false ;
  const bool r1Final =
	r1 != RESULT_NULL ? strchr (RESULT_FINALIZEDS, r1) : false ;

  if (r0Final < r1Final)
    return true ;
  if (r0Final > r1Final)
    return false ;
  if (r0Final)
   {  // -- 0 --
    const bool r0Cancel = strchr (RESULT_CANCELEDS_OR_RETURNEDS, r0) ;
    const bool r1Cancel = strchr (RESULT_CANCELEDS_OR_RETURNEDS, r1) ;
    const bool r0Decides = ! r0Cancel ;
    const bool r1Decides = ! r1Cancel ;

    if (r0Decides < r1Decides)
	return true ;
    if (r0Decides > r1Decides)
	return false ;
   }  // -- 0 --
   {  // -- 1 --
    const bool r0Enter =
	r0 != RESULT_NULL ? strchr (RESULT_ENTERABLES, r0) : false ;
    const bool r1Enter =
	r1 != RESULT_NULL ? strchr (RESULT_ENTERABLES, r1) : false ;

    if (r0Enter < r1Enter)
	return true ;
    if (r0Enter > r1Enter)
	return false ;
   }  // -- 1 --
  return ((unsigned char) r0 < (unsigned char) r1) ;
 }  // bool operator < (const TNTResult & r0, const TNTResult & r1)

bool operator<=(const TNTResult &res0, const TNTResult &res1) { return res0 < res1 || res0 == res1; }

static int results_compare (const char r0, const char r1)
 {
  TNTResult res0, res1 ;

  clearGroupData (res0) ;
  clearGroupData (res1) ;
  res0.result = r0 ;
  res1.result = r1 ;
  if (res0 == res1)
    return 0 ;
  if (res0 < res1)
    return -1 ;
  return 1 ;    
 }  // results_compare

void ResultsSet :: Clear ()
 {
  sessRes = RESULT_NULL ;
  decRes = RESULT_NULL ;
  lawGrant = GRANT_NULL ;
  afterFinish = false ;
  dateRetAfterFinish = false ;
  abolished = false ;
  count = 0 ;
 }  // ResultsSet :: Clear

ResultsSet :: ResultsSet ()
 {
  Clear () ;
 }  // ResultsSet :: ResultsSet

ResultsSet :: ResultsSet (const char sessR, const long int cnt,
                          const char decR, const char lawG,
                          const bool afterFin,
                          const bool dateRetAfterFin, const bool abolish)
 {
  Clear () ;
  sessRes = sessR ;
  decRes = decR ;
  lawGrant = lawG ;
  afterFinish = afterFin ;
  dateRetAfterFinish = dateRetAfterFin ;
  abolished = abolish ;
  count = cnt ;
 }  // ResultsSet :: ResultsSet (const char sessR, const long int cnt,
    //                           const char decR, const char lawG)

const ResultsSet & ResultsSet :: operator = (const ResultsSet & src)
 {
  Clear () ;
  sessRes = src.sessRes ;
  decRes = src.decRes ;
  lawGrant = src.lawGrant ;
  afterFinish = src.afterFinish ;
  dateRetAfterFinish = src.dateRetAfterFinish ;
  abolished = src.abolished ;
  count = src.count ;
  return (* this) ;
 }  // ResultsSet :: operator =

bool ResultsSet :: operator==(const ResultsSet &other) const
 {
  return (sessRes == other.sessRes &&
	    decRes == other.decRes &&
#     if PUNISHMENT_04_AUG_2000_PATCH  // 14.dec.2000
	    lawGrant == other.lawGrant &&
#     endif  // of PUNISHMENT_04_AUG_2000_PATCH  // 14.dec.2000
	    afterFinish == other.afterFinish &&
	    dateRetAfterFinish == other.dateRetAfterFinish &&
	    abolished == other.abolished
	   ) ;
 }  // ResultsSet :: operator ==

bool ResultsSet :: operator<(const ResultsSet &other) const
 {
  if ((* this) == other)
    return false ;
   {
    const int sRCmpRes = results_compare (sessRes, other.sessRes) ;

    if (sRCmpRes < 0)
	return true ;
    if (sRCmpRes > 0)
	return false ;
   }
   {
    const int dRCmpRes = results_compare (decRes, other.decRes) ;

    if (dRCmpRes < 0)
	return true ;
    if (dRCmpRes > 0)
	return false ;
   }
#     if PUNISHMENT_04_AUG_2000_PATCH  // 14.dec.2000
  if (lawGrant < other.lawGrant)
    return true ;
  if (lawGrant > other.lawGrant)
    return false ;
#     endif  // of PUNISHMENT_04_AUG_2000_PATCH  // 14.dec.2000
  if (afterFinish < other.afterFinish)
    return true ;
  if (afterFinish > other.afterFinish)
    return false ;
  if (dateRetAfterFinish < other.dateRetAfterFinish)
    return true ;
  if (dateRetAfterFinish > other.dateRetAfterFinish)
    return false ;
  return abolished < other.abolished ;
 }  // ResultsSet :: operator <

void ResultsSetArray :: operator += (const ResultsSet & rS)
{
	const int ind = Find (&rS) ;

	if (ind != INT_MAX)
		(* this) [ind] -> count += rS.count;
	else
	{
		ResultsSet * rRS = new ResultsSet;
		*rRS = rS;
		Add (rRS) ;
	}
 }  // ResultsSetArray :: operator += (const ResultsSet & rS)

void TRCDKey_ :: Clear ()
 {
  year = no = type = '\0' ;
 }  // TRCDKey_ :: Clear

TRCDKey_ :: TRCDKey_ ()
 {
  Clear () ;
 }  // TRCDKey_ :: TRCDKey_

TRCDKey_ :: TRCDKey_ (const TRCDKey & source)
 {
  Clear () ;
  type = source.type ;
  no = source.no ;
  year = source.year ;
 }  // TRCDKey_ :: TRCDKey_

TRCDKey_ :: TRCDKey_ (const TRCDKey_ & source)
 {
  (* this) = source ;
 }  // TRCDKey_ :: TRCDKey_

TRCDKey_ :: TRCDKey_ (char type_, long int no_, long int year_)
 {
  Clear () ;
  type = type_ ;
  no = no_ ;
  year = year_ ;
 }  // TRCDKey_ :: TRCDKey_

const TRCDKey_ & TRCDKey_ :: operator = (const TRCDKey_ & source)
 {
  Clear () ;
  type = source.type ;
  no = source.no ;
  year = source.year ;
  return (* this) ;
 }  // TRCDKey_ :: operator =

bool TRCDKey_ :: IsEmpty () const
 {
  return (! (type || no || year)) ;
 }  // TRCDKey_ :: IsEmpty ;

bool TRCDKey_ :: FilledOK () const
 {
  return (type && no && year) ;
 }  // TRCDKey_ :: FilledOK ;

char * TRCDKey_ :: Print (char * const s, const size_t sSz) const
 {
  if (s && sSz)
   {
    ostrstream (s, sSz) << (* this) << ends ;
    s [sSz - 1] = '\0' ;
   }
  return s ;
 }  // TRCDKey_ :: Print

char * TRCDKey_ :: PrintShort (char * const s, const size_t sSz) const
 {  // An old variant in VersionArchives\1232\_appname.j --> _data.cpp
  if (s && sSz)
   {
     {
	ostrstream oss (s, sSz) ;

	 {
	  TType & t = * Type ;

	  t.type = type ;
	  if (t.type && t.Try ())
	    oss << t.name [0] << " " ;
        else
          oss << "? " ;
       }
      oss << no << "/" << year << ends ;
     }
    s [sSz - 1] = '\0' ;
   }
  return s ;
 }  // TRCDKey_ :: PrintShort

char* TRCDKey_::PrintAlias(char* const s, const size_t sSz) const
{
  if(s && sSz)
  {
    {
      ostrstream oss(s, sSz);

      {
        TType& t = *Type;

        t.type = type;
        if(t.type && t.Try())
          oss << (t.alias[0] ? t.alias : t.name) << " ";
        else
          oss << "? ";
      }
      oss << no << "/" << year << ends;
    }
    s[sSz - 1] = '\0';
  }
  return s ;
}  // TRCDKey_::PrintAlias

char * TRCDKey_ :: PrintShortest (char * const s, const size_t sSz) const
 {
  if (s && sSz)
   {
    ostrstream (s, sSz) << no << "/" << year << ends ;
    s [sSz - 1] = '\0' ;
   }
  return s ;
 }  // TRCDKey_ :: PrintShortest

bool TRCDKey_ :: operator==(const TRCDKey_ &other) const
 {
  return (type == other.type && no == other.no && year == other.year) ;
 }  // TRCDKey_ :: operator ==

ostream & operator << (ostream & os, const TRCDKey_ & k)
 {  // An old variant in VersionArchives\1232\_appname.j --> _data.cpp
   {
    TType & t = * Type ;

    t.type = k.type ;
    if (t.type && t.Try ())
	os << t.name << " " ;
    else
	os << "? " ;
   }
  os << k.no << "/" << k.year << " ã." ;
  return os ;
 }

bool TRCDKey_Array::Contains(const TRCDKey_ &key) const
{
	int i;

	for (i = 0; i < Count(); i++)
		if (*ItemAt(i) == key)
			break;

	return i < Count();
}

void TRCDKeyContainer_ :: Clear ()
 {
  TRCDKey_ :: Clear () ;
  kind = '\0' ;
 }  // TRCDKeyContainer_ :: Clear

TRCDKeyContainer_ :: TRCDKeyContainer_ ()
 {
  Clear () ;
 }  // TRCDKeyContainer_ :: TRCDKeyContainer_

TRCDKeyContainer_ :: TRCDKeyContainer_ (const TRCDKey & key,
                                        const char kind_) :
                       TRCDKey_ (key), kind (kind_)
 {
 }  // TRCDKeyContainer_ :: TRCDKeyContainer_

TRCDKeyContainer_ :: TRCDKeyContainer_ (const TRCDKeyContainer_ & source)
 {
  (* this) = source ;
 }  // TRCDKeyContainer_ :: TRCDKeyContainer_

TRCDKeyContainer_ :: TRCDKeyContainer_ (const TRCDKeyContainer & source) :
                       TRCDKey_ (source.key), kind (source.kind)
 {
 }  // TRCDKeyContainer_ :: TRCDKeyContainer_

TRCDKeyContainer_ :: TRCDKeyContainer_ (char type_, long int no_,
                                        long int year_, char kind_) :
                       TRCDKey_ (type_, no_, year_), kind (kind_)
 {
 }  // TRCDKeyContainer_ :: TRCDKeyContainer_

const TRCDKeyContainer_ &
TRCDKeyContainer_ :: operator = (const TRCDKeyContainer_ & source)
 {
  TRCDKey_ :: operator = (source) ;
  kind = source.kind ;
  return (* this) ;
 }  // TRCDKeyContainer_ :: operator =

bool TRCDKeyContainer_ :: IsEmpty () const
 {
  TRCDKeyContainer_ empty ;

  return (* this) == empty ;
 }  // TRCDKeyContainer_ :: IsEmpty

bool TRCDKeyContainer_ :: FilledOK () const
 {
  return TRCDKey_ :: FilledOK () && kind != '\0' &&
         kind2type (kind) == type ;
 }  // TRCDKeyContainer_ :: FilledOK

bool TRCDKeyContainer_ :: operator==(const TRCDKeyContainer_ &other) const
 {
  return TRCDKey_ :: operator == (other) && kind == other.kind ;
 }  // TRCDKeyContainer_ :: operator

char * TRCDKeyContainer_ :: Print (char * const s, const size_t sSz) const
 {
  if (s && sSz)
   {
    ostrstream (s, sSz) << (* this) << ends ;
    s [sSz - 1] = '\0' ;
   }
  return s ;
 }  // TRCDKeyContainer_ :: Print

char* TRCDKeyContainer_::PrintAlias(char* const s, const size_t sSz) const
{
  if(s && sSz)
  {
    {
      ostrstream oss(s, sSz);

      if(FilledOK())
      {
        TKind& t = *Kind;

        t.kind = kind;
        if(t.kind && t.Try())
          oss << (t.alias[0] ? t.alias : t.name) << " ";
        else
          oss << "? ";
      }
      else
        oss << "? ";
      oss << "No ";
      oss << no << "/" << year << " ã." << ends;
    }
    s[sSz - 1] = '\0';
  }
  return s ;
}  // TRCDKeyContainer_::PrintAlias

ostream & operator << (ostream & os, const TRCDKeyContainer_ & k)
 {  // An old variant in VersionArchives\1232\_appname.j --> _data.cpp
  if (k.FilledOK ())
   {
    TKind & t = * Kind ;

    t.kind = k.kind ;
    if (t.kind && t.Try ())
      os << t.name << " " ;
    else
      os << "? " ;
   }
  else
    os << "? " ;
  os << "No " ;  // 16.nov.2001
  os << k.no << "/" << k.year << " ã." ;
  return os ;
 }

void Trade634DlgData :: Clear ()
 {
  clear_CDate (begD) ;
  clear_CDate (endD) ;
  lawKey.Clear () ;
  begSerialNo = 0 ;
  truncChainAtEndD = false ;
 }  // Trade634DlgData :: Clear

Trade634DlgData :: Trade634DlgData ()
 {
  Clear () ;
 }  // Trade634DlgData :: Trade634DlgData

void PersonListDlgData :: Clear ()
 {
  emptyRowsAtEnd = 0 ;
  whatToPrint = NotifiedsOnly ;  // allSides = false ;  // 19.dec.2001
  sentList = mergeCitizenNotifications = followColumnNames = false ;
  addLetter = false;  // 20070726
  suppressDates = false ;
 }  // PersonListDlgData :: Clear

PersonListDlgData :: PersonListDlgData ()
 {
  Clear () ;
 } ;  // PersonListDlgData :: PersonListDlgData

void AloneDatedContainer_xK :: Clear ()
 {
  clearGroupData (* this) ;
 }  // AloneDatedContainer_xK :: Clear

AloneDatedContainer_xK :: AloneDatedContainer_xK (const char * name) :
  TDatedContainer (name ? name : "ALONE_DATED_CONTAINER_XK", NULL)
 {
  Clear () ;
 }  // AloneDatedContainer_xK :: AloneDatedContainer_xK

void LinkedXKXKDatedContainer :: Clear ()
 {
  link.Clear () ;
  clearGroupData (* this) ;
 }  // LinkedXKXKDatedContainer :: Clear

LinkedXKXKDatedContainer :: LinkedXKXKDatedContainer (const char * name) :
  AloneDatedContainer_xK (name ? name : "LINKED_XKXK_DATED_CONTAINER")
 {
  Clear () ;
 }  // LinkedXKXKDatedContainer :: LinkedXKXKDatedContainer

void LinkedXKXKDatedContainerSK :: Clear ()
 {
  LinkedXKXKDatedContainer :: Clear () ;
  sessKind = '\0' ;
  clear_CDate (linkImportantDate) ;  // 19.dec.2000
  surround.Clear () ;
 }  // LinkedXKXKDatedContainerSK :: Clear

LinkedXKXKDatedContainerSK :: LinkedXKXKDatedContainerSK (const char * name) :
  LinkedXKXKDatedContainer (name ? name : "LINKED_XKXK_DATED_CONTAINER_SK")
 {
  Clear () ;
 }  // LinkedXKXKDatedContainer :: LinkedXKXKDatedContainer

CDate LinkedXKXKDatedContainerSK :: ChainDate () const
 {
  return surround.date.Empty () ? date : surround.date ;
 }  // LinkedXKXKDatedContainerSK :: ChainDate

bool LinkedXKXKDatedContainerSK :: operator==(const LinkedXKXKDatedContainerSK &other) const
 {
  bool res =
          (TRCDKey_ (key) == TRCDKey_ (other.key) &&
          date == other.date &&
          surround.date == other.surround.date &&
          (TRCDKey_) (surround.key) == (TRCDKey_) (other.surround.key) &&
          surround.kind == other.surround.kind &&
          sessKind == other.sessKind &&
          link.kind == other.link.kind &&
          kind == other.kind) ;
  if (res)  // 22.jan.2001 - begin
   {  // To sort the surrounds, linked to the same session, by no
    const bool isSurround = is_surround_kind (link.kind) ;
    const bool otherIsSurround = is_surround_kind (other.link.kind) ;

    if (isSurround == true && otherIsSurround == true)
      if (link.key.no != other.link.key.no)
        res = false ;
   }  // 22.jan.2001 - end
  if (res)  // 01.feb.2001 - begin
   {  // To sort the inregs, linked to the same session, by
      // date and no
    const bool isInReg = is_inreg_kind (link.kind) ;
    const bool otherIsInReg = is_inreg_kind (other.link.kind) ;

    if (isInReg == true && otherIsInReg == true)
     {
      if (link.date != other.link.date)
        res = false ;
      if (res)
        if (link.key.no != other.link.key.no)
          res = false ;
     }
   }  // 01.feb.2001 - end
  return res ;
 }  // LinkedXKXKDatedContainerSK :: operator ==

bool LinkedXKXKDatedContainerSK :: operator<(const LinkedXKXKDatedContainerSK &other) const
 {
// Lawsuits' keys separation
  if (key.type < other.key.type)
    return true ;
  if (key.type > other.key.type)
    return false ;
  if (key.no < other.key.no)
    return true ;
  if (key.no > other.key.no)
    return false ;
  if (key.year < other.key.year)
    return true ;
  if (key.year > other.key.year)
    return false ;

  // Chains' dates separation
  if (ChainDate () < other.ChainDate ())
    return true ;
  if (ChainDate () > other.ChainDate ())
    return false ;

   {  // This is to separate sequences with surrounds and
      // the sequences without surrounds
    const CDate surroundDate = surround.date.Empty () ?
					     DATE_MAX : surround.date ;
    const CDate otherSurroundDate = other.surround.date.Empty () ?
                                        DATE_MAX : other.surround.date ;
                                        
    if (surroundDate < otherSurroundDate)
      return true ;
    if (surroundDate > otherSurroundDate)
      return false ;
   }

  if (surround.key.year < other.surround.key.year)
    return true ;
  if (surround.key.year > other.surround.key.year)
    return false ;
  if (surround.key.no < other.surround.key.no)
    return true ;
  if (surround.key.no > other.surround.key.no)
    return false ;
  if (surround.key.type < other.surround.key.type)
    return true ;
  if (surround.key.type > other.surround.key.type)
    return false ;
  if (surround.kind < other.surround.kind)
    return true ;
  if (surround.kind > other.surround.kind)
    return false ;  

  if (sessKind < other.sessKind)
    return true ;
  if (sessKind > other.sessKind)
    return false ;
   {
    // This is to pull ahead the surrounds in the current sequence
    const bool isSurround = is_surround_kind (link.kind) ;
    const bool otherIsSurround = is_surround_kind (other.link.kind) ;

    if (isSurround == true && otherIsSurround == false)
      return true ;
    if (isSurround == false && otherIsSurround == true)
      return false ;
    if (isSurround == true && otherIsSurround == true) //22.jan.2001-begin
     {  // To sort the surrounds, linked to the same session, by no
      if (link.key.no < other.link.key.no)
        return true ;
      if (link.key.no > other.link.key.no)
        return false ;
     }  // 22.jan.2001 - end
   }

   {  // Sessions next
    const bool isSess = is_session_kind (link.kind) ;
    const bool otherIsSess = is_session_kind (other.link.kind) ;

    if (isSess == true && otherIsSess == false)
      return true ;
    if (isSess == false && otherIsSess == true)
      return false ;
   }
   {  // Then decisions 
    const bool isDec = is_decision_kind (link.kind) ;
    const bool otherIsDec = is_decision_kind (other.link.kind) ;

    if (isDec == true && otherIsDec == false)
      return true ;
    if (isDec == false && otherIsDec == true)
      return false ;
   }
   {  // Then inRegs 
    const bool isInReg = is_inreg_kind (link.kind) ;
    const bool otherIsInReg = is_inreg_kind (other.link.kind) ;

    if (isInReg == true && otherIsInReg == false)
      return true ;
    if (isInReg == false && otherIsInReg == true)
      return false ;
    if (isInReg == true && otherIsInReg == true)  // 01.feb.2001 - begin
     {  // To sort the inregs, linked to the same session, by
        // date and no
      if (link.date < other.link.date)
        return true ;
      if (link.date > other.link.date)
        return false ;
      if (link.key.no < other.link.key.no)
        return true ;
      if (link.key.no > other.link.key.no)
        return false ;
     }  // 01.feb.2001 - end
   }
  // At the end - outregs and may be any unknown yet
  return kind > other.kind ;  // Especially, zero will be last
 }  // LinkedXKXKDatedContainerSK :: operator <

TL634AloneDatedContainer :: TL634AloneDatedContainer (const char * name) :
  AloneDatedContainer_xK (name ? name : "TL634_ALONE_DATED_CONTAINER")
 {
 }  // TL634AloneDatedContainer :: TL634AloneDatedContainer

bool TL634AloneDatedContainer :: operator==(const TL634AloneDatedContainer &other) const
 {
  return conteq(this, &other);
 }  // TL634AloneDatedContainer :: operator ==

bool TL634AloneDatedContainer :: operator<(const TL634AloneDatedContainer &other) const
 {
  bool res ;

  if (date < other.date)
    res = true ;
  else
    if (date > other.date)
	res = false ;
    else // I hope, the lines below are not too important in this class...
	if (key.year < other.key.type)
	  res = true ;
	else
	  if (key.year > other.key.type)
          res = false ;
        else
          if (key.year < other.key.year)
            res = true ;
          else
            if (key.year > other.key.year)
		  res = false ;
            else
              if (key.no < other.key.no)
                res = true ;
              else
                if (key.no > other.key.no)
                  res = false ;
                else
                      res = kind < other.kind ;
  return res ;
 }  // TL634AloneDatedContainer :: operator <

IORegDatedContainer :: IORegDatedContainer (const char * name) :
  AloneDatedContainer_xK (name ? name : "IOREG_DATED_CONTAINER")
 {
 }  // IORegDatedContainer :: IORegDatedContainer

bool IORegDatedContainer :: operator==(const IORegDatedContainer &other) const
 {
  return TRCDKey_ (key) == TRCDKey_ (other.key) &&
	   date == other.date && kind == other.kind ;
 }  // IORegDatedContainer :: operator ==

bool IORegDatedContainer :: operator<(const IORegDatedContainer &other) const
 {
  if (key.type < other.key.type)
    return true ;
  else
    if (key.type > other.key.type)
	return false ;
    else
	if (key.year < other.key.year)
	  return true ;
	else
	  if (key.year > other.key.year)
          return false ;
        else
          if (key.no < other.key.no)
            return true ;
          else
            if (key.no > other.key.no)
              return false ;
            else
              if (date < other.date)
                return true ;
              else
                if (date > other.date)
                  return false ;
                else
                  return kind < other.kind ;
 }  // IORegDatedContainer :: operator <
 
void DatedContainer_ :: Clear ()
 {
  TRCDKey_ :: Clear () ;
  kind = '\0' ;
  date.Clear() ;
 }  // DatedContainer_ :: Clear

DatedContainer_ :: DatedContainer_ () : TRCDKey_ ()
 {
  Clear () ;
 }  // DatedContainer_ :: DatedContainer_

DatedContainer_ :: DatedContainer_ (const TRCDKeyContainer & source) :
			   TRCDKey_ (source.key),
                     kind (source.kind), date (source.date)
 {
 }  // DatedContainer_ :: DatedContainer_

DatedContainer_ :: DatedContainer_ (const DatedContainer_ & source) :
                     TRCDKey_ ()
 {
  (* this) = source ;
 }  // DatedContainer_ :: DatedContainer_

DatedContainer_ :: DatedContainer_ (char type_, long int no_,
                                    long int year_, char kind_,
                                    CDate date_) :
                     TRCDKey_ ()
 {
  type = type_ ;
  year = year_ ;
  no = no_ ;
  kind = kind_ ;
  date = date_ ;
 }  // DatedContainer_ :: DatedContainer_

DatedContainer_ :: DatedContainer_ (const TRCDKey key_, char kind_,
                                    CDate date_) :
                     TRCDKey_ (key_)
 {  //19.jun.2003
  kind = kind_ ;
  date = date_ ;
 }  // DatedContainer_ :: DatedContainer_

const DatedContainer_ &
DatedContainer_ :: operator = (const DatedContainer_ & source)
 {
  type = source.type ;
  year = source.year ;
  no = source.no ;
  kind = source.kind ;
  date = source.date ;
  return (* this) ;
 }  // DatedContainer_ :: operator =

bool DatedContainer_ :: IsEmpty () const
 {
  DatedContainer_ tmp ;

  return (* this) == tmp ;
 }  // DatedContainer_ :: IsEmpty

bool DatedContainer_ :: FilledOK () const
 {
  return TRCDKey_ :: FilledOK () && kind && date.Valid () ;
 }  // DatedContainer_ :: FilledOK

char * DatedContainer_ :: Print (char * const s, const size_t sSz) const
 {
  if (s && sSz)
   {
    ostrstream (s, sSz) << (* this) << ends ;
    s [sSz - 1] = '\0' ;
   }
  return s ;
 }  // DatedContainer_ :: Print

bool DatedContainer_ :: operator==(const DatedContainer_ &other) const
{
  return TRCDKey_ :: operator == (other) &&
	   kind == other.kind && date == other.date ;
 }  // DatedContainer_ :: operator ==

ostream & operator << (ostream & os, const DatedContainer_ & dC)
 {  // An old variant in VersionArchives\1232\_appname.j --> _data.cpp
  os << TRCDKey_ (dC) ;
  os << ", " ;
   {
    TKind & k = * Kind ;

    k.kind = dC.kind ;
    if (k.kind && k.Try ())
      os << k.name ;
    else
      os << "?" ;
   }
  os << " îò " << dC.date ;
  os << " ã." ;  // 27.may.2002
  return os ;
 }  // ostream & operator << (ostream & os, const DatedContainer_ & k)

void WEB_CheckupDlgData :: Clear ()
 {
  lawKinds [0] = '\0' ;
  begDate = CDate () ;
  endDate = begDate ;
  decisionsNeed = false ;
  outFileName [0] = '\0' ;
  police = false ;  // 29.jan.2001
  socialInterest = false ;  // 01.mar.2004
  withJudgeNames = true;
  orderByDate1st = true;
 }  // WEB_CheckupDlgData :: Clear

WEB_CheckupDlgData :: WEB_CheckupDlgData ()
 {
  Clear () ;
 }  // WEB_CheckupDlgData :: WEB_CheckupDlgData

void TRCDKeyContainerD_ :: Clear ()
 {
  TRCDKeyContainer_ :: Clear () ;
  clear_CDate (date) ;
 }  // TRCDKeyContainerD_ :: Clear

TRCDKeyContainerD_ :: TRCDKeyContainerD_ ()
 {
  Clear () ;
 }  // TRCDKeyContainerD_ :: TRCDKeyContainerD_

TRCDKeyContainerD_ :: TRCDKeyContainerD_ (const TRCDKey & key,
                                          const char kind_,
                                          const CDate date_) :
                        TRCDKeyContainer_ (key, kind_), date (date_)
 {
 }  // TRCDKeyContainerD_ :: TRCDKeyContainerD_

TRCDKeyContainerD_ :: TRCDKeyContainerD_ (const TRCDKeyContainerD_ & source)
 {
  (* this) = source ;
 }  // TRCDKeyContainerD_ :: TRCDKeyContainerD_

TRCDKeyContainerD_ :: TRCDKeyContainerD_ (char type_, long int no_,
							long int year_, char kind_,
							const CDate date_) :
				TRCDKeyContainer_ (type_, no_, year_, kind_),
				date (date_)
 {
 }  // TRCDKeyContainerD_ :: TRCDKeyContainerD_

const TRCDKeyContainerD_ &
  TRCDKeyContainerD_ :: operator = (const TRCDKeyContainerD_ & source)
 {
  TRCDKeyContainer_ :: operator = (source) ;
  date = source.date ;
  return (* this) ;
 }  // TRCDKeyContainerD_ :: operator =

bool TRCDKeyContainerD_ :: IsEmpty () const
 {
  return TRCDKeyContainer_ :: IsEmpty () && date.Empty () ;
 }  // TRCDKeyContainerD_ :: IsEmpty

bool TRCDKeyContainerD_ :: FilledOK () const
 {
  return TRCDKeyContainer_ :: FilledOK () && date.Valid () ;
 }  // TRCDKeyContainerD_ :: FilledOK

char * TRCDKeyContainerD_ :: Print (char * const s,
                                    const size_t sSz) const
 {
  if (s && sSz)
   {
    ostrstream (s, sSz) << (* this) << ends ;
    s [sSz - 1] = '\0' ;
   }
  return s ;
 }  // TRCDKeyContainerD_ :: Print

ostream & operator << (ostream & os, const TRCDKeyContainerD_ & k)
 {
  os << TRCDKeyContainer_ (k) << " îò " << k.date ;
  return os ;
 }

void sessLstData :: addNewItemsToGroup ()
 {  // 31.may.2001
  Add( new TTime ("F_TIME", & time)) ;
 }  // sessLstData :: addNewItemsToGroup

void sessLstData :: clearData ()
 {
  clearGroupData (* this) ;  // clearSessionData (* this) ;  31.may.2001
  number = row = 0 ;
 }  // sessLstData :: clearData

sessLstData :: sessLstData () :
                 AloneDatedContainer_xK ()  // TSession ()  31.may.2001
 {
  addNewItemsToGroup () ;  // 31.may.2001
  clearData () ;
 }  // sessLstData :: sessLstData ()

sessLstData :: sessLstData (TSession & s, int num, int r) :
                 AloneDatedContainer_xK ()
 {  // 31.may.2001
  addNewItemsToGroup () ;
  clearData () ;
  (* this) << s ;
  number = num ;
  row = r ;
 }  // sessLstData :: sessLstData (TSession & s, int num, int r)

sessLstTimeSortData :: sessLstTimeSortData () :
                         sessLstData ()  // TSession ()  31.may.2001
 {
 }  // sessLstTimeSortData :: sessLstTimeSortData

bool sessLstTimeSortData :: operator==(const sessLstTimeSortData &other) const
 {
  return (time.hour == other.time.hour &&
	    time.minute == other.time.minute &&
	    key.year == other.key.year &&
	    key.no == other.key.no &&
	    key.type == other.key.type) ;
 }  // sessLstTimeSortData :: operator ==

bool sessLstTimeSortData :: operator<(const sessLstTimeSortData &other) const
 {
  bool res = false ;

  if (time.hour < other.time.hour)
    res = true ;
  else
    if (time.hour == other.time.hour &&
	  time.minute < other.time.minute)
	res = true ;
    else
	if (time.hour == other.time.hour &&
	    time.minute == other.time.minute &&
	    key.year < other.key.year)
	  res = true ;
	else
	  if (time.hour == other.time.hour &&
		time.minute == other.time.minute &&
		key.year == other.key.year &&
		key.no < other.key.no)
	    res = true ;
	  else
	    if (time.hour == other.time.hour &&
		  time.minute == other.time.minute &&
		  key.year == other.key.year &&
		  key.no == other.key.no &&
		  key.type < other.key.type)
		res = true ;
  return res ;
 }  // sessLstTimeSortData :: operator <

void DoubleNos :: Clear ()
 {
  no0 = no1 = 0 ;
 } ;  // DoubleNos :: Clear

DoubleNos :: DoubleNos ()
 {
  Clear () ;
 }  // DoubleNos :: DoubleNos

const DoubleNos & DoubleNos :: operator = (const DoubleNos & src)
 {
  no0 = src.no0 ;
  no1 = src.no1 ;
  return * this ;
 }  // DoubleNos :: operator =

void DatedSide_ :: Clear ()
 {
  DatedContainer_ :: Clear () ;
  ucnType = '\0' ;
  setmem (ucn, sizeof (ucn), '\0') ;
  involvement = '\0' ;
 }  // DatedSide_ :: Clear

DatedSide_ :: DatedSide_ () : DatedContainer_ ()
 {
  Clear () ;
 }  // DatedSide_ :: DatedSide_ ()

DatedSide_ :: DatedSide_ (const TSideWind & source) :
		    DatedContainer_ (source.key.type, source.key.no,
					   source.key.year,
					   source.kind, source.date),
		    ucnType (source.ucnType), involvement (source.involvement)
 {
  ostrstream (ucn, sizeof (ucn)) << source.ucn << ends ;
  ucn [sizeof (ucn) - 1] = '\0' ;
 }  // DatedSide_ :: DatedSide_ (const TDatedSide & source) ;

DatedSide_ :: DatedSide_ (const DatedSide_ & source) : DatedContainer_ ()
 {
  (* this) = source ;
 }  // DatedSide_ :: DatedSide_ (const TDatedSide_ & source)

DatedSide_ :: DatedSide_ (char type_, long int no_, long int year_,
                          char kind_, CDate date_, char ucnType_,
                          const char * ucn_, char involvement_) :
                DatedContainer_ (type_, no_, year_, kind_, date_),
                ucnType (ucnType_), involvement (involvement_)
 {
  ostrstream (ucn, sizeof (ucn)) << ucn_ << ends ;
  ucn [sizeof (ucn) - 1] = '\0' ;
 }  // DatedSide_ :: DatedSide_

const DatedSide_ & DatedSide_ :: operator = (const DatedSide_ & source)
 {
  type = source.type ;
  no = source.no ;
  year = source.year ;
  kind = source.kind ;
  date = source.date ;
  ucnType = source.ucnType ;
  involvement = source.involvement ;
  ostrstream (ucn, sizeof (ucn)) << source.ucn << ends ;
  ucn [sizeof (ucn) - 1] = '\0' ;
  return (* this) ;
 }  // DatedSide_ :: operator = (const DatedSide_ & source)

bool DatedSide_ :: IsEmpty () const
 {
  bool res = DatedContainer_ :: IsEmpty () ;

  if (res)
    if (ucnType || ucn [0] || involvement)
      res = false ;
  return res ;
 }  // DatedSide_ :: IsEmpty

bool DatedSide_ :: FilledOK () const
 {
  bool res = DatedContainer_ :: FilledOK () ;

  if (res)
    if (ucnType == '\0' || ucn [0] == '\0' || involvement == '\0')
      res = false ;
  return res ;
 }  // DatedSide_ :: FilledOK

char * DatedSide_ :: Print (char * const s, const size_t sSz) const
 {  // returns s
  if (s && sSz)
   {
    ostrstream (s, sSz) << (* this) << ends ;
    s [sSz - 1] = '\0' ;
   }
  return s ;
 }  // DatedSide_ :: Print

bool DatedSide_ :: operator==(const DatedSide_ &other) const
 {
  const DatedContainer_ othDC (other) ;
  bool res = DatedContainer_ :: operator == (othDC) ;

  if (res)
    if (ucnType != other.ucnType ||
	  strcmp (ucn, other.ucn) != 0 ||
	  involvement != other.involvement)
	res = false ;
  return res ;
 }  // DatedSide_ :: operator == (const DatedSide_ & other) const

bool DatedSide_ :: operator!=(const DatedSide_ &other) const
 {
  return (((* this) == other) == false) ;
 }

ostream & operator << (ostream & os, const DatedSide_ & dS)
 {
  if (dS.FilledOK ())
   {
    os << dS.ucn << " " ;
     {
      char buff [256] ;

      ucnAndUcnTypeToName (dS.ucn, dS.ucnType, buff, sizeof (buff)) ;
      os << buff << " " ;
      involvementToString (dS.involvement, buff, sizeof (buff)) ;
      os << buff << " " ;
     }
   }
  os << DatedContainer_ (dS) ;
  return os ;
 }  // operator << (ostream & os, const DatedSide_ & dC)

// 11.jan.2002 - begin 
OutRegDatedContainer :: OutRegDatedContainer (const char * name) :
  IORegDatedContainer (name ? name : "OUTREG_DATED_CONTAINER")
 {
  Add(
    + new TStringAlias ("F_RECEIVER", receiver, sizeof (receiver)),
    + new TLong ("F_IN_NO", & inNo),
    + new TLong ("F_IN_YEAR", & inYear),
    + new TCharAlias ("F_RESULT", & result),
    + new TDate ("F_RETURNED", & returned, false),
    + new TCharAlias ("F_RESULT_I", & resultI),
    + new TCharAlias ("F_RESULT_II", & resultII),
    + new TCharAlias ("F_RESULT_KS", & resultSuper),
    + new TCharAlias ("F_RESULT_MJ", & resultMJ),
    NULL);
  Clear () ;
 } ;  // OutRegDatedContainer :: OutRegDatedContainer

void OutRegDatedContainer :: Clear ()
 {
  clearGroupData (* this) ;
 }  // OutRegDatedContainer :: Clear

// 11.jan.2002 - end

Kind_ :: Kind_ (const char k_) : k (k_)
 {
 }  // Kind_ :: Kind_

void Kind_ :: Clear ()
 {
  k = '\0' ;
 }  // Kind_ :: Clear

ostream & operator << (ostream & os, const Kind_ & k)
 {  // An old variant in VersionArchives\1232\_appname.j --> _data.cpp
  if (k.k)
   {
    TKind & tK = * Kind ;

    tK.kind = k.k ;
    if (tK.Try ())
      os << tK.name ;
   }
  return os ;
 }  // Kind_ :: operator <<

Type_ :: Type_ (const char t_) : t (t_)
 {
 }  // Type_ :: Type_

void Type_ :: Clear ()
 {
  t = '\0' ;
 }  // Type_ :: Clear

ostream & operator << (ostream & os, const Type_ & t)
 {  // An old variant in VersionArchives\1232\_appname.j --> _data.cpp
  if (t.t)
   {
    TType & tT = * Type ;

    tT.type = t.t ;
    if (tT.Try ())
      os << tT.name ;
   }
  return os ;
 }  // Type_ :: operator <<

Result_ :: Result_ (const char r_) : r (r_)
 {
 }  // Result_ :: Result_

void Result_ :: Clear ()
 {
  r = '\0' ;
 }  // Result_ :: Clear

ostream & operator << (ostream & os, const Result_ & r)
 {  // An old variant in VersionArchives\1232\_appname.j --> _data.cpp
  if (r.r)
   {
    Result->result = r.r ;
    if (Result->Try ())
	os << Result->name ;
   }
  return os ;
 }  // Result_ :: operator <<

Resolution_ :: Resolution_ (const char r_) : r (r_)
 {
 }  // Resolution_ :: Resolution_

void Resolution_ :: Clear ()
 {
  r = '\0' ;
 }  // Resolution_ :: Clear

ostream & operator << (ostream & os, const Resolution_ & r)
 {
  if (r.r)
   {
    TResolution & tR = * Resolution ;

    tR.resolution = r.r ;
    if (tR.Try ())
      os << tR.name ;
   }
  return os ;
 }  // Resolution_ :: operator <<

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
BankruptcyWideDlgData :: BankruptcyWideDlgData ()
 {
  Clear () ;
 }  // BankruptcyWideDlgData :: BankruptcyWideDlgData

void BankruptcyWideDlgData :: Clear ()
 {
  wantNoncompleted = wantCompleted = false ;
  lawBegDate = lawEndDate =
  completedBegDate = completedEndDate = CDate () ;
  wantExtraTitle = false ;
  setmem (yourOutNo, sizeof (yourOutNo), '\0') ;
  wantNoncompletedWeb = false ;  // 10.jun.2003 - begin
  wantCompletedWeb = false ;
  setmem(noncompletedWebPath, sizeof(noncompletedWebPath), 0) ;
  setmem(completedWebPath, sizeof(completedWebPath), 0) ;  // 10.jun.2003 - end
 }  // BankruptcyWideDlgData :: Clear

#   endif  // of APPEAL
# endif  // of INSTANCE

UcnLng::UcnLng()
{
	Clear();
}  // UcnLng::UcnLng

UcnLng::UcnLng(const char* const u, const long l, const CDate d_)
{
	Clear();
	if(u)
	{
		ostrstream(ucn, sizeof(ucn)) << u << ends;
		ENDING_ZERO(ucn);
	}
	else
		error("%s\n%s::%s", WRONG_ARGS_PASSED_TO,
				typeid(*this).name(), typeid(*this).name());
	lng = l;
	d = d_;
}  // UcnLng::UcnLng(const char* const u, const long l)

void UcnLng::Clear()
{
	ucn[0] = '\0';
	lng = 0;
	clear_CDate(d);
}  // UcnLng::Clear

UcnLng& UcnLng::operator = (const UcnLng& src)
{
  if(this != &src)
  {
    ostrstream(ucn, sizeof(ucn)) << src.ucn << ends;
    ENDING_ZERO(ucn);
    lng = src.lng;
    d = src.d;
  }
  return *this;
}  // UcnLng::operator =

bool UcnLng::operator == (const UcnLng& src) const
{
  return (strcmp(ucn, src.ucn) == 0 && lng == src.lng);
}  // UcnLng::operator ==

TMaxLongGroup::TMaxLongGroup()
{
	Add(new TLong("MAX", &max));
}

TCountLongGroup::TCountLongGroup()
{
	Add(new TLong("COUNT", &count));
}

// 20051010  -->
# if ! INSTANCE  // Regional court only
void ZZDN_Dlg_data :: clear ()
 {
  selectCrit1 = true;

  FromNo = 0 ;
  ToNo   = 0 ;
  Year   = 0 ;

  setmem (& begD, sizeof (begD), '\0') ;
  setmem (& endD, sizeof (endD), '\0') ;
  begN = 0 ;
 }  // ZZDN_Dlg_data :: clear

const ZZDN_Dlg_data & ZZDN_Dlg_data :: operator =
    (const ZZDN_Dlg_data & source)
 {
  clear () ;

  selectCrit1 = source.selectCrit1;

  FromNo = source.FromNo;
  ToNo   = source.ToNo;
  Year   = source.Year;

  begD = source.begD ;
  endD = source.endD ;
  begN = source.begN ;

  return (* this) ;
 }  // const ZZDN_Dlg_data & ZZDN_Dlg_data :: operator =
    //    (const ZZDN_Dlg_data & source)
# endif  // Regional court only
// 20051010 <--
