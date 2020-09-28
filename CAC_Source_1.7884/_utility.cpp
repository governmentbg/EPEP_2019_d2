#include "cac.h"  // 18.may.2001

void mkStrBlank (char * s, size_t sSz)
 {
  if (! sSz)
    return ;
  if (sSz > 1)
    setmem (s, sSz - 1, ' ') ;
  s [sSz - 1] = '\0' ;
 }  // mkStrBlank

void writeString (ostream & os, const char * s, int w,
			int align, size_t from)
 {
  const size_t l = strlen (s) ;
  bool oriLeftFlag = ((os.flags () & ios :: left) == ios :: left) ;

  if (from > l)
    from = l ;   // at the '\0'

  const char * const beginning = s + from ;  // 03.dec.2001 - begin
  const size_t bL = strlen (beginning) ;

  if (align == JUSTIFY_ALIGN)
    if (bL < (w * 2) / 3)
      align = LEFT_ALIGN ;  // 03.dec.2001 - end
  switch (align)
   {
    case CENTER_ALIGN :
    case LEFT_ALIGN :
      if (! oriLeftFlag)
        os << setiosflags (ios :: left) ;
      break ;
    default :
      break ;
   }
   {  // -- 0 --
//    const char * const beginning = s + from ;  // 03.dec.2001
//    const size_t bL = strlen (beginning) ;  // 03.dec.2001

    if (bL <= w)
     {  // -- 1 --
      if (w < WRITE_STRING_LEN_AS_IS)  // (w < 255)  // 29.oct.2001
       {  // -- 2 --
        if (align != JUSTIFY_ALIGN)  // 29.nov.2001
         {
          if (align != CENTER_ALIGN)
            os << setw (w) ;
          else
           {  // -- 3 --
            int i ;
            const size_t lead = (w - bL) / 2 ;

            for (i = 0 ; i < lead ; i ++)
              os << " " ;
            os << setw (w - lead) ;
           }  // -- 3 --
         }
       }  // -- 2 --
      if (align == JUSTIFY_ALIGN)  // 23.nov.2001 - begin
        os << JustifiedString (beginning, w) ;
      else  // 23.nov.2001 - end
        os << beginning ;
     }  // -- 1 --
    else
     {
      for (int i = 0 ; i < w ; i ++)
        os << beginning [i] ;
     }
   }  // -- 0 --
  switch (align)
   {
    case CENTER_ALIGN :
    case LEFT_ALIGN :
      if (! oriLeftFlag)
        os << resetiosflags (ios :: left) ;
      break ;
    default :
      break ;
   }
 }  // writeString

void writeDoubleStrikedString (ostream & os, const char * s, int w,
					 int align, size_t from,
					 const bool upCCyr)
 {
   {  // -- 0 --
    const char * const beginning = s + from ;
    const size_t bL = strlen (beginning) ;

    if (align == JUSTIFY_ALIGN)  // 23.nov.2001
      align = LEFT_ALIGN ;  // 23.nov.2001
    if (bL <= w)
     {  // -- 1 --
      if (w < WRITE_STRING_LEN_AS_IS)  // (w < 255)  // 29.oct.2001
       {  // -- 2 --
        const size_t blanksCnt = w - bL ;
        size_t leadingBlanksCnt = 0 ;
        size_t endingBlanksCnt = 0 ;

        switch (align)
         {
          case LEFT_ALIGN :
            endingBlanksCnt = blanksCnt ;
            break ;
          case RIGHT_ALIGN :
            leadingBlanksCnt = blanksCnt ;
            break ;
          case CENTER_ALIGN :
            leadingBlanksCnt = blanksCnt / 2 ;
            endingBlanksCnt = blanksCnt - leadingBlanksCnt ;
            break ;
         }  // switch
        os << setw (leadingBlanksCnt) << ""
           << doubleStrikedString (beginning, upCCyr)
           << setw (endingBlanksCnt) << "" ;
       }  // -- 2 --
      else
        os << doubleStrikedString (beginning, upCCyr) ;
     }  // -- 1 --
    else
      os << doubleStrikedString (beginning, upCCyr) ;
   }  // -- 0 --
 }  // writeDoubleStrikedString

void wrtLng (ostream & os, const unsigned long int n, int w,
             int align = RIGHT_ALIGN, bool notSigned = true)
 {
  bool oriLeftFlag = ((os.flags () & ios :: left) == ios :: left) ;

  if (align == JUSTIFY_ALIGN)  // 23.nov.2001
    align = RIGHT_ALIGN ;  // 23.nov.2001
  switch (align)
   {
    case LEFT_ALIGN :
      if (! oriLeftFlag)
        os << setiosflags (ios :: left) ;
      break ;
    default :
      break ;
   }
  os << setw (w) ;
  if (notSigned)
    os << n ;
  else
    os << ((const signed long int) n) ;
  switch (align)
   {
    case LEFT_ALIGN :
      if (! oriLeftFlag)
        os << resetiosflags (ios :: left) ;
      break ;
    default :
      break ;
   }
 }  // wrtLng

void writeLong (ostream & os, const long int n, int w, int align)
 {
  wrtLng (os, n, w, align, false) ;
 }  // writeLong

void writeULong (ostream & os, const unsigned long int n, int w,
		     int align)
 {
  wrtLng (os, n, w, align) ;
 }  // writeULong

void writeDate (ostream & os, const CDate d, int w, int align,
		    const size_t from)
 {
  char dS [32] ;

  dS [0] = '\0' ;
  if (align == JUSTIFY_ALIGN)  // 23.nov.2001
    align = LEFT_ALIGN ;  // 23.nov.2001
  if (! d.Empty ())
    CDateToString (d, dS, sizeof (dS)) ;
  writeString (os, dS, w, align, from) ;
 }  // writeDate

void writeShortString (ostream & os, const char * str, size_t len)
 {
  const size_t l = strlen (str) ;

  if (! (len && l))
    return ;
  if (l < len)
    os << str ;
  else
   {
    writeString (os, str, len - 1) ;
    if (l > len)
	os << '.' ;
    else
	os << str [len - 1] ;
   }
 }  // writeShortString

char * intersectionOrDifference (char * set, const char * test,
					   bool action)
  // if action is true :
  // deletes from set the chars, who are not included in test
  // if action is false :
  // deletes from set the chars, who are included in test
  // returns set
 {
  if (set && test)
   {
    const unsigned tSz = strlen (set) + 1 ;
    char * tmp = new char [tSz] ;

    if (tmp && tSz > 1)
     {
	setmem (tmp, tSz, '\0') ;
	 {
	  ostrstream oss (tmp, tSz - 1) ; //--> do not replace
						    // the ending '\0'

	  for (unsigned j = 0 ; j < (tSz - 1) ; j ++)
	   {
	    const bool match = (strchr (test, set [j]) != NULL) ;

          if (match == action)
            oss << set [j] ;
         }
       }
      strcpy (set, tmp) ;
     }
    else
      set [0] = '\0' ;
    if (tmp)
      delete[] tmp ;
   }
  return set ;
 }  // intersectionOrDifference

char * intersection (char * set, const char * test)
  // deletes from set the chars, who are not included in test
  // returns set
 {
  return intersectionOrDifference (set, test, true) ;
 }  // intersection

char * difference (char * set, const char * test)
  // deletes from set the chars, who are included in test
  // returns set
 {
  return intersectionOrDifference (set, test, false) ;
 }  // difference

char * loadAnyCharCiphers (char * criteria,
				   char extractChar (TQuery &),
				   const char * alloweds)
 // Returns pointer to a newly created string if success,
 // or NULL if error
 {
  TCountRecsQuery q (criteria) ;
  const unsigned recs = q.getRecsCount () ;
  const unsigned sZ = recs + 1 ;
  char * colCiphers = NULL ;

  if (recs && extractChar)
   {
    colCiphers = new char [sZ] ;
    if (colCiphers)
     {  // -- 0 --
      unsigned i ;

      colCiphers [0] = '\0' ;
      for (i = 0 ; i < sZ ; i ++)
       {  // -- 1 --  load from base
        char a [2] ;
         a [sizeof (a) - 1] = '\0' ;
        if (q.Read ())
         {  // -- 2 --
          a [0] = extractChar (q) ;
          a [sizeof (a) - 1] = '\0' ;
          if (strlen (colCiphers) + strlen (a) < sZ)
            strcat (colCiphers, a) ;
         }  // -- 2 --
       }  // -- 1 --
      if (alloweds)
        intersection (colCiphers, alloweds) ;
     }  // -- 0 --
   }
  return colCiphers ;
 }  // loadAnyCharCiphers

char extractCollege (TQuery & q)
 {
  TCollege aCol ;

  aCol << q ;
  return aCol.college ;
 }  // extractCollege

char * loadCollegeCiphers ()
 // Returns pointer to a newly created string if success,
 // or NULL if error
 {
  return loadAnyCharCiphers ("SELECT F_COLLEGE FROM T_COLLEGE",
				     extractCollege, Default -> colleges) ;
 }  // loadCollegeCiphers

char extractComposition (TQuery & q)
 {
  TComposition aCompos ;

  aCompos << q ;
  return char (aCompos.composition) ;
 }  // extractComposition

char * loadCompositionCiphers ()
 // Returns pointer to a newly created string if success,
 // or NULL if error
 {
  return loadAnyCharCiphers ("SELECT F_COMPOSITION FROM T_COMPOSITION",
				     extractComposition,
				     Default -> compositions) ;
 }  // loadCompositionCiphers

char extractType (TQuery & q)
 {
  TType aType ;

  aType << q ;
  return aType.type ;
 }  // extractType

char * loadTypeCiphers (bool followRights)
 // Returns pointer to a newly created string if success,
 // or NULL if error
 {
  return loadAnyCharCiphers ("SELECT F_TYPE FROM T_TYPE", extractType,
				     followRights ? Default -> colleges : COLLEGE_ALLS) ;
 }  // loadTypeCiphers

char extractResult (TQuery & q)
 {  // 15.mar.2002
  *Result << q ;
  return Result->result ;
 }  // extractResult

char * loadResultCiphers ()
 // Returns pointer to a newly created string if success,
 // or NULL if error
 {  // 14.mar.2002
  return loadAnyCharCiphers ("SELECT F_RESULT FROM T_RESULT",
                             extractResult, NULL) ;
 }  // loadTypeCiphers

long int sessionsForTSCount (const char * resultsString,
				     const collCompBegDEndDBegN * cCBDEDBNPtr,
				     const char * ageString,
				     const char * pGrantChar,
				     const char * pSessKindChar,
				     bool skcEq,
				     const char * lawKindStr,
				     const char * sourceKindStr,
				     const bool followSessDate,
				     const char * const decisResultsStr,
				     const bool resultsStringResultsMatch,
				     const bool excudeAbolishedResults,
				     const bool requestCurrentlyQuick  //25.sep.2003
				    )

 {
  long int howMany = 0 ;
  const size_t crSz = 512 + 256 + 128 + 64 + 64 ;
  char * crit = new char [crSz] ;
  const char * const what = followSessDate ?
    "DISTINCT S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE"
             ", S.F_KIND"  // 26.apr.2000
     :
    "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE" ;

  if (crit && composeTBTSCriteria (crit, crSz, what,
                                   resultsString, cCBDEDBNPtr,
                                   ageString, pGrantChar,
					     pSessKindChar, skcEq, lawKindStr,
                                   sourceKindStr,
                                   followSessDate, decisResultsStr,
                                   resultsStringResultsMatch,
                                   excudeAbolishedResults
                                  ))
   {
    TQuery q (crit) ;
    //SessPtr pSess ;  // for debugging

    LawPtr pLaw ;  // 25.sep.2003
    TRequest req ;

    while (q.Read ())  // 25.sep.2003
     {
      //(* pSess) << q ;  // for debugging
      if (requestCurrentlyQuick)  // 25.sep.2003 - begin
       {
        pLaw.Ref() << q ;
        if (TRCDKey_ (pLaw.Ref ().key).FilledOK () && pLaw.Ref ().Get ())
          if(quick_law_source_currently_quick(pLaw.Ref ().source, req))
            howMany ++ ;
       }
      else  // 25.sep.2003 - end
        howMany ++ ;
     }
   }
  if (crit)
    delete[] crit ;
  return howMany ;
 }  // sessionsForTSCount

long int reportSidesCount (const char * types,
				   const long int composition,
                           const CDate * pHighRange,
                           const CDate * pLowRange,
                           const char * resultsString,
                           const char * judgedString,
                           const char * judgementKinds,
                           const char * oldString,
				   const char * punishmentString,
                           const char * recidiveString,
                           const char * lawKindString,
                           const char * const freedStr,
                           const char * const lawPS2Str  // 11.jan.2001
                          )
 {
  long int howMany = 0 ;
  const size_t crSz = 512 + 256
                      + 512  // 12.jan.2001 because of T_JUDGEMENT.F_FLAGS
                       ;
  char * crit = new char [crSz] ;
  const char * const what =
    //"DISTINCT "  // 09.jan.2000
    // "F_NO, F_YEAR, F_TYPE, "
    "F_SUBJECT, "
    "F_UCN_TYPE, F_UCN" ;

  if (crit && composeReportSidesCriteria (crit, crSz, what, types,
                                          composition, pHighRange,
                                          pLowRange, resultsString,
                                          judgedString, judgementKinds,
                                          oldString, punishmentString,
                                          recidiveString, lawKindString,
                                          freedStr, NULL,
                                          lawPS2Str  // 11.jan.2001
                                         ))
   {
    TQuery q (crit) ;

    while (q.Read ())
      howMany ++ ;
   }
  if (crit)
    delete[] crit ;
  return howMany ;
 }  // reportSidesCount

long int sessionsForRSCount (const char * resultsString,
                             const collCompBegDEndDBegN * cCBDEDBNPtr,
                             const char * ageString,
                             const char * pGrantChar,
                             const char * pSessKindChar, bool skcEq,
                             const char * lawKindStr,
                             const bool followSessDate,
                             const bool includingClosedSessions
                            )
 {
  long int howMany = 0 ;
  const size_t crSz = 512 + 128 + 64 + 64 + 64 ;
  char * crit = new char [crSz] ;
  const char * const what =
    "DISTINCT S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE"
    ", S.F_KIND"  // 24.jan.2001
     ;

  if (crit && composeRSCriteria (crit, crSz, what, resultsString,
                                 cCBDEDBNPtr, ageString, pGrantChar,
                                 pSessKindChar, skcEq, lawKindStr,
                                 followSessDate, includingClosedSessions))
   {
    TQuery q (crit) ;
    while (q.Read ())
      howMany ++ ;
   }
  if (crit)
    delete[] crit ;
  return howMany ;
 }  // sessionsForRSCount

long int sessionsForRSCountMembership (const char * resultsString,
                             const collCompBegDEndDBegN * cCBDEDBNPtr,
                             const char * ageString,
                             const char * pGrantChar,
                             const char * pSessKindChar, bool skcEq,
                             const char * lawKindStr,
                             const bool followSessDate,
                             const bool includingClosedSessions,
                             const bool judgeOnly)
 {
  long int howMany = 0 ;
  const size_t crSz = 512 + 256 + 320 + 64 ;
  char * crit = new char [crSz] ;
  const char * const what =
    "DISTINCT S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE"
           ", S.F_KIND"  // 16.jun.2000  (more clean)
           ", S.F_JUDGE, S.F_SECRETAR, S.F_PROSECUTOR, "
             "S.F_JUROR, S.F_REJUROR, "
             "S.F_JUDGE_X, S.F_REJUDGE_X, S.F_JUROR_X, "
             "S.F_PRESIDENT" ;

  if (crit && composeRSCriteria (crit, crSz, what, resultsString,
                                 cCBDEDBNPtr, ageString, pGrantChar,
                                 pSessKindChar, skcEq, lawKindStr,
                                 followSessDate, includingClosedSessions
                                ))
   {
    TQuery q (crit) ;
    TSession s ;

    while (q.Read ())
     {
      s << q ;

      const bool judgePresent = ucnPresent (s.judge) ;
      const int secretarPresent = ucnPresent (s.secretar) ;
      const int prosecutorPresent = ucnPresent (s.prosecutor) ;
      const int jurorPresent = ucnPresent (s.juror) ;
      const int reJurorPresent = ucnPresent (s.reJuror) ;
      const int judgeXPresent = ucnPresent (s.judgeX) ;
      const int reJudgeXPresent = ucnPresent (s.reJudgeX) ;
      const int jurorXPresent = ucnPresent (s.jurorX) ;
      const int presidentPresent = ucnPresent (s.president) ;
      const bool judgeIsCopy = judgePresent &&
			       (!strcmp (s.judge, s.judgeX) ||
				!strcmp (s.judge, s.reJudgeX) ||
                                !strcmp (s.judge, s.president)) ;
      const int membCount = secretarPresent + prosecutorPresent +
                            jurorPresent + reJurorPresent +
                            judgeXPresent + reJudgeXPresent +
                            jurorXPresent + presidentPresent ;
      const bool judgeAlone = (judgePresent && membCount == 0) ||
                              (judgeIsCopy && membCount == 1) ;

      if (judgePresent && (judgeOnly == judgeAlone))
        howMany ++ ;
     }
   }
  if (crit)
    delete[] crit ;
  return howMany ;
 }  // sessionsForRSCountMembership

long int closedSessCount (const collBegDEndDBegN & cBegDEndDBegN,
                          const char * resultsString,
                          const char * decisKindsString,
# if INSTANCE
                          int instns,
# endif
                          const bool followSessDate,
                          const char * const lawSourceKindsString,
                          const bool resStrMatch)
 {
  long int howMany = 0 ;
  const size_t crSz = 1024 ;
  char * crit = new char [crSz] ;
  const char * const what = followSessDate ?
// 22.jan.2001 - begin
//    "DISTINCT S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE" :
    "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE, S.F_KIND" :
// 22.jan.2001 - end
    "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE" ;

  if (crit && composeClosedSessBSCriteria (crit, crSz, what,
                                           cBegDEndDBegN, resultsString,
							 decisKindsString,
# if INSTANCE
                                           instns,
# endif
                                           followSessDate,
                                           lawSourceKindsString,
                                           resStrMatch  // 24.apr.2000
                                           ))
   {
    // 2015:187 LPR/JRQ: order by fails because of unselected fields
    if (!followSessDate)
	strcpy(strstr(crit, "ORDER BY"), " ;");
    TQuery q (crit) ;

    //ofstream("00000007.tmp") << crit;

    while (q.Read ())
      howMany ++ ;
   }
  if (crit)
    delete[] crit ;
  return howMany ;
 }  // closedSessCount

// 24.apr.2000 - begin
long int
  closedSessDecidsOrCancelsCount (const collBegDEndDBegN & cBegDEndDBegN,
# if INSTANCE
                                  int instns,
# endif
                                  bool decideds,
                                  const char * const lawSourceKindsString
                                 )
 {
  long int howMany = 0 ;
  const size_t crSz = 1024 + 32
                          + 64  // 04.aug.2000
                       ;
  char * crit = new char [crSz] ;
  const char * const what =
    "DISTINCT S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE, S.F_RESULT"
    ", S.F_KIND AS SESS_KIND"  // 15.jan.2001
    ", S.F_RETURNED, L.F_FINISHED"  // 26.jul.2000
    ", L.F_KIND, L.F_GRANTED, L.F_SOURCE_KIND"  // 04.aug.2000
     ;

  if (crit && composeClosedSessBSCriteria (crit, crSz, what,
                                           cBegDEndDBegN,
                                           RESULT_FINALIZEDS, NULL
# if INSTANCE
                                           , instns
# endif
                                           , false,  // (followSessDate)
                                           lawSourceKindsString
                                           ))
   {
    TSession * pSess = new TSession ;
    TQuery q (crit) ;
    char sessKind ;  // 15.jan.2001 - begin
    TDummyGroup sKGroup;

    sKGroup.Add(new TChar ("SESS_KIND", & sessKind)) ;  // 15.jan.2001 - end
    while (q.Read ())
     {
      TLawsuit law ;  // 26.jul.2000 - because of F_FINISHED

      law << q ;  // 26.jul.2000
      (* pSess) << q ;
      sKGroup << q ;  // 15.jan.2001
	pSess -> kind = sessKind ;  // 15.jan.2001
      if (decideds)
       {
        if (sessionDecidesTheLaw (* pSess
             , & law,  // 04.aug.2000  // , law.finished,  // 26.jul.2000
                   ! cBegDEndDBegN.wantSortByDateReturned  // 26.jul.2000
                                 ))
          howMany ++ ;
       }
      else 
       {
        if (sessionCancelsTheLaw (* pSess
             , & law,  // 04.aug.2000  // , law.finished,  // 26.jul.2000
                   ! cBegDEndDBegN.wantSortByDateReturned  // 26.jul.2000
                                 ))
          howMany ++ ;
       }
     }
    delete pSess ;
   }
  if (crit)
    delete[] crit ;
  return howMany ;
 }  // closedSessDecidsOrCancelsCount
                                  
long int closedSessDecidedsCount (const collBegDEndDBegN & cBegDEndDBegN,
# if INSTANCE
                                  int instns,
# endif
                                  const char * const lawSourceKindsString
                                 )
 {
  return closedSessDecidsOrCancelsCount (cBegDEndDBegN,
# if INSTANCE
                                         instns,
# endif
                                         true, lawSourceKindsString
                                        ) ;
 }  // closedSessDecidedsCount

long int closedSessCanceledsCount (const collBegDEndDBegN & cBegDEndDBegN,
# if INSTANCE
                                   int instns,
# endif
                                  const char * const lawSourceKindsString
                                  )
 {
  return closedSessDecidsOrCancelsCount (cBegDEndDBegN,
# if INSTANCE
                                         instns,
# endif
                                         false, lawSourceKindsString
                                        ) ;
 }  // closedSessCanceledsCount                                  
// 24.apr.2000 - end

CDate firstDayOfThisMonth (CDate d)
 {
  d.day = 1 ;
  return d ;
 }  // firstDayOfThisMonth

CDate firstDayOfNextMonth (CDate d)
 {
  d.day = 1 ;
  d.month ++ ;
  if (d.month > 12)
   {
    d.month = 1 ;
    d.year ++ ;
   }
  return d ;
 }  // firstDayOfNextMonth

CDate lastDayOfThisMonth (CDate d)
 {
  d.day = d.DaysInMonth();
  return d ;
 }  // lastDayOfThisMonth

CDate nextDay (const CDate &d)
 {
  CDate d1 = d ;
  d1 += 1;
  return d1 ;
 }  // nextDay

long int differenceInMonths (const CDate &smaller,
				     const CDate &greater)
 {
  const bool positiveRes = (greater >= smaller) ;
  CDate sm = positiveRes ? smaller : greater ;
  CDate gr = positiveRes ? greater : smaller ;
  long int d = 0 ;

  while (sm.year * 12 + sm.month < gr.year * 12 + gr.month)
   {
    gr.month -- ;
    if (gr.month == 0)
     {
	gr.month = 12 ;
	gr.year -- ;
     }
    d ++ ;
   }
  if (d && gr.day < sm.day)
    d -- ;
  return positiveRes ? d : -d ;
 }  // differenceInMonths

// 2011:098 LPR: rewritten
int difference_in_days(const CDate &date1, const CDate &date2)
{
	const bool positive = date1 <= date2;
	CDate sm = positive ? date1 : date2;
	CDate gr = positive ? date2 : date1;
	int d = 0;

	while (sm < gr)
	{
		sm += 1;
		d++;
	}

	return positive ? d : -d;
}

unsigned long int countRecsInTable (const char * table)
 {
  long int howMany = 0 ;
  char crit [128] ;

  ostrstream (crit, sizeof (crit))
    << "SELECT COUNT (*) FROM " << table << " ;" << ends ;
  crit [sizeof (crit) - 1] = '\0' ;
   {
    TGetGroup g(crit, new TLong ("COUNT", & howMany));
   }
  return howMany ;
 }  // countRecsInTable ;

char* makeTimeBookFormatFN(const char college,
# if INSTANCE
  const bool firstInstLaws, const bool puniAdmConcLaws,
# endif
  const bool jan2005Format)
{  // An old varianth in VresionArchives\2365\_appname.j --> _utility.cpp

  // It is also used for patches (фишове). In this case, the BOOK may be
  // printed from another computer or with different print settings. So,
  // the windowsPrint argument represents the book, that has already
  // had printed.

  bool doubtful = jan2005Format;
  const char * nameSuffix = "col";  // Be sure it is 3 symbols long!
  char tmp[MAXPATH];
  char* formatFN = NULL;


  strCopy(tmp, "text\\tb", sizeof(tmp));
  strConcatCh(tmp, college ? college : '_', sizeof(tmp));

  // We have to support the following formats:
  // - the old book format as text file
  // - the new book format as text file
  if(doubtful)
  {  // the new books format
    // New formats are available only in text   //20050314
    nameSuffix = "_t_" ;
  }
  strConcat(tmp, nameSuffix, sizeof(tmp));
#if INSTANCE
  if(firstInstLaws == false)
  {
    strConcatCh(tmp, puniAdmConcLaws ? '3' : '2', sizeof(tmp));
  }
#endif  // of INSTANCE
  strConcat(tmp, DEFAULT_FORMAT_FILE_EXTENSION, sizeof(tmp));
  {
    int sz = strlen(tmp) + 1;

    if(sz < sizeof(tmp))
    {
      formatFN = new char[sz];
      strCopy(formatFN, tmp, sz);
    }
  }
  return formatFN ;
}  // makeTimeBookFormatFN

# if INSTANCE
const char * lawsuitKindSuffix (const TLawsuit & lawsuit)
 {
  const char * suffix = "" ;

  switch (lawsuit.key.type)
   {
    case TYPE_CITIZEN_LAW :
// 31.mar.2000 - begin
#   if (COURT_TYPE == COURT_AREAL)
// 31.mar.2000 - end
      if (strchr (KIND_2ND_CITIZEN_LAWS, lawsuit.kind))
       {
        if (lawsuit.source.type == TYPE_REQUEST &&
            lawsuit.sourceKind == KIND_PRIVATE_COMPLAINT_CIT_INDOC)
          suffix = "Частно" ;
//          suffix = "(Ч)" ;
//        else
//          suffix = "(В)" ;
       }
#   endif  // 31.mar.2000
      break ;
// 31.mar.2000 - begin
#  if (COURT_TYPE == COURT_AREAL)
// 31.mar.2000 - end
    case TYPE_ADMIN_LAW :
//      if (strchr (KIND_2ND_ADMIN_LAWS, lawsuit.kind))
//       {
//        if (lawsuit.source.type == TYPE_REQUEST &&
//            lawsuit.sourceKind == KIND_CONCERN_COMPLAINT_ADM_INDOC)
////          suffix = "(Ч)" ;
////          suffix = "Частно" ;
//          suffix = "Касационно" ;
//       }
      if (lawIsCocern3rdInst (lawsuit))
        suffix = "Касационно" ;
      break ;
#   endif  // APPEAL  // 31.mar.2000
   }
  return suffix ;
 }  // lawsuitKindSuffix
# else
# endif

bool alignment_or_justify_present (const char * const formatFileName,
					     int & alignment, int & justify,
					     int & linesOfPage)
   // Keeps each tested parameter-setting unchanged if it isn't present
{  // 17.oct.2001
  bool res = false ;
  const char * const funcName = "alignment_or_justify_present" ;
  const char * const isNullOrEmpty = ARG_IS_NULL_OR_EMPTY ;

  if (formatFileName && formatFileName [0])
   {  // -- 1 --
    CharPtr fmtFNameToUse (strlen (formatFileName) + SIZE_OF_LINE) ;

    ostrstream (fmtFNameToUse, fmtFNameToUse.Size ())
      << formatFileName << ends ;
    if (access (fmtFNameToUse, 0) == 0 ||
	  referenceToFmtFile (fmtFNameToUse, fmtFNameToUse.Size ()))
     {  // -- 2 --
      ifstream ifs (fmtFNameToUse) ;

      if (ifs)
       {  // -- 3 --
        if (seek_parameters_section (ifs))
         {  // -- 4 --
          int hLL, pageLines, itemLines, itemLines1 ;
          unsigned long int extraVal, extraVal1 ;
          int align, just ;

          extraVal = extraVal1 = hLL = pageLines = itemLines = itemLines1 =
          align = just = 0 ;
          load_format_parameters (ifs, hLL, pageLines, itemLines,
                                  itemLines1, extraVal, extraVal1,
                                  alignment, justify) ;
          res = align || just ;
          if (align)
            alignment = align ;
          if (just)
            justify = just ;
	    if (section_says_yes (ifs, "***_JUSTIFY_ROWS_COUNT", false))
            if (pageLines)
              linesOfPage = pageLines ;  // 13.dec.2001 - end
         }  // -- 4 --
	 }  // -- 3 --
	else
	  cannotOpenFmtFile (fmtFNameToUse);
     }  // -- 2 --
   }  // -- 1 --
  else
    error ("%s:\nThe formatFileName%s.", funcName, isNullOrEmpty) ;
  return res ;
 }  // alignment_or_justify_present

#ifdef BI_MULTI_THREAD_RTL
// N.B. Do not use MT LIBC RTL tasks, they are severely broken.

//unsigned long _beginthreadNT(void (_USERENTRY *start_address)(void *),
//	unsigned stack_size, void *arglist, void *security_attrib,
//	unsigned long create_flags, unsigned long *thread_id)
//{
//	return (CreateThread(security_attrib, stack_size, start_address, arglist, create_flags, thread_id);
//}

extern "C" unsigned long _beginthreadNT(void (_USERENTRY *start_address)(void *),
	unsigned, void *arglist, void *,
	unsigned long, unsigned long *thread_id)
{
	return ((unsigned long) CreateThread(0, 0, (LPTHREAD_START_ROUTINE) start_address, arglist, 0, thread_id));
}
#endif  // BI_MULTI_THREAD_RTL

#include <owl/printer.h>

static inline HANDLE OpenExisting(const char *name, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwFlagsAndAttributes)
{
	return CreateFile(name, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, dwFlagsAndAttributes, NULL);
}

#include "CancelPrintDialog.h"
#include "DirectPrintThread.h"
#include "WindowsPrintThread.h"
#include "TextPrintout.h"
#include "ShowTextDialog.h"

#include "DirectPrintThread.cpp"
#include "CancelPrintDialog.cpp"
#include "WindowsPrintThread.cpp"
#include "TextPrintout.cpp"
#include "ShowTextDialog.cpp"

bool show_dlg(TDialog *dialog, bool wait)
{
	bool printed;

	if (wait)
	{
		printed = dialog->Execute() == IDOK;
		delete dialog;
	}
	else
		printed = dialog->Create();

	return printed;
}

char * strDelLeadingSpaces (char * str)
 {
  if (str)
    while (str [0] == ' ')
	memmove (str, str + 1, strlen (str + 1) + 1) ;
  return str ;
 }  // strDelLeadingSpaces

char * strDelLastSpaces (char * str)
 {
  if (str)
   {
    size_t l = strlen (str) ;

    while (l && str [l - 1] == ' ')
      str [-- l] = '\0' ;
   }
  return str ;
 }  // strDelLastSpaces

# if INSTANCE
// 31.mar.2000 - begin
// 31.mar.2000 - begin
#   if APPEAL
unsigned long int adm2ndLawsCount (const CDate, const CDate,
					     const long int,  // 27.feb.2001
                                   const bool, const bool,
                                   const char * pLawTypes, // = NULL,  // 20070923
                                   const char * pLawKinds  // = NULL   // 20070923
                                   )
 {
  if ( pLawTypes ) {}
  if ( pLawKinds ) {}
  return 0 ;
 }  // adm2ndLawsCount
#   else  // APPEAL
// 31.mar.2000 - end
unsigned long int adm2ndLawsCount (const CDate endP,
                                   const CDate begP,
                               const long int composition,   // 27.feb.2001
                                   const bool finisheds,     // = true
                                   const bool nFBFromLaws,   // = true
                                   const char * pLawTypes,   // = NULL,  // 20070923
                                   const char * pLawKinds )  // = NULL   // 20070923
 // If finisheds is true, nFBFromLaws is ignored
 {
  long int cnt = 0 ;
  const size_t crtSz = 512 ;
  char * crt = new char [crtSz] ;


  if (crt)
   {
    ostrstream oss (crt, crtSz - 1) ;
    TDummyGroup g;

    crt [crtSz - 1] = '\0' ;
    oss << "SELECT COUNT (*) FROM T_LAWSUIT "
	     "WHERE ";
// 20070923 -->
// Petyo             "F_TYPE = \'" << TYPE_ADMIN_LAW << "\' "
	if (pLawTypes && pLawTypes [0])
      {
         oss << "F_TYPE IN ("
			     << stringToSQLCharSet (pLawTypes).getTheSet ()
                       << ") " ;
	}
      else
         oss << "F_TYPE = \'" << TYPE_ADMIN_LAW << "\' ";
// 20070923 <--

    oss << "AND " ;

// 20070923 -->
// Petyo
//    oss <<   "F_KIND IN ("
//                << stringToSQLCharSet (KIND_2ND_LAWSUITS).getTheSet ()
//                << ") "

      if (pLawKinds && pLawKinds [0])
      {
	   oss << "F_KIND IN ("
                       << stringToSQLCharSet (pLawKinds).getTheSet ()
                       << ") " ;
      }
      else
      {
         oss << "F_KIND IN ("
                       << stringToSQLCharSet (KIND_2ND_LAWSUITS).getTheSet ()
			     << ") " ;
      }
// 20070923 <--

    oss << "AND " ;

    if (composition)  // 27.feb.2001
      oss << "F_COMPOSITION = " << composition << "AND " ;  // 27.feb.2001
    if (finisheds)
     {
	oss <<
	     "F_FINISHED >= " << begP.Value() << " AND "
	     "F_FINISHED <= " << endP.Value() << " " ;  // 15.apr.2003 - end
     }
    else
	if (nFBFromLaws)
	 {
	  oss << "F_DATE <= " << endP.Value() << " "  // 15.apr.2003 - end
		 "AND " ;
	  oss << "(F_FINISHED IS NULL OR "
		    "F_FINISHED >= " << begP.Value() << ") " ;  // 15.apr.2003 - end
	 }
	else
	 {
	  oss << "F_DATE BETWEEN " << begP.Value() << " AND "
		<< endP.Value() << " " ;  // 15.apr.2003 - end
	 }
    oss << ";" << ends ;

    if (criteria_finished_ok (crt, "adm2ndLawsCount, "
					     "lawsuits criteria"))  // 27.feb.2001
     {
	TGetGroup g(crt, new TLong ("COUNT", & cnt));
     }
    else  // 27.feb.2001
	cnt = 0 ;  // 27.feb.2001
    delete[] crt ;
   }

  return cnt ;
 }  // adm2ndLawsCount

#   endif  // APPEAL  // 31.mar.2000
# endif

unsigned long int countFinishedsWithDecJudgDefin (
                             const collCompBegDEndDBegN * cCBEDBNPtr,
                                    const char * const decGJkindsStr,
                                      const char * const lawKindsStr,
                                    const char * const sourceKindStr,
                               const bool requestCurrentlyQuick  // 25.sep.2003
								 )
 {
  unsigned long int count = 0 ;
  const size_t crSz = 512 ;
  char * crit = new char [crSz] ;

  if (composeFinishWithDecJudgDef_Crit (crit, crSz, cCBEDBNPtr,
                                        decGJkindsStr,
                                        lawKindsStr, sourceKindStr))
   {  // -- 0 --
    TQuery q (crit) ;
    LawPtr l ;  // TLawsuit l ; - begin
    DecPtr d ;  // TDecision d ;
    TRequest req ;  // 25.sep.2003 - end
    char * critGJD = new char [512] ;

    while (q.Read ())
     {  // -- 1 --
      bool found = false ;

      l.Ref() << q ;
      if(requestCurrentlyQuick == true)  // 25.sep.2003 - begin
      {
        bool skipThisLaw = true;

        if(TRCDKey_(l.Ref().key).FilledOK() && l.Ref().Get())
          if(quick_law_source_currently_quick(l.Ref().source, req))
            skipThisLaw = false;
	  if(skipThisLaw == true)
          continue;  // the "-- 1 --" loop
      }  // 25.sep.2003 - end
      d.Ref() << q ;
      d.Ref().FloatCriteria (critGJD) ;
       {  // -- 2 --
        TQuery qG (critGJD) ;
        while (! found && qG.Read ())
         {
          d.Ref() << qG ;
          if (d.Ref().date <= l.Ref().finished &&
            strchr (decGJkindsStr, d.Ref().kind))
           {
            found = true ;
           }
         }
       }  // -- 2 --
      if (found)
        count ++ ; 
     }  // -- 1 --
    delete[] critGJD ;
   }  // -- 0 --
  delete[] crit ;
  return count ;
 }  // countFinishedsWithDecJudgDefin

void putCompositionToOutStream (const long int composition, ostream & os,
					  char * const s, const size_t len,
					  const size_t algn, const size_t skip)
 {
  if (! s)
    return ;

  compositionToStringCompl (composition, s, len + 1) ;
// 20070806 -->
/*
# if INSTANCE
#   if APPEAL
  if (os && algn && skip)
   {
    // Nothing. I'm suppressing the warnings
   }
#   else  // of APPEAL
  if (use_area_compositions ())
    writeString (os, s, len, algn, skip) ;
#   endif  // of APPEAL
# else  // of INSTANCE
  if (use_area_compositions ()) // 20051229  after 1.2552 Regional court is in dependence from PRINTCOMPOSITION
    writeString (os, s, len, algn, skip) ;
# endif  // of INSTANCE
*/

  writeString (os, s, len, algn, skip) ;

// 20070806  <--

 }  // putCompositionToOutStream

# if INSTANCE
long int minMaxFirmSubject (const bool min = true)
 {
  TDummyGroup g;
  long int minMax = 0 ;
  char * const what = min ? "MIN" : "MAX" ;
  char cr [128] ;

  g.Add(new TLong (what, & minMax)) ;
  scSprintf (sizeof (cr), cr,
		 "SELECT %s (F_SUBJECT) FROM T_SUBJECT "
		 "WHERE F_TYPE = \'%c\' ;", what, TYPE_FIRM_LAW) ;

  TQuery q (cr) ;

  if (q.Read ())
    g << q ;
  return minMax ;
 }  // minMaxFirmSubject

long int minFirmSubject ()
 {
  return minMaxFirmSubject () ;
 }  // minFirmSubject

long int maxFirmSubject ()
 {
  return minMaxFirmSubject (false) ;
 }  // maxFirmSubject
# endif

void makeJudgeCheckupFormatFN (char * const formatFN,
					 const size_t fmtFNSz,
					 const bool trueJudgeCheckup,
					const begMYEndMYTypeCIU & const bMYEMYTCIU
# if INSTANCE
				    , const collCompBegDEndDBegN & const cCBDEDBN
# endif
	   , const unsigned int cipherIfNotTrueGudgeCheckup  // 12.may.2000
	   , int mainMenuItem  // 20061207
					)
// Look for the veversion with commentaries in _noitinus.bak (12.may.2000)
 {
  ostrstream oss (formatFN, fmtFNSz) ;

  if (trueJudgeCheckup)
   {
     if (mainMenuItem==3)     // 20061207
     {oss
	 << "text\\judgeN";}
     else
     {oss
	 << "text\\judge"; }

     oss
	 << (strlen (bMYEMYTCIU.types) == 1 ?
		bMYEMYTCIU.types [0] : TYPESIGN_GENERAL_REPORT);

     oss
	 << (bMYEMYTCIU.internalUse ? SIGN_INTERNAL_USE :
# if ! INSTANCE
	     bMYEMYTCIU.internalUseCombined ? SIGN_INTERNAL_USE :
# endif
					     SIGN_EXTERNAL_USE) ;
# if INSTANCE
     if (bMYEMYTCIU.decidedLawsJudgeCheckup)
	 oss << "d" ;
# endif
     oss
	 << DEFAULT_FORMAT_FILE_EXTENSION
	 << ends ;
   }
  else
   {
    switch (cipherIfNotTrueGudgeCheckup)
     {
	case OFFSET_TIME_BOOK_CIPHER :
	  oss
	    << "text\\tj"
	    << bMYEMYTCIU.types [0]
	    << "col"
# if INSTANCE
	    << (bMYEMYTCIU.firstInstLaws ?
		  "" :
		  (bMYEMYTCIU.types [0] == TYPE_PUNISHMENT_LAW &&
		   cCBDEDBN.puniAdmConcernLaws ? "3" : "2"))
# endif  // of INSTANCE
	    << DEFAULT_FORMAT_FILE_EXTENSION
	    << ends ;
	  break ;
	case OFFSET_CLOSED_SESS_BOOK_CIPHER :
	  oss
          << "text\\clsj" 
          << bMYEMYTCIU.types [0]
          << "col"
          << DEFAULT_FORMAT_FILE_EXTENSION
          << ends ;
        break ;
      default :
        error ("Не мога да съставя името на форматния файл за "
               "рекапитулацията по съдии") ;
        oss << "text\\empty.fmt" << ends ;
        break ;
      }
   }
  formatFN [fmtFNSz - 1] = '\0' ;
 }  // makeJudgeCheckupFormatFN

void outNormDblStrkStringAtCondStream (ostream & os, const char * const s,
						   const size_t len, const int algn,
						   const size_t skip)
 {
  if (WINDOWS_PRINT)
    writeDoubleStrikedString (os, s, len, algn, skip) ;
  else
   {
    writeString (os, NORMAL_PRINT_S, LEFT_ALIGN, 1) ;
    writeDoubleStrikedString (os, s,
					(size_t) ((len * 70.0) / 120.0),
					algn, skip) ;
    writeString (os, CONDENSED_PRINT_S, LEFT_ALIGN, 1) ;
   }
 }  // outNormDblStrkStringAtCondStream

size_t strConcat (char * dest, const char * appendix, size_t destSz)
 {
  if (! (dest && appendix && destSz))
    return 0 ;

  size_t i = strlen (dest), j = 0 ;

  if (destSz <= 0 || ! strlen (appendix))
    return 0 ;
  while (i < (destSz - 1) && appendix [j])
    dest [i ++] = appendix [j ++] ;
  dest [i] = '\0' ;
  return j ;
 }  // strConcat

size_t strConcatCh(char * dest, const char appendix, size_t destSz)
{
  char a[2];

  a[0] = appendix;
  a[1] = '\0';
  return strConcat(dest, a, destSz);
}

size_t strConcatLong (char * dest, const long n, size_t destSz)
{
  size_t res = 0;

  if(dest && destSz)
  {
    char tmp[32];

    ostrstream(tmp, sizeof(tmp)) << n << ends;
    ENDING_ZERO(tmp);
    strConcat(dest, tmp, destSz);
  }
  return res;
}

char* strCopy(char *dest, const char *src, const size_t destSz)
{
  if(dest && destSz)
  {
    dest[0] = '\0';
    strConcat(dest, src, destSz);
  }
  return dest;
}  // strCopy

void invert_list_box_selection (TGroupListBox * const pLB)
 {
  if (pLB)
   {
    const int items = pLB -> GetCount () ;

    if (items > 0)
     {
	int cI = pLB -> GetCaretIndex () ;  // Used to keep the current
						// caret index after the inverting
	if (cI < 0)
	  cI = 0 ;
	for (int i = 0 ; i < items ; i++)
	  if (i != cI)
	    pLB -> SetSel (i, ! (pLB -> GetSel (i))) ;
	pLB -> SetSel (cI, ! (pLB -> GetSel (cI))) ;
     }
   }
 }  // invert_list_box_selection

bool lawsuit_finalising_decision (const TLawsuit & law, TDecision & dec,
					    TWindow * pParent,
					    const char * const forWhat,
					    const bool errMsg,
					    TSession* const sessPtr) //18.feb.2005 sessPtr

 {
  bool res = false ;

  if(sessPtr)
  {
    clearGroupData(*sessPtr);
  }
  if (TRCDKey_ (law.key).FilledOK () && law.finished.Empty () == false)
   {  // -- 0 --
    char crit [512] ;
    ostrstream oss (crit, sizeof (crit)) ;

      oss << "SELECT S.F_DATE, S.F_KIND FROM T_SESSION S WHERE "
           "S.F_TYPE = \'" << law.key.type << "\' AND "
           "S.F_YEAR = \'" << law.key.year << "\' AND "
	   "S.F_NO = \'" << law.key.no << "\' AND "
	   // 2008:281: LPR/LRQ: using FINAL date
	   "S.F_FINAL = " << law.finished.Value() << " "
	   ";" << ends ;
    crit [sizeof (crit) - 1] = '\0' ;
    if (criteria_finished_ok (crit, "lawsuit_finalising_decision, "
                                    "session criteria"))
     {  // -- 1 --
	TCountRecsQuery q (crit) ;
	const unsigned sessionsFound = q.getRecsCount () ;

      if (sessionsFound == 1)
       {  // -- 2 --
	  CDate sessDate ;
        char sessKind = 0 ;

        clear_CDate (sessDate) ;
         {
          SessPtr pSess ;

          if (q.Read ())
           {
            pSess.Ref () << q ;
            sessDate = pSess.Ref ().date ;
            sessKind = pSess.Ref ().kind ;
            if(sessPtr)
            {
              (*sessPtr) << (pSess.Ref());
            }
           }
         }
        if (sessDate.Empty () == false && sessKind)
         {  // -- 3 --
          SessionEndDocs endDocs (law.key, sessDate, sessKind) ;

          res = endDocs.ChooseOne (pParent, dec, forWhat) ;
	   }  // -- 3 --
       }  // -- 2 --
	else
        if (errMsg)
         {
	    char lKStr [128] ;

          ostrstream (lKStr, sizeof (lKStr))
            << TRCDKey_ (law.key) << ends ;
          lKStr [sizeof (lKStr) - 1] = '\0' ;
	    error ("%s:\n"
                 "Заседанието с което делото приключва не може "
                 "да бъде определено (%s).",
                 lKStr, (sessionsFound ?
                         "повече от едно заседания отговарят на критерия" :
                         "липсва такова заседание")
                ) ;
         }
     }  // -- 1 --
   }  // -- 1 --
  if(sessPtr != NULL && res == false)
  {
    clearGroupData(*sessPtr);
  }
  return res ;
 }  // lawsuit_finalising_decision

void move_string_array_contents(TStringArray* pDest,
  TStringArray* pSrc)
{  // 27.jan.2005
  FUNC_NAME("move_string_array_contents");

  if(pDest && pSrc)
  {
    int cnt = pSrc->Count();

    for(int i = 0; i < cnt; i++)
    {
	pDest->Add(strnew ((*pSrc)[i]));
    }
    pSrc->Flush();
  }
  else
    REPORT_F_INVALID_ARGS;
}  // move_string_array_contents

bool connect_type_in( TRCDKeyContainer *container,
			    const char *connectTypes )
{
	TGArray<TConnect> connects;
  bool res = false;

	if (load_connects(container, &connects))
	{
		for (int i = 0; i < connects.Count(); i++)
		{
			if (strchr(connectTypes, connects[i]->connectType))
			{
	  res = true;
			break;
			}
		}
	}
  return res;
}

TConnect *get_connect(TRCDKeyContainer *container, const char *connectType)
{
	TGArray<TConnect> connects;
	TConnect *connect = NULL;

	if (load_connects(container, &connects))
	{
		for (int i = 0; i < connects.Count(); i++)
		{
			if (strchr(connectType, connects[i]->connectType))
			{
				if (!connect)
				{
					connect = new TConnect;
					*connect << connects[i];
	    break;
				}
			}
		}
	}

	return (connect);
}

bool summCharsMatch(const char required, const char found)
{
  if (found == '*')
    return true ;
  return required == found ;
}  // Summons :: CharsMatch

void set_zero_all_comp(TLongAliasFace *alias)
{
	((TAliasGroup *) alias->GroupAt(0))->NameField->SetS(ALL_COMPOSITIONS);
}

void select_de1st_comp(TLongAliasFace *alias)
{
	alias->SetL(QUERY_COMPOSITIONS ? *QUERY_COMPOSITIONS : *COMPOSITION_ALLS);
}

bool show_text_use_settings(TWindow *parent, char *tText, const char *formatFileName, int alignment, int safetyLines)
{
	TCharPtr CP(tText);

	if (safetyLines < 0)
		safetyLines = 0;

	if (formatFileName && any(formatFileName))
	{
		TPrinterGroup *config = new TPrinterGroup;
		config->Load();
		int justify = config->justify;
		int longestPL = longest_text_page_lines(tText, safetyLines);
		static constant tooBigAnWillBeTrunked = "Тази величина е прекалено голяма и ще бъде ограничена до ";

		{
			int pageLen = 0;  // This zero is important !!!

			alignment_or_justify_present(formatFileName, alignment, justify, pageLen);

			if (pageLen > 0)
				if (longestPL < pageLen)
					longestPL = pageLen;
		}

		{
			const int MAX_PL = 999;

			if (longestPL > MAX_PL)
			{
				info("Най-дългата страница на документа съдържа %d реда. "
					"%s""%d.", longestPL, tooBigAnWillBeTrunked, MAX_PL);
				longestPL = MAX_PL;
			}
		}

		{
			const int MAX_J = 9999;

			if (justify > MAX_J)
			{
				info("Отстъпът за печат е %d. ""%s""%d.",
					justify, tooBigAnWillBeTrunked, MAX_J);
				justify = MAX_J;
			}
		}

		config->alignto = alignment;

		if (longestPL > 0)
			config->pagelen = longestPL;

		config->justify = justify;
		return show_dlg(new TShowTextDialog(parent, getcrlf(tText), config), parent);
	}

	return error("%s:\nThe formatFileName%s.", "show_stream_use_settings", ARG_IS_NULL_OR_EMPTY);
}

bool show_text_stream_use_settings(TWindow *parent, ostrstream &oss, const char *formatFileName, int alignment,
	int safetyLines)
{
	// 2015:247 FIX: getcrlf() is done in show_text_use_settings()
	return show_text_use_settings(parent, gettext(oss), formatFileName, alignment, safetyLines);
}

bool show_html_stream(TWindow *parent, const char *name, const char *what, ostrstream &oss, sign remove)
{
	TUserFile uf(what);
	char *iText = gettext(oss);
	TCharPtr CP(iText);

	return uf.WriteAll(name, iText) && show_html(parent, name, remove != -1 ? remove : parent != NULL);
}

bool show_html_stream(TWindow *parent, ostrstream &oss)
{
	return show_html(parent, gettext(oss));
}

void show_office_stream(ostrstream &oss)
{
	TUserFile uf("временен");
	char *iText = gettext(oss);
	TCharPtr CP(iText);
	const char *name = uf.WriteTemp("htm", iText);

	if (name)
		THTMLFile::Show(name, NULL, true, true);
}

int longest_text_page_lines (char *tText, int safetyLines)
 {
  int lPL = 0 ;

   {  // -- 0 --
    istrstream iss (tText) ;

     {  // -- 1 --
	char line [SIZE_OF_LINE] ;
	int currPL = 0 ;

	while (iss.getline(line, SIZE_OF_LINE))
	 {  // -- 2 --
	   {  // -- 3 --
	    if (strchr (line, (char) 12))
	     {  // -- 4 --
		const char * const newPgStr = "\x0C" ;  // (char) 12

		if (strcmp (line, newPgStr) == 0)  // 12.jun.2001 - begin
		 {
		  currPL ++ ;
		  if (currPL > lPL)
		    lPL = currPL ;
		  currPL = 1 ;
		 }
		else
		 {  // 12.jun.2001 - end
		  bool first = true ;

		  while (strtok (first ? line : NULL, newPgStr))
		   {  // -- 5 --
		    first = false ;

		    // I'm not sure that the things are
		    // exatly in this way, but let it be
		    currPL ++ ;
		    if (currPL > lPL)
			lPL = currPL ;
		    currPL = 1 ;
		   }  // -- 5 --  // while (strtok (...
		 }  // 12.jun.2001
	     }  // -- 4 --
	    else
		currPL ++ ;
	   }  // -- 3 --
	 }  // -- 2 --  // while (ifs.good ())
     }  // -- 1 --
   }  // -- 0 --


    lPL += safetyLines ;
    if (lPL < MIN_PAGE_LINES)
	lPL = MIN_PAGE_LINES ;

  return lPL ;
 }  // longest_text_page_lines

int longest_text_line_symbols (char *tText)
 {
  int lLS = 0 ;

   {  // -- 0 --
    istrstream iss (tText) ;

     {  // -- 1 --
	char line[SIZE_OF_LINE] ;

	while (iss.getline(line, sizeof line))
	 {  // -- 2 --
	   {  // -- 3 --
	    if (strchr (line, (char) 12))
	     {  // -- 4 --
		const char * const newPgStr = "\x0C" ;  // (char) 12
		bool first = true ;
		const char * l ;

		while ((l = strtok (first ? line : NULL, newPgStr)) != NULL)
		 {  // -- 5 --
		  first = false ;

		  // I'm not sure that the things are
		  // exatly in this way, but let it be
		  const int currLS = strlen (l) ;

		  if (currLS > lLS)
		    lLS = currLS ;
		 }  // -- 5 --
	     }  // -- 4 --
	    else
	     {
		const int currLS = strlen (line) ;

		if (currLS > lLS)
		  lLS = currLS ;
	     }
	   }  // -- 3 --
	 }  // -- 2 --  // while (ifs.good ())
     }  // -- 1 --
   }  // -- 0 --

  return lLS ;
 }  // longest_text_line_symbols
