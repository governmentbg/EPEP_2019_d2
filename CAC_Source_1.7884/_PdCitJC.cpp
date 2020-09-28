#include "cac.h"  // 18.may.2001
//# include "all.h"

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
// ========================== PdCitJCData ===============================
// ============================= Begin ==================================
size_t PdCitJCData :: Size () const
 {
  const char * b = (const char *) (& L_PdCitForExamineTotal1) ;
  const char * e = (const char *) (& S_PdCitUWA_Over1mPrvCompl2) +
                   sizeof (S_PdCitUWA_Over1mPrvCompl2) ;
  return (e - b) ;
 }  // PdCitJCData :: Size

void PdCitJCData :: ClearData ()
 {
  setmem (& L_PdCitForExamineTotal1, Size (), '\0') ;
 }  // PdCitJCData :: ClearData

bool PdCitJCData ::
operator == (const PdCitJCData & otherPdCitJCD) const
 {
  return ! memcmp (& L_PdCitForExamineTotal1,
                   & (otherPdCitJCD.L_PdCitForExamineTotal1), Size ()) ;
 }  // PdCitJCData :: operator ==

bool PdCitJCData :: IsEmpty () const
 {
  PdCitJCData rL1 ;

  return ((* this) == rL1) ;
 }  // PdCitJCData :: IsEmpty

PdCitJCData & PdCitJCData ::
  operator += (const PdCitJCData & otherPCJCD)
 {
  L_PdCitForExamineTotal1 += otherPCJCD.L_PdCitForExamineTotal1 ;
  L_PdCitForExamineTotal2 += otherPCJCD.L_PdCitForExamineTotal2 ;
  S_PdCitExamined1 += otherPCJCD.S_PdCitExamined1 ;
  S_PdCitExamined2 += otherPCJCD.S_PdCitExamined2 ;
  L_PdCitFinishComlp2 += otherPCJCD.L_PdCitFinishComlp2 ;
  L_PdCitFinishPrvComlp2 += otherPCJCD.L_PdCitFinishPrvComlp2 ;
  L_PdCitFinishedsTotal += otherPCJCD.L_PdCitFinishedsTotal ;
  L_PdCitDecidedsComlp2 += otherPCJCD.L_PdCitDecidedsComlp2 ;
  L_PdCitDecidedsPrvComlp2 += otherPCJCD.L_PdCitDecidedsPrvComlp2 ;
  L_PdCitDecidedsTotal += otherPCJCD.L_PdCitDecidedsTotal ;
  L_PdCitCanceledsComlp2 += otherPCJCD.L_PdCitCanceledsComlp2 ;
  L_PdCitCanceledsPrvComlp2 += otherPCJCD.L_PdCitCanceledsPrvComlp2 ;
  L_PdCitCanceledsTotal += otherPCJCD.L_PdCitCanceledsTotal ;
  S_PdCitUWA_UpTo1m1 += otherPCJCD.S_PdCitUWA_UpTo1m1 ;
  S_PdCitUWA_UpTo1mCompl2 += otherPCJCD.S_PdCitUWA_UpTo1mCompl2 ;
  S_PdCitUWA_UpTo1mPrvCompl2 += otherPCJCD.S_PdCitUWA_UpTo1mPrvCompl2 ;
  S_PdCitUWA_Over1m1 += otherPCJCD.S_PdCitUWA_Over1m1 ;
  S_PdCitUWA_Over1mCompl2 += otherPCJCD.S_PdCitUWA_Over1mCompl2 ;
  S_PdCitUWA_Over1mPrvCompl2 += otherPCJCD.S_PdCitUWA_Over1mPrvCompl2 ;
  S_PdCitUWA_UpTo1mTotal += otherPCJCD.S_PdCitUWA_UpTo1mTotal ;
  S_PdCitUWA_Over1mTotal += otherPCJCD.S_PdCitUWA_Over1mTotal ;
  L_PdCitFinishUpTo3MComlp2 += otherPCJCD.L_PdCitFinishUpTo3MComlp2 ;
  L_PdCitFinishUpTo3MPrvComlp2 += otherPCJCD.L_PdCitFinishUpTo3MPrvComlp2;
  L_PdCitFinishUpTo3MTotal += otherPCJCD.L_PdCitFinishUpTo3MTotal ;
  L_PdCitFinishOver3MComlp2 += otherPCJCD.L_PdCitFinishOver3MComlp2 ;
  L_PdCitFinishOver3MPrvComlp2 += otherPCJCD.L_PdCitFinishOver3MPrvComlp2;
  L_PdCitFinishOver3MTotal += otherPCJCD.L_PdCitFinishOver3MTotal ;
  return (* this) ;
 }  // PdCitJCData & PdCitJCData :: operator +=

// ============================ End of ==================================
// ========================== PdCitJCData ===============================

/////////////////////////////////////////////////////////////////////////

// ===================== PdCitJudgeCheckupData ==========================
// ============================= Begin ==================================
void PdCitJudgeCheckupData :: clearData ()
 {
  judgeCheckupData :: clearData () ;
  PdCitData.ClearData () ;
 }  // PdCitJudgeCheckupData :: clearData

bool PdCitJudgeCheckupData :: countingDataEmpty () const
 {
  return (judgeCheckupData :: countingDataEmpty () &&
          PdCitData.IsEmpty ()
	   ) ;
 }  // PdCitJudgeCheckupData :: countingDataEmpty

void
PdCitJudgeCheckupData :: operator += (const judgeCheckupData & other)
 {
  judgeCheckupData :: operator += (other) ;

  const PdCitJudgeCheckupData * otherPdCit =
    dynamic_cast <const PdCitJudgeCheckupData *> (& other) ;

  if (otherPdCit)
   {
    PdCitData += otherPdCit -> PdCitData ;
   }
 }  // PdCitJudgeCheckupData :: operator

// ============================ End of ==================================
// ===================== PdCitJudgeCheckupData ==========================

/////////////////////////////////////////////////////////////////////////

// ==================== PdCitJudgeCheckupColumns ========================
// ============================= Begin ==================================
void PdCitJudgeCheckupColumns ::
       MakeColumns (const PdCitJudgeCheckupData & jCD)
 {
  const PdCitJCData & PCJCD = jCD.PdCitData ;

  addNmbToClmn (columns [iC_L_PdCitForExamineTotal1],
                PCJCD.L_PdCitForExamineTotal1) ;
  addNmbToClmn (columns [iC_L_PdCitForExamineTotal2],
                PCJCD.L_PdCitForExamineTotal2) ;
  addNmbToClmn (columns [iC_S_PdCitExamined1],
                PCJCD.S_PdCitExamined1) ;
  addNmbToClmn (columns [iC_S_PdCitExamined2],
                PCJCD.S_PdCitExamined2) ;
  addNmbToClmn (columns [iC_L_PdCitFinishComlp2],
                PCJCD.L_PdCitFinishComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitFinishPrvComlp2],
                PCJCD.L_PdCitFinishPrvComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitFinishedsTotal],
                PCJCD.L_PdCitFinishedsTotal) ;
  addNmbToClmn (columns [iC_L_PdCitDecidedsComlp2],
                PCJCD.L_PdCitDecidedsComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitDecidedsPrvComlp2],
                PCJCD.L_PdCitDecidedsPrvComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitDecidedsTotal],
                PCJCD.L_PdCitDecidedsTotal) ;
  addNmbToClmn (columns [iC_L_PdCitCanceledsComlp2],
                PCJCD.L_PdCitCanceledsComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitCanceledsPrvComlp2],
                PCJCD.L_PdCitCanceledsPrvComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitCanceledsTotal],
                PCJCD.L_PdCitCanceledsTotal) ;
  addNmbToClmn (columns [iC_S_PdCitUWA_UpTo1m1],
                PCJCD.S_PdCitUWA_UpTo1m1) ;
  addNmbToClmn (columns [iC_S_PdCitUWA_UpTo1mCompl2],
                PCJCD.S_PdCitUWA_UpTo1mCompl2) ;
  addNmbToClmn (columns [iC_S_PdCitUWA_UpTo1mPrvCompl2],
                PCJCD.S_PdCitUWA_UpTo1mPrvCompl2) ;
  addNmbToClmn (columns [iC_S_PdCitUWA_Over1m1],
                PCJCD.S_PdCitUWA_Over1m1) ;
  addNmbToClmn (columns [iC_S_PdCitUWA_Over1mCompl2],
                PCJCD.S_PdCitUWA_Over1mCompl2) ;
  addNmbToClmn (columns [iC_S_PdCitUWA_Over1mPrvCompl2],
                PCJCD.S_PdCitUWA_Over1mPrvCompl2) ;
  addNmbToClmn (columns [iC_S_PdCitUWA_UpTo1mTotal],
                PCJCD.S_PdCitUWA_UpTo1mTotal) ;
  addNmbToClmn (columns [iC_S_PdCitUWA_Over1mTotal],
                PCJCD.S_PdCitUWA_Over1mTotal) ;
  addNmbToClmn (columns [iC_L_PdCitFinishUpTo3MComlp2],
                PCJCD.L_PdCitFinishUpTo3MComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitFinishUpTo3MPrvComlp2],
                PCJCD.L_PdCitFinishUpTo3MPrvComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitFinishUpTo3MTotal],
                PCJCD.L_PdCitFinishUpTo3MTotal) ;
  addNmbToClmn (columns [iC_L_PdCitFinishOver3MComlp2],
                PCJCD.L_PdCitFinishOver3MComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitFinishOver3MPrvComlp2],
                PCJCD.L_PdCitFinishOver3MPrvComlp2) ;
  addNmbToClmn (columns [iC_L_PdCitFinishOver3MTotal],
                PCJCD.L_PdCitFinishOver3MTotal) ;
 }  // PdCitJudgeCheckupColumns :: MakeColumns

PdCitJudgeCheckupColumns ::
PdCitJudgeCheckupColumns (const char * const formatFN,
                          TWindow * parent,
                          const PdCitJudgeCheckupData & jCD,
                          const CDate * const pDateForMonthColumn,
                          const int serialNo
                         ) :
              judgeCheckupColumns (formatFN, parent, jCD,
                                   pDateForMonthColumn, serialNo)
 {
  MakeColumns (jCD) ;
  defineMaxRows () ;
 }  // PdCitJudgeCheckupColumns :: PdCitJudgeCheckupColumns

// ============================ End of ==================================
// ==================== PdCitJudgeCheckupColumns ========================

/////////////////////////////////////////////////////////////////////////

void TPdCitJudgeCheckupThread::makeBookFormatFN()
 {
  scSprintf (sizeof (formatFN), formatFN, "text\\ju%03dci%s",
		 whichPart, DEFAULT_FORMAT_FILE_EXTENSION) ;
 }

judgeCheckupData * TPdCitJudgeCheckupThread::newDataItem() const
 {
  return new PdCitJudgeCheckupData ;
 }

judgeCheckupOutFile * TPdCitJudgeCheckupThread::newOutFile() const
 {
  return new PdCitJudgeCheckupOutFile(wtdl->Parent, formatFN);
 }

judgeCheckupColumns * TPdCitJudgeCheckupThread::newJudgeCheckupColumns(const char * const formatFN,
	const judgeCheckupData & jCD, const CDate * const pDateForMonthColumn, const int serialNo) const
 {
  const PdCitJudgeCheckupData * pPCJCD =
    dynamic_cast <const PdCitJudgeCheckupData *> (& jCD) ;

  if (pPCJCD)
   {
    return
      new PdCitJudgeCheckupColumns (formatFN, wtdl->Parent, (* pPCJCD),
                                    pDateForMonthColumn, serialNo) ;
   }
  else
   {
    fatal ("P????JudgeCheckup::newJudgeCheckupColumns: "
           "unexpected type of the jCD parameter") ;
    return NULL ;  // This will never be executed
   }
 }

void TPdCitJudgeCheckupThread::IncrementValues_Judge(const char * const judgeUcn, unsigned int dataCiph)
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
      PdCitJudgeCheckupData * PCJCD_Ptr =
        dynamic_cast <PdCitJudgeCheckupData *> ((* pArr) [i]) ;

      if (! PCJCD_Ptr)
        continue ;
      if (strcmp (PCJCD_Ptr -> judgeUCN, judgeUcn) == 0)
       {  // -- 1a --
        found = true ;
        PdCitJCData & PCJCD = PCJCD_Ptr -> PdCitData ;

        switch (dataCiph)
         {  // -- 4 --
          case iC_L_PdCitForExamineTotal1 :
            PCJCD.L_PdCitForExamineTotal1 += iV ;
            break ;
          case iC_L_PdCitForExamineTotal2 :
		PCJCD.L_PdCitForExamineTotal2 += iV ;
            break ;
          case iC_S_PdCitExamined1 :
            PCJCD.S_PdCitExamined1 += iV ;
            break ;
          case iC_S_PdCitExamined2 :
            PCJCD.S_PdCitExamined2 += iV ;
            break ;
          case iC_L_PdCitFinishComlp2 :
            PCJCD.L_PdCitFinishComlp2 += iV ;
            break ;
          case iC_L_PdCitFinishPrvComlp2 :
            PCJCD.L_PdCitFinishPrvComlp2 += iV ;
            break ;
          case iC_L_PdCitFinishedsTotal :
            PCJCD.L_PdCitFinishedsTotal += iV ;
            break ;
          case iC_L_PdCitDecidedsComlp2 :
            PCJCD.L_PdCitDecidedsComlp2 += iV ;
            break ;
          case iC_L_PdCitDecidedsPrvComlp2 :
            PCJCD.L_PdCitDecidedsPrvComlp2 += iV ;
            break ;
          case iC_L_PdCitDecidedsTotal :
            PCJCD.L_PdCitDecidedsTotal += iV ;
            break ;
          case iC_L_PdCitCanceledsComlp2 :
            PCJCD.L_PdCitCanceledsComlp2 += iV ;
            break ;
          case iC_L_PdCitCanceledsPrvComlp2 :
            PCJCD.L_PdCitCanceledsPrvComlp2 += iV ;
            break ;
          case iC_L_PdCitCanceledsTotal :
            PCJCD.L_PdCitCanceledsTotal += iV ;
            break ;
          case iC_S_PdCitUWA_UpTo1m1 :
            PCJCD.S_PdCitUWA_UpTo1m1 += iV ;
            break ;
          case iC_S_PdCitUWA_UpTo1mCompl2 :
            PCJCD.S_PdCitUWA_UpTo1mCompl2 += iV ;
            break ;
          case iC_S_PdCitUWA_UpTo1mPrvCompl2 :
            PCJCD.S_PdCitUWA_UpTo1mPrvCompl2 += iV ;
            break ;
          case iC_S_PdCitUWA_Over1m1 :
            PCJCD.S_PdCitUWA_Over1m1 += iV ;
		break ;
          case iC_S_PdCitUWA_Over1mCompl2 :
            PCJCD.S_PdCitUWA_Over1mCompl2 += iV ;
            break ;
          case iC_S_PdCitUWA_Over1mPrvCompl2 :
            PCJCD.S_PdCitUWA_Over1mPrvCompl2 += iV ;
            break ;
          case iC_S_PdCitUWA_UpTo1mTotal :
            PCJCD.S_PdCitUWA_UpTo1mTotal += iV ;
            break ;
          case iC_S_PdCitUWA_Over1mTotal :
            PCJCD.S_PdCitUWA_Over1mTotal += iV ;
            break ;
          case iC_L_PdCitFinishUpTo3MComlp2 :
            PCJCD.L_PdCitFinishUpTo3MComlp2 += iV ;
            break ;
          case iC_L_PdCitFinishUpTo3MPrvComlp2 :
            PCJCD.L_PdCitFinishUpTo3MPrvComlp2 += iV ;
            break ;
          case iC_L_PdCitFinishUpTo3MTotal :
            PCJCD.L_PdCitFinishUpTo3MTotal += iV ;
            break ;
          case iC_L_PdCitFinishOver3MComlp2 :
            PCJCD.L_PdCitFinishOver3MComlp2 += iV ;
            break ;
          case iC_L_PdCitFinishOver3MPrvComlp2 :
            PCJCD.L_PdCitFinishOver3MPrvComlp2 += iV ;
            break ;
          case iC_L_PdCitFinishOver3MTotal :
            PCJCD.L_PdCitFinishOver3MTotal += iV ;
            break ;
         }  // -- 4 --  switch
       }  // -- 1a --
     }  // -- 1 --  for
   }  // -- 0b --
 }

void TPdCitJudgeCheckupThread::FinishedLawsDistribute(const TLawsuit & law, const TSession & sess)
 {
  const char * const judgeUcn = sess.judge ;

  IncrementValues_Judge (judgeUcn, iC_L_PdCitFinishedsTotal) ;

  const bool exactComplaint =
    strchr (KIND_EXACT_COMPLAINTS, law.sourceKind)
    || strchr (KIND_SLOWDOWN_COMPLAINTS, law.sourceKind)  // 17.apr.2001
     ;
  const bool privateComplaint =
    strchr (KIND_PRIVATE_COMPLAINTS, law.sourceKind) ;

  if (exactComplaint)
    IncrementValues_Judge (judgeUcn, iC_L_PdCitFinishComlp2) ;
  else
    if (privateComplaint)
      IncrementValues_Judge (judgeUcn, iC_L_PdCitFinishPrvComlp2) ;

  if (strchr (AGE_UP_TO_THREES, law.age))
   {
    IncrementValues_Judge (judgeUcn, iC_L_PdCitFinishUpTo3MTotal) ;
    // 2008:299 LRQ/LPR: 2ND only
    if (strchr (KIND_2ND_LAWSUITS, law.kind))
    {  // -- 2 --
      if (exactComplaint)
	IncrementValues_Judge (judgeUcn, iC_L_PdCitFinishUpTo3MComlp2) ;
      else
	if (privateComplaint)
	  IncrementValues_Judge (judgeUcn,
				 iC_L_PdCitFinishUpTo3MPrvComlp2) ;
    }
   }
  else
    if (strchr (AGE_MORE_THAN_THREES_ALL, law.age))
     {
      IncrementValues_Judge (judgeUcn, iC_L_PdCitFinishOver3MTotal) ;
      // 2008:299 LRQ/LPR: 2ND only
      if (strchr (KIND_2ND_LAWSUITS, law.kind))
      {  // -- 2 --
	if (exactComplaint)
	  IncrementValues_Judge (judgeUcn, iC_L_PdCitFinishOver3MComlp2) ;
	else
	  if (privateComplaint)
	    IncrementValues_Judge (judgeUcn,
				   iC_L_PdCitFinishOver3MPrvComlp2) ;
      }
     }

  const bool decided = sessionDecidesTheLaw (sess, & law,
			  (! strchr (KIND_CLOSED_SESSIONS, sess.kind))) ;
  const bool canceled = sessionCancelsTheLaw (sess, & law,
			  (! strchr (KIND_CLOSED_SESSIONS, sess.kind))) ;

  if (decided == canceled)
   {
    unknown_finished_law_status (law.key) ;
    return ;
   }
  if (decided)
    IncrementValues_Judge (judgeUcn, iC_L_PdCitDecidedsTotal) ;
  else
    IncrementValues_Judge (judgeUcn, iC_L_PdCitCanceledsTotal) ;
  if (strchr (KIND_2ND_LAWSUITS, law.kind))
   {  // -- 2 --
    if (exactComplaint)
     {
      if (decided)
	IncrementValues_Judge (judgeUcn, iC_L_PdCitDecidedsComlp2) ;
      else
	IncrementValues_Judge (judgeUcn, iC_L_PdCitCanceledsComlp2) ;
     }
    else
      if (privateComplaint)
       {
	if (decided)
	  IncrementValues_Judge (judgeUcn, iC_L_PdCitDecidedsPrvComlp2) ;
	else
	  IncrementValues_Judge (judgeUcn, iC_L_PdCitCanceledsPrvComlp2) ;
       }
   }  // -- 2 --
 }

void TPdCitJudgeCheckupThread::CountFinishedLaws (const CDate begDate, const CDate endDate)
 {
  FinalizedSessions finS (bMYEMYTCIU.types, begDate, endDate) ;
   {
    TSession * pSess = new TSession ;
    TLawsuit law ;

    do
     {
      if (finS.NextSession (law, (* pSess)))
        if (bMYEMYTCIU.composition == 0 ||  // 08.feb.2001
            bMYEMYTCIU.composition == law.composition) // 08.feb.2001
          FinishedLawsDistribute (law, * pSess) ;
     }
    while (TRCDKey_ (law.key).FilledOK ()) ;
    delete pSess ;
   }
 }

void TPdCitJudgeCheckupThread::ForExamineDistribute(const TLawsuit & law)
 {
  const char * const judgeUcn = law.judge ;

  if (strchr (KIND_1ST_LAWSUITS, law.kind))
    IncrementValues_Judge (judgeUcn, iC_L_PdCitForExamineTotal1) ;
  else
    IncrementValues_Judge (judgeUcn, iC_L_PdCitForExamineTotal2) ;
 }

void TPdCitJudgeCheckupThread::CountForExamine (const CDate begD, const CDate endD)
 {
  const size_t crtSz = 512 ;
  char * const crt = new char [crtSz] ;
   {
    ostrstream oss (crt, crtSz) ;

    oss << "SELECT F_TYPE, F_YEAR, F_NO, F_KIND, F_JUDGE "
           "FROM T_LAWSUIT WHERE "
           "F_TYPE IN (" <<
             stringToSQLCharSet (bMYEMYTCIU.types).getTheSet ()
                         << ") AND "
	     "F_DATE <= " << endD.Value() << " AND "
           "(F_FINISHED IS NULL OR F_FINISHED >= " << begD.Value() << ") " ;
    if (bMYEMYTCIU.composition)  // 16.feb.2001 - begin
      oss <<
           " AND F_COMPOSITION = "
          << bMYEMYTCIU.composition ;// 16.feb.2001 - end
    oss << ";" << ends ;
   }
  crt [crtSz - 1] = '\0' ;
  if (criteria_finished_ok (crt, "P????JudgeCheckup::CountForExamine"))
   {
    TLawsuit law ;
    TQuery q (crt) ;

    while (q.Read ())
     {
      law << q ;
      ForExamineDistribute (law) ;
     }
   }
  delete [] crt ;
 }

void TPdCitJudgeCheckupThread::SessionsDistribute (const TLawsuit & law, const TSession & sess)
 {
  if (strchr (KIND_CLOSED_SESSIONS, sess.kind))
    return ;
  // 2008:299 LRQ
  if (sess.kind == KIND_OTHER_SESSION)
    return;

  const char * const judgeUcn = sess.judge ;

  if (strchr (KIND_1ST_LAWSUITS, law.kind))
   {
    IncrementValues_Judge (judgeUcn, iC_S_PdCitExamined1) ;
   }
  else
   {
    IncrementValues_Judge (judgeUcn, iC_S_PdCitExamined2) ;
   }
 }

void TPdCitJudgeCheckupThread::CountSessions(const CDate begD, const CDate endD)
 {
  const size_t crtSz = 1024 * 2 ;
  char * const crt = new char [crtSz] ;
  const char * const what =
    "L.F_KIND, L.F_SUBJECT, L.F_SOURCE_KIND, "
    "S.F_KIND AS SESS_KIND, "
    "S.F_NO, S.F_YEAR, S.F_TYPE, "
    "S.F_JUDGE, S.F_RESULT " ;
  char sessResToExclude [16] ;

  scSprintf (sizeof (sessResToExclude), sessResToExclude,
             "%c", RESULT_FIXED) ;
  if (composeJudgeCheckupCriteria (crt, crtSz, what, bMYEMYTCIU.types,
				   bMYEMYTCIU.composition, & endD,
                                   & begD, sessResToExclude, NULL,
                                   NULL, false, NULL, NULL, NULL, false,
                                   false, false, true))
   {  // -- 0 --
    TQuery q (crt) ;
    TSession * pSess = new TSession ;
    TLawsuit law ;
    char sessKind ;
    TDummyGroup sKGroup;

    sKGroup.Add(new TChar ("SESS_KIND", & sessKind)) ;
    while (q.Read ())
     {  // -- 1 --
      (* pSess) << q ;
      law << q ;
      sKGroup << q ;
      pSess -> kind = sessKind ;
      law.key = pSess -> key ;
      SessionsDistribute (law, * pSess) ;
     }  // -- 1 --
    delete pSess ;
   }  // -- 0 --
  delete [] crt ;
 }

void TPdCitJudgeCheckupThread::UnwrittensDistribute (const TLawsuit & law, const TSession & sess, const CDate endDate)
 {
  const char * const judgeUcn = sess.judge ;
  const bool upTo1 =
    differenceInMonths ((strchr (KIND_CLOSED_SESSIONS, sess.kind) ?
                         sess.returned : sess.date),
                        endDate) == 0 ;

  if (upTo1)
    IncrementValues_Judge (judgeUcn, iC_S_PdCitUWA_UpTo1mTotal) ;
  else
    IncrementValues_Judge (judgeUcn, iC_S_PdCitUWA_Over1mTotal) ;
  if (strchr (KIND_1ST_LAWSUITS, law.kind))
   {  // -- 0 --
    if (upTo1)
      IncrementValues_Judge (judgeUcn, iC_S_PdCitUWA_UpTo1m1) ;
    else
      IncrementValues_Judge (judgeUcn, iC_S_PdCitUWA_Over1m1) ;
   }  // -- 0 --
  else
   {  // -- 1 --
    if (strchr (KIND_EXACT_COMPLAINTS, law.sourceKind)
	  || strchr (KIND_SLOWDOWN_COMPLAINTS, law.sourceKind) //17.apr.2001
	 )
	if (upTo1)
	  IncrementValues_Judge (judgeUcn, iC_S_PdCitUWA_UpTo1mCompl2) ;
	else
	  IncrementValues_Judge (judgeUcn, iC_S_PdCitUWA_Over1mCompl2) ;
    else
	if (strchr (KIND_PRIVATE_COMPLAINTS, law.sourceKind))
	  if (upTo1)
	    IncrementValues_Judge (judgeUcn, iC_S_PdCitUWA_UpTo1mPrvCompl2);
	  else
	    IncrementValues_Judge (judgeUcn, iC_S_PdCitUWA_Over1mPrvCompl2);
   }  // -- 1 --
 }

#   if UNWRITTEN_ACTS_CALL_JIMMY  // 08.feb.2000 - begin
void TPdCitJudgeCheckupThread::CountUnwrittens (const CDate, const CDate endDate)
 {
  TLawsuit law ;
  TSession * pSess = new TSession ;
  SessionsToCheck sessions (bMYEMYTCIU.types, DATE_MIN, endDate, 0) ;
    // Especially I do not use bMYEMYTCIU.composition
    // Later I shall use law.composition

  clearGroupData (law) ;
  while (sessions.NextSession (* pSess))
   {
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
 }
#   else  // 0f UNWRITTEN_ACTS_CALL_JIMMY  // 08.feb.2000 - end
void CDate TPdCitJudgeCheckupThread::CountUnwrittens (const CDate begDate, const CDate endDate)
 {
  const size_t tmpResSz = strlen (RESULT_ENTERABLES) + 1 ;
  char * const tmpRes = new char [tmpResSz] ;

  scSprintf (tmpResSz, tmpRes, "%s", RESULT_ENTERABLES) ;
  intersection (tmpRes, RESULT_FINALIZEDS) ;
  if (tmpRes [0])
   {  // -- 0 --
    FinalizedSessions finSU (bMYEMYTCIU.types, begDate, endDate) ;
     {  // -- 1 --
      TSession * pSess = new TSession ;
      TLawsuit law ;

      do
       {  // -- 2 --
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

void TPdCitJudgeCheckupThread::MakeArray (const bool includingUnwrittens)
 {
  const CDate begD = bMYEMYTCIU.BegDate () ;
  const CDate endD = bMYEMYTCIU.EndDate () ;

#if 20150266
    TWaitWindow ww(wtdl->Parent, PLEASE_WAIT_PROCESS_FINISHEDS);
    CountFinishedLaws(begD, endD);

    ww.SetText(PLEASE_WAIT_PROCESS_FOR_EXAM);
    CountForExamine(begD, endD);

    ww.SetText(PLEASE_WAIT_PROCESS_SESSIONS);
    CountSessions(begD, endD);

  if (includingUnwrittens)
   {
    ww.SetText(PLEASE_WAIT_LOOKING_UNWRITTENS);
    CountUnwrittens(DATE_MIN, endD);
   }
#else  // 20150266
   {
    TWaitWindow ww (wtdl->Parent, PLEASE_WAIT_PROCESS_FINISHEDS) ;
    CountFinishedLaws (begD, endD) ;
   }
   {
    TWaitWindow ww (wtdl->Parent, PLEASE_WAIT_PROCESS_FOR_EXAM) ;
    CountForExamine (begD, endD) ;
   }
   {
    TWaitWindow ww (wtdl->Parent, PLEASE_WAIT_PROCESS_SESSIONS) ;
    CountSessions (begD, endD) ;
   }
  if (includingUnwrittens)
   {
    TWaitWindow ww (wtdl->Parent, PLEASE_WAIT_LOOKING_UNWRITTENS) ;
    CountUnwrittens (DATE_MIN, endD) ;
   }
#endif  // 20150266
 }

void TPdCitJudgeCheckupThread::prepare()
 {
  if (! pArr)
    return ;

  const bool includingUnwrittens = confirm_unwritten_acts_search () ;

  sayAlmostDone = false;  // 28.may.2002
  TJudgeCheckupThread :: prepare () ;
  whichPart = 2 ;
  TJudgeCheckupThread :: prepare () ;
  MakeArray (includingUnwrittens) ;
 }

TPdCitJudgeCheckupThread::TPdCitJudgeCheckupThread(const begMYEndMYTypeCIU & rBMYEMYTCIU) :
				TJudgeCheckupThread(rBMYEMYTCIU),
				whichPart (1)
 {
 }

TPdCitJudgeCheckupThread::~TPdCitJudgeCheckupThread()
 {
 }

void TPdCitJudgeCheckupThread::printJudgeCheckup()
 {
  whichPart = 1 ;
  makeBookFormatFN () ;
  TJudgeCheckupThread :: printJudgeCheckup () ;
  whichPart = 2 ;
  makeBookFormatFN () ;
  TJudgeCheckupThread :: printJudgeCheckup () ;
 }

#   endif  // of APPEAL
# endif  // of INSTANCE
