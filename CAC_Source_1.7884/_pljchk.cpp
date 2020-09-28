#include "cac.h"

void TJudge :: addFields ()
 {
  Add(
    new TChar ("F_POST", & post),
    new TChar ("NAME_1ST_SYMB", & name1stSymb),
    new TChar ("FAMILY_1ST_SYMB", & family1stSymb),
    NULL);
 }  // TJudge :: addFields

void TJudge :: clear ()
 {
  clearGroupData (* this) ;
 }  // TJudge :: clear

bool TJudge :: load (const char * const anUCN, const char anUCNType,
                    const bool loadInitials  // 06.feb.2003
                   )
 {
  bool rT = true ;  // 02.apr.2003  // bool res ;

  scSprintf (sizeof (ucn), ucn, "%s", anUCN) ;
  ucnType = anUCNType ;
  post = (char) 255 ;
  name1stSymb = family1stSymb = '\0' ;
  if (loadInitials)
   {
    TCitizen cit ;

    cit << (* this) ;
    if ((rT = cit.Get ()) == true)
     {
      name1stSymb = cit.name [0] ;
      family1stSymb = cit.family [0] ;
     }
   }
  if (rT)
   {
    TUser u ;

    u << (* this) ;
    if (u.Try ())
      post = u.post ;
   }
  if (! rT)
    clear () ;
  return rT ;
 }  // TJudge :: load

TJudge :: TJudge ()
 {
  addFields () ;
  clear () ;
 } ;  // TJudge :: TJudge

TJudge :: TJudge (const char * const anUCN, const char anUCNType,
                const bool loadInitials  // 06.feb.2003
		   )
 {
  addFields () ;
  load (anUCN, anUCNType, loadInitials) ;
 } ;  // TJudge :: TJudge

bool TJudge :: nameFamily (char * pN, const size_t nSz,
                          char * pF, const size_t fSz)
 {  // An old varianth - in VersionArchives\1232\_appname.j --> _pljchk.cpp
  const bool n = pN && nSz ;
  const bool f = pF && fSz ;

  if (! (n || f))
		return false ;
	if (n)
		pN [0] = '\0' ;
	if (f)
		pF [0] = '\0' ;
	if (! ucnType)
		return false ;

	TCitizen c ;

	c << (* this) ;
	if (c.Get ())
	 {
    if (n && nSz > 1)
			scSprintf (nSz, pN, "%.*s", nSz - 1, c.name) ;
		else
      if (! (f && fSz > 1))
        return false ;
    if (f && fSz > 1)
      scSprintf (fSz, pF, "%.*s", fSz - 1, c.family) ;
    else
      if (! (n && nSz > 1))
        return false ;
    return true ;
   }
  else
    return false ;
 }  // TJudge :: nameFamily

bool TJudge::operator==(const TJudge& other) const           // 25.nov.2004
{
  return (
    //post == other.post &&  // 25.nov.2004
    name1stSymb == other.name1stSymb && family1stSymb == other.family1stSymb &&
    strcmp(ucn, other.ucn) == 0);
}

bool TJudge::operator<(const TJudge& other) const            // 25.nov.2004
{
  const bool thisInter = strcmp(ucn, INTER) == 0 ;
  const bool otherInter = strcmp(other.ucn, INTER) == 0 ;

  if(thisInter == otherInter)
   {
//    if (post < other.post)                            // 25.nov.2004 - begin
//      return true ;
//    else
//      if (post > other.post)
//        return false ;
//      else                                            // 25.nov.2004 - end
        if (name1stSymb < other.name1stSymb)
          return true ;
        else
          if (name1stSymb > other.name1stSymb)
            return false ;
          else
            if (family1stSymb < other.family1stSymb)
              return true ;
            else
              if (family1stSymb > other.family1stSymb)
                return false ;
              else
                return (strcmp (ucn, other.ucn) < 0) ;
   }  // 14.aug.2002 - begin
  else
    return (thisInter == false && otherInter == true);  // 14.aug.2002 - end
}

// 25.nov.2004 - begin
//
// Remove the sort by post (One mess less, I think)...
//
//bool operator == (const TJudge & j1, const TJudge & j2)
// {
//  return (j1.post == j2.post && j1.name1stSymb == j2.name1stSymb &&
//          j1.family1stSymb == j2.family1stSymb &&
//          strcmp (j1.ucn, j2.ucn) == 0) ;
// } ;  // bool operator == (const TJudge & j1, const TJudge & j2)
//
//bool operator < (const TJudge & j1, const TJudge & j2)
// {
//  const bool j1Inter = strcmp(j1.ucn, INTER) == 0 ;  // 14.aug.2002 - begin
//  const bool j2Inter = strcmp(j2.ucn, INTER) == 0 ;
//
//  if(j1Inter == j2Inter)
//   {  // 14.aug.2002 - end
//    if (j1.post < j2.post)
//      return true ;
//    else
//      if (j1.post > j2.post)
//        return false ;
//      else
//        if (j1.name1stSymb < j2.name1stSymb)
//          return true ;
//        else
//          if (j1.name1stSymb > j2.name1stSymb)
//            return false ;
//          else
//            if (j1.family1stSymb < j2.family1stSymb)
//              return true ;
//            else
//              if (j1.family1stSymb > j2.family1stSymb)
//                return false ;
//              else
//                return (strcmp (j1.ucn, j2.ucn) < 0) ;
//   }  // 14.aug.2002 - begin
//  else
//    return (j1Inter == false && j2Inter == true);  // 14.aug.2002 - end
// } ;  // bool operator < (const TJudge & j1, const TJudge & j2)
// 25.nov.2004 - end

subject_With_LongIntArray ::
  subject_With_LongIntArray (const size_t lonsgCnt, const long subject) :
    arrItems (lonsgCnt), subj (subject), pLongs (NULL)
 {
  pLongs = new long [arrItems + 1] ;
  for (size_t i = 0 ; i < arrItems + 1 ; i ++)
    pLongs [i] = 0 ;
  // pLongs [arrItems] will be used in case of out of range parameter
  // passed to 'operator []'
 }  // subject_With_LongIntArray :: subject_With_LongIntArray

long & subject_With_LongIntArray :: operator [] (size_t index) const
 {
  if (arrItems && index < arrItems)
    return pLongs [index] ;
  else
   {
    error ("%s::operator[]\nIndex is ouf of range.", typeid(*this).name());
    return pLongs [arrItems] ;
   }
 }  // long & subject_With_LongIntArray ::
    // operator [] (size_t index) const

const long & subject_With_LongIntArray ::
  itemAtIndex (size_t index) const
 {
  return (* this) [index] ;
 }  // const long & subject_With_LongIntArray ::
    //   itemAtIndex (size_t index) const

bool subject_With_LongIntArray ::
  smallestVal(const size_t index, const size_t indToExclude) const
{  // 14.aug.2002
  bool res = false;

  if(arrItems && index < arrItems)  // indToExclude MAY be >= arrItems
  {
    bool initialized = false;
    long int smallest = 0;

    for(size_t i = 0 ; i < arrItems ; i ++)
      if(i != indToExclude)
        if(initialized == false || smallest > itemAtIndex(i))
        {
          smallest = itemAtIndex(i);
          initialized = true;
        }
    if(initialized == true && index != indToExclude)
      if(smallest == itemAtIndex(index))
        res = true;
  }
  return res;
}  // subject_With_LongIntArray :: smallestVal

bool subject_With_LongIntArray ::
  biggestVal(const size_t index, const size_t indToExclude) const
{  // 14.aug.2002
  bool res = false;

  if(arrItems && index < arrItems)  // indToExclude MAY be >= arrItems
  {
    bool initialized = false;
    long int biggest = 0;

    for(size_t i = 0 ; i < arrItems ; i ++)
      if(i != indToExclude)
        if(initialized == false || biggest < itemAtIndex(i))
        {
          biggest = itemAtIndex(i);
          initialized = true;
        }
    if(initialized == true && index != indToExclude)
      if(biggest == itemAtIndex(index))
        res = true;
  }
  return res;
}  // subject_With_LongIntArray :: biggestVal

plainJudgeCheckup ::
  plainJudgeCheckup (TWindow * parent,
                     const begMYEndMYTypeCIU & rBMYEMYTCIU) :
    pArr (NULL), pParent (parent), bMYEMYTCIU (rBMYEMYTCIU)
 {
  pArr = new TISArray <subject_With_LongIntArray> ;
 }  // plainJudgeCheckup :: plainJudgeCheckup

plainJudgeCheckup :: ~plainJudgeCheckup ()
 {
  if (pArr)
    delete pArr ;
 }  // plainJudgeCheckup :: ~plainJudgeCheckup

void plainJudgeCheckup :: makeJudgesSet (TQuery & const q)
 {
// ----------------------------------------------------------------------
    bool findOut = false;

# if INSTANCE
#   if APPEAL
#   else
      // 20060612  a new report is asked - extended - all subjects and all judges
      if ((bMYEMYTCIU.types [0]) && (bMYEMYTCIU.incomingExtend))
      {
	    findOut = true;

          /*
select * from t_user where f_post in ('1', '2', 'j', 'y')
and f_flags in (1, 3) and not f_distribution[_#] is null;

	    */
          size_t critSz = 2 * 1024 ;
          char * crit = new char [critSz] ;
	    ostrstream oss (crit, critSz) ;

          oss << "SELECT * FROM T_USER U WHERE U.F_POST  IN ('1', '2', 'j', 'y') "
              << "AND U.F_FLAGS IN (1, 3) "
              << "AND( SELECT COUNT(F_SUBJECT) FROM T_USER_SUBJECT S "   //  20071005
              <<         "WHERE S.F_UCN=U.F_UCN AND S.F_TYPE "           //  20071005
              <<         "IN (" << stringToSQLCharSet (bMYEMYTCIU.types).getTheSet () << ") "//  20071005
              <<         ")>0;"                                          //  20071005
              << ends ;

          crit [critSz - 1] = '\0' ;
          TQuery qUsers (crit) ;

          TUser  c_user;

          while (qUsers.Read ())
          {
             c_user << qUsers ;

		 {
                  JudgeWithNames j ( c_user.ucn, UCN_CITIZEN_UCN) ;

                  if (j.ucnType)  // successfuly loaded
                  {
                      if (judgesSet.Find (& j) == INT_MAX)  // not found
                      {
                          TJudge * pNewJ = new JudgeWithNames ;  // TJudge ;
                          (* pNewJ) << j ;
                          judgesSet.Add (pNewJ) ;
                      }
                  }
             }
          }

          delete [] crit;
      }
      // 20060509 <--
#   endif
# endif

  if ( !findOut )
  // ----------------------------------------------------------------------
  { // from Petyo -->
    TLawsuit law ;

    while (q.Read ())
     {
       law << q ;

       JudgeWithNames j (law.judge, UCN_CITIZEN_UCN) ;  // TJudge

       if (j.ucnType)  // successfuly loaded
        {
         if (judgesSet.Find (& j) == INT_MAX)  // not found
          {
           TJudge * pNewJ = new JudgeWithNames ;  // TJudge ;  // 25.nov.2004

           (* pNewJ) << j ;
           judgesSet.Add (pNewJ) ;
          }
        }
      }
  } // from Petyo <--
 }  // plainJudgeCheckup :: makeJudgesSet

void plainJudgeCheckup :: makeArr (TQuery & const q,
                                   const bool first,         // 20060608
                                   const bool second )       // 20060608
 {
    bool findOut = false;

# if INSTANCE
#   if APPEAL
#   else
      // 20060509  hope of deliverance
      // 20060608  a new report is asked - extended - all subjects and all judges
      if ((bMYEMYTCIU.types [0]) && (bMYEMYTCIU.incomingExtend))
      {
          findOut = true;

          size_t critSz = 2 * 1024 ;
          char * crit = new char [critSz] ;
          ostrstream oss (crit, critSz) ;

          oss << "SELECT F_SUBJECT FROM T_SUBJECT WHERE F_TYPE IN (" <<
                  stringToSQLCharSet (bMYEMYTCIU.types).getTheSet () << ")";

          if (!(first && second))
          {
             oss << " AND F_KIND IN ("
                    << SQLCharSet(first ? KIND_1ST_LAWSUITS :
                                          KIND_2ND_LAWSUITS) << ")";
          }
          oss << " ;" << ends ;
          crit [critSz - 1] = '\0' ;
          TQuery qSubj (crit) ;
          TSubject subj ;

          while (qSubj.Read ())
          {
             subj << qSubj ;

             subject_With_LongIntArray tmp (0) ;

             tmp.subj = subj.subject ;
             if (pArr -> Find (& tmp) == INT_MAX)
                     pArr -> Add ( new subject_With_LongIntArray (
						   judgesSet.Count (),
                                       tmp.subj)
                                 ) ;
          }

          delete [] crit;
      }
      // 20060509 <--
#   endif
# endif

  if ( !findOut )
  {
     TLawsuit law ;

     while (q.Read ())
     {
         law << q ;

         subject_With_LongIntArray tmp (0) ;

         tmp.subj = law.subject ;
         if (pArr -> Find (& tmp) == INT_MAX)
             pArr -> Add ( new subject_With_LongIntArray (
					 judgesSet.Count (),
                               tmp.subj)
                          ) ;
     }
  }
  if (!(first && second))
  {}
}  // plainJudgeCheckup :: makeArr

void plainJudgeCheckup :: incCell (const long subject,
                                   const char * const ucn)
 {
  if (! ucn)
    return ;

  subject_With_LongIntArray key (0, subject) ;
  const int sInd = pArr -> Find (& key) ;

  if (sInd != INT_MAX && sInd < pArr -> Count ())
   {
    int jInd = INT_MAX ;

    for (int i = 0 ;
	   jInd == INT_MAX && i < judgesSet.Count () ;
         i ++)
      if (! strcmp (ucn, judgesSet [i] -> ucn))
        jInd = i ;
    if (jInd != INT_MAX && jInd < judgesSet.Count ())
      (* ((* pArr) [sInd])) [jInd] += 1 ;
   }
 }  // plainJudgeCheckup :: incCell

bool plainJudgeCheckup :: MakeCriteria (char * const crit,
                                        const size_t critSz,
                                        const CDate & bP,
                                        const CDate & eP)
 {  // 12.apr.2001
  if (! (bMYEMYTCIU.types [0] && crit && critSz))
    return false ;

  const bool wantRejudge =
    DISTRIB_COUNT_REJUDGE ?
    true : false;  // 11.dec.2003
  ostrstream oss (crit, critSz) ;

  oss << "SELECT F_SUBJECT, ";
    // "F_JUDGE"  // 11.dec.2003
  oss << (wantRejudge ? "F_REJUDGE AS F_JUDGE" : "F_JUDGE") << " ";//11.dec.2003
  oss <<
           "FROM T_LAWSUIT "
           "WHERE F_DATE "
             "BETWEEN " << bP.Value() << " AND " << eP.Value() << " AND "  // 15.apr.2003
	     "F_TYPE IN (" <<
             stringToSQLCharSet (bMYEMYTCIU.types).getTheSet ()
                         << ")" ;
  if (bMYEMYTCIU.composition)  // 16.feb.2001 - begin
    oss <<
           " AND F_COMPOSITION = "
        << bMYEMYTCIU.composition ;  // 16.feb.2001 - end
  oss << " ;" << ends ;
  crit [critSz - 1] = '\0' ;
  return criteria_finished_ok (crit, typeid (* this).name (),
                               "MakeCriteria") ;
 }  // plainJudgeCheckup :: MakeCriteria

void plainJudgeCheckup :: fillArr (const CDate & bP,
                                   const CDate & eP,
                                   const bool  first,        // 20060608
                                   const bool  second )      // 20060608
 {
  size_t critSz = 2 * 1024 ;
  char * crit = new char [critSz] ;

  if (MakeCriteria (crit, critSz, bP, eP))  // essential
   {
    TQuery q (crit) ;

    makeJudgesSet (q) ;
    q.Restart () ;
    makeArr (q, first, second) ;  // 20060608
    q.Restart () ;
     {  // 11.dec.2003
      LawPtr pLaw ;  // 11.dec.2003 - Performance issue

      while (q.Read ())
       {
        //TLawsuit law ;  // 11.dec.2003 - Performance issue

        pLaw.Ref () << q ;
        incCell (pLaw.Ref ().subject, pLaw.Ref ().judge) ;
       }
     }  // 11.dec.2003
   }
  delete [] crit ;
 }  // plainJudgeCheckup :: fillArr

void plainJudgeCheckup :: prepare ( const bool first, const bool second )  // 20060608
 {
  TWaitWindow ww (pParent, PLEASE_WAIT_EXTRACTING) ;

  CDate bP ;
  CDate eP ;

  bP.day = eP.day = 1 ;
  bP.month = bMYEMYTCIU.begMonth ;
  eP.month = bMYEMYTCIU.endMonth ;
  bP.year = bMYEMYTCIU.begYear ;
  eP.year = bMYEMYTCIU.endYear ;
  bP = firstDayOfThisMonth (bP) ;
  eP = lastDayOfThisMonth (eP) ;
  fillArr (bP, eP, first, second) ;                            // 20060608
 }  // plainJudgeCheckup :: prepare

void plainJudgeCheckup ::
  makeGeneralRow (subject_With_LongIntArray & rSWJHA)
 {
  for (int sInd = 0 ; sInd < pArr -> Count () ; sInd ++)
    for (int jInd = 0 ; jInd < judgesSet.Count () ; jInd ++)
      rSWJHA [jInd] += (* ((* pArr) [sInd])) [jInd] ;
 }  // plainJudgeCheckup :: makeGeneralRow

void plainJudgeCheckup :: makeFmtFN (const begMYEndMYTypeCIU & bMYEMYTCIU,
                                     char * const fmtFN,
                                     const size_t fmtFNSz)
 {
# if INSTANCE
  const collCompBegDEndDBegN forSyntaxOnly ;
# endif

  makeJudgeCheckupFormatFN (fmtFN, fmtFNSz, true, bMYEMYTCIU
# if INSTANCE
                            , forSyntaxOnly
# endif
				   ) ;
 }  // plainJudgeCheckup :: makeFmtFN

void plainJudgeCheckup::printPlainJudgeCheckup()
{
	char formatFN[128];
	TWaitWindow ww(pParent, PLEASE_WAIT_GENERATING);

	makeFmtFN(bMYEMYTCIU, formatFN, sizeof (formatFN));

	int linesWritten = 0 ;
	plainJudgeCheckupOutFile outF(pParent, formatFN, judgesSet.Count());

	outF.title();
	outF.formattedHead(&bMYEMYTCIU);
	outF.topLine();
	outF.writePlainItem(judgesSet);

	for (int i = 0 ; i < pArr->Count(); i++)
	{
		if (outF.pageLinesLeft () < outF.writePlainItem (* ((* pArr) [i]), bMYEMYTCIU.types, true, false) + 1)
		{
			if (linesWritten < outF.pageLinesUsed())
				linesWritten = outF.pageLinesUsed();

			outF.footherLine();
			outF.newPage();
			outF.topLine();				// 20060518
			outF.writePlainItem(judgesSet);	// 20060518
			//outF.topLine();				// 20060724
			outF.horzLine ();				// 20060724
		}
		else
			outF.horzLine();

		outF.writePlainItem (* ((* pArr) [i]), bMYEMYTCIU.types) ;
	}

	{
		subject_With_LongIntArray summaryRow(judgesSet.Count());

		makeGeneralRow(summaryRow);

		if (outF.pageLinesLeft() < outF.writePlainItem(summaryRow, bMYEMYTCIU.types, false, false) + 1)
		{
			if (linesWritten < outF.pageLinesUsed())
				linesWritten = outF.pageLinesUsed();

			outF.footherLine();
			outF.newPage();
			outF.topLine();
		}
		else
			outF.horzLine () ;

		outF.writePlainItem (summaryRow, bMYEMYTCIU.types, false) ;
     }

	outF.footherAndTail () ;

	if (linesWritten < outF.pageLinesUsed())
		linesWritten = outF.pageLinesUsed();

	if (linesWritten)
	{
		// Nothing. I'm suppressing the warning
	}

	show_text_stream_use_settings(NULL, outF, formatFN, FLOAT_ALIGNMENT, SPARE_PAGE_LINES);
}

//=============================================================================

DecidedLawsPlainJudgeCheckup ::
DecidedLawsPlainJudgeCheckup (TWindow * parent,
                              const begMYEndMYTypeCIU & rBMYEMYTCIU) :
                plainJudgeCheckup (parent, rBMYEMYTCIU)
 {
 }  // DecidedLawsPlainJudgeCheckup :: DecidedLawsPlainJudgeCheckup

bool DecidedLawsPlainJudgeCheckup ::
       MakeNonFirmCriteria (char * const crit, const size_t critSz,
                            const CDate & bP, const CDate & eP)
 {
  if (! bMYEMYTCIU.types [0])
    return false ;

  ostrstream oss (crit, critSz) ;

  oss << "SELECT L.F_SUBJECT, S.F_JUDGE FROM "
         "T_LAWSUIT L, T_SESSION S "
         "WHERE "
	   "L.F_TYPE IN (" <<	// 2014:209 L.
		 stringToSQLCharSet (bMYEMYTCIU.types).getTheSet ()
				   << ") AND "
	   "L.F_TYPE != \'" << TYPE_FIRM_LAW << "\' AND "
	   "L.F_FINISHED BETWEEN " << bP.Value() << " AND " << eP.Value() << " " ;
  if (bMYEMYTCIU.composition)
    oss <<
	   " AND L.F_COMPOSITION = " << bMYEMYTCIU.composition ;
  oss << "AND S.F_TYPE = L.F_TYPE "
	   "AND S.F_YEAR = L.F_YEAR "
	   "AND S.F_NO = L.F_NO "
	 "AND S.F_RESULT IN ("
	 // 2008:301 LPR: COMPLETIZEDS + F_FINAL
		 << stringToSQLCharSet (RESULT_COMPLETIZEDS).getTheSet ()
			<< ") AND NOT S.F_FINAL IS NULL "
	 "AND S.F_DATE <= L.F_FINISHED "
	   "AND (L.F_DEC_MADE IS NULL OR L.F_DEC_MADE <= S.F_DATE)" ;
  oss << " ;" << ends ;
  crit [critSz - 1] = '\0' ;
  return criteria_finished_ok (crit,
		   "DecidedLawsPlainJudgeCheckup :: MakeNonFirmCriteria") ;
 }  // DecidedLawsPlainJudgeCheckup :: MakeNonFirmCriteria

bool DecidedLawsPlainJudgeCheckup :: MakeCriteria (char * const crit,
								   const size_t critSz,
                                                   const CDate & bP,
                                                   const CDate & eP)
 {
  if (! bMYEMYTCIU.types [0])
    return false ;
  return MakeNonFirmCriteria (crit, critSz, bP, eP) ;
 }  // DecidedLawsPlainJudgeCheckup :: MakeCriteria

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
DecidedFirmLawsPlainJudgeCheckup ::
DecidedFirmLawsPlainJudgeCheckup (TWindow * parent,
                                  const begMYEndMYTypeCIU & rBMYEMYTCIU) :
                plainJudgeCheckup (parent, rBMYEMYTCIU)
 {
 } // DecidedFirmLawsPlainJudgeCheckup :: DecidedFirmLawsPlainJudgeCheckup

bool DecidedFirmLawsPlainJudgeCheckup ::
       MakeFinishedLawsCriteria (char * const crit, const size_t critSz,
                                 const CDate & bP, const CDate & eP)
 {
  if (! bMYEMYTCIU.types [0])
    return false ;

  ostrstream oss (crit, critSz) ;

  oss << "SELECT L.F_SUBJECT, S.F_JUDGE FROM "
	   "T_LAWSUIT L, T_SESSION S "
	   "WHERE "
	   "L.F_TYPE = \'" << TYPE_FIRM_LAW << "\' AND "
	   "L.F_FINISHED BETWEEN " << bP.Value() << " AND " << eP.Value() << " " ;
  if (bMYEMYTCIU.composition)
    oss <<
	   " AND L.F_COMPOSITION = " << bMYEMYTCIU.composition ;
  oss << "AND S.F_TYPE = L.F_TYPE "
	   "AND S.F_YEAR = L.F_YEAR "
	   "AND S.F_NO = L.F_NO "
	   "AND S.F_RESULT IN ("
    << stringToSQLCharSet (RESULT_FIRM_LEGAL_FINALIZEDS).getTheSet ()
				<< ") "
	   "AND S.F_DATE <= L.F_FINISHED " ;
  oss << " ;" << ends ;
  crit [critSz - 1] = '\0' ;
  return criteria_finished_ok (crit,
	  "DecidedFirmLawsPlainJudgeCheckup :: MakeFinishedLawsCriteria") ;
 }  // DecidedFirmLawsPlainJudgeCheckup :: MakeFinishedLawsCriteria

bool DecidedFirmLawsPlainJudgeCheckup ::
MakeRegisteredFirmsCriteria (char * const crit, const size_t critSz,
                             const CDate & bP, const CDate & eP)
 {
  if (! bMYEMYTCIU.types [0])
    return false ;

  ostrstream oss (crit, critSz) ;

  oss << "SELECT L.F_SUBJECT, S.F_JUDGE FROM "
	   "T_LAWSUIT L, T_SESSION S, T_DECISION D "
	   "WHERE "
	   "D.F_TYPE = \'" << TYPE_FIRM_LAW << "\' AND "
	   "D.F_ENTER_DATE BETWEEN " << bP.Value() << " AND "
					     << eP.Value() << " AND "
	   "D.F_RESULT IN ("
  << stringToSQLCharSet (RESULT_FIRM_REGISTRATIONS).getTheSet ()
			  << ") AND "
	   "S.F_TYPE = D.F_TYPE AND "
	   "S.F_YEAR = D.F_YEAR AND "
	   "S.F_NO = D.F_NO AND "
	   "S.F_DATE = D.F_DATE AND "
	   "S.F_KIND = D.F_SESSION_KIND AND "
	   "L.F_TYPE = S.F_TYPE AND "
	   "L.F_YEAR = S.F_YEAR AND "
	   "L.F_NO = S.F_NO AND "
	   "L.F_FINISHED IS NULL OR S.F_DATE <= L.F_FINISHED " ;
  if (bMYEMYTCIU.composition)
    oss <<
	   " AND L.F_COMPOSITION = " << bMYEMYTCIU.composition ;
  oss << " ;" << ends ;
  crit [critSz - 1] = '\0' ;
  return criteria_finished_ok (crit,
     "DecidedFirmLawsPlainJudgeCheckup :: MakeRegisteredFirmsCriteria") ;
 }  // DecidedFirmLawsPlainJudgeCheckup :: MakeRegisteredFirmsCriteria

void DecidedFirmLawsPlainJudgeCheckup ::
FillArr_ (const CDate & bP, const CDate & eP, const int which)
 {
  size_t critSz = 2 * 1024 ;
  char * crit = new char [critSz] ;
  bool criteriaOK = false ;

  switch (which)
   {
    case 0 :
      criteriaOK = MakeFinishedLawsCriteria (crit, critSz, bP, eP) ;
      break ;
    case 1 :
      criteriaOK = MakeFinishedLawsCriteria (crit, critSz, bP, eP) ;
      break ;
   }
  if (criteriaOK)
   {
    TQuery q (crit) ;

    makeJudgesSet (q) ;
    q.Restart () ;
    makeArr (q) ;
    q.Restart () ;
    while (q.Read ())
     {
      TLawsuit law ;

      law << q ;
      incCell (law.subject, law.judge) ;
     }
   }
  delete [] crit ;
 }  // DecidedFirmLawsPlainJudgeCheckup :: FillArr_

void DecidedFirmLawsPlainJudgeCheckup :: fillArr (const CDate & bP,
                                              const CDate & eP,
                                              const bool first, const bool second )
 {
  FillArr_ (bP, eP, 0) ;
  FillArr_ (bP, eP, 1) ;
  if (first) {}
  if (second) {}
 }  // DecidedFirmLawsPlainJudgeCheckup :: fillArr
#   endif  // of APPEAL
# endif  // of INSTANCE

Date_With_LongIntArray::Date_With_LongIntArray(const size_t lonsgCnt) :
  subject_With_LongIntArray(lonsgCnt)
{
  clear_CDate(d);
}  // Date_With_LongIntArray::Date_With_LongIntArray

Date_With_LongIntArrayR::Date_With_LongIntArrayR(const size_t lonsgCnt) :
  Date_With_LongIntArray(lonsgCnt)
{
}  // Date_With_LongIntArrayR::Date_With_LongIntArrayR

bool operator<(const Date_With_LongIntArrayR& j1,
  const Date_With_LongIntArrayR& j2)
{
  return j1.d > j2.d;  // Reversed order
}  // bool operator==(const Date_With_LongIntArrayR& j1,
   //  const Date_With_LongIntArrayR& j2)

#if INSTANCE
#  if APPEAL
#  else  // of APPEAL
InstancePlainJudgeCheckup::InstancePlainJudgeCheckup(TWindow* parent,
  const begMYEndMYTypeCIU& rBMYEMYTCIU) :
    plainJudgeCheckup(parent, rBMYEMYTCIU)
{
}  // InstancePlainJudgeCheckup::InstancePlainJudgeCheckup

bool InstancePlainJudgeCheckup::MakeCriteria(char* const crit,
  const size_t critSz, const CDate& bP, const CDate& eP)
{
  if (bMYEMYTCIU.types[0] && crit && critSz)
  {
    ostrstream oss(crit, critSz);
    const bool wantRejudge =
      DISTRIB_COUNT_REJUDGE ?
      true : false;  // 11.dec.2003

    oss <<
      "SELECT F_SUBJECT, ";
    // "F_JUDGE"  // 11.dec.2003
    oss << (wantRejudge ? "F_REJUDGE AS F_JUDGE" : "F_JUDGE");  // 11.dec.2003
    oss <<
	", F_DATE "
	"FROM T_LAWSUIT "
	"WHERE F_DATE BETWEEN " << bP.Value() <<" AND " << eP.Value() << " AND " //15.apr.2003
	  "F_TYPE IN (" << SQLCharSet(bMYEMYTCIU.types) << ") AND "
        "F_KIND IN ("
        << SQLCharSet(bMYEMYTCIU.firstInstLaws ? KIND_1ST_LAWSUITS :
            KIND_2ND_LAWSUITS) << ")";
    if(bMYEMYTCIU.composition)
      oss << " AND F_COMPOSITION = " << bMYEMYTCIU.composition;
    oss << " ;" << ends;
    crit[critSz - 1] = '\0';
    return criteria_finished_ok(crit, typeid(*this).name(),"MakeCriteria");
  }
  else
    return false;
}  // InstancePlainJudgeCheckup::MakeCriteria
      
#  endif  // of APPEAL
#endif   // of INSTANCE
