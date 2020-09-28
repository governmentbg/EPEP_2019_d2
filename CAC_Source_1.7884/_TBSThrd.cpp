#include "cac.h"

TTimeBookThread::TTimeBookThread(TCountRecsQuery* const pQuery,
					 TQuery* const pAbPrQuery,
					 timeBookOutFile& of,
					 const char* const formatFName,
					 const collCompBegDEndDBegN& rCCBDEDBN):
    pQ(pQuery), pAbPrQ(pAbPrQuery), outF(of), formatFN(formatFName),
    cCBDEDBN(rCCBDEDBN),
    description("Срочна книга")
{
	if(ArgsOK() == false)
    error("%s\n%s::%s", WRONG_ARGS_PASSED_TO,
	    typeid(*this).name(), typeid(*this).name());
}

bool TTimeBookThread::ArgsOK() const
{
	return (pQ && pAbPrQ && outF && formatFN && formatFN[0] && description);
}

void TTimeBookThread::ChangeCurrentItemText(const TSession& sess,
							 const int current,
							 const int total)
{
	char buff[192];
	ostrstream oss(buff, sizeof(buff));
  DatedContainer_ dc(sess);

  if(dc.FilledOK())
	oss << dc << " ";
  oss << "(" << current << " от " << total << ")";
  oss << ends;
  buff[sizeof(buff) - 1] = '\0';
	wtdl->SetText(buff);
}

void TTimeBookThread::WriteHeader()
{
    outF.title();
    outF.formattedHead(&cCBDEDBN);
    outF.head();
}

void TTimeBookThread::WriteTail()
{
    outF.nextSToPrint (outF.tail (false), & cCBDEDBN) ;
    outF.footherAndTail () ;
}

void TTimeBookThread::ChangePageIfNecessary(timeBookColumns& tBC, bool dateRowNeed)
{
    const int tBCMaxRows = tBC.getMaxRows();

    if(dateRowNeed)
    {
      bool dateRowWritten = false;

      outF.nextSToPrint(tBCMaxRows + 1 + outF.linesOfItem(false) + 1,
        &cCBDEDBN, &tBC, &dateRowWritten);
      if(dateRowWritten == false)
      {
        outF.mediumFootherLine();
        outF.writeItem(tBC, false);
        outF.mediumTopLine();
      }
    }
    else
    {
      bool dateRowWritten = false;

      outF.nextSToPrint(tBCMaxRows + 1, &cCBDEDBN, &tBC, &dateRowWritten);
      if(dateRowWritten == false)
        outF.horzLine();
    }
}

void TTimeBookThread::WriteOneSession(TSession& sess, TLawsuit& lawsuit,
  unsigned& sessSerialNo, TSession& abolishingSess, timeBookHeap& tBH,
  accusedsDistribution& accDistrHeap, CDate& prevDate, CDate& prevEnd)
{
  const bool printThisSess =
# if INSTANCE
    timeInRange(cCBDEDBN.noon, cCBDEDBN.whichPartOfDay, sess.time) &&
# endif
    sess.date >= cCBDEDBN.begD;
  CDate abolishingDate;
  CDate abolishingDateReturned;
  TStringArray* pXmlItems = NULL;

  if (printThisSess || cCBDEDBN.begN == 0)
    sessSerialNo ++ ;
  clearGroupData(abolishingSess);

  {  // -- 4 --
    const bool abolishedProgr =
      sessResOverridden_AbolishProgr(sess, pAbPrQ, &abolishingDate,
        &abolishingDateReturned, &abolishingSess);
    timeBookColumns tBC(formatFN, wtdl->Parent, sess, lawsuit,
      sessSerialNo + (cCBDEDBN.begN ? 0 : 1) + cCBDEDBN.begN,
      tBH, accDistrHeap, pXmlItems, &cCBDEDBN, outF.minRowsForSess(),
      printThisSess, abolishedProgr ? &abolishingDate : NULL,
      abolishedProgr ? &abolishingDateReturned : NULL,
      0, // Carefully! Don't touch this zero!
      abolishedProgr ? &abolishingSess : NULL);
    const int tBCMaxRows = tBC.getMaxRows() ;
    int j ;
    const bool dateRowNeed = prevDate != sess.date || prevEnd != sess.end;

    if(printThisSess)
    {
	ChangePageIfNecessary(tBC, dateRowNeed);
	if(dateRowNeed)
	{
	  prevDate = sess.date;
	  prevEnd = sess.end;
	}

	for (j = 0 ; j < tBCMaxRows ; j ++)
	  outF.writeItem (tBC, true) ;
    }
  }  // -- 4 --
}

void TTimeBookThread::Execute()
{
	if(ArgsOK())
  {  // -- 0 --
	try
  	{  // -- 1 --
      // ---
      // Declared here (out of a loop) by performance reasons - the construction
      // of TSession and TLawsuit is expensive
      LawPtr pLaw ;
      SessPtr pSess ;
      SessPtr abolishingSess ;
      // ---

      timeBookHeap tBH ;
      accusedsDistribution accDistrHeap ;
      CDate prevDate ;
      unsigned sessSerialNo = -1 ;
	CDate prevEnd ;
	int recs = pQ -> getRecsCount () ;
	int current = 0 ;

	wtdl->SetHeader(description);
	wtdl->PleaseWait();
	wtdl->SetProgressRange(0, recs);
      clear_CDate(prevEnd);
      clear_CDate(prevDate);
	WriteHeader();
      {  // -- 2 --
	  SimpleStringsArray pageAttribs;

	  while (wtdl->WantBreak() == false && pQ -> Read ())
	  {  // -- 3 --
	    TSession & sess = pSess.Ref () ;
	    TLawsuit & lawsuit = pLaw.Ref () ;

	    sess << (*pQ);
	    lawsuit.key = sess.key ;
	    current ++ ;
	    ChangeCurrentItemText(sess, current, recs) ;   // Gossip service
	    if (DatedContainer_(sess).FilledOK() && sess.Get() && lawsuit.Get())
	    {  // -- 3.5 --
		WriteOneSession(pSess.Ref(), lawsuit, sessSerialNo,
		  abolishingSess.Ref(), tBH, accDistrHeap, prevDate, prevEnd);
	    }  // -- 3.5 --
	    else  // 16.jun.2004
		info ("%s", OMISSIONS_WILL_BE_OCCURED) ;  // 16.jun.2004
	    wtdl->SetProgress(current);
	  }  // -- 3 --
	  wtdl->AlmostDone();
	  WriteTail();
	}  // -- 2 --
    }  // -- 1 --
    CATCH_ANYTHING
  }  // -- 0 --
}

class TTimeBookSummaryThread : public TWaitThread
{
protected:
    timeBookSummaryOutFile& outF;
    const collCompBegDEndDBegN* const pCCBDEDBN;
    const char* const description;
    int stepsNum;
    int currentStep;
    void ChangeCurrentItemText();
    virtual void Execute();

public:
	TTimeBookSummaryThread (timeBookSummaryOutFile& of,
				   const collCompBegDEndDBegN* const cCBDEDBNPtr,
				   const char* whichSummary);  // It may be NULL
    virtual void Tick(const bool completed);
};

TTimeBookSummaryThread:: TTimeBookSummaryThread(timeBookSummaryOutFile& of,
				 const collCompBegDEndDBegN* const cCBDEDBNPtr,
				 const char* whichSummary):
    outF(of), pCCBDEDBN(cCBDEDBNPtr),
    description(whichSummary ? whichSummary : SUMMARY),
    stepsNum(0), currentStep(0)
{
}

void TTimeBookSummaryThread::ChangeCurrentItemText()
{
	char buff[64];

  ostrstream(buff, sizeof(buff))
    << "Стъпка " << currentStep << " от " << stepsNum << " ..." << ends;
  buff [sizeof(buff) - 1] = '\0' ;
	wtdl->SetText(buff);
}

void TTimeBookSummaryThread::Tick(const bool completed)
{
  if(completed)
	wtdl->SetProgress(currentStep);
  else
  {
    currentStep++;
    ChangeCurrentItemText();
  }
}

void TTimeBookSummaryThread::Execute()
{
  if(outF)
  {
    try
    {
	wtdl->SetHeader(description);
	wtdl->PleaseWait();
	outF.formattedHead(pCCBDEDBN, false, &stepsNum);
	wtdl->SetProgressRange(0, stepsNum);
	outF.formattedHead(pCCBDEDBN, true, NULL, this, wtdl);
	wtdl->AlmostDone();
    }
    CATCH_ANYTHING
  }
}

void writeTBSummary(const char *summFFN, const collCompBegDEndDBegN *pCCBDEDBN, TWindow * pParent,
	const CodeValueArray * const pCodeValueArr,
#if INSTANCE
	const TISArray<reportData> *const pRpDataArr,
#endif
	const char *const whichSummary)
{
	if (!(summFFN && pCCBDEDBN && pParent))
		return;

	timeBookSummaryOutFile outF(pParent, summFFN, pCodeValueArr
	#if INSTANCE
		, pRpDataArr
	#endif
		);

	try
	{
		TTimeBookSummaryThread(outF, pCCBDEDBN, whichSummary).ExecWait();
	}
	CATCH_ANYTHING

	show_text_stream_use_settings(NULL, outF, summFFN, FLOAT_ALIGNMENT);
}
