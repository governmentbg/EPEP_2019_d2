#include "cac.h"

// 2008:298 LPR: all FINALIZEDS -> COMPLETIZEDS

judgeCheckupEnum :: judgeCheckupEnum ()
 {
 }  // judgeCheckupEnum :: judgeCheckupEnum

size_t judgeCheckupLawsuits :: size () const
 {
  const char * b = (const char *) (& notFinishedsBeg1) ;
  const char * e = (const char *) (& notFinishedsEnd1) +
                   sizeof (notFinishedsEnd1) ;
  return (e - b) ;
 }  // judgeCheckupLawsuits :: size

void judgeCheckupLawsuits :: clearData ()
 {
  setmem (& notFinishedsBeg1, size (), '\0') ;
 }  // judgeCheckupLawsuits :: clearData

bool judgeCheckupLawsuits ::
  operator == (const judgeCheckupLawsuits & otherJCL) const
 {
  return ! memcmp (& notFinishedsBeg1, & (otherJCL.notFinishedsBeg1),
                   size ()) ;
 }  // judgeCheckupLawsuits :: operator ==

bool judgeCheckupLawsuits :: isEmpty () const
 {
  judgeCheckupLawsuits rL1 ;

  return ((* this) == rL1) ;
 }  // judgeCheckupLawsuits :: isEmpty

judgeCheckupLawsuits & judgeCheckupLawsuits ::
  operator += (const judgeCheckupLawsuits & otherJCL)
 {
  notFinishedsBeg1   += otherJCL.notFinishedsBeg1 ;
	news1 += otherJCL.news1 ;
	newsRejudge1 += otherJCL.newsRejudge1 ;  // 11.dec.2003
	newsPunGenChar1 += otherJCL.newsPunGenChar1 ;  // 10.jul.2002
	newsPunGenCharRejudge1 += otherJCL.newsPunGenCharRejudge1 ;  // 11.dec.2003
  finishedsTotal1 += otherJCL.finishedsTotal1 ;
  finishedsWithDecision1 += otherJCL.finishedsWithDecision1 ;
  finishedsCanceleds1 += otherJCL.finishedsCanceleds1 ;
  finishedsUpTo3M1 += otherJCL.finishedsUpTo3M1 ;
  finishedsOver3M1 += otherJCL.finishedsOver3M1 ;
  notFinishedsEnd1 += otherJCL.notFinishedsEnd1 ;
  abolisheds1 += otherJCL.abolisheds1 ;
  finishedsCanceledsByAgr1 += otherJCL.finishedsCanceledsByAgr1 ;
  finishedsCanceledsOtherC1 += otherJCL.finishedsCanceledsOtherC1 ;
  keepingMethodConfirmed1 += otherJCL.keepingMethodConfirmed1 ;
  keepingMethodChanged1 += otherJCL.keepingMethodChanged1 ;
  subject0210 += otherJCL.subject0210 ;
  subject0600 += otherJCL.subject0600 ;
  subject0500 += otherJCL.subject0500 ;
  subject0810 += otherJCL.subject0810 ;
  subjects0400_0410 += otherJCL.subjects0400_0410 ;
  subjects0220_0310_0800 += otherJCL.subjects0220_0310_0800 ;
  decisDefinClosedS1 += otherJCL.decisDefinClosedS1 ;
  canceledsClosedS1 += otherJCL.canceledsClosedS1 ;
# if INSTANCE
	notFinishedsBeg2 += otherJCL.notFinishedsBeg2 ;
	news2 += otherJCL.news2 ;
	newsRejudge2 += otherJCL.newsRejudge2 ;  // 11.dec.2003
	newsPunGenChar2 += otherJCL.newsPunGenChar2 ;  // 10.jul.2002
	newsPunGenCharRejudge2 += otherJCL.newsPunGenCharRejudge2 ;  // 11.dec.2003
  finishedsTotal2 += otherJCL.finishedsTotal2 ;
  finishedsWithDecision2 += otherJCL.finishedsWithDecision2 ;
  finishedsCanceleds2 += otherJCL.finishedsCanceleds2 ;
  finishedsUpTo3M2 += otherJCL.finishedsUpTo3M2 ;
  finishedsOver3M2 += otherJCL.finishedsOver3M2 ;
  notFinishedsEnd2 += otherJCL.notFinishedsEnd2 ;
  finishedsCanceledsByAgr2 += otherJCL.finishedsCanceledsByAgr2 ;
  finishedsCanceledsOtherC2 += otherJCL.finishedsCanceledsOtherC2 ;
  exactComplaints2 += otherJCL.exactComplaints2 ;
  privateComplaints2 += otherJCL.privateComplaints2 ;
  decisDefinClosedS2 += otherJCL.decisDefinClosedS2 ;
  canceledsClosedS2 += otherJCL.canceledsClosedS2 ;
#   if APPEAL
  newsPGenCharAplCourt += otherJCL.newsPGenCharAplCourt ;
  newsPKeepOpenSessAplCourt += otherJCL.newsPKeepOpenSessAplCourt ;
  newsPKeepClSessAplCourt += otherJCL.newsPKeepClSessAplCourt ;
  newsPEnactmentAplCourt += otherJCL.newsPEnactmentAplCourt ;
  newsPOthersAplCourt += otherJCL.newsPOthersAplCourt ;
  decPGenCharAplCourt += otherJCL.decPGenCharAplCourt ;
  decPKeepOpenSessAplCourt += otherJCL.decPKeepOpenSessAplCourt ;
  decPKeepClSessAplCourt += otherJCL.decPKeepClSessAplCourt ;
  decPEnactmentAplCourt += otherJCL.decPEnactmentAplCourt ;
  decPOthersAplCourt += otherJCL.decPOthersAplCourt ;
#   endif  // of APPEAL
// 05.apr.2002 - begin
  detailNotFinishedBegTotal2 += otherJCL.detailNotFinishedBegTotal2 ;
  detailNotFinBegPunGen2 += otherJCL.detailNotFinBegPunGen2 ;
  detailNotFinBegPunPrv2 += otherJCL.detailNotFinBegPunPrv2 ;
  detailNotFinishedEndTotal2 += otherJCL.detailNotFinishedEndTotal2 ;
  detailNotFinEndPunGen2 += otherJCL.detailNotFinEndPunGen2 ;
  detailNotFinEndPunPrv2 += otherJCL.detailNotFinEndPunPrv2 ;
// 05.apr.2002 - end
# endif
// 05.apr.2002 - begin
  detailNotFinishedBegTotal1 += otherJCL.detailNotFinishedBegTotal1 ;
  detailNotFinishedEndTotal1 += otherJCL.detailNotFinishedEndTotal1 ;
// 05.apr.2002 - end

// 20061207 -->
	newsRejudge1_J += otherJCL.newsRejudge1_J ; // Постъпили дела - Новообразувани
  byJurisd_J     += otherJCL.byJurisd_J;      // Постъпили дела - Получени по подсъдност
  returneds_J    += otherJCL.returneds_J;     // Постъпили дела - Върнати за ново разглеждане

  completeds_Decision_RC_J    += otherJCL.completeds_Decision_RC_J;   // Свършени дела - С решение
  completeds_grant_entire_J   += otherJCL.completeds_grant_entire_J;  // Свършени дела - Искът уважен изцяло
  completeds_grant_partial_J  += otherJCL.completeds_grant_partial_J; // Свършени дела - Искът уважен отчасти
  completeds_grant_rejected_J += otherJCL.completeds_grant_rejected_J;// Свършени дела - Искът отхвърлен
  completeds_ess_CS_RC_J  += otherJCL.completeds_ess_CS_RC_J;         // Свършени дела - Свършени с акт по същество в З.З
  completeds_canceleds_byAgreement_J += otherJCL.completeds_canceleds_byAgreement_J;                //Св.дела-Прекратени дела-По спогодба
  completeds_canceleds_byOtherCauses_OS_J += otherJCL.completeds_canceleds_byOtherCauses_OS_J;      //Св.дела-Прекратени дела-Др.причини-ОЗ-Влезли в законова сила
  completeds_canceleds_byOtherCauses_CS_J += otherJCL.completeds_canceleds_byOtherCauses_CS_J;      //Св.дела-Прекратени дела-Др.причини-ЗЗ-Влезли в законова сила
  completeds_canceleds_summByAgreeByOth_RC_J += otherJCL.completeds_canceleds_summByAgreeByOth_RC_J;//Свършени дела - Прекратени дела - SUM

  notFinishedsBeg1_J += otherJCL.notFinishedsBeg1_J ;// Останали несвършени  в началото на отчетния период - RC CL report+judge distribution
  notFinishedsEnd1_J += otherJCL.notFinishedsEnd1_J ;// Останали несвършени дела в края на отчетния период
// 20061207 <--

// 20070202 ->
  appealeds_RC_J += otherJCL.appealeds_RC_J;  // Обжалвани дела
// 20070202 <-

  return (* this) ;
 }  // judgeCheckupLawsuits & judgeCheckupLawsuits :: operator +=

size_t judgeCheckupSessions :: size () const
 {
  const char * b = (const char *) (& fixeds1) ;
  const char * e = (const char *) (& lefts1) + sizeof (lefts1) ;

  return (e - b) ;
 }  // judgeCheckupSessions :: size

void judgeCheckupSessions :: clearData ()
 {
  setmem (& fixeds1, size (), '\0') ;
 }  // judgeCheckupSessions :: clearData

bool judgeCheckupSessions ::
  operator == (const judgeCheckupSessions & otherJCS) const
 {
  return ! memcmp (& fixeds1, & (otherJCS.fixeds1), size ()) ;
 }  // judgeCheckupSessions :: operator ==

bool judgeCheckupSessions :: isEmpty () const
 {
  judgeCheckupSessions rL1 ;

  return ((* this) == rL1) ;
 }  // judgeCheckupSessions :: isEmpty

judgeCheckupSessions & judgeCheckupSessions ::
  operator += (const judgeCheckupSessions & otherJCS)
 {
  fixeds1 += otherJCS.fixeds1 ;

  lefts1 += otherJCS.lefts1 ;
  otherActs1 += otherJCS.otherActs1 ;
  fixedsIncludingOtherActs1 += otherJCS.fixedsIncludingOtherActs1 ;
  fixeds1Judge1 += otherJCS.fixeds1Judge1 ;
  fixeds2Judges1 += otherJCS.fixeds2Judges1 ;
  fixeds3Judges1 += otherJCS.fixeds3Judges1 ;
  lefts1stSess1 += otherJCS.lefts1stSess1 ;
  lefts2ndSess1 += otherJCS.lefts2ndSess1 ;
  stoppeds1 += otherJCS.stoppeds1 ;
  fixedsIfNotJudge1 += otherJCS.fixedsIfNotJudge1 ;
  lefts3Judges1 += otherJCS.lefts3Judges1 ;
  lefts3Judges1stSess1 += otherJCS.lefts3Judges1stSess1 ;
  lefts3Judges2ndSess1 += otherJCS.lefts3Judges2ndSess1 ;
  abolishedResults1 += otherJCS.abolishedResults1 ;
  additionalDecisDefinOpenS1 += otherJCS.additionalDecisDefinOpenS1 ;
  additionalDecisDefinCldsS1 += otherJCS.additionalDecisDefinCldsS1 ;
# if INSTANCE
  fixeds2 += otherJCS.fixeds2 ;
  lefts2 += otherJCS.lefts2 ;
  otherActs2 += otherJCS.otherActs2 ;
  fixedsIncludingOtherActs2 += otherJCS.fixedsIncludingOtherActs2 ;
  fixedsIfInMembership2 += otherJCS.fixedsIfInMembership2 ;
  leftsIfInMembership2 += otherJCS.leftsIfInMembership2 ;
  otherActsIfInMembership2 += otherJCS.otherActsIfInMembership2 ;
  fixedsIncludOthActsIfInMembersh2 +=
    otherJCS.fixedsIncludOthActsIfInMembersh2 ;
  fixeds3Judges2 += otherJCS.fixeds3Judges2 ;
  lefts1stSess2 += otherJCS.lefts1stSess2 ;
  lefts2ndSess2 += otherJCS.lefts2ndSess2 ;
  stoppeds2 += otherJCS.stoppeds2 ;
  fixedsIfNotJudge3_2 += otherJCS.fixedsIfNotJudge3_2 ;
  lefts3Judges2 += otherJCS.lefts3Judges2 ;
  lefts3Judges1stSess2 += otherJCS.lefts3Judges1stSess2 ;
  lefts3Judges2ndSess2 += otherJCS.lefts3Judges2ndSess2 ;
  abolishedResults2 += otherJCS.abolishedResults2 ;
  additionalDecisDefinOpenS2 += otherJCS.additionalDecisDefinOpenS2 ;
  additionalDecisDefinCldsS2 += otherJCS.additionalDecisDefinCldsS2 ;
  appeal192_193sessRes2 += otherJCS.appeal192_193sessRes2 ;
# endif

// 20061207 -->
  lefts_reconcileSess_J += otherJCS.lefts_reconcileSess_J;            // Отложени дела - Помирителен срок
  lefts_BeforeFirstSess_RC_J += otherJCS.lefts_BeforeFirstSess_RC_J;  // Отложени дела - Преди първо заседание
  lefts_Interim_RC_J  += otherJCS.lefts_Interim_RC_J;                 // Отложени дела - Заседание по привременни мерки и друго заседание
  lefts_total_RC_J  += otherJCS.lefts_total_RC_J;                     // Отложени дала - Отложени ОБЩО
  split_J  += otherJCS.split_J;                                       // Допускане на делба
  otherSessAfterFinish_J  += otherJCS.otherSessAfterFinish_J;         // Допълване на решение, определение

  lefts1stSess1_J += otherJCS.lefts1stSess1_J ;
  lefts2ndSess1_J += otherJCS.lefts2ndSess1_J ;
  fixeds_RC_J += otherJCS.fixeds_RC_J;                            // Насрочени дела

// 20061207 <--


  return (* this) ;
 }  // judgeCheckupSessions & judgeCheckupSessions :: operator +=

#if USE_JUDGE_CHECKUP_OTHERS
size_t judgeCheckupOthers :: size () const
 {
  const char * b = (const char *) (& reqResolutions1) ;
  const char * e = (const char *) (& inRegEvents1) + sizeof (inRegEvents1) ;

  return (e - b) ;
 }  // judgeCheckupOthers :: size

void judgeCheckupOthers :: clearData ()
 {
  setmem (& reqResolutions1, size (), '\0') ;
 }  // judgeCheckupOthers :: clearData

bool judgeCheckupOthers ::
  operator == (const judgeCheckupOthers & otherJCO) const
 {
  return ! memcmp (& reqResolutions1, & (otherJCO.reqResolutions1), size ()) ;
 }  // judgeCheckupOthers :: operator ==

bool judgeCheckupOthers :: isEmpty () const
 {
  judgeCheckupOthers rL1 ;

  return ((* this) == rL1) ;
 }  // judgeCheckupOthers :: isEmpty

judgeCheckupOthers & judgeCheckupOthers ::
  operator += (const judgeCheckupOthers & otherJCO)
 {
  reqResolutions1 += otherJCO.reqResolutions1 ;
  inRegEvents1 += otherJCO.inRegEvents1 ;
# if INSTANCE
  reqResolutions2 += otherJCO.reqResolutions2 ;
  inRegEvents2 += otherJCO.inRegEvents2 ;
# endif
  return (* this) ;
 }  
#endif  // of USE_JUDGE_CHECKUP_OTHERS

void judgeCheckupData :: clearData ()
 {
  setmem (judgeName, sizeof (judgeName), '\0') ;
  setmem (judgeUCN, sizeof (judgeUCN), '\0') ;
  judgeComposit = 0 ;
  jCL.clearData () ;
  jCS.clearData () ;
  judgePost = '\0' ;
#if USE_JUDGE_CHECKUP_OTHERS
  jCO.clearData () ;  // 07.jun.2004
#endif  // of USE_JUDGE_CHECKUP_OTHERS
 }  // judgeCheckupData :: clearData

bool judgeCheckupData :: countingDataEmpty () const
 {
  return jCL.isEmpty () && jCS.isEmpty ()
#if USE_JUDGE_CHECKUP_OTHERS
    && jCO.isEmpty ()  // 07.jun.2004
#endif  // of USE_JUDGE_CHECKUP_OTHERS
   ;
 }  // judgeCheckupData :: countingDataEmpty

bool judgeCheckupData ::
	operator == (const judgeCheckupData & other) const
 {
  return (judgeComposit == other.judgeComposit &&
	    //judgePost == other.judgePost &&  // 25.nov.2004
	    strcmp (judgeName, other.judgeName) == 0 &&
	    strcmp (judgeUCN, other.judgeUCN) == 0) ;
 }  // judgeCheckupData :: operator ==

bool judgeCheckupData :: operator < (const judgeCheckupData & other) const
 {
  const bool thisInter = strcmp(judgeUCN, INTER) == 0 ;  // 25.nov.2004 - begin
  const bool otherInter = strcmp(other.judgeUCN, INTER) == 0 ;

  if(thisInter == otherInter)
   {                                                     // 25.nov.2004 - end
    if (judgeComposit < other.judgeComposit)
	return true ;
    else
	if (judgeComposit > other.judgeComposit)
	  return false ;
	else
	  //if (judgePost < other.judgePost)  // 25.nov.2004 - begin
	  //  return 1 ;
	  //else
	  //  if (judgePost > other.judgePost)
	  //    return 0 ;
	  //  else                            // 25.nov.2004 - end
	     {
		const int namesCmpRes = strcmp (judgeName, other.judgeName) ;

		if (namesCmpRes < 0)
		  return true ;
		else
		  if (namesCmpRes > 0)
		    return false ;
		  else
		    return (strcmp (judgeUCN, other.judgeUCN) < 0) ;
	     }
   }  // 25.nov.2004 - begin
  else
    return (thisInter == false && otherInter == true);  // 25.nov.2004 - end
 }  // judgeCheckupData :: operator <

void judgeCheckupData :: operator += (const judgeCheckupData & other)
 {
  jCL += other.jCL ;
  jCS += other.jCS ;
#if USE_JUDGE_CHECKUP_OTHERS
  jCO += other.jCO ;  // 07.jun.2004
#endif  // of USE_JUDGE_CHECKUP_OTHERS
 }  // judgeCheckupData :: operator +=




// --------  judgeCheckupColumns ---------------------------------- >>>
// не се използва за справка за дейността на съда - справката по съдии

void judgeCheckupColumns :: addNmbToClmn (column * pColumn,
                                         const long int n)
 {
  if (pColumn && n)
   {
    char b [128] ;

    ostrstream (b, sizeof (b)) << n << ends ;
    b [sizeof (b) - 1] = '\0' ;
    pColumn -> addText (b) ;
   }
 }  // judgeCheckupColumns :: addNmbToClmn

void judgeCheckupColumns :: makeColumns (const judgeCheckupData & jCD,
                               const CDate * const pDateForMonthColumn,
                                        const int serialNo)
 {
  column * pColumn ;  // To make looking of the program text more easy
  const judgeCheckupLawsuits & jCL = jCD.jCL ;
  const judgeCheckupSessions & jCS = jCD.jCS ;
#if USE_JUDGE_CHECKUP_OTHERS
  const judgeCheckupOthers & jCO = jCD.jCO ;
#endif  // of USE_JUDGE_CHECKUP_OTHERS

  if (! columns)
    return ;

  if ((pColumn = columns [iC_Judge]) != NULL)
   {  // -- iC_Judge --
    pColumn -> addText (jCD.judgeName) ;
   }  // -- iC_Judge --
  if ((pColumn = columns [iC_Composition]) != NULL)
   {  // -- iC_Composition --
    if (jCD.judgeComposit)
     {
      char tmp [16] ;  ZERO_LEN( tmp );

      compositionToString (jCD.judgeComposit, tmp, sizeof (tmp)) ;
      pColumn -> addText (tmp) ;
     }
   }  // -- iC_Composition --
  if ((pColumn = columns [iC_Month]) != NULL)
   {  // -- iC_Month --
    if (pDateForMonthColumn)
     {
      char tmp [16] ;

      ostrstream (tmp, sizeof (tmp)) << (* pDateForMonthColumn) << ends ;
      tmp [sizeof (tmp) - 1] = '\0' ;
      memmove (tmp, tmp + 3, strlen (tmp + 3) + 1) ;
      pColumn -> addText (tmp) ;
     }
   }  // -- iC_Month --
  addNmbToClmn (columns [iC_L_NotFinishedsBeg1], jCL.notFinishedsBeg1) ;
  addNmbToClmn (columns [iC_L_News1], jCL.news1) ;
  addNmbToClmn (columns [iC_L_NewsRejudge1], jCL.newsRejudge1) ;  // 11.dec.2003
  addNmbToClmn (columns [iC_L_NewsPunGenChar1], jCL.newsPunGenChar1) ;
  addNmbToClmn (columns [iC_L_NewsPunGenCharRejudge1],
                jCL.newsPunGenCharRejudge1) ;  // 11.dec.2003
	addNmbToClmn (columns [iC_L_FinishedsTotal1], jCL.finishedsTotal1) ;
  addNmbToClmn (columns [iC_L_FinishedsWithDecision1],
                jCL.finishedsWithDecision1) ;
  addNmbToClmn (columns [iC_L_FinishedsCanceleds1],
                jCL.finishedsCanceleds1) ;
  addNmbToClmn (columns [iC_L_DecisDefinClosedS1],
                jCL.decisDefinClosedS1) ;
  addNmbToClmn (columns [iC_L_CanceledsClosedS1], jCL.canceledsClosedS1) ;
  addNmbToClmn (columns [iC_L_FinishedsUpTo3M1], jCL.finishedsUpTo3M1) ;
  addNmbToClmn (columns [iC_L_FinishedsOver3M1], jCL.finishedsOver3M1) ;
  addNmbToClmn (columns [iC_L_NotFinishedsEnd1], jCL.notFinishedsEnd1) ;
  addNmbToClmn (columns [iC_L_Abolisheds1], jCL.abolisheds1) ;
  addNmbToClmn (columns [iC_L_FinishedsCanceledsByAgr1],
                jCL.finishedsCanceledsByAgr1) ;
  addNmbToClmn (columns [iC_L_FinishedsCanceledsOtherC1],
                jCL.finishedsCanceledsOtherC1) ;
  addNmbToClmn (columns [iC_L_KeepingMethodConfirmed1],
                jCL.keepingMethodConfirmed1) ;
  addNmbToClmn (columns [iC_L_KeepingMethodChanged1],
                jCL.keepingMethodChanged1) ;
  addNmbToClmn (columns [iC_L_Subject0210], jCL.subject0210) ;
  addNmbToClmn (columns [iC_L_Subject0600], jCL.subject0600) ;
  addNmbToClmn (columns [iC_L_Subject0500], jCL.subject0500) ;
  addNmbToClmn (columns [iC_L_Subject0810], jCL.subject0810) ;
  addNmbToClmn (columns [iC_L_Subjects0400_0410], jCL.subjects0400_0410);
  addNmbToClmn (columns [iC_L_Subjects0220_0310_0800],
                jCL.subjects0220_0310_0800);
# if INSTANCE
  addNmbToClmn (columns [iC_L_NotFinishedsBeg2], jCL.notFinishedsBeg2) ;
  addNmbToClmn (columns [iC_L_News2], jCL.news2) ;
  addNmbToClmn (columns [iC_L_NewsRejudge2], jCL.newsRejudge2) ;  // 11.dec.2003
  addNmbToClmn (columns [iC_L_NewsPunGenChar2], jCL.newsPunGenChar2) ;
  addNmbToClmn (columns [iC_L_NewsPunGenCharRejudge2],
                jCL.newsPunGenCharRejudge2) ;  // 11.dec.2003
	addNmbToClmn (columns [iC_L_FinishedsTotal2], jCL.finishedsTotal2) ;
  addNmbToClmn (columns [iC_L_FinishedsWithDecision2],
                jCL.finishedsWithDecision2) ;
  addNmbToClmn (columns [iC_L_FinishedsCanceleds2],
                jCL.finishedsCanceleds2) ;
  addNmbToClmn (columns [iC_L_DecisDefinClosedS2],
                jCL.decisDefinClosedS2) ;
  addNmbToClmn (columns [iC_L_CanceledsClosedS2], jCL.canceledsClosedS2) ;
  addNmbToClmn (columns [iC_L_FinishedsUpTo3M2], jCL.finishedsUpTo3M2) ;
  addNmbToClmn (columns [iC_L_FinishedsOver3M2], jCL.finishedsOver3M2) ;
  addNmbToClmn (columns [iC_L_NotFinishedsEnd2], jCL.notFinishedsEnd2) ;
  addNmbToClmn (columns [iC_L_FinishedsCanceledsByAgr2],
                jCL.finishedsCanceledsByAgr2) ;
  addNmbToClmn (columns [iC_L_FinishedsCanceledsOtherC2],
                jCL.finishedsCanceledsOtherC2) ;
  addNmbToClmn (columns [iC_L_ExactComplaints2], jCL.exactComplaints2) ;
  addNmbToClmn (columns [iC_L_PrivateComplaints2],
                jCL.privateComplaints2) ;
#   if APPEAL
  addNmbToClmn (columns [iC_L_NewsPGenCharAplCourt],
                jCL.newsPGenCharAplCourt) ;
  addNmbToClmn (columns [iC_L_NewsPKeepOpenSessAplCourt],
                jCL.newsPKeepOpenSessAplCourt) ;
  addNmbToClmn (columns [iC_L_NewsPKeepClSessAplCourt],
                jCL.newsPKeepClSessAplCourt) ;
  addNmbToClmn (columns [iC_L_NewsPEnactmentAplCourt],
                jCL.newsPEnactmentAplCourt) ;
  addNmbToClmn (columns [iC_L_NewsPOthersAplCourt],
                jCL.newsPOthersAplCourt) ;
  addNmbToClmn (columns [iC_L_DecPGenCharAplCourt],
                jCL.decPGenCharAplCourt) ;
  addNmbToClmn (columns [iC_L_DecPKeepOpenSessAplCourt],
                jCL.decPKeepOpenSessAplCourt) ;
  addNmbToClmn (columns [iC_L_DecPKeepClSessAplCourt],
                jCL.decPKeepClSessAplCourt) ;
  addNmbToClmn (columns [iC_L_DecPEnactmentAplCourt],
                jCL.decPEnactmentAplCourt) ;
  addNmbToClmn (columns [iC_L_DecPOthersAplCourt],
                jCL.decPOthersAplCourt) ;
#   endif  // of APPEAL
// 05.apr.2002 - begin
  addNmbToClmn (columns [iC_L_DetailNotFinishedBegTotal2],
                jCL.detailNotFinishedBegTotal2) ;
  addNmbToClmn (columns [iC_L_DetailNotFinBegPunGen2],
                jCL.detailNotFinBegPunGen2) ;
  addNmbToClmn (columns [iC_L_DetailNotFinBegPunPrv2],
                jCL.detailNotFinBegPunPrv2) ;
  addNmbToClmn (columns [iC_L_DetailNotFinishedEndTotal2],
                jCL.detailNotFinishedEndTotal2) ;
  addNmbToClmn (columns [iC_L_DetailNotFinEndPunGen2],
                jCL.detailNotFinEndPunGen2) ;
  addNmbToClmn (columns [iC_L_DetailNotFinEndPunPrv2],
                jCL.detailNotFinEndPunPrv2) ;
// 05.apr.2002 - end
# endif
// 05.apr.2002 - begin
  addNmbToClmn (columns [iC_L_DetailNotFinishedBegTotal1],
                jCL.detailNotFinishedBegTotal1) ;
  addNmbToClmn (columns [iC_L_DetailNotFinishedEndTotal1],
                jCL.detailNotFinishedEndTotal1) ;
// 05.apr.2002 - end

  addNmbToClmn (columns [iC_S_Fixeds1], jCS.fixeds1) ;
  addNmbToClmn (columns [iC_S_Lefts1], jCS.lefts1) ;
  addNmbToClmn (columns [iC_S_OtherActs1], jCS.otherActs1) ;
  addNmbToClmn (columns [iC_S_FixedsIncludingOtherActs1],
                         jCS.fixedsIncludingOtherActs1) ;
  addNmbToClmn (columns [iC_S_Fixeds1Judge1], jCS.fixeds1Judge1) ;
  addNmbToClmn (columns [iC_S_Fixeds2Judges1], jCS.fixeds2Judges1) ;
  addNmbToClmn (columns [iC_S_Fixeds3Judges1], jCS.fixeds3Judges1) ;
  addNmbToClmn (columns [iC_S_Lefts1stSess1], jCS.lefts1stSess1) ;
  addNmbToClmn (columns [iC_S_Lefts2ndSess1], jCS.lefts2ndSess1) ;
  addNmbToClmn (columns [iC_S_Stoppeds1], jCS.stoppeds1) ;
  addNmbToClmn (columns [iC_S_FixedsIfNotJudge1],
                jCS.fixedsIfNotJudge1) ;
  addNmbToClmn (columns [iC_S_Lefts3Judges1], jCS.lefts3Judges1) ;
  addNmbToClmn (columns [iC_S_Lefts3Judges1stSess1],
                jCS.lefts3Judges1stSess1) ;
  addNmbToClmn (columns [iC_S_Lefts3Judges2ndSess1],
                jCS.lefts3Judges2ndSess1) ;
  addNmbToClmn (columns [iC_S_AbolishedResults1],
                jCS.abolishedResults1) ;
  addNmbToClmn (columns [iC_S_AdditionalDecisDefinOpenS1],
                jCS.additionalDecisDefinOpenS1) ;
  addNmbToClmn (columns [iC_S_AdditionalDecisDefinCldsS1],
                jCS.additionalDecisDefinCldsS1) ;

# if INSTANCE
  addNmbToClmn (columns [iC_S_Fixeds2], jCS.fixeds2) ;
  addNmbToClmn (columns [iC_S_Lefts2], jCS.lefts2) ;
  addNmbToClmn (columns [iC_S_OtherActs2], jCS.otherActs2) ;
  addNmbToClmn (columns [iC_S_FixedsIncludingOtherActs2],
                         jCS.fixedsIncludingOtherActs2) ;
  addNmbToClmn (columns [iC_S_FixIfInMembersh2],
                         jCS.fixedsIfInMembership2) ;
  addNmbToClmn (columns [iC_S_LeftsIfInMembersh2],
			 jCS.leftsIfInMembership2) ;
  addNmbToClmn (columns [iC_S_OtherActsIfInMembersh2],
                         jCS.otherActsIfInMembership2) ;
  addNmbToClmn (columns [iC_S_FixInclOthActsIfInMemb2],
                         jCS.fixedsIncludOthActsIfInMembersh2) ;
  addNmbToClmn (columns [iC_S_Fixeds3Judges2], jCS.fixeds3Judges2) ;
  addNmbToClmn (columns [iC_S_Lefts1stSess2], jCS.lefts1stSess2) ;
  addNmbToClmn (columns [iC_S_Lefts2ndSess2], jCS.lefts2ndSess2) ;
  addNmbToClmn (columns [iC_S_Stoppeds2], jCS.stoppeds2) ;
  addNmbToClmn (columns [iC_S_FixedsIfNotJudge3_2],
                jCS.fixedsIfNotJudge3_2) ;
  addNmbToClmn (columns [iC_S_Lefts3Judges2], jCS.lefts3Judges2) ;
  addNmbToClmn (columns [iC_S_Lefts3Judges1stSess2],
                jCS.lefts3Judges1stSess2) ;
  addNmbToClmn (columns [iC_S_Lefts3Judges2ndSess2],
                jCS.lefts3Judges2ndSess2) ;
  addNmbToClmn (columns [iC_S_AbolishedResults2],
                jCS.abolishedResults2) ;
  addNmbToClmn (columns [iC_S_AdditionalDecisDefinOpenS2],
                jCS.additionalDecisDefinOpenS2) ;
  addNmbToClmn (columns [iC_S_AdditionalDecisDefinCldsS2],
                jCS.additionalDecisDefinCldsS2) ;
  addNmbToClmn (columns [iC_S_Appeal192_193sessRes2],
                jCS.appeal192_193sessRes2) ;
# endif

#if USE_JUDGE_CHECKUP_OTHERS
  addNmbToClmn (columns [iC_Oth_ReqResolutions1],  // 07.jun.2004 - begin
		jCO.reqResolutions1) ;
  addNmbToClmn (columns [iC_Oth_InRegEvents1],
                jCO.inRegEvents1) ;                // 07.jun.2004 - end
# if INSTANCE
  addNmbToClmn (columns [iC_Oth_ReqResolutions2],  // 07.jun.2004 - begin
                jCO.reqResolutions2) ;
  addNmbToClmn (columns [iC_Oth_InRegEvents2],
                jCO.inRegEvents2) ;                // 07.jun.2004 - end
# endif
#endif  // of USE_JUDGE_CHECKUP_OTHERS


// 20061207 -->
// Lawsuit
  addNmbToClmn (columns [iC_L_NewsRejudge1_J], jCL.newsRejudge1_J); // Постъпили дела - Новообразувани
  addNmbToClmn (columns [iC_L_byJurisd_J],  jCL.byJurisd_J);        // Постъпили дела - Получени по подсъдност
  addNmbToClmn (columns [iC_L_returneds_J], jCL.returneds_J);       // Постъпили дела - Върнати за ново разглеждане
  addNmbToClmn (columns [iC_L_Completeds_Decision_RC_J],   jCL.completeds_Decision_RC_J);   // Свършени дела - С решение
  addNmbToClmn (columns [iC_L_Completeds_Grant_Entire_J],  jCL.completeds_grant_entire_J);  // Свършени дела - Искът уважен изцяло
  addNmbToClmn (columns [iC_L_Completeds_Grant_Partial_J], jCL.completeds_grant_partial_J); // Свършени дела - Искът уважен отчасти
  addNmbToClmn (columns [iC_L_Completeds_Grant_Rejected_J],jCL.completeds_grant_rejected_J);// Свършени дела - Искът отхвърлен
  addNmbToClmn (columns [iC_L_Completeds_ess_CS_RC_J],     jCL.completeds_ess_CS_RC_J);     // Свършени дела - Свършени с акт по същество в З.З
  addNmbToClmn (columns [iC_L_Completeds_canceleds_byAgreement_J],        jCL.completeds_canceleds_byAgreement_J);         // Свършени дела - Прекратени дела - По спогодба
  addNmbToClmn (columns [iC_L_Completeds_canceleds_byOtherCauses_OS_J],   jCL.completeds_canceleds_byOtherCauses_OS_J);    // Свършени дела - Прекратени дела - Други причини - OZ- Влезли в законова сила
  addNmbToClmn (columns [iC_L_Completeds_canceleds_byOtherCauses_CS_J],   jCL.completeds_canceleds_byOtherCauses_CS_J);    // Свършени дела - Прекратени дела - Други причини - ZZ- Влезли в законова сила
  addNmbToClmn (columns [iC_L_Completeds_canceleds_summByAgreeByOth_RC_J],jCL.completeds_canceleds_summByAgreeByOth_RC_J); // Свършени дела - SUM
  addNmbToClmn (columns [iC_L_NotFinishedsBeg1_J], jCL.notFinishedsBeg1_J) ; // Останали несвършени  в началото на отчетния период
  addNmbToClmn (columns [iC_L_NotFinishedsEnd1_J], jCL.notFinishedsEnd1_J) ; // Останали несвършени дела в края на отчетния период
  // 20070202
  addNmbToClmn (columns [iC_L_Appealeds_RC_J], jCL.appealeds_RC_J) ; // Обжалвани дела


// Session
  addNmbToClmn (columns [iC_S_Lefts_ReconcileSess_J], jCS.lefts_reconcileSess_J);       // Отложени дела - Помирителен срок
  addNmbToClmn (columns [iC_S_Lefts_BeforeFirstSess_J], jCS.lefts_BeforeFirstSess_RC_J);// Отложени дела - Преди първо заседание
  addNmbToClmn (columns [iC_S_Lefts_Interim_RC_J], jCS.lefts_Interim_RC_J);// Отложени дела - Заседание по привременни мерки и друго заседание
  addNmbToClmn (columns [iC_S_Lefts_total_RC_J], jCS.lefts_total_RC_J);    // Отложени дала - Отложени ОБЩО
  addNmbToClmn (columns [iC_S_Splits_J], jCS.split_J);                     // Допускане на делба
  addNmbToClmn (columns [iC_S_otherSessAfterFinish_J], jCS.otherSessAfterFinish_J);     // Допълване на решение, определение
  addNmbToClmn (columns [iC_S_Lefts1stSess1_J], jCS.lefts1stSess1_J) ;
  addNmbToClmn (columns [iC_S_Lefts2ndSess1_J], jCS.lefts2ndSess1_J) ;
  addNmbToClmn (columns [iC_S_Fixeds_RC_J], jCS.fixeds_RC_J) ;
/*
  iC_L_Receiveds_Total_J,           // 209   Постъпили дела - Общо
  iC_L_ForExamineEval_J,            // 210   Всичко дела за разглеждане
*/

// 20061207 <--

  addNmbToClmn (columns [iC_SerialNo], serialNo) ;

 }  // judgeCheckupColumns :: makeColumns

judgeCheckupColumns :: judgeCheckupColumns (const char * const formatFN,
					    TWindow * parent,
					    const judgeCheckupData & jCD,
			 const CDate * const pDateForMonthColumn,
					const int serialNo) :
			 judgeCheckupEnum (),
			 columnsArr (formatFN, parent, i_columns,
			 1 + OFFSET_JUDGE_CHECKUP_CIPHER)
 {
  makeColumns (jCD, pDateForMonthColumn, serialNo) ;
  defineMaxRows () ;
 }  // judgeCheckupColumns :: judgeCheckupColumns

// -------- TJudgeCheckupThread ----------------------------------------------
void TJudgeCheckupThread::putJudgeCheckupDataInArr(const judgeCheckupData *jCDPtr)
 {
  const int index = pArr -> Find (jCDPtr) ;

  if (index == INT_MAX)
   {  // Not found
    judgeCheckupData * pJCD = newDataItem () ;

    if (pJCD)
     {
	(* pJCD) = (* jCDPtr) ;
	pArr -> Add (pJCD) ;
     }
   }
 }

void TJudgeCheckupThread::makeArr()
 {
 }

bool TJudgeCheckupThread::columnRequired(unsigned int dataCiph, const bookAreas * bkAreas, int areasCnt) const
 {
  // 200612 ..
  const int dC = OFFSET_JUDGE_CHECKUP_CIPHER + 1 + dataCiph ;

  if (! (bkAreas && areasCnt))
    return false ;
  for (int i = 0 ; i < areasCnt ; i ++)
    if (dC == bkAreas [i].dataCipher)
	return true ;
  return false ;
 }

void TJudgeCheckupThread::addJCDToArrayIfNecessary(const char * const judge)
 {
  if (judge)
   {
    bool found = false ;

    for (int i = 0 ; ! found && i < pArr -> Count () ;
	 i ++)
      found = ! strcmp (((* pArr) [i]) -> judgeUCN, judge) ;
    if (! found)
     {
      judgeCheckupData * const pTmpJCD = newDataItem () ;
      judgeCheckupData & jCD = (* pTmpJCD) ;
      TCitizen c ;

      scSprintf (sizeof (jCD.judgeUCN), jCD.judgeUCN, "%s", judge) ;
      c.ucnType = UCN_CITIZEN_UCN ;
      scSprintf (sizeof (c.ucn), c.ucn, "%s", judge) ;
      if (c.Get ())
	scSprintf (sizeof (jCD.judgeName), jCD.judgeName, "%s %s",
		   c.name, c.family) ;
       {
	TUser user ;

	scSprintf (sizeof (user.ucn), user.ucn, "%s", judge) ;
	if (user.Try ())
	  jCD.judgePost = user.post ;
	else
	  jCD.judgePost = (char) 255 ;
       }
      putJudgeCheckupDataInArr (& jCD) ;
      delete pTmpJCD ;
     }
   }
 }

bool TJudgeCheckupThread::skipThisLaw(const char
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
	lK
#   endif  // of APPEAL
# endif  // of INSTANCE
	)
 {
# if INSTANCE
#   if APPEAL
  return false ;
#   else  // of APPEAL
  return (
	  lK &&
	  ! trueJudgeCheckup &&
	  ! cCBDEDBN.firstInstLaws &&
	    strchr (cCBDEDBN.colleges, TYPE_PUNISHMENT_LAW) &&
	    strchr (KIND_2ND_LAWSUITS, lK) &&
	    ! cCBDEDBN.all2ndPuniLaws &&  // 02.jan.2002
	    (cCBDEDBN.puniAdmConcernLaws !=
	     (lK == KIND_2ND_ADMIN_PUNISHMENT_LAW))
	   ) ;
#   endif  // of APPEAL
# else  // of INSTANCE
  return false ;
# endif  // of INSTANCE
 }

bool TJudgeCheckupThread::skipThisLaw (const long int
# if INSTANCE
#   if APPEAL
#   endif  // APPEAL
# else  // INSTANCE
	subj
# endif
	)
 {
# if INSTANCE
#   if APPEAL
  return false ;  // 20.jul.2004 - end
#   else  // of APPEAL
  return false ;
#   endif  // of APPEAL
# else  // of INSTANCE
  if (! subj)
    return false ;
  if (reqKindsString () == NULL)
    return false ;

  const arrayOfLong &
    const allowedSubjectsArr = * (bMYEMYTCIU.lArrPtr ()) ;

  for (int i = 0 ;
       i < allowedSubjectsArr.Count () ; i ++)
    if (subj == allowedSubjectsArr [i])
      return false ;
  return true ;
# endif  // INSTANCE
 }

void TJudgeCheckupThread::incrementValues_Law(TLawsuit & l, unsigned int dataCiph,
# if INSTANCE
	unsigned int dataCiph2nd,
# endif
	const long int iV)
 {
  if (skipThisLaw (l.kind))
    return ;  // continue ;
  if (skipThisLaw (l.subject))
    return ;  // continue ;
  addJCDToArrayIfNecessary (l.judge) ;
   {  // -- 0b --
    bool found = false ;

    for (int i = 0 ; ! found && i < pArr -> Count () ;
	   i ++)
     {  // -- 1 --
      judgeCheckupData * pJCD = (* pArr) [i] ;

      if (! strcmp (pJCD -> judgeUCN, l.judge))
       {  // -- 1a --
        found = true ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
        const bool firstInstLaw =
                                  strchr (KIND_1ST_LAWSUITS, l.kind)
			  ;

        if (firstInstLaw)
#   endif  // of APPEAL
# endif
        {

	    if (    ( dataCiph == iC_S_Lefts_ReconcileSess_J )
               || ( dataCiph == iC_S_Lefts_BeforeFirstSess_J )
               || ( dataCiph == iC_S_Lefts1stSess1_J )
               || ( dataCiph == iC_S_Lefts2ndSess1_J )
	       || ( dataCiph == iC_S_Lefts_Interim_RC_J )
             )
             pJCD -> jCS.lefts_total_RC_J   += iV ;

          if (    ( dataCiph == iC_L_Completeds_canceleds_byAgreement_J )
               || ( dataCiph == iC_L_Completeds_canceleds_byOtherCauses_OS_J )
               || ( dataCiph == iC_L_Completeds_canceleds_byOtherCauses_CS_J )
             )
             pJCD -> jCL.completeds_canceleds_summByAgreeByOth_RC_J   += iV ;

          switch (dataCiph)
           {  // -- 2 --                         // for fmt

           case iC_L_NotFinishedsBeg1 :            //  2
	      pJCD -> jCL.notFinishedsBeg1 += iV ;
              break ;
            case iC_L_News1 :                       //  4
              pJCD -> jCL.news1 += iV ;
              break ;
            case iC_L_NewsRejudge1 :                // 191  11.dec.2003
              pJCD -> jCL.newsRejudge1 += iV ;
              break ;
						case iC_L_NewsPunGenChar1 :             // 189
							pJCD -> jCL.newsPunGenChar1 += iV ;
							break ;
						case iC_L_NewsPunGenCharRejudge1 :      // 193  11.dec.2003
							pJCD -> jCL.newsPunGenCharRejudge1 += iV ;
							break ;
						case iC_L_FinishedsTotal1 :             //  6
              pJCD -> jCL.finishedsTotal1 += iV ;
              break ;
            case iC_L_FinishedsWithDecision1 :      //  8
              pJCD -> jCL.finishedsWithDecision1 += iV ;
              break ;
            case iC_L_FinishedsCanceleds1 :         // 10
		  pJCD -> jCL.finishedsCanceleds1 += iV ;
              break ;
            case iC_L_FinishedsUpTo3M1 :            // 12
              pJCD -> jCL.finishedsUpTo3M1 += iV ;
              break ;
            case iC_L_FinishedsOver3M1 :            // 14
              pJCD -> jCL.finishedsOver3M1 += iV ;
	      break ;
            case iC_L_NotFinishedsEnd1 :            // 16
              pJCD -> jCL.notFinishedsEnd1 += iV ;
              break ;
            case iC_L_Abolisheds1 :                 // 37
              pJCD -> jCL.abolisheds1 += iV ;
              break ;
            case iC_L_FinishedsCanceledsByAgr1 :    // 38
              pJCD -> jCL.finishedsCanceledsByAgr1 += iV ;
              break ;
            case iC_L_FinishedsCanceledsOtherC1 :   // 40
              pJCD -> jCL.finishedsCanceledsOtherC1 += iV ;
              break ;
            case iC_L_KeepingMethodConfirmed1 :     // 50
	      pJCD -> jCL.keepingMethodConfirmed1 += iV ;
              break ;
            case iC_L_KeepingMethodChanged1 :       // 51
              pJCD -> jCL.keepingMethodChanged1 += iV ;
              break ;
            case iC_L_Subject0210 :                 // 52
            case iC_L_Subject0600 :                 // 53
            case iC_L_Subject0500 :                 // 54
            case iC_L_Subject0810 :                 // 55
            case iC_L_Subjects0400_0410 :           // 56
            case iC_L_Subjects0220_0310_0800 :      // 57
// Here bkAreas and areasCnt are not accessible
// (to call columnRequired ()), but the corresponding
// fields of pJCD -> jCL  ARE accessible and I use them.
// I hope, this will not spoil the output (if column is not required,
// I will not print this data).
              if (l.subject == 210)
                pJCD -> jCL.subject0210 += iV ;
              if (l.subject == 600)
                pJCD -> jCL.subject0600 += iV ;
              if (l.subject == 500)
                pJCD -> jCL.subject0500 += iV ;
              if (l.subject == 810)
                pJCD -> jCL.subject0810 += iV ;
              if (l.subject == 400 || l.subject == 410)
		    pJCD -> jCL.subjects0400_0410 += iV ;
              if (l.subject == 220 || l.subject == 310 ||
                  l.subject == 800)
		pJCD -> jCL.subjects0220_0310_0800 += iV ;
              break ;
            case iC_L_DecisDefinClosedS1 :            // 72
              pJCD -> jCL.decisDefinClosedS1 += iV ;
              break ;
            case iC_L_CanceledsClosedS1 :             // 74
              pJCD -> jCL.canceledsClosedS1 += iV ;
              break ;

            case iC_S_Fixeds1 :                     // 18
              pJCD -> jCS.fixeds1 += iV ;
              break ;
            case iC_S_Lefts1 :                      // 20
              pJCD -> jCS.lefts1 += iV ;
	      break ;
            case iC_S_OtherActs1 :                  // 22
              pJCD -> jCS.otherActs1 += iV ;
              break ;
            case iC_S_Lefts1stSess1 :               // 42
              pJCD -> jCS.lefts1stSess1 += iV ;
              break ;
            case iC_S_Lefts2ndSess1 :               // 44
              pJCD -> jCS.lefts2ndSess1 += iV ;
              break ;
            case iC_S_Stoppeds1 :                   // 46
              pJCD -> jCS.stoppeds1 += iV ;
              break ;
            case iC_S_AbolishedResults1 :           // 66
	      pJCD -> jCS.abolishedResults1 += iV ;
              break ;
      //       i_columns  // i_columns == (the number of columns)

            // 20061207 -->
            // Regional court - Справка за дейността на съда - Разпределение по съдии ->
            case iC_L_NewsRejudge1_J : pJCD -> jCL.newsRejudge1_J += iV ; break ;// 230 Постъпили дела - Новообразувани
            case iC_L_byJurisd_J     : pJCD -> jCL.byJurisd_J  += iV ;    break ;// 207 Постъпили дела - Получени по подсъдност
            case iC_L_returneds_J    : pJCD -> jCL.returneds_J  += iV ;   break ;// 208 Постъпили дела - Върнати за ново разглеждане

            // case iC_L_Receiveds_Total_J : pJCD -> jCL.  += iV ; break ;// 209   Постъпили дела - Общо
            // case iC_L_ForExamineEval_J : pJCD -> jCL.  += iV ;  break ;// 210   Всичко дела за разглеждане

            case iC_L_Completeds_Decision_RC_J    : pJCD -> jCL.completeds_Decision_RC_J   += iV ; break ;// 229 Свършени дела - С решение
	    case iC_L_Completeds_Grant_Entire_J   : pJCD -> jCL.completeds_grant_entire_J  += iV ; break ;// 215 Свършени дела - Искът уважен изцяло
		case iC_L_Completeds_Grant_Partial_J  : pJCD -> jCL.completeds_grant_partial_J += iV ; break ;// 216 Свършени дела - Искът уважен отчасти
            case iC_L_Completeds_Grant_Rejected_J : pJCD -> jCL.completeds_grant_rejected_J+= iV ; break ;// 217 Свършени дела - Искът отхвърлен
            case iC_L_Completeds_ess_CS_RC_J      : pJCD -> jCL.completeds_ess_CS_RC_J     += iV ; break ;// 218 Свършени дела - Свършени с акт по същество в З.З
            case iC_L_Completeds_canceleds_byAgreement_J : pJCD -> jCL.completeds_canceleds_byAgreement_J  += iV ; break ;// 219 - Свършени дела - Прекратени дела - По спогодба
            case iC_L_Completeds_canceleds_byOtherCauses_OS_J : pJCD -> jCL.completeds_canceleds_byOtherCauses_OS_J  += iV ;break ; // 220 - Св.дела - Прекратени дела - Др.причини - СЗ - Влезли в законова сила
            case iC_L_Completeds_canceleds_byOtherCauses_CS_J : pJCD -> jCL.completeds_canceleds_byOtherCauses_CS_J  += iV ;break ; // 221 - Св.дела - Прекратени дела - Др.причини - ЗЗ - Влезли в законова сила
            case iC_L_Completeds_canceleds_summByAgreeByOth_RC_J : pJCD -> jCL.completeds_canceleds_summByAgreeByOth_RC_J  += iV ;break ; // 222 - Свършени дела - Прекратени дела - ОБЩО

            case iC_S_Splits_J                : pJCD -> jCS.split_J  += iV ; break ;// 223   Допускане на делба
            case iC_S_otherSessAfterFinish_J  : pJCD -> jCS.otherSessAfterFinish_J  += iV ; break ;// 224   Допълване на решение, определение

            // Отложени дела ->
            case iC_S_Lefts_ReconcileSess_J   : pJCD -> jCS.lefts_reconcileSess_J  += iV ; break ;// 211   Отложени дела - Помирителен срок
	    case iC_S_Lefts_BeforeFirstSess_J : pJCD -> jCS.lefts_BeforeFirstSess_RC_J  += iV ; break ;// 212   Отложени дела - Преди първо заседание
            case iC_S_Lefts_Interim_RC_J      : pJCD -> jCS.lefts_Interim_RC_J += iV ; break ;  // 213 Oтл.дела - Заседание по привременни мерки и друго заседание
            //case iC_S_Lefts_total_RC_J        : pJCD -> jCS.lefts_total_RC   += iV ; break ;// 214 Отл.дала - Отложени ОБЩО
            case iC_S_Lefts1stSess1_J         : pJCD -> jCS.lefts1stSess1_J  += iV ; break ;  // 226 Отл.дела - В I зас.
            case iC_S_Lefts2ndSess1_J         : pJCD -> jCS.lefts2ndSess1_J  += iV ; break ;  // 217 Oтл.дела - Във II зас.и следващо съд.зас

            case iC_S_Fixeds_RC_J             : pJCD -> jCS.fixeds_RC_J += iV ; break ;       // 228 Насрочени дела

            case iC_L_NotFinishedsBeg1_J      : pJCD -> jCL.notFinishedsBeg1_J += iV ; break ; //  225

            case iC_L_NotFinishedsEnd1_J      : pJCD -> jCL.notFinishedsEnd1_J += iV ; break ; // 231

            // 20061207 <--
            // 20070202 ->
	    case iC_L_Appealeds_RC_J           : pJCD -> jCL.appealeds_RC_J += iV ; break ;   // Обжалвани дела


           }  // -- 2 --  switch
        }     // if (firstInstLaw)
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
        else
#   endif  // of APPEAL
         {  // -- 3 --                           // AREAL COURT -->
          switch (dataCiph2nd)
           {  // -- 4 --                         // for fmt
            case iC_L_NotFinishedsBeg2 :            //  3
	      pJCD -> jCL.notFinishedsBeg2 += iV ;
              break ;
            case iC_L_News2 :                       //  5
              pJCD -> jCL.news2 += iV ;
              break ;
		case iC_L_NewsRejudge2 :                // 192  11.dec.2003
              pJCD -> jCL.newsRejudge2 += iV ;
              break ;
						case iC_L_NewsPunGenChar2 :             // 190
							pJCD -> jCL.newsPunGenChar2 += iV ;
							break ;
						case iC_L_NewsPunGenCharRejudge2 :      // 194  11.dec.2003
							pJCD -> jCL.newsPunGenCharRejudge2 += iV ;
							break ;
						case iC_L_FinishedsTotal2 :             //  7
              pJCD -> jCL.finishedsTotal2 += iV ;
              break ;
            case iC_L_FinishedsWithDecision2 :      //  9
              pJCD -> jCL.finishedsWithDecision2 += iV ;
              break ;
            case iC_L_FinishedsCanceleds2 :         // 11
              pJCD -> jCL.finishedsCanceleds2 += iV ;
              break ;
            case iC_L_FinishedsUpTo3M2 :            // 13
              pJCD -> jCL.finishedsUpTo3M2 += iV ;
              break ;
            case iC_L_FinishedsOver3M2 :            // 15
              pJCD -> jCL.finishedsOver3M2 += iV ;
	      break ;
            case iC_L_NotFinishedsEnd2 :            // 17
              pJCD -> jCL.notFinishedsEnd2 += iV ;
              break ;
            case iC_L_FinishedsCanceledsByAgr2 :    // 39
              pJCD -> jCL.finishedsCanceledsByAgr2 += iV ;
              break ;
            case iC_L_FinishedsCanceledsOtherC2 :   // 41
              pJCD -> jCL.finishedsCanceledsOtherC2 += iV ;
              break ;
            case iC_L_ExactComplaints2 :            // 58
              pJCD -> jCL.exactComplaints2 += iV ;
              break ;
            case iC_L_PrivateComplaints2 :          // 59
	      pJCD -> jCL.privateComplaints2 += iV ;
              break ;
            case iC_L_DecisDefinClosedS2 :          // 73
              pJCD -> jCL.decisDefinClosedS2 += iV ;
              break ;
            case iC_L_CanceledsClosedS2 :           // 75
              pJCD -> jCL.canceledsClosedS2 += iV ;
              break ;
# if INSTANCE
#   if APPEAL
            case iC_L_NewsPGenCharAplCourt :        // 76
              pJCD -> jCL.newsPGenCharAplCourt += iV ;
              break ;
            case iC_L_NewsPKeepOpenSessAplCourt :   // 77
	      pJCD -> jCL.newsPKeepOpenSessAplCourt += iV ;
              break ;
            case iC_L_NewsPKeepClSessAplCourt :     // 78
              pJCD -> jCL.newsPKeepClSessAplCourt += iV ;
              break ;
            case iC_L_NewsPEnactmentAplCourt :      // 79
              pJCD -> jCL.newsPEnactmentAplCourt += iV ;
              break ;
            case iC_L_NewsPOthersAplCourt :         // 80
              pJCD -> jCL.newsPOthersAplCourt += iV ;
              break ;
            case iC_L_DecPGenCharAplCourt :         // 81
              pJCD -> jCL.decPGenCharAplCourt += iV ;
              break ;
	    case iC_L_DecPKeepOpenSessAplCourt :    // 82
              pJCD -> jCL.decPKeepOpenSessAplCourt += iV ;
              break ;
            case iC_L_DecPKeepClSessAplCourt :      // 83
              pJCD -> jCL.decPKeepClSessAplCourt += iV ;
              break ;
            case iC_L_DecPEnactmentAplCourt :       // 84
              pJCD -> jCL.decPEnactmentAplCourt += iV ;
              break ;
            case iC_L_DecPOthersAplCourt :          // 85
              pJCD -> jCL.decPOthersAplCourt += iV ;
              break ;
#   endif  // of APPEAL
// 05.apr.2002 - begin
	    case iC_L_DetailNotFinishedBegTotal2 :  // 173
              pJCD -> jCL.detailNotFinishedBegTotal2 += iV ;
              break ;
            case iC_L_DetailNotFinBegPunGen2 :      // 174
              pJCD -> jCL.detailNotFinBegPunGen2 += iV ;
              break ;
            case iC_L_DetailNotFinBegPunPrv2 :      // 175
              pJCD -> jCL.detailNotFinBegPunPrv2 += iV ;
              break ;
            case iC_L_DetailNotFinishedEndTotal2 :  // 177
              pJCD -> jCL.detailNotFinishedEndTotal2 += iV ;
              break ;
		case iC_L_DetailNotFinEndPunGen2 :      // 178
		  pJCD -> jCL.detailNotFinEndPunGen2 += iV ;
	      break ;
            case iC_L_DetailNotFinEndPunPrv2 :      // 179
              pJCD -> jCL.detailNotFinEndPunPrv2 += iV ;
              break ;
// 05.apr.2002 - end
# endif  // of INSTANCE
// 05.apr.2002 - begin
            case iC_L_DetailNotFinishedBegTotal1 :  // 172
              pJCD -> jCL.detailNotFinishedBegTotal1 += iV ;
              break ;
            case iC_L_DetailNotFinishedEndTotal1 :  // 176
              pJCD -> jCL.detailNotFinishedEndTotal1 += iV ;
              break ;
// 05.apr.2002 - end

            case iC_S_Fixeds2 :                     // 19
              pJCD -> jCS.fixeds2 += iV ;
              break ;
            case iC_S_Lefts2 :                      // 21
              pJCD -> jCS.lefts2 += iV ;
              break ;
            case iC_S_OtherActs2 :                  // 22
              pJCD -> jCS.otherActs2 += iV ;
              break ;
            case iC_S_Lefts1stSess2 :               // 43
              pJCD -> jCS.lefts1stSess2 += iV ;
              break ;
            case iC_S_Lefts2ndSess2 :               // 45
	      pJCD -> jCS.lefts2ndSess2 += iV ;
              break ;
            case iC_S_Stoppeds2 :                   // 47
              pJCD -> jCS.stoppeds2 += iV ;
              break ;
            case iC_S_AbolishedResults2 :           // 67
              pJCD -> jCS.abolishedResults2 += iV ;
              break ;
           }  // -- 4 --  switch
	   }  // -- 3 --
# endif
	 }  // -- 1a --
     }  // -- 1 --  for
   }  // -- 0b --
 }

void TJudgeCheckupThread::incrementValues (TQuery & q, unsigned int dataCiph
# if INSTANCE
	, unsigned int dataCiph2nd
# endif
										    ,
	const long int incrementWith)
 {
  const long int iV = incrementWith ;
  LawPtr pLaw;  // 11.dec.2003  // Performance issue

  while (q.Read ())
   {  // -- 0 --
//    TLawsuit l ;  // 11.dec.2003  // Performance issue

    pLaw.Ref() << q; // It does not matter from which table has been got F_JUDGE
    incrementValues_Law (pLaw.Ref(), dataCiph,
# if INSTANCE
			 dataCiph2nd,
# endif
			 iV) ;
   }  // -- 0 --
 }

// 20061212 -->
void TJudgeCheckupThread::lawsDistribute_RJ (unsigned int dataCiph,	//  1
# if INSTANCE
	unsigned int dataCiph2nd,
# endif
	const bookAreas * bkAreas,			//  2
	int areasCnt,					//  3
	const CDate * pHighRange,			//  4
	const CDate * pLowRange,			// = NULL,	//  5
	const char * resultsString,			// = NULL,	//  6
	const char * requestKindsString,		// = NULL,	//  7
	const char * ageString,				// = NULL,	//  8
	const bool finishedTest,			// = true,	//  9
	const char * sourceTypesString,		// = NULL,	// 10
	const char * grantedsString,			// = NULL,	// 11
	const char * sessKindsString,			// = NULL,	// 12
	const bool sKMatch,				// = false,	// 13
	const bool excludeCreatedsLater,		// = false,	// 14
	const bool rSMatch,				// = true,	// 15
	const char * const decResultsString,	// = NULL,	// 16
	const char * const lawKindsString,		// = NULL,	// 17
	const bool lKSMatch,				// = true,	// 18
	const senderChecksActions aboutTheSender,	// = IgnoreSender,	// 19
	const bool requestCurrentlyQuick,		// = false,	// 20
	const long int incrementWith,			// = 1	// 21

	const char * connectTypesString,		// = NULL	// 22
	const bool lConnSMatch)				// = true	// 23
{
  if (columnRequired (dataCiph, bkAreas, areasCnt)
  # if INSTANCE
						   ||
	columnRequired (dataCiph2nd, bkAreas, areasCnt)
  # endif
     )
  {
    Tick(false);
    if(wtdl->WantBreak())
	return;
    if (decResultsString == NULL ) {}
    if (lKSMatch) {}

    CharPtr crit (512 + 256 + 64 + 64 + 220) ;  //  + 220  24.sep.2003

    const char * const what =
        (resultsString || sessKindsString || requestCurrentlyQuick
        ) ?
          "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_SUBJECT, S.F_JUDGE" :
          ( (dataCiph == iC_L_NewsRejudge1_J) || (dataCiph == iC_L_byJurisd_J) || (dataCiph == iC_L_returneds_J)) ?
               "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_SUBJECT, L.F_REJUDGE AS F_JUDGE" :
               "DISTINCT F_NO, F_YEAR, F_TYPE, F_SUBJECT, F_JUDGE" ;

    if (    (dataCiph == iC_L_NewsRejudge1_J)
         || (dataCiph == iC_L_byJurisd_J)
         || (dataCiph == iC_L_returneds_J) )
    {
       if ( crit &&
            composeReportLawsConnectCriteria
			       (crit, crit.Size (),     // char * dest, size_t destSz,
                                what,                   // const char * what,
                                bMYEMYTCIU.types,       // const char * typesString,
                                bMYEMYTCIU.composition, // const long int composition,
                                pHighRange,             // const CDate * pHighRange,
                                pLowRange,              // const CDate * pLowRange = NULL,
					  sourceTypesString,      // const char * sourceTypesString = NULL,
                                lawKindsString,         // const char * const lawKindsString = NULL,
                                true,                   // const bool lKSMatch = true
                                connectTypesString,     // const char * connectTypesString = NULL,
                                lConnSMatch             // const bool lConnSMatch = true
                               )
           )
        {
	    TQuery q (crit) ;

		incrementValues (q, dataCiph
		# if INSTANCE
					, dataCiph2nd
		# endif
					, incrementWith
				     ) ;
	} //if (crit &&
    }
    else
    {   if (crit &&
          composeReportLawsCriteria (crit, crit.Size (), what,
                                     bMYEMYTCIU.types,
                                     bMYEMYTCIU.composition,
                                     pHighRange, pLowRange,
                                     resultsString,
                                     requestKindsString,
                                     ageString,  // pAgeChar,
                                     finishedTest,
                                     sourceTypesString,
                                     grantedsString,
				     sessKindsString,
                                     sKMatch,
                                     excludeCreatedsLater,
                                     rSMatch,
                                     NULL, //decResultsString,
                                     lawKindsString,
                                     true, //lKSMatch,
                                     aboutTheSender,
						 requestCurrentlyQuick  // 24.sep.2003
                                    ))
	  {
            TQuery q (crit) ;

            incrementValues (q, dataCiph
	    # if INSTANCE
                              , dataCiph2nd
            # endif
                              , incrementWith
                             ) ;
        } //if (crit &&
    } // else

    Tick(true);                                          // Gossip service
  }  // if (columnRequired
}

void TJudgeCheckupThread::sessDistribute_RJ(unsigned int dataCiph,          //  1
# if INSTANCE
	unsigned int dataCiph2nd,
# endif
	const bookAreas * bkAreas,                 //  2
	int areasCnt,                              //  3
	const CDate * pHighRange,                //  4
	const CDate * pLowRange,     // = NULL,  //  5
	const char * resultsString,    // = NULL,  //  6
	const char * sessKindsString,  // = NULL,  //  7
	const bool sKSMatch,           // = false, //  8
	const bool excludeClosedSess,            // = true,  //  9
	const bool beforeLawFinish)               // = true   // 10
 {
  if (columnRequired (dataCiph, bkAreas, areasCnt)
# if INSTANCE
								   ||
	columnRequired (dataCiph2nd, bkAreas, areasCnt)
# endif
      )
  {
		Tick(false);
    if(wtdl->WantBreak())
	return;
    {
      CharPtr crit (512 + 256 + 64 + 64) ;
	const bool sessFullKeyNeed = false ; //@@@
      //  (dataCiph == iC_S_PuniGen_ReqSurround_CS_Def ||
      //   dataCiph == iC_S_PuniGen_ReqSurround_CS ||
      //   dataCiph == iC_S_PuniGen_ReqSurround) ;  // 18.apr.2001
      const char * const what =
        sessFullKeyNeed ?  // 18.apr.2001 - begin
        "L.F_SUBJECT, L.F_KIND AS LAW_KIND, "
        "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_DATE, S.F_KIND, S.F_JUDGE"
        :  // 18.apr.2001 - end
	"L.F_SUBJECT, S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE" ;

      if (composeReportSessionsCriteria (crit, crit.Size (),
                                         what,
                                         bMYEMYTCIU.types,
                                         bMYEMYTCIU.composition,
                                         pHighRange, pLowRange,
                                         resultsString,
                                         sessKindsString,
						     sKSMatch,
                                         excludeClosedSess,
                                         beforeLawFinish))
       {
        TQuery q (crit) ;

//        if (dataCiph == iC_S_Lefts_BeforeFirstSess_J)
//            ofstream("00000001.tmp") << crit;


	int incrementWith = 1; //@@@
	incrementValues (q, dataCiph
# if INSTANCE
				  , dataCiph2nd
# endif
					       , incrementWith
			  ) ;
       }
     }
			Tick(true);                                        // Gossip service
   }
}

// 20061212 <--

// 20070202 -->

# if INSTANCE
# else       // REGIONAL COURT only  -->
void TJudgeCheckupThread::appealedLawsDistribute_RJ(const unsigned int dataCiph, const bookAreas * bkAreas,
	const int areasCnt, const CDate highRange, const CDate lowRange)
{
  if (columnRequired (dataCiph, bkAreas, areasCnt))
   {  // -- -2 --
		Tick(false);                                         // Gossip service
    if(wtdl->WantBreak())
	return;
     {  // -- -1 --
	bool basicOK ;
	CharPtr crt (1024) ;

	crt [0] = crt [crt.Size () - 1] = '\0' ;
	 {  // -- 0 --
	  const char * const what =
	  "DISTINCT I.F_LAWSUIT_NO, I.F_LAWSUIT_YEAR, I.F_LAWSUIT_TYPE, I.F_JUDGE " ;
	  ostrstream oss (crt, crt.Size () - 1) ;

	  // 20070208 ->
	  // basicOK = appealed_laws_basic_criteria (oss, bMYEMYTCIU.types,
        //                                         lowRange, highRange,
        //                                         what,
        //                                         false  // 10.apr.2001
        //                                        ) ;
        basicOK = appealed_laws_basic_with_composition
                                              ( oss, bMYEMYTCIU.types,
                                                lowRange, highRange,
                                                bMYEMYTCIU.composition,
                                                what,
						false  // 10.apr.2001
							     ) ;
        // 20070208 <-

        if (basicOK)
         {

          // 20070208 --> add composition in the select

          if ( dataCiph == iC_L_Appealeds_RC_J )
          {
            oss << " AND I.F_KIND IN ( "
                << stringToSQLCharSet (KIND_PRIVATE_COMPL_REPORT_DESIGNATION).getTheSet ()
                << ") " ;
	  }

          else
          {
            oss << " AND "
              "("
              "(I.F_LAWSUIT_TYPE = '" << TYPE_PUNISHMENT_LAW << "' AND "
              "I.F_KIND IN ('" << KIND_INREG_2ND_COMPLAINT << "','"
                                << KIND_INREG_CONCERN_COMPLAINT << "','"
                                << KIND_INREG_OBJECTION << "')"
              ") OR "
		  "(I.F_LAWSUIT_TYPE IN ('" << TYPE_CITIZEN_LAW << "','" << TYPE_ADMIN_LAW << "','"
						     << TYPE_MARRIAGE_LAW << "') AND "
		  "I.F_KIND = '" << KIND_INREG_2ND_COMPLAINT << "'"
		")"
		  ") " ;
          }
          oss << " ;" << ends ;
          basicOK = (crt [0] && crt [strlen (crt) - 1] == ';') ;
         }
       }  // -- 0 --
      if (basicOK)
       {  // -- 1 --
        TQuery q (crt) ;

        // ofstream("00000010.tmp") << crt;

        int incrementWith = 1; //@@@
	incrementValues (q, dataCiph
                              , incrementWith
                             ) ;


	 }  // -- 1 --
      else
        error ("%s", "Cannot compose the criteria for the "
			   "TJudgeCheckupThread::appealedLawsDistribute_RJ") ;
     }  // -- -1 --
		Tick(true);                                          // Gossip service
   }  // -- -2 --
 }
# endif

// 20070202 <--

void TJudgeCheckupThread::lawsSessDistribute(unsigned int dataCiph,
# if INSTANCE
	unsigned int dataCiph2nd,
# endif
	const bookAreas * bkAreas,
	int areasCnt,
	const CDate * pHighRange,
	const CDate * pLowRange,
	const char * const resultsString,
	const char * const requestKindsString,
	const char * const ageString,
	const bool finishedTest,
	const char * const sourceTypesString,
	const char * const grantedsString,
	const char * const sessKindsString,
	const bool sKMatch,
	const bool excludeCreatedsLater,
	const bool rSMatch,
	const bool sessDateTest,
	const bool bindSessDateAndFinished,
	const char * const decisResultsString,
	const bool withRequestKindsString,
	const char * const lawKindsString,
	const bool lKMatch,
	const long int incrementWith)
 {
  if (columnRequired (dataCiph, bkAreas, areasCnt)
# if INSTANCE
								   ||
	columnRequired (dataCiph2nd, bkAreas, areasCnt)
# endif
     )
   {
    Tick(false);
    if(wtdl->WantBreak())
	return;
    const size_t crSz = 512 + 256 + 3 * 64
     + 128  // 20.jul.2004
     ;
    char * crit = new char [crSz] ;
    const bool countSessions = (dataCiph == iC_S_Fixeds1 ||
                                dataCiph == iC_S_OtherActs1 ||
                                dataCiph == iC_S_Lefts1stSess1 ||
                                dataCiph == iC_S_Lefts2ndSess1 ||
                                dataCiph == iC_S_Stoppeds1 ||
				dataCiph == iC_S_Lefts1
# if INSTANCE
                                                        ||
                                dataCiph == iC_S_Fixeds2 ||
                                dataCiph == iC_S_OtherActs2 ||
                                dataCiph == iC_S_Lefts1stSess2 ||
					  dataCiph == iC_S_Lefts2ndSess2 ||
                                dataCiph == iC_S_Stoppeds2 ||
                                dataCiph == iC_S_Lefts2
# endif
                               ) ;
    const bool subjectNeed = (dataCiph == iC_L_Subject0210 ||
                                    dataCiph == iC_L_Subject0600 ||
                                    dataCiph == iC_L_Subject0500 ||
				    dataCiph == iC_L_Subject0810 ||
                                    dataCiph == iC_L_Subjects0400_0410 ||
                                  dataCiph == iC_L_Subjects0220_0310_0800
# if INSTANCE
                                    ||
                                    dataCiph == iC_L_ExactComplaints2 ||
                                    dataCiph == iC_L_PrivateComplaints2
#   if APPEAL
                                    ||
                                    reqKindsString ()
#   endif
# else
                                    ||
                                    reqKindsString ()
# endif
                             ) ;
    const bool judgeFromTLawsuit = (dataCiph == iC_L_NotFinishedsBeg1 ||
                                    dataCiph == iC_L_News1 ||
      dataCiph == iC_L_NewsRejudge1 ||  // 11.dec.2003
      dataCiph == iC_L_NewsPunGenChar1 ||  // 10.jul.2002
	dataCiph == iC_L_NewsPunGenCharRejudge1 ||  // 11.dec.2003
																		dataCiph == iC_L_NotFinishedsEnd1 ||
			dataCiph == iC_L_DetailNotFinishedBegTotal1 ||  // 13.jun.2002
			dataCiph == iC_L_DetailNotFinishedEndTotal1     // 13.jun.2002


# if INSTANCE
																																			||
																		dataCiph == iC_L_NotFinishedsBeg2 ||
																		dataCiph == iC_L_News2 ||
      dataCiph == iC_L_NewsRejudge2 ||  // 11.dec.2003
      dataCiph == iC_L_NewsPunGenChar2 ||  // 10.jul.2002
      dataCiph == iC_L_NewsPunGenCharRejudge2 ||  // 11.dec.2003
																		dataCiph == iC_L_NotFinishedsEnd2 ||
			dataCiph == iC_L_DetailNotFinishedBegTotal2 ||  // 13.jun.2002
			dataCiph == iC_L_DetailNotFinBegPunGen2 ||  // 13.jun.2002
			dataCiph == iC_L_DetailNotFinBegPunPrv2 ||  // 13.jun.2002
			dataCiph == iC_L_DetailNotFinishedEndTotal2 ||  // 13.jun.2002
			dataCiph == iC_L_DetailNotFinEndPunGen2 ||  // 13.jun.2002
			dataCiph == iC_L_DetailNotFinEndPunPrv2     // 13.jun.2002

# endif
# if ! APPEAL
                                 ||   subjectNeed
# endif  // ! APPEAL
                                   ) ;
//
#if 0 // ------------------- 11.dec.2003  An (awkward) attempt to make things more readeble
//
    const char * const what = countSessions ?
# if INSTANCE
#   if APPEAL
      (subjectNeed ?
	   "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND, L.F_SUBJECT" :
         "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND")
#   else  // of APPEAL
      "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND"
#   endif  // of APPEAL
# else  // of INSTANCE
      "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE"
# endif  // of INSTANCE
                               : (judgeFromTLawsuit ?
# if INSTANCE
                                    (subjectNeed ?
	"DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE"
                                                 ", L.F_SUBJECT" :
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE"
                                    ) :
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, S.F_JUDGE"
#   if APPEAL
								 ", L.F_SUBJECT"
#   endif  // of APPEAL
# else  // of INSTANCE
                                    (subjectNeed ?
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_JUDGE, L.F_SUBJECT" :
	"DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_JUDGE"
                                    ) :
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, S.F_JUDGE"
# endif  // of INSTANCE
                                 ) ;
//
#endif   // of 0  11.dec.2003  An (awkward) attempt to make things more readeble
// ---------------------------------------------------------------------
//
//  11.dec.2003 - begin  An (awkward) attempt to make things more readeble
    const bool wantRejudge = (dataCiph == iC_L_NewsRejudge1 ||
	dataCiph == iC_L_NewsPunGenCharRejudge1  // ||  // 16.mar.2004
#if INSTANCE
      ||  // 16.mar.2004
      dataCiph == iC_L_NewsRejudge2 ||
	dataCiph == iC_L_NewsPunGenCharRejudge2
#endif   // of INSTANCE
      );
    const char* what;

#if INSTANCE  // =============================================================
#  if APPEAL  // -------------------------------------------------------------
#    if 0
    const char * const what = countSessions ?
      (subjectNeed ?
         "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND, L.F_SUBJECT" :  // 1
         "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND")  // 2
                               : (judgeFromTLawsuit ?
				    (subjectNeed ?
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE, "
                                                   "L.F_SUBJECT" :  // 3
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE"  // 4
                                    ) :
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, S.F_JUDGE, "
                                                   "L.F_SUBJECT"  // 5
                                 ) ;
#    else  // of 0
    if(countSessions)
    {
      if(subjectNeed)
      {
        what = "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND, "
	  "L.F_SUBJECT";  // 1
      }
      else
      {
      	what = "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND";  // 2
      }
    }
    else
    {
      if(judgeFromTLawsuit)
      {
        if(subjectNeed)
        {
          if(wantRejudge)
	  {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, "
              "L.F_REJUDGE AS F_JUDGE, "
              "L.F_SUBJECT";  // 3 additional
          }
	    else
          {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE, "
              "L.F_SUBJECT";  // 3
          }
        }
        else
        {
          if(wantRejudge)
	  {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, "
              "L.F_REJUDGE AS F_JUDGE";  // 4  additional
          }
          else
          {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, "
              "L.F_JUDGE";  // 4
          }
        }
      }
      else
      {
        what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, S.F_JUDGE, "
	  "L.F_SUBJECT";  // 5
      }
    }
#    endif  // of 0
#  else  // of APPEAL --------------------------------------------------------
#    if 0
    const char * const what = countSessions ?
      "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND"  // 1
                               : (judgeFromTLawsuit ?
                                    (subjectNeed ?
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE"
						 ", L.F_SUBJECT" :  // 2
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE"  // 3
                                    ) :
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, S.F_JUDGE"  // 4
                                 ) ;
#    else  // of 0
    if(countSessions)
    {
      what = "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE, L.F_KIND"
      ;  // 1
    }
    else
    {
	if(judgeFromTLawsuit)
	{
	  if(subjectNeed)
	  {
	    if(wantRejudge)
	    {
	    what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, "
		  "L.F_REJUDGE AS F_JUDGE, "
		  "L.F_SUBJECT";  // 2 - additional
	    }
	    else
	    {
		what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE, "
		  "L.F_SUBJECT";  // 2
	    }
	  }
	  else
	  {
	    if(wantRejudge)
          {
	    what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, "
              "L.F_REJUDGE AS F_JUDGE"
            ;  // 3 - additional
	    }
          else
	    {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE"
		;  // 3
	  }
        }
	}
      else
      {
        what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, S.F_JUDGE"
        ;  // 4
      }
    }
#    endif  // of 0
#  endif  // of APPEAL -------------------------------------------------------
#else  // of INSTANCE ========================================================
#    if 0
    const char * const what = countSessions ?
      "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE"  // 1
                               : (judgeFromTLawsuit ?
                                    (subjectNeed ?
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_JUDGE, L.F_SUBJECT" :  // 2
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_JUDGE"  // 3
                                    ) :
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, S.F_JUDGE"  // 4
                                 ) ;
#    else  // of 0
    if(countSessions)
    {
      what = "S.F_NO, S.F_YEAR, S.F_TYPE, S.F_JUDGE";  // 1
    }
    else
    {
      if(judgeFromTLawsuit)
      {
        if(subjectNeed)
        {
          if(wantRejudge)
          {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, "
              "L.F_REJUDGE AS F_JUDGE, "
              "L.F_SUBJECT";  // 2 additional
          }
	  else
          {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_JUDGE, "
		  "L.F_SUBJECT";  // 2
          }
        }
        else
        {
          if(wantRejudge)
          {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, "
              "L.F_REJUDGE AS F_JUDGE";  // 3 additional
          }
          else
	  {
            what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_JUDGE";  // 3
          }
        }
      }
      else
      {
        what = "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, S.F_JUDGE";  // 4
      }
    }
#    endif  // of 0
#endif  // of INSTANCE =======================================================
//  11.dec.2003 - end    An (awkward) attempt to make things more readeble

    if (crit && composeJudgeCheckupCriteria (crit, crSz, what,
							   bMYEMYTCIU.types,
							   bMYEMYTCIU.composition,
							   pHighRange, pLowRange,
							   resultsString,
							   requestKindsString,
							   ageString,
							   finishedTest,
							   sourceTypesString,
							   grantedsString,
							   sessKindsString,
							   sKMatch,
							   excludeCreatedsLater,
							   rSMatch, sessDateTest,
					     bindSessDateAndFinished,
							   decisResultsString,
						     withRequestKindsString,
							   lawKindsString, // 08.apr.2002
							   lKMatch,   // 08.apr.2002
				 trueJudgeCheckup ? NULL : & bMYEMYTCIU   // 20.jul.2004
							  ))
     {
	TQuery q (crit) ;

	incrementValues (q, dataCiph
# if INSTANCE
					    , dataCiph2nd
# endif
							     , incrementWith
			    ) ;

	 {  // Debug only
//        if (dataCiph == iC_S_Lefts1)    ofstream("00000009.tmp") << crit;
	 }
     }
    if (crit)
	delete [] crit ;
    Tick(true);                                        // Gossip service
   }
 }

void TJudgeCheckupThread::incrementValuesFixedSess (TQuery & q, unsigned int dataCiph)
 {
  const long int iV = 1 ;

  while (q.Read ())
   {  // -- 0 --
# if INSTANCE
#   if APPEAL
     {
      TSubject subj ;

      subj << q ;
      if (skipThisLaw (subj.subject))
        continue ;
     }
#   endif
# endif

    TSession s ;
    int judgeLoopCounter ;

    s << q ;
    if (skipThisLaw (s.kind))  // s.kind is the lawsuit's kind !!!
      continue ;
    s.kind = '\0' ;
    for (judgeLoopCounter = 0 ; judgeLoopCounter < 3 ;
         judgeLoopCounter ++)
     {  // -- 0a --
      const char * anyJudge ;

      switch (judgeLoopCounter)
       {
        case 0 :
          anyJudge = s.president ;
          break ;
        case 1 :
          anyJudge = s.judgeX ;
          break ;
        case 2 :
          anyJudge = s.reJudgeX ;
          break ;
	 }
      if (anyJudge [0] &&
          ucnPresent (anyJudge)  // atof (anyJudge) > 0.5
         )
       {  // -- 0c --
        addJCDToArrayIfNecessary (anyJudge) ;
         {  // -- 0b --
          bool found = false ;

	    for (int i = 0 ; ! found && i < pArr -> Count () ;
               i ++)
           {  // -- 1 --
            judgeCheckupData * pJCD = (* pArr) [i] ;

            if (! strcmp (pJCD -> judgeUCN, anyJudge))
             {  // -- 1a --
              found = true ;
              switch (dataCiph)
	       {  // -- 2 --                         // for fmt
		    case iC_S_Fixeds1Judge1 :            // 30
                  pJCD -> jCS.fixeds1Judge1 += iV ;
                  break ;
                case iC_S_Fixeds2Judges1 :           // 31
                  pJCD -> jCS.fixeds2Judges1 += iV ;
                  break ;
                case iC_S_Fixeds3Judges1 :           // 32
                  pJCD -> jCS.fixeds3Judges1 += iV ;
                  break ;
                case iC_S_Fixeds3Judges2 :           // 33
# if INSTANCE
                  pJCD -> jCS.fixeds3Judges2 += iV ;
# endif
		  break ;
                case iC_S_FixedsIfNotJudge1 :        // 48
                  if (strcmp (s.judge, anyJudge))
                    pJCD -> jCS.fixedsIfNotJudge1 += iV ;
                  break ;
                case iC_S_FixedsIfNotJudge3_2 :        // 49
# if INSTANCE
                  if (strcmp (s.judge, anyJudge))
                    pJCD -> jCS.fixedsIfNotJudge3_2 += iV ;
# endif
                  break ;
                case iC_S_Lefts3Judges1 :            // 60
                  pJCD -> jCS.lefts3Judges1 += iV ;
                  break ;
		case iC_S_Lefts3Judges2 :            // 61
# if INSTANCE
                  pJCD -> jCS.lefts3Judges2 += iV ;
# endif
                  break ;
                case iC_S_Lefts3Judges1stSess1 :     // 62
                  pJCD -> jCS.lefts3Judges1stSess1 += iV ;
                  break ;
                case iC_S_Lefts3Judges1stSess2 :     // 63
# if INSTANCE
                  pJCD -> jCS.lefts3Judges1stSess2 += iV ;
# endif
                  break ;
                case iC_S_Lefts3Judges2ndSess1 :     // 62
		  pJCD -> jCS.lefts3Judges2ndSess1 += iV ;
                  break ;
                case iC_S_Lefts3Judges2ndSess2 :     // 63
# if INSTANCE
                  pJCD -> jCS.lefts3Judges2ndSess2 += iV ;
# endif
                  break ;
               }  // -- 2 --  switch
             }  // -- 1a --
           }  // -- 1 --  for
	   }  // -- 0b --
       }  // -- 0c --
     }  // -- 0a --
   }  // -- 0 --
 }

void TJudgeCheckupThread::fixedSessDistribute(unsigned int dataCiph, const bookAreas * bkAreas, const int areasCnt,
	const CDate highRange, const CDate lowRange, const char * const sessKindsString, const bool sKMatch,
	const char * const sessResultsString)
 {
  if (columnRequired (dataCiph, bkAreas, areasCnt))
   {  // -- 0 --
    Tick(false);
    if(wtdl->WantBreak())
	return;
    const size_t crSz = 1024 + 128
     + 128  //20.jul.2004
     ;
    char * crit = new char [crSz] ;
    SessionJudgesNumber howManyJudges = UndefinedNumberOfJudges ;
# if INSTANCE
    bool firstInstLaws = true ;
# endif

    switch (dataCiph)
     {
      case iC_S_Fixeds1Judge1 :
        howManyJudges = OneJudge ;  // 1
        break ;
      case iC_S_Fixeds2Judges1 :
        howManyJudges = TwoJudges ;  // 2
        break ;
	case iC_S_Fixeds3Judges1 :
      case iC_S_Lefts3Judges1 :
      case iC_S_Lefts3Judges1stSess1 :
      case iC_S_Lefts3Judges2ndSess1 :
        howManyJudges = ThreeJudges ;  // 3
	break ;
      case iC_S_Fixeds3Judges2 :
      case iC_S_Lefts3Judges2 :
      case iC_S_Lefts3Judges1stSess2 :
      case iC_S_Lefts3Judges2ndSess2 :
        howManyJudges = ThreeJudges ;  // 3
# if INSTANCE
        firstInstLaws = false ;
# endif
        break ;
      case iC_S_FixedsIfNotJudge1 :
        howManyJudges = JudgeOnly ;  // -1
        break ;
      case iC_S_FixedsIfNotJudge3_2 :
	howManyJudges = JudgeOnly ;  // -1
# if INSTANCE
        firstInstLaws = false ;
# endif
        break ;
     }
    if (composeJdgChkupFixedsCriteria (crit, crSz, howManyJudges,
# if INSTANCE
                                       firstInstLaws,
# endif
                                       bMYEMYTCIU.types,
                                       bMYEMYTCIU.composition,
                                       highRange, lowRange,
                                       sessKindsString, sKMatch,
				       sessResultsString,
			 trueJudgeCheckup ? NULL : & bMYEMYTCIU   // 20.jul.2004
                                      ))
     {
      TQuery q (crit) ;

	incrementValuesFixedSess (q, dataCiph) ;
     }
    if (crit)
	delete [] crit ;
//    Tick(true);                                        // Gossip service
   }  // -- 0 --
 }

const char *TJudgeCheckupThread::reqKindsString()
 {
  return
# if INSTANCE
#   if APPEAL
         trueJudgeCheckup ? NULL : "" ;
#   else  // of APPEAL
         NULL ;
#   endif  // of APPEAL
# else  // of INSTANCE
    bMYEMYTCIU.internalUse ? NULL :
      strchr (bMYEMYTCIU.types, TYPE_PUNISHMENT_LAW) ?
        (trueJudgeCheckup ? "" : NULL) :
        NULL ;
# endif  // INSTANCE
 }

void TJudgeCheckupThread::addOverriddenToLefts()
 {
  LawPtr pLaw;
  LawPtr pTmpL;

  for (int i = 0 ; i < pASRArr -> Count () ; i ++)
   {  // -- 0 --
    if (trueJudgeCheckup == false)
      if (strchr (KIND_CLOSED_SESSIONS, ((* pASRArr) [i]) -> kind))
        continue ;
//    TLawsuit l ;  // 11.dec.2003  Performance issue

    clearGroupData (pLaw.Ref()) ;
    pLaw.Ref() << (* ((* pASRArr) [i])) ;
    pLaw.Ref().kind = '\0' ;

     {  // because of "skipThisLaw" ...
//      TLawsuit tmpL ;  // 11.dec.2003  Performance issue

	pTmpL.Ref() << pLaw.Ref() ;

	if (pTmpL.Ref().Get ())
	 {
	  pLaw.Ref().kind = pTmpL.Ref().kind ;
	  pLaw.Ref().subject = pTmpL.Ref().subject ;
	 }
	else
	pLaw.Ref().subject = pLaw.Ref().kind = '\0' ;
     }


    // 20070202 не трябва да се отчитат ЗЗ с резултат "С отменен ход по същество" в колона "Отложени"
    // ( след резултат в ОЗ "Обявено  за решаване", но без да е вписан самия резултат в T_DECISION)
    //incrementValues_Law (pLaw.Ref(), iC_S_Lefts1    // 25020
    //# if INSTANCE
    //    , iC_S_Lefts2
    //# endif
    //    ) ;
    incrementValues_Law (pLaw.Ref(), iC_S_AbolishedResults1
# if INSTANCE
						   , iC_S_AbolishedResults2
# endif
				) ;
   }  // -- 0 --
 }

void TJudgeCheckupThread::incrementAfterFinishValues (TQuery & q, unsigned int dataCiph,
# if INSTANCE
	unsigned int dataCiph2nd,
# endif
	const long int incrementWith)
 {
  const long int iV = incrementWith ;
  TSession s ;
  int judgeLoopCounter ;

  while (q.Read ())
   {  // -- 0 --
    s << q ;  // sess.kind is the lawsuit's kind

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
    const bool firstInstLaw =
	strchr (KIND_1ST_LAWSUITS, s.kind) ;  // sess.kind is the
							  // lawsuit's kind
#   endif  // APPEAL
# else  // INSTANCE
# endif  // INSTANCE

    if (skipThisLaw (s.kind))  // sess.kind is the lawsuit's kind
      continue ;
    s.kind = '\0' ;

    // 20070507
    if (!atof(s.president))
      strcpy(s.president, s.judge);

    for (judgeLoopCounter = 0 ; judgeLoopCounter < 3 ;
         judgeLoopCounter ++)
     {  // -- 1 --
      const char * anyJudge = NULL ;

      switch (judgeLoopCounter)
       {
        case 0 :
          anyJudge = s.president;
          break ;
        case 1 :
          anyJudge = s.judgeX ;
          break ;
        case 2 :
	    anyJudge = s.reJudgeX ;
          break ;
       }


      if (anyJudge)
        if (dataCiph == iC_S_AdditionalDecisDefinOpenS1 ||
	    dataCiph == iC_S_AdditionalDecisDefinCldsS1
# if INSTANCE
                                                        ||
#   if APPEAL
            dataCiph ==  iC_S_Appeal192_193sessRes2 ||
#   endif  // APPEAL
            dataCiph2nd == iC_S_AdditionalDecisDefinOpenS2 ||
            dataCiph2nd == iC_S_AdditionalDecisDefinCldsS2
# endif
           )
           if (strcmp (anyJudge, s.judge))
               anyJudge = NULL ;

      if (anyJudge && anyJudge [0] &&
	  ucnPresent (anyJudge)  // atof (anyJudge) > 0.5
         )
       {  // -- 2 --
	addJCDToArrayIfNecessary (anyJudge) ;
         {  // -- 3 --

          bool found = false ;

	    for (int i = 0 ; ! found && i < pArr -> Count () ;
               i ++)
           {  // -- 4 --
            judgeCheckupData * pJCD = (* pArr) [i] ;

            if (! strcmp (pJCD -> judgeUCN, anyJudge))
	     {  // -- 5 --
              found = true ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
              if (firstInstLaw)
#   endif  // APPEAL
# endif  // INSTANCE
                switch (dataCiph)
                 {  // -- 6 --
                  case iC_S_AdditionalDecisDefinOpenS1 :
		  pJCD -> jCS.additionalDecisDefinOpenS1 += iV ;
                  break ;
		  case iC_S_AdditionalDecisDefinCldsS1 :
		  pJCD -> jCS.additionalDecisDefinCldsS1 += iV ;   // 2007
                  break ;
                 }  // -- 6 --  switch
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
              else
#   endif  // APPEAL
                switch (dataCiph2nd)
                 {  // -- 7 --
                  case iC_S_AdditionalDecisDefinOpenS2 :
                    pJCD -> jCS.additionalDecisDefinOpenS2 += iV ;
                    break ;
                  case iC_S_AdditionalDecisDefinCldsS2 :
                    pJCD -> jCS.additionalDecisDefinCldsS2 += iV ;
                    break ;
#   if APPEAL
		 case iC_S_Appeal192_193sessRes2 :       // 86
                   pJCD -> jCS.appeal192_193sessRes2 += iV ;
                   break ;
#   endif  // APPEAL
                 }  // -- 7 --  switch
# endif
             }  // -- 5 --
           }  // -- 4 --  for
	   }  // -- 3 --
       }  // -- 2 --
     }  // -- 1 --
   }  // -- 0 --
 }

void TJudgeCheckupThread::afterFinishDistribute (unsigned int dataCiph,
# if INSTANCE
	unsigned int dataCiph2nd,
# endif
	const bookAreas * bkAreas,
	int areasCnt,
	const CDate highRange,
	const CDate lowRange,
	const char * const sessKindsString,
	const bool sKMatch,
	const bool conformWithSessDate,
	const bool exscludeResDecree,
	const char * const decisKindsString,
	const long int incrementWith,
	const char * const sessResStr,
	const bool sRSMatch)
 {
  if (columnRequired (dataCiph, bkAreas, areasCnt)
# if INSTANCE
                                                   ||
      columnRequired (dataCiph2nd, bkAreas, areasCnt)
# endif
     )
   {
    Tick(false);
    if(wtdl->WantBreak())
	return;
    SessionJudgesNumber howManyJudges = UndefinedNumberOfJudges ;

    switch (dataCiph)
     {
      iC_S_AdditionalDecisDefinOpenS1 :
      iC_S_AdditionalDecisDefinOpenS2 :
      iC_S_AdditionalDecisDefinCldsS1 :
      iC_S_AdditionalDecisDefinCldsS2 :
      iC_S_Appeal192_193sessRes2 :
	howManyJudges = JudgeOnly ;
	break ;
     }

    const size_t crSz = 512 + 256 + 3 * 64  + 128 + 64 ;
    char * crit = new char [crSz] ;

    if (composeJdgChkupAfterFinCriteria (crit, crSz,
					 howManyJudges,
					 bMYEMYTCIU.types,
					 bMYEMYTCIU.composition,
					 highRange, lowRange,
					 sessKindsString, sKMatch,
					 conformWithSessDate,
					 exscludeResDecree,
					 decisKindsString,
					 sessResStr, sRSMatch))
     {
	TQuery q (crit) ;

      incrementAfterFinishValues (q, dataCiph,
# if INSTANCE
                                  dataCiph2nd,
# endif
                                  incrementWith) ;
     }
    delete [] crit ;
//    Tick(true);                                        // Gossip service
   }
 }

# if INSTANCE
#   if APPEAL
void TJudgeCheckupThread::specificAplCourtNewsDistribute(const bookAreas * bkAreas, int areasCnt, const CDate * pHighRange,
	const CDate * pLowRange, const char * const defaultRequestKindsString)
 {
  if (columnRequired (iC_L_NewsPGenCharAplCourt, bkAreas, areasCnt) ||
      columnRequired (iC_L_NewsPKeepOpenSessAplCourt, bkAreas,areasCnt) ||
      columnRequired (iC_L_NewsPKeepClSessAplCourt, bkAreas, areasCnt) ||
      columnRequired (iC_L_NewsPEnactmentAplCourt, bkAreas, areasCnt) ||
      columnRequired (iC_L_NewsPOthersAplCourt, bkAreas, areasCnt)
     )
   {  // -- 0 --
    Tick(false);
    if(wtdl->WantBreak())
	return;
    const size_t crSz = 512 + 256 + 3 * 64 ;
    char * crit = new char [crSz] ;
    const char * const what =
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_JUDGE, "
      "L.F_SUBJECT, L.F_SOURCE_KIND, L.F_KIND" ;

    if (crit && composeJudgeCheckupCriteria (crit, crSz, what,
                                             bMYEMYTCIU.types,
                                             bMYEMYTCIU.composition,
                                             pHighRange, pLowRange,
                                             NULL,
                                             defaultRequestKindsString,
                                             NULL, false))
     {  // -- 1 --
      TQuery q (crit) ;
      TLawsuit law ;

	while (q.Read ())
	 {  // -- 2 --
	  law << q ;

	  if (law.subject == SUBJECT_9002 || law.subject == SUBJECT_9003)
	   {  // iC_L_NewsPKeepOpenSessAplCourt
	    incrementValues_Law (law, iC_L_NewsPKeepOpenSessAplCourt,
					 iC_L_NewsPKeepOpenSessAplCourt) ;
	    continue ;
	   }
	  if (law.subject == SUBJECT_9001)
	   {  // iC_L_NewsPKeepClSessAplCourt
	    incrementValues_Law (law, iC_L_NewsPKeepClSessAplCourt,
					 iC_L_NewsPKeepClSessAplCourt) ;
	    continue ;
	   }
	  if (law.sourceKind == KIND_CANCEL_ENACTMENT)
	   {  // iC_L_NewsPEnactmentAplCourt
	    incrementValues_Law (law, iC_L_NewsPEnactmentAplCourt,
					 iC_L_NewsPEnactmentAplCourt) ;
	    continue ;
	   }
	  if (law.kind == KIND_PUNI_GENERAL_CHAR_LAW_2A)
	   {  // iC_L_NewsPGenCharAplCourt
	    incrementValues_Law (law, iC_L_NewsPGenCharAplCourt,
					 iC_L_NewsPGenCharAplCourt) ;
	    continue ;
	   }
	  // Any other cases
	   {  // iC_L_NewsPOthersAplCourt
	    incrementValues_Law (law, iC_L_NewsPOthersAplCourt,
					 iC_L_NewsPOthersAplCourt) ;
	    continue ;
	   }
	 }  // -- 2 --  // while
     }  // -- 1 --
    if (crit)
      delete [] crit ;
//    Tick(true);                                        // Gossip service
   }  // -- 0 --
 }

void TJudgeCheckupThread::specificAplCourtDecDistribute (const bookAreas * bkAreas, int areasCnt, const CDate * pHighRange,
	const CDate * pLowRange, const char * const defaultRequestKindsString)
 {
  if (columnRequired (iC_L_DecPGenCharAplCourt, bkAreas, areasCnt) ||
      columnRequired (iC_L_DecPKeepOpenSessAplCourt, bkAreas, areasCnt) ||
	columnRequired (iC_L_DecPKeepClSessAplCourt, bkAreas, areasCnt) ||
      columnRequired (iC_L_DecPEnactmentAplCourt, bkAreas, areasCnt) ||
      columnRequired (iC_L_DecPOthersAplCourt, bkAreas, areasCnt)
     )
   {  // -- 0 --
    Tick(false);
    if(wtdl->WantBreak())
	return;
    const size_t crSz = 512 + 256 + 3 * 64 ;
    char * crit = new char [crSz] ;
    const char * const what =
      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, "
      "S.F_JUDGE, "  // !!!
      //"S.F_KIND AS SESS_KIND, "  // 20.jul.2004 never used (possible way only)
      "L.F_SUBJECT, L.F_SOURCE_KIND, L.F_KIND" ;

    if (crit && composeJudgeCheckupCriteria (crit, crSz, what,
                                             bMYEMYTCIU.types,
                                             bMYEMYTCIU.composition,
                                             pHighRange, pLowRange,
							   RESULT_COMPLETIZEDS,
                                             defaultRequestKindsString,
                                             NULL, true, NULL, NULL,
                        trueJudgeCheckup ? NULL : KIND_CLOSED_SESSIONS))
     {  // -- 1 --
      TQuery q (crit) ;
      TLawsuit law ;

	while (q.Read ())
	 {  // -- 2 --
	  law << q ;

	  if (law.subject == SUBJECT_9002 || law.subject == SUBJECT_9003)
	   {  // iC_L_DecPKeepOpenSessAplCourt
	    incrementValues_Law (law, iC_L_DecPKeepOpenSessAplCourt,
					 iC_L_DecPKeepOpenSessAplCourt) ;
          continue ;
         }
        if (law.subject == SUBJECT_9001)
         {  // iC_L_DecPKeepClSessAplCourt
          incrementValues_Law (law, iC_L_DecPKeepClSessAplCourt,
                               iC_L_DecPKeepClSessAplCourt) ;
          continue ;
         }
        if (law.sourceKind == KIND_CANCEL_ENACTMENT)
         {  // iC_L_DecPEnactmentAplCourt
          incrementValues_Law (law, iC_L_DecPEnactmentAplCourt,
                               iC_L_DecPEnactmentAplCourt) ;
          continue ;
         }
        if (law.kind == KIND_PUNI_GENERAL_CHAR_LAW_2A)
         {  // iC_L_DecPGenCharAplCourt
          incrementValues_Law (law, iC_L_DecPGenCharAplCourt,
                               iC_L_DecPGenCharAplCourt) ;
          continue ;
         }
        // Any other cases
         {  // iC_L_DecPOthersAplCourt
          incrementValues_Law (law, iC_L_DecPOthersAplCourt,
                               iC_L_DecPOthersAplCourt) ;
          continue ;
         }
       }  // -- 2 --  // while
     }  // -- 1 --
    if (crit)
      delete [] crit ;
    Tick(true);                                        // Gossip service
   }  // -- 0 --
 }
#   endif  // APPEAL
# endif  // INSTANCE

#if USE_JUDGE_CHECKUP_OTHERS
void TJudgeCheckupThread::incrementReqResolutionValues(TRequestResolution& rReqResol,
  unsigned int dataCiph,
# if INSTANCE
  unsigned int dataCiph2nd,
# endif
  TRequest& rReq, TLawsuit& rLaw, const long int incrementWith)
{
  long int lawSubj = 0;
  char lawKind = '\0';
  // -----
  long int lawComposition = 0;

  rReq << rReqResol;  // F_KIND is from T_REQUEST
  if((bMYEMYTCIU.lArrPtr()->Count() > 0 ||
    bMYEMYTCIU.composition > 0) &&
    TRCDKey_(rReq.key).FilledOK())
  {  // lawSubj or lawComposition is required
    if(requestColumns::tryToLawsuit(rReq, rLaw))
    {
      lawSubj = rLaw.subject;
      lawKind = rLaw.kind;
      lawComposition = rLaw.composition;
    }
  }
	// lawKind is required anyway...
	if(//trueJudgeCheckup == false &&
		lawKind == '\0' && rReq.kind != '\0')
	{
		lawKind = rReq.lawKind;
	}

  if(skipThisLaw(lawSubj))
    return;
  if(skipThisLaw(lawKind))
    return;
  if(bMYEMYTCIU.composition > 0 && bMYEMYTCIU.composition != lawComposition)
    return;

  addJCDToArrayIfNecessary(rReqResol.judge);
  {  // -- 0 --
    bool found = false;

    for(int i = 0; found == false && i < pArr->Count(); i++)
    {  // -- 1 --
      judgeCheckupData* pJCD = (*pArr)[i];

      if(strcmp(pJCD->judgeUCN, rReqResol.judge) == 0)
      {  // -- 2 --
        found = true ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
        const bool firstInstLaw =
          lawKind && strchr(KIND_1ST_LAWSUITS, lawKind);

        if(firstInstLaw)
#   endif  // of APPEAL
# endif
        {  // -- 3 --
          // It is not fatal that this is compiled for APPEAL (will probaly
          // increment something, if these codes appear in the format file.
          // But the program won't crash - I hope).
          switch (dataCiph)
          {  // -- 4 --                         // for fmt
            case iC_Oth_ReqResolutions1:            // 196
              pJCD->jCO.reqResolutions1 += incrementWith;
              break ;
          }  // -- 4 --  switch
        }  // -- 3 --
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
	  else
#   endif  // of APPEAL
        {  // -- 5 --
          switch (dataCiph2nd)
          {  // -- 6 --                         // for fmt
            case iC_Oth_ReqResolutions2:            // 197
              pJCD->jCO.reqResolutions2 += incrementWith;
              break ;
          }  // -- 6 --  switch
        }  // -- 5 --
# endif  // of INSTANCE
      }  // -- 2 --
    }  // -- 1 --  for
  }  // -- 0 --
}

void TJudgeCheckupThread::reqResolutionsDistribute(unsigned int dataCiph,
# if INSTANCE
  unsigned int dataCiph2nd,
# endif
  const bookAreas* bkAreas, int areasCnt, const CDate highRange,
  const CDate lowRange, const long int incrementWith)
{
  METHOD_NAME("reqResolutionsDistribute");

  if(columnRequired(dataCiph, bkAreas, areasCnt)
# if INSTANCE
    || columnRequired(dataCiph2nd, bkAreas, areasCnt)
# endif
    )
  {
    Tick(false);
    if(wtdl->WantBreak())
      return;

    char lawKinds[SIZE_OF_KINDS];

    if(law_types_to_law_kinds(bMYEMYTCIU.types, lawKinds, sizeof(lawKinds)))
    {
      CharPtr crit(1536);
      bool partCritOk = false;

      SUPPRESS_UNUSED(partCritOk);
      {
        ostrstream oss(crit, crit.Size());

	  oss << "SELECT RR.F_TYPE, RR.F_YEAR, RR.F_NO, RR.F_DATE, RR.F_JUDGE, "
          "R.F_KIND " // R.F_KIND! (performance -- incrementReqResolutionValues)
          "FROM T_REQUEST_RESOLUTION RR, T_REQUEST R "
          "WHERE "
	    "RR.F_DATE >= " << lowRange.Value() << " AND "
          "RR.F_DATE <= " << highRange.Value() << " AND "
          "RR.F_JUDGE != '" << EMPTY << "' AND "
          "RR.F_JUDGE != '" << INTER << "' AND "
          "R.F_TYPE = RR.F_TYPE AND "
          "R.F_YEAR = RR.F_YEAR AND "
          "R.F_NO = RR.F_NO AND "
          ;
        partCritOk = request_by_lawKinds_part_crit (oss, lawKinds, "R.") ;
        oss << " ;" << ends;
      }
      crit.End();
      if(partCritOk &&
        criteria_finished_ok(crit, typeid(*this).name(), methodName))
      {
        RequestResolutionPtr pReqResol;
        LawPtr pLaw;
        TRequest req;
	  TCountRecsQuery q(crit);
        const unsigned recs = q.getRecsCount();
        int r = 0;
	  char textBufferTmp1[0x100];
	  char textBufferTmp2[0x100];

	  wtdl->GetText(textBufferTmp1, sizeof textBufferTmp1);

        // I call WantBreak() in the next row, because the process may be
        // highly time consuming.
	  while(wtdl->WantBreak() == false && q.Read())
	  {
	    pReqResol.Ref() << q;
	    incrementReqResolutionValues(pReqResol.Ref(), dataCiph,
# if INSTANCE
		dataCiph2nd,
# endif
            req, pLaw.Ref(), incrementWith);
          if((r % 5) == 0)                       // Gossip service - begin
          {
            double perc = 100.0;

            if(recs > 0)
            {
              perc *= r;
              perc /= recs;
            }
            ostrstream(textBufferTmp2, sizeof(textBufferTmp2))
              << textBufferTmp1 << " (" << resetiosflags(ios::scientific)
              << setiosflags(ios::fixed | ios::showpoint)
              << setprecision(5) << perc << "% обработени) ..." << ends;
            ENDING_ZERO(textBufferTmp2);
            wtdl->SetText(textBufferTmp2);
          }
          r++;                                    // Gossip service - end
        }
      }
    }
  }
}

void TJudgeCheckupThread::incrementInRegEventValues(TInRegEvent& rIRE,
  unsigned int dataCiph,
# if INSTANCE
  unsigned int dataCiph2nd,
# endif
  TInReg& rIR, TLawsuit& rLaw, TRequest& rReq, const long int incrementWith)
{  // 07.jun.2004
  rIR << rIRE;
  if(TRCDKey_(rIR.key).FilledOK() && rIR.Get())
  {
    const char lawType = inreg_to_law_type(rIR, &(rLaw.key), NULL, NULL, true,
      &(rReq.key));

    if(lawType && strchr(bMYEMYTCIU.types, lawType))
    {
      // ----- These variables are used in skipThislaw() methods. But the
      // gathering of data is time consuming. So, I try simple shecks in order to
	// detect whether the data is necessary (after looking at the code of
      // skipThislaw() methods)
      long int lawSubj = 0;
      char lawKind = '\0';
      // -----
      long int lawComposition = 0;

	if(bMYEMYTCIU.lArrPtr()->Count() > 0 ||
        bMYEMYTCIU.composition > 0)
      {  // lawSubj or lawComposition is required
        if(TRCDKey_(rLaw.key).FilledOK())
        {
          if(rLaw.Get())
          {
            lawSubj = rLaw.subject;
            lawKind = rLaw.kind;
            lawComposition = rLaw.composition;
          }
        }
        else
          if(TRCDKey_(rReq.key).FilledOK())
          {
            if(rReq.Get() && requestColumns::tryToLawsuit(rReq, rLaw))
            {
              lawSubj = rLaw.subject;
              lawKind = rLaw.kind;
              lawComposition = rLaw.composition;
            }
          }
      }
      // lawKind is required anyway...
      if(//trueJudgeCheckup == false &&
        lawKind == '\0'  // still == '\0'
        )
	{
	  if(TRCDKey_(rLaw.key).FilledOK())
	  {
	    if(rLaw.Get())
	    {
		lawKind = rLaw.kind;
	    }
	  }
	  else
		if (TRCDKey_(rReq.key).FilledOK() && rReq.Get("F_LAW_KIND"))
			lawKind = rReq.lawKind;
	}

      if(skipThisLaw(lawSubj))
        return;
      if(skipThisLaw(lawKind))
        return;
      if(bMYEMYTCIU.composition > 0 && bMYEMYTCIU.composition != lawComposition)
        return;

      addJCDToArrayIfNecessary(rIRE.judge);
      {  // -- 0 --
        bool found = false;

	  for(int i = 0; found == false && i < pArr->Count(); i++)
        {  // -- 1 --
          judgeCheckupData* pJCD = (*pArr)[i];

          if(strcmp(pJCD->judgeUCN, rIRE.judge) == 0)
          {  // -- 2 --
            found = true ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
            const bool firstInstLaw =
              lawKind && strchr(KIND_1ST_LAWSUITS, lawKind);

            if(firstInstLaw)
#   endif  // of APPEAL
# endif
            {  // -- 3 --
              // It is not fatal that this is compiled for APPEAL (will probaly
              // increment something, if these codes appear in the format file.
              // But the program won't crash - I hope).
              switch (dataCiph)
              {  // -- 4 --                         // for fmt
                case iC_Oth_InRegEvents1:               // 198
                  pJCD -> jCO.inRegEvents1 += incrementWith;
                  break ;
              }  // -- 4 --  switch
		}  // -- 3 --
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
            else
#   endif  // of APPEAL
            {  // -- 5 --
              switch (dataCiph2nd)
              {  // -- 6 --                         // for fmt
                case iC_Oth_InRegEvents2:               // 199
                  pJCD -> jCO.inRegEvents2 += incrementWith;
                  break ;
              }  // -- 6 --  switch
            }  // -- 5 --
# endif  // of INSTANCE
          }  // -- 2 --
        }  // -- 1 --  for
	}  // -- 0 --
    }
  }
}

void TJudgeCheckupThread::inRegEventSDistribute(unsigned int dataCiph,
# if INSTANCE
  unsigned int dataCiph2nd,
# endif
  const bookAreas* bkAreas, int areasCnt, const CDate highRange,
  const CDate lowRange, const long int incrementWith)
{
  METHOD_NAME("inRegEventSDistribute");

  if(columnRequired(dataCiph, bkAreas, areasCnt)
# if INSTANCE
    || columnRequired(dataCiph2nd, bkAreas, areasCnt)
# endif
    )
  {
    CharPtr crit(512+128);  // +128  05.jan.2005 because of ..._RESOLUTIONS

    Tick(false);
    if(wtdl->WantBreak())
	return;

    {
      ostrstream oss(crit, crit.Size());

	oss << "SELECT F_TYPE, F_YEAR, F_NO, F_JUDGE, F_DATE " // F_FROM_DATE replaced with F_DATE  20051118
	  "FROM T_INREG_EVENT "
        "WHERE "
        "F_DATE >= " << lowRange.Value() << " AND "                // F_FROM_DATE replaced with F_DATE  20051118
        "F_DATE <= " << highRange.Value() << " AND "               // F_FROM_DATE replaced with F_DATE  20051118
        "F_JUDGE != '" << EMPTY << "' AND "
        "F_JUDGE != '" << INTER << "' "
        ;
      oss << " AND F_RESULT IN(";  // 05.jan.2005 - begin
      if((RESULT_INREG_RESOLUTIONS && RESULT_INREG_RESOLUTIONS[0]) ||
        (RESULT_TEXTUAL_RESOLUTIONS && RESULT_TEXTUAL_RESOLUTIONS[0]))
      {
        if(RESULT_INREG_RESOLUTIONS && RESULT_INREG_RESOLUTIONS[0])
        {
          oss << SQLCharSet(RESULT_INREG_RESOLUTIONS);
        }
        if((RESULT_INREG_RESOLUTIONS && RESULT_INREG_RESOLUTIONS[0]) &&
          (RESULT_TEXTUAL_RESOLUTIONS && RESULT_TEXTUAL_RESOLUTIONS[0]))
        {
          oss << ",";
        }
        if(RESULT_TEXTUAL_RESOLUTIONS && RESULT_TEXTUAL_RESOLUTIONS[0])
        {
          oss << SQLCharSet(RESULT_TEXTUAL_RESOLUTIONS);
        }
      }
      else
      {
        oss << STUB_CHAR;
      }
      oss << ") ";  // 05.jan.2005 - end
      oss << " ;" << ends;
    }
    crit.End();
    if(criteria_finished_ok(crit, typeid(*this).name(), methodName))
    {
      InRegEventPtr pIRE;
      TInReg inReg;
      LawPtr pLaw;
      TRequest req;
      TCountRecsQuery q(crit);
      const unsigned recs = q.getRecsCount();
      int r = 0;
	char textBufferTmp1[0x100];
	char textBufferTmp2[0x100];

	wtdl->GetText(textBufferTmp1, sizeof textBufferTmp1);

	// I call WantBreak() in the next row, because the process is
	// highly time consuming.
	while(wtdl->WantBreak() == false && q.Read())
	{
        pIRE.Ref() << q;
        incrementInRegEventValues(pIRE.Ref(), dataCiph,
# if INSTANCE
          dataCiph2nd,
# endif
          inReg, pLaw.Ref(), req, incrementWith);
	  if((r % 5) == 0)
	  {
	    double perc = 100.0;

	    if(recs > 0)
	    {
		perc *= r;
		perc /= recs;
	    }
	    ostrstream(textBufferTmp2, sizeof(textBufferTmp2))
		<< textBufferTmp1 << " (" << resetiosflags(ios::scientific)
		<< setiosflags(ios::fixed | ios::showpoint)
		<< setprecision(5) << perc << "% обработени) ..." << ends;
	    ENDING_ZERO(textBufferTmp2);
	    wtdl->SetText(textBufferTmp2);
	  }
	  r++;
      }
    }
  }
}

#endif  // of USE_JUDGE_CHECKUP_OTHERS

void TJudgeCheckupThread::fillArr (const CDate & bP, const CDate & eP,
					const bookAreas * bkAreas, int areasCnt)
 {
  const CDate begP = trueJudgeCheckup ? firstDayOfThisMonth (bP) : bP ;
  const CDate endP = trueJudgeCheckup ? lastDayOfThisMonth (eP) : eP ;
  const char * const clSess = trueJudgeCheckup ? NULL :
					KIND_CLOSED_SESSIONS ;
  const char * const defaultRequestKindsString = reqKindsString () ;
// All NULL values for the argument "requestKindsString" of
// lawsSessDistribute () are changed to "defaultRequestKindsString".
  makeArr () ;
// ------------------- for these - nothing yet ----------------------
//  iC_L_NotFinishedsBeg1
//  iC_L_NotFinishedsEnd1
//# if INSTANCE
//  iC_L_NotFinishedsBeg2
//  iC_L_NotFinishedsEnd2
//# endif
// ------------------------------------------------------------------

// 20061208 ------------------------------>  REGIONAL COURT ONLY
#  if ! INSTANCE

  // k.1   N Останали несвършени  в началото на отчетния период // 25225  P  - 25002
  //    lawsDistribute (iC_L_NotDecisionBeg, bkAreas, areasCnt,
  //                    & begP, NULL, NULL, NULL, NULL, true,
  //                    NULL, NULL, NULL, false, true) ;
	lawsDistribute_RJ ( iC_L_NotFinishedsBeg1_J,
				  bkAreas, areasCnt,
				  & begP, NULL, NULL, NULL, NULL, true,
				  NULL, NULL, NULL, false, true) ;

  // k.2   N Постъпили дела - Новообразувани  // 25230   P -  25191
  //         Отчитат се всички Новообразувани без
  //                    "Получени по подсъдност" и "Върнати за ново разглеждане"
  char srcType [2] ;
  srcType [0] = '\0' ; // TYPE_REQUEST ;
  srcType [1] = '\0' ;
  char connectTypesString [4] ;
  connectTypesString [0] = CONNECT_TYPE_JURISDICTION ;     // = 'J'
  connectTypesString [1] = CONNECT_TYPE_RETURNED_LAWSUIT ; //	= 'R' Върнати за ново разглеждане
  connectTypesString [2] = CONNECT_TYPE_RERETURNED_LAWSUIT;// = 'U' Внесено след доразследване 	- към Върнати за ново разглеждане
  connectTypesString [3] = '\0' ;
  lawsDistribute_RJ (iC_L_NewsRejudge1_J, bkAreas, areasCnt,
                     & endP, & begP, NULL, NULL, NULL, false, srcType, NULL,
                     NULL, false, false, true,
                     NULL, NULL, true,
                     SenderMustBeEmpty, //const senderChecksActions aboutTheSender,// = IgnoreSender,  // 19
                     false,             //const bool requestCurrentlyQuick,        // = false,         // 20
                     1,                 //const long int incrementWith,            // = 1              // 21
			   connectTypesString,//const char * connectTypesString,         // = NULL           // 22
                     false              //const bool lConnSMatch                   // = true           // 23
                     ) ;

  // k.3   N Постъпили дела - Получени по подсъдност // 25207
  srcType [0] = '\0'; // TYPE_JURISD ;
  srcType [1] = '\0' ;
  connectTypesString [0] = CONNECT_TYPE_JURISDICTION ;     // = 'J'
  connectTypesString [1] = '\0' ;
  lawsDistribute_RJ (iC_L_byJurisd_J, bkAreas, areasCnt,
                    & endP, & begP, NULL, NULL, NULL, false, srcType, NULL,
                    NULL, false, false, true,
                    NULL, NULL, true,
                    SenderMustBeEmpty, //const senderChecksActions aboutTheSender,// = IgnoreSender,  // 19
                    false,             //const bool requestCurrentlyQuick,        // = false,         // 20
                    1,                 //const long int incrementWith,            // = 1              // 21
                    connectTypesString,//const char * connectTypesString,         // = NULL           // 22
                    true               //const bool lConnSMatch                   // = true           // 23
                    ) ;

  // k.4   N Постъпили дела - Върнати за ново разглеждане  // 25208
  srcType [0] = '\0'; // TYPE_RETURNED ;
  connectTypesString [0] = CONNECT_TYPE_RETURNED_LAWSUIT ; //	= 'R' --> Върнати за ново разглеждане
  connectTypesString [1] = CONNECT_TYPE_RERETURNED_LAWSUIT;// = 'U' Внесено след доразследване 	--> Върнати за ново разглеждане
  connectTypesString [2] = '\0' ;
  lawsDistribute_RJ (iC_L_returneds_J, bkAreas, areasCnt,
                    & endP, & begP, NULL, NULL, NULL, false, srcType, NULL,
                    NULL, false, false, true,
                    NULL, NULL, true,
                    SenderMustBeEmpty, //const senderChecksActions aboutTheSender,// = IgnoreSender,  // 19
                    false,             //const bool requestCurrentlyQuick,        // = false,         // 20
                    1,                 //const long int incrementWith,            // = 1              // 21
                    connectTypesString,//const char * connectTypesString,         // = NULL           // 22
                    true               //const bool lConnSMatch                   // = true           // 23
                    ) ;

  // k.5   N Постъпили дела - Общо  25209     - SUM
  // k.6   N Всичко дела за разглеждане  25210  - SUM

  // само за разпределение по съдии към Справка за дейността на РС - само за Гр.Д и Бр.Д
  // пресмятанията са аналогични на разпределението по предмети
  CharPtr tmpStr(strlen (RESULT_LEFTS) +
                 strlen (RESULT_UNMOVEABLES) + 1) ;
  ostrstream (tmpStr, tmpStr.Size ())
        << RESULT_LEFTS << RESULT_UNMOVEABLES << ends ;
  tmpStr [tmpStr.Size () - 1] = '\0' ;

  char kindC [6] ;

  // k.7   Насрочени дела
  kindC [0] = KIND_RECONCILIATION_SESSION ;
  kindC [1] = KIND_FIRST_SESSION ;
  kindC [2] = KIND_SECOND_SESSION ;
  kindC [3] = KIND_INTERIM_SESSION ;
  kindC [4] = KIND_OTHER_SESSION ;
  kindC [5] = '\0' ;
  sessDistribute_RJ (iC_S_Fixeds_RC_J, bkAreas, areasCnt, & endP, & begP,
                     NULL,             // resultsString
                     kindC,            // sessKindsString
                     true) ;
  // k.8    Спрени дела  - 25046

  // k.9    Отложени дела - Помирителен срок
  kindC [0] = KIND_RECONCILIATION_SESSION ;
  kindC [1] = '\0' ;
  sessDistribute_RJ (iC_S_Lefts_ReconcileSess_J, bkAreas, areasCnt,
                     & endP, & begP, tmpStr, kindC, true) ;

  // k.10   Отложени дела - Преди първо заседание
  // търсим "Първо заседание" с резултат "Отложено...", но различен от "Отложено в първо заседание"
  kindC [0] = KIND_FIRST_SESSION ;
  kindC [1] = '\0' ;
  sessDistribute_RJ (iC_S_Lefts_BeforeFirstSess_J, bkAreas, areasCnt,
                     & endP, & begP,
                     RESULT_LEFT_REPORT_DESIGNATION, // resultsString
                     kindC,  // sessKindsString
                     true,   // sKSMatch         ; false by def
                     true,   // excludeClosedSess; true  by def
                     true    // beforeLawFinish  ; true  by def
                     ) ;
  // k.11   Отложени дела - В първо заседание
  // търсим "Първо заседание" с резултат "Отложено в първо заседание"
  kindC [0] = KIND_FIRST_SESSION ;
  kindC [1] = '\0' ;
  sessDistribute_RJ (iC_S_Lefts1stSess1_J, bkAreas, areasCnt,
                     & endP, & begP,
                     RESULT_LEFT_1_REPORT_DESIGNATION, // resultsString
                     kindC, true) ;

  // k.12   Отложени дела - Във второ заседание и следващо съдебно заседание
  kindC [0] = KIND_SECOND_SESSION ;
  kindC [1] = '\0' ;
  sessDistribute_RJ (iC_S_Lefts2ndSess1_J, bkAreas, areasCnt,
                     & endP, & begP,
                     tmpStr,
                     kindC, true) ;

  // k.12а N Отложени дела - Зас.по привременни мерки и др.заседание
  // Отложени (без движение) заседания по привременни мерки и други заседания
  kindC [0] = KIND_INTERIM_SESSION ;
  kindC [1] = KIND_OTHER_SESSION ;
  kindC [2] = '\0' ;
  sessDistribute_RJ (iC_S_Lefts_Interim_RC_J, bkAreas, areasCnt,
                     & endP, & begP,
                     tmpStr,
                     kindC, true) ;

  // k.13  N Отложени дала - Отложени ОБЩО  - SUM
  // k.14  P Свършени дела - ОБЩО - SUM

  // k.15  N Свършени дела - С решение 25229 ; P 25012
  // Отчитат се брой дела, за които Дата на  свършване е през отчения период и
  // преди дата на свършване има проведено
  // Помирително заседани, 1-во, 2-ро, заседание по привременни мерки
  // с резултат Обявено за решаване или С решение.
  lawsDistribute_RJ ( iC_L_Completeds_Decision_RC_J, //25229
                     bkAreas, areasCnt, & endP, & begP,
                     RESULT_DECIDED_REPORT_DESIGNATION,   // resultsString
                     NULL, // const char * requestKindsString = NULL,    //  7
                     NULL, // const char * ageString = NULL,             //  8
			   true, // const bool finishedTest = true,            //  9
                     NULL, // const char * sourceTypesString = NULL,     // 10
                     NULL, // const char * grantedsString = NULL,        // 11
                     KIND_SESS_COMPLE_REPORT_DESIGNATION, // const char * sessKindsString = NULL,       // 12
                     true  // const bool sKMatch = false,                // 13
                    ) ;

  // k.16  N Свършени дела - Искът уважен изцяло  // 25215
  char granted [2] ;
  granted [0] = GRANT_ENTIRE ;
  granted [1] = '\0' ;
  /* Ако трябва да се изведат същите дела както в кол.15 + условие за иска
  lawsDistribute_RJ (iC_L_Completeds_Grant_Entire_J,
                    bkAreas, areasCnt, & endP, & begP,
                    RESULT_DECIDED_REPORT_DESIGNATION,   // resultsString
                    NULL,   // requestKindsString
                    NULL,   // ageString
                    true,   // finishedTest
			  NULL,   // sourceTypesString
                    granted,// grantedsString
                    KIND_SESS_COMPLE_REPORT_DESIGNATION, // sessKindsString
                    true,  // sKMatch
                    false,  // excludeCreatedsLater
                    true) ;// rSMatch = true
                    */
  lawsDistribute_RJ(iC_L_Completeds_Grant_Entire_J, bkAreas, areasCnt,
                    & endP, & begP,
                    NULL,
			  NULL, NULL, true,
                    NULL, granted, NULL, false, false, false) ;

  // k.17  N Свършени дела - Искът уважен отчасти   // 25216
  granted [0] = GRANT_PARTIAL ;
  /* Ако трябва да се изведат същите дела както в кол.15 + условие за иска
  lawsDistribute_RJ (iC_L_Completeds_Grant_Partial_J,
                    bkAreas, areasCnt, & endP, & begP,
                    RESULT_DECIDED_REPORT_DESIGNATION,   // resultsString
                    NULL, NULL, true,
                    NULL,
                    granted,
                    KIND_SESS_COMPLE_REPORT_DESIGNATION, // sessKindsString
                    true, false, true) ;
                    */
  lawsDistribute_RJ(iC_L_Completeds_Grant_Partial_J, bkAreas, areasCnt,
                    & endP, & begP,
                    NULL,
                    NULL, NULL, true,
                    NULL, granted, NULL, false, false, false) ;

  // k.18  N Свършени дела - Искът отхвърлен        // 25217
  //    granted [0] = GRANT_REJECTED ;  // 20.apr.2001
  /* Ако трябва да се изведат същите дела както в кол.15 + условие за иска
  lawsDistribute_RJ (iC_L_Completeds_Grant_Rejected_J,
                    bkAreas, areasCnt, & endP, & begP,
                    RESULT_DECIDED_REPORT_DESIGNATION,   // resultsString
                    NULL, NULL, true,
			  NULL,
                    GRANT_REJECTEDS,  // granted  // 20.apr.2001
                    KIND_SESS_COMPLE_REPORT_DESIGNATION, // sessKindsString
                    true, false, true) ;
                    */
  lawsDistribute_RJ (iC_L_Completeds_Grant_Rejected_J, bkAreas, areasCnt,
                    & endP, & begP,
                    NULL,
			  NULL, NULL, true,
                    NULL,
                    GRANT_REJECTEDS,  // granted
                    NULL, false, false, false) ;

  // k.19  N Свършени дела - Свършени с акт по същество в З.З // 25218
  // Отчитат се дела, за които дата на свършване е през отчетния период  и преди дата на свършване
  // има проведено ЗЗ/РЗ с резултат "С определение, приключващо делото" или "С решение"
  lawsDistribute_RJ (iC_L_Completeds_ess_CS_RC_J, bkAreas, areasCnt,
                    & endP, & begP,
                    RESULT_ESS_CS_REPORT_DESIGNATION,   // resultsString
                    NULL, // const char * requestKindsString = NULL,
                    NULL, // const char * ageString = NULL,
                    true, // const bool finishedTest = true,
                    NULL, // const char * sourceTypesString = NULL,
                    NULL, // const char * grantedsString = NULL,
                    KIND_CLOSED_SESSIONS , // const char * sessKindsString = NULL
                    true  // const bool sKMatch = false,
                    ) ;

  // k.20  N Свършени дела - Прекратени дела - По спогодба;    // 25219
  {
     char resCancAgr [2] ;
     resCancAgr [0] = RESULT_CANCEL_BY_AGREEMENT ;   //'P'
     resCancAgr [1] = '\0' ;
     lawsDistribute_RJ (iC_L_Completeds_canceleds_byAgreement_J,
                        bkAreas, areasCnt,
                        & endP, & begP, resCancAgr, NULL, NULL,
                        true, NULL, NULL, KIND_CLOSED_SESSIONS) ;

  // k.21  - nothing

     CharPtr tmpStr (strlen (RESULT_CANCELEDS_OR_RETURNEDS) + 1 + 1 + 1) ;

     ostrstream (tmpStr, tmpStr.Size ())
        << RESULT_SEND << RESULT_CANCELEDS_OR_RETURNEDS << ends ;
     tmpStr [tmpStr.Size () - 1] = '\0' ;

     difference (tmpStr, resCancAgr) ;

     if (tmpStr [0])
     {
  // k.22  N Свършени дела - Прекратени дела - Други причини - Съдебно заседание // 25040
         lawsDistribute_RJ (iC_L_Completeds_canceleds_byOtherCauses_OS_J,
                            bkAreas, areasCnt,
                            & endP, & begP, tmpStr, NULL, NULL, true,
				    NULL,NULL, KIND_CLOSED_SESSIONS) ;
  // k.23  - nothing
  // k.24  N Свършени дела - Прекратени дела - Други причини - Закрито заседание // 25221
         lawsDistribute_RJ (iC_L_Completeds_canceleds_byOtherCauses_CS_J,
                            bkAreas, areasCnt,
                            & endP, & begP, tmpStr, NULL, NULL, true,
                            NULL,NULL, KIND_CLOSED_SESSIONS, true) ;
     }
  // k.25  N Свършени дела - Прекратени дела - ОБЩО   // 25222  SUM
  // jCL.completeds_canceleds_summByAgreeByOth_RC_J
  }


  // k.28  N Допускане на делба  //25223
  sessDistribute_RJ (iC_S_Splits_J, bkAreas, areasCnt, & endP, & begP,
                    RESULT_SPLITS, NULL, false, false) ;

  // k.29  N Допълване на решение, определение   // 25224
  kindC [0] = KIND_OTHER_SESSION ;
  kindC [1] = '\0' ;
  sessDistribute_RJ (iC_S_otherSessAfterFinish_J, bkAreas, areasCnt,
                        & endP, & begP, tmpStr, kindC, true, true, false) ;

  // k.30  - Други съдебни актове от съдебно заседание - nothing

  // k.31 N Останали несвършени дела в края на отчетния период // 25231 // P 25016

    lawsDistribute_RJ (iC_L_NotFinishedsEnd1_J, bkAreas, areasCnt,
			  //& nextP,
			  & endP,
			  NULL, NULL, NULL, NULL, true,
			  NULL, NULL, NULL, false, true) ;



  // k.32  - Обжалвани дела - брой изпратени въззивни жалби и частни жалби - nothing
  // 20070202
    appealedLawsDistribute_RJ (iC_L_Appealeds_RC_J, bkAreas, areasCnt,
                               endP, begP) ;


# endif

// 20061212 <------------------------------ REGIONAL COURT ONLY

  lawsSessDistribute (iC_L_News1,
# if INSTANCE
                                  iC_L_News2,
# endif
                                              bkAreas, areasCnt,
                      & endP, & begP, NULL, defaultRequestKindsString,
											NULL, false) ;
  lawsSessDistribute (iC_L_NewsRejudge1,
# if INSTANCE
					    iC_L_NewsRejudge2,
# endif
                                              bkAreas, areasCnt,
			    & endP, & begP, NULL, defaultRequestKindsString,
											NULL, false) ;  // 11.dec.2003
	 {  // 10.jul.2002 - begin
		char lawKinds [16] ;

		ostrstream (lawKinds, sizeof (lawKinds))
# if INSTANCE
#   if APPEAL
			<< KIND_PUNI_GENERAL_CHAR_LAW_2A
#   else  // of APPEAL
			<< KIND_PUNISHMENT_GENERAL_CHAR_LAW
			<< KIND_2ND_COMMON_PUNISHMENT_LAW
#   endif  // of APPEAL
# else  // of INSTANCE
	<< KIND_PUNISHMENT_GENERAL_CHAR_LAW
# endif  // of INSTANCE
			<< ends ;
		ENDING_ZERO (lawKinds) ;
		// 2010:284 LRQ/LPQ: excludeCreatedsLater true -> false
		lawsSessDistribute (iC_L_NewsPunGenChar1,
# if INSTANCE
																		iC_L_NewsPunGenChar2,
# endif
																								bkAreas, areasCnt,
												& endP, & begP, NULL, defaultRequestKindsString,
												NULL, false, NULL, NULL, NULL, false,
												false,	// <- this one 2010:284
												false, false, true, NULL, true, lawKinds) ;
		// 2010:284 LRQ/LPQ: excludeCreatedsLater true -> false
		lawsSessDistribute (iC_L_NewsPunGenCharRejudge1,
# if INSTANCE
																		iC_L_NewsPunGenCharRejudge2,
# endif
																								bkAreas, areasCnt,
												& endP, & begP, NULL, defaultRequestKindsString,
												NULL, false, NULL, NULL, NULL, false,
												false,	// <- this one 2010:284
												false, false, true, NULL, false,
				lawKinds) ;  // 11.dec.2003
	 }  // 10.jul.2002 - end
	lawsSessDistribute (iC_L_FinishedsTotal1,
# if INSTANCE
			iC_L_FinishedsTotal2,
# endif
			bkAreas, areasCnt, & endP, & begP,
			RESULT_COMPLETIZEDS,
			defaultRequestKindsString,
			NULL, true, NULL, NULL, clSess
		     ) ;  // RESULT_FINALIZEDS -
			  // to cause searching in T_SESSION
  lawsSessDistribute (iC_L_FinishedsWithDecision1,
# if INSTANCE
			iC_L_FinishedsWithDecision2,
# endif
			    bkAreas, areasCnt,
			    & endP, & begP, RESULT_WITH_DECISIONS,
			    defaultRequestKindsString,
			    NULL, true, NULL, NULL, clSess
			   ) ;
// 04.aug.2000 - begin  ------------------ PATCH -------------------
#     if PUNISHMENT_04_AUG_2000_PATCH
  if (! strchr (bMYEMYTCIU.types, TYPE_PUNISHMENT_LAW))
   {
    char tmpResStr [2] ;
    char tmpGrantString [2] ;

    scSprintf (sizeof (tmpResStr), tmpResStr, "%c",
               RESULT_DEFINITION_TERMINATING) ;
    scSprintf (sizeof (tmpGrantString), tmpGrantString, "%c",
               GRANT_NONE_YET) ;
    if (endP >= PATCH_04_AUG_2000_RDT_DATE)
     {
      const CDate fakedBegP = PATCH_04_AUG_2000_RDT_DATE > begP ?
                                PATCH_04_AUG_2000_RDT_DATE : begP ;

      lawsSessDistribute (iC_L_FinishedsWithDecision1,
# if INSTANCE
                          iC_L_FinishedsWithDecision1,
# endif
                          bkAreas, areasCnt, & endP, & fakedBegP,
                          tmpResStr, defaultRequestKindsString, NULL,
                          true, NULL, tmpGrantString, clSess, false,
				  false, true, false, true, NULL, true, -1) ;
	lawsSessDistribute (iC_L_FinishedsCanceleds1,
# if INSTANCE
				  iC_L_FinishedsCanceleds1,
# endif
				  bkAreas, areasCnt, & endP, & fakedBegP,
				  tmpResStr, defaultRequestKindsString, NULL,
				  true, NULL, tmpGrantString, clSess) ;
      lawsSessDistribute (iC_L_FinishedsCanceledsOtherC1,
# if INSTANCE
				  iC_L_FinishedsCanceledsOtherC1,
# endif
                          bkAreas, areasCnt, & endP, & fakedBegP,
                          tmpResStr, defaultRequestKindsString, NULL,
                          true, NULL, tmpGrantString, clSess) ;
      lawsSessDistribute (iC_L_CanceledsClosedS1,
# if INSTANCE
                          iC_L_CanceledsClosedS1,
# endif
                          bkAreas, areasCnt, & endP, & fakedBegP,
                          tmpResStr, defaultRequestKindsString, NULL,
                          true, NULL, tmpGrantString,
                          KIND_CLOSED_SESSIONS, true) ;
     }
   }
#     endif  // PUNISHMENT_04_AUG_2000_PATCH
// 04.aug.2000 - end    --------------------------------------------
  lawsSessDistribute (iC_L_DecisDefinClosedS1,
# if INSTANCE
                      iC_L_DecisDefinClosedS2,
# endif
                      bkAreas, areasCnt,
                      & endP, & begP, RESULT_WITH_DECISIONS,
                      defaultRequestKindsString,
                      NULL, true, NULL, NULL,
                      KIND_CLOSED_SESSIONS, true
                     ) ;
// 11.jan.2000 - begin  (patch)
# if INSTANCE
   {
    char tmpSR [2] ;
    constant tmpDRC = RESULT_CANCELAWS;

    tmpSR [1] = '\0' ;
    tmpSR [0] = RESULT_DEFINITION_TERMINATING ;
    lawsSessDistribute (iC_L_FinishedsWithDecision2,  // 2 !!!
                        iC_L_FinishedsWithDecision2,
                        bkAreas, areasCnt, & endP, & begP, tmpSR,
                        defaultRequestKindsString,
                        NULL, true, NULL, NULL, clSess,
                        false, false, true, false, true, tmpDRC, true,
												NULL, true  // 08.apr.2002
												,  // 04.jun.2002  BUGFIX
                        -1
                       ) ;
    lawsSessDistribute (iC_L_DecisDefinClosedS2,  // 2 !!!
                        iC_L_DecisDefinClosedS2,
                        bkAreas, areasCnt, & endP, & begP, tmpSR,
                        defaultRequestKindsString,
                        NULL, true, NULL, NULL, KIND_CLOSED_SESSIONS,
                        true, false, true, false, true, tmpDRC, true,
                        NULL, true  // 08.apr.2002
												,  // 04.jun.2002  BUGFIX
                        -1
                       ) ;
   }
# endif
// 11.jan.2000 - end
   {
    const size_t tmpCSz =
      strlen (RESULT_CANCELEDS_OR_RETURNEDS) + 1 + 1 ;
    char * const tmpCancel = new char [tmpCSz] ;

    if (tmpCancel)
     {
      ostrstream (tmpCancel, tmpCSz)
        << RESULT_CANCELEDS_OR_RETURNEDS << RESULT_SEND << ends ;
	tmpCancel [tmpCSz - 1] = '\0' ;
	lawsSessDistribute (iC_L_FinishedsCanceleds1,
# if INSTANCE
				  iC_L_FinishedsCanceleds2,
# endif
				  bkAreas, areasCnt, & endP, & begP, tmpCancel,
				  defaultRequestKindsString,
				  NULL, true, NULL, NULL, clSess
				 ) ;
	lawsSessDistribute (iC_L_CanceledsClosedS1,
# if INSTANCE
				  iC_L_CanceledsClosedS2,
# endif
                          bkAreas, areasCnt, & endP, & begP, tmpCancel,
                          defaultRequestKindsString,
                          NULL, true, NULL, NULL,
                          KIND_CLOSED_SESSIONS, true
                         ) ; 
       {
        char rCA [2] ;

        rCA [1] = '\0' ;
        rCA [0] = RESULT_CANCEL_BY_AGREEMENT ;
        lawsSessDistribute (iC_L_FinishedsCanceledsByAgr1,
# if INSTANCE
                            iC_L_FinishedsCanceledsByAgr2,
# endif
                            bkAreas, areasCnt, & endP, & begP, rCA,
                            defaultRequestKindsString,
                            NULL, true, NULL, NULL, clSess
                           ) ;
        lawsSessDistribute (iC_L_FinishedsCanceledsOtherC1,
# if INSTANCE
                            iC_L_FinishedsCanceledsOtherC2,
# endif
                            bkAreas, areasCnt, & endP, & begP,
                            difference (tmpCancel, rCA),
                            defaultRequestKindsString,
                            NULL, true, NULL, NULL, clSess
                           ) ;
          // !!! tmpCancel is already changed !!!  <-- difference ()
       }
      delete [] tmpCancel ;
     }
   }
// 24.jan.2000 - begin  // 11.jan.2000 - begin  (Patches)
# if INSTANCE
   {
    char tmpSR [2] ;
    constant tmpDRC = RESULT_CANCELAWS;

    tmpSR [1] = '\0' ;
    tmpSR [0] = RESULT_DEFINITION_TERMINATING ;
    lawsSessDistribute (iC_L_FinishedsCanceleds2,  // 2 !!!
				iC_L_FinishedsCanceleds2,
				bkAreas, areasCnt, & endP, & begP, tmpSR,
				defaultRequestKindsString,
				NULL, true, NULL, NULL, clSess,
				false, false, true, false, true, tmpDRC) ;
    lawsSessDistribute (iC_L_CanceledsClosedS2,  // 2 !!!
				iC_L_CanceledsClosedS2,
                        bkAreas, areasCnt, & endP, & begP, tmpSR,
				defaultRequestKindsString,
                        NULL, true, NULL, NULL, KIND_CLOSED_SESSIONS,
                        true, false, true, false, true, tmpDRC
                       ) ;
    lawsSessDistribute (iC_L_FinishedsCanceledsOtherC2,  // 2 !!!
                        iC_L_FinishedsCanceledsOtherC2,
                        bkAreas, areasCnt, & endP, & begP, tmpSR,
                        defaultRequestKindsString,
                        NULL, true, NULL, NULL, clSess,
                        false, false, true, false, true, tmpDRC) ;
   }
# endif
// 24.jan.2000 - end  // 11.jan.2000 - end
   {
    lawsSessDistribute (iC_L_FinishedsUpTo3M1,
# if INSTANCE
                        iC_L_FinishedsUpTo3M2,
# endif
			bkAreas, areasCnt,
			& endP, & begP, RESULT_COMPLETIZEDS,
			defaultRequestKindsString,
                        AGE_UP_TO_THREES,
                        true, NULL, NULL, clSess
                       ) ;  // RESULT_FINALIZEDS -
                            // to cause searching in T_SESSION
   }
   {
    lawsSessDistribute (iC_L_FinishedsOver3M1,
# if INSTANCE
                        iC_L_FinishedsOver3M2,
# endif
                        bkAreas, areasCnt,
                        & endP, & begP, RESULT_COMPLETIZEDS,
                        defaultRequestKindsString,
                        AGE_MORE_THAN_THREES_ALL,
                        true, NULL, NULL, clSess
		       ) ;  // RESULT_FINALIZEDS -
				    // to cause searching in T_SESSION
   }
   {
    char rA [2] ;

# if INSTANCE
    rA [0] = RESULT_ABOLISH ;
# else
    rA [0] = RESULT_PUNISHM_DECREE_ABOLISHED ;
# endif
    rA [1] = '\0' ;
    lawsSessDistribute (iC_L_Abolisheds1,
# if INSTANCE
                                          iC_L_Abolisheds1,
                                          //   ...1 yet !!!
# endif
                        bkAreas, areasCnt, & endP, & begP, rA,
                        defaultRequestKindsString,
                        NULL, true, NULL, NULL, clSess
                       ) ;
   }
# if ! INSTANCE
  lawsSessDistribute (iC_L_KeepingMethodConfirmed1,
                      bkAreas, areasCnt, & endP, & begP, "",
                      defaultRequestKindsString,
                      NULL, true, NULL, NULL, NULL, false, false,
                      true, false, true,
                      RESULT_KEEPING_METHOD_CONFIRMEDS) ;
  lawsSessDistribute (iC_L_KeepingMethodChanged1,
                      bkAreas, areasCnt, & endP, & begP, "",
                      defaultRequestKindsString,
                      NULL, true, NULL, NULL, NULL, false, false,
                      true, false, true,
                      RESULT_KEEPING_METHOD_CHANGEDS) ;
# endif
// -------------------
  lawsSessDistribute (iC_L_Subject0210,
# if INSTANCE
                                        iC_L_Subject0210,
# endif
                                              bkAreas, areasCnt,
                      & endP, & begP, NULL, defaultRequestKindsString,
                      NULL, false) ;
// ATTENTION! There must be only one call of lawsSessDistribute () for
// the all set of
// iC_L_Subject0210,
// iC_L_Subject0600,
// iC_L_Subject0500,
// iC_L_Subject0810,
// iC_L_Subjects0400_0410 and
// iC_L_Subjects0220_0310_0800.
// -------------------
# if INSTANCE
  lawsSessDistribute (iC_L_ExactComplaints2, iC_L_ExactComplaints2,
                      bkAreas, areasCnt, & endP, & begP, NULL,
                      KIND_EXACT_COMPLAINTS,
                      NULL, true, NULL, NULL, NULL, false, false,
                      true, false, true, NULL, false) ;
  lawsSessDistribute (iC_L_PrivateComplaints2, iC_L_PrivateComplaints2,
                      bkAreas, areasCnt, & endP, & begP, NULL,
                      KIND_PRIVATE_COMPLAINTS,
                      NULL, true, NULL, NULL, NULL, false, false,
                      true, false, true, NULL, false) ;
# endif
   {
    const size_t kPCSz = strlen (KIND_PSEUDO_SESSIONS) + 2
                         + strlen (KIND_CLOSED_SESSIONS) ;
    char * const kPC = new char [kPCSz] ;

    if (kPC)
     {
      ostrstream (kPC, kPCSz) << KIND_PSEUDO_SESSIONS
        << KIND_CLOSED_SESSIONS << ends ;
      kPC [kPCSz - 1] = '\0' ;
      lawsSessDistribute (iC_S_Fixeds1,
# if INSTANCE
                                        iC_S_Fixeds2,
# endif
                          bkAreas, areasCnt, & endP, & begP, "",
                          // "" - to cause searching in T_SESSION
                          defaultRequestKindsString, NULL, false, NULL,
                          NULL, kPC, false, false, true, true) ;
// 04.feb.2000 - begin =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
       {
	  const size_t t1SSz = strlen (RESULT_LEFTS) +
                             strlen (RESULT_STOPPEDS) +
                             strlen (RESULT_UNMOVEABLES) + 1 ;
        char * tmp1Str = new char [t1SSz] ;

         {
          ostrstream tmp1Oss (tmp1Str, t1SSz) ;

	    tmp1Oss << RESULT_LEFTS << RESULT_UNMOVEABLES ;
          if (columnRequired (iC_S_Stoppeds1, bkAreas, areasCnt) == false
# if INSTANCE
              &&
              columnRequired (iC_S_Stoppeds2, bkAreas, areasCnt) == false
# endif
             )
            tmp1Oss << RESULT_STOPPEDS ;
          tmp1Oss << ends ;
         }
        tmp1Str [t1SSz - 1] = '\0' ;
        lawsSessDistribute (iC_S_Lefts1,                       //25020
# if INSTANCE
                                         iC_S_Lefts2,
# endif
                            bkAreas, areasCnt, & endP, & begP, tmp1Str,// result string
                            defaultRequestKindsString, NULL, false,
                            NULL, NULL, kPC, false, false, true, true);
        fixedSessDistribute (iC_S_Lefts3Judges1, bkAreas, areasCnt,
                             endP, begP, kPC, false, tmp1Str) ;
        fixedSessDistribute (iC_S_Lefts3Judges2, bkAreas, areasCnt,
                             endP, begP, kPC, false, tmp1Str) ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
         {
          char k1stSess [2] ;

          k1stSess [1] = '\0' ;
          k1stSess [0] = KIND_FIRST_SESSION ;
          lawsSessDistribute (iC_S_Lefts1stSess1, iC_S_Lefts1stSess2,
                              bkAreas, areasCnt, & endP, & begP,
                              tmp1Str, defaultRequestKindsString, NULL,
                              false, NULL, NULL, k1stSess, true, false,
                              true, true) ;
          fixedSessDistribute (iC_S_Lefts3Judges1stSess1,
                               bkAreas, areasCnt, endP, begP,
                               k1stSess, true, tmp1Str) ;
          fixedSessDistribute (iC_S_Lefts3Judges1stSess2,
                               bkAreas, areasCnt, endP, begP,
                               k1stSess, true, tmp1Str) ;
         }
         {
          char k2ndSess [2] ;

          k2ndSess [1] = '\0' ;
	    k2ndSess [0] = KIND_SECOND_SESSION ;
          lawsSessDistribute (iC_S_Lefts2ndSess1, iC_S_Lefts2ndSess2,
                              bkAreas, areasCnt, & endP, & begP,
                              tmp1Str, defaultRequestKindsString, NULL,
                              false, NULL, NULL, k2ndSess, true, false,
                              true, true) ;
          fixedSessDistribute (iC_S_Lefts3Judges2ndSess1,
                               bkAreas, areasCnt, endP, begP,
                               k2ndSess, true, tmp1Str) ;
          fixedSessDistribute (iC_S_Lefts3Judges2ndSess2,
                               bkAreas, areasCnt, endP, begP,
                               k2ndSess, true, tmp1Str) ;
         }
#   endif  // of APPEAL
# else  // of INSTANCE
         {
          char k1stSess [2] ;

          k1stSess [1] = '\0' ;
          k1stSess [0] = KIND_FIRST_SESSION ;
          lawsSessDistribute (iC_S_Lefts1stSess1,
                              bkAreas, areasCnt, & endP, & begP,
                              tmp1Str, defaultRequestKindsString, NULL,
                              false, NULL, NULL, k1stSess, true, false,
                              true, true) ;
          fixedSessDistribute (iC_S_Lefts3Judges1stSess1,
                               bkAreas, areasCnt, endP, begP,
                               k1stSess, true, tmp1Str) ;
          fixedSessDistribute (iC_S_Lefts3Judges1stSess2,
                               bkAreas, areasCnt, endP, begP,
                               k1stSess, true, tmp1Str) ;
         }
         {
          char k2ndSess [2] ;

          k2ndSess [1] = '\0' ;
          k2ndSess [0] = KIND_SECOND_SESSION ;
          lawsSessDistribute (iC_S_Lefts2ndSess1,
                              bkAreas, areasCnt, & endP, & begP,
                              tmp1Str, defaultRequestKindsString, NULL,
                              false, NULL, NULL, k2ndSess, true, false,
                              true, true) ;
          fixedSessDistribute (iC_S_Lefts3Judges2ndSess1,
                               bkAreas, areasCnt, endP, begP,
                               k2ndSess, true, tmp1Str) ;
          fixedSessDistribute (iC_S_Lefts3Judges2ndSess2,
					 bkAreas, areasCnt, endP, begP,
                               k2ndSess, true, tmp1Str) ;
         }
# endif  // of INSTANCE 
        delete [] tmp1Str ;
       }
// 04.feb.2000 - end   =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// 05.apr.2002 - begin =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
      lawsSessDistribute (iC_L_DetailNotFinishedBegTotal1,
# if INSTANCE
                          iC_L_DetailNotFinishedBegTotal2,
# endif
                          bkAreas, areasCnt, & begP, NULL, NULL,
                          defaultRequestKindsString,
                          NULL,
                          true,  // finishedTest
                          NULL, NULL,
                          NULL,  // sessKindsString
                          true,
                          true   // excludeCreatedsLater
                         ) ;
      lawsSessDistribute (iC_L_DetailNotFinBegPunGen2,
# if INSTANCE
                          iC_L_DetailNotFinBegPunGen2,
# endif
                          bkAreas, areasCnt, & begP, NULL, NULL,
                          defaultRequestKindsString,
                          NULL,
                          true,  // finishedTest
                          NULL, NULL,
                          NULL,  // sessKindsString
                          true,
                          true,  // excludeCreatedsLater
                          true, false, true, NULL, true,
                          KIND_PUN_GENERAL_LAWS
                         ) ;
      lawsSessDistribute (iC_L_DetailNotFinBegPunPrv2,
# if INSTANCE
                          iC_L_DetailNotFinBegPunPrv2,
# endif
                          bkAreas, areasCnt, & begP, NULL, NULL,
                          defaultRequestKindsString,
                          NULL,
                          true,  // finishedTest
                          NULL, NULL,
                          NULL,  // sessKindsString
				  true,
                          true,  // excludeCreatedsLater
                          true, false, true, NULL, true,
                          KIND_PUN_PRIVATE_LAWS
                         ) ;
       {
        const CDate nextP = trueJudgeCheckup ? firstDayOfNextMonth (eP) :
                                                 nextDay (eP) ;

        lawsSessDistribute (iC_L_DetailNotFinishedEndTotal1,
# if INSTANCE
                            iC_L_DetailNotFinishedEndTotal2,
# endif
                            bkAreas, areasCnt, & nextP, NULL, NULL,
                            defaultRequestKindsString,
                            NULL,
                            true,  // finishedTest
                            NULL, NULL,
                            NULL,  // sessKindsString
                            true,
                            true   // excludeCreatedsLater
                           ) ;
        lawsSessDistribute (iC_L_DetailNotFinEndPunGen2,
# if INSTANCE
                            iC_L_DetailNotFinEndPunGen2,
# endif
                            bkAreas, areasCnt, & nextP, NULL, NULL,
                            defaultRequestKindsString,
                            NULL,
                            true,  // finishedTest
                            NULL, NULL,
                            NULL,  // sessKindsString
                            true,
                            true,  // excludeCreatedsLater
                            true, false, true, NULL, true,
                            KIND_PUN_GENERAL_LAWS
                           ) ;
        lawsSessDistribute (iC_L_DetailNotFinEndPunPrv2,
# if INSTANCE
                            iC_L_DetailNotFinEndPunPrv2,
# endif
                            bkAreas, areasCnt, & nextP, NULL, NULL,
                            defaultRequestKindsString,
                            NULL,
                            true,  // finishedTest
                            NULL, NULL,
				    NULL,  // sessKindsString
                            true,
                            true,  // excludeCreatedsLater
                            true, false, true, NULL, true,
                            KIND_PUN_PRIVATE_LAWS
                           ) ;
       }
// 04.apr.2002 - end   =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
      lawsSessDistribute (iC_S_Stoppeds1,
# if INSTANCE
                                          iC_S_Stoppeds2,
# endif
                          bkAreas, areasCnt, & endP, & begP,
                          RESULT_STOPPEDS, defaultRequestKindsString,
                          NULL, false, NULL, NULL, kPC,
                          false, false, true, true) ;
      lawsSessDistribute (iC_S_OtherActs1,
# if INSTANCE
                                           iC_S_OtherActs2,
# endif
                          bkAreas, areasCnt, & endP, & begP, "",
                          // "" - to cause searching in T_SESSION
                          defaultRequestKindsString, NULL, false, NULL,
                          NULL, KIND_PSEUDO_SESSIONS,
                          true, false, true, true, false
                         ) ;
      lawsSessDistribute (iC_S_FixedsIncludingOtherActs1,
# if INSTANCE
                          iC_S_FixedsIncludingOtherActs2,
# endif
                          bkAreas, areasCnt, & endP, & begP, "",
                          // "" - to cause searching in T_SESSION
                          defaultRequestKindsString, NULL, false, NULL,
                          NULL, KIND_CLOSED_SESSIONS,
                          false, false, true, true) ;
      fixedSessDistribute (iC_S_Fixeds1Judge1, bkAreas, areasCnt,
                           endP, begP, kPC) ;
      fixedSessDistribute (iC_S_Fixeds2Judges1, bkAreas, areasCnt,
                           endP, begP, kPC) ;
      fixedSessDistribute (iC_S_Fixeds3Judges1, bkAreas, areasCnt,
                           endP, begP, kPC) ;
      fixedSessDistribute (iC_S_Fixeds3Judges2, bkAreas, areasCnt,
                           endP, begP, kPC) ;
      fixedSessDistribute (iC_S_FixedsIfNotJudge1, bkAreas, areasCnt,
                           endP, begP, kPC) ;
      fixedSessDistribute (iC_S_FixedsIfNotJudge3_2, bkAreas, areasCnt,
				   endP, begP, kPC) ;
      delete [] kPC ;
     }
   }

  addOverriddenToLefts () ;

   {
    const size_t tLSSz = strlen (RESULT_LEFTS) +
                         strlen (RESULT_STOPPEDS) +
                         strlen (RESULT_UNMOVEABLES) + 1 ;
    char * const tmpLeft = new char [tLSSz] ;

    scSprintf (tLSSz, tmpLeft, "%s%s%s",
               RESULT_LEFTS, RESULT_STOPPEDS, RESULT_UNMOVEABLES
              ) ;
    afterFinishDistribute (iC_S_AdditionalDecisDefinOpenS1,
# if INSTANCE
			   iC_S_AdditionalDecisDefinOpenS2,
# endif
			   bkAreas, areasCnt, endP, begP,
			   KIND_CLOSED_SESSIONS,  // sessKindsString,
			   false                  // sKMatch,

			   , true, false,         // conformWithSessDate, exscludeResDecree,

			   NULL, 1, tmpLeft) ;    // decisKindsString, incrementWith, sessResStr
    delete [] tmpLeft ;
   }
  afterFinishDistribute (iC_S_AdditionalDecisDefinCldsS1,
# if INSTANCE
                         iC_S_AdditionalDecisDefinCldsS2,
# endif
                         bkAreas, areasCnt, endP, begP,
                         KIND_CLOSED_SESSIONS,    // sessKindsString,
                         true                     // sKMatch,
# if INSTANCE
                         , false, false,          // conformWithSessDate, exscludeResDecree,
# else
                         , true, true,            // conformWithSessDate, exscludeResDecree,
# endif
                         NULL, 1, RESULT_LEFTS) ; // decisKindsString, incrementWith, sessResStr
#if USE_JUDGE_CHECKUP_OTHERS
  reqResolutionsDistribute (iC_Oth_ReqResolutions1,
# if INSTANCE
                            iC_Oth_ReqResolutions2,
# endif
                            bkAreas, areasCnt, endP, begP) ;
  inRegEventSDistribute (iC_Oth_InRegEvents1,
# if INSTANCE
                         iC_Oth_InRegEvents2,
# endif
                         bkAreas, areasCnt, endP, begP) ;
#endif  // of USE_JUDGE_CHECKUP_OTHERS
# if INSTANCE
#   if APPEAL
   {
    char r192 [16] ;

    r192 [sizeof (r192) - 1] = '\0' ;
    ostrstream (r192, sizeof (r192) - 1)
      << RESULT_CIT_192_193 << ends ;
    afterFinishDistribute (iC_S_Appeal192_193sessRes2,
                           iC_S_Appeal192_193sessRes2,
                           bkAreas, areasCnt, endP, begP,
                           KIND_CLOSED_SESSIONS, false, true, true,
                           NULL, 1, r192, true
                          );
   }
  specificAplCourtNewsDistribute (bkAreas, areasCnt, & endP, & begP,
                                  defaultRequestKindsString) ;
  specificAplCourtDecDistribute (bkAreas, areasCnt, & endP, & begP,
                                 defaultRequestKindsString) ;
#   endif  // of APPEAL
# endif  // of INSTANCE
 }

void TJudgeCheckupThread::makeBookFormatFN ()
 {
  makeJudgeCheckupFormatFN (formatFN, sizeof (formatFN),
				    trueJudgeCheckup, bMYEMYTCIU
# if INSTANCE
				    , cCBDEDBN
# endif
                            , cipherIfNotTrueGudgeCheckup , v_mainMenuItem
                           ) ;
 }

judgeCheckupData * TJudgeCheckupThread::newDataItem() const
 {
  return new judgeCheckupData ;
 }

judgeCheckupOutFile * TJudgeCheckupThread::newOutFile() const
 {
  return new judgeCheckupOutFile (wtdl->Parent, formatFN) ;
 }

judgeCheckupColumns * TJudgeCheckupThread::newJudgeCheckupColumns (const char * const formatFN, const judgeCheckupData & jCD,
	const CDate * const pDateForMonthColumn, const int serialNo) const
 {
  return new judgeCheckupColumns (formatFN, wtdl->Parent, jCD,
					    pDateForMonthColumn, serialNo) ;
 }

void TJudgeCheckupThread::Execute()
{
  CDate bP, eP;
  bkFormatsPseudoOutput bF(formatFN, wtdl->Parent);

  if (bF.formatLoadIsOK())
  {
    if (trueJudgeCheckup)
    {
	bP.day = eP.day = (char)1;
	bP.month = bMYEMYTCIU.begMonth;
	bP.year = bMYEMYTCIU.begYear;
	eP.month = bMYEMYTCIU.endMonth;
	eP.year = bMYEMYTCIU.endYear;
    }
    else
    {
	bP = cCBDEDBN.begD;
	eP = cCBDEDBN.endD;
    }
    if (bP.Empty() == false && eP.Empty() == false)
    {
	try
	{  // -- 1 --
		char descr[64];
	  totalPartSteps =
		own_areas_count(bF.getBkAreas(), bF.getBkAreasCnt(),
		1 + OFFSET_JUDGE_CHECKUP_CIPHER, i_columns);

	  ostrstream (descr, sizeof descr)
		<< (trueJudgeCheckup ? "Справка" : "Рекапитулация")
		<< " по съдии " << ends;
	  descr[sizeof descr - 1] = '\0';
	  wtdl->SetHeader(descr);
	  wtdl->PleaseWait();
	  endingVal = totalPartSteps;
	  wtdl->SetProgressRange(0, totalPartSteps);
	  passeds = "Стъпка";
	  reportFinishedItems = false;
	  fillArr(bP, eP, bF.getBkAreas(), bF.getBkAreasCnt());
	  for(int i = currentPartStep; i < totalPartSteps; i ++)
	  {                                                // Faking actions
	    if(wtdl->WantBreak() == false)
	    {
	     Tick(false);
	     Sleep(100);	// 2015:274 LPR: was 300
	    }
	  }
	  wtdl->SetProgress(totalPartSteps);
	  if(sayAlmostDone)
		wtdl->AlmostDone();
	}  // -- 1 --
	CATCH_ANYTHING
    }
  }
}

void TJudgeCheckupThread::prepare()
 {
  if (pArr)
   {
    makeBookFormatFN () ;
    Generate () ;
   }
 }

TJudgeCheckupThread::TJudgeCheckupThread(const begMYEndMYTypeCIU & rBMYEMYTCIU,
					int mainMenuItem ) :  // 20061207
					judgeCheckupEnum (),
					pArr (NULL),
					bMYEMYTCIU (rBMYEMYTCIU),
					trueJudgeCheckup (true),
					cCBDEDBN (cCBDEDBN_Empty), pWW (NULL),
		    cipherIfNotTrueGudgeCheckup (OFFSET_JUDGE_CHECKUP_CIPHER),
					pASRArr (NULL),
					sayAlmostDone (true)
 {
  pArr = new TISArray<judgeCheckupData> ;
   {
    collCompBegDEndDBegN tmp ;

    tmp << bMYEMYTCIU ;
    pASRArr = new overriddenResultsSessionsBS (wtdl->Parent, tmp) ;
    v_mainMenuItem = mainMenuItem;       // 20061207
   }
 }

TJudgeCheckupThread::TJudgeCheckupThread(const collCompBegDEndDBegN & rCCBDEDBN
				 , const unsigned int ciphIfNotTrueGudgeCheckup
				     ) :
					judgeCheckupEnum (),
					pArr (NULL),
					bMYEMYTCIU (rCCBDEDBN),
					trueJudgeCheckup (false),
					cCBDEDBN (rCCBDEDBN), pWW (NULL),
		     cipherIfNotTrueGudgeCheckup (ciphIfNotTrueGudgeCheckup),
					pASRArr (NULL),
					sayAlmostDone (true)  // 18.may.2002
 {
  pASRArr = new overriddenResultsSessionsBS (wtdl->Parent, cCBDEDBN) ;
  pArr = new TISArray<judgeCheckupData> ;
 }

TJudgeCheckupThread::~TJudgeCheckupThread()
 {
    delete pArr ;
    delete pASRArr ;
    delete pWW ;
 }

void TJudgeCheckupThread::makeGeneralRow(judgeCheckupData & rJCD)
 {
  bkFormatsPseudoOutput bF (formatFN, wtdl->Parent) ;
  const bookAreas * const bkAreas = bF.getBkAreas () ;
  const int areasCnt = bF.getBkAreasCnt () ;
  int i ;
  const bool nFBRequired =
//    columnRequired (iC_L_NotFinishedsBeg1_J, bkAreas, areasCnt) ||
    columnRequired (iC_L_NotFinishedsBeg1, bkAreas, areasCnt)
# if INSTANCE
                                                              ||
    columnRequired (iC_L_NotFinishedsBeg2, bkAreas, areasCnt)
# endif
                            ;
  const bool nFERequired =
    columnRequired (iC_L_NotFinishedsEnd1, bkAreas, areasCnt)
# if INSTANCE
                                                              ||
    columnRequired (iC_L_NotFinishedsEnd2, bkAreas, areasCnt)
# endif
                            ;

  rJCD.clearData () ;
  ostrstream (rJCD.judgeName, sizeof (rJCD.judgeName))
    << "ОБЩО" << ends ;
  rJCD.judgeName [sizeof (rJCD.judgeName) - 1] = '\0' ;
  for (i = 0 ; i < pArr -> Count () ; i ++)
   {  // -- -1 --
    const judgeCheckupData * pJCD = (* pArr) [i] ;

    rJCD += (* pJCD) ;
   }  // -- -1 --
  rJCD.jCS.fixeds2Judges1 /= 2 ;
  rJCD.jCS.fixeds3Judges1 /= 3 ;
  rJCD.jCS.lefts3Judges1 /= 3 ;
  rJCD.jCS.lefts3Judges1stSess1 /= 3 ;
  rJCD.jCS.lefts3Judges2ndSess1 /= 3 ;
# if INSTANCE
  rJCD.jCS.fixeds3Judges2 /= 3 ;
  rJCD.jCS.lefts3Judges2 /= 3 ;
  rJCD.jCS.lefts3Judges1stSess2 /= 3 ;
  rJCD.jCS.lefts3Judges2ndSess2 /= 3 ;
   { 
    // I'm not happy with this. It is possible that the error message
    // below will appear some time.
    const unsigned long int old = rJCD.jCS.fixedsIfNotJudge3_2 ;

    rJCD.jCS.fixedsIfNotJudge3_2 /= 2 ;
    if (rJCD.jCS.fixedsIfNotJudge3_2 * 2 != old)
     {
      rJCD.jCS.fixedsIfNotJudge3_2 = old ;  // Undo
      error ("Общият брой заседания за колоната\n"
             "\"Насрочени дела / II / в състава\" не може да се изчисли "
             "без остатък (%lu не се дели на 2). "
             "Оставям сумата неразделена.", old) ;
     }
   } 
# endif
  if (nFBRequired || nFERequired)
   {  // -- 0 --
    CDate bP, eP ;

    if (trueJudgeCheckup)
     {
      bP.day = eP.day = (char) 1 ;
      bP.month = bMYEMYTCIU.begMonth ;
      bP.year = bMYEMYTCIU.begYear ;
      eP.month = bMYEMYTCIU.endMonth ;
	eP.year = bMYEMYTCIU.endYear ;
     }
    else
     {
      bP = cCBDEDBN.begD ;
      eP = cCBDEDBN.endD ;
     }

    const long int iV = 1 ;
    const CDate begP = trueJudgeCheckup ? firstDayOfThisMonth (bP) :
                                            bP ;
    const CDate nextP = trueJudgeCheckup ? firstDayOfNextMonth (eP) :
                                             nextDay (eP) ;
    const size_t crSz = 512 + 256 + 3 * 64 ;
    char * crit = new char [crSz] ;
    const char * const what =
# if INSTANCE
//      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_KIND, L.F_JUDGE" ;
      "L.F_KIND" ;
# else
//      "DISTINCT L.F_NO, L.F_YEAR, L.F_TYPE, L.F_JUDGE" ;
      "L.F_KIND" ;
# endif

    if (nFBRequired)
     {  // -- 1 --
	 {  // -- 2 --
	  if (composeJudgeCheckupCriteria (crit, crSz, what,
						     bMYEMYTCIU.types,
                                         bMYEMYTCIU.composition,
                                         & begP, NULL,
                                         NULL,
                                         reqKindsString (),
                                         NULL,
                                         true,  // finishedTest
                                         NULL, NULL,
                                         NULL,  // sessKindsString,
                                         true,
                                         true   // excludeCreatedsLater,
                                        ))
         {
          TQuery q (crit) ;


	    while (q.Read ())
	     {
# if INSTANCE
#   if APPEAL
		rJCD.jCL.notFinishedsBeg2 += iV ;
#   else  // of APPEAL
		TKind k ;

		k << q ;
//            if (columnRequired (iC_L_NotFinishedsBeg1_J, bkAreas, areasCnt)
//                && strchr (KIND_1ST_LAWSUITS, k.kind))
//              rJCD.jCL.notFinishedsBeg1_J += iV ;
		if (columnRequired (iC_L_NotFinishedsBeg1, bkAreas, areasCnt)
		    && strchr (KIND_1ST_LAWSUITS, k.kind))
		{
		  rJCD.jCL.notFinishedsBeg1 += iV ;
		}
		if (columnRequired (iC_L_NotFinishedsBeg2, bkAreas, areasCnt)
		    && ! strchr (KIND_1ST_LAWSUITS, k.kind))
		  rJCD.jCL.notFinishedsBeg2 += iV ;
#   endif  // of APPEAL
# else
		if (columnRequired (iC_L_NotFinishedsBeg1, bkAreas, areasCnt) )
		{
		  rJCD.jCL.notFinishedsBeg1 += iV ;
//            if (columnRequired (iC_L_NotFinishedsBeg1_J, bkAreas, areasCnt)
//              rJCD.jCL.notFinishedsBeg1_J += iV ;
		}
# endif
	     }
	   }
	 }  // -- 2 --
     }  // -- 1 --
    if (nFERequired)
     {  // -- 4 --
	 {  // -- 5 --
	  if (composeJudgeCheckupCriteria (crit, crSz, what,
                                         bMYEMYTCIU.types,
                                         bMYEMYTCIU.composition,
                                         & nextP, NULL,
                                         NULL,
                                         reqKindsString (),
                                         NULL,
						     true,  // finishedTest
                                         NULL, NULL,
                                         NULL,  // sessKindsString,
                                         true,
						     true   // excludeCreatedsLater,
                                        ))
         {
          TQuery q (crit) ;

          while (q.Read ())
           {
# if INSTANCE
#   if APPEAL
            rJCD.jCL.notFinishedsEnd2 += iV ;
#   else  // of APPEAL
            TKind k ;

            k << q ;
            if (columnRequired (iC_L_NotFinishedsEnd1, bkAreas, areasCnt)
                && strchr (KIND_1ST_LAWSUITS, k.kind))
              rJCD.jCL.notFinishedsEnd1 += iV ;
            if (columnRequired (iC_L_NotFinishedsEnd2, bkAreas, areasCnt)
                && ! strchr (KIND_1ST_LAWSUITS, k.kind))
              rJCD.jCL.notFinishedsEnd2 += iV ;
#   endif  // of APPEAL  
# else
            rJCD.jCL.notFinishedsEnd1 += iV ;
# endif
           }
         }
	 }  // -- 5 --
     }  // -- 4 --
    if (crit)
      delete [] crit ;
   }  // -- 0 --
 }

void TJudgeCheckupThread::writeOneRow (const judgeCheckupData * pJCD, judgeCheckupOutFile & outF,
	const CDate * const pDateForMonthColumn, const int serialNo)
 {
  if ((! pJCD -> countingDataEmpty ()))
   {  // -- 5 --
    judgeCheckupColumns * pJCC =
	newJudgeCheckupColumns (formatFN, (* pJCD),
					pDateForMonthColumn, serialNo);
    judgeCheckupColumns & jCC = (* pJCC) ;
    const int jCCMaxRows = jCC.getMaxRows () ;
    int i ;

    if (outF.pageLinesLeft () < jCCMaxRows + 1 )
	outF.headAtNewPage () ;
    outF.horzLine () ;
    for (i = 0 ; i < jCCMaxRows ; i ++)
	outF.writeItem (jCC, true) ;
    delete pJCC ;
   }  // -- 5 --
 }

void TJudgeCheckupThread::prtJdgChkup()
{
	int linesWritten;

	if (!pArr)
		return;

	judgeCheckupOutFile *const pOutF = newOutFile();
	judgeCheckupOutFile &outF = (*pOutF);

	{  // -- 1 --
		int i ;
		TWaitWindow ww(wtdl->Parent, PLEASE_WAIT_GENERATING);

		outF.title();
		outF.formattedHead (&bMYEMYTCIU);
		outF.head();

		for (i = 0; i < pArr->Count(); i++)
			writeOneRow((*pArr) [i], outF, NULL, i + 1);

		judgeCheckupData *pJCDG = newDataItem();

		if (pJCDG)
		{
			makeGeneralRow(*pJCDG);
			writeOneRow(pJCDG, outF);
			delete pJCDG;
		}

		outF.footherLine();

		if (outF.pageLinesLeft() < (outF.formattedTail (& bMYEMYTCIU, false) + outF.tail(false) + 1))
			outF.newPage();

		outF.formattedTail (& bMYEMYTCIU);
		outF.tail();
		linesWritten = outF.pageLinesUsed();
	}  // -- 1 --

	if (linesWritten)
	{
		// Nothing. I'm suppressint the warning
	}

	show_text_stream_use_settings(NULL, outF, formatFN, FLOAT_ALIGNMENT);
	delete pOutF;
}

void TJudgeCheckupThread::printJudgeCheckup()
 {
  if (! pArr)
    return ;

  prtJdgChkup () ;
 }

const collCompBegDEndDBegN TJudgeCheckupThread::cCBDEDBN_Empty ;
