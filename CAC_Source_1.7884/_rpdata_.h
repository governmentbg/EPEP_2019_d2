struct reportLawsuits
 {
  void clearData () ;
  reportLawsuits () { clearData () ; }
  bool operator == (const reportLawsuits & otherRL) const ;
  bool isEmpty () const ;
  const reportLawsuits & operator << (const judgeCheckupLawsuits & jCL) ;

  unsigned long int notDecisionBeg ;           //  3   !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
  unsigned long int notDecisionBeg_VSS ;       // 16128 - Останали несв.- I и II инстанция
  struct
   {
    unsigned long int total ;                  //  4
    unsigned long int total_VSS ;              // 16129 - Постъпили всичко - I и II инстанция
    unsigned long int news ;                   //  5
    unsigned long int news_corr ;              // 16099 - Постъпили от началото на годината == нач.на отч.период
    unsigned long int specials ;               //  6
    unsigned long int byJurisd ;               // 35  citizen
    unsigned long int returneds ;              // 36  citizen
    unsigned long int byInDoc ;                // 44  citizen
   } receiveds ;

  unsigned long int forExamine ;               //  7
  unsigned long int forExamineEval ;           // 47

  struct
   {
    unsigned long int withDecision ;           //     8
    unsigned long int decision_RC ;            //    89 20060627
    unsigned long int ess_CS_RC ;              //    90 20060627
    unsigned long int withDecision_corr ;      // 16101 20070608


    struct
     {
      unsigned long int total ;                //     9
      unsigned long int total_corr ;           // 16102  20070608
      unsigned long int returneds ;            //    10
      unsigned long int byAgreement ;          //    40  citizen
      unsigned long int byOtherCauses ;        //    41  citizen
      unsigned long int byOtherCauses_OS ;     //    81  25.may.2001
      unsigned long int byOtherCauses_CS ;     //    82  25.may.2001
      unsigned long int canclsOrSends ;        //    55
      unsigned long int byOthCausFirm ;        //    58  12.jan.2000
      unsigned long int withBulwark ;          //    62  13.jun.2000
      unsigned long int withBulwark_corr ;     // 16103  13.jun.2000
      unsigned long int closedSess ;           //    68  10.jan.2001
      unsigned long int summByAgreeByOth_RC ;  //    95  citizen RC 20060627
      // 20070810 ->
      unsigned long int total_VSS ;            // 16123  Прекратени производства - Всичко
      unsigned long int otherReason_VSS ;      // 16124  Прекратени производства - по други причини - да се получава като сума от 16126 и 16127
      unsigned long int otherReason_VSS_1 ;    // 16126  Прекратени производства - по други причини - Прекратено, Изпратено по подсъдност
      unsigned long int otherReason_VSS_2 ;    // 16127  Прекратени производства - по други причини - За касационни дела - резултат от заседанието:"С определение, приключващо делото" и резултат от съдебния акт "Прекратява делото"
      // 20070810 <-
     } canceleds ;
    struct
     {
      unsigned long int withDecision ;         //  11
      unsigned long int canceleds ;            //  12
     } specials ;
    struct
     {
      unsigned long int entire ;               //  37  citizen
      unsigned long int partial ;              //  38  citizen
      unsigned long int rejected ;             //  39  citizen
     } grant ;
    struct
     {
      unsigned long int confirmed ;            //  48  punishment admin
      unsigned long int changed ;              //  49  punishment admin
      unsigned long int abolished ;            //  50  punishment admin
     } punishDecree ;
    struct
     {
      unsigned long int granted ;              //  60
      unsigned long int grantedCS ;            //  64
      unsigned long int rejected ;             //  61
      unsigned long int rejectedCS ;           //  65
     } demand ;
    unsigned long int total ;                  //  46  citizen
    unsigned long int total_VSS ;              // 131  Свършени  - общо - 1 и 2 инст.
   } completeds ;
  struct
   {
    unsigned long int upTo3Months ;            //  13
    unsigned long int over3Months ;            //  14

    unsigned long int upTo3Months_corr ;       // 104  // 20070608
    unsigned long int over3Months_corr ;       // 105  // 20070608

    unsigned long int upTo3Months_VSS ;        // 130  // 20070829

   } age ;
  unsigned long int appealeds ;                //  15
  unsigned long int appealeds_RC ;             //  91  20060627
  unsigned long int allowSplit ;               //  45  citizen
  unsigned long int changedsFirm ;             //  59

  // 20070606  ->  отчет корупционни престъпления
  unsigned long int notDecisionBeg_corr ;           // 16098 - Несвършени в нач. на годината == нач.на отч.период
  unsigned long int forExamineEval_corr ;           // 16100 - Дела за разглеждане  16100 = 16098 + 16099
  unsigned long int notDecisionEnd_corr ;           // 16106 - Останали несв. дела в края на отч.период
  // 20070606 <-

  // 20070810  ->  отчет ВСС
  unsigned long int forExamineEval_VSS ;            // 16120 - Дела за разглеждане -  получава се като сума на 16003 и 16004
  unsigned long int compl_to3m_Percentage_VSS;      // 16121 - Свършени дела - продължителност - до 3 месеца  - %
  unsigned long int compl_inEssence_VSS ;           // 16122 - Със съдебен акт по същество
  unsigned long int notDecisionEnd_VSS ;            // 16132 - Останали несв. дела в края на отч.период - Висящи
  unsigned long int appealeds_VSS ;                 // 16133 - Обжалвани
  // 20070810  <-  отчет ВСС

  unsigned long int notDecisionEnd ;           //  16   !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()
  protected :
    size_t size () const ;
 } ;  // struct reportLawsuits

struct reportCitizens
 {
  void clearData () ;
  reportCitizens () { clearData () ; }
  bool operator == (const reportCitizens & otherRC) const ;
  bool isEmpty () const ;

  struct
   {
    unsigned long int total ;                      // 17   !!! This
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
    unsigned long int total_corr ;                 // 16107 Съдени лица;   Общо // 20070611

    unsigned long int acquits ;                    // 18
    unsigned long int acquits_corr ;               // 108   Съдени лица;   В т.ч. оправдани // 20070611
   } judgeds ;
  struct
   {
    unsigned long int freeds ;                     // 67
   } freeds ;
  struct
   {
    unsigned long int total ;                      // 19
    unsigned long int total_corr ;                 // 109 // Осъдени лица. Всичко
    unsigned long int notMajors ;                  // 20
    struct  // 10.jan.2001
     {
      unsigned long int total ;                    // 66
      unsigned long int effective ;                // 69  // 01.mar.2001
     } upTo1Year ;
    struct  // 01.mar.2001
     {
      unsigned long int effective ;                // 70
     } n3Years ;
    struct
     {
      unsigned long int total ;                    // 21
      unsigned long int total_corr ;               // 16110
      unsigned long int conditional ;              // 22
      unsigned long int conditional_corr ;         // 16111
      unsigned long int effective ;                // 56
     } upTo3Years ;
    unsigned long int betw3And15Years ;            // 23
    unsigned long int penalty ;                    // 24
    unsigned long int correctionWork ;             // 25
    unsigned long int others ;                     // 26

    unsigned long int fine_corr ;                  // 16114  ;  Осъдени лица. Глоба
    unsigned long int trial_corr ;                 // 16115  ;  Осъдени лица. Пробация
    unsigned long int others_corr ;                // 16116  ;  Осъдени лица. Други

    unsigned long int betw3And10Years ;            // 31  !
    unsigned long int betw3And10Years_corr ;       // 16112  !
    unsigned long int betw10And30Years ;           // 32  !
    unsigned long int betw10And30Years_corr ;      // 16113  !

    unsigned long int lifeImprisonment ;           // 33  !
    unsigned long int lifeImprisonmentNotAppeal ;  // 34  !
    unsigned long int wrkEducateSchool ;           // 51
    unsigned long int othersWithoutWrkEducSch ;    // 57
    unsigned long int clause414G ;                 // 63

    unsigned long int agreement_corr ;             // 16117  ; Брой наказани лица по споразумение - чл.381 - 384 НПК

   } convicteds ;
  struct
   {
    unsigned long int general ;                    // 52
    unsigned long int special ;                    // 53
    unsigned long int dangerous ;                  // 54
   } recidive ;
  unsigned long int complementPunishments ;        // 27   !!! This
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()
  protected :
    size_t size () const ;
 } ;  // struct reportCitizens

struct reportSessions
 {
  void clearData () ;
  reportSessions () { clearData () ; }
  bool operator == (const reportSessions & otherRS) const ;
  bool isEmpty () const ;
  const reportSessions & operator << (const judgeCheckupSessions & jCS) ;

  unsigned long int fixeds ;    //    !!! This     // 28
    // must be the first field. Otherwise, you have to correct
    // clearData () and size ()
  unsigned long int fixeds_RC ;           // 20060627     // 86
  unsigned long int fixeds_Sum_RC ;       // 20060627     // 93

  unsigned long int number_Sessions_VSS;  // 20070810 - 16125  Брой заседания

  struct
   {
    unsigned long int total ;                      // 29
    unsigned long int firstSess ;                  // 42 citizen
    unsigned long int secondSess ;                 // 43 citizen
    unsigned long int reconcileSess ;              // 74  25.may.2001
    unsigned long int rgCourtVn_OtherCause ;       // 75  25.may.2001
    unsigned long int penalty ;                    // 76  25.may.2001
    unsigned long int orStoppeds ;                 // 85  07.may.2002

    unsigned long int BeforeFirstSess_RC ;         // 87  // 20060627
    unsigned long int Interim_RC ;                 // 88  // 20060627
    unsigned long int total_RC ;                   // 92  // 20060627
    unsigned long int firstSess_RC ;               // 94  // 20060627; citizen

   } lefts ;

  struct
   {
    unsigned long int puniGen_ReqS_CS_Def ;
    unsigned long int puniGen_ReqS_CS ;
    unsigned long int puniGen_ReqS ;
   } bySurround ;  // 18.apr.2001

  unsigned long int splits ;                       // 77  25.may.2001
  unsigned long int mayAppeal ;                    // 78  25.may.2001
  unsigned long int lateDecisions ;                // 79  25.may.2001
  unsigned long int otherSessAfterFinish ;         // 80  25.may.2001
  struct
   {
    unsigned long int openSess ;                   // 83  // 20.mar.2002
    unsigned long int closedSess ;                 // 84  // 20.mar.2002
   } otherActs ;  // 20.mar.2002

  unsigned long int stoppeds ;  //   !!! This      // 30
    // must be the last field. Otherwise, you have to correct
    // clearData () and size ()
  protected :
    size_t size () const ;
 } ;  // struct reportSessions

struct reportData
 {
  void clearData () ;
  reportData () { clearData () ; }
  bool countingDataEmpty () const ;

  bool operator==(const reportData &other) const ;  // required
  bool operator<(const reportData &other) const ;  // required
  bool operator<=(const reportData &that) const { return operator<(that) || operator==(that); }

public:
  const reportData & operator << (const judgeCheckupData & jCD) ;

  long int subject ;
  long int lowSubjRange ;
  long int highSubjRange ;
  char subjName [SIZE_OF_SUBJECT_NAME] ;
  // 20070810 -->
  char rowLetters_VSS[SIZE_OF_SUBJECT_RANGE_LETTERS] ;   // 16118  за АДМ.съд, отчет за ВСС - шифър на реда е буква - T_SUBJECT_RANGE.F_LETTER
  int  year_VSS ;         // 16119  Година

  bool covered ;
  reportLawsuits l ;
  reportCitizens c ;
  reportSessions s ;
  bool InsideRange(long tSubject) const;	// 2016:098
 } ;  // struct reportData
