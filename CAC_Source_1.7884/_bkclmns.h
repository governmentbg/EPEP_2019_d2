class columnsArr : public bookFormats
 {
  protected :
    const int columnsTotal ;            
    const int bOC ;  // basic output cipher
    column * * columns ;
    size_t maxRows ;
    const bool twoSets ;  // 23.oct.2001
    void newColumn (column * & rpC,
                    unsigned int l, const char * str = NULL) ;
    virtual bool knownCCipher (unsigned int c) const ;
    virtual void defineMaxRows () ;
    int countMaxRows () const ;
    virtual bool cipherPresent (const int cipher,
                                const bool first,
                                const bool knownCheck = true
                               ) const ;
    void standardSingleNCOutput (ostream & os,
                                 const bookAreas & bA) ;  // 23.oct.2001
  public :
    columnsArr (const char * formatFN, TWindow * parent,
                int totalColumns, int basicOutputCipher,
                const bool complex = false) ;
    virtual ~columnsArr () ;
    int getMaxRows () const ;
    virtual void output (ostream &, bool) const ;
    virtual void notConstOutput (ostream & os, bool first) ;
    void reWind () ;
    int ownColumnsCnt () const ;  // 12.mar.2002
    column * getColumn(const size_t cipher);  // 20.nov.2002
    int ColumnsTotal() const;  // 07.feb.2005
    int BasicOutputCipher() const;  // 07.feb.2005
 } ;  // class columnsArr

int own_areas_count(const bookAreas* bkAreas, const int areasCnt,
			const int basicOutputCipher, const int supportedColumnsCount);

// 11.jan.2002 - begin
class ColumnsArrIncMaxRows : public columnsArr
 {
  public :
    ColumnsArrIncMaxRows (const char * formatFN, TWindow * parent,
                          int totalColumns, int basicOutputCipher,
                          const bool complex = false) ;
    void IncMaxRowsIfNecessary (const int newMaxRowsVal) ;
 } ;  // class ColumnsArrIncMaxRows

// 11.jan.2002 - end

class timeBookEnum
 {
  protected :
    enum
     {  // Attention!  Add new items ONLY before i_columns (to prevent
        // any confuses in decisionOrJudgementColumn)
      iC_SerialNo = 0,                  // 101
      iC_NoAndMakeDate,                 // 102
      iC_Kind,                          // 103
			iC_Subject,                       // 104
      iCF_NotMajorDefendants,           // 105
      iCF_DecisJudgem,                  // 106
      iCF_Returned,                     // 107
      iCF_Send,                         // 108
      iCF_CanceledOthReasons,           // 109
      iCF_Left,                         // 110
      iCF_Stopped,                      // 111
      iC_CancLeftStop_reason,           // 112
      iCF_UpTo3MonLong,                 // 113
      iCF_Over3MonLong,                 // 114
      iC_Membership,                    // 115
      iC_Defendants,                    // 116
      iC_DecisionsJudgements,           // 117
      iC_Blank,                         // 118

      iC_SubjInvolvem,                  // 119
      iC_Sides,                         // 120
      iC_MembershipForMColl,            // 121
	iCF_NewlyCreated,                 // 122
      iCF_Received,                     // 123
      iCF_RetFrom2ndInstance,           // 124
	iCF_ClaimFullyGranted,            // 125
      iCF_ClaimPartialGranted,          // 126
      iCF_ClaimNonsuit,                 // 127
      iCF_CancelByAgreement,            // 128
      iCF_Cancel_ExactByOtherCauses,    // 129
      iC_DecisionDateAndHow,            // 130
      iC_LeftsStoppedsAndCauses,        // 131
      iCF_UpTo1MonthLong,               // 132   //2005
      iCF_Betw1And3MonthLong,           // 133   //2005
      iC_GivingAndReturnDate,           // 134
// ---------------- Processed only # if INSTANCE -----------------------
      iC_1stInstCourtAndOthData,        // 135
      iCF_ReceivedsByComplaint,         // 136
      iCF_ReceivedsBYObjection,         // 137
      iCF_JudgementValidityConfirmed,   // 138
      iCF_ConditConvictingApplied,      // 139
	iCF_JudgementRepealedPartilally,  // 140
      iCF_JudgemRepealFully_Returned,   // 141
      iCF_JudgemRepealFully_Canceled,   // 142
      iCF_JudgemRepealFully_NewJudgem,  // 143
      iC_NumberOfHoldedsLawNotFinish,   // 144
// ---------------------------------------------------------------------
      iC_SubjectWithText,               // 145
      iC_ClaimantsVsDefendants,         // 146
      iC_InDocAndSubjectWithText,       // 147
// ---------------- Processed only # if INSTANCE -----------------------
      iCF_DecisionValidityConfirmed,    // 148
      iCF_DecisionRepealedPartilally,   // 149
	iCF_DecisRepealFully_Returned,    // 150
      iCF_DecisRepealFully_Canceled,    // 151
      iCF_DecisRepealFully_NewDecis,    // 152
// ---------------------------------------------------------------------
      iC_DecisionsJudgementsCatchOth,   // 153
      iC_CancLeftStop_reasonCatchOth,   // 154

      iC_Claimants,                     // 155
      iC_AccusedsNum,                   // 156
      iCF_JudgedsTotal,                 // 157
      iCF_JudgedsCond,                  // 158
      iCF_Accquits,                     // 159
      iCF_FromCancLaws,                 // 160
      iCF_Freeds,                       // 161
      iCF_GenRecidive,                  // 162
      iCF_SpecRecidive,                 // 163
      iCF_cNewCrimeDuringTestTime,      // 164

      iCF_RcvByComplIncludingNext,      // 165
      iCF_RcvBYObjectIncludingNext,     // 166

      iC_InDocAndRelatedLaws,           // 167  // only if # INSTANCE
      iC_DefendantsWithAddr,            // 168
      iC_GivingAndReturnDateManyDec,    // 169

      iC_SessionDate,                   // 170  20.jan.2005 - begin
      iC_PresidentAndMembers,           // 171
      iC_Judge,                         // 172
	iC_Prosecutor,                    // 173
      iC_Secretar,                      // 174
      iC_President,                     // 175
      iC_ComplaintMaker_Citizen,        // 176
      iC_Defendant_Citizen,             // 177
      iC_SessResult_Decided,            // 178
      iC_SessResult_Canceled,           // 179
      iC_SessResult_Stopped,            // 180
	iC_SessResult_Left,               // 181
      iC_JudicialActNo,                 // 182
      iC_SubjectCode,                   // 183
      iC_NewlyCreatedOrTransformed,     // 184
      iC_MiscellaneousObstacles_Rgn,    // 185
      iC_SessResult_Decided_Rgn_Cit,    // 186
      iC_SessResult_Stopped_Rgn_Cit,    // 187
      iC_SessResult_Left_Rgn_Cit,       // 188
      iC_Jurors,                        // 189
      iC_FixedPunishmentGeneral,        // 190
      iC_FixedPunishmentPrivateChar,    // 191
      iC_FixedAdminPunishment,          // 192
      iC_FixedPrivatePunishment,        // 193
      iC_FixedBySourceCancAppealRet,    // 194
      iC_____free_for_future_use_____,  // 195
      iCF_DangerousRecidive,            // 196 //  20.jan.2005 - end
      iCF_IIinst_Decision,              // 197 // 20051208
      iCF_IIinst_Cease,                 // 198 // 20051208
      iC_Nature,                        // 199 // 20060320
      iC_DecisionsJudgements_TB_NF_PL,  // 200 // 20060331 - Time Book, New Format, Penal Lawsuit only - Regional Court
	iC_CancLeftStop_TB_NF_PL,         // 201 // 20060405 - Time Book, New Format, Penal Lawsuit only - Regional Court
      iC_Result_TB_NF_AL,               // 202 // 20060406 - Time Book, New Format, Adm Lawsuit only   - Areal Court

      i_columns  // i_columns == (the number of columns)
     } ;
  public :
    timeBookEnum () ;
 } ;  // class timeBookEnum

class timeBookColumns : public timeBookEnum, public columnsArr
 {
  protected :
    static bool showDecNoIsSet ;  // 21.feb.2001
    static bool showDecisionNo ;  // 21.feb.2001
//    const TSession & sess ;  // 03.aug.2001
    TSession & sess ;  // 03.aug.2001
    TStringArray * const pItems ;  // 27.jan.2005
    void sidesAndInvolvements (const TSession & sess,
                               TLawsuit & lawsuit,
                               char * buff, size_t buffL) ;
    void makeColumns (TSession & sess,  // 03.aug.2001 const TSession
                      TLawsuit & lawsuit,
                      timeBookHeap & tBH,
                      accusedsDistribution & accDistrHeap, size_t sNo,
               const collCompBegDEndDBegN * pCBDEDBN = NULL, // 08.feb.2001
                      const bool allFmtFileColumns = true,
                      const CDate * pDateAbolish = NULL,
                      const CDate * const pDateReturnedAbolish = NULL,
			    const int additionalRowsAfterSess = 0,
                      const TSession * pAbolishingSess = NULL //12.dec.2001
                     ) ;
// ----------------- one session print service -----------------------
    void addResKindDResTxtToColumn (char result, char kind,
                                    char dJResult, const char * text,
                                    column * pColumn,
                                    long event_no=0) ;   // 200511202 is added
// -------------------------------------------------------------------
    void defaultDateReturnedProcessing (column * const pColumn,
                                        const bool abolishedProgress,
                               const CDate * const pDateReturnedAbolish,
                                        char * const buff,
                                        const size_t buffL) ; //24.sep.2001
    bool defaultDecDateAndHowCondition () ;  // 24.sep.2001
    bool firstManyDecCase (const TLawsuit & lawsuit) ;  // 24.sep.2001
    void moveArrayContents (TStringArray & src) ;  // 27.jan.2005
  public :
    timeBookColumns (const char * formatFN, TWindow * parent,
                     TSession & rSess,  // 03.aug.2001  const TSession
                     TLawsuit & lawsuit,
                     size_t sNo, timeBookHeap & tBH,
                     accusedsDistribution & accDistrHeap,
                     TStringArray * itemsPtr,  // 27.jan.2005
                     const collCompBegDEndDBegN * pCCBDEDBN = NULL,//08.feb.2001
                     size_t minRows = 0,
                     const bool allFmtFileColumns = true,
                     const CDate * pDateAbolish = NULL,
                     const CDate * const pDateReturnedAbolish = NULL,
                     const int additionalRowsAfterSess = 0,
                     const TSession * pAbolishingSess = NULL // 12.dec.2001
                    ) ;
    virtual void notConstOutput (ostream & os, bool first) ;
// ----------------- one session print services -----------------------
    void arrangeDesJudgColumns (char result, char kind,
                                char dJResult, const char * text,
                                const bool flushAtFirst = false,
                                size_t minRows = 0,
                                long event_no = 0
                               ) ;
    void decisionsAndSurroundments
           (const TGArray <TSurroundment> & surroundments,
            TSession & rSess, const TLawsuit & law) ;  // 10.oct.2001
// --------------------------------------------------------------------
    const CDate * sessDatePtr () const ;
    void sessionLabel (char * const dest,
                       const size_t destSz) const ;
    static bool showDecisionNumber () ;  // 21.feb.2001
    static void setShowDecisionNumber () ;  // 21.feb.2001
    static  // and public 25.jan.2005
    void addMemberToMembership (const char * ucn, char * buff,
					  size_t buffLen, column * pColumn,
					  const char * prefix,
					  const char * speakerUCN = NULL
					 ) ;
 } ;  // class timeBookColumns

// # if ! INSTANCE  // 15.nov.2001
enum senderChecksActions
 {
  IgnoreSender,
  SenderMustBeEmpty,
  SenderMustNotBeEmpty
 } ;  // enum senderChecksActions

// # endif  // 15.nov.2001

class closedSessBookColumns : public columnsArr
 {
  protected :
    enum
     {
      iC_SerialNo = 0,                //  1
      iC_Composition,                 //  2
      iC_LawsuitKind,                 //  3
      iC_LawNoAndYear,                //  4
      iC_Clause,                      //  5
      iC_Date,                        //  6
      iC_Judge,                       //  7
      iC_LawsuitText,                 //  8

      iC_LawNoYearKind,               //  9
      iC_InDoc,                       // 10
      iC_SessionResultAndText,        // 11
      iCF_WithDecision,               // 12
      iCF_Canceled,                   // 13
// ------------------- processed only # if INSTANCE --------------------
      iC_LawKindWithSuffix,           // 14
      iC_FromCourtOrSubjectCode,      // 15
// ---------------------------------------------------------------------
      iC_WhenSpeakerHasServe,         // 16
// ------------------- processed only # if INSTANCE --------------------
      iC_DecResultAndDamnName,        // 17
      iC_DecEnterDate,                // 18
      iCF_DamnWithDec,                // 19
      iCF_DamnCanceled,               // 20
      iC_DecisionDate,                // 21

      iC_LawNoYear_Appeal,            // 22
      iC_FromCourt,                   // 23
// ---------------------------------------------------------------------
// ------------------- processed only # if INSTANCE --------------------
      iC_FromCourtOrSubject,          // 24

      iC_DecResultAndDamnName_Short,  // 25
// ---------------------------------------------------------------------

      iC_Membership,                  // 26  21.jan.2005 - begin
      iC_JudicialActNo,               // 27
      iC_iC_DateJudicialActAnnounced, // 28  21.jan.2005 - end

                                      // 20060412 - to replace iC_DecEnterDate used in old format -> NF
      iC_CS_NF_DateAnnounced,         // 29  20060412
      iC_CS_NF_DL_DateAnnounced,      // 30  20060412 - only for DLawsuits

      iC_CS_NF_LawNoAndYear,          // 31  20070201 - without composition

      i_columns  // i_columns == (the number of columns)
     } ;
    TSession & session ;  // because of "operator <<"
    const collBegDEndDBegN & const rBegDEndDBegN ;
# if INSTANCE
    const CDate decisionEnterDate ;
# endif
    TStringArray * const pItems ;  // 21.jan.2005
    bool mayShowDateFinished (const CDate lawFinished,
                              const TSession & sess,
                              const collBegDEndDBegN & rCBegDEndDBegN
                             ) const ; // 12.feb.2001
    void makeColumns (long int serialNo, const TSession & sess
                      , TLawsuit & lawsuit
                , const collBegDEndDBegN & rCBegDEndDBegN
                      ) ;
    void moveArrayContents (TStringArray & src) ;  // 25.jan.2005
  public :
    closedSessBookColumns (const char * formatFN, TWindow * parent,
                           long int serialNo,
                           TSession & sess, TLawsuit & law,
                           const collBegDEndDBegN & cBegDEndDBegN,
# if INSTANCE
                           const CDate & decisionED,
# endif
                            TStringArray * itemsPtr  // 21.jan.2005
                           ) ;
    virtual void notConstOutput (ostream & os, bool first) ;
 } ;  // class closedSessBookColumns

class judgeCheckupEnum
 {
  public :
    enum
     {                           // For fmt  Column No
	iC_Judge = 0,                    //   1  //  1

	iC_L_NotFinishedsBeg1,           //   2  //  2
	iC_L_NotFinishedsBeg2,           //   3  //  3
	iC_L_News1,                      //   4  //  4
	iC_L_News2,                      //   5  //  5
	iC_L_FinishedsTotal1,            //   6  // 10
	iC_L_FinishedsTotal2,            //   7  // 11
	iC_L_FinishedsWithDecision1,     //   8  // 12
	iC_L_FinishedsWithDecision2,     //   9  // 13
	iC_L_FinishedsCanceleds1,        //  10  // 14
	iC_L_FinishedsCanceleds2,        //  11  // 15
      iC_L_FinishedsUpTo3M1,           //  12  // 16
      iC_L_FinishedsUpTo3M2,           //  13  // 17
      iC_L_FinishedsOver3M1,           //  14  // 18
      iC_L_FinishedsOver3M2,           //  15  // 19
      iC_L_NotFinishedsEnd1,           //  16  // 20
      iC_L_NotFinishedsEnd2,           //  17  // 21

      iC_S_Fixeds1,                    //  18  //  6
      iC_S_Fixeds2,                    //  19  //  7
      iC_S_Lefts1,                     //  20  //  8
      iC_S_Lefts2,                     //  21  //  9
      iC_S_OtherActs1,                 //  22
      iC_S_OtherActs2,                 //  23
      iC_S_FixedsIncludingOtherActs1,  //  24
      iC_S_FixedsIncludingOtherActs2,  //  25

      iC_S_FixIfInMembersh2,           //  26
      iC_S_LeftsIfInMembersh2,         //  27
      iC_S_OtherActsIfInMembersh2,     //  28
      iC_S_FixInclOthActsIfInMemb2,    //  29

      iC_S_Fixeds1Judge1,              //  30
      iC_S_Fixeds2Judges1,             //  31
      iC_S_Fixeds3Judges1,             //  32
      iC_S_Fixeds3Judges2,             //  33

      iC_Composition,                  //  34         // Do not use yet !
	iC_Month,                        //  35         // Do not use yet !

      iC_A_Appealeds1,                 //  36         // Do not use yet !

      iC_L_Abolisheds1,                //  37

      iC_L_FinishedsCanceledsByAgr1,   //  38
      iC_L_FinishedsCanceledsByAgr2,   //  39
      iC_L_FinishedsCanceledsOtherC1,  //  40
      iC_L_FinishedsCanceledsOtherC2,  //  41

      iC_S_Lefts1stSess1,              //  42
      iC_S_Lefts1stSess2,              //  43
      iC_S_Lefts2ndSess1,              //  44
      iC_S_Lefts2ndSess2,              //  45
      iC_S_Stoppeds1,                  //  46
      iC_S_Stoppeds2,                  //  47

      iC_S_FixedsIfNotJudge1,          //  48
      iC_S_FixedsIfNotJudge3_2,        //  49

      iC_L_KeepingMethodConfirmed1,    //  50
      iC_L_KeepingMethodChanged1,      //  51
// ---------------
// I lift up my hands by now (for the subjects) ...
      iC_L_Subject0210,                //  52
      iC_L_Subject0600,                //  53
      iC_L_Subject0500,                //  54
	iC_L_Subject0810,                //  55
      iC_L_Subjects0400_0410,          //  56
      iC_L_Subjects0220_0310_0800,     //  57
// ---------------
      iC_L_ExactComplaints2,           //  58
      iC_L_PrivateComplaints2,         //  59

      iC_S_Lefts3Judges1,              //  60
      iC_S_Lefts3Judges2,              //  61
      iC_S_Lefts3Judges1stSess1,       //  62
      iC_S_Lefts3Judges1stSess2,       //  63
      iC_S_Lefts3Judges2ndSess1,       //  64
      iC_S_Lefts3Judges2ndSess2,       //  65
      iC_S_AbolishedResults1,          //  66
      iC_S_AbolishedResults2,          //  67
      iC_S_AdditionalDecisDefinOpenS1, //  68
      iC_S_AdditionalDecisDefinOpenS2, //  69
      iC_S_AdditionalDecisDefinCldsS1, //  70
      iC_S_AdditionalDecisDefinCldsS2, //  71
      iC_L_DecisDefinClosedS1,         //  72
      iC_L_DecisDefinClosedS2,         //  73
      iC_L_CanceledsClosedS1,          //  74
      iC_L_CanceledsClosedS2,          //  75
      iC_L_NewsPGenCharAplCourt,       //  76
      iC_L_NewsPKeepOpenSessAplCourt,  //  77
      iC_L_NewsPKeepClSessAplCourt,    //  78
      iC_L_NewsPEnactmentAplCourt,     //  79
      iC_L_NewsPOthersAplCourt,        //  80
	iC_L_DecPGenCharAplCourt,        //  81
      iC_L_DecPKeepOpenSessAplCourt,   //  82
      iC_L_DecPKeepClSessAplCourt,     //  83
      iC_L_DecPEnactmentAplCourt,      //  84
      iC_L_DecPOthersAplCourt,         //  85
      iC_S_Appeal192_193sessRes2,      //  86  // 24.nov.2000

      iC_L_IlievaFin2ndTotal,          //  87
      iC_L_IlievaFin2ndCancelClosed,   //  88
      iC_L_IlievaFin2ndCancelOpen,     //  89
      iC_L_IlievaFin2nsDecUpTo3Mon,    //  90
      iC_L_IlievaFin2ndDecOver3Mon,    //  91
      iC_L_IlievaFinPrvTotal,          //  92
      iC_L_IlievaFinPrvPrvCompl,       //  93
      iC_L_IlievaFinPrvSlowdown,       //  94
      iC_B_IlievaOthTotal,             //  95
			iC_S_IlievaOth205Closed,         //  96
      iC_R_IlievaOthReqResol,          //  97
      iC_S_Ilieva192_193,              //  98
      iC_S_IlievaOth192p4,             //  99

      iC_SerialNo,                     // 100
      iC_S_KracholovReporteds1,        // 101
      iC_S_KracholovReportedsGenChar1, // 102
      iC_S_KracholovReportedsLeft1,    // 103
      iC_L_KracholovFinisheds1,        // 104
      iC_L_KracholovFinsGenChJudgem1,  // 105
      iC_L_KracholovFinsGenCh241_1,    // 106
	iC_L_KracholovFinsGenCh287_1,    // 107
      iC_S_KracholovReporteds2,        // 108
      iC_S_KracholovReporteds2nd2,     // 109
      iC_S_KracholovReporteds2ndLeft2, // 110
      iC_S_KracholovReportedsPrvC2,    // 111
      iC_S_KracholovReportedsAdm2,     // 112
      iC_S_KracholovReportedsAdmLeft2, // 113
      iC_S_KracholovReportedsPreCrt2,  // 114
      iC_L_KracholovFinisheds2,        // 115
      iC_L_KracholovFinisheds2nd2,     // 116
      iC_L_KracholovFinishedsPrvC2,    // 117
			iC_L_KracholovFinishedsAdm2,     // 118
      iC_L_KracholovFinishedsPreCrt2,  // 119
      iC_S_KracholovReportedsTotal,    // 120
      iC_L_KracholovFinishedsTotal,    // 121
      iC_L_KracholovFinsPrvCh1,        // 122
      iC_L_KracholovFinsPrvPreCrt1,    // 123
      iC_L_KracholovFinsAdmin1,        // 124

      iC_L_PdCitForExamineTotal1,      // 125
      iC_L_PdCitForExamineTotal2,      // 126
      iC_S_PdCitExamined1,             // 127
      iC_S_PdCitExamined2,             // 128
      iC_L_PdCitFinishComlp2,          // 129
      iC_L_PdCitFinishPrvComlp2,       // 130
      iC_L_PdCitFinishedsTotal,        // 131
      iC_L_PdCitDecidedsComlp2,        // 132
      iC_L_PdCitDecidedsPrvComlp2,     // 133
	iC_L_PdCitDecidedsTotal,         // 134
      iC_L_PdCitCanceledsComlp2,       // 135
      iC_L_PdCitCanceledsPrvComlp2,    // 136
      iC_L_PdCitCanceledsTotal,        // 137
      iC_S_PdCitUWA_UpTo1m1,           // 138
      iC_S_PdCitUWA_UpTo1mCompl2,      // 139
			iC_S_PdCitUWA_UpTo1mPrvCompl2,   // 140
      iC_S_PdCitUWA_Over1m1,           // 141
      iC_S_PdCitUWA_Over1mCompl2,      // 142
      iC_S_PdCitUWA_Over1mPrvCompl2,   // 143
      iC_S_PdCitUWA_UpTo1mTotal,       // 144
      iC_S_PdCitUWA_Over1mTotal,       // 145
      iC_L_PdCitFinishUpTo3MComlp2,    // 146
      iC_L_PdCitFinishUpTo3MPrvComlp2, // 147
      iC_L_PdCitFinishUpTo3MTotal,     // 148
      iC_L_PdCitFinishOver3MComlp2,    // 149
      iC_L_PdCitFinishOver3MPrvComlp2, // 150
      iC_L_PdCitFinishOver3MTotal,     // 151
      iC_S_KracholovUWAOver30d,        // 152

      iC_L_IlievaBankruptcyFinTotal,   // 153
      iC_L_IlievaBankruptcyDec2nd_OS,  // 154
      iC_L_IlievaBankruptcyDec2nd_CS,  // 155
      iC_L_IlievaBankruptcyDecPrv,     // 156
      iC_S_IlievaOthOpenSessTotal,     // 157

      iC_S_IlievaBankruptcyFixed_OS,   // 158
      iC_L_IlievaBankruptcyDecided_OS, // 159
	iC_L_IlievaBankruptcyCanceled_OS,// 160
			iC_L_IlievaBankruptcyCancel2nd,  // 161
      iC_L_IlievaBankruptcyCancelPrv,  // 162
      iC_L_IlievaBankruptcyFin_OS,     // 163
      iC_S_IlievaBankruptcyLeft_OS,    // 164

      iC_Firm_Feceived,                // 165
      iC_Firm_Entered,                 // 166
      iC_Firm_RegistrationRefused,     // 167
      iC_Firm_ChangeRefused,           // 168
      iC_Firm_Erased,                  // 169
      iC_Firm_CancelDecision,          // 170
      iC_Firm_Changed,                 // 171

      iC_L_DetailNotFinishedBegTotal1, // 172  // 05.apr.2002 - begin
      iC_L_DetailNotFinishedBegTotal2, // 173
      iC_L_DetailNotFinBegPunGen2,     // 174
      iC_L_DetailNotFinBegPunPrv2,     // 175
      iC_L_DetailNotFinishedEndTotal1, // 176
      iC_L_DetailNotFinishedEndTotal2, // 177
      iC_L_DetailNotFinEndPunGen2,     // 178
      iC_L_DetailNotFinEndPunPrv2,     // 179  // 05.apr.2002 - end

			iC_L_IlievaFinPrvPetitions,      // 180  // 11.jun.2002 - begin
			iC_S_IlievaOthPendingDecree,     // 181
			iC_S_IlievaOthPendingDefinition, // 182
			iC_S_IlievaOthAftFinDecisionOS,  // 183
			iC_S_IlievaOthAftFinDefinitionOS,// 184
			iC_S_IlievaOthAftFinDefinitionCS,// 185
			iC_S_IlievaOthAftFinDecree,      // 186
			iC_S_IlievaOthPending205,        // 187  // 11.jun.2002 - end
			iC_S_IlievaOthStopedUnmoveableCS,// 188  // 19.jun.2002

			iC_L_NewsPunGenChar1,            // 189
			iC_L_NewsPunGenChar2,            // 190

      iC_L_NewsRejudge1,               // 191  // 11.dec.2003 - begin
      iC_L_NewsRejudge2,               // 192
      iC_L_NewsPunGenCharRejudge1,     // 193
      iC_L_NewsPunGenCharRejudge2,     // 194

      iC_Firm_FeceivedRejudge,         // 195  // 11.dec.2003 - end

      iC_Oth_ReqResolutions1,          // 196  // 07.jun.2004 - begin
      iC_Oth_ReqResolutions2,          // 197

      iC_Oth_InRegEvents1,             // 198
      iC_Oth_InRegEvents2,             // 199  // 07.jun.2004 - end

      iC_Firm_Other_TradeRegister,     // 200  // 21.feb.2005
      iC_Firm_Other_NoTradeRegister,   // 201  // 21.feb.2005

      iC_Oth_InReg_Resol_AC,           // 202  // 20050804


      iC_L_IlievaFinPrvPrvCompl_Decis,  // 203   20060417  like 93
	iC_L_IlievaFinPrvPrvCompl_Ceased, // 204   20060417  like 93

      iC_L_IlievaFinPrvSlowdown_Decis,  // 205   20060417  like 94
      iC_L_IlievaFinPrvSlowdown_Ceased, // 206   20060417  like 94

      // 20061207 -->
      // Regional Court - Справка за дейността на съда - Разпределение по съдии ->

      iC_L_byJurisd_J,                  // 207   Постъпили дела - Получени по подсъдност
      iC_L_returneds_J,                 // 208   Постъпили дела - Върнати за ново разглеждане
      iC_L_Receiveds_Total_J,           // 209   Постъпили дела - Общо
      iC_L_ForExamineEval_J,            // 210   Всичко дела за разглеждане
      iC_S_Lefts_ReconcileSess_J,       // 211   Отложени дела - Помирителен срок
      iC_S_Lefts_BeforeFirstSess_J,     // 212   Отложени дела - Преди първо заседание
      iC_S_Lefts_Interim_RC_J,          // 213   Oтложени дела - Заседание по привременни мерки и друго заседание
      iC_S_Lefts_total_RC_J,            // 214   Отложени дала - Отложени ОБЩО
      iC_L_Completeds_Grant_Entire_J,   // 215   Свършени дела - Искът уважен изцяло
      iC_L_Completeds_Grant_Partial_J,  // 216   Свършени дела - Искът уважен отчасти
      iC_L_Completeds_Grant_Rejected_J, // 217   Свършени дела - Искът отхвърлен
      iC_L_Completeds_ess_CS_RC_J,      // 218   Свършени дела - Свършени с акт по същество в З.З
      iC_L_Completeds_canceleds_byAgreement_J,  // 219 - Свършени дела - Прекратени дела - По спогодба
      iC_L_Completeds_canceleds_byOtherCauses_OS_J,   // 220 - Свършени дела - Прекратени дела - Други причини - Съдебно заседание - Влезли в законова сила
      iC_L_Completeds_canceleds_byOtherCauses_CS_J,   // 221 - Свършени дела - Прекратени дела - Други причини - Закрито заседание - Влезли в законова сила
      iC_L_Completeds_canceleds_summByAgreeByOth_RC_J,// 222 - Свършени дела - Прекратени дела - ОБЩО
      iC_S_Splits_J,                    // 223   Допускане на делба
      iC_S_otherSessAfterFinish_J,      // 224   Допълване на решение, определение

      iC_L_NotFinishedsBeg1_J,          // 225  Останали несвършени  в началото на отчетния период
	iC_S_Lefts1stSess1_J,             // 226  Отложени дела - В първо заседание  P 25042
      iC_S_Lefts2ndSess1_J,             // 227  Отложени дела - Във II зас.и следващо съд.зас. P 25044
      iC_S_Fixeds_RC_J,                 // 228  Насрочени дела

      iC_L_Completeds_Decision_RC_J,    // 229  Свършени дела - С решение

      iC_L_NewsRejudge1_J,              // 230  Постъпили дела - Новообразувани
      iC_L_NotFinishedsEnd1_J,          // 231  Останали несвършени дела в края на отчетния период  P 25016
      // 20061207 <--

      // 20070202 ->
      iC_L_Appealeds_RC_J,              // 232  Обжалвани дела  P 25016
      // 20070202 <-

			i_columns  // i_columns == (the number of columns)
     } ;
  public :
    judgeCheckupEnum () ;
 } ;  // class judgeCheckupEnum

class judgeCheckupColumns : public judgeCheckupEnum, public columnsArr
 {
  protected :
    void addNmbToClmn (column * pColumn, const long int n) ;
    void makeColumns (const judgeCheckupData & jCD,
			    const CDate * const pDateForMonthColumn = NULL,
			    const int serialNo = 0  // 08.jan.2001
			   ) ;
  public :
    judgeCheckupColumns (const char * const formatFN,
				 TWindow * parent,
				 const judgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
				 const int serialNo = 0  // 08.jan.2001
				) ;
 } ;  // class judgeCheckupColumns

// 04.dec.2000 - begin
# if INSTANCE
#   if APPEAL
class IlievaJudgeCheckupColumns : public judgeCheckupColumns
 {
  protected :
    void MakeColumns (const IlievaJudgeCheckupData & jCD) ;
  public :
    IlievaJudgeCheckupColumns (const char * const formatFN,
					 TWindow * parent,
					 const IlievaJudgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
					 const int serialNo = 0  // 08.jan.2001
					) ;
 } ;  // class IlievaJudgeCheckupColumns
#   else  // of APPEAL
class KracholovJudgeCheckupColumns : public judgeCheckupColumns
 {
  protected :
    void MakeColumns (const KracholovJudgeCheckupData & jCD) ;
  public :
    KracholovJudgeCheckupColumns (const char * const formatFN,
					    TWindow * parent,
					    const KracholovJudgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
					    const int serialNo = 0  // 08.jan.2001
					   ) ;
 } ;  // class KracholovJudgeCheckupColumns

class PdCitJudgeCheckupColumns : public judgeCheckupColumns
 {
  protected :
    void MakeColumns (const PdCitJudgeCheckupData & jCD) ;
  public :
    PdCitJudgeCheckupColumns (const char * const formatFN,
					TWindow * parent,
					const PdCitJudgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
					const int serialNo = 0  // 08.jan.2001
				     ) ;
 } ;  // class PdCitJudgeCheckupColumns

class FirmJudgeCheckupColumns : public judgeCheckupColumns
 {
  protected :
    void MakeColumns (const FirmJudgeCheckupData & jCD) ;
  public :
    FirmJudgeCheckupColumns (const char * const formatFN,
				     TWindow * parent,
				     const FirmJudgeCheckupData & jCD,
				const CDate * const pDateForMonthColumn = NULL,
				     const int serialNo = 0  // 08.jan.2001
				    ) ;
 } ;  // class FirmJudgeCheckupColumns
#   endif  // of APPEAL
# endif  // of INSTANCE

// 04.dec.2000 - end

class summaryColumns : public columnsArr
 {
  protected :
    enum
     {
	iC_Name,
	iC_Heap,

	i_columns
     } ;
    void makeColumns (const summaryData & sD) ;
  public :
    summaryColumns (const char * const formatFN, TWindow * parent,
                    const summaryData & sD
                   ) ;
 } ;  // class summaryColumns

class requestColumnsEnum
 {
  protected :
    enum
     {
      iC_No = 0,                   //  1
      iC_LawKind,                  //  2
      iC_College,                  //  3
      iC_Composition,              //  4
      iC_Claimant,                 //  5
      iC_Defendant,                //  6
      iC_Sender,                   //  7
      iC_Remark,                   //  8
      iC_JudgeAndLawNo,            //  9
      iC_RemarkAreal,              // 10
      iC_JudgeHTMLSelect,          // 11  // 02.dec.2002 - begin
      iC_SubjectCodeHTMLSelect,    // 12
      iC_DayOfMonth,               // 13
      iC_SidesBrief,               // 14
	iC_RequestNo,                // 15  // 02.dec.2002 - end

      i_columns
     } ;
  public :
    requestColumnsEnum () ;
 } ;  // class requestColumnsEnum

class requestColumns : public requestColumnsEnum, public columnsArr
 {
  protected :
    void judgesAndResolutions (TRequest & rRequest, TLawsuit & law,
                               const bool lawFound,
                               column * pJudgeColumn,
                               column * pResolutionColumn) ;
    void makeColumns (TRequest & rRequest,
                      const JudgeNameRoll* const pJudges = NULL,  // 02.dec.2002
                      const SubjectCodeRoll* pSubjectCodes = NULL // 02.dec.2002
                     ) ;
  public :
    requestColumns (const char * formatFN, TWindow * parent,
                    TRequest & rRequest,
                    const JudgeNameRoll* const pJudges = NULL,    // 02.dec.2002
                    const SubjectCodeRoll* pSubjectCodes = NULL   // 02.dec.2002
                    ) ;
    static bool tryToLawsuit (const TRequest & rRequest,  // 14.feb.2003
                              TLawsuit & law) ;
 } ;  //  class requestColumns

class AppealedLawsInRegEnum
 {
  protected :
    enum
     {
      iC_SerialNo = 0,                       //  1
      iC_DateReceived,                       //  2
      iC_DocKind,                            //  3
      iC_SenderOfComplaint,                  //  4
      iC_LawKindLawNoYear,                   //  5
      iC_LawSubject,                         //  6
      iC_AppealedAct,                        //  7
      iC_DateSend,                           //  8
      iC_HigherInstance,                     //  9
      iC_DateReturned,                       // 10
      iC_ResultAppealCourt,                  // 11
      iC_ResultSupremeConcernCourt,          // 12
      iC_ResultAppealCourt2,                 // 13
      iC_ResultMinistry,                     // 14
      iC_Result,                             // 15
      iC_ResultWithoutMinistry,              // 16
      iC_ReturnForComplete,                  // 17
      iCF_RetForCompl,                       // 18
      iC_Remark,                             // 19
      iC_InRegNo,                            // 20
      iC_InRegNoOnly,                        // 21
      iC_RegNoAndDateReceived,               // 22
      iC_Judge,                              // 23
      iC_OutNo,                              // 24
      iC_LawKindLawNoYear_Exact,             // 25

	i_columns
     } ;
  public :
    AppealedLawsInRegEnum () ;
 } ;  // class AppealedLawsInRegEnum

class AppealedLawsInRegColumns :
  public AppealedLawsInRegEnum, public columnsArr
 {
  protected :
    void MakeEventDependencyColumns (TInReg & rInReg,
                                     char * const wrkBuff,
                                     const size_t wrkBuffL,
                                     TInRegEvent * const pInRegEvent,
                                     TOutReg * const pOutReg
                                    ) ;
    void MakeColumns (TInReg & rInReg, const int serialNo,
                      TInRegEvent * const pInRegEvent,
                      TOutReg * const pOutReg,
                      stringArray* pDocKindItems, // 21.dec.2004
                      stringArray* pLawsuitItems, // 21.dec.2004
                      stringArray* pApldActItems, // 21.dec.2004
                      stringArray* pAppealerItems = NULL // 21.dec.2004
                     ) ;
  public :
    AppealedLawsInRegColumns (const char * formatFN, TWindow * parent,
                              TInReg & rInReg, const int serialNo,
                              const size_t minRows,
                              TInRegEvent * const pInRegEvent = NULL,
                              TOutReg * const pOutReg = NULL,
                              stringArray* pDocKindItems = NULL, // 21.dec.2004
                              stringArray* pLawsuitItems = NULL, // 21.dec.2004
                              stringArray* pApldActItems = NULL, // 21.dec.2004
                              stringArray* pAppealerItems = NULL // 21.dec.2004
                             ) ;
    static bool MakeInRegEventsCriteria (TInReg & rInReg,
                                         char * const crtIRE,
                                         const size_t crtIRESz
                                        ) ;  // 25.sep.2000
    static void AddAppealersToColumn (column * const pColumn,
                                      char * const buff,
                                      const size_t buffL,
                                      TInReg & rInReg,
                                      const bool withProxies = true,
                                      stringArray* pAppealerItems = NULL
                                      ) ;  // Moved here
      // from the protected section and 'static' qualifier added
      // at 21.nov.2000
 } ;  //  class AppealedLawsInRegColumns

class AppealedLawsOutRegEnum
 {
  protected :
    enum
     {
      iC_OutRegNoOnly = 0,                   //  1
      iC_DateSent,                           //  2
      iC_DocKind,                            //  3
      iC_ComeInDate,                         //  4
      iC_LawKindLawNoYear,                   //  5
      iC_Destination,                        //  6
      iC_DateReturned,                       //  7
      iC_Remark,                             //  8

      i_columns
     } ;
  public :
    AppealedLawsOutRegEnum () ;
 } ;  // class AppealedLawsOutRegEnum

class AppealedLawsOutRegColumns :
  public AppealedLawsOutRegEnum, public columnsArr
 {
  protected :
    void MakeColumns (TOutReg & rOutReg, const int serialNo) ;
  public :
    AppealedLawsOutRegColumns (const char * formatFN, TWindow * parent,
                              TOutReg & rOutReg, const int serialNo,
                              const size_t minRows) ;
    static void AddLinkedInregsToColumn (column * pColumn,
                                         char * const buff,
                                         const size_t buffL,
                                         const TOutReg & rOutReg) ;
    static void AddReceiversToColumn (column * pColumn, char * const buff,
                                      const size_t buffL,
                                      TOutReg & rOutReg) ;
 } ;  //  class AppealedLawsOutRegColumns

class ResultsSetBookEnum
 {
  protected :
    enum
     {
      iC_SessRes = 0,                  //  1
      iC_DecRes,                       //  2
      iC_LawGrant,                     //  3
      iC_Count,                        //  4
      iC_AfterFinish,                  //  5
      iC_Abolished,                    //  6
      iC_DateRetAfterFinish,           //  7

      i_columns
     } ;
  public :
    ResultsSetBookEnum () ;
 } ;  // class ResultsSetBookEnum

class ResultsSetBookColumns :
  public ResultsSetBookEnum, public columnsArr
 {
  protected :
    void MakeColumns (const ResultsSet & rSet, const bool total) ;
  public :
    ResultsSetBookColumns (const char * formatFN, TWindow * parent,
                           const ResultsSet & rSet,
                           const bool total = false) ;
 } ;  //  class ResultsSetBookColumns

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
class TradeLaw634BookEnum
 {
  protected :
    enum
     {
      iC_SerialNo = 0,                                       //  1
	iC_ComeInDate,                                         //  2
      iC_SurroundAct,                                        //  3
      iC_Involvement,                                        //  4
      iC_ArealCourtAct,                                      //  5
	iC_AppealCourtAct,                                     //  6
      iC_SupremeConcernCourtAct,                             //  7
      iC_LawsuitNoYear,                                      //  8    20050221
      iC_Judge,                                              //  9    20050221  Dokladchik
      iC_FirmAndAddress,                                     // 10    20050221
      iC_ActionDate,                                         // 11    20050221

      i_columns
     } ;
  public :
    TradeLaw634BookEnum () ;
 } ;  // class TradeLaw634BookEnum

class TradeLaw634BookColumns :
  public TradeLaw634BookEnum, public columnsArr
 {
  protected :
    const Trade634DlgData & t634DD ;
    TGSArray <LinkedXKXKDatedContainerSK> & events ;
    const int items ;
    TSurround leadSurround ;
    bool IsSurround (const int ind) const ;
    bool IsSession (const int ind) const ;
    bool DateInRange (const CDate & d) ;
    void MakeSurroundColumns (const int ind) ;
    void MakeSessionColumns (const int ind) ;
    void MakeDecisionColumns (const int ind) ;
    void MakeInRegEventColumns (TInReg & rInReg, char * const line,
                                const size_t lineL) ;  // 31.jan.2001
    void MakeInRegColumns (const int ind) ;
    void MakeInRegEventColumns (const int ind) ;
    void MakeOutRegColumns (const int ind) ;
    void MakeOutRegColumns (TOutReg & rOutReg) ;  // 01.feb.2001
    void MakeColumns (int & startIndex, const int begSerialNo) ;
  public :
    TradeLaw634BookColumns (const char * formatFN, TWindow * parent,
                            const Trade634DlgData & rT634DD,
                       TGSArray <LinkedXKXKDatedContainerSK> & eventsArr,
				    int & startIndex, const int begSerialNo,
                            const size_t minRows = 0) ;
    static bool IsKindForTradeLaw634Book (const char k) ;
    static bool IndexAndDateNotTooBig
      (TGSArray <LinkedXKXKDatedContainerSK> & eventsArr, const int ind,
       const Trade634DlgData & t634dd) ;
    static bool PreliminaryChecks
     (TGSArray <LinkedXKXKDatedContainerSK> & eventsArr,
      int & startIndex, const Trade634DlgData & t634dd) ;
    static void AddSurroundSidesToColumn (column * const pColumn,
							char * const buff,
                                          const size_t buffL,
                                          TSurround & rSurround) ;
 } ;  //  class TradeLaw634BookColumns
#   endif  // of APPEAL
# endif  // of INSTANCE

class SessDistributionEnum
{
	protected :
		enum
		{
			iC_SerialNo = 0,                           //  1
			iC_Date,                                   //  2
			iC_Instance,                               //  3
			iC_PartOfDay,                              //  4
			iC_Judge,                                  //  5
			iC_NumberOfSessions,                       //  6
			iC_Secretary,                              //  7

			i_columns
		} ;
} ;  // class SessDistributionEnum

class JudgeWithNames;

class SessDistributionColumns :
	public SessDistributionEnum, public columnsArr
{
	protected :
		void MakeColumns(const SessJudgeDistrData& data, const int serialNo,
			TGSArray<JudgeWithNames>& jNames);
	public :
		SessDistributionColumns(const char * formatFN, TWindow * parent,
			const SessJudgeDistrData& data, const int serialNo,
			TGSArray<JudgeWithNames>& jNames,
			const int minRows = 0);
} ;  // class SessDistributionColumns

// 20051010 -->
# if ! INSTANCE
// Regional court only
class BookZZDNEnum
 {
  protected :
    enum
     {  // Attention!  Add new items ONLY before i_columns (to prevent
        // any confuses
	iC_SerialNo = 0,                  // 01
      iC_RequestNoAndDate,              // 02
	iC_Reserved,                      // 03
			iC_LawKey,                        // 04
      iC_Sides,                         // 05

      i_columns  // i_columns == (the number of columns)
     } ;
  public :
    BookZZDNEnum () ;
 } ;  // class BookZZDNEnum

class BookZZDNColumns : public BookZZDNEnum, public columnsArr
{
  protected :
    TStringArray * const pItems ;
    void makeColumns (TRequest & request
                      , const int serialNo
                     ) ;
  public :
    BookZZDNColumns (const char * formatFN,
			   TWindow * parent,
			   TRequest & request,
			   size_t minRows,
			   const int serialNo,
			   TStringArray * itemsPtr
			  ) ;
} ;  // class BookZZDNColumns
# endif

// 2011:178 moved here and made public for _column_result()
class descBookEnum
{
  public :
    enum
     {
	iC_LawNo = 0,                      //  1
	iC_CreateDate,                     //  2
	iC_Source_Areal,                   //  3   Само за ОКРЪЖЕН СЪД и АПЕЛАТИВЕН СЪД
						     //      - Ако делото е образувано по Вх.док., се извеждат <Точен вид на входящ докумнет, номер и Дата на постъпване>
						     //      - Изпратено по посъдност; възобновено; внесено след доразследване; Върнато за ново разглеждане
						     //      - Паркетно дело; Следствено дело; Дознание; Бързо полицейско производство

	iC_Subject_Areal,                  //  4   Предмет на делото - ОКРЪЖЕН СЪД
	iC_ClaimAndSettlementAddr_Areal,   //  5
	iC_DefendAndSettlementAddr_Areal,  //  6
	iC_LawsuitKind,                    //  7
	iC_Composition,                    //  8
	iC_Subject,                        //  9   Предмет на делото - РАЙОНЕН СЪД
      iC_Claimant,                       // 10
      iC_Defendant,                      // 11
      iC_Source,                         // 12   Източник на постъпване
      iC_DecisionDate,                   // 13   Кога е решено или прекратено делото
      iC_ClaimantAndSettlement_Areal,    // 14
      iC_DefendantAndSettlement_Areal,   // 15
      iCF_FastProcedure,                 // 16

      iC_H_Claimant,                     // 17    Обвинител, тъжител, молител, жалбоподател
      iC_H_Defendant,                    // 18    Обвиняем, подсъдим, нарушител, въззиваема страна
      iC_H_ClaimantAndSettlem_Areal,     // 19    Обвинител, тъжител, молител, жалбоподател
      iC_H_DefendantAndSettlem_Areal,    // 20    Обвиняем, подсъдим, нарушител, въззиваема страна
      iC_H_ClaimAndSettlemAddr_Areal,    // 21
	iC_H_DefendAndSettlemAddr_Areal,   // 22

	iC_Remark,                         // 23

      iC_SubjectNameOnly,                // 24   Предмет на делото - нов формат - РАЙОНЕН и ОКРЪЖЕН СЪД

// ----------------- processeds only if INSTANCE --------------------
      iC_LawNoAndDateCreated,            // 25
      iC_FirmKind,                       // 26
      iC_FirmAndBoss,                    // 27
      iC_Seat,                           // 28
      iC_Register,                       // 29
	iC_Volume,                         // 30
	iC_Page,                           // 31
	iC_Lot,                            // 32
// ------------------------------------------------------------------
// ------------- processeds only if INSTANCE and APPEAL -------------
      iC_ArealCourt,                     // 33
      iC_DateReturned,                   // 34
// ------------------------------------------------------------------

      iC_OutregNo,                       // 35

      iC_1stInstLawsuitAndCourt,         // 36  20.jan.2005 - begin
      iC_ArchiveFileNo,                  // 37
      iC_SubjectCode,                    // 38
      iC_DateDecisionEnacted,            // 39
      iC_DecisionFullyUpheld,            // 40
	iC_DecisionPartiallyChanged,       // 41
      iC_DecisionRepealed_NewDecision,   // 42
	iC_DecRepealed_Reconsideration,    // 43
      iC_DecisionInvalidated,            // 44
      iC_LawsuitCanceled,                // 45
      iC_SessionJudge,                   // 46
      iC_LawsuitJudge,                   // 47
      iC_DateSentToAnotherInstance,      // 48
      iC_DateRetFromAnotherInstance,     // 49
      iC_Appeal_FullyUpheld,             // 50
      iC_Appeal_PartiallyChanged,        // 51
      iC_Appeal_Repealed_NewDecision,    // 52
	iC_Appeal_Repeal_Reconsideration,  // 53

	iC_JudgementUpheld,                // 54
      iC_ConditionalConvictingApplied,   // 55
      iC_ConditionalConvictingRepealed,  // 56
      iC_PunishmentDecreased,            // 57
      iC_PunishmentIncreased,            // 58
      iC_AnotherChangesPunishmentPart,   // 59
      iC_ChangesCitizenPart,             // 60
      iC_JudgemRepealPart_Reconsider,    // 61
      iC_Reconsideration,                // 62
      iC_NewJudgement,                   // 63

      iC_Sides,                          // 64    Страни
      iC_LetterToFromAnotherInstData,    // 65
      iC_AppealResult_Admin,             // 66

      iC_Age_UpTo1Month,                 // 67
	iC_Age_UpTo3Months,                // 68
      iC_Age_UpTo6Months,                // 69
      iC_Age_UpTo1Year,                  // 70
	iC_Age_Over1Year,                  // 71
      iC_ArchiveLinkNo,                  // 72

      iC_InByComplaint,                  // 73  // for new books AREAL II and APPEAL - CRIMINAL LAW
      iC_InByObjection,                  // 74  // for new books AREAL II and APPEAL - CRIMINAL LAW
      iC_LawNoAndFastProcedure,          // 75  // for XML

						     // iC_LawNo and iCF_LawFastProcedure
	//20.jan.2005 - end
	// 20070201 the same as iC_Source
	iC_Source_alter,                   // 76  // for new books REGIONAL and AREAL I inst
								// Източник на постъпване (новообразувано, по подсъдност, за ново разглеждане)
	// 20060309
	iC_AnnounceDate,                   // 77  Дата на обявяване на делото за решаване

	iC_LawNo_Type_Compo_DateCre,       // 78 Номер на дело + точен вид на делото (alias) + състав - по искане на РС Варна

	iC_LawNo_Info_DateCre,             // 79  // 20080312

	iC_LawNo_Type_Compo,               // 80 Номер на дело + точен вид на делото (alias) + състав - по искане на РС Варна

	i_columns  // i_columns == (the number of columns)
     } ;
};

const
	_OFFSET_DESK_BOOK_CIPHER = OFFSET_DESK_BOOK_CIPHER + 1,
	_OFFSET_JUDGE_CHECKUP_CIPHER = OFFSET_JUDGE_CHECKUP_CIPHER + 1;

enum _full_column
{
	dB_JudgementUpheld = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_JudgementUpheld,
	dB_ConditionalConvictingApplied = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_ConditionalConvictingApplied,
	dB_ConditionalConvictingRepealed = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_ConditionalConvictingRepealed,
	dB_PunishmentDecreased = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_PunishmentDecreased,
	dB_PunishmentIncreased = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_PunishmentIncreased,
	dB_AnotherChangesPunishmentPart = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_AnotherChangesPunishmentPart,
	dB_ChangesCitizenPart = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_ChangesCitizenPart,
	dB_JudgemRepealPart_Reconsider = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_JudgemRepealPart_Reconsider,
	dB_Reconsideration = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_Reconsideration,
	dB_NewJudgement = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_NewJudgement,
	dB_DecisionFullyUpheld = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_DecisionFullyUpheld,
	dB_DecisionPartiallyChanged = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_DecisionPartiallyChanged,
	dB_DecisionRepealed_NewDecision = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_DecisionRepealed_NewDecision,
	dB_DecRepealed_Reconsideration = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_DecRepealed_Reconsideration,
	dB_DecisionInvalidated = _OFFSET_DESK_BOOK_CIPHER + descBookEnum::iC_DecisionInvalidated
};
