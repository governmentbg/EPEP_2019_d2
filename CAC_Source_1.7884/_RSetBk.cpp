#include "cac.h"

ResultsSetBookEnum :: ResultsSetBookEnum ()
 {
 }  // ResultsSetBookEnum :: ResultsSetBookEnum
 
void ResultsSetBookColumns :: MakeColumns (const ResultsSet & rSet,
                                           const bool total)
 {
  column * pColumn ;

  if (total)
   {  // -- 0 --
    if ((pColumn = columns [iC_SessRes]) != NULL)
     {  // -- iC_SessRes --
      pColumn -> addText ("Общо") ;
     }  // -- iC_SessRes --
    if ((pColumn = columns [iC_Count]) != NULL)
     {  // -- iC_Count --
      addLongToColumn (pColumn, rSet.count, true) ;
     }  // -- iC_Count --
   }  // -- 1 --
  else
   {  // -- 2 --
    if ((pColumn = columns [iC_SessRes]) != NULL)
     {  // -- iC_SessRes --
	resultToString (rSet.sessRes, Result->name, sizeof Result->name) ;
      if (Result->name [0])
	  pColumn -> addText (Result->name) ;
     }  // -- iC_SessRes --
    if ((pColumn = columns [iC_DecRes]) != NULL)
     {  // -- iC_DecRes --
      if (rSet.decRes != RESULT_SESS_WITH_MANY_END_DOCS)
       {
        if (rSet.decRes && rSet.decRes != RESULT_NULL)
         {
	    Result->result = rSet.decRes ;
	    if(Result->Get ())
	     {
		if (strIsBlankOr160 (Result->name))
		  pColumn -> addText ("<< Празно име на резултат >>") ;
		else
		  pColumn -> addText (Result->name) ;
	     }
	    else
           pColumn -> addText ("-- Невалиден резултат --") ;
         }
       }
      else
        pColumn -> addText ("Повече от един краен документ") ;
     }  // -- iC_DecRes --
    if ((pColumn = columns [iC_LawGrant]) != NULL)
     {  // -- iC_LawGrant --
      TGrant g ;

      grantToString (rSet.lawGrant, g.name, sizeof (g.name)) ;
      if (g.name [0])
        pColumn -> addText (g.name) ;
     }  // -- iC_LawGrant --
    if ((pColumn = columns [iC_AfterFinish]) != NULL)
     {  // -- iC_AfterFinish --
      if (rSet.afterFinish)
        pColumn -> addText (STAR_STRING) ;
     }  // -- iC_AfterFinish --
    if ((pColumn = columns [iC_DateRetAfterFinish]) != NULL)
     {  // -- iC_DateRetAfterFinish --
      if (rSet.dateRetAfterFinish)
        pColumn -> addText (STAR_STRING) ;
     }  // -- iC_DateRetAfterFinish --
    if ((pColumn = columns [iC_Abolished]) != NULL)
     {  // -- iC_Abolished --
      if (rSet.abolished)
        pColumn -> addText (STAR_STRING) ;
     }  // -- iC_Abolished --
    if ((pColumn = columns [iC_Count]) != NULL)
     {  // -- iC_Count --
      addLongToColumn (pColumn, rSet.count) ;
     }  // -- iC_Count --
   }  // -- 2 --
 }  // ResultsSetBookColumns :: MakeColumns

ResultsSetBookColumns :: ResultsSetBookColumns (const char * formatFN,
                                                TWindow * parent,
                                                const ResultsSet & rSet,
                                                const bool total
                                               ) :
  ResultsSetBookEnum (),
  columnsArr (formatFN, parent, i_columns, 1 + OFFSET_RESULTS_SET_CIPHER)
 {
  MakeColumns (rSet, total) ;
  defineMaxRows () ;
 }  // ResultsSetBookColumns :: ResultsSetBookColumns

class TResultsSetBookThread : TWaitThread
 {
  protected :
    const collCompBegDEndDBegN & rCCBDEDBN ;
    TWindow * const pParent ;
    char formatFN [128] ;
    const size_t critSz ;
    char * criteria ;
    TQuery * pLocalQ ;
    TQuery * const pExternQ ;
    const bool fromTimeBk ;
    ResultsSetArray rSetArr ;
    void MakeBookFormatFN () ;
    bool MakeCriteria () ;
    void MakeQuery () ;
    void FillArray () ;
    void WriteOneItem (const char * formatFN, const ResultsSet & rSet,
			     ResultsSetBkOutFile & of, const bool totalRow) ;
  public :
    TResultsSetBookThread (TWindow * const parent,
			  const collCompBegDEndDBegN & cCBDEDBN,
			  TQuery * pQ, const bool timeBk) ;
    virtual ~TResultsSetBookThread () ;
    virtual void Execute();
 } ;

void TResultsSetBookThread :: MakeBookFormatFN ()
 {
  ostrstream (formatFN, sizeof (formatFN))
    << "text\\rset"
    << rCCBDEDBN.colleges [0]
    << (fromTimeBk ? "t" : "c")
    << "bk" << DEFAULT_FORMAT_FILE_EXTENSION << ends ;
  formatFN [sizeof (formatFN) - 1] = '\0' ;
 }

bool TResultsSetBookThread :: MakeCriteria ()
 {
  if (! (rCCBDEDBN.colleges [0]))
    return false ;
  if (fromTimeBk)
    return composeTBTSCriteria (criteria, critSz,
					  "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE, "
					  "S.F_END, S.F_KIND, S.F_RESULT",
					  NULL, & rCCBDEDBN) ;
  else
   {
    collBegDEndDBegN cBDEDBN ;

    cBDEDBN << rCCBDEDBN ;
    return composeClosedSessBSCriteria (criteria, critSz,
						    "S.F_NO, S.F_YEAR, S.F_TYPE, "
						    "S.F_DATE, S.F_KIND, "
						    "S.F_RETURNED",
						    cBDEDBN) ;
   }
 }

void TResultsSetBookThread :: MakeQuery ()
 {
  TWaitWindow ww (pParent, PLEASE_WAIT) ;
  pLocalQ = new TQuery (criteria) ;
 }

void TResultsSetBookThread :: FillArray ()
 {
  if (pExternQ == NULL && pLocalQ == NULL)
    return ;

  overriddenResultsSessionsBS abProgrSessArr (pParent, rCCBDEDBN) ;
  TQuery & q = pExternQ ? (* pExternQ) : (* pLocalQ) ;
  SessPtr pses ;
  TSession * pSess = pses.Ptr () ;
  LawPtr pLaw ;
  TLawsuit & law = pLaw.Ref() ;
  TWaitWindow ww (pParent, PLEASE_WAIT_EXTRACTING) ;

  q.Restart () ;
  while (q.Read ())
   {  // -- 0 --
    ResultsSet rS ;

    (* pSess) << q ;
    if (pSess -> date < rCCBDEDBN.begD)
	if (fromTimeBk) // 30.mar.2001
	  continue ;
    law << q ;
    if (pSess -> Get () && law.Get ())
     {  // -- 1 --
	rS.count = 1 ;
	rS.sessRes = pSess -> result ;
	if (strchr (RESULT_FINALIZEDS, pSess -> result))  // 03.oct.2000
	  rS.lawGrant = law.grant ;
	if (law.finished.Empty () == false && pSess -> date > law.finished)
	  rS.afterFinish = true ;
	if (law.finished.Empty () == false &&
	    pSess -> returned > law.finished)
	  rS.dateRetAfterFinish = true ;

	 {
	  BriefSessionData bsd ;

	  bsd.key = law.key ;
	  bsd.date = pSess -> date ;
	  bsd.kind = pSess -> kind ;
	  bsd.time = pSess -> time ;  // not need here (yet).
	  if (abProgrSessArr.Contains (& bsd))
	    rS.abolished = true ;
	 }
	 {  // -- 2 --
	  if (strchr (RESULT_ENTERABLES, pSess -> result))
	   {
	    SessionEndDocs endDocs (pSess -> key, pSess -> date,
					    pSess -> kind) ;

	    if (endDocs.DocsFound ())
		if (endDocs.DocsFound () == 1)
		 {
		  TDecision * pDec = new TDecision ;

		  if (endDocs.NextDoc (* pDec))
		    rS.decRes = pDec -> result ;
		  else
		   {
		    char txt [128] ;

		    DatedContainer_ (* pSess).Print (txt, sizeof (txt)) ;
		    error ("Cannot extract the end doc of %s", txt) ;
		   }
		  delete pDec ;
		 }
		else
		  rS.decRes = RESULT_SESS_WITH_MANY_END_DOCS ;
	   }
	 }  // -- 2 --
	rSetArr += rS ;
     }  // -- 1 --
   }  // -- 0 --
 }

TResultsSetBookThread::TResultsSetBookThread(TWindow * const parent,
					    const collCompBegDEndDBegN & cCBDEDBN,
					    TQuery * pQ, const bool timeBk) :
  rCCBDEDBN (cCBDEDBN), pParent (parent), critSz (1024),
  criteria (NULL), pLocalQ (NULL), pExternQ (pQ), fromTimeBk (timeBk)
 {
  wtdl->ShowWindow(SW_HIDE);
  if (pExternQ == NULL)
    if (MakeCriteria ())
	MakeQuery () ;
  MakeBookFormatFN () ;
  FillArray () ;
 }

TResultsSetBookThread :: ~TResultsSetBookThread ()
 {
    delete [] criteria ;
    delete pLocalQ ;
 }

void TResultsSetBookThread :: WriteOneItem (const char * formatFN,
						 const ResultsSet & rSet,
						 ResultsSetBkOutFile & of,
						 const bool totalRow
						)
 {
  ResultsSetBookColumns rSC (formatFN, pParent, rSet, totalRow) ;
  const int rows = rSC.getMaxRows () ;
  int i ;

  of.nextLSToPrint (rows + 1) ;
    of.horzLine () ;
  for (i = 0 ; i < rows ; i ++)
    of.WriteItem (rSC, true) ;
 }


void TResultsSetBookThread::Execute()
{
	ResultsSetBkOutFile of(pParent, formatFN);
	TWaitWindow ww(pParent, PLEASE_WAIT_GENERATING);
	ResultsSet total;
	bool beginning = true;

	for (int i = 0; i < rSetArr.Count(); i++)
	{
		total.count += rSetArr[i]->count;

		if (beginning)
		{
			of.title();
			of.FormattedHead(&rCCBDEDBN);
			of.head();
			beginning = false;
		}

		WriteOneItem(formatFN, *rSetArr[i], of, false);
	}

	WriteOneItem(formatFN, total, of, true);
	of.footherAndTail();
	show_text_stream_use_settings(NULL, of, formatFN, FLOAT_ALIGNMENT);
}

void results_set_book(TWindow * const parent, const collCompBegDEndDBegN & cCBDEDBN, TQuery * pQ, const bool timeBk)
{
	TResultsSetBookThread(parent, cCBDEDBN, pQ, timeBk).Execute();
}
