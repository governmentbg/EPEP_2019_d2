#include "cac.h"  // 18.may.2001
//# include "all.h"

#if COURT_TYPE == COURT_AREAL
// ==========================  FirmJCData ===============================
// ============================= Begin ==================================
size_t FirmJCData :: Size () const
 {
  const char * b = (const char *) (& L_Firm_Feceived) ;
  const char * e = (const char *) (& D_Firm_Changed) +
                   sizeof (D_Firm_Changed) ;
  return (e - b) ;
 }  // FirmJCData :: Size

void FirmJCData :: ClearData ()
 {
  setmem (& L_Firm_Feceived, Size (), '\0') ;
 }  // FirmJCData :: ClearData

bool FirmJCData ::
operator == (const FirmJCData & otherFirmJCD) const
 {
  return ! memcmp (& L_Firm_Feceived,
			 & (otherFirmJCD.L_Firm_Feceived), Size ()) ;
 }  // FirmJCData :: operator ==

bool FirmJCData :: IsEmpty () const
 {
  FirmJCData rL1 ;

  return ((* this) == rL1) ;
 }  // FirmJCData :: IsEmpty

FirmJCData & FirmJCData ::
  operator += (const FirmJCData & otherFirmJCD)
 {
  L_Firm_Feceived += otherFirmJCD.L_Firm_Feceived ;
  L_Firm_FeceivedRejudge += otherFirmJCD.L_Firm_FeceivedRejudge ; // 11.dec.2003
  D_Firm_Entered += otherFirmJCD.D_Firm_Entered ;
  D_Firm_RegistrationRefused += otherFirmJCD.D_Firm_RegistrationRefused ;
  D_Firm_ChangeRefused += otherFirmJCD.D_Firm_ChangeRefused ;
  D_Firm_Erased += otherFirmJCD.D_Firm_Erased ;
  D_Firm_CancelDecision += otherFirmJCD.D_Firm_CancelDecision ;
  D_Other_TradeRegister += otherFirmJCD.D_Other_TradeRegister ;  // 21.feb.2005
  D_Other_NoTradeRegister += otherFirmJCD.D_Other_NoTradeRegister ;//21.feb.2005
  D_Firm_Changed += otherFirmJCD.D_Firm_Changed ;
  return (* this) ;
 }  // FirmJCData & FirmJCData :: operator +=

// ============================ End of ==================================
// =========================== FirmJCData ===============================

/////////////////////////////////////////////////////////////////////////

// ====================== FirmJudgeCheckupData ==========================
// ============================= Begin ==================================
void FirmJudgeCheckupData :: clearData ()
 {
  judgeCheckupData :: clearData () ;
  firmData.ClearData () ;
 }  // FirmJudgeCheckupData :: clearData

bool FirmJudgeCheckupData :: countingDataEmpty () const
 {
  return (judgeCheckupData :: countingDataEmpty () &&
          firmData.IsEmpty ()
	   ) ;
 }  // FirmJudgeCheckupData :: countingDataEmpty

void
FirmJudgeCheckupData :: operator += (const judgeCheckupData & other)
 {
  judgeCheckupData :: operator += (other) ;

  const FirmJudgeCheckupData * otherFirm =
    dynamic_cast <const FirmJudgeCheckupData *> (& other) ;

  if (otherFirm)
   {
    firmData += otherFirm -> firmData ;
   }
 }  // FirmJudgeCheckupData :: operator =

// ============================ End of ==================================
// ====================== FirmJudgeCheckupData ==========================

/////////////////////////////////////////////////////////////////////////

// ===================== FirmJudgeCheckupColumns ========================
// ============================= Begin ==================================
void FirmJudgeCheckupColumns ::
       MakeColumns (const FirmJudgeCheckupData & jCD)
 {
  const FirmJCData & FJCD = jCD.firmData ;

  addNmbToClmn (columns [iC_Firm_Feceived],
                FJCD.L_Firm_Feceived) ;
  addNmbToClmn (columns [iC_Firm_FeceivedRejudge],
                FJCD.L_Firm_FeceivedRejudge) ;  // 11.dec.2003
  addNmbToClmn (columns [iC_Firm_Entered],
                FJCD.D_Firm_Entered) ;
  addNmbToClmn (columns [iC_Firm_RegistrationRefused],
                FJCD.D_Firm_RegistrationRefused) ;
  addNmbToClmn (columns [iC_Firm_ChangeRefused],
                FJCD.D_Firm_ChangeRefused) ;
  addNmbToClmn (columns [iC_Firm_Erased],
                FJCD.D_Firm_Erased) ;
  addNmbToClmn (columns [iC_Firm_CancelDecision],
                FJCD.D_Firm_CancelDecision) ;
  addNmbToClmn (columns [iC_Firm_Other_TradeRegister],
                FJCD.D_Other_TradeRegister) ;  // 21.feb.2005
  addNmbToClmn (columns [iC_Firm_Other_NoTradeRegister],
                FJCD.D_Other_NoTradeRegister) ;  // 21.feb.2005
  addNmbToClmn (columns [iC_Firm_Changed],
                FJCD.D_Firm_Changed) ;
 }  // FirmJudgeCheckupColumns :: MakeColumns

FirmJudgeCheckupColumns ::
FirmJudgeCheckupColumns (const char * const formatFN,
                          TWindow * parent,
                          const FirmJudgeCheckupData & jCD,
                          const CDate * const pDateForMonthColumn,
                          const int serialNo
                         ) :
              judgeCheckupColumns (formatFN, parent, jCD,
                                   pDateForMonthColumn, serialNo)
 {
  MakeColumns (jCD) ;
  defineMaxRows () ;
 }  // FirmJudgeCheckupColumns :: FirmJudgeCheckupColumns

// ============================ End of ==================================
// ===================== FirmJudgeCheckupColumns ========================

/////////////////////////////////////////////////////////////////////////

void TFirmJudgeCheckupThread::makeBookFormatFN()
 {
  scSprintf (sizeof (formatFN), formatFN, "text\\judgedi%s",
		 DEFAULT_FORMAT_FILE_EXTENSION) ;
 }

judgeCheckupData * TFirmJudgeCheckupThread::newDataItem() const
 {
  return new FirmJudgeCheckupData ;
 }

judgeCheckupOutFile * TFirmJudgeCheckupThread::newOutFile() const
 {
  return new FirmJudgeCheckupOutFile (wtdl->Parent, formatFN) ;
 }

judgeCheckupColumns * TFirmJudgeCheckupThread::newJudgeCheckupColumns (const char * const formatFN,
	const judgeCheckupData & jCD, const CDate * const pDateForMonthColumn, const int serialNo) const
 {
  const FirmJudgeCheckupData * pFJCD =
    dynamic_cast <const FirmJudgeCheckupData *> (& jCD) ;

  if (pFJCD)
   {
    return
	new FirmJudgeCheckupColumns (formatFN, wtdl->Parent, (* pFJCD),
					     pDateForMonthColumn, serialNo) ;
   }
  else
   {
    fatal ("FirmJudgeCheckup::newJudgeCheckupColumns: "
           "unexpected type of the jCD parameter") ;
    return NULL ;  // This will never be executed
   }
 }

void TFirmJudgeCheckupThread::IncrementValues_Judge(const char * const judgeUcn, unsigned int dataCiph)
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
      FirmJudgeCheckupData * FJCD_Ptr =
        dynamic_cast <FirmJudgeCheckupData *> ((* pArr) [i]) ;

      if (! FJCD_Ptr)
        continue ;
	if (strcmp (FJCD_Ptr -> judgeUCN, judgeUcn) == 0)
       {  // -- 1a --
        found = true ;
        FirmJCData & FJCD = FJCD_Ptr -> firmData ;

        switch (dataCiph)
         {  // -- 4 --
          case iC_Firm_Feceived :
            FJCD.L_Firm_Feceived += iV ;
            break ;
          case iC_Firm_FeceivedRejudge :  // 11.dec.2003
            FJCD.L_Firm_FeceivedRejudge += iV ;
            break ;
          case iC_Firm_Entered :
            FJCD.D_Firm_Entered += iV ;
            break ;
          case iC_Firm_RegistrationRefused :
            FJCD.D_Firm_RegistrationRefused += iV ;
            break ;
          case iC_Firm_ChangeRefused :
            FJCD.D_Firm_ChangeRefused += iV ;
            break ;
          case iC_Firm_Erased :
            FJCD.D_Firm_Erased += iV ;
            break ;
          case iC_Firm_CancelDecision :
            FJCD.D_Firm_CancelDecision += iV ;
            break ;
          case iC_Firm_Other_TradeRegister :  // 21.feb.2005
            FJCD.D_Other_TradeRegister += iV ;
            break ;
          case iC_Firm_Other_NoTradeRegister :  // 21.feb.2005
            FJCD.D_Other_NoTradeRegister += iV ;
            break ;
          case iC_Firm_Changed :
            FJCD.D_Firm_Changed += iV ;
            break ;
         }  // -- 4 --  switch
       }  // -- 1a --
     }  // -- 1 --  for
   }  // -- 0b --
 }

void TFirmJudgeCheckupThread::LawsDistribute(const TLawsuit & law)

 {
  const char * const judgeUcn = law.judge ;

  IncrementValues_Judge (judgeUcn, iC_Firm_Feceived) ;
  IncrementValues_Judge (law.rejudge, iC_Firm_FeceivedRejudge) ;
 }

void TFirmJudgeCheckupThread::CountLaws(const CDate begDate, const CDate endDate)
 {
  if (bMYEMYTCIU.types [0] =='\0')
    return ;
  char crit [256] ;
   {
    ostrstream oss (crit, sizeof (crit)) ;

    oss << "SELECT F_TYPE, F_YEAR, F_NO, F_KIND, F_JUDGE, "
           "F_REJUDGE, "  // 11.dec.2003 
           "F_DATE "
           "FROM T_LAWSUIT "
           "WHERE F_TYPE IN ("
            << stringToSQLCharSet (bMYEMYTCIU.types).getTheSet () <<
                           ") AND "
		     "F_DATE BETWEEN " << begDate.Value() << " AND "
                                   << endDate.Value() << " " ;
    if (bMYEMYTCIU.composition)
      oss <<
           " AND F_COMPOSITION = "
          << bMYEMYTCIU.composition ;
    oss << ";" << ends ;
   }
  if (criteria_finished_ok (crit, "FirmJudgeCheckup::CountLaws"))
   {
    TQuery q (crit) ;
    TLawsuit law ;

    clearGroupData (law) ;
    while (q.Read ())
     {
      law << q ;
      LawsDistribute (law) ;
     }
   }
 }

void TFirmJudgeCheckupThread::DecisionsDistribute (const TDecision & dec, const TSession & sess)
 {
  const char * const judgeUcn = sess.judge ;
  const char r = dec.result ;

  if (! r)
    return ;
  if (strchr (RESULT_FIRM_DECISION_CHANGES, r))
    if (strchr (RESULT_FIRM_REFUSED_CHANGES, r))
      IncrementValues_Judge (judgeUcn, iC_Firm_ChangeRefused) ;
    else
     {
      if (dec.result == RESULT_FIRM_DEFINITION_FOR_TR ||  // 21.feb.2005 - begin
         dec.result == RESULT_FIRM_DECREE_FOR_TR)
        IncrementValues_Judge (judgeUcn, iC_Firm_Other_TradeRegister) ;
      else
        IncrementValues_Judge (judgeUcn, iC_Firm_Changed) ;
     }
  else
    if (strchr (RESULT_FIRM_REGISTRATIONS, r))
      if (r == RESULT_REGISTRATION_DENIAL)
        IncrementValues_Judge (judgeUcn, iC_Firm_RegistrationRefused) ;
      else
        IncrementValues_Judge (judgeUcn, iC_Firm_Entered) ;
    else
      if (strchr (RESULT_FIRM_CANCELEDS, r))
        IncrementValues_Judge (judgeUcn, iC_Firm_CancelDecision) ;
      else
        if (r == RESULT_FIRM_ERASING)  // 26.apr.2001 condition fixed
          IncrementValues_Judge (judgeUcn, iC_Firm_Erased) ;
 }

// 20060322+ search in T_Session - firm lawsuits only ->
void TFirmJudgeCheckupThread::CountSession( const CDate begD, const CDate endD )
{
  if (bMYEMYTCIU.types [0] =='\0')
    return ;

  if ( !( strchr (bMYEMYTCIU.types, TYPE_FIRM_LAW) ))
    return ;

  const size_t crtSz = 1024 ;
  char * const crt = new char [crtSz] ;
  {
    crt[0] = '\0';
    ostrstream oss (crt, crtSz) ;

    oss << "SELECT S.F_NO, S.F_YEAR, S.F_TYPE, S.F_KIND, S.F_DATE, S.F_JUDGE, S.F_RESULT "
           "FROM  T_SESSION S, T_LAWSUIT L "
	     "WHERE "
               " S.F_TYPE IN ("
                  << stringToSQLCharSet (bMYEMYTCIU.types).getTheSet () <<
                              ") AND "
		   " S.F_DATE BETWEEN " << begD.Value() << " AND "
                                 << endD.Value() << " AND "

               "L.F_TYPE = S.F_TYPE AND "
               "L.F_YEAR = S.F_YEAR AND "
               "L.F_NO   = S.F_NO "
               ;

        if (bMYEMYTCIU.composition)
           oss << "AND L.F_COMPOSITION = " << bMYEMYTCIU.composition ;

        oss <<   " AND (L.F_FINISHED IS NULL OR "   // 20060426 AND
                 "S.F_DATE <= L.F_FINISHED"
                 ") " ;
        oss << ";" << ends ;
  }
  if (criteria_finished_ok (crt, "FirmJudgeCheckup::CountSessions"))
  {
    TQuery q (crt) ;
    TSession * pSess = new TSession ;

    clearGroupData (* pSess) ;
    while (q.Read ())
    {
          (* pSess) << q ;
          if (pSess -> result == RESULT_SESS_FIRM_JURIDICTION)
              IncrementValues_Judge (pSess -> judge, iC_Firm_Changed) ;
          // 20060427 --> only_for_test
//          if (pSess -> result == RESULT_FIRM_DEFINITION_FOR_TR ||
//              pSess -> result == RESULT_FIRM_DECREE_FOR_TR)
//              IncrementValues_Judge (pSess -> judge, iC_Firm_Other_TradeRegister) ;
          // 20060427 <-- only_for_test
    }
    delete pSess ;
  }
  delete [] crt ;
}  // 20060322+

void TFirmJudgeCheckupThread::CountDecisions(const CDate begD, const CDate endD)
{
  if (bMYEMYTCIU.types [0] =='\0')
    return ;
  const size_t crtSz = 1024 ;
  char * const crt = new char [crtSz] ;

  {
    ostrstream oss (crt, crtSz) ;

    oss << "SELECT D.F_TYPE, D.F_YEAR, D.F_NO, D.F_KIND, D.F_DATE, "
			"D.F_RESULT, D.F_ENTER_DATE, S.F_JUDGE," // S.F_RESULT, " // 20060426 don't add session rerult here
			"L.F_FINISHED "  // who knows, sometimes they want
			   // to count the events after the finish, too
	     "FROM T_DECISION D, T_SESSION S, T_LAWSUIT L "
           "WHERE "
             "D.F_TYPE IN ("
                 << stringToSQLCharSet (bMYEMYTCIU.types).getTheSet () <<
                         ") AND "
		 "D.F_ENTER_DATE BETWEEN " << begD.Value() << " AND "
                                       << endD.Value() << " AND "

             "S.F_TYPE = D.F_TYPE AND "
             "S.F_YEAR = D.F_YEAR AND "
             "S.F_NO = D.F_NO AND "
             "S.F_DATE = D.F_DATE AND "
             "S.F_KIND = D.F_SESSION_KIND AND "

             "L.F_TYPE = D.F_TYPE AND "
             "L.F_YEAR = D.F_YEAR AND "
             "L.F_NO = D.F_NO AND " ;

    if (bMYEMYTCIU.composition)
      oss << "L.F_COMPOSITION = " << bMYEMYTCIU.composition << " AND " ;

    oss <<   "(L.F_FINISHED IS NULL OR "
              "D.F_DATE <= L.F_FINISHED"
             ") " ;
    oss << ";" << ends ;
  }

  if (criteria_finished_ok (crt, "FirmJudgeCheckup::CountDecisions"))
  {
    TQuery q (crt) ;
    TDecision * pDec = new TDecision ;
    TSession * pSess = new TSession ;

    clearGroupData (* pDec) ;
    clearGroupData (* pSess) ;
    while (q.Read ())
    {
      (* pDec) << q ;
      (* pSess) << q ;

      pSess -> kind = pSess -> result = '\0' ;
      DecisionsDistribute (* pDec, * pSess) ;
    }
    delete pSess ;
    delete pDec ;
  }

  delete [] crt ;

}

long int TFirmJudgeCheckupThread::CountFinishedFirmLaws (const CDate begD, const CDate endD, const long int composition)
 {
  if (begD.Empty () || endD.Empty ())
    return 0 ;

  long int amount = 0 ;
  const size_t crtSz = 1024 ;
  char * crt = new char [crtSz] ;

   {
    ostrstream oss (crt, crtSz) ;
    oss << "SELECT L.F_TYPE, L.F_YEAR, L.F_NO, L.F_DATE "
           "FROM T_LAWSUIT L WHERE "
           "L.F_TYPE = \'" << TYPE_FIRM_LAW <<  "\' AND "
           "(L.F_FINISHED IS NULL OR L.F_FINISHED >= " << begD.Value() << 
           ") AND " ;
    if (composition)
      oss <<
           "L.F_COMPOSITION = " << composition  << " AND " ;
    oss << "((L.F_FINISHED BETWEEN " << begD.Value() << " AND "
                                     << endD.Value() << 
            ")"
             " OR "
            "((L.F_FINISHED IS NULL OR L.F_FINISHED >= " << endD.Value() << 
             ") AND "
             "((SELECT COUNT (D.F_NO) FROM T_DECISION D WHERE "
              "D.F_TYPE = L.F_TYPE AND "
              "D.F_YEAR = L.F_YEAR AND "
              "D.F_NO = L.F_NO AND "
              "D.F_RESULT IN ("  // 26.apr.2001 - begin (bugfix)
                 << SQLCharSet (RESULT_FIRM_REGISTRATIONS)
		     << ") AND "  // 26.apr.2001 - end
		  "D.F_ENTER_DATE BETWEEN " << begD.Value() << " AND "
                                          << endD.Value() << 
              ") != 0"
             ")"
            ")"
           ")" ;
    oss << ";" << ends ;
   }
  if (criteria_finished_ok (crt,
                            "FirmJudgeCheckup::CountFinishedFirmLaws"))
   {
    TQuery q (crt) ;

    while (q.Read ())
      amount ++ ;
   }
  delete [] crt ;
  return amount ;
 }

long int TFirmJudgeCheckupThread::CountNonFinishedFirmLaws (const CDate rangeDate, const long int composition)
 {
  if (rangeDate.Empty ())
    return 0 ;
    
  long int amount = 0 ;
  const size_t crtSz = 1024 * 2 ;
  char * crt = new char [crtSz] ;

   {
    ostrstream oss (crt, crtSz) ;

    oss << "SELECT F_TYPE, F_YEAR, F_NO, F_DATE "
           "FROM T_LAWSUIT WHERE "
           "F_TYPE = \'" << TYPE_FIRM_LAW << "\'"
           " AND F_DATE <= " << rangeDate.Value() << " " ;  // 26.apr.2001
    if (composition)
      oss <<
           " AND F_COMPOSITION = " << composition ;
    oss << ends ;
   }
  crt [crtSz - 1] = '\0' ;

  const size_t l1 = strlen (crt) ;

  if (l1 + 2 < crtSz)
   {
    const CDate nextDate = nextDay (rangeDate) ;

    non_finished_firm_law_patch (crt + l1, crtSz - l1, KIND_FIRM_LAWS,
                                 & nextDate) ;
    strConcat (crt, ";", crtSz) ;  // Especially is here (in this block) -
    // to force an error message if this block is skipped.  
   }
  if (criteria_finished_ok (crt,
                            "FirmJudgeCheckup::CountNonFinishedFirmLaws"))
   {
    TQuery q (crt) ;

    while (q.Read ())
      amount ++ ;
   }
  delete [] crt ;
  return amount ;
 }

long int TFirmJudgeCheckupThread::CountThisCheckupContextFinisheds()
 {
  long int heap = 0 ;

  for (int i = 0 ; i < pArr -> Count () ; i ++)
   {
    FirmJudgeCheckupData * FJCD_Ptr =
      dynamic_cast <FirmJudgeCheckupData *> ((* pArr) [i]) ;

    if (! FJCD_Ptr)
      continue ;

    const FirmJCData & fD = FJCD_Ptr -> firmData ;

    heap += fD.D_Firm_Entered ;
    heap += fD.D_Firm_RegistrationRefused ;
    heap += fD.D_Firm_ChangeRefused ;
    heap += fD.D_Firm_Erased ;
    heap += fD.D_Firm_CancelDecision ;
    heap += fD.D_Other_TradeRegister ;  // 21.feb.2005
    heap += fD.D_Other_NoTradeRegister ;  // 21.feb.2005
    heap += fD.D_Firm_Changed ;
   }
  return heap ;
 }

void TFirmJudgeCheckupThread::MakeArray()
 {
  const CDate begD = bMYEMYTCIU.BegDate () ;
  const CDate endD = bMYEMYTCIU.EndDate () ;

  if (begD.Empty () || endD.Empty ())
    return ;
   {
    TWaitWindow ww (wtdl->Parent, PLEASE_WAIT_PROCESS_NEW_LAWS) ;

    CountLaws (begD, endD) ;
   }
   {
    TWaitWindow ww (wtdl->Parent, PLEASE_WAIT_PROCESS_DECISIONS) ;

    CountDecisions (begD, endD) ;
    CountSession   (begD, endD) ;  // 20060322+
   }

   // Some of data, that CountLaws and CountDecisions
   // have collect can be extracted from the array and
   // copied into the arrayOfLonf of bMYEMYTCIU and
   // printed in the formatted tail of the document.

  if (bMYEMYTCIU.lArrPtr () -> Count () == 0)
   {
    begMYEndMYTypeCIU * const pBMYEMYCIU =
	const_cast <begMYEndMYTypeCIU *> (& bMYEMYTCIU) ;

    if (pBMYEMYCIU)
     {
       {
	  TWaitWindow ww (wtdl->Parent, PLEASE_WAIT_PROCESS_FINISHEDS) ;

	  pBMYEMYCIU -> addToArr (CountFinishedFirmLaws (begD, endD,
							    bMYEMYTCIU.composition)) ;
	  pBMYEMYCIU -> addToArr (CountThisCheckupContextFinisheds ()) ;
	 }
	 {
        TWaitWindow ww (wtdl->Parent, PLEASE_WAIT_PROCESS_NON_FINISHEDS) ;

        pBMYEMYCIU -> addToArr (CountNonFinishedFirmLaws (endD,
                                              bMYEMYTCIU.composition)) ;
       }
     }
   }
 }

void TFirmJudgeCheckupThread::prepare()
 {
  if (! pArr)
    return ;

  sayAlmostDone = false;
  TJudgeCheckupThread :: prepare () ;
  MakeArray () ;
 }

TFirmJudgeCheckupThread::TFirmJudgeCheckupThread(const begMYEndMYTypeCIU & rBMYEMYTCIU) :
				TJudgeCheckupThread(rBMYEMYTCIU)
 {
 }

TFirmJudgeCheckupThread::~TFirmJudgeCheckupThread()
 {
 }

#endif  // AREAL
