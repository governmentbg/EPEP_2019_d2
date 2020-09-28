#include "cac.h"

const char
	PSEUDO_GRAPHIC_HORIZONTAL	= 151,
	PSEUDO_GRAPHIC_VERTICAL		= 166,
	PSEUDO_GRAPHIC_UP_LEFT		= 171,
	PSEUDO_GRAPHIC_UP_T		= 147,
	PSEUDO_GRAPHIC_UP_RIGHT		= 187,
	PSEUDO_GRAPHIC_RIGHT_T		= 146,
	PSEUDO_GRAPHIC_DOWN_RIGHT	= 155,
	PSEUDO_GRAPHIC_DOWN_T		= 132,
	PSEUDO_GRAPHIC_DOWN_LEFT	= 139,
	PSEUDO_GRAPHIC_LEFT_T		= 145,
	PSEUDO_GRAPHIC_CROSS		= 134;

void pageManager :: setParams (int pL, int l)
 {
  pgLines = pL ;
  lines = l ;
 }  // pageManager :: setParams

pageManager :: pageManager (int pL, int l)
 {
  setParams (pL, l) ;
 }  // pageManager :: pageManager

pageManager :: pageManager ()
 {
  setParams (0, 0) ;
 }  // pageManager :: pageManager

int pageManager :: linesLeft () const
 {
  return pgLines - lines ;
 }  // pageManager :: linesLeft

int pageManager :: getLines () const
 {
  return lines ;
 }  // pageManager :: getLines

int pageManager :: getPgLines () const
 {
  return pgLines ;
 }  // pageManager :: getPgLines

void pageManager :: setLines (int n)
 {
  lines = n ;
 }  // pageManager :: setLines

void pageManager :: incLines (int n)
 {
  lines += n ;
 }  // pageManager :: incLines

bool pageManager :: newPageNeed () const
 {
  return (lines >= pgLines) ;
 }  // pageManager :: newPageNeed

bool pageManager :: enoughLinesLeft (int n) const
 {
  return (linesLeft () >= n) ;
 }  // pageManager :: enoughLinesLeft

bool check_ifstream (ifstream & fs, const char * const whatFile,
                     const char * const where)
 {
  const bool res = fs && fs.good () ;
  
  if (res == false)
    error ("%s:\n"
           "%s файл не е отворен или е възникнала грешка "
           "при обработката му.", 
           where ? where : "", whatFile ? whatFile : "") ;
  return res ;
 }  // check_ifstream

bool seek_parameters_section(ifstream &ifs)
{
	bool sectionFound = false;

	if (check_ifstream(ifs, "Форматният", "seek_parameters_section"))
	{
		if (WINDOWS_PRINT)
		{
			if (WINDOWS_A3)
				sectionFound = findSection (ifs, " ", WINDOWS_A3_PARAMETERS_SECTION, false);
			else
				sectionFound = findSection (ifs, " ", WINDOWS_PARAMETERS_SECTION, false) ;
		}

		if (sectionFound == false)
			sectionFound = findSection(ifs, " ", PARAMETERS_SECTION);
	}

	return sectionFound ;
}

void load_format_parameters (ifstream & ifs, int & hLL, int & pageLines,
                             int & itemLines, int & itemLines1,
                             unsigned long int & extraVal,
                             unsigned long int & extraVal1,
                             int & alignment, int & justification)
 {
  int b, c, d, e ;
  unsigned long int f, g ;
  int h, eV3 ;  // .oct.2001

  b = c = d = e =0 ;
  f = g = 0 ;
  h = eV3 = 0 ;
  if (check_ifstream (ifs, "Форматният", "load_format_parameters"))
   {  // -- 1 --
    ifs >> b >> c >> d >> e ;
    if (ifs.good ())
     {
      if (b)
        hLL = b ;
      if (c)
        pageLines = c ;
      if (d)
        itemLines = d ;
      if (e)
        itemLines1 = e ;
      ifs >> f ;
      if (ifs.good ())
       {
        extraVal = f ;
        ifs >> g ;
        if (ifs.good ())
         {
          extraVal1 = g ;
          ifs >> h ;
          if (ifs.good ())
           {
            alignment = h ;  // .oct.2001 (before 1.1078)
            ifs >> eV3 ;  // 17.oct.2001 - begin
            if (ifs.good ())
              justification = eV3 ;  // 17.oct.2001 - end
           }
         }
       }
      if (itemLines == -1)
        itemLines = 0 ;
      if (itemLines1 == -1)
        itemLines1 = 0 ;
     }
    ifs.clear () ;
    if (pageLines >= 0)
	pageLines = DEFAULT_PAGE_LEN - 5;
    else
      pageLines = - pageLines ;
   }  // -- 1 --
 }  // load_format_parameters

// 20051010 -->
# if ! INSTANCE
bookZZDNOutFile :: bookZZDNOutFile (TWindow * parent,
						const char * hFName) :
			    frmHeadOutText (parent, hFName)
 {
 }  // bookZZDNOutFile :: bookZZDNOutFile

void bookZZDNOutFile :: fHOutput(const void* p, char* s, size_t len, int algn,
  size_t skip, int dCipher) // processes codes from FORMATED_HEADER
{
	if(p && s)
  {
    const ZZDN_Dlg_data * ptr =
	reinterpret_cast<const ZZDN_Dlg_data *>(p) ;

    if (len && s && ptr )
    {
	 switch (((long int) dCipher) - OFFSET_BEGN_ENDN_YEAR_AC_CIPHER)
	 {
	    case 66 :
	     s [0] = '\0' ;
	     {
		 ostrstream oss (s, len + 1) ;
		 if ((ptr->selectCrit1)&&(ptr->FromNo>0)&&(ptr->ToNo>0))
		    oss << "От No " << ptr->FromNo << " до No "
					  << ptr->ToNo << " за " << ptr->Year << "г." ;
		 else
		    oss << "От " << ptr->begD << "г. до "
				     << ptr->endD << "г." ;
		 oss << ends ;
		 s [len] = '\0' ;
	     }
	     ::writeString ((* this) , s, len, algn, skip) ;
           break ;
       }
  } }
}  // bookZZDNOutFile :: fHOutput

void bookZZDNOutFile :: toBeContinued (const void * const anyData)
 {
   toBeContinuedWithoutEndl () ;
   if (anyData)
      {
      const char * const pSL = (const char *) anyData ;

      (* this) << " (" << pSL << ")" << endl ;
      pM.incLines () ;
     }
 }  // bookZZDNOutFile :: toBeContinued

void bookZZDNOutFile :: writeItem (BookZZDNColumns & tBC, bool first)
 {
    if (pM.newPageNeed ())
     {
      char tmp [64] ;

      //tBC.sessionLabel (tmp, sizeof (tmp)) ;
      toBeContinued (tmp) ;
     }
    tBC.notConstOutput (* this, first) ;
    pM.incLines (linesOfItem (first)) ;
 }  // bookZZDNOutFile :: writeItem

int bookZZDNOutFile :: formattedHead(const ZZDN_Dlg_data * cCBDEDBNPtr_ZZDN,
  bool write, ostream* altStreamPtr)  // 02.feb.2005 altStreamPtr
{
  int res = 0;

  pAltStream = altStreamPtr;
  try
    {
      res = formHead(cCBDEDBNPtr_ZZDN, write);
      // Simulate text output - in order to have a rough approximation
      // to the length of the page.
      if(write == false && pAltStream != NULL)
      {
        pM.incLines(res);
      }
  }
  CATCH_ANYTHING
  pAltStream = NULL;
  return res;
}  // bookZZDNOutFile :: formattedHead

void bookZZDNOutFile :: nextSToPrint (int linesNeed,
                                      const ZZDN_Dlg_data * cCBDEDBNPtr_ZZDN,
                                      BookZZDNColumns * pTBC,
                                      bool * pDateRowWritten
                                     )
 {
  if (! pM.enoughLinesLeft (linesNeed))
   {
    footherLine () ;
    newPage () ;
    formattedHead (cCBDEDBNPtr_ZZDN) ;
    head () ;
    if (pDateRowWritten)
      (* pDateRowWritten) = false ;
    if (pTBC)
     {
      mediumFootherLine () ;
      writeItem (* pTBC, false) ;
      mediumTopLine () ;
      if (pDateRowWritten)
        (* pDateRowWritten) = true ;
     }
   }
 }  // bookZZDNOutFile :: nextSToPrint

size_t bookZZDNOutFile :: minRowsForSess ()
 {
  return (size_t) extraVal1 ;
 }  // bookZZDNOutFile :: minRowsForSess

void bookZZDNOutFile :: oneSessPrtStartFinalChar (const bool start,
                                                  const bool newLBefore,
                                                  const bool newLAfter)
 {
  const char ch = start ?
                  (char) (extraVal / 256) : (char) (extraVal % 256) ;

  if (newLBefore)
    (* this) << endl ;
  if (ch)
    (* this) << ch ;
  if (newLAfter)
    (* this) << endl ;
 }  // bookZZDNOutFile :: oneSessPrtStartChar

void bookZZDNOutFile :: IncItemLines(bool first)
{  // 02.feb.2005
  pM.incLines(linesOfItem(first));
}

void bookZZDNOutFile :: IncLines(int n)
{  // 02.feb.2005
  pM.incLines(n);
}

void bookZZDNOutFile :: SetPageLines(int n)
{  // 09.feb.2005
  pM.setParams(n, pM.getLines());
}
# endif
// 20051010 <--

timeBookOutFile :: timeBookOutFile (TWindow * parent,
						const char * hFName) :
                      frmHeadOutText (parent, hFName)
 {
 }  // timeBookOutFile :: timeBookOutFile

void timeBookOutFile::fHOutput(const void* p, char* s, size_t len, int algn,
  size_t skip, int dCipher)
{  // An old varianth in VersionArchives\2365\_appname.j --> _outfile.cpp
  if(p != NULL)
  {
    const collCompBegDEndDBegN* pCCBDEDBN =
      reinterpret_cast<const collCompBegDEndDBegN*>(p);
    ostream* osPtr = pAltStream ? pAltStream : this;

    collCompBegDEndDBegNOutput(*pCCBDEDBN, *osPtr, s, len, algn, skip, dCipher);
  }
}  // timeBookOutFile :: fHOutput

void timeBookOutFile :: toBeContinued (const void * const anyData)
 {
  toBeContinuedWithoutEndl () ;
  if (anyData)
   {
    const char * const pSL = (const char *) anyData ;

    (* this) << " (" << pSL << ")" << endl ;
    pM.incLines () ;
   }
 }  // timeBookOutFile :: toBeContinued

void timeBookOutFile :: writeItem (timeBookColumns & tBC, bool first)
 {
  if (pM.newPageNeed ())
   {
    char tmp [64] ;

    tBC.sessionLabel (tmp, sizeof (tmp)) ;
    toBeContinued (tmp) ;
   }
  tBC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // timeBookOutFile :: writeItem

int timeBookOutFile::formattedHead(const collCompBegDEndDBegN* cCBDEDBNPtr,
  bool write, ostream* altStreamPtr)  // 02.feb.2005 altStreamPtr
{  // An old varianth in VersionArchives\2365\_appname.j --> _outfile.cpp
  int res = 0;

  pAltStream = altStreamPtr;
  try
  {
    res = formHead(cCBDEDBNPtr, write);
    // Simulate text output - in order to have a rough approximation
    // to the length of the page.
    if(write == false && pAltStream != NULL)
    {
      pM.incLines(res);
    }
  }
  CATCH_ANYTHING
  pAltStream = NULL;
  return res;
}  // timeBookOutFile :: formattedHead

void timeBookOutFile :: nextSToPrint (int linesNeed,
                                      const collCompBegDEndDBegN *
                                                          cCBDEDBNPtr,
                                      timeBookColumns * pTBC,
                                      bool * pDateRowWritten
                                     )
 {
  if (! pM.enoughLinesLeft (linesNeed))
   {
    footherLine () ;
    newPage () ;
    formattedHead (cCBDEDBNPtr) ;
    head () ;
    if (pDateRowWritten)
      (* pDateRowWritten) = false ;
    if (pTBC)
     {
      mediumFootherLine () ;
      writeItem (* pTBC, false) ;
      mediumTopLine () ;
      if (pDateRowWritten)
        (* pDateRowWritten) = true ;
     }
   }
 }  // timeBookOutFile :: nextSToPrint

size_t timeBookOutFile :: minRowsForSess ()
 {
  return (size_t) extraVal1 ;
 }  // timeBookOutFile :: minRowsForSess

void timeBookOutFile :: oneSessPrtStartFinalChar (const bool start,
                                                  const bool newLBefore,
                                                  const bool newLAfter)
 {
  const char ch = start ?
                  (char) (extraVal / 256) : (char) (extraVal % 256) ;

  if (newLBefore)
    (* this) << endl ;
  if (ch)
    (* this) << ch ;
  if (newLAfter)
    (* this) << endl ;
 }  // timeBookOutFile :: oneSessPrtStartChar

void timeBookOutFile::IncItemLines(bool first)
{  // 02.feb.2005
  pM.incLines(linesOfItem(first));
}

void timeBookOutFile::IncLines(int n)
{  // 02.feb.2005
  pM.incLines(n);
}

void timeBookOutFile::SetPageLines(int n)
{  // 09.feb.2005
  pM.setParams(n, pM.getLines());
}

timeBookSummaryOutFile :: timeBookSummaryOutFile (TWindow * parent,
								  const char * hFName
             , const CodeValueArray * const pCodeValueArr  // 19.jun.2000
# if INSTANCE
                                                                     ,
                            const TISArray <reportData>
                              * const pRpDataArr
# endif
                                                 ) :
				    frmHeadOutText (parent, hFName)
				    , pCodeValArr (pCodeValueArr)  // 19.jun.2000
# if INSTANCE
											,
				    pRDArr (pRpDataArr)
# endif
 {
 }  // timeBookSummaryOutFile :: timeBookSummaryOutFile

void timeBookSummaryOutFile :: fHOutput (const void * p, char * s,
						     size_t len, int algn,
						     size_t skip, int dCipher)
 {
  if (! (s && p))
    return ;
  // 2015:198 LPR: FIX: don't sort summaries
  collCompBegDEndDBegN q = * (const collCompBegDEndDBegN *) p;
  q.orderBy1 = q.orderBy2 = false;
  if (! (collCompBegDEndDBegNOutput (* ((const collCompBegDEndDBegN *) p),
						 * this, s, len, algn, skip,
						 dCipher) ||
	   tSCountOutput (&q, * this,
				len, algn, skip, dCipher
				, pCodeValArr  // 19.jun.2000
# if INSTANCE
								, pRDArr
# endif
			     ) ||
	   miscOutput (* this, s, len, algn, skip, dCipher) ||
	   regionalArealOutput (* this, s, len, algn, skip, dCipher)))
   {
    mkStrBlank (s, len + 1) ;
    writeString (s) ;
   }
 }  // timeBookSummaryOutFile :: fHOutput

void timeBookSummaryOutFile :: formattedHead (const
								collCompBegDEndDBegN *
										cCBDEDBNPtr,
							const bool write,
							int * pCodesNum,
							TWaitThread * pDLThrd,
							TWaitThreadDialog *wtdl
						     )
 {
//  formHead (cCBDEDBNPtr, true) ;  // 14.feb.2002 - begin
  formHead (cCBDEDBNPtr, write, FORMATTED_HEADER_SECTION,
		pCodesNum, pDLThrd, wtdl) ;  // 14.feb.2002 - end
 }  // timeBookSummaryOutFile :: formattedHead

judgeCheckupOutFile :: judgeCheckupOutFile (TWindow * parent,
							  const char * hFName) :
				 frmHeadOutText (parent, hFName)
 {
 }  // judgeCheckupOutFile :: judgeCheckupOutFile

void judgeCheckupOutFile :: fHOutput (const void * p, char * s,
						  size_t len, int algn,
						  size_t skip, int dCipher)
 {
  if (! begMYEndMYTypeCIUOutput (* ((const begMYEndMYTypeCIU *) p),
					   * this, s, len, algn, skip, dCipher))
    if (! miscOutput (* this, s, len, algn, skip, dCipher))
     {
	collCompBegDEndDBegN tmp ;

	(* ((const begMYEndMYTypeCIU *) p)) >> tmp ;
	collCompBegDEndDBegNOutput (tmp, * this, s, len, algn, skip,
					    dCipher) ;
     }
 }  // judgeCheckupOutFile :: fHOutput

int judgeCheckupOutFile :: formattedHead (const begMYEndMYTypeCIU
								  * bMYEMYTCIUPtr,
							bool write)
 {
  return formHead (bMYEMYTCIUPtr, write) ;
 }  // judgeCheckupOutFile :: formattedHead

bool judgeCheckupOutFile :: supportsFormatterTail () const // 04.apr.2001
 {
  return false ;
 }  // judgeCheckupOutFile :: supportsFormatterTail

int judgeCheckupOutFile :: formattedTail (const begMYEndMYTypeCIU
                                                  * bMYEMYTCIUPtr,
                                          bool write)  // 04.apr.2001
 {
  if (supportsFormatterTail ())
    return formHead (bMYEMYTCIUPtr, write, FORMATTED_TAIL_SECTION) ;
  else
    return 0 ;  
 }  // judgeCheckupOutFile :: formattedHead

void judgeCheckupOutFile :: writeItem (judgeCheckupColumns & rJCC,
                                       bool first)
 {
  rJCC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // judgeCheckupOutFile :: writeItem

# if INSTANCE
#   if APPEAL
IlievaJudgeCheckupOutFile ::
IlievaJudgeCheckupOutFile (TWindow * parent,
				   const char * hFName) :
			   judgeCheckupOutFile (parent, hFName)
 {
 }  // IlievaJudgeCheckupOutFile :: IlievaJudgeCheckupOutFile

void IlievaJudgeCheckupOutFile :: writeItem (judgeCheckupColumns & rJCC,
							   bool first)
 {
  IlievaJudgeCheckupColumns * pIJCC =
    dynamic_cast <IlievaJudgeCheckupColumns *> (& rJCC) ;

  if (pIJCC)
   {
    pIJCC -> notConstOutput (* this, first) ;
    pM.incLines (linesOfItem (first)) ;
   }
 }  // IlievaJudgeCheckupOutFile :: writeItem

#   else  // of APPEAL
KracholovJudgeCheckupOutFile ::
KracholovJudgeCheckupOutFile (TWindow * parent,
					const char * hFName) :
			   judgeCheckupOutFile (parent, hFName)
 {
 }  // KracholovJudgeCheckupOutFile :: KracholovJudgeCheckupOutFile

void KracholovJudgeCheckupOutFile ::
	 writeItem (judgeCheckupColumns & rJCC, bool first)
 {
  KracholovJudgeCheckupColumns * pKJCC =
    dynamic_cast <KracholovJudgeCheckupColumns *> (& rJCC) ;

  if (pKJCC)
   {
    pKJCC -> notConstOutput (* this, first) ;
    pM.incLines (linesOfItem (first)) ;
   }
 }  // KracholovJudgeCheckupOutFile :: writeItem

PdCitJudgeCheckupOutFile ::
PdCitJudgeCheckupOutFile (TWindow * parent,
					const char * hFName) :
			   judgeCheckupOutFile (parent, hFName)
 {
 }  // PdCitJudgeCheckupOutFile :: PdCitJudgeCheckupOutFile

void PdCitJudgeCheckupOutFile ::
	 writeItem (judgeCheckupColumns & rJCC, bool first)
 {
  PdCitJudgeCheckupColumns * pPCJCC =
    dynamic_cast <PdCitJudgeCheckupColumns *> (& rJCC) ;

  if (pPCJCC)
   {
    pPCJCC -> notConstOutput (* this, first) ;
    pM.incLines (linesOfItem (first)) ;
   }
 }  // PdCitJudgeCheckupOutFile :: writeItem

FirmJudgeCheckupOutFile ::
FirmJudgeCheckupOutFile (TWindow * parent,
				 const char * hFName) :
			   judgeCheckupOutFile (parent, hFName)
 {
 }  // FirmJudgeCheckupOutFile :: FirmJudgeCheckupOutFile

void FirmJudgeCheckupOutFile ::
	 writeItem (judgeCheckupColumns & rJCC, bool first)
 {
  FirmJudgeCheckupColumns * pFJCC =
    dynamic_cast <FirmJudgeCheckupColumns *> (& rJCC) ;

  if (pFJCC)
   {
    pFJCC -> notConstOutput (* this, first) ;
    pM.incLines (linesOfItem (first)) ;
   }
 }  // FirmJudgeCheckupOutFile :: writeItem

bool FirmJudgeCheckupOutFile :: supportsFormatterTail () const
 {
  return true ;
 }  // FirmJudgeCheckupOutFile :: supportsFormatterTail

#   endif  // of APPEAL
# endif  // of INSTANCE

summaryDataOutFile :: summaryDataOutFile (TWindow * parent,
							const char * hFName) :
			     frmHeadOutText (parent, hFName)
 {
 }  // summaryDataOutFile :: summaryDataOutFile

void summaryDataOutFile :: fHOutput (const void * p, char * s,
                                    size_t len, int algn, size_t skip,
                                    int dCipher)
 {
  if (! p)
    return ;

  const summaryFmtHeadData * const pSHD =
                                         (const summaryFmtHeadData *) p ;

  if (! begNEndNYearOutput (pSHD -> bNENY, * this, s,
                            len, algn, skip, dCipher))
    collCompBegDEndDBegNOutput (pSHD -> cCBDEDBN, * this, s,
                                len, algn, skip, dCipher) ;
 }  // summaryDataOutFile :: fHOutput

void summaryDataOutFile :: writeItem (summaryColumns & pSC, bool first)
 {
  if (pM.newPageNeed ())
    toBeContinued () ;
  pSC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // summaryDataOutFile :: writeItem

int summaryDataOutFile :: formattedHead (const summaryFmtHeadData & sFHD,
                                         bool write)
 {
  return (formHead (& sFHD, write)) ;
 }  // summaryDataOutFile :: formattedHead

void summaryDataOutFile :: nextItemToPrint (int linesNeed,
                             const summaryFmtHeadData & sFHD) 
 {
  if (! pM.enoughLinesLeft (linesNeed))
   {
    footherLine () ;
    newPage () ;
    formattedHead (sFHD) ;
    head () ;
   }
 }  // summaryDataOutFile :: nextItemToPrint

size_t plainJudgeCheckupOutFile :: vertLines ()
 {
  return judgesCnt
           + 1  // For the column 'Subject'
           + 1  // For the column 'Name of the Subject'
           + 1  // The left vert line
          ;
 }  // plainJudgeCheckupOutFile :: vertLines

size_t plainJudgeCheckupOutFile :: numberColumnWidth ()
 {
  const int allColumnsWidth = hLL - vertLines () ;
  const int clmnNumber =
      judgesCnt
        + 1  // For the column 'Subject'
	  + 1 * subjNameColumnWidthCoef  // For the column
                                       // 'Name of the Subject'
        ;

  if (allColumnsWidth >= clmnNumber)
   {
    const int cW = allColumnsWidth / clmnNumber ;

    if (extraVal1)
	return cW < extraVal1 ? cW : extraVal1 ;
    else
      return cW < 10 ? cW : 10 ;
   }
  else
    return 1 ;
 }  // plainJudgeCheckupOutFile :: numberColumnWidth

size_t plainJudgeCheckupOutFile :: subjectTextColumnWidth ()
 {
  return numberColumnWidth () * subjNameColumnWidthCoef ;
 }  // plainJudgeCheckupOutFile :: subjectTextColumnWidth

void plainJudgeCheckupOutFile :: makeMediumLine ()
 {
  ostrstream oss (horzLn, sizeof (horzLn)) ;

  oss.fill (PSEUDO_GRAPHIC_HORIZONTAL) ;  // ((char) 151) ;
  oss.setf (ios :: left) ;
  oss << setw (subjectTextColumnWidth () + 1)
      << PSEUDO_GRAPHIC_LEFT_T ;  // (char) 145 ;
  for (int i = 0 ; i <= judgesCnt ; i ++)
    oss << setw (numberColumnWidth () + 1)
        << PSEUDO_GRAPHIC_CROSS ;  // (char) 134 ;
  oss << PSEUDO_GRAPHIC_RIGHT_T  // (char) 146
      << ends ;
  horzLn [sizeof (horzLn) - 1] = '\0' ;
 }  // plainJudgeCheckupOutFile :: makeMediumLine

plainJudgeCheckupOutFile :: plainJudgeCheckupOutFile (TWindow * parent,
							     const char * hFName,
							     const int howManyJudges) :
					judgeCheckupOutFile (parent,
								   hFName),
					judgesCnt (howManyJudges),
					subjNameColumnWidthCoef (5)
 {
  makeMediumLine () ;
 }  // plainJudgeCheckupOutFile :: plainJudgeCheckupOutFile

void plainJudgeCheckupOutFile ::
  fillColumnsWithNames (const TJudgeArray & const sWJHA,
                               TIArray <numeratedColumn> & columns)
 {
  const int cCnt = columns.Count () ;
  const int sJCnt = sWJHA.Count () ;

  if (cCnt < 3 || sJCnt < 1)
    return ;
  (columns [0]) -> addText ("Предмет") ;
  (columns [1]) -> addText ("Шифър") ;
  for (int i = 0 ; i < sJCnt && i + 2 < cCnt ; i ++)
   {
    char n [SIZE_OF_CITIZEN_NAME] ;
    char f [SIZE_OF_CITIZEN_FAMILY] ;
    char nF [sizeof (n) + sizeof (f)] ;

    if ((sWJHA [i]) -> nameFamily (n, sizeof (n), f, sizeof (f)))
     {
      scSprintf (sizeof (nF), nF, "%s %s", n, f) ;
      (columns [i + 2]) -> addText (nF) ;
     }
   }
 }  // plainJudgeCheckupOutFile :: fillColumnsWithNames

void plainJudgeCheckupOutFile ::
 fillColumnsWithNumbers (const subject_With_LongIntArray & const sWJHA,
                         TIArray <numeratedColumn> & columns,
                         const char * const types,
                         const bool first)
 {
  const int cCnt = columns.Count () ;
  const int sJCnt = sWJHA.arrItems ;

  if (cCnt < 3 || sJCnt < 1)
    return ;
  if (first)
   {
    if (types && types [0])
     {
      char crit [256] ;

      scSprintf (sizeof (crit), crit,
                 "SELECT F_NAME FROM T_SUBJECT WHERE F_SUBJECT = %ld "
                 "AND F_TYPE IN (%s) ;", sWJHA.subj,
                 stringToSQLCharSet (types).getTheSet ()) ;

	TQuery q (crit) ;

      if (q.Read ())
       {
        TSubject s ;

        s << q ;
        (columns [0]) -> addText (s.name) ;
       }
     }
     {
      char n [128] ;

      scSprintf (sizeof (n), n, "%ld", sWJHA.subj) ;
      (columns [1]) -> addText (n) ;
     }
   }
  else
    (columns [0]) -> addText ("Общо") ;
  for (int i = 0 ; i < sJCnt && i + 2 < cCnt ; i ++)
   {
    char n [128] ;
    const long int d = sWJHA.itemAtIndex (i) ;

    if (d)
      scSprintf (sizeof (n), n, "%ld", d) ;
    else
      scSprintf (sizeof (n), n, "%c", (char) 160) ;
    (columns [i + 2]) -> addText (n) ;
   }
 }  // plainJudgeCheckupOutFile :: fillColumnsWithNumbers

void plainJudgeCheckupOutFile ::
  mkNColumnsArr (TIArray <numeratedColumn> & columns)
 {
  columns.Add (new numeratedColumn (subjectTextColumnWidth (), 0)) ;
  for (int i = 0 ; i < judgesCnt + 1 ; i ++)
    columns.Add (new numeratedColumn (numberColumnWidth (), i + 1)) ;
 }  // plainJudgeCheckupOutFile :: mkNColumnsArr

int plainJudgeCheckupOutFile ::
  wrtItem (const TIArray <numeratedColumn> & columns, bool write)
 {
  int maxRows = 0 ;
  const int clmnItems = columns.Count () ;

  for (int i = 0 ; i < clmnItems ; i ++)
   {
    const int rC = columns [i] -> getRowsCount () ;

    if (rC > maxRows)
      maxRows = rC ;
   }
  if (write)
   {
    if (IsHtml ())  // 20.nov.2002 - begin
     {
      for (int j = 0 ; j < clmnItems ; j ++)
       {
        HTML_td td (* this, CellTxtVAlign (), CellTxtAlign (),
          CellBgColor ()) ;

	  HTMLOutText :: DumpColumn (* this, columns [j]);
       }
     }
    else  // 20.nov.2002 - end
      for (int i = 0 ; i < maxRows ; i ++)
       {
        (* this) << PSEUDO_GRAPHIC_VERTICAL ;  // (char) 166 ;
        for (int j = 0 ; j < clmnItems ; j ++)
          (* this)
            << resetiosflags (j ? (ios :: left) : (ios :: right))
            << setiosflags (j ? (ios :: right) : (ios :: left))
            << setw (j ? numberColumnWidth () : subjectTextColumnWidth ())
            << columns [j] -> nextRowOrEmptyStr ()
            << PSEUDO_GRAPHIC_VERTICAL ;  // (char) 166 ;
        (* this) << endl ;
        pM.incLines () ;
       }
   }
  return maxRows ;
 }  // plainJudgeCheckupOutFile :: wrtItem


 int plainJudgeCheckupOutFile ::
  writePlainItem (const TJudgeArray & const jArray, const bool write)
// returns number of lines written
 {
  TIArray <numeratedColumn> columns ;
  const int jCnt = jArray.Count () ;

  if (jCnt == 0)
    return 0 ;
  mkNColumnsArr (columns) ;
  fillColumnsWithNames (jArray, columns) ;
  return wrtItem (columns, write) ;
 }  // int plainJudgeCheckupOutFile ::
    //   writePlainItem (const judgeArray & const pArray, bool write)

int plainJudgeCheckupOutFile ::
  writePlainItem (const subject_With_LongIntArray & const lArray,
                  const char * const types,
                  const bool first, const bool write)
// returns number of lines written
 {
  TIArray <numeratedColumn> columns ;
  const int lCnt = lArray.arrItems ;

  if (lCnt == 0)
    return 0 ;
  mkNColumnsArr (columns) ;
  fillColumnsWithNumbers (lArray, columns, types, first) ;
  return wrtItem (columns, write) ;
 }  // plainJudgeCheckupOutFile :: writePlainItem

// 20.nov.2002 - end
// 18.jan.2000 - begin
inputDiaryOutFile :: inputDiaryOutFile (TWindow * parent,
						    const char * hFName) :
			    frmHeadOutText (parent, hFName)
 {
 }  // inputDiaryOutFile :: inputDiaryOutFile

# if INSTANCE
#   if APPEAL
void inputDiaryOutFile :: fHOutput (const void * p, char * s,
                                  size_t len, int algn, size_t skip,
                                  int dCipher)
 {
  if (! begNEndNYearTypesOutput (* ((const begNEndNYearTypes *) p),
                                 * this, s, len, algn, skip, dCipher))
    miscOutput (* this, s, len, algn, skip, dCipher) ;
 }  // inputDiaryOutFile :: fHOutput
#   else  // of APPEAL
void inputDiaryOutFile :: fHOutput (const void * p, char * s,
                                  size_t len, int algn, size_t skip,
                                  int dCipher)
 {
  const begNEndNYearTypes * ptr =
    reinterpret_cast <const begNEndNYearTypes *> (p) ;
  const begNEndNYearTypesAC * ptrAC =
    dynamic_cast <const begNEndNYearTypesAC *> (ptr) ;

  if (ptrAC)
   {
    if (! begNEndNYearTypesACOutput (* ptrAC,
                                     * this, s, len, algn, skip, dCipher))
      miscOutput (* this, s, len, algn, skip, dCipher) ;
   }
  else
   {
    if (! begNEndNYearTypesOutput (* ptr,
                                   * this, s, len, algn, skip, dCipher))
      miscOutput (* this, s, len, algn, skip, dCipher) ;
   }
 }  // inputDiaryOutFile :: fHOutput
#   endif  // of APPEAL
# else  // of INSTANCE
void inputDiaryOutFile :: fHOutput (const void * p, char * s,
					    size_t len, int algn, size_t skip,
                                  int dCipher)
 {
  if (! begNEndNYearTypesOutput (* ((const begNEndNYearTypes *) p),
                                 * this, s, len, algn, skip, dCipher))
    miscOutput (* this, s, len, algn, skip, dCipher) ;
 }  // inputDiaryOutFile :: fHOutput
# endif  // of INSTANCE

void inputDiaryOutFile :: writeItem (requestColumns & rC, bool first)
 {
  if (pM.newPageNeed ())
    toBeContinued () ;
  rC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // inputDiaryOutFile :: writeItem

int inputDiaryOutFile :: formattedHead (const begNEndNYearTypes *
                                                             bNENYTPtr,
                                        bool write)
 {
  return formHead (bNENYTPtr, write) ;
 }  // inputDiaryOutFile :: formattedHead

void inputDiaryOutFile :: nextReqToPrint (int linesNeed,
                                      const begNEndNYearTypes * bNENYTPtr)
 {
  if (! pM.enoughLinesLeft (linesNeed))
   {
    footherLine () ;
    newPage () ;
    if (bNENYTPtr)  // 29.sep.2000
      formattedHead (bNENYTPtr) ;
    head () ;
   }
 }  // inputDiaryOutFile :: nextReqToPrint

AppealedLawsInputDiaryOutFile ::
AppealedLawsInputDiaryOutFile (TWindow * parent,
					 const char * hFName) :
  inputDiaryOutFile (parent, hFName)
 {
 }  // AppealedLawsInputDiaryOutFile :: AppealedLawsInputDiaryOutFile

void AppealedLawsInputDiaryOutFile ::
	 writeItem (AppealedLawsInRegColumns & iRC, bool first)
 {
  if (pM.newPageNeed ())
    toBeContinued () ;
  iRC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // AppealedLawsInputDiaryOutFile :: writeItem

size_t AppealedLawsInputDiaryOutFile :: minRowsForInreg () const
 {
  return (size_t) extraVal1 ;
 }  // AppealedLawsInputDiaryOutFile :: minRowsForInreg

void AppealedLawsInputDiaryOutFile ::
       prtStartFinalChar (const bool start,
                          const bool newLBefore, const bool newLAfter)
 {
  const char ch = start ?
                  (char) (extraVal / 256) : (char) (extraVal % 256) ;

  if (newLBefore)
    (* this) << endl ;
  if (ch)
    (* this) << ch ;
  if (newLAfter)
    (* this) << endl ;
 }  // AppealedLawsInputDiaryOutFile :: prtStartFinalChar

// 18.jan.2000 - end
ResultsSetBkOutFile :: ResultsSetBkOutFile (TWindow * parent,
							  const char * hFName) :
			    frmHeadOutText (parent, hFName)
 {
 }  // ResultsSetBkOutFile :: ResultsSetBkOutFile

void ResultsSetBkOutFile :: fHOutput (const void * p, char * s,
                                      size_t len, int algn, size_t skip,
                                      int dCipher)
 {
  collCompBegDEndDBegNOutput (* ((const collCompBegDEndDBegN *) p),
                              * this, s, len, algn, skip, dCipher) ;
 }  // ResultsSetBkOutFile :: fHOutput

int ResultsSetBkOutFile :: FormattedHead (const collCompBegDEndDBegN *
                                                          cCBDEDBNPtr,
                                          bool write)
 {
  return formHead (cCBDEDBNPtr, write) ;
 }  // ResultsSetBkOutFile :: FormattedHead

void ResultsSetBkOutFile :: WriteItem (ResultsSetBookColumns & rSBC,
                                       const bool first)
 {
  if (pM.newPageNeed ())
    toBeContinued () ;
  rSBC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // ResultsSetBkOutFile :: writeItem

# if INSTANCE
#   if APPEAL
#   else  // APPEAL
TradeLaw634OutFile :: TradeLaw634OutFile (TWindow * parent,
							const char * hFName) :
                        frmHeadOutText (parent, hFName)
 {
 }  // TradeLaw634OutFile :: TradeLaw634OutFile

void TradeLaw634OutFile :: fHOutput (const void * p, char * s, size_t len,
                                     int algn, size_t skip, int dCipher)
 {
  Trade634DlgDataOutput (* ((const Trade634DlgData *) p),
                         * this, s, len, algn, skip, dCipher) ;
 }  // TradeLaw634OutFile :: fHOutput

int TradeLaw634OutFile ::
      FormattedHead (const Trade634DlgData * T634DDPtr, bool write)
 {
  return formHead (T634DDPtr, write) ;
 }  // TradeLaw634OutFile :: FormattedHead

void TradeLaw634OutFile :: WriteItem (TradeLaw634BookColumns & tL634BC,
                                      const bool first)
 {
  if (pM.newPageNeed ())
    toBeContinued () ;
  tL634BC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // TradeLaw634OutFile :: WriteItem

size_t TradeLaw634OutFile :: MinRowsTL634Chain () const
 {
  return (size_t) extraVal1 ;
 }  // TradeLaw634OutFile :: MinRowsTL634Chain

void TradeLaw634OutFile :: NextChainToPrint (const int linesNeed,
                                             const bool withHead)
 {
  if (! pM.enoughLinesLeft (linesNeed))
   {
    footherLine () ;
    newPage () ;
    newLine () ;
    newLine () ;
    newLine () ;
    if (withHead)
      head () ;
   }
 }  // TradeLaw634OutFile :: NextChainToPrint

void TradeLaw634OutFile ::
       OneChainPrtStartFinalChar (const bool start,
                                  const bool newLBefore,
                                  const bool newLAfter)
 {
  const char ch = start ?
                  (char) (extraVal / 256) : (char) (extraVal % 256) ;

  if (newLBefore)
    (* this) << endl ;
  if (ch)
    (* this) << ch ;
  if (newLAfter)
    (* this) << endl ;
 }  // TradeLaw634OutFile :: OneChainPrtStartFinalChar

#   endif  // APPEAL
# endif  // INSTANCE 

SessDistributionOutFile::SessDistributionOutFile(TWindow* parent, const char* hFName):
		frmHeadOutText (parent, hFName)
{
}  // SessDistributionOutFile::SessDistributionOutFile

void SessDistributionOutFile::fHOutput(const void* p, char* s, size_t len,
	int algn, size_t skip, int dCipher)
{
	if(p && s)
  {
		const begMYEndMYTypeCIU * ptrAC =
      reinterpret_cast<const begMYEndMYTypeCIU *>(p) ;

		if (! begMYEndMYTypeCIUOutput (* ptrAC,
				* this, s, len, algn, skip, dCipher))
      miscOutput (* this, s, len, algn, skip, dCipher) ;
  }
}  // SessDistributionOutFile::fHOutput

int SessDistributionOutFile::
	FormattedHead (const begMYEndMYTypeCIU& dlgData, bool write)
{
	return formHead(& dlgData, write);
}  // SessDistributionOutFile::FormattedHead

void SessDistributionOutFile::
	WriteItem(SessDistributionColumns& aSDC, const bool first)
{
	if (pM.newPageNeed())
    toBeContinued();
  aSDC.notConstOutput(* this, first);
  pM.incLines(linesOfItem(first));
}  // SessDistributionOutFile::WriteItem

void SessDistributionOutFile::NextItemToPrint(const int linesNeed,
	const bool addHead)
{
  if (! pM.enoughLinesLeft(linesNeed))
  {
    if(addHead)
      footherLine();
    newPage();
    if(addHead)
      head();
  }
}  // SessDistributionOutFile::NextProveToPrint

ZdravkovaDistributionOutFile::
ZdravkovaDistributionOutFile(TWindow* parent,
	const char* hFName, const int howManyJudges) :
		judgeCheckupOutFile(parent, hFName),
		judgesCnt(howManyJudges), subjNameColumnWidthCoef(2),
		numberColumnDefaultMaxWidth(5), pageWidth(79),
    numberColumnMinWidth(1)
{
  pageWidth = get_section_int(hName, "***_PAGE_WIDTH", hLL);
  numberColumnMinWidth = get_section_int(hName, "***_NUMBER_COLUMN_MIN_WIDTH", 2);
}  // ZdravkovaDistributionOutFile::ZdravkovaDistributionOutFile

size_t ZdravkovaDistributionOutFile::VertLines()
{
	return 
		judgesCnt * 2  // One for 'primary' and one for 'changes'
		+ 1  // For the column 'Subject name'
		+ 1  // The vert line from the left
		;
}  // ZdravkovaDistributionOutFile::VertLines

size_t ZdravkovaDistributionOutFile::HeaderJudgeVertLines()
{
	return 
		judgesCnt  // One for each judge
		+ 1  // For the column 'Subject name'
		+ 1  // The vert line from the left
		;
}  // ZdravkovaDistributionOutFile::HeaderJudgeVertLines

size_t ZdravkovaDistributionOutFile::NumberColumnWidth()
{
	const int allColumnsWidth = pageWidth - VertLines();
	const int columnsNumber =
		judgesCnt
			+ 1  // For the column 'Subject'
			+ 1 * subjNameColumnWidthCoef  // For the column 'Subject name'
			;
  size_t nCWidth = 1;

	if(allColumnsWidth >= columnsNumber)
	{
		const int cW = allColumnsWidth / columnsNumber;

		if(extraVal1)
			nCWidth = cW < extraVal1 ? cW : extraVal1;
		else
			nCWidth = cW < numberColumnDefaultMaxWidth ?
				cW : numberColumnDefaultMaxWidth;
   }
  if(nCWidth < numberColumnMinWidth)
    nCWidth = numberColumnMinWidth;
  return nCWidth;
}  // ZdravkovaDistributionOutFile::NumberColumnWidth

size_t ZdravkovaDistributionOutFile::SubjectTextColumnWidth()
{
	return NumberColumnWidth() * subjNameColumnWidthCoef;
}  // ZdravkovaDistributionOutFile::SubjectTextColumnWidth

size_t ZdravkovaDistributionOutFile::HeaderJudgeColumnWidth()
{
	return NumberColumnWidth() * 2 + 1;
}  // ZdravkovaDistributionOutFile::HeaderJudgeColumnWidth

void ZdravkovaDistributionOutFile::MakeMediumLine()
{ 
	{
		ostrstream oss(horzLn, sizeof (horzLn));

		oss.fill(PSEUDO_GRAPHIC_HORIZONTAL);
		oss.setf(ios::left);
		oss << setw(SubjectTextColumnWidth() + 1) << PSEUDO_GRAPHIC_LEFT_T;
		for(int i = 0; i < judgesCnt; i++)
			oss << setw(NumberColumnWidth() + 1) << PSEUDO_GRAPHIC_CROSS
				<< setw(NumberColumnWidth() + 1) << PSEUDO_GRAPHIC_CROSS;
		oss << PSEUDO_GRAPHIC_RIGHT_T << ends;
	}
	ENDING_ZERO(horzLn);
}  // ZdravkovaDistributionOutFile::MakeMediumLine

void ZdravkovaDistributionOutFile::MakeHeaderAboveJudgeMediumLine()
{
	{
		ostrstream oss(horzLn, sizeof (horzLn));

		oss.fill(PSEUDO_GRAPHIC_HORIZONTAL);
		oss.setf(ios::left);
		oss << setw(SubjectTextColumnWidth() + 1) << PSEUDO_GRAPHIC_LEFT_T;
		for(int i = 0; i < judgesCnt; i++)
			oss << setw(HeaderJudgeColumnWidth() + 1) << PSEUDO_GRAPHIC_CROSS;
		oss << PSEUDO_GRAPHIC_RIGHT_T << ends;
	}
	ENDING_ZERO(horzLn);
}  // ZdravkovaDistributionOutFile::MakeHeaderAboveJudgeMediumLine

void ZdravkovaDistributionOutFile::MakeHeaderBelowJudgeMediumLine()
{
	{
		ostrstream oss(horzLn, sizeof (horzLn));

		oss.setf(ios::left);
		oss << setw(SubjectTextColumnWidth() + 1) << "¦";
		oss.fill(PSEUDO_GRAPHIC_HORIZONTAL);
		for(int i = 0; i < judgesCnt; i++)
			oss << setw(NumberColumnWidth() + 1)
        << (i == 0 ? PSEUDO_GRAPHIC_LEFT_T : PSEUDO_GRAPHIC_CROSS)
				<< setw(NumberColumnWidth() + 1) << PSEUDO_GRAPHIC_UP_T;
		oss << PSEUDO_GRAPHIC_RIGHT_T << ends;
	}
	ENDING_ZERO(horzLn);
}  // ZdravkovaDistributionOutFile::MakeHeaderBelowJudgeMediumLine

		
void ZdravkovaDistributionOutFile::
FillColumnsWithNames(const TJudgeArray& judgArr,
  TIArray<numeratedColumn>& columns, const bool first)
{
	const int cCnt = columns.Count();
	const int sJCnt = judgArr.Count();
	const bool truncNames = section_says_yes(hName, "***_TRUNC_NAMES");

	if(cCnt < 2 || sJCnt < 1)
		return ;
  if(first)
	{  // -- Manipulating the word "Предмет" - begin --
		const size_t sTCW = SubjectTextColumnWidth();
		const char * const s = "Предмет";
		const int sL = strlen(s);
		
		if(truncNames)
		{
			CharPtr tmp(strlen(s) + 1);

			trunc_BG_string(s, tmp, tmp.Size(), sTCW);
			(columns[0])->addText(tmp);
		}
		else
		{
			if(sL <= sTCW ||   // No need to split
				sTCW < 4)    // 4 == strlen("Пред"), so split can't help much.
				(columns[0])->addText(s);
			else
				(columns[0])->addText("Пред мет");  // Split the word
		}
	}  // -- Manipulating the word "Предмет" - end --
  else
		(columns[0])->addText("Дата"); 
	for (int i = 0; i < sJCnt && i + 1 < cCnt; i++)
		// i + 1 --> because of the "Предмет" column
	{
		char n[SIZE_OF_CITIZEN_NAME];
		char f[SIZE_OF_CITIZEN_FAMILY];
		char nF[sizeof(n) + sizeof(f)];

		if((judgArr[i])->nameFamily(n, sizeof(n), f, sizeof(f)))
		{
			if(truncNames || 
				strcmp(INTER, judgArr[i]->ucn) == 0)  // the INTER
					// is always trunked, because his family is too long
			{
				char tmp_N[sizeof(n)];
				char tmpF[sizeof(f)];
				const size_t hJCW = HeaderJudgeColumnWidth();

				trunc_BG_string(n, tmp_N, sizeof(tmp_N), hJCW);
				trunc_BG_string(f, tmpF, sizeof(tmpF), hJCW);
				scSprintf(sizeof(nF), nF, "%s %s", tmp_N, tmpF);
			}
			else
				scSprintf(sizeof(nF), nF, "%s %s", n, f);
			(columns[i + 1])->addText(nF);
				// i + 1 --> because of the "Предмет" column
		}
	}
}  // ZdravkovaDistributionOutFile::FillColumnsWithNames

void ZdravkovaDistributionOutFile::
FillHeaderPrimaryChangesColumns(TIArray<numeratedColumn>& columns)
{
	const int cCnt = columns.Count();
	const size_t nCW = NumberColumnWidth();

	if(cCnt >= 2)
	{
		for(int i = 1; i < cCnt; i ++)
		{  // columns[0] remains empty (it's the "Предмет" column)
			const char* const w = i % 2 ? "Първ." : "Пром.";
			CharPtr tmp(strlen(w) + 1);

			trunc_BG_string(w, tmp, tmp.Size(), nCW > 2 ? nCW : 2);
				// 2 - because both "Първ." and "Пром." begin with 'П'
			columns[i]->addText(tmp);
		}
	}
}  // ZdravkovaDistributionOutFile::FillHeaderPrimaryChangesColumns

void ZdravkovaDistributionOutFile::
FillColumnsWithNumbers(const subject_With_LongIntArray& primary,
	const subject_With_LongIntArray& changes,
  TIArray<numeratedColumn>& columns, const char* const types,
  const CDate* pDate, const bool first, const bool write,
  CharPtr* pShortNameHolder)
{
	if(types && types[0] && primary.subj == changes.subj)
	{
		const int cCnt = columns.Count();
		const int primCnt = primary.arrItems;
		const int chgCnt = changes.arrItems;

		if (cCnt >= 2 && primCnt >= 1 && chgCnt >= 1)
		{
      const bool zeroAllowed = (first == true &&
        section_says_yes(hName, "***_SHOW_ZEROES_IN_CHECK_UP")) ||
        (first == false &&
          section_says_yes(hName, "***_SHOW_ZEROES_IN_HISTORY"));
			if(primCnt != chgCnt || (primCnt + chgCnt + 1) != cCnt)
				error("%s\n%s::FillColumnsWithNumbers\n"
          "%d <--> %d  ;  %d <--> %d",
					UNDAJUSTED_ARRAYS_PASSED_TO, typeid(*this).name(),
          primCnt, chgCnt, primCnt + chgCnt + 1, cCnt);
      if(first)
      {
        if(primary.subj)
        {
          char subjName[SIZE_OF_SUBJECT_NAME];

          subjName[0] = '\0';
          if(pShortNameHolder != NULL && pShortNameHolder->Size() > 1)
          {  // Code with suspicious value. Goal: To prevent the user from
              // entering a short subject name twice (unusual situation -
              // unknown subject code, only during creation of text
              // output).
            if(write == true)
            {
              if(pShortNameHolder->Str()[0])
              {
                ostrstream(subjName, sizeof(subjName))
                  << pShortNameHolder->Str() << ends;
                ENDING_ZERO(subjName);
              }
              else
                firm_subject_short_name(primary.subj, types[0], subjName,
                  sizeof(subjName));
            }
            else
            {
              firm_subject_short_name(primary.subj, types[0], subjName,
                sizeof(subjName));
              ostrstream(pShortNameHolder->Str(), pShortNameHolder->Size())
                << subjName << ends;
              pShortNameHolder->Str()[pShortNameHolder->Size() - 1] = '\0';
            }
          }
          else
            firm_subject_short_name(primary.subj, types[0], subjName,
              sizeof(subjName));
          columns[0]->addText(subjName);
        }
        else
          columns[0]->addText("ОБЩО");
      }
      else
      {
        if(pDate)
        {
          char tmp[16];   // Be attentive with tmp and it's indexes!
          const size_t truncInd = 6;

          CDateToString(*pDate, tmp, sizeof(tmp));
          if(truncInd <= strlen(tmp) &&
            SubjectTextColumnWidth() < strlen(tmp))
          {
            tmp[truncInd] = '\0';
            if(SubjectTextColumnWidth() < 5)
            {
              tmp[2] = ' ';
              tmp[5] = '\0';
            }
          }
          columns[0]->addText(tmp);
        }
      }
			for(int i = 0; i < primCnt && i < chgCnt; i++)
      {
        size_t clmnInd = 2 * i + 1;
          // + 1 --> because of the "Предмет" column

        if(clmnInd < cCnt)
          addLongToColumn(columns[clmnInd], primary[i], zeroAllowed);
        clmnInd += 1;
        if(clmnInd < cCnt)
          addLongToColumn(columns[clmnInd], changes[i], zeroAllowed);
      }
		}
	}
	else
		error("%s\n%s::FillColumnsWithNumbers", WRONG_ARGS_PASSED_TO,
			typeid(*this).name());
}  // ZdravkovaDistributionOutFile::FillColumnsWithNumbers

void ZdravkovaDistributionOutFile::
MakeHeaderNamesArray(TIArray<numeratedColumn>& columns)
{
	columns.Add(new numeratedColumn(SubjectTextColumnWidth(), 0));
	for(int i = 0; i < judgesCnt; i ++)
		columns.Add(new numeratedColumn(HeaderJudgeColumnWidth(), i + 1));
}  // ZdravkovaDistributionOutFile::MakeHeaderNamesArray

void ZdravkovaDistributionOutFile::
MakeHeaderPrimaryChangesArray(TIArray<numeratedColumn>& columns)
{
	MakeDataLinesArray(columns);
}  // ZdravkovaDistributionOutFile::MakeHeaderPrimaryChangesArray

void ZdravkovaDistributionOutFile::
MakeDataLinesArray(TIArray<numeratedColumn>& columns)
{
	columns.Add(new numeratedColumn(SubjectTextColumnWidth(), 0));
	for(int i = 0; i < judgesCnt; i ++)
	{
		columns.Add(new numeratedColumn(NumberColumnWidth(), 2 * i + 1));
		columns.Add(new numeratedColumn(NumberColumnWidth(), 2 * i + 2));
	}
}  // ZdravkovaDistributionOutFile::MakeDataLinesArray

int ZdravkovaDistributionOutFile::WriteHeader(const TJudgeArray& jArray,
  const bool first, const bool write)
{  // returns the number of lines written
	int linesWritten = 0;  // I do not use pM.getLines() in order to
	// calculate the lines written because the 'write' argument
	// may be false.

	if(jArray.Count() > 0)
	{
		MakeHeaderAboveJudgeMediumLine();
		if(write)
			topLine();
		linesWritten += 1;
		MakeHeaderBelowJudgeMediumLine();
		{
			TIArray<numeratedColumn> columns;
			
			MakeHeaderNamesArray(columns);
			FillColumnsWithNames(jArray, columns, first);
			linesWritten += WrtItem(columns, HeaderJudgeColumnWidth(), write);
		}
		if(write)
			horzLine();
		linesWritten += 1;
		{
			TIArray<numeratedColumn> columns;

			MakeHeaderPrimaryChangesArray(columns);
			FillHeaderPrimaryChangesColumns(columns);
			linesWritten += WrtItem(columns, NumberColumnWidth(), write);
		}
	}
	return linesWritten;
}  // ZdravkovaDistributionOutFile::WriteHeader

int ZdravkovaDistributionOutFile::
WritePlainItem(const subject_With_LongIntArray& primary,
	const subject_With_LongIntArray& changes,
	const char* const types, const bool atTheTop, const bool first,
  const CDate* pDate, const bool write, CharPtr* pShortNameHolder)
{
	MakeMediumLine();  // Call this unconditionally.

	int linesWritten = 0;  // I do not use pM.getLines() in order to
	// calculate the lines written because the 'write' argument
	// may be false.

  if(write)
    atTheTop ? topLine() : horzLine();
  linesWritten += 1;
  {
    TIArray <numeratedColumn> columns ;

    MakeDataLinesArray(columns);
    FillColumnsWithNumbers(primary, changes, columns, types, pDate, first,
      write, pShortNameHolder);
    linesWritten += WrtItem(columns, NumberColumnWidth(), write);
  }
	return linesWritten;
}  // ZdravkovaDistributionOutFile::WritePlainItem

int ZdravkovaDistributionOutFile::
	WrtItem (const TIArray<numeratedColumn>& columns,
  const int nonSubjectClmnWidth, const bool write)
{
	int maxRows = 0;
	const int clmnItems = columns.Count();

	for(int i = 0; i < clmnItems; i++)
	{
		const int rC = columns[i]->getRowsCount();

		if(rC > maxRows)
			maxRows = rC;
   }
	if(write)
	{
		for(int i = 0; i < maxRows; i++)
		{
			(*this) << PSEUDO_GRAPHIC_VERTICAL;
			for(int j = 0; j < clmnItems; j++)
			(*this)
				<< resetiosflags(j ? ios::left : ios::right)
				<< setiosflags(j ? ios::right : ios::left)
				<< setw(j ? nonSubjectClmnWidth : SubjectTextColumnWidth())
				<< columns[j]->nextRowOrEmptyStr()
				<< PSEUDO_GRAPHIC_VERTICAL;
			(*this) << endl;
			pM.incLines();
		}
	}
	return maxRows ;
}  // ZdravkovaDistributionOutFile::WrtItem

ZdravkovaDistributionHTMLOutFile::
ZdravkovaDistributionHTMLOutFile(TWindow* parent,
	const char* hFName, const int howManyJudges) :
		ZdravkovaDistributionOutFile(parent, hFName, howManyJudges)
{
}  // ZdravkovaDistributionHTMLOutFile::ZdravkovaDistributionHTMLOutFile

void ZdravkovaDistributionHTMLOutFile::
WriteHeader(const TJudgeArray& jArray, const bool first)
{
	if(jArray.Count() > 0)
	{
    char tableFontFace[64];
    char tableFontColor[16];
    char tableFontSize[16];
    char headerBackground[16];

    get_section_string (hName, "***_BODY_FONT_FACE", tableFontFace, sizeof(tableFontFace));
    get_section_string (hName, "***_BODY_FONT_COLOR", tableFontColor, sizeof(tableFontColor));
    get_section_string (hName, "***_BODY_FONT_SIZE", tableFontSize, sizeof(tableFontSize));
    get_section_string (hName, first ? "***_CHECK_UP_TABLE_HEADER_BGCOLOR" : "***_HISTORY_TABLE_HEADER_BGCOLOR",
		headerBackground, sizeof(headerBackground));


    {
  		HTML_tr tr(*this);
      TIArray<numeratedColumn> columns;

      MakeHeaderNamesArray(columns);
      FillColumnsWithNames(jArray, columns, first);
	for(int i = 0; i < columns.Count(); i ++)
      {
        const int colspan = i == 0 ? 0 : 2;
        const int rowspan = i == 0 ? 2 : 1;
        HTML_th th(*this, "top", "center", headerBackground, colspan,
          rowspan);
        HTML_font * pHTMLTableFont =
          (tableFontFace[0] || tableFontColor[0] || tableFontSize[0]) ?
            new HTML_font(*this, tableFontFace,
              tableFontColor, tableFontSize) : NULL;

        FillCell(*(columns[i]));
        if(pHTMLTableFont)
          delete pHTMLTableFont;
      }
    }
    {
  		HTML_tr tr(*this);
      TIArray<numeratedColumn> columns;

      MakeHeaderPrimaryChangesArray(columns);
      FillHeaderPrimaryChangesColumns(columns);
	for(int i = 1; i < columns.Count(); i ++)
      {
        HTML_th th(*this, "top", "center", headerBackground, 0, 0);
        HTML_font * pHTMLTableFont =
          (tableFontFace[0] || tableFontColor[0] || tableFontSize[0]) ?
            new HTML_font(*this, tableFontFace,
              tableFontColor, tableFontSize) : NULL;

        FillCell(*(columns[i]));
        if(pHTMLTableFont)
          delete pHTMLTableFont;
      }
    }
	}
} // ZdravkovaDistributionHTMLOutFile::WriteHeader

void ZdravkovaDistributionHTMLOutFile::
WritePlainItem(const TJudgeArray& jArr,
  const subject_With_LongIntArray& primary,
	const subject_With_LongIntArray& changes,
	const char* const types, const bool first, const CDate* pDate)
{
	if(primary.arrItems && changes.arrItems)
	{
		HTML_tr tr(*this);
    char tableFontFace[64];
    char tableFontColor[16];
    char tableFontSize[16];
    char smallestValColor[16];
    char biggestValColor[16];
    char rowBgcolor[16];
    const bool summaryRow = (first && primary.subj == 0);
    size_t interInd = jArr.Count();  // out of range

    get_section_string (hName, "***_BODY_FONT_FACE", tableFontFace, sizeof(tableFontFace));
    get_section_string (hName, "***_BODY_FONT_COLOR", tableFontColor, sizeof(tableFontColor));
    get_section_string (hName, "***_BODY_FONT_SIZE", tableFontSize, sizeof(tableFontSize));
    get_section_string (hName, "***_SMALLEST_NUMBER_COLOR", smallestValColor, sizeof(smallestValColor));
    get_section_string (hName, "***_BIGGEST_NUMBER_COLOR", biggestValColor, sizeof(biggestValColor));
    rowBgcolor[0] = '\0';
    if(summaryRow)
      get_section_string (hName, "***_SUMMARY_ROW_BGCOLOR", rowBgcolor, sizeof(rowBgcolor));
    {
	for(int i = 0; i < jArr.Count(); i++)
        if(strcmp(jArr[i]->ucn, INTER) == 0)
        {
          interInd = i;
          break;
        }
    }
    {
      TIArray<numeratedColumn> columns;

      MakeDataLinesArray(columns);
      FillColumnsWithNumbers(primary, changes, columns, types, pDate,
        first);
	for(int i = 0; i < columns.Count(); i ++)
      {
        const int testIndex = (i > 0 ? (i - 1) / 2 : -1);
        const bool inter =
          (summaryRow == false &&
            testIndex >= 0 && testIndex < jArr.Count()) ?
              (testIndex == interInd) : false;
        char interBgcolor[16];

        interBgcolor[0] = '\0';
        if(inter)
          get_section_string (hName, "***_FOR_DISTRIBUTE_COLUMN_BGCOLOR", interBgcolor, sizeof(interBgcolor));

        {
          HTML_td td(*this, "top", i == 0 ? "left" : "right",
		inter ? interBgcolor : rowBgcolor,
            0, 0);
          const char* color = tableFontColor;
          const subject_With_LongIntArray& toTest =
            i % 2 ? primary : changes;
          const bool smallest =
            testIndex >= 0 && testIndex < toTest.arrItems ?
            toTest.smallestVal(testIndex, interInd) : false;
          const bool biggest =
            testIndex >= 0 && testIndex < toTest.arrItems ?
            toTest.biggestVal(testIndex, interInd) : false;

          if(smallest != biggest)
          {
            if(smallest)
              color = smallestValColor;
            if(biggest)
              color = biggestValColor;
          }
          HTML_font * pHTMLTableFont =
            (tableFontFace[0] || color[0] || tableFontSize[0]) ?
              new HTML_font(*this, tableFontFace, color, tableFontSize) :
              NULL;
          HTML_b* pHTMLCellB =
            (summaryRow &&
              section_says_yes(hName, "***_SUMMARY_ROW_BOLD"))?
                new HTML_b(*this, false) : NULL;

          FillCell(*(columns[i]));
          if(pHTMLCellB)
            delete pHTMLCellB;
          if(pHTMLTableFont)
            delete pHTMLTableFont;
        }
      }
    }
	}
}  // ZdravkovaDistributionHTMLOutFile::WritePlainItem

void ZdravkovaDistributionHTMLOutFile::FillCell(column& clmn)
{
	const size_t r = clmn.getRowsCount();
	bool empty = true;

	for(size_t j = 0; j < r; j++)
	{
		const char* const t = clmn.nextRowOrEmptyStr();

		if(empty)
			empty = strIsBlank(t);
		(*this) << (j ? " " : "") << t;
	}
	if(empty)
		(*this) << html_CER_blank();
}  // ZdravkovaDistributionHTMLOutFile::FillCell

void outText :: loadHLL ()
// An old variant - in VersionArchives\1078\_appname.j (_outfile.cpp)
 {
  ifstream ifs (hName) ;

  if (ifs)
   {  // -- 0 --
    if (seek_parameters_section (ifs))
	load_format_parameters (ifs, hLL, pageLines, itemLines, itemLines1,
					extraVal, extraVal1, alignment, justify) ;
     {  // 16.oct.2001 - begin
	bool sectionFound = false ;  // 16.oct.2001

	if (ALT_PATCH_COLUMNS)
	  sectionFound = findSection (ifs, hName, ALT_PATCH_MEDIUM_LINE_SECTION, false) ;
	if (sectionFound == false)
	  sectionFound = findSection (ifs, hName, MEDIUM_LINE_SECTION) ;
//      if (findSection (pParent, ifs, MEDIUM_LINE_SECTION))
	if (sectionFound)  // 16.oct.2001 - end
	 {  // -- 2 --
	  char buff [SIZE_OF_LINE] ;

	  buff [0] = '\0' ;
	  ifs.getline (buff, sizeof (buff)) ;
	  if (ifs.good () && ! endSection (buff))
	   {
	    ostrstream (horzLn, sizeof (horzLn)) << buff << ends ;
	    horzLn [sizeof (horzLn) - 1]  = '\0' ;
	   }
	  ifs.clear () ;
	 }  // -- 2 --
     }  // -- 1 --  // 16.oct.2001
    ifs.clear () ;
   }  // -- 0 --
  else
    cannotOpenFmtFile (hName) ;
 }  //  outText :: loadHLL

bool outText :: changeFmt (const char * const otherFmt)
 {
  if (! (otherFmt && otherFmt [0]))
    return error ("outText::changeFmt faied because the otherFmt%s",
			ARG_IS_NULL_OR_EMPTY) ;
  if (hName)
   {
    delete [] hName ;
    hName = NULL ;
   }
   {
    int sZ = strlen (otherFmt) + SIZE_OF_LINE ;

    hName = new char [sZ] ;
    ostrstream (hName, sZ) << otherFmt << ends ;
    if (access (hName, 0))
	if (! referenceToFmtFile (hName, sZ))
	  return false ;
   }
  loadHLL () ;
  return true ;
 }  // outText :: changeFmt

outText :: outText (TWindow * parent, const char * hFName, bEActText initAct, bEActText doneAct) :
		 hLL (79), pageLines (55), itemLines (1), itemLines1 (1),
		 continuation (0), extraVal (0), extraVal1 (0),
		 alignment (0), justify (0),
		 pParent (parent), done (doneAct),
		 hName (NULL),  // Important !!!
		 pagesPrinted (0)
 {
  horzLn [0] = '\0' ;
  if (initAct)
    initAct (* this) ;
  if (hFName)
    initFF(hFName);
 }  // outText :: outText

void outText :: initFF (const char * hFName)
{
  changeFmt (hFName) ;
  pM.setParams (pageLines, 0) ;
}

outText :: ~outText ()
 {
  if (done)
    done (* this) ;
  flush () ;
  if (hName)
    delete [] hName ;
  clear () ;
 }  // outText :: ~outText

void outText :: title (ostream * oSPtr)
 {
  if (hName)
   {  // -- 0 --
    char buff [SIZE_OF_LINE] ;
    ifstream ifs (hName) ;

//    if (ifs && findSection (pParent, ifs,  // 08.feb.2002 - begin
//                            TITLE_SECTION))
    if (ifs)
     {  // -- 2 --
	bool sectionFound = false ;

	if (WINDOWS_PRINT)
	  sectionFound = findSection (ifs, hName, WINDOWS_TITLE_SECTION, false) ;
	if (sectionFound == false)
	  sectionFound = findSection (ifs, hName, TITLE_SECTION) ;
	if (sectionFound)  // 08.feb.2002 - end
	 {  // -- 2 --
	  bool endSectionDetected = false ;

	  do
	   {
	    buff [0] = '\0' ;
	    ifs.getline (buff, sizeof (buff)) ;
	    if (ifs.good () || ifs.rdstate () == ios :: eofbit)
		if ((endSectionDetected = endSection (buff)) == false)
		  if (oSPtr)
		    (* oSPtr) << buff << endl ;
		  else
		   {
		    (* this) << buff << endl ;
		    pM.incLines () ;
		   }
	   }
	  while (ifs.good () && ! endSectionDetected) ;
	  ifs.clear () ;
	 }  // -- 2 --
     }  // -- 1 --  // 08.feb.2002
   }  // -- 0 --
 }  // outText :: title ()

int outText :: sectionOut (const char * const section, bool write)
 {
  int printedLines = 0 ;

  if (hName)
   {
    char buff [SIZE_OF_LINE] ;
    ifstream ifs (hName) ;

    buff [0] = '\0' ;
    if (ifs && findSection (ifs, hName, section))
     {
	bool endSectionDetected = false ;

	do
	 {
	  buff [0] = '\0' ;
	  ifs.getline (buff, sizeof (buff)) ;
	  if (ifs.good () || ifs.rdstate () == ios :: eofbit)
	    if ((endSectionDetected = endSection (buff)) == false)
	     {
		printedLines ++ ;
		if (write)
		 {
		  (* this) << buff << endl ;
		  pM.incLines () ;
		 }
	     }
	 } while (ifs.good () && ! endSectionDetected) ;
	ifs.clear () ;
     }
   }
  return printedLines ;
 }  // outText :: sectionOut

int outText :: head (bool write)
 {
  return sectionOut (HEADER_SECTION, write) ;
 }  // outText :: head

int outText :: tail (bool write )
 {
  return sectionOut (TAIL_SECTION, write) ;
 }  // outText :: tail

void outText :: newPage ()
 {
  (* this) << (char) 12 ;
  pM.setLines (0) ;
  pagesPrinted ++ ;  // 19.nov.2001
 }  // outText :: newPage

void outText :: headAtNewPage ()
 {
  footherLine () ;
  newPage () ;
  head () ;
 }  // outText :: headAtNewPage

void outText :: horzLine ()
 {
  (* this) << horzLn << endl ;
  pM.incLines () ;
 }  // outText :: horzLine

void outText :: topLine ()
 {
  const size_t l = strlen (horzLn) ;
  size_t i ;
  char toOut ;

  for (i = 0 ; i < l ; i ++)
   {
    switch (horzLn [i])
     {
	case PSEUDO_GRAPHIC_LEFT_T :  // 145 :        // like |-
	  toOut = PSEUDO_GRAPHIC_UP_LEFT ;  // 171 ;  // like  _
								    //      |
	  break ;
	case PSEUDO_GRAPHIC_RIGHT_T :  // 146 :        // like -|
	  toOut = PSEUDO_GRAPHIC_UP_RIGHT ;  // 187 ;  // like _
								     //       |
	  break ;
	case PSEUDO_GRAPHIC_CROSS :  // 134 :      // like +
	  toOut = PSEUDO_GRAPHIC_UP_T ;  // 147 ;  // like _ _
								 //       |
	  break ;
	default :
	  toOut = horzLn [i] ;
	  if (toOut != PSEUDO_GRAPHIC_CROSS &&  // (char) 134 &&  // like +
		toOut != PSEUDO_GRAPHIC_LEFT_T &&  // (char)145 &&  // like |-
		toOut != PSEUDO_GRAPHIC_RIGHT_T)  // (char) 146)    // like -|
	    toOut = PSEUDO_GRAPHIC_HORIZONTAL ;  // 151 ;         // like -
	  break ;
     }
    (* this) << toOut ;
   }
  (* this) << endl ;
  pM.incLines () ;
 }  // outText :: topLine

void outText :: footherLine ()
 {
  const size_t l = strlen (horzLn) ;
  size_t i ;
  char toOut ;

  for (i = 0 ; i < l ; i ++)
   {
    switch (horzLn [i])
     {
	case PSEUDO_GRAPHIC_LEFT_T :  // 145 :           // like |-
	  toOut = PSEUDO_GRAPHIC_DOWN_LEFT ;  // 139 ;   // like |_
	  break ;
	case PSEUDO_GRAPHIC_RIGHT_T:  // 146 :           // like -|
	  toOut = PSEUDO_GRAPHIC_DOWN_RIGHT ;  // 155 ;  // like _|
	  break ;
	case PSEUDO_GRAPHIC_CROSS :  // 134 :            // like +
	  toOut = PSEUDO_GRAPHIC_DOWN_T ;  // 132 ;      // like _|_
	  break ;
	default :
	  toOut = horzLn [i] ;
	  if (toOut != PSEUDO_GRAPHIC_CROSS &&  // (char) 134 &&   // like +
		toOut != PSEUDO_GRAPHIC_LEFT_T &&  // (char) 145 &&  // like |-
		toOut != PSEUDO_GRAPHIC_RIGHT_T &&  // (char)146 &&  // like -|
		((unsigned char) toOut) >= ' ')  // 15.feb.2000
	    toOut = PSEUDO_GRAPHIC_HORIZONTAL ;  // 151 ;  // like -
	  break ;
     }
    (* this) << toOut ;
   }
  (* this) << endl ;
  pM.incLines () ;
 }  // outText :: footherLine

void outText :: mediumFootherLine ()
 {
  const size_t l = strlen (horzLn) ;
  size_t i ;
  char toOut ;

  for (i = 0 ; i < l ; i ++)
   {
    switch (horzLn [i])
     {
	case PSEUDO_GRAPHIC_CROSS :  // 134 :        // like +
	  toOut = PSEUDO_GRAPHIC_DOWN_T ;  // 132 ;  // like _|_
	  break ;
	default :
	  toOut = horzLn [i] ;
	  if (toOut != PSEUDO_GRAPHIC_CROSS &&  // (char) 134 &&  // like +
		toOut != PSEUDO_GRAPHIC_LEFT_T &&  // (char)145 &&  // like |-
		toOut != PSEUDO_GRAPHIC_RIGHT_T)  // (char) 146)    // like -|
	    toOut = PSEUDO_GRAPHIC_HORIZONTAL ;         // 151 ;  // like -
	  break ;
     }
    (* this) << toOut ;
   }
  (* this) << endl ;
  pM.incLines () ;
 }  // outText :: mediumFootherLine

void outText :: mediumTopLine ()
 {
  const size_t l = strlen (horzLn) ;
  size_t i ;
  char toOut ;

  for (i = 0 ; i < l ; i ++)
   {
    switch (horzLn [i])
     {
	case PSEUDO_GRAPHIC_CROSS :  // 134 :      // like +
	  toOut = PSEUDO_GRAPHIC_UP_T ;  // 147 ;  // like _ _
								 //       |
	  break ;
	default :
	  toOut = horzLn [i] ;
	  if (toOut != PSEUDO_GRAPHIC_CROSS &&  // (char) 134 &&  // like +
		toOut != PSEUDO_GRAPHIC_LEFT_T &&  // (char)145 &&  // like |-
		toOut != PSEUDO_GRAPHIC_RIGHT_T)  // (char) 146)    // like -|
	    toOut = PSEUDO_GRAPHIC_HORIZONTAL ;  // 151 ;  // like -
	  break ;
     }
    (* this) << toOut ;
   }
  (* this) << endl ;
  pM.incLines () ;
 }  // outText :: mediumTopLine

void outText :: footherAndTail ()
 {
  footherLine () ;
  tail () ;
 }  // outText :: footherAndTail

void outText :: resetPrintedLines ()
 {
  pM.setLines (0) ;
 }  // outText :: resetPrintedLines

void outText :: nextLSToPrint (int linesNeed)
 {
  if (! pM.enoughLinesLeft (linesNeed))
    headAtNewPage () ;
 }  // outText :: nextLSToPrint

void outText :: toBeContinuedWithoutEndl ()
 {
  const char * const cont = " ПРОДЪЛЖЕНИЕ " ;

  (* this) << "*** СЛЕДВА" << cont << ( ++ continuation) ;
  newPage () ;
  (* this) << endl ;  // 10.may.2000
  pM.incLines () ;    // 10.may.2000  I'm not sure is this necessary
  (* this) << "***" << cont << continuation ;
 }  // outText :: toBeContinuedWithoutEndl

void outText :: toBeContinued (const void * const)
 {
  toBeContinuedWithoutEndl () ;
  (* this) << endl ;
  pM.incLines () ;
 }  // outText :: toBeContinued

void outText :: writeString (const char * str)
 {
  size_t l = 0 ;
  const char * nLCharPtr = str ;

  while ((nLCharPtr = strchr (nLCharPtr, '\n')) != NULL)
  {  l ++ ;
//     nLCharPtr ++ ; // 20060120
  }
  (* this) << str ;
  pM.incLines (l) ;
 }  // outText :: writeString

int outText :: pageLinesLeft () const
 {
  return pM.linesLeft () ;
 }  // outText :: pageLinesLeft

int outText :: pageLinesUsed () const
 {
  return pM.getLines () ;
 }  // outText :: pageLinesUsed

int outText :: getPageLines () const
 {  // 09.feb.2005
  return pM.getPgLines () ;
 }

int outText :: linesOfItem (bool first) const
 {
  return first ? itemLines : itemLines1 ;
 }  // outText :: linesOfItem

void outText :: newLine ()  // 21.jul.2000
 {
  (* this) << endl ;
  pM.incLines () ;
 }  // outText :: newLine

int outText :: getAlignment () const  // 17.oct.2001
 {
  return alignment ;
 }  // outText :: getAlignment

int outText :: getJustification () const  // 17.oct.2001
 {
  return justify ;
 }  // outText :: getJustification

bool outText :: usedFormatFileName (char * const dest,
						const size_t destSz) const
 {  // 17.oct.2001
  bool res = false ;

  if (dest && destSz && hName && hName [0] && strlen (hName) < destSz)
   {
    strcpy (dest, hName) ;
    res = true ;
   }
  else
    if (dest && destSz)
	dest [0] = '\0' ;
  if (! res)
    error ("Cannot get the name of the used format file.") ;
  return res ;
 }  // outText :: usedFormatFileName

int outText :: getPagesPrinted () const
 {  // 19.npv.2001
  return pagesPrinted ;
 }  // outText :: getPagesPrinted

frmHeadOutText :: frmHeadOutText (TWindow * parent, const char * hFName) :
			  outText (parent, hFName)
 {
  pAltStream = NULL;  // 22.jan.2004
 }  // frmHeadOutText :: frmHeadOutText

void frmHeadOutText :: fHOutput (const void *, char *, size_t,
					   int, size_t, int)
 {
 }  // frmHeadOutText :: fHOutput

int frmHeadOutText :: formHead (const void * p, bool write,
					  const char * const section,
					  int * pNumberOfCodes,
					  TWaitThread * pDLThrd,
					  TWaitThreadDialog *wtdl
					 ) // Reads from *.fm* file, section FORMATED_HEADER
 {
  const size_t buffSz = 1024 ;
  char * buff = new char [buffSz] ;
  ifstream ifs (hName) ;
  int printedLines = 0 ;

  buff [0] = '\0' ;
  if (pNumberOfCodes)         // 14.feb.2002
    (* pNumberOfCodes) = 0 ;  // 14.feb.2002
  if (ifs)
   {
    long int inpSz ;
    bool endSectionDetected = false ;

    ifs.seekg (0, ios :: end) ;
    inpSz = ifs.tellg () ;
    ifs.seekg (0) ;
    if (findSection (ifs, hName, section))
	while (ifs.good () && good () && ifs.tellg () < inpSz &&
		 ! endSectionDetected
		)
	 {  // -- 2 --
	  buff [0] = '\0' ;
	  if (wtdl)  // 04.mar.2002 - begin
          if (wtdl -> WantBreak ())
            break ;  // 04.mar.2002 - end
        ifs.getline (buff, buffSz, frmTxtStmtEnd) ;
	  if (! ifs.good ())
         {
	    if (ifs.rdstate () != ios :: eofbit)
            break ;
	   }
         {
          char * endSectPtr ;

          if ((endSectPtr = strstr (buff, END_OF_SECTION)) != NULL)
           {
            * endSectPtr = '\0' ;
            endSectionDetected = true ;
	     }
	   }
        if (buff [0] == frmTxtParam)
         {  // -- 3 --
          bookAreas bA ;

          if (pNumberOfCodes)        // 14.feb.2002
            (* pNumberOfCodes) ++ ;  // 14.feb.2002
	     {
		istrstream iss (buff + 1, strlen (buff)) ;

		bA.load (iss) ;
	     }
	    if (bA.dataCipher && bA.areaLen)        // 7066  119
	     {  // -- 4 --
            size_t len = bA.areaLen + bA.from ;
            char * s = new char [len + 1] ;
		const int algn = bA.align ;
            const size_t skip = bA.from ;

            if (pDLThrd)  // 14.feb.2002
              pDLThrd -> Tick (false) ;  // 14.feb.2002
            if (algn == CENTER_ALIGN &&
		    bA.areaLen == 254 && bA.from ==  0)
		 {
              len = strlen (horzLn) ;
              if (len < extraVal)
                len = extraVal ;
		 }
		if (write)
		  if (! regionalArealOutput (* this, s, len, algn,
						     skip, bA.dataCipher))
		    if (p)  // 05.jul.2001
			fHOutput (p, s, len, algn, skip, bA.dataCipher) ;
		if (pDLThrd)  // 14.feb.2002
		  pDLThrd -> Tick (true) ;  // 14.feb.2002
		delete [] s ;
	     }  // -- 4 --
	    else
		break ;
	    if (! good ())
		break ;
	   }  // -- 3 --
	  else
	   {
	    if (write)
	    {
		if (pAltStream)
		 {
		  //info("using pAltStream");
		  * pAltStream << buff ;
		 }
		else
		 {
		  * this << buff ;
		 }
	    }
	    for (int i = 0 ; buff [i] ; i ++)
		if (buff [i] == '\n')
		 {
		  printedLines ++ ;
		  if (write)
		    pM.incLines () ;
		 }
	   }
	 }  // -- 2 --
   }
  delete [] buff ;
  return printedLines ;
 }  // frmHeadOutText :: formHead

bool frmHeadOutText :: supportsFormatterTail () const  // 04.apr.2001
 {
  return false ;
 }  // frmHeadOutText :: supportsFormatterTail

bool frmHeadOutText :: supportsAnyFormattedText () const  // 12.jun.2001
 {
  return false ;
 }  // frmHeadOutText :: supportsAnyFormattedText


HTMLOutText::HTMLOutText(TWindow* parent, const char* fmtFName) :
		frmHeadOutText(parent, fmtFName)
{  // 20.nov.2002
}  // HTMLOutText::HTMLOutText

void HTMLOutText::WriteToTableRow(columnsArr& cArr, const int basicCipher,
			const char* const section, outText& outF, const char* const hName)
{  // 20.nov.2002
  if(hName && hName[0])
  {
    ifstream ifs(hName);

    if(ifs)
    {
	long int inpSz;
	bool endSectionDetected = false;

	ifs.seekg(0, ios::end);
	inpSz = ifs.tellg();
	ifs.seekg(0);
	if(findSection(ifs, hName, section ? section : TABLE_ROW_SECTION))
	{
	  CharPtr buff(1024);

	  while(ifs.good() && outF.good() && ifs.tellg() < inpSz &&
		endSectionDetected == false)
	  {  // -- 2 --
	    buff[0] = '\0' ;
	    ifs.getline(buff, buff.Size(), frmTxtStmtEnd);
	    if (!ifs.good())
	    {
		if(ifs.rdstate() != ios::eofbit)
		  break;
          }
          {
            char* const endSectPtr = strstr(buff, END_OF_SECTION);

            if(endSectPtr != NULL)
            {
              *endSectPtr = '\0';
              endSectionDetected = true;
            }
          }
          if(buff[0] == frmTxtParam)
          {  // -- 3 --
            bookAreas bA;

            {
		  istrstream iss(buff + 1);  // , strlen(buff));

              bA.load(iss);
            }
            if (bA.dataCipher && bA.areaLen)
            {  // Do some work
              const int internalCipher =  // 23.apr.2003 - begin
		    bA.dataCipher - 1 - basicCipher;

              if(cArr.getColumn(internalCipher) == NULL)
		  {
                CharPtr s(bA.areaLen + bA.from + 1);

                miscOutput (outF, s, bA.areaLen, bA.align, bA.from,
                  bA.dataCipher);
                //info("Non-column ciper: %d", bA.dataCipher);
              }
              else  // 23.apr.2003 - end
                DumpColumn(outF, cArr, internalCipher);

            }
            else
              break;
            if(!(outF.good()))
			break;
          }  // -- 3 --
	    else
            outF << buff;
        }  // -- 2 --
      }
    }
    else
	cannotOpenFmtFile (hName);
  }
}  // HTMLOutText::WriteToTableRow

void HTMLOutText::WriteToTableRow(columnsArr& cArr, const int basicCipher,
	const char* const section)
{  // 20.nov.2002
  WriteToTableRow(cArr, basicCipher, section, *this, hName);
}  // HTMLOutText::WriteToTableRow

void HTMLOutText::DumpColumn(ostream& os, columnsArr& cArr,
  const int cipher)
{  // 20.nov.2002
	DumpColumn(os, cArr.getColumn(cipher));
}  // HTMLOutText::DumpColumn

void HTMLOutText::DumpColumn(ostream& os, column * const pCol)
{  // 20.nov.2002
	if(pCol)  // IMPORTANT!
	{
		const size_t lines = pCol->getRowsCount();

		for(size_t i = 0; i < lines; i++)
		{  // Writing
			string_to_html_ostream_allowCode(os, (*pCol)[i]);
      if(i < lines - 1)
        html_br(os);
		}
    // insert "&nbsp;" if the cell is empty. Otherwise the cell is left
    // without border.
    if(lines == 0)
      os << html_CER_blank();
    if(lines == 1)
    {
      if(strIsBlank((*pCol)[0]))
        os << html_CER_blank();
    }
	}
}  // HTMLOutText::DumpColumn

HTMLOutput::HTMLOutput(outText& outTextRef) : outF(outTextRef)
{
}  // HTMLOutput::HTMLOutput

HTMLOutput::~HTMLOutput()
{
}  // HTMLOutput::~HTMLOutput

void HTMLOutput::WriteToTableRow(columnsArr& cArr, const int basicCipher,
  const char* const section)
{
  HTMLOutText::WriteToTableRow(cArr, basicCipher, section, outF, outF.hName);
}  // HTMLOutput::WriteToTableRow

void HTMLOutput::SectionOut(const char* const section)
{  // 20.nov.2002
  if(section && section[0])
    outF.sectionOut(section);
}  // HTMLOutput::SectionOut
