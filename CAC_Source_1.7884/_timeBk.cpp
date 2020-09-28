#include "cac.h"

class timeBook : public timeBookEnum
 {
  protected :
    TCountRecsQuery * pQ ;
    char * criteria ;
    const size_t critSz ;
    bool selectSessionsOK ;
    collCompBegDEndDBegN cCBDEDBN ;
    TWindow * pParent ;
    TQuery * pAbPrQ ;
    char * abPrCriteria ;
    const size_t abPrCritSz ;
    bool init_cCBDEDBN () ;
    bool composeCriteria () ;
    void makeQuery () ;
    bool composeAbPrCriteria () ;
    void makeAbPrQuery () ;
    void destroyQueries () ;
    char * makeBookFormatFN () ;
      // returns a pointer to a newly created string or NULL
    char * makeSummaryFormatFN (const char * bookFFN) ;
      // returns a pointer to a newly created string or NULL
# if INSTANCE
#   if APPEAL
    void fillAppealCitCodeValueArray (CodeValueArray & codeValueArr,
                                      TSession & rSess) ;
#   endif
# endif
    void fillCodeValueArray (CodeValueArray & codeValueArr) ;
    virtual void writeSummary (const char * bookFFN) ;
  public :
    timeBook (TWindow * parent) ;
    virtual ~timeBook () ;
    void printTimeBook () ;
 } ;  // class timeBook

bool timeBook :: init_cCBDEDBN ()
 {
  return (TPrintTimeBookDialog (pParent, & cCBDEDBN
# if INSTANCE
#   if APPEAL
                                , IDD_PRINT_TIME_BOOK_APPEAL
#   else  // of APPEAL
                                , IDD_PRINT_TIME_BOOK_AREAL
#   endif  // of APPEAL  
# endif
                                ).Execute () == IDOK) ;
 }  // timeBook :: init_cCBDEDBN

bool timeBook :: composeCriteria ()
 {
  collCompBegDEndDBegN tmpCCBDEDBN = cCBDEDBN ;

  if (tmpCCBDEDBN.begD > tmpCCBDEDBN.heapingStartDate)
    tmpCCBDEDBN.begD = tmpCCBDEDBN.heapingStartDate ;
  return composeTBTSCriteria (criteria, critSz,
                              "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE, "
                              "S.F_END, S.F_KIND, S.F_RESULT",
                              NULL, & tmpCCBDEDBN) ;
// Attention! There must be selected enough data for the
// sessResOverridden_AbolishProgr function

// Warning: Do not place resultsString or pAgeChar or pGrantChar here
// (any results will be ignored)!
 }  // timeBook :: composeCriteria

void timeBook :: makeQuery ()
 {
  if (cCBDEDBN.wantBook || cCBDEDBN.wantSummary ||
      cCBDEDBN.wantResultsCheckup 
     )
   {
    TWaitWindow ww (pParent, PLEASE_WAIT) ;

    pQ = new TCountRecsQuery (criteria) ;
   } 
 }  // timeBook :: makeQuery

// 13.apr.2000 - begin
bool timeBook :: composeAbPrCriteria ()
 {
  collCompBegDEndDBegN tmpCCBDEDBN = cCBDEDBN ;

  if (tmpCCBDEDBN.begD > tmpCCBDEDBN.heapingStartDate)
    tmpCCBDEDBN.begD = tmpCCBDEDBN.heapingStartDate ;
  return composeAbolishedProgressCriteria (abPrCriteria, abPrCritSz,
                                           tmpCCBDEDBN) ;
 }  // timeBook :: composeAbPrCriteria

void timeBook :: makeAbPrQuery ()
 {
  if (cCBDEDBN.wantBook || cCBDEDBN.wantSummary ||
      cCBDEDBN.wantResultsCheckup 
     )
   {
    TWaitWindow ww (pParent, PLEASE_WAIT) ;

    pAbPrQ = new TQuery (abPrCriteria) ;
   }
 }  // timeBook :: makeAbPrQuery
// 13.apr.2000 - end

timeBook :: timeBook (TWindow * parent) :
              timeBookEnum (),
              pParent (parent), pQ (NULL), selectSessionsOK (false),
              criteria (NULL), critSz (512 + 256 + 128 + 64),
              pAbPrQ (NULL), abPrCriteria (NULL), abPrCritSz (256)
 {
  if (init_cCBDEDBN () &&
      ((criteria = new char [critSz]) != NULL)
      && ((abPrCriteria = new char [abPrCritSz]) != NULL)
     )
   {
    if (composeCriteria () && composeAbPrCriteria ())
     {
      makeQuery () ;
      makeAbPrQuery () ;
//    if (pQ && pQ -> getRecsCount ())
      if (pQ && pAbPrQ)
        selectSessionsOK = true ;
     }
   }
 }  // timeBook :: timeBook

void timeBook :: destroyQueries ()
 {
  if (pQ)
   {
    delete pQ ;
    pQ = NULL ;
   }
  if (pAbPrQ)
   {
    delete pAbPrQ ;
    pAbPrQ = NULL ;
   }
  selectSessionsOK = false ;
 }  // timeBook :: destroyQueries

timeBook :: ~timeBook ()
 {
  destroyQueries () ;  // 13.apr.2000  // instead of inline text
  if (criteria)
    delete [] criteria ;
  if (abPrCriteria)
    delete [] abPrCriteria ;
 }  // timeBook :: ~timeBook

char * timeBook :: makeBookFormatFN ()
  // returns a pointer to a newly created string or NULL
 {
  if (strlen (cCBDEDBN.colleges) != 1)
    return NULL ;
  return makeTimeBookFormatFN (cCBDEDBN.colleges [0],
# if INSTANCE
    cCBDEDBN.firstInstLaws, cCBDEDBN.puniAdmConcernLaws,
# endif
    cCBDEDBN.jan2005Format) ;  // 08.jan.2005 cCBDEDBN.jan2005Format
 }  // timeBook :: makeBookFormatFN

char * timeBook :: makeSummaryFormatFN (const char * bookFFN)
  // returns a pointer to a newly created string or NULL
 {
  const int bPos = 6 ;

  if (bookFFN && strlen (bookFFN) > 7 && bookFFN [bPos] == 'b')
   {  // -- 0 --
    char * formatFN = new char [strlen (bookFFN) + 1] ;

    if (formatFN)
     {
      strcpy (formatFN, bookFFN) ;
      formatFN [bPos] = 's' ;
     }
    return formatFN ;
   }  // -- 0 --
  else
    return NULL ;
 }  // timeBook :: makeSummaryFormatFN

// 13.apr.2000 - begin
//bool timeBook :: sessResOverriddenAbolishedProgr (const TSession & s,
//                                               CDate & abolishingDate,
//                        CDate & abolishingDateReturned, // 11.sep.2000
//                          TSession * pAbolishingSess  // 13.dec.2001
//                                         )  // 20.feb.2002 commented
// Last used version of this method in VersionArchives\1232\_appname.j -->
// _timebk.cpp

# if INSTANCE
#   if APPEAL

class TAppealCitCodeValueArrayFillerThread : public TCheckupThread
{  // 20.jun.2002
	public:
		TAppealCitCodeValueArrayFillerThread(CodeValueArray& codeValueArray,
			TSession& rSession, TCountRecsQuery* const pQuery,
			const collCompBegDEndDBegN& rCCBDEDBN);
	protected:
		CodeValueArray& codeValueArr;
		TSession& rSess;
		TCountRecsQuery* const pQ;
		const collCompBegDEndDBegN& cCBDEDBN;
		virtual bool ArgsOk();
		virtual void Execute();
		void FillFinishedLaws();
		void FillSessions();
		void RegimeStr(CharPtr& dest, const char* const regime);
};

TAppealCitCodeValueArrayFillerThread::
TAppealCitCodeValueArrayFillerThread(CodeValueArray& codeValueArray,
	TSession& rSession, TCountRecsQuery* const pQuery,
	const collCompBegDEndDBegN& rCCBDEDBN) :
		codeValueArr(codeValueArray), rSess(rSession), pQ(pQuery),
		cCBDEDBN(rCCBDEDBN)
{
	if(ArgsOk() == false)
		error("%s\n%s::%s", WRONG_ARGS_PASSED_TO,
			typeid(*this).name(), typeid(*this).name());
}

bool TAppealCitCodeValueArrayFillerThread::ArgsOk()
{
	return (TCheckupThread::ArgsOk() && pQ &&
		(strchr (cCBDEDBN.colleges, TYPE_CITIZEN_LAW)
     || strchr (cCBDEDBN.colleges, TYPE_TRADE_LAW)  // 29.jun.2004
    ));
}

void TAppealCitCodeValueArrayFillerThread::RegimeStr(CharPtr& dest,
	const char* const regime)
{
	if(dest.Size() > 0)
	{
		{
			ostrstream oss(dest, dest.Size ());

			oss << SUMMARY_DATA;
			if(regime && regime[0])
				oss << " - " << regime;
			oss << ends;
		}
		dest[dest.Size() - 1] = '\0';
	}
}

void TAppealCitCodeValueArrayFillerThread::FillFinishedLaws()
{
	{
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, FINISHED_LAWS);
		wtdl->SetHeader(regimeDescr);
	}
	long int vFinisheds = 0 ;
	long int vDecideds = 0 ;
	long int vCanceleds = 0 ;
	long int vDecidedBankruptcy = 0 ;
	long int vCanceledBankruptcy = 0 ;
	long int vPrvCanceledBankruptcy = 0 ;  // 20.jun.2002
	long int vPrvDefin = 0 ;               // 20.jun.2002
	long int vPrvDefinBankruptcy = 0 ;     // 20.jun.2002
	long int vPrvCanceled = 0 ;            // 20.jun.2002
	long int vPrvDefin37 = 0 ;             // 20.jun.2002
	FinalizedSessions finSess (cCBDEDBN.colleges, cCBDEDBN.begD,
														 cCBDEDBN.endD) ;
	TLawsuit law ;

	totalPartSteps = finSess.FinishedLawsNumber();        // Gossip service
	do
	 {  // -- 2 --
		Tick(false);
		if(wtdl->WantBreak())
			break;
		if (finSess.NextSession (law, rSess))
		 {  // -- 3 --
			if (! strchr (KIND_CLOSED_SESSIONS, rSess.kind))
			 {
				const bool decided =
					sessionDecidesTheLaw (rSess, & law, true) ;
				const bool canceled =
					sessionCancelsTheLaw (rSess, & law, true) ;
				const bool prv =
	    law.kind == KIND_CIT_PRV_LAW || law.kind == KIND_TRA_PRV_LAW ;

				vFinisheds ++ ;
				if (decided == canceled)
				 {
					unknown_finished_law_status (law.key) ;
					continue ;
				 }
				if (bankruptcy_law (law))
				 {
					if (decided)
						if (prv)                    // 20.jun.2002 - begin
							vPrvDefinBankruptcy ++ ;
						else                        // 20.jun.2002 - end
							vDecidedBankruptcy ++ ;
					else
						if (prv)                    // 20.jun.2002 - begin
							vPrvCanceledBankruptcy ++ ;
						else                        // 20.jun.2002 - end
							vCanceledBankruptcy ++ ;
				 }
				else
				 {
					if (decided)
						if (prv)                    // 20.jun.2002 - begin
							if (rSess.result == RESULT_CIT_37)
								vPrvDefin37 ++ ;
							else
								vPrvDefin ++ ;
						else                        // 20.jun.2002 - end
							vDecideds ++ ;
					else
						if (prv)                    // 20.jun.2002 - begin
							vPrvCanceled ++ ;
						else                        // 20.jun.2002 - end
							vCanceleds ++ ;
				 }
			 }
		 }  // -- 3 --
	 }  // -- 2 --
	while (TRCDKey_ (law.key).FilledOK ()) ;
	codeValueArr.Add (new CodeValue (10497, vFinisheds)) ;
	codeValueArr.Add (new CodeValue (10498, vDecideds)) ;
	codeValueArr.Add (new CodeValue (10499, vCanceleds)) ;
	codeValueArr.Add (new CodeValue (10508, vDecidedBankruptcy)) ;
	codeValueArr.Add (new CodeValue (10526, vCanceledBankruptcy)) ;
	codeValueArr.Add (new CodeValue (10572, vPrvCanceledBankruptcy)) ;
	codeValueArr.Add (new CodeValue (10573, vPrvDefin)) ;
	codeValueArr.Add (new CodeValue (10574, vPrvDefinBankruptcy)) ;
	codeValueArr.Add (new CodeValue (10575, vPrvCanceled)) ;
	codeValueArr.Add (new CodeValue (10576, vPrvDefin37)) ;
}

void TAppealCitCodeValueArrayFillerThread::FillSessions()
{
	long int v192_193 = 0 ;
	long int v192_4 = 0 ;  // 08.apr.2002
	long int v192_5 = 0 ;  // 24.jun.2002
	long int v194p1 = 0 ;
	long int vDefin37 = 0 ;
	long int vOther = 0 ;
	long int v2ndFixed = 0 ;
	long int v2ndFixedBankruptcy = 0 ;
	long int vPrvFixed = 0 ;  // 08.apr.2002
	long int vPrvFixedBankruptcy = 0 ;  // 08.apr.2002
	long int v2ndLeft = 0 ;
	long int v2ndLeftBankruptcy = 0 ;
	long int vPrvLeft = 0 ;            // 20.jun.2002
	long int vPrvLeftBankruptcy = 0 ;  // 20.jun.2002

	{
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Заседания");
		wtdl->SetHeader(regimeDescr);
	}
	totalPartSteps = pQ->getRecsCount();
	while (pQ -> Read ())
	 {  // -- 1 --
		Tick(false);
		if(wtdl->WantBreak())
			break;
		(rSess) << (* pQ) ;
		if (rSess.date < cCBDEDBN.begD)
			continue ;
		 {
			TLawsuit law ;

			law.key = rSess.key ;
			if (law.Get ())
			 {
				if (law.finished.Empty () == false &&
						rSess.date > law.finished &&
						rSess.result != RESULT_FIXED)
				 {
					vOther ++ ;
					if (rSess.result == RESULT_CIT_192_193)
						v192_193 ++ ;
					if (rSess.result == RESULT_CIT_192_4)
						v192_4 ++ ;
					if (rSess.result == RESULT_CIT_192_5)
						v192_5 ++ ;
					if (rSess.result == RESULT_CIT_DECIS_194_1)
						v194p1 ++ ;
					if (rSess.result == RESULT_CIT_37)
						vDefin37 ++ ;
				 }
			 }
		 }
		 {
			TLawsuit law ;

			law.key = rSess.key ;
			if (law.Get () == false)
				continue ;
			if (law.finished.Empty () == false &&
					rSess.date > law.finished)
				continue ;

			const bool bankruptcy = bankruptcy_law (law) ;
			const bool left =
				strchr (RESULT_LEFTS_OR_STOPPEDS, rSess.result) ||
					strchr (RESULT_UNMOVEABLES, rSess.result) ;

			if (law.kind == KIND_CIT_2ND_LAW
          || law.kind == KIND_TRA_2ND_LAW  // 29.jun.2004
         )
			 {
				if (bankruptcy)
					v2ndFixedBankruptcy ++ ;
				else
					v2ndFixed ++ ;
				if (left)
					if (bankruptcy)
						v2ndLeftBankruptcy ++ ;
					else
						v2ndLeft ++ ;
			 }
			else  // 08.apr.2002 - begin
				if (law.kind == KIND_CIT_PRV_LAW
            || law.kind == KIND_TRA_PRV_LAW  // 29.jun.2004
	     )
				 {
					if (bankruptcy)
					 {
						vPrvFixedBankruptcy ++ ;
						if (left)                  // 20.jun.2002
							vPrvLeftBankruptcy ++ ;  // 20.jun.2002
					 }
					else
					 {
						vPrvFixed ++ ;
						if (left)                  // 20.jun.2002
							vPrvLeft ++ ;            // 20.jun.2002
					 }
				 }  // 08.apr.2002 - end
		 }

	 }  // -- 1 --
	codeValueArr.Add (new CodeValue (10461, v192_193)) ;
	codeValueArr.Add (new CodeValue (10506, v194p1)) ;
	codeValueArr.Add (new CodeValue (10507, vDefin37)) ;
	codeValueArr.Add (new CodeValue (10509, vOther)) ;
	codeValueArr.Add (new CodeValue (10527, v2ndFixed)) ;
	codeValueArr.Add (new CodeValue (10528, v2ndFixedBankruptcy)) ;
	codeValueArr.Add (new CodeValue (10529, v2ndLeft)) ;
	codeValueArr.Add (new CodeValue (10530, v2ndLeftBankruptcy)) ;
	codeValueArr.Add (new CodeValue (10564, vPrvFixed)) ;
	codeValueArr.Add (new CodeValue (10565, vPrvFixedBankruptcy)) ;
	codeValueArr.Add (new CodeValue (10566, v192_4)) ;
	codeValueArr.Add (new CodeValue (10577, vPrvLeft)) ;
	codeValueArr.Add (new CodeValue (10578, vPrvLeftBankruptcy)) ;
	codeValueArr.Add (new CodeValue (10586, v192_5)) ;
}

void TAppealCitCodeValueArrayFillerThread::Execute()
{
	try
	{
		const int singleSpan = (barHiVal - barLowVal) / 2;  // Gossip service

		endingVal = barLowVal + singleSpan;
		wtdl->PleaseWait();
		FillFinishedLaws();
		if(wtdl->WantBreak() == false)
		{
			Tick(true);
			endingVal = barHiVal;
			wtdl->PleaseWait();
			currentPartStep = 0;
			FillSessions();
			if(wtdl->WantBreak() == false)
			{
				Tick(true);
				wtdl->AlmostDone();
			}
		}
	}
	CATCH_ANYTHING
}

void timeBook ::
fillAppealCitCodeValueArray (CodeValueArray & codeValueArr,
														 TSession & rSess)
{
	if(strchr(cCBDEDBN.colleges, TYPE_CITIZEN_LAW)
     || strchr(cCBDEDBN.colleges, TYPE_TRADE_LAW)  // 29.jun.2004
    )
		TAppealCitCodeValueArrayFillerThread(codeValueArr, rSess, pQ, cCBDEDBN).Generate();
}  // timeBook :: fillAppealCitCodeValueArray
#   else  // of APPEAL
#   endif  // of APPEAL
# else  // INSTANCE
# endif  // INSTANCE

void timeBook :: fillCodeValueArray (CodeValueArray & codeValueArr)
 {
  if (! pQ)
    return ;
  codeValueArr.Flush () ;

  TSession * pSess = new TSession ;

  pQ -> Restart () ;
# if INSTANCE
#   if APPEAL
  fillAppealCitCodeValueArray (codeValueArr, * pSess) ;
#   else  // of APPEAL
#   endif  // of APPEAL
# else  // INSTANCE
# endif  // INSTANCE
  delete pSess ;
 }  // timeBook :: fillCodeValueArray

// 13.apr.2000 - end
void timeBook :: writeSummary (const char * bookFFN)
 {
	if (! selectSessionsOK)
		return ;

	char * formatFNS = makeSummaryFormatFN (bookFFN) ;

	if (! formatFNS)
		return ;

	CodeValueArray codeValueArr ;

// 26.apr.2000, 20.feb.2002 back again - begin
	pQ -> Restart () ;
//  delete pQ ;
//  pQ = new TCountRecsQuery (criteria) ;
// 26.apr.2000, 20.feb.2002 back again - end
	pAbPrQ -> Restart () ;
	if (cCBDEDBN.wantSummary)  // 20.sep.2000
	 {
		SessPtr pSess ;  // 20.feb.2002
		TSession & s = pSess.Ref () ;  // TSession s ;  // 20.feb.2002
		CDate forSyntaxOnly, forSyntaxOnly1 ;
		TWaitWindow ww (pParent, PLEASE_WAIT) ;  // 20.feb.2002

		while (pQ -> Read ())
		 {
			s << (* pQ) ;
			if (s.date < cCBDEDBN.begD)
				continue ;
//      if (sessResOverriddenAbolishedProgr (s,  // 20.feb.2002 - begin
//                                         forSyntaxOnly, forSyntaxOnly1))
			if (sessResOverridden_AbolishProgr (s, pAbPrQ, & forSyntaxOnly,
																	& forSyntaxOnly1))  // 20.feb.2002 - end
				cCBDEDBN.extraVal0 ++ ;
		 }
	 }
	if (cCBDEDBN.wantResultsCheckup)
  {
    register_usage(REGIME_TIME_SUMMARY, 1, cCBDEDBN.colleges, "СК - Рекап.Рез.");
		results_set_book (pParent, cCBDEDBN, pQ, true);
  }
	if (cCBDEDBN.wantSummary)  // 21.jun.2002
		fillCodeValueArray (codeValueArr) ;
	destroyQueries () ;
	if (cCBDEDBN.wantSummary)
  {
    register_usage(REGIME_TIME_SUMMARY, 1, cCBDEDBN.colleges, "СК - Рекапит.");
		writeTBSummary (formatFNS, & cCBDEDBN, pParent, & codeValueArr) ;
  }
	if (formatFNS)
    delete [] formatFNS ;
 }  // timeBook :: writeSummary

void timeBook::printTimeBook()
{
	TWaitThreadDialog wtdl;
	timeBookColumns::setShowDecisionNumber();
	char *formatFN = makeBookFormatFN();

	register_usage(REGIME_TIME_BOOK, 1, cCBDEDBN.colleges, "Срочна Книга");

	if (!formatFN)
		return;

	if (selectSessionsOK && cCBDEDBN.wantBook)
	{  // -- 0 --

		pQ->Restart();
		timeBookOutFile outF(pParent, formatFN);

		try
		{
		#if SPEEDTESTING
			#pragma message speed testing only
			DWORD ticks = GetTickCount();
		#endif  // SPEEDTESTING
			TTimeBookThread(pQ, pAbPrQ, outF, formatFN, cCBDEDBN).ExecWait();
		#if SPEEDTESTING
			message("test version: %lu d-seconds\n", (GetTickCount() - ticks) / 100);
		#endif  // SPEEDTESTING
		}
		CATCH_ANYTHING

		show_text_stream_use_settings(NULL, outF, formatFN, FLOAT_ALIGNMENT);
	}  // -- 0 --

	if (cCBDEDBN.wantSummary || cCBDEDBN.wantResultsCheckup)
	{
	#if INSTANCE
		if (cCBDEDBN.colleges [0] != TYPE_FIRM_LAW)
	#endif
			writeSummary(formatFN);
	#if INSTANCE
		else
			destroyQueries();
	#endif
	}
	else
		destroyQueries();

	if (cCBDEDBN.wantJudgesSummary)
	{
	#if INSTANCE
	#if APPEAL
		if (cCBDEDBN.colleges [0] == TYPE_CITIZEN_LAW || cCBDEDBN.colleges [0] == TYPE_TRADE_LAW)
		{
			TIlievaJudgeCheckupThread jC(cCBDEDBN);

			register_usage(REGIME_TIME_JUDGES_SUMMARY, 1, cCBDEDBN.colleges, "СС - Срочна кн.");  // 27.dec.2004
			jC.prepare();
			jC.printJudgeCheckup();
		}
		else
		{
			TJudgeCheckupThread jC(cCBDEDBN);

			register_usage(REGIME_TIME_JUDGES_SUMMARY, 2, cCBDEDBN.colleges, "СС - Срочна кн.");  // 27.dec.2004
			jC.prepare();
			jC.printJudgeCheckup();
	     }
	#else  // of APPEAL
		if (cCBDEDBN.colleges [0] != TYPE_FIRM_LAW)
		{
			TJudgeCheckupThread jC(cCBDEDBN);

			register_usage(REGIME_TIME_JUDGES_SUMMARY, 1, cCBDEDBN.colleges, "СС - Срочна кн.");  // 27.dec.2004
			jC.prepare();
			jC.printJudgeCheckup();
		}
	#endif  // of APPEAL
	#else  // of INSTANCE
		{
			TJudgeCheckupThread jC(cCBDEDBN);

			register_usage(REGIME_TIME_JUDGES_SUMMARY, 1, cCBDEDBN.colleges, "СС - Срочна кн.");  // 27.dec.2004
			jC.prepare();
			jC.printJudgeCheckup();
		}
	#endif  // of INSTANCE
	}

	delete[] formatFN;
}  // timeBook :: printTimeBook

void print_time_book (TWindow * pParent)
 {
  timeBook (pParent).printTimeBook () ;
 }  // print_time_book
