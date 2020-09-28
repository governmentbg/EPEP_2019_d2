#include "cac.h"  // 18.may.2001
//# include "all.h"

size_t reportLawsuits :: size () const
 {
  const char * b = (const char *) (& notDecisionBeg) ;
  const char * e = (const char *) (& notDecisionEnd) +
                   sizeof (notDecisionEnd) ;
  return (e - b) ;                 
 }  // reportLawsuits :: size
 
void reportLawsuits :: clearData ()
 {
  setmem (& notDecisionBeg, size (), '\0') ;
 }  // reportLawsuits :: clearData

bool reportLawsuits :: operator == (const reportLawsuits & otherRL) const
 {
  return ! memcmp (& notDecisionBeg, & (otherRL.notDecisionBeg),
                   size ()) ;
 }  // reportLawsuits :: operator ==

bool reportLawsuits :: isEmpty () const
 {
  reportLawsuits rL1 ;

  return ((* this) == rL1) ;
 }  // reportLawsuits :: isEmpty

const reportLawsuits & reportLawsuits ::
  operator << (const judgeCheckupLawsuits & jCL)
 {
  clearData () ;

  // 20061207 after 1.3014 Справки за дейнстта на съда/За вътрешно ползване - Разпределение по съдии

//N
  // 16003 ; к.1 Останали несв.в началото на отчетния период // iC_L_NotDecisionBeg - rpLaws.notDecisionBeg
  notDecisionBeg = jCL.notFinishedsBeg1_J;  // 25225 - iC_L_NotFinishedsBeg1_J

//N
  // 16044 ; к.2 Постъпили дела - Новообразувани // iC_L_Receiveds_ByInDoc - rpLaws.receiveds.byInDoc
  receiveds.byInDoc   = jCL.newsRejudge1_J; //  25230 - iC_L_NewsRejudge1_J - Постъпили дела - Новообразувани
                                            //P 25191 - iC_L_NewsRejudge1 - Постъпили дела - I-ва инстанция

//N                                                         iC_L_Receiv
  // 16035 ; к.3 Постъпили дела - Получени по подсъдност // iC_L_Receiveds_ByJurisd - rpLaws.receiveds.byJurisd
  receiveds.byJurisd  = jCL.byJurisd_J; // iC_L_byJurisd_J     25207

//N
  // 16036 ; к.4 Постъпили дела - Върнати за ново разглеждане // iC_L_Receiveds_Returneds - rpLaws.receiveds.returneds
  receiveds.returneds = jCL.returneds_J;// iC_L_returneds_J    25208

//N
  // 16004 ; к.5 Постъпили дела - Общо - за отчета - iC_L_Receiveds_Total - rpLaws.receiveds.total
  //        Сума на к.2 + к.3 + к.4   iC_L_Receiveds_Total_J 25209
  receiveds.total =   receiveds.byInDoc
                    + receiveds.byJurisd
                    + receiveds.returneds;
//N
  // 16047 ; к.6 Всичко дела за разглеждане // iC_L_ForExamineEval - rpLaws.forExamineEval
  //            Сума на к.1 + к.5                            25210
  forExamineEval = notDecisionBeg+receiveds.total;

//N SESS
  // 16086 ; к.7  Насрочени дела - селект;
  //  RC only     iC_S_Fixeds_RC    pRpSess -> fixeds_RC
  // fixeds_RC    =  jCS.fixeds_RC_J; // iC_S_Fixeds_RC_J - jCS.fixeds_RC_J - 25228


// SESS
  // 16030 ; k.8    Спрени дела    //  iC_S_Stoppeds -  s.stoppeds
  //stoppeds     =  jCS.stoppeds1; // iC_S_Stoppeds1 - jCS.stoppeds1   25046

//N SESS
  // 16074 ; k.9    Отложени дела - Помирителен срок     	; сумира се в к."Отложени ОБЩО"
  // RC only        iC_S_Lefts_ReconcileSess - s.lefts.reconcileSess
  // lefts.reconcileSess  = jCS.lefts_reconcileSess_J ; // iC_S_Lefts_ReconcileSess_J - 25211

//N SESS
  //16087  3    ; k.10   Отложени дела - Преди първо заседание  	; сумира се в к."Отложени ОБЩО"
  // RC only        iC_S_Lefts_BeforeFirstSess_RC - pRpSess -> lefts.BeforeFirstSess_RC
  // lefts.BeforeFirstSess_RC = jCS.lefts_BeforeFirstSess_RC_J ; // iC_S_Lefts_BeforeFirstSess_J  - 25212

//N SESS
  //16094  3    ; k.11   Отложени дела - В първо заседание   		; сумира се в к."Отложени ОБЩО"
  // RC only        iC_S_Lefts_FirstSess_RC - pRpSess -> lefts.firstSess_RC
  // lefts.firstSess_RC = jCS.lefts1stSess1_J ; // iC_S_Lefts1stSess1_J  - 25226

//N SESS
  //16043  3    ; k.12   Отложени дела - Във второ заседание и следващо съдебно заседание	; сумира се в к."Отложени ОБЩО"
  //                iC_S_Lefts_SecondSess -  pRpSess -> lefts.secondSess
  // lefts.secondSess = jCS.lefts2ndSess1_J     // iC_S_Lefts2ndSess1_J - 25227

//N SESS
  //16088  3    ; k.12а  Отложени дела - Заседание по привременни мерки и друго заседание	; сумира се в к."Отложени ОБЩО"
  //RC only         iC_S_LeftsInterim_RC  -  pRpSess -> lefts.Interim_RC
  // lefts.Interim_RC = jCS.lefts_Interim_RC_J  // iC_S_Lefts_Interim_RC_J  - 25213

//N SESS
  //16092  3    ; k.13   Отложени дала - Отложени ОБЩО ; 16029
  //RC only         iC_S_Lefts_Total_RC  -  pRpSess -> lefts.total_RC
  // lefts.total_RC = jCS.lefts_total_RC_J      // iC_S_Lefts_total_RC_J  - 25214

  //16046  4    ; k.14   Свършени дела - ОБЩО
  //                iC_L_Completeds_Total - rpLaws.completeds.total
  completeds.total = jCL.finishedsTotal1;     // iC_L_FinishedsTotal1 - 25006

//N
  //16089  4    ; k.15   Свършени дела - С решение
  //RC only         iC_L_Completeds_Decision_RC - rpLaws.completeds.decision_RC
  completeds.decision_RC = jCL.completeds_Decision_RC_J;  // iC_L_Completeds_Decision_RC_J - 25229

//N
  //16037  4    ; k.16   Свършени дела - Искът уважен изцяло
  //                iC_L_Completeds_Grant_Entire -  rpLaws.completeds.grant.entire
  completeds.grant.entire = jCL.completeds_grant_entire_J;  // iC_L_Completeds_Grant_Entire_J - 25215

//N
  //16038  4    ; k.17   Свършени дела - Искът уважен отчасти
  //                iC_L_Completeds_Grant_Partial -  rpLaws.completeds.grant.partial
  completeds.grant.partial = jCL.completeds_grant_partial_J;//  iC_L_Completeds_Grant_Partial_J - 25216

//N
  //16039  4    ; k.18   Свършени дела - Искът отхвърлен
  //                iC_L_Completeds_Grant_Rejected - rpLaws.completeds.grant.rejected
  completeds.grant.rejected = jCL.completeds_grant_rejected_J;//iC_L_Completeds_Grant_Rejected_J - 25217

//N
  //16090  4    ; k.19   Свършени дела - Свършени с акт по същество в З.З
  //                iC_L_Completeds_ESS_CS_RC - rpLaws.completeds.ess_CS_RC
  completeds.ess_CS_RC = jCL.completeds_ess_CS_RC_J ; // iC_L_Completeds_ess_CS_RC_J - 25218

//N
  //16040  4    ; k.20   Свършени дела - Прекратени дела - По спогодба   ; сумира се в к."Свършени дела - Прекратени дела - ОБЩО"
  //                iC_L_Completeds_Canceleds_ByAgr - rpLaws.completeds.canceleds.byAgreement
                    //jCL.completeds_canceleds_byAgreement;//  iC_L_Completeds_canceleds_byAgreement - 25219
  completeds.canceleds.byAgreement = jCL.completeds_canceleds_byAgreement_J ;  //P - iC_L_FinishedsCanceledsByAgr1 - 25038

//    0  4    ; k.21   Свършени дела - Прекратени дела - Други причини - Съдебно заседание - Невлвзли в законова сила

  //16081  4    ; k.22   Свършени дела - Прекратени дела - Други причини - Съдебно заседание - Влезли в законова сила     ; сумира се в к."Свършени дела - Прекратени дела - ОБЩО"
  //                iC_L_Complets_Cancels_ByOthC_OS -  rpLaws.completeds.canceleds.byOtherCauses_OS
  completeds.canceleds.byOtherCauses_OS = jCL.completeds_canceleds_byOtherCauses_OS_J;//jCL.completeds_canceleds_byOtherCauses_OS;
                                        //N - iC_L_Completeds_canceleds_byOtherCauses_OS_J - 25220
                                        //P - iC_L_FinishedsCanceledsOtherC1 - 25040

//    0  4    ; k.23   Свършени дела - Прекратени дела - Други причини - Закрито заседание - Невлезли в законова сила

//N
  //16082  4    ; k.24   Свършени дела - Прекратени дела - Други причини - Закрито заседание - Влезли в законова сила     ; сумира се в к."Свършени дела - Прекратени дела - ОБЩО"
  //                iC_L_Complets_Cancels_ByOthC_CS - rpLaws.completeds.canceleds.byOtherCauses_CS
  completeds.canceleds.byOtherCauses_CS = jCL.completeds_canceleds_byOtherCauses_CS_J;
                                        //iC_L_Completeds_canceleds_byOtherCauses_CS_J - 25221
//N
  //16095  4    ; k.25   Свършени дела - Прекратени дела - ОБЩО
  //                iC_L_Completeds_Canceleds_Summ_RC - rpLaws.completeds.canceleds.summByAgreeByOth_RC
  completeds.canceleds.summByAgreeByOth_RC = jCL.completeds_canceleds_summByAgreeByOth_RC_J;
                                        // jCL.completeds_canceleds_byAgreement_J
                                        //+jCL.completeds_canceleds_byOtherCauses_OS_J
                                        //+jCL.completeds_canceleds_byOtherCauses_CS_J;
                                        //iC_L_Completeds_canceleds_summByAgreeByOth_RC_J - 25222

  //16013  4    ; k.26   Продължителност - до 3 м
  //                iC_L_Age_UpTo3Months - rpLaws.age.upTo3Months
  age.upTo3Months = jCL.finishedsUpTo3M1 ;  //iC_L_FinishedsUpTo3M1 - 25012

  //16014  4    ; k.27   Продължителност - над 3 м
  //                 iC_L_Age_Over3Months - rpLaws.age.over3Months
  age.over3Months = jCL.finishedsOver3M1 ;  //iC_L_FinishedsOver3M1 - 25014

// SESS
  //16077  4    ; k.28   Допускане на делба
  //                 iC_S_Splits] - pRpSess -> splits
  //splits = jCS.split_J;  //   iC_S_Splits_J - 25223

// SESS
  //16080  4    ; k.29   Допълване на решение, определение
  //                  IC_S_OtherSessAfterFinish  -  pRpSess -> otherSessAfterFinish
  // otherSessAfterFinish = jCS.otherSessAfterFinish;

//    0  4      ; k.30   Други съдебни актове от съдебно заседание

//N
  //16016  4    ; k.31   Останали несвършени дела в края на отчетния период
  // 25231             iC_L_NotDecisionEnd -> rpLaws.notDecisionEnd
  notDecisionEnd = jCL.notFinishedsEnd1_J; //   iC_L_NotFinishedsEnd1_J
                      // P  25016

  //16091  4    ; k.32   Обжалвани дела - брой изпратени въззивни жалби и частни жалби
  //16051       - за Наказателни дела reportpi.fmt
  //  0    4
  // 20070202->
  appealeds_RC = jCL.appealeds_RC_J;

//

// from Petyo -->
  receiveds.news = jCL.news1 ;
//R  completeds.total = jCL.finishedsTotal1 ;
  completeds.withDecision = jCL.finishedsWithDecision1 ;
  completeds.canceleds.total = jCL.finishedsCanceleds1 ;
//R  age.upTo3Months = jCL.finishedsUpTo3M1 ;
//R  age.over3Months = jCL.finishedsOver3M1 ;
  // = jCL.abolisheds1 ;  // for this nothing yet
//R  completeds.canceleds.byAgreement = jCL.finishedsCanceledsByAgr1 ;
//R  completeds.canceleds.byOtherCauses = jCL.finishedsCanceledsOtherC1 ;
//R  notDecisionEnd = jCL.notFinishedsEnd1 ;
  //  completeds.canceleds.summByAgreeByOth_RC ; do nothing 20060627
  return (* this) ;
 }  // const reportLawsuits & reportLawsuits ::
    //   operator << (const judgeCheckupLawsuits & jCL)

size_t reportCitizens :: size () const
 {
  const char * b = (const char *) (& (judgeds.total)) ;
  const char * e = (const char *) (& complementPunishments) +
                   sizeof (complementPunishments) ;
  return (e - b) ;
 }  // reportCitizens :: size

void reportCitizens :: clearData ()
 {
  setmem (& (judgeds.total), size (), '\0') ;
 }  // reportCitizens :: clearData

bool reportCitizens :: operator == (const reportCitizens & otherRC) const
 {
  return ! memcmp (& (judgeds.total), & (otherRC.judgeds.total),
                   size ()) ;
 }  // reportCitizens :: operator

bool reportCitizens :: isEmpty () const
 {
  reportCitizens rC1 ;

  return ((* this) == rC1) ;
 }  //reportCitizens :: isEmpty
 
size_t reportSessions :: size () const
 {
  const char * b = (const char *) (& fixeds) ;
  const char * e = (const char *) (& stoppeds) + sizeof (stoppeds) ;
  return (e - b) ;
 }  // reportSessions :: size

void reportSessions :: clearData ()
 {
  setmem (& fixeds, size (), '\0') ;
 }  // reportSessions :: clearData

bool reportSessions :: operator == (const reportSessions & otherRS) const
 {
  return ! memcmp (& fixeds, & (otherRS.fixeds), size ()) ;
 }  // reportSessions :: operator

bool reportSessions :: isEmpty () const
 {
  reportSessions rS1 ;

  return ((* this) == rS1) ;
 }  // reportSessions :: isEmpty

const reportSessions & reportSessions ::
  operator << (const judgeCheckupSessions & jCS)
 {
  clearData () ;
// 20061207 -->
//N SESS
  // 16086 ; к.7  Насрочени дела - селект;
  //  RC only     iC_S_Fixeds_RC    pRpSess -> fixeds_RC
  fixeds_RC    =  jCS.fixeds_RC_J; // iC_S_Fixeds_RC_J - jCS.fixeds_RC_J - 25228

// SESS
  // 16030 ; k.8    Спрени дела    // iC_S_Stoppeds -  s.stoppeds
  stoppeds     =  jCS.stoppeds1;   // iC_S_Stoppeds1 - jCS.stoppeds1   25046

//N SESS
  // 16074 ; k.9    Отложени дела - Помирителен срок     	; сумира се в к."Отложени ОБЩО"
  // RC only        iC_S_Lefts_ReconcileSess - s.lefts.reconcileSess
  lefts.reconcileSess  = jCS.lefts_reconcileSess_J ; // iC_S_Lefts_ReconcileSess_J - 25211

//N SESS
  //16087  3    ; k.10   Отложени дела - Преди първо заседание  	; сумира се в к."Отложени ОБЩО"
  // RC only        iC_S_Lefts_BeforeFirstSess_RC - pRpSess -> lefts.BeforeFirstSess_RC
  lefts.BeforeFirstSess_RC = jCS.lefts_BeforeFirstSess_RC_J ; // iC_S_Lefts_BeforeFirstSess_J  - 25212

//N SESS
  //16094  3    ; k.11   Отложени дела - В първо заседание   		; сумира се в к."Отложени ОБЩО"
  // RC only        iC_S_Lefts_FirstSess_RC - pRpSess -> lefts.firstSess_RC
  lefts.firstSess_RC = jCS.lefts1stSess1_J ;  // iC_S_Lefts1stSess1_J  - 25226

//N SESS
  //16043  3    ; k.12   Отложени дела - Във второ заседание и следващо съдебно заседание	; сумира се в к."Отложени ОБЩО"
  //                iC_S_Lefts_SecondSess -  pRpSess -> lefts.secondSess
  lefts.secondSess   = jCS.lefts2ndSess1_J ;     // iC_S_Lefts2ndSess1_J - 25227

//N SESS
  //16088  3    ; k.12а  Отложени дела - Заседание по привременни мерки и друго заседание	; сумира се в к."Отложени ОБЩО"
  //RC only         iC_S_LeftsInterim_RC  -  pRpSess -> lefts.Interim_RC
  lefts.Interim_RC = jCS.lefts_Interim_RC_J ;  // iC_S_Lefts_Interim_RC_J  - 25213

//N SESS
  //16092  3    ; k.13   Отложени дала - Отложени ОБЩО ; 16029
  //RC only         iC_S_Lefts_Total_RC  -  pRpSess -> lefts.total_RC
  lefts.total_RC = jCS.lefts_total_RC_J ;     // iC_S_Lefts_total_RC_J  - 25214





//N SESS
  //16077  4    ; k.28   Допускане на делба
  //                 iC_S_Splits] - pRpSess -> splits
  splits = jCS.split_J;  //   iC_S_Splits_J - 25223

// SESS
  //16080  4    ; k.29   Допълване на решение, определение
  //                  IC_S_OtherSessAfterFinish  -  pRpSess -> otherSessAfterFinish_J
  otherSessAfterFinish = jCS.otherSessAfterFinish_J;   // iC_S_otherSessAfterFinish_J  25224   




// 20061207 <--



  // From Petyo -->
  fixeds = jCS.fixeds1 ;
  // = jCS.otherActs1 ;  // for this nothing yet
  // = jCS.fixedsIncludingOtherActs1 ;  // for this nothing yet
  if (! fixeds)
    fixeds = jCS.fixeds1Judge1 +
             jCS.fixeds2Judges1 / 2 + jCS.fixeds3Judges1 / 3 ;
  lefts.firstSess = jCS.lefts1stSess1 ;
  //R lefts.secondSess = jCS.lefts2ndSess1 ;
  stoppeds = jCS.stoppeds1 ;
  lefts.total = jCS.lefts1 ;
  return (* this) ;
 }  //  const reportSessions & reportSessions ::
    //    operator << (const judgeCheckupSessions & jCS)

void reportData :: clearData ()
 {
  subject = lowSubjRange = highSubjRange = 0 ;
  setmem (subjName, sizeof (subjName), '\0') ;
  covered = false ;
  l.clearData () ;
  c.clearData () ;
  s.clearData () ;
  *rowLetters_VSS = '\0';
 }  // reportData :: clearData

bool reportData :: countingDataEmpty () const
 {
  return (l.isEmpty () && c.isEmpty () && s.isEmpty ()) ;
 }  // reportData :: countingDataEmpty
 
bool reportData :: operator == (const reportData & other) const
 {
  return subject == other.subject ;
 }  // reportData :: operator ==

bool reportData :: operator < (const reportData & other) const
 {
  return subject < other.subject ;
 }  // reportData :: operator <

const reportData & reportData ::
  operator << (const judgeCheckupData & jCD)
 {
  clearData () ;
  scSprintf (sizeof (subjName), subjName, "%s", jCD.judgeName) ;
  l << jCD.jCL ;
  s << jCD.jCS ;
  return (* this) ;
 }  //  const reportData & reportData ::
    //    operator << (const judgeCheckupData & jCD)

bool reportData::InsideRange(long tSubject) const
{
	return TSubjectRange::InsideRange(lowSubjRange, highSubjRange, tSubject);
}
