struct judgeCheckupLawsuits
 {
  void clearData () ;
  judgeCheckupLawsuits () { clearData () ; }
  bool operator == (const judgeCheckupLawsuits & otherJCL) const ;
  judgeCheckupLawsuits &
    operator += (const judgeCheckupLawsuits & otherJCL) ;
  bool isEmpty () const ;

  // If you change any data members, do not forget
  // to change the operator +=, too.
  unsigned long int notFinishedsBeg1 ;       //  !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
  unsigned long int news1 ;
  unsigned long int newsRejudge1 ;  // 11.dec.2003
	unsigned long int newsPunGenChar1 ;  // 10.jul.2002
	unsigned long int newsPunGenCharRejudge1 ;  // 11.dec.2003
	unsigned long int finishedsTotal1 ;
	unsigned long int finishedsWithDecision1 ;
	unsigned long int finishedsCanceleds1 ;
	unsigned long int finishedsUpTo3M1 ;
	unsigned long int finishedsOver3M1 ;
	unsigned long int abolisheds1 ;
	unsigned long int finishedsCanceledsByAgr1 ;
	unsigned long int finishedsCanceledsOtherC1 ;
	unsigned long int keepingMethodConfirmed1 ;
	unsigned long int keepingMethodChanged1 ;
	unsigned long int subject0210 ;
	unsigned long int subject0600 ;
	unsigned long int subject0500 ;
	unsigned long int subject0810 ;
	unsigned long int subjects0400_0410 ;
	unsigned long int subjects0220_0310_0800 ;
	unsigned long int decisDefinClosedS1 ;
	unsigned long int canceledsClosedS1 ;
# if INSTANCE
	unsigned long int notFinishedsBeg2 ;
	unsigned long int news2 ;
  unsigned long int newsRejudge2 ;  // 11.dec.2003
	unsigned long int newsPunGenChar2 ;  // 10.jul.2002
	unsigned long int newsPunGenCharRejudge2 ;  // 11.dec.2003
	unsigned long int finishedsTotal2 ;
	unsigned long int finishedsWithDecision2 ;
	unsigned long int finishedsCanceleds2 ;
	unsigned long int finishedsUpTo3M2 ;
  unsigned long int finishedsOver3M2 ;
  unsigned long int notFinishedsEnd2 ;
  unsigned long int finishedsCanceledsByAgr2 ;
  unsigned long int finishedsCanceledsOtherC2 ;
  unsigned long int exactComplaints2 ;
  unsigned long int privateComplaints2 ;
  unsigned long int decisDefinClosedS2 ;
  unsigned long int canceledsClosedS2 ;
#   if APPEAL
	unsigned long int newsPGenCharAplCourt ;
	unsigned long int newsPKeepOpenSessAplCourt ;
	unsigned long int newsPKeepClSessAplCourt ;
	unsigned long int newsPEnactmentAplCourt ;
	unsigned long int newsPOthersAplCourt ;
	unsigned long int decPGenCharAplCourt ;
	unsigned long int decPKeepOpenSessAplCourt ;
  unsigned long int decPKeepClSessAplCourt ;
  unsigned long int decPEnactmentAplCourt ;
  unsigned long int decPOthersAplCourt ;
#   endif  // of APPEAL
  unsigned long int detailNotFinishedBegTotal2 ;  // 05.apr.2002 - begin
  unsigned long int detailNotFinBegPunGen2 ;
  unsigned long int detailNotFinBegPunPrv2 ;
  unsigned long int detailNotFinishedEndTotal2 ;
  unsigned long int detailNotFinEndPunGen2 ;
  unsigned long int detailNotFinEndPunPrv2 ;     // 05.apr.2002 - end
# endif  // of INSTANCE
  unsigned long int detailNotFinishedBegTotal1 ;  // 05.apr.2002
  unsigned long int detailNotFinishedEndTotal1 ;  // 05.apr.2002

// 20061207 new fields --> defined for all courts,  used by REGIONAL COURT lawsuit type='c'
  unsigned long int newsRejudge1_J;// Постъпили дела - Новообразувани
  unsigned long int byJurisd_J;    // Постъпили дела - Получени по подсъдност
  unsigned long int returneds_J;   // Постъпили дела - Върнати за ново разглеждане

  unsigned long int completeds_Decision_RC_J;   // Свършени дела - С решение
  unsigned long int completeds_grant_entire_J;  // Свършени дела - Искът уважен изцяло
  unsigned long int completeds_grant_partial_J; // Свършени дела - Искът уважен отчасти
  unsigned long int completeds_grant_rejected_J;// Свършени дела - Искът отхвърлен
  unsigned long int completeds_ess_CS_RC_J;     // Свършени дела - Свършени с акт по същество в З.З
  unsigned long int completeds_canceleds_byAgreement_J;          // Свършени дела - Прекратени дела - По спогодба
  unsigned long int completeds_canceleds_byOtherCauses_OS_J;     // Свършени дела - Прекратени дела - Други причини - Otkrito заседание - Влезли в законова сила
  unsigned long int completeds_canceleds_byOtherCauses_CS_J;     // Свършени дела - Прекратени дела - Други причини - Закрито заседание - Влезли в законова сила
  unsigned long int completeds_canceleds_summByAgreeByOth_RC_J;  // Свършени дела - Прекратени дела - SUM

  unsigned long int notFinishedsBeg1_J ;    // Останали несвършени  в началото на отчетния период - RC CL report+judge distribution
  unsigned long int notFinishedsEnd1_J ;    // Останали несвършени дела в края на отчетния период

// 20061207 new fields   <--

  // 20070202 --> for RC penal.laws
  unsigned long int appealeds_RC_J;         // Обжалвани дела
  // 20070202 <--

  unsigned long int notFinishedsEnd1 ;       // !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()

  protected :
    size_t size () const ;
 } ;  // struct judgeCheckupLawsuits

struct judgeCheckupSessions
 {
  void clearData () ;
  judgeCheckupSessions () { clearData () ; }
  bool operator == (const judgeCheckupSessions & otherJCS) const ;
  judgeCheckupSessions &
    operator += (const judgeCheckupSessions & otherJCS) ;
  bool isEmpty () const ;

  // If any changes of the data members will be made, do not forget
  // to change the operator +=, too.
  unsigned long int fixeds1 ;       //  !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
  unsigned long int otherActs1 ;
  unsigned long int fixedsIncludingOtherActs1 ;
  unsigned long int fixeds1Judge1 ;
  unsigned long int fixeds2Judges1 ;
  unsigned long int fixeds3Judges1 ;
  unsigned long int lefts1stSess1 ;
  unsigned long int lefts2ndSess1 ;
  unsigned long int stoppeds1 ;
  unsigned long int fixedsIfNotJudge1 ;
  unsigned long int lefts3Judges1 ;
  unsigned long int lefts3Judges1stSess1 ;
  unsigned long int lefts3Judges2ndSess1 ;
  unsigned long int abolishedResults1 ;
  unsigned long int additionalDecisDefinOpenS1 ;
  unsigned long int additionalDecisDefinCldsS1 ;
# if INSTANCE
  unsigned long int fixeds2 ;
  unsigned long int lefts2 ;
  unsigned long int otherActs2 ;
  unsigned long int fixedsIncludingOtherActs2 ;
  unsigned long int fixedsIfInMembership2 ;            // not in use yet
  unsigned long int leftsIfInMembership2 ;             // not in use yet
  unsigned long int otherActsIfInMembership2 ;         // not in use yet
  unsigned long int fixedsIncludOthActsIfInMembersh2 ; // not in use yet
  unsigned long int fixeds3Judges2 ;
  unsigned long int lefts1stSess2 ;
  unsigned long int lefts2ndSess2 ;
  unsigned long int stoppeds2 ;
  unsigned long int fixedsIfNotJudge3_2 ;
  unsigned long int lefts3Judges2 ;
  unsigned long int lefts3Judges1stSess2 ;
  unsigned long int lefts3Judges2ndSess2 ;
  unsigned long int abolishedResults2 ;
  unsigned long int additionalDecisDefinOpenS2 ;
  unsigned long int additionalDecisDefinCldsS2 ;
  unsigned long int appeal192_193sessRes2 ;  // 24.nov.2000
# endif

  // 20061207 new fields --> defined for all courts,  used by REGIONAL COURT lawsuit type='c'
  unsigned long int  lefts_reconcileSess_J ;     // Отложени дела - Помирителен срок
  unsigned long int  lefts_BeforeFirstSess_RC_J; // Отложени дела - Преди първо заседание
  unsigned long int  lefts_Interim_RC_J;         // Отложени дела - Заседание по привременни мерки и друго заседание
  unsigned long int  lefts_total_RC_J;           // Отложени дала - Отложени ОБЩО

  unsigned long int  split_J;                    // Допускане на делба
  unsigned long int  otherSessAfterFinish_J;     // Допълване на решение, определение

  unsigned long int lefts1stSess1_J ;            // Отложени дела - В първо заседание
  unsigned long int lefts2ndSess1_J ;            // Отложени дела - Във II зас.и следващо съд.зас.

  unsigned long int fixeds_RC_J;                 // Насрочени дела
  // 20061207 new fields <--

  unsigned long int lefts1 ;       // !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()

  protected :
    size_t size () const ;
 } ;  // struct judgeCheckupSessions

#if defined(USE_JUDGE_CHECKUP_OTHERS)
#error USE_JUDGE_CHECKUP_OTHERS must not de defined here
#endif

#define USE_JUDGE_CHECKUP_OTHERS 1

#if USE_JUDGE_CHECKUP_OTHERS
struct judgeCheckupOthers
 {  // 07.jun.2004
  void clearData () ;
  judgeCheckupOthers () { clearData () ; }
  bool operator == (const judgeCheckupOthers & otherJCO) const ;
  judgeCheckupOthers &
    operator += (const judgeCheckupOthers & otherJCO) ;
  bool isEmpty () const ;

  // If any changes of the data members will be made, do not forget
  // to change the operator +=, too.
  unsigned long int reqResolutions1 ;       //  !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
# if INSTANCE
  unsigned long int reqResolutions2 ;
  unsigned long int inRegEvents2 ;
# endif  // of INSTANCE
  unsigned long int inRegEvents1 ;       // !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()

  protected :
    size_t size () const ;
 } ;  // struct judgeCheckupOthers
#endif  // of USE_JUDGE_CHECKUP_OTHERS 

struct judgeCheckupData
 {
  virtual  // 04.dec.2000
  void clearData () ;
  judgeCheckupData () { clearData () ; }
  virtual  // 04.dec.2000
  bool countingDataEmpty () const ;

  bool operator<=(const judgeCheckupData &that) const { return operator<(that) || operator==(that); }
private:
  bool operator==(const judgeCheckupData & other) const;
  bool operator<(const judgeCheckupData & other) const;

public:
  virtual void operator += (const judgeCheckupData & other) ;//04.dec.2000

  char judgeName [SIZE_OF_NAME] ;
  char judgeUCN [SIZE_OF_UCN] ;
  long int judgeComposit ;
  judgeCheckupLawsuits jCL ;
  judgeCheckupSessions jCS ;
#if USE_JUDGE_CHECKUP_OTHERS
  judgeCheckupOthers jCO ;  // 07.jun.2004
#endif  // of USE_JUDGE_CHECKUP_OTHERS
  char judgePost ;
 } ;  // struct judgeCheckupData

// 04.dec.2000 - begin
# if INSTANCE
#   if APPEAL
struct IlievaJCData
 {
  void ClearData () ;
  IlievaJCData () { ClearData () ; }
  bool operator == (const IlievaJCData & otherIJCD) const ;
  IlievaJCData &
    operator += (const IlievaJCData & otherIJCD) ;
  bool IsEmpty () const ;

  // If you changes any data members, do not forget
  // to change the operator +=, too.
  unsigned long int IlievaFin2ndTotal ;       //  !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
  unsigned long int IlievaFin2ndCancelClosed ;
  unsigned long int IlievaFin2ndCancelOpen ;
  unsigned long int IlievaFin2nsDecUpTo3Mon ;
  unsigned long int IlievaFin2ndDecOver3Mon ;
  unsigned long int IlievaFinPrvTotal ;

  unsigned long int IlievaFinPrvPrvCompl ;
  unsigned long int IlievaFinPrvPrvCompl_Decis ;  // 20060417
  unsigned long int IlievaFinPrvPrvCompl_Ceased ; // 20060417

  unsigned long int IlievaFinPrvSlowdown ;
  unsigned long int IlievaFinPrvSlowdown_Decis ;  // 20060417
  unsigned long int IlievaFinPrvSlowdown_Ceased ; // 20060417

  unsigned long int IlievaOthTotal ;
  unsigned long int IlievaOth205Closed ;
  unsigned long int IlievaOthReqResol ;

  unsigned long int inRegResol_AC ;  // 20050804

  unsigned long int Ilieva192_193 ;
  unsigned long int IlievaBankruptcyFinTotal ;  // 07.feb.2001 - begin
  unsigned long int IlievaBankruptcyDec2nd_OS ;
  unsigned long int IlievaBankruptcyDec2nd_CS ;
  unsigned long int IlievaBankruptcyDecPrv ;
  unsigned long int IlievaOthOpenSessTotal ;  // 07.feb.2001 - end
  unsigned long int IlievaBankruptcyFixed_OS ;  // 20.feb.2001 - begin
  unsigned long int IlievaBankruptcyDecided_OS ;
  unsigned long int IlievaBankruptcyCanceled_OS ;
  unsigned long int IlievaBankruptcyCancel2nd ;
  unsigned long int IlievaBankruptcyCancelPrv ;
  unsigned long int IlievaBankruptcyFin_OS ;
	unsigned long int IlievaBankruptcyLeft_OS ;  // 20.feb.2001 - end
	unsigned long int IlievaFinPrvPetitions ;  // 11.jun.2002 - begin
	unsigned long int IlievaOthPendingDecree ;
	unsigned long int IlievaOthPendingDefinition ;
	unsigned long int IlievaOthAftFinDecisionOS ;
	unsigned long int IlievaOthAftFinDefinitionOS ;
	unsigned long int IlievaOthAftFinDefinitionCS ;
	unsigned long int IlievaOthAftFinDecree ;
	unsigned long int IlievaOthPending205 ;  // 11.jun.2002 - end
	unsigned long int IlievaOthStopedUnmoveableCS ;  // 19.jun.2002
	unsigned long int IlievaOth192p4 ;       // !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()

  protected :
    size_t Size () const ;
 } ;  // struct IlievaJCData

struct IlievaJudgeCheckupData : public judgeCheckupData
 {
  virtual void clearData () ;
  IlievaJudgeCheckupData () { clearData () ; }
  virtual bool countingDataEmpty () const ;
  virtual void operator += (const judgeCheckupData & other) ;

  IlievaJCData IlievaData ;
 } ;  // struct IlievaJudgeCheckupData

#   else  // of APPEAL

struct KracholovJCData
 {
  void ClearData () ;
  KracholovJCData () { ClearData () ; }
  bool operator == (const KracholovJCData & otherKJCD) const ;
  KracholovJCData &
    operator += (const KracholovJCData & otherKJCD) ;
  bool IsEmpty () const ;

  // If any changes of the data members will be made, do not forget
  // to change the operator +=, too.
  unsigned long int S_KracholovReporteds1 ;       //  !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
  unsigned long int S_KracholovReportedsGenChar1 ;
  unsigned long int S_KracholovReportedsLeft1 ;
  unsigned long int L_KracholovFinisheds1 ;
  unsigned long int L_KracholovFinsGenChJudgem1 ;
  unsigned long int L_KracholovFinsGenCh241_1 ;
  unsigned long int L_KracholovFinsGenCh287_1 ;
  unsigned long int L_KracholovFinsPrvCh1 ;
  unsigned long int L_KracholovFinsPrvPreCrt1 ;
  unsigned long int L_KracholovFinsAdmin1 ;
  unsigned long int S_KracholovReporteds2 ;
  unsigned long int S_KracholovReporteds2nd2 ;
  unsigned long int S_KracholovReporteds2ndLeft2 ;
  unsigned long int S_KracholovReportedsPrvC2 ;
  unsigned long int S_KracholovReportedsAdm2 ;
  unsigned long int S_KracholovReportedsAdmLeft2 ;
  unsigned long int S_KracholovReportedsPreCrt2 ;
  unsigned long int L_KracholovFinisheds2 ;
  unsigned long int L_KracholovFinisheds2nd2 ;
  unsigned long int L_KracholovFinishedsPrvC2 ;
  unsigned long int L_KracholovFinishedsAdm2 ;
  unsigned long int L_KracholovFinishedsPreCrt2 ;
  unsigned long int S_KracholovReportedsTotal ;
  unsigned long int S_KracholovUWAOver30d ;
  unsigned long int L_KracholovFinishedsTotal ;  // !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()

  protected :
    size_t Size () const ;
 } ;  // struct KracholovJCData

struct KracholovJudgeCheckupData : public judgeCheckupData
 {
  virtual void clearData () ;
  KracholovJudgeCheckupData () { clearData () ; }
  virtual bool countingDataEmpty () const ;
  virtual void operator += (const judgeCheckupData & other) ;

  KracholovJCData KracholovData ;
 } ;  // struct KracholovJudgeCheckupData

struct PdCitJCData
 {
  void ClearData () ;
  PdCitJCData () { ClearData () ; }
  bool operator == (const PdCitJCData & otherPdCitJCD) const ;
  PdCitJCData &
    operator += (const PdCitJCData & otherPdCitJCD) ;
  bool IsEmpty () const ;

  // If any changes of the data members will be made, do not forget
  // to change the operator +=, too.
  unsigned long int L_PdCitForExamineTotal1 ;       //  !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
  unsigned long int L_PdCitForExamineTotal2 ;
  unsigned long int S_PdCitExamined1 ;
  unsigned long int S_PdCitExamined2 ;
  unsigned long int L_PdCitFinishComlp2 ;
  unsigned long int L_PdCitFinishPrvComlp2 ;
  unsigned long int L_PdCitFinishedsTotal ;
  unsigned long int L_PdCitDecidedsComlp2 ;
  unsigned long int L_PdCitDecidedsPrvComlp2 ;
  unsigned long int L_PdCitDecidedsTotal ;
  unsigned long int L_PdCitCanceledsComlp2 ;
  unsigned long int L_PdCitCanceledsPrvComlp2 ;
  unsigned long int L_PdCitCanceledsTotal ;
  unsigned long int S_PdCitUWA_UpTo1m1 ;
  unsigned long int S_PdCitUWA_UpTo1mCompl2 ;
  unsigned long int S_PdCitUWA_UpTo1mPrvCompl2 ;
  unsigned long int S_PdCitUWA_Over1m1 ;
  unsigned long int S_PdCitUWA_Over1mCompl2 ;
  unsigned long int S_PdCitUWA_UpTo1mTotal ;
  unsigned long int S_PdCitUWA_Over1mTotal ;
  unsigned long int L_PdCitFinishUpTo3MComlp2 ;
  unsigned long int L_PdCitFinishUpTo3MPrvComlp2 ;
  unsigned long int L_PdCitFinishUpTo3MTotal ;
  unsigned long int L_PdCitFinishOver3MComlp2 ;
  unsigned long int L_PdCitFinishOver3MPrvComlp2 ;
  unsigned long int L_PdCitFinishOver3MTotal ;
  unsigned long int S_PdCitUWA_Over1mPrvCompl2 ;  // !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()

  protected :
    size_t Size () const ;
 } ;  // struct PdCitJCData

struct PdCitJudgeCheckupData : public judgeCheckupData
 {
  virtual void clearData () ;
  PdCitJudgeCheckupData () { clearData () ; }
  virtual bool countingDataEmpty () const ;
  virtual void operator += (const judgeCheckupData & other) ;

  PdCitJCData PdCitData ;
 } ;  // struct PdCitJudgeCheckupData

// 03.mar.2001 - begin
struct FirmJCData
 {
  void ClearData () ;
  FirmJCData () { ClearData () ; }
  bool operator == (const FirmJCData & otherFirmJCD) const ;
  FirmJCData &
    operator += (const FirmJCData & otherFirmJCD) ;
  bool IsEmpty () const ;

  // If any changes of the data members will be made, do not forget
  // to change the operator +=, too.
  unsigned long int L_Firm_Feceived ;       //  !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
  unsigned long int L_Firm_FeceivedRejudge ;  // 11.dec.2003
  unsigned long int D_Firm_Entered ;
  unsigned long int D_Firm_RegistrationRefused ;
  unsigned long int D_Firm_ChangeRefused ;
  unsigned long int D_Firm_Erased ;
  unsigned long int D_Firm_CancelDecision ;
  unsigned long int D_Other_TradeRegister ;  // 21.feb.2005
  unsigned long int D_Other_NoTradeRegister ;  // 21.feb.2005
  unsigned long int D_Firm_Changed ;  // !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()

  protected :
    size_t Size () const ;
 } ;  // struct FirmJCData

struct FirmJudgeCheckupData : public judgeCheckupData
 {
  virtual void clearData () ;
  FirmJudgeCheckupData () { clearData () ; }
  virtual bool countingDataEmpty () const ;
  virtual void operator += (const judgeCheckupData & other) ;

  FirmJCData firmData ;
 } ;  // struct FirmJudgeCheckupData
// 03.mar.2001 - end
#   endif  // of APPEAL
# endif  // of INSTANCE
// 04.dec.2000 - end
