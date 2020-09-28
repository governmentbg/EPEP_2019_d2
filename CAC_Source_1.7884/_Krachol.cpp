#include "cac.h"  // 18.may.2001
//# include "all.h"

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
// ======================== KracholovJCData =============================
// ============================= Begin ==================================
size_t KracholovJCData :: Size () const
 {
  const char * b = (const char *) (& S_KracholovReporteds1) ;
  const char * e = (const char *) (& L_KracholovFinishedsTotal) +
                   sizeof (L_KracholovFinishedsTotal) ;
  return (e - b) ;
 }  // KracholovJCData :: Size

void KracholovJCData :: ClearData ()
 {
  setmem (& S_KracholovReporteds1, Size (), '\0') ;
 }  // KracholovJCData :: ClearData

bool KracholovJCData ::
operator == (const KracholovJCData & otherIJCD) const
 {
  return ! memcmp (& S_KracholovReporteds1,
			 & (otherIJCD.S_KracholovReporteds1), Size ()) ;
 }  // KracholovJCData :: operator ==

bool KracholovJCData :: IsEmpty () const
 {
  KracholovJCData rL1 ;

  return ((* this) == rL1) ;
 }  // KracholovJCData :: IsEmpty

KracholovJCData & KracholovJCData ::
  operator += (const KracholovJCData & otherKJCD)
 {
  S_KracholovReporteds1 += otherKJCD.S_KracholovReporteds1 ;
  S_KracholovReportedsGenChar1 += otherKJCD.S_KracholovReportedsGenChar1 ;
  S_KracholovReportedsLeft1 += otherKJCD.S_KracholovReportedsLeft1 ;
  L_KracholovFinisheds1 += otherKJCD.L_KracholovFinisheds1 ;
  L_KracholovFinsGenChJudgem1 += otherKJCD.L_KracholovFinsGenChJudgem1 ;
  L_KracholovFinsGenCh241_1 += otherKJCD.L_KracholovFinsGenCh241_1 ;
  L_KracholovFinsGenCh287_1 += otherKJCD.L_KracholovFinsGenCh287_1 ;
  L_KracholovFinsPrvCh1 += otherKJCD.L_KracholovFinsPrvCh1 ;
  L_KracholovFinsPrvPreCrt1 += otherKJCD.L_KracholovFinsPrvPreCrt1 ;
  L_KracholovFinsAdmin1 += otherKJCD.L_KracholovFinsAdmin1 ;
  S_KracholovReporteds2 += otherKJCD.S_KracholovReporteds2 ;
  S_KracholovReporteds2nd2 += otherKJCD.S_KracholovReporteds2nd2 ;
  S_KracholovReporteds2ndLeft2 += otherKJCD.S_KracholovReporteds2ndLeft2 ;
  S_KracholovReportedsPrvC2 += otherKJCD.S_KracholovReportedsPrvC2 ;
  S_KracholovReportedsAdm2 += otherKJCD.S_KracholovReportedsAdm2 ;
  S_KracholovReportedsAdmLeft2 += otherKJCD.S_KracholovReportedsAdmLeft2 ;
  S_KracholovReportedsPreCrt2 += otherKJCD.S_KracholovReportedsPreCrt2 ;
  L_KracholovFinisheds2 += otherKJCD.L_KracholovFinisheds2 ;
  L_KracholovFinisheds2nd2 += otherKJCD.L_KracholovFinisheds2nd2 ;
  L_KracholovFinishedsPrvC2 += otherKJCD.L_KracholovFinishedsPrvC2 ;
  L_KracholovFinishedsAdm2 += otherKJCD.L_KracholovFinishedsAdm2 ;
  L_KracholovFinishedsPreCrt2 += otherKJCD.L_KracholovFinishedsPreCrt2 ;
  S_KracholovReportedsTotal += otherKJCD.S_KracholovReportedsTotal ;
  S_KracholovUWAOver30d += otherKJCD.S_KracholovUWAOver30d ;
  L_KracholovFinishedsTotal += otherKJCD.L_KracholovFinishedsTotal ;
  return (* this) ;
 }  // KracholovJCData & KracholovJCData :: operator +=

// ============================ End of ==================================
// ======================== KracholovJCData =============================

/////////////////////////////////////////////////////////////////////////

// =================== KracholovJudgeCheckupData ========================
// ============================= Begin ==================================
void KracholovJudgeCheckupData :: clearData ()
 {
  judgeCheckupData :: clearData () ;
  KracholovData.ClearData () ;
 }  // KracholovJudgeCheckupData :: clearData

bool KracholovJudgeCheckupData :: countingDataEmpty () const
 {
  return (judgeCheckupData :: countingDataEmpty () &&
          KracholovData.IsEmpty ()
	   ) ;
 }  // KracholovJudgeCheckupData :: countingDataEmpty

void
KracholovJudgeCheckupData :: operator += (const judgeCheckupData & other)
 {
  judgeCheckupData :: operator += (other) ;

  const KracholovJudgeCheckupData * otherKracholov =
    dynamic_cast <const KracholovJudgeCheckupData *> (& other) ;

  if (otherKracholov)
   {
    KracholovData += otherKracholov -> KracholovData ;
   } 
 }  // KracholovJudgeCheckupData :: operator

// ============================ End of ==================================
// =================== KracholovJudgeCheckupData ========================

/////////////////////////////////////////////////////////////////////////

// ================== KracholovJudgeCheckupColumns ======================
// ============================= Begin ==================================
void KracholovJudgeCheckupColumns ::
       MakeColumns (const KracholovJudgeCheckupData & jCD)
 {
  const KracholovJCData & KJCD = jCD.KracholovData ;

  addNmbToClmn (columns [iC_S_KracholovReporteds1],
                KJCD.S_KracholovReporteds1) ;
  addNmbToClmn (columns [iC_S_KracholovReportedsGenChar1],
                KJCD.S_KracholovReportedsGenChar1) ;
  addNmbToClmn (columns [iC_S_KracholovReportedsLeft1],
                KJCD.S_KracholovReportedsLeft1) ;
  addNmbToClmn (columns [iC_L_KracholovFinisheds1],
                KJCD.L_KracholovFinisheds1) ;
  addNmbToClmn (columns [iC_L_KracholovFinsGenChJudgem1],
                KJCD.L_KracholovFinsGenChJudgem1) ;
  addNmbToClmn (columns [iC_L_KracholovFinsGenCh241_1],
                KJCD.L_KracholovFinsGenCh241_1) ;
  addNmbToClmn (columns [iC_L_KracholovFinsGenCh287_1],
                KJCD.L_KracholovFinsGenCh287_1) ;
  addNmbToClmn (columns [iC_L_KracholovFinsPrvCh1],
                KJCD.L_KracholovFinsPrvCh1) ;
  addNmbToClmn (columns [iC_L_KracholovFinsPrvPreCrt1],
                KJCD.L_KracholovFinsPrvPreCrt1) ;
  addNmbToClmn (columns [iC_L_KracholovFinsAdmin1],
                KJCD.L_KracholovFinsAdmin1) ;
  addNmbToClmn (columns [iC_S_KracholovReporteds2],
                KJCD.S_KracholovReporteds2) ;
  addNmbToClmn (columns [iC_S_KracholovReporteds2nd2],
                KJCD.S_KracholovReporteds2nd2) ;
  addNmbToClmn (columns [iC_S_KracholovReporteds2ndLeft2],
                KJCD.S_KracholovReporteds2ndLeft2) ;
  addNmbToClmn (columns [iC_S_KracholovReportedsPrvC2],
                KJCD.S_KracholovReportedsPrvC2) ;
  addNmbToClmn (columns [iC_S_KracholovReportedsAdm2],
                KJCD.S_KracholovReportedsAdm2) ;
  addNmbToClmn (columns [iC_S_KracholovReportedsAdmLeft2],
                KJCD.S_KracholovReportedsAdmLeft2) ;
  addNmbToClmn (columns [iC_S_KracholovReportedsPreCrt2],
                KJCD.S_KracholovReportedsPreCrt2) ;
  addNmbToClmn (columns [iC_L_KracholovFinisheds2],
                KJCD.L_KracholovFinisheds2) ;
  addNmbToClmn (columns [iC_L_KracholovFinisheds2nd2],
                KJCD.L_KracholovFinisheds2nd2) ;
  addNmbToClmn (columns [iC_L_KracholovFinishedsPrvC2],
                KJCD.L_KracholovFinishedsPrvC2) ;
  addNmbToClmn (columns [iC_L_KracholovFinishedsAdm2],
                KJCD.L_KracholovFinishedsAdm2) ;
  addNmbToClmn (columns [iC_L_KracholovFinishedsPreCrt2],
                KJCD.L_KracholovFinishedsPreCrt2) ;
  addNmbToClmn (columns [iC_S_KracholovReportedsTotal],
                KJCD.S_KracholovReportedsTotal) ;
  addNmbToClmn (columns [iC_L_KracholovFinishedsTotal],
                KJCD.L_KracholovFinishedsTotal) ;
  addNmbToClmn (columns [iC_S_KracholovUWAOver30d],
                KJCD.S_KracholovUWAOver30d) ;
 }  // KracholovJudgeCheckupColumns :: MakeColumns

KracholovJudgeCheckupColumns ::
KracholovJudgeCheckupColumns (const char * const formatFN,
                              TWindow * parent,
                              const KracholovJudgeCheckupData & jCD,
                              const CDate * const pDateForMonthColumn,
                              const int serialNo
                             ) :
              judgeCheckupColumns (formatFN, parent, jCD,
                                   pDateForMonthColumn, serialNo)
 {
  MakeColumns (jCD) ;
  defineMaxRows () ;
 }  // KracholovJudgeCheckupColumns :: KracholovJudgeCheckupColumns

// ============================ End of ==================================
// ================== KracholovJudgeCheckupColumns ======================

/////////////////////////////////////////////////////////////////////////

// ----- TKracholovJudgeCheckupThread ----------------------------------------
void TKracholovJudgeCheckupThread::makeBookFormatFN()
 {
  scSprintf (sizeof (formatFN), formatFN, "text\\ju001pi%s",
		 DEFAULT_FORMAT_FILE_EXTENSION) ;
 }

judgeCheckupData * TKracholovJudgeCheckupThread::newDataItem() const
 {
  return new KracholovJudgeCheckupData ;
 }

judgeCheckupOutFile * TKracholovJudgeCheckupThread::newOutFile() const
 {
  return new KracholovJudgeCheckupOutFile(wtdl->Parent, formatFN);
 }

judgeCheckupColumns * TKracholovJudgeCheckupThread::newJudgeCheckupColumns (const char * const formatFN,
	const judgeCheckupData & jCD, const CDate * const pDateForMonthColumn, const int serialNo) const
 {
  const KracholovJudgeCheckupData * pIJCD =
    dynamic_cast <const KracholovJudgeCheckupData *> (& jCD) ;

  if (pIJCD)
   {
    return
	new KracholovJudgeCheckupColumns (formatFN, wtdl->Parent, (* pIJCD),
                                        pDateForMonthColumn, serialNo) ;
   }
  else
   {
    fatal ("K????????JudgeCheckup::newJudgeCheckupColumns: "
           "unexpected type of the jCD parameter") ;
    return NULL ;
   }
 }

void TKracholovJudgeCheckupThread::IncrementValues_Judge(const char * const judgeUcn, unsigned int dataCiph)
 {
  if (! ucnPresent (judgeUcn))
    return ;
  addJCDToArrayIfNecessary (judgeUcn) ;
   {  // -- 0b --
    const long int iV = 1 ;
    bool found = false ;

    for (int i = 0 ; ! found && i < pArr -> Count () ;
	   i ++)
     {  // -- 1 --
	KracholovJudgeCheckupData * KJCD_Ptr =
        dynamic_cast <KracholovJudgeCheckupData *> ((* pArr) [i]) ;

      if (! KJCD_Ptr)
        continue ;
      if (strcmp (KJCD_Ptr -> judgeUCN, judgeUcn) == 0)
       {  // -- 1a --
        found = true ;
        KracholovJCData & KJCD = KJCD_Ptr -> KracholovData ;

        switch (dataCiph)
         {  // -- 4 --
          case iC_S_KracholovReporteds1 :
            KJCD.S_KracholovReporteds1 += iV ;
            break ;
          case iC_S_KracholovReportedsGenChar1 :
            KJCD.S_KracholovReportedsGenChar1 += iV ;
            break ;
          case iC_S_KracholovReportedsLeft1 :
            KJCD.S_KracholovReportedsLeft1 += iV ;
            break ;
          case iC_L_KracholovFinisheds1 :
            KJCD.L_KracholovFinisheds1 += iV ;
            break ;
          case iC_L_KracholovFinsGenChJudgem1 :
            KJCD.L_KracholovFinsGenChJudgem1 += iV ;
            break ;
          case iC_L_KracholovFinsGenCh241_1 :
            KJCD.L_KracholovFinsGenCh241_1 += iV ;
            break ;
          case iC_L_KracholovFinsGenCh287_1 :
            KJCD.L_KracholovFinsGenCh287_1 += iV ;
            break ;
          case iC_L_KracholovFinsPrvCh1 :
            KJCD.L_KracholovFinsPrvCh1 += iV ;
            break ;
          case iC_L_KracholovFinsPrvPreCrt1 :
						KJCD.L_KracholovFinsPrvPreCrt1 += iV ;
            break ;
          case iC_L_KracholovFinsAdmin1 :
            KJCD.L_KracholovFinsAdmin1 += iV ;
            break ;
          case iC_S_KracholovReporteds2 :
            KJCD.S_KracholovReporteds2 += iV ;
            break ;
          case iC_S_KracholovReporteds2nd2 :
            KJCD.S_KracholovReporteds2nd2 += iV ;
            break ;
          case iC_S_KracholovReporteds2ndLeft2 :
            KJCD.S_KracholovReporteds2ndLeft2 += iV ;
            break ;
          case iC_S_KracholovReportedsPrvC2 :
            KJCD.S_KracholovReportedsPrvC2 += iV ;
            break ;
          case iC_S_KracholovReportedsAdm2 :
            KJCD.S_KracholovReportedsAdm2 += iV ;
            break ;
          case iC_S_KracholovReportedsAdmLeft2 :
            KJCD.S_KracholovReportedsAdmLeft2 += iV ;
            break ;
					case iC_S_KracholovReportedsPreCrt2 :
            KJCD.S_KracholovReportedsPreCrt2 += iV ;
            break ;
          case iC_L_KracholovFinisheds2 :
            KJCD.L_KracholovFinisheds2 += iV ;
            break ;
          case iC_L_KracholovFinisheds2nd2 :
            KJCD.L_KracholovFinisheds2nd2 += iV ;
            break ;
          case iC_L_KracholovFinishedsPrvC2 :
            KJCD.L_KracholovFinishedsPrvC2 += iV ;
            break ;
          case iC_L_KracholovFinishedsAdm2 :
            KJCD.L_KracholovFinishedsAdm2 += iV ;
            break ;
          case iC_L_KracholovFinishedsPreCrt2 :
            KJCD.L_KracholovFinishedsPreCrt2 += iV ;
            break ;
          case iC_S_KracholovReportedsTotal :
            KJCD.S_KracholovReportedsTotal += iV ;
            break ;
          case iC_L_KracholovFinishedsTotal :
            KJCD.L_KracholovFinishedsTotal += iV ;
						break ;
          case iC_S_KracholovUWAOver30d :
            KJCD.S_KracholovUWAOver30d += iV ;
            break ;
         }  // -- 4 --  switch
       }  // -- 1a --
     }  // -- 1 --  for
   }  // -- 0b --
 }

void TKracholovJudgeCheckupThread::RegimeStr(CharPtr& dest, const char* const regime)
{
	if(dest.Size() > 0)
	{
		{
			ostrstream oss(dest, dest.Size ());

			oss << "Справка по съдии";
			if(regime && regime[0])
				oss << " - " << regime;
			oss << ends;
		}
		dest[dest.Size() - 1] = '\0';
	}
}

void TKracholovJudgeCheckupThread::FinishedLawsDistribute (const TLawsuit & law, const TSession & sess)
 {
  const char * const judgeUcn = sess.judge ;

  IncrementValues_Judge (judgeUcn, iC_L_KracholovFinishedsTotal) ;
  if (strchr (KIND_1ST_LAWSUITS, law.kind))
   {  // -- 0 --
    IncrementValues_Judge (judgeUcn, iC_L_KracholovFinisheds1) ;
    if (law.kind == KIND_PUNISHMENT_GENERAL_CHAR_LAW)
     {  // -- 1 --
      switch (sess.result)
       {
        case RESULT_WITH_JUDGEMENT :
        case RESULT_WITH_BULWARK :  // 17.sep.2001
        case RESULT_WITH_JUDGEMENT_BULWARK :  // 17.sep.2001
          IncrementValues_Judge (judgeUcn,
                                 iC_L_KracholovFinsGenChJudgem1) ;
          break ;
				case RESULT_CANCEL_241 :
				case RESULT_CANCELED :
          IncrementValues_Judge (judgeUcn, iC_L_KracholovFinsGenCh241_1) ;
          break ;
        case RESULT_CANCEL_FOR_MORE_LOOK_INTO :
          IncrementValues_Judge (judgeUcn, iC_L_KracholovFinsGenCh287_1) ;
          break ;
       }  // switch
     }  // -- 1 --
    else
	if (law.kind == KIND_PRIVATE_PUNISHMENT_LAW)
       {
        if (law_is_pre_court (law))
          IncrementValues_Judge (judgeUcn, iC_L_KracholovFinsPrvPreCrt1) ;
        else
          IncrementValues_Judge (judgeUcn, iC_L_KracholovFinsPrvCh1) ;
       }
      else
        if (law.kind == KIND_1ST_ADMIN_PUNISHMENT_LAW)
          IncrementValues_Judge (judgeUcn, iC_L_KracholovFinsAdmin1) ;
   }  // -- 0 --
  else
   {  // -- 2 --
    IncrementValues_Judge (judgeUcn, iC_L_KracholovFinisheds2) ;
		// Attentively! The order below is important!
    // (because of sub-sets)
    if (law_is_pre_court (law))
      IncrementValues_Judge (judgeUcn,
                             iC_L_KracholovFinishedsPreCrt2) ;
    else
      if (strchr (KIND_EXACT_COMPLAINTS_CONCERN, law.sourceKind) ||
          strchr (KIND_PRIVATE_COMPLAINTS_CONCERN, law.sourceKind))
        IncrementValues_Judge (judgeUcn, iC_L_KracholovFinishedsAdm2) ;
      else
        if (strchr (KIND_PRIVATE_COMPLAINTS, law.sourceKind) ||
            strchr (KIND_PRIVATE_OBJECTIONS, law.sourceKind))
          IncrementValues_Judge (judgeUcn, iC_L_KracholovFinishedsPrvC2) ;
        else
          if (strchr (KIND_EXACT_COMPLAINTS, law.sourceKind) ||
              strchr (KIND_EXACT_OBJECTIONS, law.sourceKind))
            IncrementValues_Judge (judgeUcn,
                                   iC_L_KracholovFinisheds2nd2) ;
   }  // -- 2 --
 }

void TKracholovJudgeCheckupThread::CountFinishedLaws(const CDate begDate, const CDate endDate)
 {
	CharPtr regimeDescr(64);

	RegimeStr(regimeDescr, "Свършили дела");
	wtdl->SetHeader(regimeDescr);
	FinalizedSessions finS (bMYEMYTCIU.types, begDate, endDate) ;
	 {
		TSession * pSess = new TSession ;
		TLawsuit law ;

	  totalPartSteps = finS.FinishedLawsNumber();
		do
		 {
			Tick(false);
			if(wtdl->WantBreak())
				break;
			if (finS.NextSession (law, (* pSess)))
        if (bMYEMYTCIU.composition == 0 ||  // 08.feb.2001
		bMYEMYTCIU.composition == law.composition) // 08.feb.2001
          FinishedLawsDistribute (law, * pSess) ;
     }
    while (TRCDKey_ (law.key).FilledOK ()) ;
    delete pSess ;
   }
 }

void TKracholovJudgeCheckupThread::SessionsDistribute(const TLawsuit & law, const TSession & sess)
 {
  IncrementValues_Judge (sess.judge, iC_S_KracholovReportedsTotal) ;
  if (strchr (KIND_1ST_LAWSUITS, law.kind))
	 {  // -- 0 --
    IncrementValues_Judge (sess.judge, iC_S_KracholovReporteds1) ;
    if (law.kind == KIND_PUNISHMENT_GENERAL_CHAR_LAW)
      IncrementValues_Judge (sess.judge,
                             iC_S_KracholovReportedsGenChar1) ;
    // No else --> especially                         
    if (strchr (RESULT_LEFTS_OR_STOPPEDS, sess.result) ||
        strchr (RESULT_UNMOVEABLES, sess.result))
      IncrementValues_Judge (sess.judge,
                             iC_S_KracholovReportedsLeft1) ;
   }  // -- 0 --
  else
   {  // -- 1 --
    IncrementValues_Judge (sess.judge, iC_S_KracholovReporteds2) ;
    // Attentively! The order below is important!
    // (because of sub-sets)
    if (law_is_pre_court (law))
      IncrementValues_Judge (sess.judge,
                             iC_S_KracholovReportedsPreCrt2) ;
    else
      if (strchr (KIND_EXACT_COMPLAINTS_CONCERN, law.sourceKind) ||
          strchr (KIND_PRIVATE_COMPLAINTS_CONCERN, law.sourceKind))
       {
				IncrementValues_Judge (sess.judge,
                               iC_S_KracholovReportedsAdm2) ;
        if (strchr (RESULT_LEFTS_OR_STOPPEDS, sess.result) ||
            strchr (RESULT_UNMOVEABLES, sess.result))
          IncrementValues_Judge (sess.judge,
                                 iC_S_KracholovReportedsAdmLeft2) ;
       }
      else
        if (strchr (KIND_PRIVATE_COMPLAINTS, law.sourceKind) ||
            strchr (KIND_PRIVATE_OBJECTIONS, law.sourceKind))
          IncrementValues_Judge (sess.judge,
                                 iC_S_KracholovReportedsPrvC2) ;
      else
        if (strchr (KIND_EXACT_COMPLAINTS, law.sourceKind) ||
            strchr (KIND_EXACT_OBJECTIONS, law.sourceKind))
         {
          IncrementValues_Judge (sess.judge,
                                 iC_S_KracholovReporteds2nd2) ;
          if (strchr (RESULT_LEFTS_OR_STOPPEDS, sess.result) ||
              strchr (RESULT_UNMOVEABLES, sess.result))
            IncrementValues_Judge (sess.judge,
                                   iC_S_KracholovReporteds2ndLeft2) ;
         }
	 }  // -- 1 --
 }

void TKracholovJudgeCheckupThread::CountSessions (const CDate begDate, const CDate endDate)
 {
	const size_t crtSz = 1024 * 2 ;
	char * const crt = new char [crtSz] ;
	const char * const what =
		"L.F_KIND, L.F_SUBJECT, L.F_SOURCE_KIND, "
		"S.F_NO, S.F_YEAR, S.F_TYPE, "
		"S.F_JUDGE, S.F_RESULT " ;
	char sessResToExclude [16] ;

	scSprintf (sizeof (sessResToExclude), sessResToExclude,
						 "%c", RESULT_FIXED) ;
	if (composeJudgeCheckupCriteria (crt, crtSz, what, bMYEMYTCIU.types,
																	 bMYEMYTCIU.composition, & endDate,
																	 & begDate, sessResToExclude, NULL,
																	 NULL, false, NULL, NULL, NULL, false,
																	 false, false, true))
	 {  // -- 0 --
		CharPtr regimeDescr(64);

		RegimeStr(regimeDescr, "Заседания");
		wtdl->SetHeader(regimeDescr);
		TCountRecsQuery q (crt) ;
			// TQuery  // 10.jul.2002
		TSession * pSess = new TSession ;
		TLawsuit law ;

	  totalPartSteps = q.getRecsCount();
		while (q.Read ())
		 {  // -- 1 --
			Tick(false);
			if(wtdl->WantBreak())
				break;
			(* pSess) << q ;
			law << q ;
			pSess -> kind = '\0' ;
	law.key = pSess -> key ;
	SessionsDistribute (law, * pSess) ;
     }  // -- 1 --
    delete pSess ;
   }  // -- 0 --
  delete [] crt ;
 }

void TKracholovJudgeCheckupThread::UnwrittensDistribute (const TLawsuit &, const TSession & sess, const CDate endDate)
 {
  const char * const judgeUcn = sess.judge ;
  const bool over30d =
    difference_in_days ((strchr (KIND_CLOSED_SESSIONS, sess.kind) ?
                         sess.returned : sess.date),
                        endDate) > 30 ;

  if (over30d)
    IncrementValues_Judge (judgeUcn, iC_S_KracholovUWAOver30d) ;
 }

#   if UNWRITTEN_ACTS_CALL_JIMMY  // 08.feb.2000 - begin
void TKracholovJudgeCheckupThread :: CountUnwrittens (const CDate, const CDate endDate)
 {
	TLawsuit law ;
	TSession * pSess = new TSession ;
	CharPtr regimeDescr(96);

	RegimeStr(regimeDescr, "Ненаписани съдебни актове");
	wtdl->SetHeader(regimeDescr);
	SessionsToCheck sessions (bMYEMYTCIU.types, DATE_MIN, endDate, 0) ;
		// Especially I do not use bMYEMYTCIU.composition
		// Later I shall use law.composition

	totalPartSteps = sessions.SessionsFound();
	clearGroupData (law) ;
	while (sessions.NextSession (* pSess))
	 {
		Tick(false);
		if(wtdl->WantBreak())
			break;
		if ((TRCDKey_(law.key) == TRCDKey_(pSess -> key)) == false)
		 {
			law.key = pSess -> key ;
			if (law.Get () == false)
			 {
				clearGroupData (law) ;
				continue ;
			 }
		 }
		if (bMYEMYTCIU.composition == 0 ||
				bMYEMYTCIU.composition == law.composition)
			if (unwritten_act (pSess))
				UnwrittensDistribute (law, * pSess, endDate) ;
	 }
	delete pSess ;
 }  // KracholovJudgeCheckup :: CountUnwrittens
#   else  // 0f UNWRITTEN_ACTS_CALL_JIMMY  // 08.feb.2000 - end
void TKracholovJudgeCheckupThread::CountUnwrittens (const CDate begDate, const CDate endDate)
 {
	const size_t tmpResSz = strlen (RESULT_ENTERABLES) + 1 ;
	char * const tmpRes = new char [tmpResSz] ;

	scSprintf (tmpResSz, tmpRes, "%s", RESULT_ENTERABLES) ;
	intersection (tmpRes, RESULT_FINALIZEDS) ;
	if (tmpRes [0])
	 {  // -- 0 --
		CharPtr regimeDescr(96);

		RegimeStr(regimeDescr, "Ненаписани съдебни актове");
		UpdateDescription(regimeDescr);
		FinalizedSessions finSU (bMYEMYTCIU.types, begDate, endDate) ;
		 {  // -- 1 --
			TSession * pSess = new TSession ;
			TLawsuit law ;

			totalPartSteps = finSU.FinishedLawsNumber();
			do
			 {  // -- 2 --
				Tick(false);
				if(WantBreak())
					break;
				if (finSU.NextSession (law, (* pSess)))
	   {  // -- 3 --
	    if (strchr (tmpRes, pSess -> result))
            if (bMYEMYTCIU.composition == 0 ||
                bMYEMYTCIU.composition == law.composition)
             {  // -- 4 --
              SessionEndDocs endDocs (law.key, pSess->date, pSess->kind) ;

              if (endDocs.DocsFound () == 0)
                UnwrittensDistribute (law, * pSess, endDate) ;
             }  // -- 4 --
         }  // -- 3 --
       }  // -- 2 --
      while (TRCDKey_ (law.key).FilledOK ()) ;
      delete pSess ;
     }  // -- 1 --
	 }  // -- 0 --
	delete [] tmpRes ;
 }
#   endif  // 0f UNWRITTEN_ACTS_CALL_JIMMY  // 08.feb.2000

void TKracholovJudgeCheckupThread::PrepareNextStep(const int setEndingValTo)
{
	Tick(true);
	endingVal = setEndingValTo;
	wtdl->PleaseWait();
	currentPartStep = 0;
}

void TKracholovJudgeCheckupThread::Execute()
{
	if(useInheritedGossips)
	{
		 TJudgeCheckupThread::Execute();
		 return;
	}

	const CDate begD = bMYEMYTCIU.BegDate () ;
	const CDate endD = bMYEMYTCIU.EndDate () ;

	try
	{  // -- 1 --
		if(wtdl->WantBreak() == false)
		{
		 // If you add a new brick: do not forget to include it in all
		 // places, where it is necessary!
			const int bricks =
				 2 + (includingUnwrittens ? 2 : 0);  // CountUnwrittens is
				// very slow, so I assign double indicator space to it.
			const int singleSpan =
				(barHiVal - barLowVal) / bricks;                 // Gossip service

			endingVal = barLowVal + singleSpan;                // Gossip service
			passeds = "Обработени";                            // Gossip service
			reportFinishedItems = true;                        // Gossip service
			wtdl->PleaseWait();
			currentPartStep = 0;                               // Gossip service
			lastShownVal = barLowVal;                          // Gossip service
			if(includingUnwrittens)
			{
				CountFinishedLaws(begD, endD);                   // Do work 1
				if(wtdl->WantBreak() == false)                         // Gossip service
				{
					PrepareNextStep(endingVal + singleSpan);       // Gossip service
					CountSessions(begD, endD);                     // Do work 2
					if(wtdl->WantBreak() == false)                       // Gossip service
					{
						PrepareNextStep(barHiVal);                   // Gossip service
						CountUnwrittens(DATE_MIN, endD);             // Do work 3, 4
						if(wtdl->WantBreak() == false)                     // Gossip service
						{
							Tick(true);
							wtdl->AlmostDone();
						}
					}
				}
			}
			else
			{
				CountFinishedLaws(begD, endD);                   // Do work 1
				if(wtdl->WantBreak() == false)
				{
					PrepareNextStep(barHiVal);
					CountSessions(begD, endD);                     // Do work 2
					if(wtdl->WantBreak() == false)
					{
						Tick(true);
						wtdl->AlmostDone();
					}
				}
			}
		}
	}  // -- 1 --
	CATCH_ANYTHING
}

void TKracholovJudgeCheckupThread::prepare()
 {
  if (! pArr)
    return ;
	useInheritedGossips = true ;
	sayAlmostDone = false ;
	TJudgeCheckupThread :: prepare () ;
	useInheritedGossips = false ;
	Generate () ;
 }

TKracholovJudgeCheckupThread::TKracholovJudgeCheckupThread(const begMYEndMYTypeCIU & rBMYEMYTCIU) :
	TJudgeCheckupThread (rBMYEMYTCIU),
	useInheritedGossips (true),  // 10.jul.2002
	includingUnwrittens (confirm_unwritten_acts_search ())
 {
 }

TKracholovJudgeCheckupThread::~TKracholovJudgeCheckupThread()
 {
 }

#   endif  // of APPEAL
# endif  // of INSTANCE