#include "cac.h"

#if COURT_TYPE == COURT_AREAL

static bool confirm_file_overwrite(const char* const fName)
{
  return ask("Файлът\n%s\nсъществува. Да бъде ли презаписан?",
    fName ? fName : "");
}

#include "_prbnkrp.h"  // 10.jun.2003 (hide public symbols)
#include "_prbnkrp.cpp"  // 10.jun.2003 (hide public symbols)

class BankruptcyLeftEnum
 {
  protected :
    enum
     {
      iC_LLawNo_1 = 0,                           //  1
      iC_LDebtor_2,                              //  2
      iC_LInitiator_3,                           //  3
      iC_LDate_4,                                //  4
      iC_LCanceled100_5,                         //  5
      iC_LDecisionRefusal_6,                     //  6
      iC_LDecisionCancel632_7,                   //  7
      iC_LAgreement740_8,                        //  8
      iC_LDecision630_9,                         //  9
      iC_LAppealBefore_10,                       // 10
      iC_LFirstCreditorsMeeting_11,              // 11
      iC_LAppealedActs_20,                       // 12
      iC_LAppealResults_21,                      // 13
      iC_LSerialNo,                              // 14
      iC_HTML_DateCompleted,                     // 15  // 23.apr.2003 - begin
      iC_HTML_Accomply,                          // 16
      iC_HTML_LastActDate,                       // 17
      iC_HTML_LastAct,                           // 18  // 23.apr.2003 - end

      i_columns
     } ;
 } ;  // class BankruptcyLeftEnum

class BankruptcyLeftColumns :
        public BankruptcyLeftEnum, public ColumnsArrIncMaxRows
 {
  protected :
    void MakePureLawColumns (TLawsuit & law, LawsuitAllSessions &,
                             BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                             TGSArray <OutRegDatedContainer> &,
                             const int) ;
    void MakeSideColumns (TLawsuit & law, LawsuitAllSessions &,
                          BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                          TGSArray <OutRegDatedContainer> &,
                          const int) ;
    void MakeSessionColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                             BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                             TGSArray <OutRegDatedContainer> &, const int);
    void MakeDecisionColumns (TLawsuit & law, LawsuitAllSessions &,
                              BankruptcyLawAllDecisions & decisions,
                              LawsuitAllInRegs &,
                              TGSArray <OutRegDatedContainer> &,
                              const int) ;
    void MakeAppealColumns (TLawsuit & law, LawsuitAllSessions &,
                            BankruptcyLawAllDecisions & decisions,
                            LawsuitAllInRegs & inRegs,
                            TGSArray <OutRegDatedContainer> & outRegs,
                            const int) ;
    void MakeColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                      BankruptcyLawAllDecisions & decisions,
                      LawsuitAllInRegs & inRegs,
                      TGSArray <OutRegDatedContainer> & outRegs,
                      TDatedContainer & lastDec,
                      TDatedContainer & lastSess,
                      const int serialNo) ;
    static void ProcessAppealBeforeColumn (column * pAppealBeforeColumn,
                                           const TInReg & iR,
                                 TGSArray <OutRegDatedContainer> & outRegs,
                                           const bool firstPrinted,
                                           bool & appealBeforePrinted) ;
    static void ProcessAppealedActsColumn (column * pAppealedActsColumn,
                                           TInReg & iR,
                                           const bool firstPrinted,
                                           bool & appealedActsPrinted) ;
    static void ProcessAppealResultsColumn (column * pAppealResultsColumn,
                                            const TInReg & iR,
                                 TGSArray <OutRegDatedContainer> & outRegs,
                                            const bool firstPrinted,
                                            bool & appealResultsPrinted,
                                            const int oRs) ;
  public :
    BankruptcyLeftColumns (const char * formatFN, TWindow * parent,
                           TLawsuit & law, LawsuitAllSessions & sessions,
                           BankruptcyLawAllDecisions & decisions,
                           LawsuitAllInRegs & inRegs,
                           TGSArray <OutRegDatedContainer> & outRegs,
                           TDatedContainer & lastDec,
                           TDatedContainer & lastSess,
                           const int serialNo, const int minRows) ;
    static void MakeAppealColumns (TLawsuit & law,
                                   BankruptcyLawAllDecisions & decisions,
                                   LawsuitAllInRegs & inRegs,
                                TGSArray <OutRegDatedContainer> & outRegs,
                                   column * const pAppealBeforeColumn,
                                   column * const pAppealedActsColumn,
                                   column * const pAppealResultsColumn) ;
 } ;  // class BankruptcyLeftColumns

class BankruptcyMiddleEnum
 {
  protected :
    enum
     {
      iC_MLawNo_1 = 0,                           //  1
      iC_MAppealBefore_10,                       //  2
      iC_MFirstCreditorsMeeting_11,              //  3
      iC_MClaimsList_12,                         //  4
      iC_MDecisionHealthPlan707_13,              //  5
      iC_MDecisionBankruptcy711_14,              //  6
      iC_MDistributionAccount729_15,             //  7
      iC_MDecisBankruptcyFinalized_16,           //  8
      iC_MDecisRestoreDebtorRights_17,           //  9
      iC_MStoppedLaw_18,                         // 10
      iC_MRenew744_19,                           // 11
      iC_MAppealedActs_20,                       // 12
      iC_MAppealResults_21,                      // 13
      iC_MSerialNo,                              // 14

      i_columns
     } ;
 } ;  // class BankruptcyMiddleEnum

class BankruptcyMiddleColumns :
        public BankruptcyMiddleEnum, public ColumnsArrIncMaxRows
 {
  protected :
    void MakePureLawColumns (TLawsuit & law, LawsuitAllSessions &,
                             BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                             TGSArray <OutRegDatedContainer> &,
                             const int) ;
    void MakeSideColumns (TLawsuit & law, LawsuitAllSessions &,
                          BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                          TGSArray <OutRegDatedContainer> &,
                          const int) ;
    void MakeSessionColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                             BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                             TGSArray <OutRegDatedContainer> &, const int);
    void MakeDecisionColumns (TLawsuit & law, LawsuitAllSessions &,
                              BankruptcyLawAllDecisions & decisions,
                              LawsuitAllInRegs &,
                              TGSArray <OutRegDatedContainer> &,
                              const int) ;
    void MakeAppealColumns (TLawsuit & law, LawsuitAllSessions &,
                            BankruptcyLawAllDecisions & decisions,
                            LawsuitAllInRegs & inRegs,
                            TGSArray <OutRegDatedContainer> & outRegs,
                            const int) ;
    void MakeColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                      BankruptcyLawAllDecisions & decisions,
                      LawsuitAllInRegs & inRegs,
                      TGSArray <OutRegDatedContainer> & outRegs,
                      TDatedContainer & lastDec,
                      TDatedContainer & lastSess,
                      const int serialNo) ;
  public :
    BankruptcyMiddleColumns (const char * formatFN, TWindow * parent,
                             TLawsuit & law, LawsuitAllSessions & sessions,
                             BankruptcyLawAllDecisions & decisions,
                             LawsuitAllInRegs & inRegs,
                             TGSArray <OutRegDatedContainer> & outRegs,
                             TDatedContainer & lastDec,
                             TDatedContainer & lastSess,
                             const int serialNo, const int minRows) ;
 } ;  // class BankruptcyMiddleColumns

class BankruptcyRightEnum
 {
  protected :
    enum
     {
      iC_RLawNo_1 = 0,                           //  1
      iC_RAppealedActs_20,                       //  2
      iC_RAppealResults_21,                      //  3
      iC_RJudge_22,                              //  4
      iC_RAssigneeInBankruptcy_23,               //  5
      iC_RRemark_24,                             //  6
      iC_RAppealBefore_10,                       //  7
      iC_RSerialNo,                              //  8

      i_columns
     } ;
 } ;  // class BankruptcyRightEnum

class BankruptcyRightColumns :
        public BankruptcyRightEnum, public ColumnsArrIncMaxRows
 {
  protected :
    void MakePureLawColumns (TLawsuit & law, LawsuitAllSessions &,
                             BankruptcyLawAllDecisions &,
                             LawsuitAllInRegs &,
                             TGSArray <OutRegDatedContainer> &,
                             const int) ;
    void MakeSideColumns (TLawsuit & law, LawsuitAllSessions &,
                          BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                          TGSArray <OutRegDatedContainer> &,
                          const int) ;
    void MakeSessionColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                             BankruptcyLawAllDecisions &,
                             LawsuitAllInRegs &,
                             TGSArray <OutRegDatedContainer> &, const int);
    void MakeDecisionColumns (TLawsuit & law, LawsuitAllSessions &,
                              BankruptcyLawAllDecisions & decisions,
                              LawsuitAllInRegs &,
                              TGSArray <OutRegDatedContainer> &,
                              const int) ;
    void MakeAppealColumns (TLawsuit & law, LawsuitAllSessions &,
                            BankruptcyLawAllDecisions & decisions,
                            LawsuitAllInRegs & inRegs,
                            TGSArray <OutRegDatedContainer> & outRegs,
                            const int) ;
    void MakeColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                      BankruptcyLawAllDecisions & decisions,
                      LawsuitAllInRegs & inRegs,
                      TGSArray <OutRegDatedContainer> & outRegs,
                      TDatedContainer & lastDec,
			    TDatedContainer & lastSess,
			    const int serialNo) ;
  public :
    BankruptcyRightColumns (const char * formatFN, TWindow * parent,
				    TLawsuit & law, LawsuitAllSessions & sessions,
				    BankruptcyLawAllDecisions & decisions,
				    LawsuitAllInRegs & inRegs,
				    TGSArray <OutRegDatedContainer> & outRegs,
				    TDatedContainer & lastDec,
				    TDatedContainer & lastSess,
				    const int serialNo, const int minRows) ;
 } ;  // class BankruptcyRightColumns

// ---------------------- BankruptcyLMROutFile - begin ----------------------

class BankruptcyLMROutFile : public frmHeadOutText
 {
  protected :
    virtual void fHOutput (const void * p, char * s, size_t len,
				   int algn, size_t skip, int dCipher) ;
  public :
    BankruptcyLMROutFile (TWindow * parent, const char * hFName) ;
    int FormattedHead (const BankruptcyWideDlgData & dlgData,
			     bool write = true) ;
    int FormattedHeadComleted (const BankruptcyWideDlgData & dlgData,
					 bool write = true) ;
    int FormattedTail (const BankruptcyWideDlgData & dlgData,
			     bool write = true) ;
    void WriteItem (ColumnsArrIncMaxRows & bC, const bool first) ;
    void NextLawToPrint (const int linesNeed, const bool addHead = true) ;
    virtual bool supportsFormatterTail () const ;
    virtual bool supportsAnyFormattedText () const ;
    size_t MinRowsForLaw () const ;
 } ;  // class BankruptcyLMROutFile

BankruptcyLMROutFile :: BankruptcyLMROutFile (TWindow * parent, const char * hFName) :
     frmHeadOutText (parent, hFName)
 {
 }  // BankruptcyLMROutFile :: BankruptcyLMROutFile

void BankruptcyLMROutFile ::
  fHOutput (const void * p, char * s, size_t len, int algn, size_t skip,
		int dCipher)
 {
  if (s)
   {
    const BankruptcyWideDlgData * pDlgD =
	reinterpret_cast <const BankruptcyWideDlgData *> (p) ;

    if (pDlgD)
     {
      if (! (BankruptcyWideDlgData_output (* pDlgD, * this, s, len, algn,
							 skip, dCipher) ||
             miscOutput (* this, s, len, algn, skip, dCipher)
            )
         )
       {
	  mkStrBlank (s, len + 1) ;
        writeString (s) ;
	 }
     }
   }
 }  // BankruptcyLMROutFile :: fHOutput

int BankruptcyLMROutFile ::
      FormattedHead (const BankruptcyWideDlgData & dlgData, bool write)
 {
  return formHead (& dlgData, write) ;
 }  // BankruptcyLMROutFile :: FormattedHead

int BankruptcyLMROutFile ::
  FormattedHeadComleted (const BankruptcyWideDlgData & dlgData, bool write)
 {
  int iRes = 0 ;

  if (supportsAnyFormattedText ())
   {
    CharPtr sect (strlen (FORMATTED_HEADER_SECTION) + 64) ;

    ostrstream (sect, sect.Size ())
      << FORMATTED_HEADER_SECTION << "_COMPLETED" << ends ;
    sect [sect.Size () - 1] = '\0' ;
    iRes = formHead (& dlgData, write, sect) ;
   }
  return iRes ;
 }  // BankruptcyLMROutFile :: FormattedHeadComleted

void BankruptcyLMROutFile ::
       WriteItem (ColumnsArrIncMaxRows & bC, const bool  // first
                 )
 {
  const bool first = true ;

  if (pM.newPageNeed ())
    toBeContinued () ;
  bC.notConstOutput (* this, first) ;
  pM.incLines (linesOfItem (first)) ;
 }  // BankruptcyLMROutFile :: WriteItem

void BankruptcyLMROutFile :: NextLawToPrint (const int linesNeed,
                                             const bool addHead)
 {
  if (! pM.enoughLinesLeft (linesNeed))
   {
    if (addHead)
	footherLine () ;
    newPage () ;
    if (addHead)
	head () ;
   }
 }  // BankruptcyLMROutFile :: NextLawToPrint

bool BankruptcyLMROutFile :: supportsFormatterTail () const
 {
  return true ;
 }  // BankruptcyLMROutFile :: supportsFormatterTail

bool BankruptcyLMROutFile :: supportsAnyFormattedText () const
 {
  return true ;
 }  // BankruptcyLMROutFile :: supportsAnyFormattedText

int BankruptcyLMROutFile ::
	FormattedTail (const BankruptcyWideDlgData & dlgData, bool write)
 {
  int iRes = 0 ;

  if (supportsFormatterTail ())
    iRes = formHead (& dlgData, write, FORMATTED_TAIL_SECTION) ;
  return iRes ;
 }  // BankruptcyLMROutFile :: FormattedTail

size_t BankruptcyLMROutFile :: MinRowsForLaw () const
 {
  return (size_t) extraVal1 ;
 }  // BankruptcyLMROutFile :: minRowsForLaw

// ---------------------- BankruptcyLMROutFile - end ----------------------

// ---------------------- BancruptcyHTMLWriter - begin ----------------------

class BancruptcyHTMLWriter : public HTMLOutput
{
  public:
    BancruptcyHTMLWriter(BankruptcyLMROutFile& outFileRef);
};  // class BancruptcyHTMLWriter

BancruptcyHTMLWriter::BancruptcyHTMLWriter(BankruptcyLMROutFile& outFileRef) :
  HTMLOutput(outFileRef)
{
}

// ---------------------- BancruptcyHTMLWriter - end ------------------------

// ---------------------- BankruptcyWideCheckup - begin ----------------------
// ------------------------------ Declaration --------------------------------

class BankruptcyWideCheckup
 {
  protected :
    TWindow * const pParent ;

    bool MakeFormatFNames (char * const fFNLeft, const size_t fFNLeftSz,
                           char * const fFNMiddle, const size_t fFNMiddleSz,
                           char * const fFNRight, const size_t fFNRightSz,
                   char * const fFNHtml, const size_t fFNHtmlSz  // 23.apr.2003
                          );
    void AdditionalCriteriaConditions (ostrstream & oss,
						   const BankruptcyWideDlgData &
                                       dlgData) ;  // 06.feb.2002
    bool MakeCriteria (char * const crit, const size_t critSz,
                       const BankruptcyWideDlgData & dlgData // 06.feb.2002
			    ) ;
    static void PrintLaws (const char * const criteria,
				   BankruptcyLMROutFile & ofLeft,
                           BankruptcyLMROutFile & ofLeftComplet,
				   const char * const formatFNLeft,
                           BankruptcyLMROutFile & ofMiddle,
                           BankruptcyLMROutFile & ofMiddleComplet,
                           const char * const formatFNMiddle,
                           BankruptcyLMROutFile & ofRight,
                           BankruptcyLMROutFile & ofRightComplet,
                           const char * const formatFNRight,
                           BankruptcyLMROutFile & ofHtml,  // 23.apr.2003-begin
                           BankruptcyLMROutFile & ofHtmlComplet,
                           const char * const formatFNHtml,  // 23.apr.2003-end
                           int & serialNo,
                           int & serialNoNonComplet,
                           int & serialNoComplet,
                           const BankruptcyWideDlgData & dlgData
                          ) ;  // 14.mar.2002

  public :
    BankruptcyWideCheckup (TWindow * parent) ;
    ~BankruptcyWideCheckup () ;
    void PrintBankruptcyWideCheckup(const BankruptcyWideDlgData& dlgData);
    static bool DateInRange (const CDate lo, const CDate hi,
                             const CDate test) ;
    static  // 14.mar.2002
    void SetLastEvents (LawsuitAllSessions & sessions,
                        BankruptcyLawAllDecisions & decisions,
                        TDatedContainer & lastSess,
                        TDatedContainer & lastDec,
                        const TLawsuit & law) ;
    static  // 14.mar.2002
    void PrtOneLaw (const int rows, BankruptcyLMROutFile & of,
                    ColumnsArrIncMaxRows & bC) ;
    static  // 14.mar.2002
    void PrintOneLaw (TWindow * const pParent,  // 14.mar.2002
                      TLawsuit & law,
                      BankruptcyLMROutFile & ofLeft,
                      const char * const formatFNLeft,
                      BankruptcyLMROutFile & ofMiddle,
                      const char * const formatFNMiddle,
                      BankruptcyLMROutFile & ofRight,
                      const char * const formatFNRight,
                      BankruptcyLMROutFile & ofHtml,  // 23.apr.2003
                      const char * const formatFNHtml,  // 23.apr.2003
                      const int serialNo,
                      const BankruptcyWideDlgData & dlgData,
                      const bool isCompleted  // 10.jun.2003
                      ) ;
    static  // 14.mar.2002
    void BeginDoc (BankruptcyLMROutFile & of,
                   const BankruptcyWideDlgData & dlgData,
                   const bool complet = false) ;
    static  // 14.mar.2002
    void EndDoc (BankruptcyLMROutFile & of,
                 const BankruptcyWideDlgData & dlgData) ;
    static  // 14.mar.2002
    void EndBindedDocs (BankruptcyLMROutFile & ofL, BankruptcyLMROutFile & ofM,
                        BankruptcyLMROutFile & ofR,
                        BankruptcyLMROutFile & ofH,  // 23.apr.2003
                        const BankruptcyWideDlgData & dlgData,
                        const bool isCompleted) ;
 } ;  // class BankruptcyWideCheckup

// ------------------------------ Declaration --------------------------------
// ---------------------- BankruptcyWideCheckup - end ----------------------


# define APPEAL_SENT_INREGS_ONLY 1

BankruptcyLeftColumns ::
  BankruptcyLeftColumns (const char * formatFN, TWindow * parent,
                         TLawsuit & law, LawsuitAllSessions & sessions,
                         BankruptcyLawAllDecisions & decisions,
                         LawsuitAllInRegs & inRegs,
                         TGSArray <OutRegDatedContainer> & outRegs,
                         TDatedContainer & lastSess,
                         TDatedContainer & lastDec,
                         const int serialNo, const int minRows) :
    ColumnsArrIncMaxRows (formatFN, parent, i_columns,
                          1 + OFFSET_BANKRUPTCY_LEFT, true)
 {
  sessions.Rewind () ;
  decisions.Rewind () ;
  inRegs.Rewind () ;
  MakeColumns (law, sessions, decisions, inRegs, outRegs,
               lastSess, lastDec, serialNo) ;
  defineMaxRows () ;
  if (maxRows < minRows)
    maxRows = minRows ;
 }  // BankruptcyLeftColumns :: BankruptcyLeftColumns

void BankruptcyLeftColumns ::
  MakePureLawColumns (TLawsuit & law, LawsuitAllSessions &,
                      BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                      TGSArray <OutRegDatedContainer> &, const int)
 {
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * pColumn ;

    if ((pColumn = columns [iC_LLawNo_1]) != NULL)
     {  // -- iC_LLawNo_1 --
      char tmp [64] ;

      TRCDKey_ (law.key).PrintShortest (tmp, sizeof (tmp)) ;
      pColumn -> addText (tmp) ;
     }  // -- iC_LLawNo_1 --
    if ((pColumn = columns [iC_LDate_4]) != NULL)
     {  // -- iC_LDate_4 --
      add_date_to_column (pColumn, law.date) ;
     }  // -- iC_LDate_4 --
    if ((pColumn = columns [iC_HTML_DateCompleted]) != NULL)
     {  // -- iC_HTML_DateCompleted --
      add_date_to_column (pColumn, law.completed) ;
     }  // -- iC_HTML_DateCompleted --
   }  // -- 0 --
 }  // BankruptcyLeftColumns :: MakePureLawColumns

void BankruptcyLeftColumns ::
  MakeSideColumns (TLawsuit & law, LawsuitAllSessions &,
                   BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                   TGSArray <OutRegDatedContainer> &, const int)
 {
  if (DatedContainer_ (law.key, law.kind, law.date).FilledOK ())
   {  // -- 0 --
    column * const pDebtorColumn = columns [iC_LDebtor_2] ;
    column * const pInitiatorColumn = columns [iC_LInitiator_3] ;

    if (pDebtorColumn || pInitiatorColumn)
     {
	TSideWind s ;
      CharPtr crit (1024) ;

      s.key = law.key ;
	s.kind = law.kind ;
	s.date = law.date ;
      s.FloatCriteria (crit) ;
	 {
        TQuery q (crit) ;

        while (q.Read ())
         {
          s << q ;
          if (s.involvement == INVOLVEMENT_DEBTOR ||
              s.involvement == INVOLVEMENT_DEFENDANT ||
              s.involvement == INVOLVEMENT_REQUESTER)
           {
            char tmp [128] ;


            if (ucnAndUcnTypeToName (s.ucn, s.ucnType, tmp, sizeof (tmp)))
             {
              if (s.involvement == INVOLVEMENT_DEBTOR ||
                  s.involvement == INVOLVEMENT_DEFENDANT)
                if (pDebtorColumn)
                  pDebtorColumn -> addText (tmp) ;
              if (s.involvement == INVOLVEMENT_REQUESTER)
                if (pInitiatorColumn)
                  pInitiatorColumn -> addText (tmp) ;
             }
           }
         }
       }
     }
   }  // -- 0 --
 }  // BankruptcyLeftColumns :: MakeSideColumns

void BankruptcyLeftColumns ::
  MakeSessionColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                      BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                      TGSArray <OutRegDatedContainer> &, const int)
 {
  sessions.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * const pCanceled100Column = columns [iC_LCanceled100_5] ;
    column * const pFirstCreditorsMeetingColumn =
                       columns [iC_LFirstCreditorsMeeting_11] ; // ! There
      // is equal processing for the FirstCreditorsMeeting_11 column
      // in BankruptcyMiddleColumns :: MakeSessionColumns

    if (pCanceled100Column || pFirstCreditorsMeetingColumn)
     {
      SessPtr pSess ;
      TSession & rSess = pSess.Ref () ;
      bool canceled100Printed = (pCanceled100Column == NULL || // No column
                                 law.finished.Empty ()//Law is not filished
                                ) ;
      bool firstCreditorsMeetingPrinted =
             (pFirstCreditorsMeetingColumn == NULL) ;

      while ((canceled100Printed == false ||
              firstCreditorsMeetingPrinted == false) &&
             sessions.NextWholeRecord (rSess))
       {
        const bool openSess =
          strchr (KIND_CLOSED_SESSIONS, rSess.result) == NULL ;
        const CDate mainSessionDate =
          openSess ? rSess.date : rSess.returned ;

        if (law.finished.Empty () == false &&
            mainSessionDate > law.finished)
          canceled100Printed = true ;
        if (pCanceled100Column && canceled100Printed == false)
         {
          if (sessionCancelsTheLaw (rSess, & law, openSess))
           {
            add_date_to_column (pCanceled100Column, mainSessionDate) ;
            canceled100Printed = true ;
           }
         }
        if (pFirstCreditorsMeetingColumn &&
            firstCreditorsMeetingPrinted == false) // ! There
      // is equal processing for the FirstCreditorsMeeting_11 column
      // in BankruptcyMiddleColumns :: MakeSessionColumns
          if (rSess.kind == KIND_CREDITORS_MEETING)
            if (RESULT_FIXED != rSess.result &&
                (strchr (RESULT_LEFTS, rSess.result) == NULL ||
		     LEFT_MEETING
		    )
               )
             {
              add_date_to_column (pFirstCreditorsMeetingColumn,
                                  mainSessionDate) ;
              firstCreditorsMeetingPrinted = true ;
             }
       }
     }
   }  // -- 0 --
 }  // BankruptcyLeftColumns :: MakeSessionColumns

void BankruptcyLeftColumns ::
  MakeDecisionColumns (TLawsuit & law, LawsuitAllSessions &,
                       BankruptcyLawAllDecisions & decisions,
                       LawsuitAllInRegs &,
                       TGSArray <OutRegDatedContainer> &, const int)
 {
  decisions.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * const pDecisionRefusalColumn =
                       columns [iC_LDecisionRefusal_6] ;
    column * const pDecisionCancel632Column =
                       columns [iC_LDecisionCancel632_7] ;
    column * const pAgreement740Column =
                       columns [iC_LAgreement740_8] ;
    column * const pDecision630Column =
                       columns [iC_LDecision630_9] ;

    if (pDecisionRefusalColumn || pDecisionCancel632Column ||
        pAgreement740Column || pDecision630Column)
     {
      DecPtr pDec ;
      TDecision & rDec = pDec.Ref () ;
      const CDate & accomplyDate = rDec.fromDate ;

      while (decisions.NextWholeRecord (rDec))
	 {
	  // 2011:013 LRQ: 632, 630 single -> sets
	  if (pDecisionRefusalColumn)
	    if (strchr (rDec.accomplys, ACCOMPLY_TL_631))
		add_date_to_column (pDecisionRefusalColumn, accomplyDate) ;
	  if (pDecisionCancel632Column)
	    //if (strchr (rDec.accomplys, ACCOMPLY_TL_632))
	    if (intersect(rDec.accomplys, ACCOMPLY_TL_632S))
		add_date_to_column (pDecisionCancel632Column, accomplyDate) ;
	  if (pAgreement740Column)
	    if (strchr (rDec.accomplys, ACCOMPLY_TL_740))
		add_date_to_column (pAgreement740Column, accomplyDate) ;
	  if (pDecision630Column)
	    //if (strchr (rDec.accomplys, ACCOMPLY_TL_630))
	    if (intersect(rDec.accomplys, ACCOMPLY_TL_630S))
            add_date_to_column (pDecision630Column, accomplyDate) ;
       }
     }
   }  // -- 0 --
 }  // BankruptcyLeftColumns :: MakeDecisionColumns

void BankruptcyLeftColumns ::
  ProcessAppealBeforeColumn (column * pAppealBeforeColumn,
                             const TInReg & iR,
                             TGSArray <OutRegDatedContainer> & outRegs,
                             const bool firstPrinted,
                             bool & appealBeforePrinted)
 {
  if (TRCDKey_ (iR.key).FilledOK ())
   {  // -- 0 --
    if (pAppealBeforeColumn)
     {  // -- pAppealBeforeColumn --
      if (iR.decision.type && strchr (TYPE_LAWSUITS, iR.decision.type))
       {  // -- pAppealBeforeColumn # 0 --
        if (firstPrinted == true)
          pAppealBeforeColumn -> addEmptyRow () ;
#     if APPEAL_SENT_INREGS_ONLY
#     else  // of APPEAL_SENT_INREGS_ONLY
        add_date_to_column (pAppealBeforeColumn, iR.date) ;
        appealBeforePrinted = true ;
#     endif  // of APPEAL_SENT_INREGS_ONLY
	  for (int i = 0 ; i < outRegs.Count () ; i ++)
         {  // -- pAppealBeforeColumn # 1 --
          OutRegDatedContainer * pORDC = outRegs [i] ;

          if (pORDC)
           {
            OutRegDatedContainer & rORDC = * pORDC ;

            if (rORDC.inNo == iR.key.no &&
                rORDC.inYear == iR.key.year)
             {
              DatedContainer_ aplAct (iR.lawsuit.type,
                                      iR.lawsuit.no,
                                      iR.lawsuit.year,
                                      iR.decisionKind,
                                      iR.decisionDate) ;
              bool printIt = false ;

              if (aplAct.FilledOK ())
               {
                DecPtr pDec ;
                TDecision & rDec = pDec.Ref () ;

                rDec.key = iR.lawsuit ;
                rDec.kind = iR.decisionKind ;
                rDec.date = iR.decisionDate ;
                if (rDec.Try ())  // Get is inappropriate here, because
                                  // the appealed act may be a session.
			//if (strchr (rDec.accomplys, ACCOMPLY_TL_630))
			if (intersect(rDec.accomplys, ACCOMPLY_TL_630S))
                    printIt = true ;
               }
              if (printIt)
               {
#     if APPEAL_SENT_INREGS_ONLY
                add_date_to_column (pAppealBeforeColumn, iR.date) ;
#     else  // of APPEAL_SENT_INREGS_ONLY
#     endif  // of APPEAL_SENT_INREGS_ONLY

                 {
                  CharPtr buff (1024 * 2) ;
                  TOutReg oR ;

                  oR << rORDC ;
                  AppealedLawsOutRegColumns ::
                    AddReceiversToColumn (pAppealBeforeColumn, buff,
                                          buff.Size () - 1, oR) ;
                  appealBeforePrinted = true ;
                 }
               }
             }
           }
         }  // -- pAppealBeforeColumn # 1 --
       }  // -- pAppealBeforeColumn # 0 --
     }  // -- pAppealBeforeColumn --
   }  // -- 0 --
 }  // BankruptcyLeftColumns :: ProcessAppealBeforeColumn

void BankruptcyLeftColumns ::
  ProcessAppealedActsColumn (column * pAppealedActsColumn,
                             TInReg & iR, const bool firstPrinted,
                             bool & appealedActsPrinted)
 {
  if (TRCDKey_ (iR.key).FilledOK ())
   {  // -- 0 --
    if (pAppealedActsColumn)
     {  // -- pAppealedActsColumn --
      if (iR.decision.type && strchr (TYPE_LAWSUITS, iR.decision.type))
       {  // -- pAppealedActsColumn # 0 --
        size_t bigBuffSz = 512 ;  // Starting space for identification
                                  // of the inreg and the appealed act.
        column clmn (SIZE_OF_LINE - 1) ;
        CharPtr buff (1024) ;

        if (firstPrinted == true)
          pAppealedActsColumn -> addEmptyRow () ;
        AppealedLawsInRegColumns ::
          AddAppealersToColumn (& clmn, buff, buff.Size (), iR,
                                false) ;
        for (int i = 0 ; i < clmn.getRowsCount () ; i ++)
         {
          buff [0] = '\0' ;
          strConcat (buff, clmn [i], buff.Size ()) ;
          strDelLastSpaces (buff) ;
          bigBuffSz += (strlen (buff) + 2) ;   // + ", "
         }
        clmn.reWind () ;
         {  // -- pAppealedActsColumn # 1 --
          CharPtr bigBuff (bigBuffSz) ;

           {  // -- pAppealedActsColumn # 2 --
            ostrstream oss (bigBuff, bigBuff.Size ()) ;

            oss << TRCDKeyContainer_ (iR.key, iR.kind) << " " ;
            if (DatedContainer_ (iR.lawsuit.type, iR.lawsuit.no,
                                 iR.lawsuit.year, iR.decisionKind,
                                 iR.decisionDate).FilledOK ())
             {  // -- pAppealedActsColumn # 3 --
              oss << " против " ;
               {  // -- pAppealedActsColumn # 4 --
                CDate dateToPrint ;
                bool decAppealed = false ;
                bool sessAppealed = false ;

                 {
                  DecPtr pDec ;
                  TDecision & rDec = pDec.Ref () ;

                  rDec.key = iR.lawsuit ;
                  rDec.kind = iR.decisionKind ;
                  rDec.date = iR.decisionDate ;
                  if (rDec.Try ())
                   {
                    decAppealed = true ;
                    dateToPrint = rDec.fromDate ;
                   }
                 }
                if (decAppealed == false)
                 {
                  if (iR.sessionKind)
                   {
                    SessPtr pSess ;
                    TSession & rSess = pSess.Ref () ;
                    CDate mainSessDate ;

                    rSess.key = iR.lawsuit ;
                    rSess.kind = iR.sessionKind ;
                    rSess.date = iR.decisionDate ;
                    if (rSess.Try ())
                     {
                      sessAppealed = true ;
                      dateToPrint = strchr (KIND_CLOSED_SESSIONS,
                                            rSess.kind) ?
                                        rSess.returned : rSess.date ;
                     }
                   }
                 }
                 {
                  char inregIdStr [128] ;

                  TRCDKey_ (iR.key).Print (inregIdStr,
                                           sizeof (inregIdStr)) ;
                  if (decAppealed || sessAppealed)
                   {
                    oss << Kind_ (iR.decisionKind) ;
                    if (dateToPrint.Empty () == false)
                      oss << " от " << dateToPrint ;
                    else
                      error ("%s:\n"
                             "Невалидна дата на обжалвания акт",
                             inregIdStr) ;
                   }
                  else
                   {
                    oss << "???" ;
                    error ("%s:\n""Невалиден обжалван акт", inregIdStr) ;
                   }
                 }
               }  // -- pAppealedActsColumn # 4 --
             }  // -- pAppealedActsColumn # 3 --
            for (int i = 0 ; i < clmn.getRowsCount () ; i ++)
             {
              buff [0] = '\0' ;
              strConcat (buff, clmn [i], buff.Size ()) ;
              strDelLastSpaces (buff) ;
              oss << (i == 0 ? " от " : ", ") << buff ;
             }
            oss << ends ;
           }  // -- pAppealedActsColumn # 2 --
          bigBuff [bigBuff.Size () - 1] = '\0' ;
          pAppealedActsColumn -> addText (bigBuff) ;
          appealedActsPrinted = true ;
         }  // -- pAppealedActsColumn # 1 --
       }  // -- pAppealedActsColumn # 0 --
     }  // -- pAppealedActsColumn --
   }  // -- 0 --
 }  // BankruptcyLeftColumns :: ProcessAppealedActsColumn

void BankruptcyLeftColumns ::
  ProcessAppealResultsColumn (column * pAppealResultsColumn,
                              const TInReg & iR,
                              TGSArray <OutRegDatedContainer> & outRegs,
                              const bool firstPrinted,
                              bool & appealResultsPrinted, const int oRs)
 {
  if (TRCDKey_ (iR.key).FilledOK ())
   {  // -- 0 --
    if (pAppealResultsColumn)
     {  // -- pAppealResultsColumn --
      if (oRs != 0)
        if (firstPrinted == true)
          pAppealResultsColumn -> addEmptyRow () ;
	for (int i = 0 ; i < outRegs.Count () ; i ++)
       {  // -- pAppealResultsColumn # 1 --
        OutRegDatedContainer * pORDC = outRegs [i] ;

        if (pORDC &&
            pORDC -> inNo == iR.key.no &&
            pORDC -> inYear == iR.key.year)
         {  // -- pAppealResultsColumn # 2 --
          int results = 0 ;
          const char * const higherInst1 = HIGHER_INST_1 ;
          const char * const higherInst2 = HIGHER_INST_2 ;
          CharPtr wrkBuff (SIZE_OF_LONGER_NAME + 128) ;
          const size_t wrkBuffL = wrkBuff.Size () - 1 ;

          if (pORDC -> resultI && pORDC -> resultI != RESOLUTION_FAKE)
            results ++ ;
          if (pORDC -> resultII &&
              pORDC -> resultII != RESOLUTION_FAKE)
            results ++ ;
          if (pORDC -> resultSuper &&
              pORDC -> resultSuper != RESOLUTION_FAKE)
            results ++ ;
          if (pORDC -> resultMJ &&
              pORDC -> resultMJ != RESOLUTION_FAKE)
            results ++ ;

// 22.jan.2002 - begin
//          if (oRs != 0)
//           {
//            ostrstream (wrkBuff, wrkBuffL)
//              << TRCDKeyContainer_ (pORDC -> key, pORDC -> kind)
//              << ends ;
//            wrkBuff [wrkBuffL - 1] = '\0' ;
//            pAppealResultsColumn -> addText (wrkBuff) ;
//           }
// 22.jan.2002 - end
          add_resolution_name_to_column (wrkBuff, wrkBuffL,
                                     results > 1 ? higherInst1 : "",
                                         pORDC -> resultI,
                                         pAppealResultsColumn) ;
          add_resolution_name_to_column (wrkBuff, wrkBuffL,
//                           results > 1 ? SUPREME_CONCERN_COURT : "",
                                         results > 1 ? "ВКС" : "",
                                         pORDC -> resultSuper,
                                         pAppealResultsColumn) ;
          add_resolution_name_to_column (wrkBuff, wrkBuffL,
                                     results > 1 ? higherInst2 : "",
                                         pORDC -> resultII,
                                         pAppealResultsColumn) ;
          add_resolution_name_to_column (wrkBuff, wrkBuffL,
                            results > 1 ? AppNameMinistryShort : "",
                                         pORDC -> resultMJ,
                                         pAppealResultsColumn) ;

           {  // 22.jan.2001 - begin
            TOutReg oR ;

            oR << (* pORDC) ;
            if (oR.Get ())
             {
		  if (results)
		    // 2012:003 ERR
		    if (any(oR.remark))
		     {
			pAppealResultsColumn -> addEmptyRow () ;
			pAppealResultsColumn -> addText (oR.remark) ;
		     }
		  if (strchr (RESULT_IOREG_RETURNEDS, oR.result) == NULL)
		   {
		    CharPtr buff (1024 * 2) ;

		    AppealedLawsOutRegColumns ::
			AddReceiversToColumn (pAppealResultsColumn, buff,
						    buff.Size () - 1, oR) ;
		    ostrstream (buff, buff.Size ())
			<< "от " << oR.date << ends ;
		    buff [buff.Size () - 1] = '\0' ;
		    pAppealResultsColumn -> addText (oR.remark) ;
               }
             }
           }  // 22.jan.2001 - end
          if (oRs != 0)
            appealResultsPrinted = true ;
         }  // -- pAppealResultsColumn # 2 --
       }  // -- pAppealResultsColumn # 1 --
     }  // -- pAppealResultsColumn --
   }  // -- 0 --
 }   // BankruptcyLeftColumns :: ProcessAppealResultsColumn

void BankruptcyLeftColumns :: MakeAppealColumns (TLawsuit & law,
                                   BankruptcyLawAllDecisions &,
                                   LawsuitAllInRegs & inRegs,
                                TGSArray <OutRegDatedContainer> & outRegs,
                                   column * const pAppealBeforeColumn,
                                   column * const pAppealedActsColumn,
                                   column * const pAppealResultsColumn)
 {
  inRegs.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    if (pAppealBeforeColumn || pAppealedActsColumn || pAppealResultsColumn)
     {  // -- 1 --
	TInReg iR ;
	columnArray forAdjusting;
	bool firstPrinted = false ;

      forAdjusting.Add (pAppealBeforeColumn) ;
      forAdjusting.Add (pAppealedActsColumn) ;
      forAdjusting.Add (pAppealResultsColumn) ;
      while (inRegs.NextWholeRecord (iR))
       {  // -- 2 --
        bool appealBeforePrinted = false ;
        bool appealedActsPrinted = false ;
        bool appealResultsPrinted = false ;
        int oRs = 0 ;

        for (int i = 0 ; i < outRegs.Count () ; i ++)
         {
          OutRegDatedContainer * pORDC = outRegs [i] ;

          if (pORDC &&
              pORDC -> inNo == iR.key.no &&
              pORDC -> inYear == iR.key.year)
            oRs ++ ;
         }
#     if APPEAL_SENT_INREGS_ONLY
        if (oRs != 0)
#     else  // of APPEAL_SENT_INREGS_ONLY
#     endif  // of APPEAL_SENT_INREGS_ONLY
         {  // -- 3 --
          ProcessAppealBeforeColumn (pAppealBeforeColumn, iR,
                                     outRegs, firstPrinted,
                                     appealBeforePrinted) ;
          ProcessAppealedActsColumn (pAppealedActsColumn, iR,
                                     firstPrinted, appealedActsPrinted) ;
          if (appealedActsPrinted)
            ProcessAppealResultsColumn (pAppealResultsColumn, iR, outRegs,
                                        firstPrinted, appealResultsPrinted,
                                        oRs) ;
         }  // -- 3 --
        adjust_column_rows (forAdjusting) ;
        if (appealedActsPrinted || appealBeforePrinted ||
            appealResultsPrinted)
          firstPrinted = true ;
       }  // -- 2 --
     }  // -- 1 --
   }  // -- 0 --
 }  // BankruptcyLeftColumns ::

void BankruptcyLeftColumns ::
  MakeAppealColumns (TLawsuit & law, LawsuitAllSessions &,
                     BankruptcyLawAllDecisions & decisions,
                     LawsuitAllInRegs & inRegs,
			   TGSArray <OutRegDatedContainer> & outRegs, const int)
 {
  inRegs.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    MakeAppealColumns (law, decisions, inRegs, outRegs,
                       columns [iC_LAppealBefore_10],
                       columns [iC_LAppealedActs_20],
                       columns [iC_LAppealResults_21]) ;
   }  // -- 0 --
 }  // BankruptcyLeftColumns :: MakeAppealColumns

const char* const CANCELED_100 =
  "Прекратено на основание чл. 100 ГПК";

void BankruptcyLeftColumns ::
  MakeColumns (TLawsuit & law, LawsuitAllSessions & sessions,
               BankruptcyLawAllDecisions & decisions,
               LawsuitAllInRegs & inRegs,
               TGSArray <OutRegDatedContainer> & outRegs,
               TDatedContainer & lastSess, TDatedContainer & lastDec,
               const int serialNo)
 {
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    MakePureLawColumns (law, sessions, decisions, inRegs, outRegs,
                        serialNo) ;
    MakeSideColumns (law, sessions, decisions, inRegs, outRegs, serialNo) ;
    MakeSessionColumns (law, sessions, decisions, inRegs, outRegs,
                        serialNo) ;
    MakeDecisionColumns (law, sessions, decisions, inRegs, outRegs,
                         serialNo) ;
    MakeAppealColumns (law, sessions, decisions, inRegs, outRegs,
                       serialNo) ;
   }  // -- 0 --
  //info("%s::MakeColumns", typeid(*this).name());
   {
    column * pColumn ;

    if ((pColumn = columns [iC_LSerialNo]) != NULL)
     {  // -- iC_LSerialNo --
      addLongToColumn (pColumn, serialNo, true) ;
     }  // -- iC_LSerialNo --
    if ((pColumn = columns [iC_HTML_Accomply]) != NULL)
     {  // -- iC_HTML_Accomply --
      DecPtr pDec;
      SessPtr pSess;
      bool match = false;
      bool somethingPrinted = false;  // 30.may.2003

      //info("iC_HTML_Accomply");
      decisions.Rewind();
      while(match == false && decisions.NextWholeRecord(pDec.Ref()))
       {
        if(pDec.Ref().sessionKind &&
          strchr(KIND_CLOSED_SESSIONS, pDec.Ref().sessionKind))
         {
          if(law.completed == pDec.Ref().date)
            match = true;
         }
        else
         {
          pSess.Ref().key = pDec.Ref().key;
          pSess.Ref().date = pDec.Ref().date;
          pSess.Ref().kind = pDec.Ref().sessionKind;
          if(DatedContainer_(pSess.Ref()).FilledOK())
           {
            if(pSess.Ref().Get())
             {
              if(law.completed == pSess.Ref().returned)
                match = true;
             }
            else
             {
              char tmp[256];

              DatedContainer_(pSess.Ref()).Print(tmp, sizeof(tmp));
              info("%s липсва (%d)", tmp, __LINE__);
             }
           }
         }
        if(match)
         {
          TAccomply accomply;

          for (int i = 0; i < strlen(pDec.Ref().accomplys); i++)
           {
            accomply.accomply = pDec.Ref().accomplys[i];
            if(accomply.Get())
             {
              pColumn->addText(accomply.name);
              somethingPrinted = true ;
             }
           }
         }
       }
      if(match == false)
       {
        CDate decD;
        CDate sessD;
        DecPtr pDec;
        SessPtr pSess;
        bool sessExtracted = false;

        clear_CDate(decD);
        clear_CDate(sessD);
        if(DatedContainer_(lastDec).FilledOK())
          decD = lastDec.date;
        if(DatedContainer_(lastSess).FilledOK())
         {
          if(strchr(KIND_CLOSED_SESSIONS, lastSess.kind) == NULL)
            sessD = lastSess.date;
          else
           {
            pSess.Ref() << lastSess;
            if(DatedContainer_(pSess.Ref()).FilledOK())
             {
              if(pSess.Ref().Get())
               {
                sessExtracted = true;
                sessD = pSess.Ref().returned;
               }
              else
               {
                char tmp[256];

                DatedContainer_(pSess.Ref()).Print(tmp, sizeof(tmp));
                info("%s липсва (%d)", tmp, __LINE__);
               }
             }
           }
         }
        if(decD.Empty() == false && decD >= sessD)
         {
          TAccomply accomply;

          pDec.Ref() << lastDec;
          if(pDec.Ref().Get())
            for (int i = 0; i < strlen(pDec.Ref().accomplys); i++)
             {
              accomply.accomply = pDec.Ref().accomplys[i];
              if(accomply.Get())
               {
                pColumn->addText(accomply.name);
                somethingPrinted = true ;
               }
             }
         }
        else
          if(sessD.Empty() == false)
           {
            if(sessExtracted == false)
             {
              pSess.Ref() << lastSess;
              if(DatedContainer_(pSess.Ref()).FilledOK())
               {
                if(pSess.Ref().Get())
                  sessExtracted = true;
                else
                 {
                  char tmp[256];

                  DatedContainer_(pSess.Ref()).Print(tmp, sizeof(tmp));
                  info("%s липсва (%d)", tmp, __LINE__);
                 }
               }
             }
            if(sessExtracted)
             {
              if(sessionCancelsTheLaw(pSess.Ref(), &law,
                strchr(KIND_CLOSED_SESSIONS, pSess.Ref().kind) == NULL))
               {
                pColumn->addText(CANCELED_100);
                somethingPrinted = true ;
               }
//              else
//               {
//                char n[SIZE_OF_LONGER_NAME];
//
//                resultToString(pSess.Ref().result, n, sizeof(n))
//                pColumn->AddText(n);
//                pColumn->AddText(pSess.Ref().text);
//               }
             }
           }
       }
// 30.may.2003 - begin
      if (somethingPrinted == false && law.finished.Empty () == false)
       {
        SessPtr pSess ;
        TSession & rSess = pSess.Ref () ;
        bool sessMatch = false ;

        sessions.Rewind () ;  // 04.jul.2003 - begin
        while (law.completed.Empty() == false &&
               somethingPrinted == false && sessMatch == false &&
               sessions.NextWholeRecord (rSess))
         {  // Loop - seek a session, that may match with law.completed - begin
          const bool openSess =
            strchr (KIND_CLOSED_SESSIONS, rSess.result) == NULL ;
          bool testTheSess =
            (openSess ? rSess.date == law.completed :
              rSess.returned == law.completed) ;

          if(testTheSess)
           {  // testTheSess
            LawPtr pFakeLaw;

            pFakeLaw.Ref () << law ;
            pFakeLaw.Ref ().finished = law.completed ;
            if (sessionCancelsTheLaw (rSess, pFakeLaw.Ptr(), openSess))
             {
              sessMatch = true ;
		  if (any(rSess.text))
		   {
		    pColumn -> addText (rSess.text) ;
                somethingPrinted = true ;
               }
              else
               {
                DecPtr pDec ;
                TDecision & rDec = pDec.Ref () ;
                bool decMatch = false ;

                decisions.Rewind () ;
                while (decMatch == false && somethingPrinted == false &&
                       decisions.NextKey (rDec))
                 {
                  if (rSess.date == rDec.date && rDec.Get())
                   {
                    DatedContainer_ ctrl(rDec) ;

                    ctrl.kind = rDec.sessionKind ;
                    if (DatedContainer_(rSess) == ctrl &&
                        sessionCancelsTheLaw (rSess, pFakeLaw.Ptr (), openSess,
                          & rDec)
                         // Because there may be more than one decision...
                       )
                     {
                      decMatch = true ;
			    if (any(rDec.text))
			     {
				pColumn ->addText(rDec.text);
                        somethingPrinted = true ;
                       }
                     }
                   }
                 }
               }
             }
           }  // testTheSess
         }  // Loop - seek a session, that may match with law.completed - end
        // 04.jul.2003 - end
        sessions.Rewind () ;
        while (somethingPrinted == false && sessMatch == false &&
               sessions.NextWholeRecord (rSess))
         {
          const bool openSess =
            strchr (KIND_CLOSED_SESSIONS, rSess.result) == NULL ;

          if (sessionCancelsTheLaw (rSess, & law, openSess))
           {
            sessMatch = true ;
		if (any(rSess.text))
             {
              pColumn -> addText (rSess.text) ;
              somethingPrinted = true ;
             }
            else
             {
              DecPtr pDec ;
              TDecision & rDec = pDec.Ref () ;
              bool decMatch = false ;

              decisions.Rewind () ;
              while (decMatch == false && somethingPrinted == false &&
                     decisions.NextKey (rDec))
               {
                if (rSess.date == rDec.date && rDec.Get())
                 {
                  DatedContainer_ ctrl(rDec) ;

                  ctrl.kind = rDec.sessionKind ;
                  if (DatedContainer_(rSess) == ctrl &&
                      sessionCancelsTheLaw (rSess, & law, openSess, & rDec)//Be-
                        // cause there may be more than one decision...
                     )
                   {
                    decMatch = true ;
			  if (any(rDec.text))
                     {
                      pColumn -> addText (rDec.text) ;
                      somethingPrinted = true ;
                     }
                   }
                 }
               }
             }
           }
         }
       }  // 30.may.2003 - end
     }  // -- iC_HTML_Accomply --
    //else
      //info("iC_HTML_Accomply = %d", iC_HTML_Accomply);
    if ((pColumn = columns [iC_HTML_LastActDate]) != NULL)
     {  // -- iC_HTML_LastActDate --
      CDate decD;
      CDate sessD;

      //info("iC_HTML_LastActDate");
      clear_CDate(decD);
      clear_CDate(sessD);
      if(DatedContainer_(lastDec).FilledOK())
        decD = lastDec.date;
      if(DatedContainer_(lastSess).FilledOK())
        sessD = lastSess.date;
      if(decD.Empty() == false && decD >= sessD)
        add_date_to_column (pColumn, decD) ;
      else
        if(sessD.Empty() == false)
          add_date_to_column (pColumn, sessD) ;
     }  // -- iC_HTML_LastActDate --
    if ((pColumn = columns [iC_HTML_LastAct]) != NULL)
     {  // -- iC_HTML_LastAct --
      CDate decD;
      CDate sessD;

      //info("iC_HTML_LastAct");
      clear_CDate(decD);
      clear_CDate(sessD);
      if(DatedContainer_(lastDec).FilledOK())
        decD = lastDec.date;
      if(DatedContainer_(lastSess).FilledOK())
        sessD = lastSess.date;
      if(decD.Empty() == false && decD >= sessD)
       {
        DecPtr pDec;

        pDec.Ref() << lastDec;
        if(DatedContainer_(pDec.Ref()).FilledOK() && pDec.Ref().Get())
         {
          TAccomply accomply;

          for (int i = 0; i < strlen(pDec.Ref().accomplys); i++)
           {
            accomply.accomply = pDec.Ref().accomplys[i];
            if(accomply.Get())
              pColumn->addText(accomply.name);
           }
         }
       }
      else
        if(sessD.Empty() == false)
         {
          SessPtr pSess;

          pSess.Ref() << lastSess;
          if(DatedContainer_(pSess.Ref()).FilledOK())
           {
            if(pSess.Ref().Get())
             {
              if(sessionCancelsTheLaw(pSess.Ref(), &law,
                strchr(KIND_CLOSED_SESSIONS, pSess.Ref().kind) == NULL))
                pColumn->addText(CANCELED_100);
             }   
            else
             {
              char tmp[256];

              DatedContainer_(pSess.Ref()).Print(tmp, sizeof(tmp));
              info("%s липсва (%d)", tmp, __LINE__);
             }
           }
         }
     }  // -- iC_HTML_LastAct --
   }
 }  // BankruptcyLeftColumns :: MakeColumns

BankruptcyMiddleColumns ::
  BankruptcyMiddleColumns (const char * formatFN, TWindow * parent,
                           TLawsuit & law, LawsuitAllSessions & sessions,
                           BankruptcyLawAllDecisions & decisions,
                           LawsuitAllInRegs & inRegs,
                           TGSArray <OutRegDatedContainer> & outRegs,
                           TDatedContainer & lastSess,
                           TDatedContainer & lastDec,
                           const int serialNo, const int minRows) :
    ColumnsArrIncMaxRows (formatFN, parent, i_columns,
                          1 + OFFSET_BANKRUPTCY_MIDDLE, true)
 {
  sessions.Rewind () ;
  decisions.Rewind () ;
  inRegs.Rewind () ;
  MakeColumns (law, sessions, decisions, inRegs, outRegs,
               lastSess, lastDec, serialNo) ;
  defineMaxRows () ;
  if (maxRows < minRows)
    maxRows = minRows ;
 }  // BankruptcyMiddleColumns :: BankruptcyMiddleColumns

void BankruptcyMiddleColumns ::
  MakePureLawColumns (TLawsuit & law, LawsuitAllSessions &,
                      BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                      TGSArray <OutRegDatedContainer> &, const int)
 {
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * pColumn ;

    if ((pColumn = columns [iC_MLawNo_1]) != NULL)
     {  // -- iC_LLawNo_1 --
      char tmp [64] ;

      TRCDKey_ (law.key).PrintShortest (tmp, sizeof (tmp)) ;
      pColumn -> addText (tmp) ;
     }  // -- iC_LLawNo_1 --
   }  // -- 0 --
 }  // BankruptcyMiddleColumns :: MakePureLawColumns

void BankruptcyMiddleColumns ::
  MakeSideColumns (TLawsuit & law, LawsuitAllSessions &,
                   BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                   TGSArray <OutRegDatedContainer> &, const int)
 {
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
   }  // -- 0 --
 }  // BankruptcyMiddleColumns :: MakeSideColumns

void BankruptcyMiddleColumns ::
  MakeSessionColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                      BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                      TGSArray <OutRegDatedContainer> &, const int)
 {
  sessions.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * const pStoppedLawColumn = columns [iC_MStoppedLaw_18] ;
    column * const pFirstCreditorsMeetingColumn =
                       columns [iC_MFirstCreditorsMeeting_11] ;  // ! There
      // is equal processing for the FirstCreditorsMeeting_11 column
      // in BankruptcyLeftColumns :: MakeSessionColumns

    if (pStoppedLawColumn || pFirstCreditorsMeetingColumn)
     {  // -- 1 --
      SessPtr pSess ;
      TSession & rSess = pSess.Ref () ;
      bool firstCreditorsMeetingPrinted =
             (pFirstCreditorsMeetingColumn == NULL) ;
      AloneDatedContainer_xK lastStop ;  // 06.feb.2002  It is empty.       

      while (sessions.NextWholeRecord (rSess))
       {  // -- 2 --
        const bool openSess =
          strchr (KIND_CLOSED_SESSIONS, rSess.result) == NULL ;
        const CDate mainSessionDate =
          openSess ? rSess.date : rSess.returned ;

//        if (strchr (RESULT_STOPPEDS, rSess.result))  // 06.feb.2002 - begin
//          if (pStoppedLawColumn)
//           {
//            add_date_to_column (pStoppedLawColumn, mainSessionDate) ;
//            pStoppedLawColumn -> addText (rSess.text) ;
//           }
        if (pStoppedLawColumn)
          if (strchr (RESULT_STOPPEDS, rSess.result))
            lastStop << rSess ;
          else
            if (strchr (KIND_CLOSED_SESSIONS, rSess.kind) == NULL)
              lastStop.Clear () ;  // 06.feb.2002 - end
        if (pFirstCreditorsMeetingColumn &&
            firstCreditorsMeetingPrinted == false) // ! There
      // is equal processing for the FirstCreditorsMeeting_11 column
      // in BankruptcyLeftColumns :: MakeSessionColumns
          if (rSess.kind == KIND_CREDITORS_MEETING)
            if (RESULT_FIXED != rSess.result &&
                (strchr (RESULT_LEFTS, rSess.result) == NULL ||
		     LEFT_MEETING
		    )
               )
             {
              add_date_to_column (pFirstCreditorsMeetingColumn,
                                  mainSessionDate) ;
              firstCreditorsMeetingPrinted = true ;
             }
       }  // -- 2 --
      if (pStoppedLawColumn)  // 06.feb.2002 - begin
        if (DatedContainer_ (lastStop).FilledOK ())
         {
          rSess << lastStop ;
          if (rSess.Get ())
           {
             {
              const bool openSess =
                strchr (KIND_CLOSED_SESSIONS, rSess.result) == NULL ;
              const CDate mainSessionDate =
                openSess ? rSess.date : rSess.returned ;

              add_date_to_column (pStoppedLawColumn, mainSessionDate) ;
             } 
            pStoppedLawColumn -> addText (rSess.text) ;
           }
         }  // 06.feb.2002 - end
     }  // -- 1 --
   }  // -- 0 --
 }  // BankruptcyMiddleColumns :: MakeSessionColumns

void BankruptcyMiddleColumns ::
  MakeDecisionColumns (TLawsuit & law, LawsuitAllSessions &,
                       BankruptcyLawAllDecisions & decisions,
                       LawsuitAllInRegs &,
                       TGSArray <OutRegDatedContainer> &, const int)
 {
  decisions.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * const pClaimsListColumn =
                       columns [iC_MClaimsList_12] ;
    column * const pDecisionHealthPlan707Column =
                       columns [iC_MDecisionHealthPlan707_13] ;
    column * const pDecisionBankruptcy711Column =
                       columns [iC_MDecisionBankruptcy711_14] ;
    column * const pDistributionAccountColumn =
                       columns [iC_MDistributionAccount729_15] ;
    column * const pDecisBankruptcyFinalizedColumn =
                       columns [iC_MDecisBankruptcyFinalized_16] ;
    column * const pDecisRestoreDebtorRightsColumn =
                       columns [iC_MDecisRestoreDebtorRights_17] ;
    column * const pRenew744Column =
                       columns [iC_MRenew744_19] ;

    if (pClaimsListColumn || pDecisionHealthPlan707Column ||
        pDecisionBankruptcy711Column || pDistributionAccountColumn ||
        pDecisBankruptcyFinalizedColumn ||
        pDecisRestoreDebtorRightsColumn || pRenew744Column)
     {
      DecPtr pDec ;
      TDecision & rDec = pDec.Ref () ;
      const CDate & accomplyDate = rDec.fromDate ;

      while (decisions.NextWholeRecord (rDec))
       {
        if (pClaimsListColumn)
          if (strchr (rDec.accomplys, ACCOMPLY_TL_692))
            add_date_to_column (pClaimsListColumn, accomplyDate) ;
        if (pDecisionHealthPlan707Column)
          if (strchr (rDec.accomplys, ACCOMPLY_TL_707))
            add_date_to_column (pDecisionHealthPlan707Column,
                                accomplyDate) ;
        if (pDecisionBankruptcy711Column)
          if (strchr (rDec.accomplys, ACCOMPLY_TL_711))
            add_date_to_column (pDecisionBankruptcy711Column,
                                accomplyDate) ;
        if (pDistributionAccountColumn)
          if (strchr (rDec.accomplys, ACCOMPLY_TL_729))
            add_date_to_column (pDistributionAccountColumn, accomplyDate) ;
        if (pDecisBankruptcyFinalizedColumn)
          if (strchr (rDec.accomplys, ACCOMPLY_TL_735))
            add_date_to_column (pDecisBankruptcyFinalizedColumn,
                                accomplyDate) ;
        if (pDecisRestoreDebtorRightsColumn)
          if (strchr (rDec.accomplys, ACCOMPLY_TL_755))
            add_date_to_column (pDecisRestoreDebtorRightsColumn,
                                accomplyDate) ;
        if (pRenew744Column)
          if (strchr (rDec.accomplys, ACCOMPLY_TL_744))
            add_date_to_column (pRenew744Column, accomplyDate) ;
       }
     }
   }  // -- 0 --
 }  // BankruptcyMiddleColumns :: MakeDecisionColumns

void BankruptcyMiddleColumns ::
  MakeAppealColumns (TLawsuit & law, LawsuitAllSessions &,
                     BankruptcyLawAllDecisions & decisions,
                     LawsuitAllInRegs & inRegs,
                     TGSArray <OutRegDatedContainer> & outRegs, const int)
 {
  inRegs.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    BankruptcyLeftColumns ::
      MakeAppealColumns (law, decisions, inRegs, outRegs,
                         columns [iC_MAppealBefore_10],
                         columns [iC_MAppealedActs_20],
                         columns [iC_MAppealResults_21]) ;
   }  // -- 0 --
 }  // BankruptcyMiddleColumns :: MakeAppealColumns

void BankruptcyMiddleColumns ::
  MakeColumns (TLawsuit & law, LawsuitAllSessions & sessions,
               BankruptcyLawAllDecisions & decisions,
               LawsuitAllInRegs & inRegs,
               TGSArray <OutRegDatedContainer> & outRegs,
               TDatedContainer &, TDatedContainer &,
               const int serialNo)
 {
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    MakePureLawColumns (law, sessions, decisions, inRegs, outRegs,
                        serialNo) ;
    MakeSideColumns (law, sessions, decisions, inRegs, outRegs, serialNo) ;
    MakeSessionColumns (law, sessions, decisions, inRegs, outRegs,
                        serialNo) ;
    MakeDecisionColumns (law, sessions, decisions, inRegs, outRegs,
                         serialNo) ;
    MakeAppealColumns (law, sessions, decisions, inRegs, outRegs,
                       serialNo) ;
   }  // -- 0 --
   {
    column * pColumn ;

    if ((pColumn = columns [iC_MSerialNo]) != NULL)
     {  // -- iC_LSerialNo --
      addLongToColumn (pColumn, serialNo, true) ;
     }  // -- iC_LSerialNo --
   }
 }  // BankruptcyMiddleColumns :: MakeColumns

BankruptcyRightColumns ::
  BankruptcyRightColumns (const char * formatFN, TWindow * parent,
                          TLawsuit & law, LawsuitAllSessions & sessions,
                          BankruptcyLawAllDecisions & decisions,
                          LawsuitAllInRegs & inRegs,
                          TGSArray <OutRegDatedContainer> & outRegs,
                          TDatedContainer & lastSess,
                          TDatedContainer & lastDec,
                          const int serialNo, const int minRows) :
    ColumnsArrIncMaxRows (formatFN, parent, i_columns,
                          1 + OFFSET_BANKRUPTCY_RIGHT, true)
 {
  sessions.Rewind () ;
  decisions.Rewind () ;
  inRegs.Rewind () ;
  MakeColumns (law, sessions, decisions, inRegs, outRegs,
               lastSess, lastDec, serialNo) ;
  defineMaxRows () ;
  if (maxRows < minRows)
    maxRows = minRows ;
 }  // BankruptcyRightColumns :: BankruptcyRightColumns

void BankruptcyRightColumns ::
  MakePureLawColumns (TLawsuit & law, LawsuitAllSessions &,
                      BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                      TGSArray <OutRegDatedContainer> &, const int)
 {
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * pColumn ;
    
    if ((pColumn = columns [iC_RLawNo_1]) != NULL)
     {  // -- iC_LLawNo_1 --
      char tmp [64] ;

      TRCDKey_ (law.key).PrintShortest (tmp, sizeof (tmp)) ;
      pColumn -> addText (tmp) ;
     }  // -- iC_LLawNo_1 --
   }  // -- 0 --
 }  // BankruptcyRightColumns :: MakePureLawColumns

void BankruptcyRightColumns ::
  MakeSideColumns (TLawsuit & law, LawsuitAllSessions &,
                   BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                   TGSArray <OutRegDatedContainer> &, const int)
 {
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
   }  // -- 0 --
 }  // BankruptcyRightColumns :: MakeSideColumns

void BankruptcyRightColumns ::
  MakeSessionColumns (TLawsuit & law, LawsuitAllSessions & sessions,
                      BankruptcyLawAllDecisions &, LawsuitAllInRegs &,
                      TGSArray <OutRegDatedContainer> &, const int)
 {
  sessions.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * const pJudgeColumn = columns [iC_RJudge_22] ;

    if (pJudgeColumn)
     {
      SessPtr pSess ;
      TSession & rSess = pSess.Ref () ;
      char lastJudge [sizeof (law.judge)] ;
      CDate lastJudgeDate = law.date ;
      CDate prevSessDate = law.date ;

//      ostrstream (lastJudge, sizeof (lastJudge))  // 06.feb.2002 - begin
//        << law.judge << ends ;
//      lastJudge [sizeof (lastJudge) - 1] = '\0' ;
      lastJudge [0] = '\0' ;  // 06.feb.2002 - end
      while (sessions.NextWholeRecord (rSess))
       {
        if (pJudgeColumn)
         {
          if (strcmp (lastJudge, rSess.judge))
            if ((rSess.flags & SESSION_CHANJED) ==
                SESSION_CHANJED)  // 06.jan.2002
             {
              lastJudgeDate = prevSessDate ;
              ostrstream (lastJudge, sizeof (lastJudge))
                << rSess.judge << ends ;
              lastJudge [sizeof (lastJudge) - 1] = '\0' ;
             }
          prevSessDate = rSess.date ;
         }
       }
      if (pJudgeColumn)
       {
        if (lastJudge [0] == '\0')  // 06.feb.2002 - begin
         {
          ostrstream (lastJudge, sizeof (lastJudge))
            << law.judge << ends ;
          lastJudge [sizeof (lastJudge) - 1] = '\0' ;
         }  // 06.feb.2002 - end
         {
          char tmp [128] ;

          citizenUCNAndUCNTypeToName (lastJudge, UCN_CITIZEN_UCN,
                                      tmp, sizeof (tmp),
                                      false, false, false) ;
          pJudgeColumn -> addText (tmp) ;
         }
        add_date_to_column (pJudgeColumn, lastJudgeDate) ;
       }
     }
   }  // -- 0 --
 }  // BankruptcyRightColumns :: MakeSessionColumns

void BankruptcyRightColumns ::
  MakeDecisionColumns (TLawsuit & law, LawsuitAllSessions &,
                       BankruptcyLawAllDecisions & decisions,
                       LawsuitAllInRegs &,
                       TGSArray <OutRegDatedContainer> &, const int)
 {
  decisions.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    column * const pAssigneeInBankruptcyColumn =
                       columns [iC_RAssigneeInBankruptcy_23] ;

    if (pAssigneeInBankruptcyColumn)
     {
      DecPtr pDec ;
      TDecision & rDec = pDec.Ref () ;

      while (decisions.NextWholeRecord (rDec))
       {
        if (pAssigneeInBankruptcyColumn)
          if (any(rDec.syndicate))
            pAssigneeInBankruptcyColumn -> addText (rDec.syndicate) ;
       }
     }
   }  // -- 0 --
 }  // BankruptcyRightColumns :: MakeDecisionColumns

void BankruptcyRightColumns ::
  MakeAppealColumns (TLawsuit & law, LawsuitAllSessions &,
                     BankruptcyLawAllDecisions & decisions,
                     LawsuitAllInRegs & inRegs,
                     TGSArray <OutRegDatedContainer> & outRegs, const int)
 {
  inRegs.Rewind () ;
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    BankruptcyLeftColumns ::
      MakeAppealColumns (law, decisions, inRegs, outRegs,
                         columns [iC_RAppealBefore_10],
                         columns [iC_RAppealedActs_20],
                         columns [iC_RAppealResults_21]) ;
   }  // -- 0 --
 }  // BankruptcyRightColumns :: MakeAppealColumns

void BankruptcyRightColumns ::
  MakeColumns (TLawsuit & law, LawsuitAllSessions & sessions,
               BankruptcyLawAllDecisions & decisions,
               LawsuitAllInRegs & inRegs,
               TGSArray <OutRegDatedContainer> & outRegs,
               TDatedContainer & lastSess,
               TDatedContainer & lastDec,
               const int serialNo)
 {
  if (TRCDKey_ (law.key).FilledOK ())
   {  // -- 0 --
    MakePureLawColumns (law, sessions, decisions, inRegs, outRegs,
                        serialNo) ;
    MakeSideColumns (law, sessions, decisions, inRegs, outRegs, serialNo) ;
    MakeSessionColumns (law, sessions, decisions, inRegs, outRegs,
                        serialNo) ;
    MakeDecisionColumns (law, sessions, decisions, inRegs, outRegs,
                         serialNo) ;
    MakeAppealColumns (law, sessions, decisions, inRegs, outRegs,
                       serialNo) ;
   }  // -- 0 --
   {
    column * pColumn ;

    if ((pColumn = columns [iC_RSerialNo]) != NULL)
     {  // -- iC_LSerialNo --
      addLongToColumn (pColumn, serialNo, true) ;
     }  // -- iC_LSerialNo --

	if ((pColumn = columns [iC_RRemark_24]) != NULL)
	{  // -- iC_RRemark_24 --
		if (DatedContainer_ (lastSess).FilledOK ())
		{  // Do not miss the check above!
			if (law.completed.Empty ())
			{
				SessPtr pSess ;
				TSession & rSess = pSess.Ref () ;

				rSess << lastSess ;

				if (rSess.Get ())
					if (rSess.result == RESULT_FIXED ||
						rSess.result == RESULT_ANNOUNCE_TO_BE_DECIDED)
					{
						if (rSess.result == RESULT_FIXED)
						{
							char tmp [128] ;

							ostrstream (tmp, sizeof (tmp))
								<< Result_ (rSess.result) << " за "
								<< rSess.date << ends ;
							tmp [sizeof (tmp) - 1] = '\0' ;
							pColumn -> addText (tmp) ;
						}

						if (rSess.result == RESULT_ANNOUNCE_TO_BE_DECIDED)
						{
							if (SessionEndDocs (rSess.key, rSess.date,
								rSess.kind).DocsFound () == 0)
							{
								char tmp [128] ;

								ostrstream (tmp, sizeof (tmp))
									<< Result_ (rSess.result) << " от "
									<< rSess.date << ends ;
								tmp [sizeof (tmp) - 1] = '\0' ;
								pColumn -> addText (tmp) ;
							}
						}
					}
			}
		}

		if (DatedContainer_ (lastDec).FilledOK ())
		{  // Do not miss the check above!
			if (law.completed.Empty () == false)
			{
				DecPtr pDec ;
				TDecision & rDec = pDec.Ref () ;

				rDec << lastDec ;
				if (rDec.Get ())
				{
					add_date_to_column (pColumn, rDec.fromDate) ;
					pColumn -> addText (rDec.text) ;
				}
			}
		}
	}  // -- iC_RRemark_24 --

   }
 }  // BankruptcyRightColumns :: MakeColumns


// ------------------- BankruptcyWideCheckupThread - begin -------------------
class TBankruptcyWideCheckupThread : public TWaitThread
{  // 14.mar.2002
	protected:
    const char * const criteria;
    BankruptcyLMROutFile & ofLeft;
    BankruptcyLMROutFile & ofLeftComplet;
    const char * const formatFNLeft;
    BankruptcyLMROutFile & ofMiddle;
    BankruptcyLMROutFile & ofMiddleComplet;
    const char * const formatFNMiddle;
    BankruptcyLMROutFile & ofRight;
    BankruptcyLMROutFile & ofRightComplet;
    const char * const formatFNRight;
    BankruptcyLMROutFile & ofHtml ;  // 23.apr.2003 - begin
    BankruptcyLMROutFile & ofHtmlComplet ;
    const char * const formatFNHtml ; // 23.apr.2003 - end
    int & serialNo;
    int & serialNoNonComplet;
    int & serialNoComplet;
    const BankruptcyWideDlgData & dlgData;
    const char* const description;

    bool ArgsOk() const;
    virtual void Execute();

public:
    TBankruptcyWideCheckupThread (const char * const crit,
					   BankruptcyLMROutFile & aOfLeft,
					   BankruptcyLMROutFile & aOfLeftComplet,
					   const char * const aFormatFNLeft,
					   BankruptcyLMROutFile & aOfMiddle,
					   BankruptcyLMROutFile & aOfMiddleComplet,
					   const char * const aFormatFNMiddle,
					   BankruptcyLMROutFile & aOfRight,
					   BankruptcyLMROutFile & aOfRightComplet,
					   const char * const aFormatFNRight,
				     BankruptcyLMROutFile & aOfHtml, //23.apr.2003-begin
				     BankruptcyLMROutFile & aOfHtmlComplet,
				     const char * const aFormatFNHtml, //23.apr.2003-end
					   int & aSerialNo,
					   int & aSerialNoNonComplet,
					   int & aSerialNoComplet,
					   const BankruptcyWideDlgData & aDlgData
					  ) ;
} ;

TBankruptcyWideCheckupThread::
  TBankruptcyWideCheckupThread (const char * const crit,
					 BankruptcyLMROutFile & aOfLeft,
					 BankruptcyLMROutFile & aOfLeftComplet,
					 const char * const aFormatFNLeft,
					 BankruptcyLMROutFile & aOfMiddle,
					 BankruptcyLMROutFile & aOfMiddleComplet,
					 const char * const aFormatFNMiddle,
					 BankruptcyLMROutFile & aOfRight,
					 BankruptcyLMROutFile & aOfRightComplet,
					 const char * const aFormatFNRight,
				     BankruptcyLMROutFile & aOfHtml, //23.apr.2003-begin
				     BankruptcyLMROutFile & aOfHtmlComplet,
				     const char * const aFormatFNHtml, //23.apr.2003-end
					 int & aSerialNo,
					 int & aSerialNoNonComplet,
					 int & aSerialNoComplet,
					 const BankruptcyWideDlgData & aDlgData
					) :
    criteria(crit),
    ofLeft(aOfLeft), ofLeftComplet(aOfLeftComplet),
    formatFNLeft(aFormatFNLeft),
    ofMiddle(aOfMiddle), ofMiddleComplet(aOfMiddleComplet),
    formatFNMiddle(aFormatFNMiddle),
    ofRight(aOfRight), ofRightComplet(aOfRightComplet),
    formatFNRight(aFormatFNRight),
    ofHtml(aOfHtml), ofHtmlComplet(aOfHtmlComplet),
    formatFNHtml(aFormatFNHtml),
    serialNo(aSerialNo), serialNoNonComplet(aSerialNoNonComplet),
    serialNoComplet(aSerialNoComplet),
    dlgData(aDlgData),
    description("Справка за делата по несъстоятелност")
{
	if(ArgsOk() == false)
    error("%s\n%s::%s", WRONG_ARGS_PASSED_TO,
	    typeid(*this).name(), typeid(*this).name());
}

bool TBankruptcyWideCheckupThread::ArgsOk() const
{
	return (criteria && criteria [0] &&
	    ofLeft && ofLeftComplet && formatFNLeft && formatFNLeft [0] &&
	    ofMiddle && ofMiddleComplet &&
	    formatFNMiddle && formatFNMiddle [0] &&
	    ofRight && ofRightComplet && formatFNRight && formatFNRight [0] &&
	    ofHtml && ofHtmlComplet && formatFNHtml && formatFNHtml [0]
	   );
}

void TBankruptcyWideCheckupThread::Execute()
{
  if(ArgsOk())
  {  // -- 0 --
	try
    {  // -- 1 --
	char msgBuff [128] ;  // Let it exist longer than the wait window
				    // (although this is not necessary).
	wtdl->SetHeader(description);
	wtdl->PleaseWait();

	{  // -- 4 --
	  TCountRecsQuery q (const_cast <char *> (criteria));
	  const unsigned recs = q.getRecsCount () ;

	  wtdl->SetProgressRange(0, recs);
	  wtdl->SetProgress(serialNo = 0);
        if(dlgData.wantNoncompleted)  // 10.jun.2003
        {
          BankruptcyWideCheckup::BeginDoc (ofLeft, dlgData) ;
          BankruptcyWideCheckup::BeginDoc (ofMiddle, dlgData) ;
          BankruptcyWideCheckup::BeginDoc (ofRight, dlgData) ;
        }
        if(dlgData.wantNoncompletedWeb)  // 10.jun.2003
          BankruptcyWideCheckup::BeginDoc (ofHtml, dlgData) ;
        if(dlgData.wantCompleted)  // 10.jun.2003
        {
          BankruptcyWideCheckup::BeginDoc (ofLeftComplet, dlgData, true) ;
          BankruptcyWideCheckup::BeginDoc (ofMiddleComplet, dlgData, true) ;
          BankruptcyWideCheckup::BeginDoc (ofRightComplet, dlgData, true) ;
        }
        if(dlgData.wantCompletedWeb)  // 10.jun.2003
          BankruptcyWideCheckup::BeginDoc (ofHtmlComplet, dlgData, true) ;
	  while (wtdl->WantBreak() == false && q.Read ())
         {  // -- 5 --
          TLawsuit law ;

          law << q ;
          serialNo ++ ;  // Do not forget this!!! If it remains 0,
                         // I will erase all docs without showing.
          if (law.Get ())
           {  // -- 6 --
             {
              char lSh [64] ;

              TRCDKey_ (law.key).PrintShortest (lSh, sizeof (lSh)) ;
              ostrstream (msgBuff, sizeof (msgBuff))
		    << lSh << " (" << serialNo << " от "
                << recs << ")"
                << ends ;
             }
            msgBuff [sizeof (msgBuff) - 1] = '\0' ;
            wtdl->SetText(msgBuff);
            if (law.completed.Empty ())
             {
		  if (dlgData.wantNoncompleted || dlgData.wantNoncompletedWeb)
               {
                if (BankruptcyWideCheckup::DateInRange (dlgData.lawBegDate,
                      dlgData.lawEndDate, law.date))
                 {
                  serialNoNonComplet ++ ;  // Do not forget this!!!
			BankruptcyWideCheckup::PrintOneLaw (wtdl->Parent, law,
			  ofLeft, formatFNLeft, ofMiddle, formatFNMiddle,
			  ofRight, formatFNRight, ofHtml, formatFNHtml,
			  serialNoNonComplet, dlgData, false) ;
		     }
		   }
		 }
		else
		  if (dlgData.wantCompleted || dlgData.wantCompletedWeb)
		    if (BankruptcyWideCheckup::
			    DateInRange (dlgData.completedBegDate,
				dlgData.completedEndDate, law.completed))
		     {
			serialNoComplet ++ ;  // Do not forget this!!!
			BankruptcyWideCheckup::PrintOneLaw (wtdl->Parent, law,
			  ofLeftComplet, formatFNLeft, ofMiddleComplet,
			  formatFNMiddle, ofRightComplet, formatFNRight,
			  ofHtmlComplet, formatFNHtml, serialNoComplet, dlgData,
			  true) ;
		     }
		wtdl->SetProgress(serialNo);
	     }  // -- 6 --
	   }  // -- 5 --
	  wtdl->AlmostDone();
	  BankruptcyWideCheckup::EndBindedDocs (ofLeft, ofMiddle, ofRight,
							    ofHtml, dlgData, false);
	  BankruptcyWideCheckup::EndBindedDocs (ofLeftComplet, ofMiddleComplet,
	    ofRightComplet, ofHtmlComplet, dlgData, true) ;
	}  // -- 4 --
		}  // -- 1 --
    CATCH_ANYTHING
  }  // -- 0 --
}

// ------------------- BankruptcyWideCheckupThread - end -------------------

// ------------------- BankruptcyWideCheckup - begin -------------------

BankruptcyWideCheckup :: BankruptcyWideCheckup (TWindow * parent) :
				   pParent (parent)
 {
 }  // BankruptcyWideCheckup :: BankruptcyWideCheckup

BankruptcyWideCheckup :: ~BankruptcyWideCheckup ()
 {
 }  // BankruptcyWideCheckup :: ~BankruptcyWideCheckup

bool BankruptcyWideCheckup ::
	 MakeFormatFNames (char * const fFNLeft, const size_t fFNLeftSz,
				 char * const fFNMiddle, const size_t fFNMiddleSz,
				 char * const fFNRight, const size_t fFNRightSz,
			 char * const fFNHtml, const size_t fFNHtmlSz  // 23.apr.2003
				  )
 {
  bool res = false ;
  const char * const mask = "text\\bankrup" ;

   {
    const int l = strlen (mask) + 1 +
                  strlen (DEFAULT_FORMAT_FILE_EXTENSION) ;

    if (fFNLeft && fFNLeftSz > l &&
        fFNMiddle && fFNMiddleSz > l &&
        fFNRight && fFNRightSz > l
       )
     {
      ostrstream (fFNLeft, fFNLeftSz)
        << mask << "l" << DEFAULT_FORMAT_FILE_EXTENSION << ends ;
      ostrstream (fFNMiddle, fFNMiddleSz)
        << mask << "m" << DEFAULT_FORMAT_FILE_EXTENSION << ends ;
      ostrstream (fFNRight, fFNRightSz)
        << mask << "r" << DEFAULT_FORMAT_FILE_EXTENSION << ends ;
      ostrstream (fFNHtml, fFNHtmlSz)
        << mask << "h" << DEFAULT_FORMAT_FILE_EXTENSION << ends ;
      fFNLeft [fFNLeftSz - 1] = fFNMiddle [fFNMiddleSz - 1] =
      fFNRight [fFNRightSz - 1] = fFNHtml [fFNHtmlSz - 1] = '\0' ;
      res = strlen (fFNLeft) == l && strlen (fFNMiddle) == l &&
            strlen (fFNRight) == l ;
     }
    if (! res)
      error ("%s\n%s::MakeFormatFNames",
             WRONG_ARGS_PASSED_TO, typeid (* this).name ()) ;
   }
  return res ;
 }  // BankruptcyWideCheckup :: MakeFormatFNames

void BankruptcyWideCheckup ::
       AdditionalCriteriaConditions (ostrstream & oss,
                                     const BankruptcyWideDlgData & dlgData)
 {  // 06.feb.2002
  if (oss)
   {  // -- 0 --
    const bool noncNeed = dlgData.wantNoncompleted ;
    const bool complNeed = dlgData.wantCompleted ;

    if (noncNeed || complNeed)
     {  // -- 1 --
      const bool orStatementNeed = noncNeed && complNeed ;

      oss << " AND "
          << "(" ;  // == 0 ==
      if (noncNeed)
       {  // -- 2 --
        oss << "(" ;  // == 1 ==
	  oss << "F_COMPLETED IS NULL" ;
        if (dlgData.lawBegDate.Empty () == false)
	    oss << " AND F_DATE >= " << dlgData.lawBegDate.Value() ;
	  if (dlgData.lawEndDate.Empty () == false)
	    oss << " AND F_DATE <= " << dlgData.lawEndDate.Value() ;
	  oss << ")" ;  // == 1 ==
	 }  // -- 2 --
	if (orStatementNeed)
	  oss << " OR " ;
	if (complNeed)
	 {  // -- 3 --
	  oss << "(" ;  // == 2 ==
	  oss << "F_COMPLETED IS NOT NULL" ;
        if (dlgData.completedBegDate.Empty () == false)
	    oss << " AND F_COMPLETED >= "
		  << dlgData.completedBegDate.Value() ;
	  if (dlgData.completedEndDate.Empty () == false)
	    oss << " AND F_COMPLETED <= "
		  << dlgData.completedEndDate.Value() ;
        oss << ")" ;  // == 2 ==
       }  // -- 3 --
      oss << ") " ;  // == 0 ==
     }  // -- 1 --
   }  // -- 0 --
 }  // BankruptcyWideCheckup :: AdditionalCriteriaConditions

bool BankruptcyWideCheckup ::
       MakeCriteria (char * const crit,const size_t critSz,
                     const BankruptcyWideDlgData & dlgData // 06.feb.2002
                    )
 {
  bool res = false ;
  char methodFullName [128] ;

  ostrstream (methodFullName, sizeof (methodFullName))
    << typeid (* this).name () << "::MakeCriteria" << ends ;
  methodFullName [sizeof (methodFullName) - 1] = '\0' ;
  if (crit && critSz)
   {  // -- 0 --
     {  // -- 1 --
      ostrstream oss (crit, critSz) ;

      oss << "SELECT F_TYPE, F_YEAR, F_NO FROM T_LAWSUIT "
		 "WHERE " ;
// ----- Keep this logic in conformity with the bankruptcy_law function
	 // 2015:027 LPR/TRQ/IRQ: rewritten
	oss << "F_TYPE IN ('" << TYPE_TRADE_LAW << "', '" << TYPE_CITIZEN_LAW << "')";
	  oss << "AND " ;
	// 2015:099 LPR/TRQ/IRQ: was KIND_INCONSISTENCE_INDOCS
	oss << "F_SOURCE_KIND IN ("<< SQLCharSet (KIND_INCONSISTENCE_REQUESTS) << ") " ;
// -----
	AdditionalCriteriaConditions (oss, dlgData) ;  // 06.feb.2002
      oss << "ORDER BY F_YEAR, F_NO, F_TYPE " ;
      oss << " ;" << ends ;
     }  // -- 1 --
    crit [critSz - 1] = '\0' ; 
    res = criteria_finished_ok (crit, methodFullName) ;
   }  // -- 0 --
  else
    error ("%s\n%s",
           WRONG_ARGS_PASSED_TO, methodFullName) ;
  return res ;
 }  // BankruptcyWideCheckup :: MakeCriteria

void BankruptcyWideCheckup ::
       SetLastEvents (LawsuitAllSessions & sessions,
                      BankruptcyLawAllDecisions & decisions,
                      TDatedContainer & lastSess,
                      TDatedContainer & lastDec,
                      const TLawsuit & law)
 {
  char lawIdStr [128] ;

  TRCDKey_ (law.key).Print (lawIdStr, sizeof (lawIdStr)) ;
  sessions.Rewind () ;
  clearGroupData (lastSess) ;
  if (law.completed.Empty ())
   {
    AloneDatedContainer_xK tmp ;
    AloneDatedContainer_xK prevTmp ;

    clearGroupData (prevTmp) ;
    while (sessions.NextKey (tmp))
     {
      prevTmp << lastSess ;
      lastSess << tmp ;
     }
    if (DatedContainer_ (lastSess).FilledOK ())
      if (lastSess.date == prevTmp.date)
       {
        clearGroupData (lastSess) ;
//        error ("%s:\n"
//               "Не може да бъде определено последното "
//               "заседание по делото (повече от едно заседание за "
//               "дата).", lawIdStr) ;
// Not need yet ?
       }
   }
  sessions.Rewind () ;

  decisions.Rewind () ;
  clearGroupData (lastDec) ;
  if (law.completed.Empty () == false)
   {
    AloneDatedContainer_xK tmp ;
    AloneDatedContainer_xK prevTmp ;

    while (decisions.NextKey (tmp))
     {
      prevTmp << lastDec ;
      lastDec << tmp ;
     }
    if (DatedContainer_ (lastDec).FilledOK () &&
        DatedContainer_ (prevTmp).FilledOK ())
      if (lastDec.date == prevTmp.date)
       {  // Analysis of fromDate fields
        DecPtr pDec ;
        TDecision & rDec = pDec.Ref () ;
	  CDate lastDecFromDate ;
        CDate prevTmpFromDate ;

        rDec << lastDec ;
        if (rDec.Get ())
         {
          lastDecFromDate = rDec.fromDate ;
          rDec << prevTmp ;
          if (rDec.Get ())
           {
            prevTmpFromDate = rDec.fromDate ;
            if (lastDecFromDate == prevTmpFromDate)
             {
		  clearGroupData (lastDec) ;
               {
                char tmpD [32] ;
                char tmpSD [32] ;

                CDateToString (rDec.fromDate, tmpD, sizeof (tmpD)) ;
                CDateToString (rDec.date, tmpSD, sizeof (tmpSD)) ;
                error ("%s:\n"
                       "Не може да бъде определен последният "
                       "краен документ по делото (повече от едно "
                       "решение/определение от %s).\n"
                       "Заседанието е от %s.",
                       lawIdStr, tmpD, tmpSD) ;
               }
             }
           }
         }
       }
   }
  decisions.Rewind () ;
 }  // BankruptcyWideCheckup :: SetLastEvents

void BankruptcyWideCheckup ::
       BeginDoc (BankruptcyLMROutFile & of,
                 const BankruptcyWideDlgData & dlgData, const bool complet)
 {
  of.title () ;
  if (complet)
    of.FormattedHeadComleted (dlgData) ;
  else
    of.FormattedHead (dlgData) ;
  of.head () ;
 }  // BankruptcyWideCheckup :: BeginDoc

void BankruptcyWideCheckup ::
  EndDoc (BankruptcyLMROutFile & of, const BankruptcyWideDlgData & dlgData)
 {
  of.FormattedTail (dlgData) ;
  of.tail () ;
 }  // BankruptcyWideCheckup :: EndDoc

void BankruptcyWideCheckup ::
       EndBindedDocs (BankruptcyLMROutFile & ofL, BankruptcyLMROutFile & ofM,
                      BankruptcyLMROutFile & ofR,
                      BankruptcyLMROutFile & ofH,  // 23.apr.2003
                      const BankruptcyWideDlgData & dlgData,
                      const bool isCompleted)
 {
  const bool processText =  // 10.jun.2003 - begin
    ((isCompleted == true && dlgData.wantCompleted == true) ||
      (isCompleted == false && dlgData.wantNoncompleted == true));
  const bool processHtml =
    ((isCompleted == true && dlgData.wantCompletedWeb == true) ||
      (isCompleted == false && dlgData.wantNoncompletedWeb == true));
  // 10.jun.2003 - end

  if(processText)
  {
    ofL.footherLine () ;
    ofM.footherLine () ;
    ofR.footherLine () ;
     {
      int frmTailLines = ofL.FormattedTail (dlgData, false) +
                         ofL.tail (false) ;
       {
        const int fTLM = ofM.FormattedTail (dlgData, false) +
                         ofM.tail (false)  ;

        if (frmTailLines < fTLM)
          frmTailLines = fTLM ;
       }
       {
	  const int fTLR = ofR.FormattedTail (dlgData, false) +
                         ofR.tail (false)  ;

        if (frmTailLines < fTLR)
          frmTailLines = fTLR ;
       }
      ofL.NextLawToPrint (frmTailLines + 1, false) ;
      ofM.NextLawToPrint (frmTailLines + 1, false) ;
      ofR.NextLawToPrint (frmTailLines + 1, false) ;
     }
    EndDoc (ofL, dlgData) ;
    EndDoc (ofM, dlgData) ;
    EndDoc (ofR, dlgData) ;
  }
  if(processHtml)
    EndDoc (ofH, dlgData) ;
 }  // BankruptcyWideCheckup :: EndBindedDocs

void BankruptcyWideCheckup :: PrtOneLaw (const int rows,
                                         BankruptcyLMROutFile & of,
                                         ColumnsArrIncMaxRows & bC)
 {
  bC.IncMaxRowsIfNecessary (rows) ;
  of.NextLawToPrint (rows + 1) ;
  of.horzLine () ;
  for (int i = 0 ; i < rows ; i ++)
    of.WriteItem (bC, true) ;
 }  // BankruptcyWideCheckup :: PrtOneLaw

void BankruptcyWideCheckup ::
       PrintOneLaw (TWindow * const pParent,  // 14.mar.2002
                    TLawsuit & law,
                    BankruptcyLMROutFile & ofLeft,
                    const char * const formatFNLeft,
                    BankruptcyLMROutFile & ofMiddle,
                    const char * const formatFNMiddle,
                    BankruptcyLMROutFile & ofRight,
                    const char * const formatFNRight,
                    BankruptcyLMROutFile & ofHtml,  // 23.apr.2003
                    const char * const formatFNHtml,  // 23.apr.2003
                    const int serialNo,
                    const BankruptcyWideDlgData & dlgData,
                    const bool isCompleted  // 10.jun.2003
                    )
 {
  if (formatFNLeft && formatFNLeft [0] &&
	formatFNMiddle && formatFNMiddle [0] &&
      formatFNRight && formatFNRight [0] &&
      formatFNHtml && formatFNHtml [0])
   {  // -- 0 --
    LawsuitAllSessions sessions (law.key) ;
    BankruptcyLawAllDecisions decisions (law.key) ;
    LawsuitAllInRegs inregs (law.key) ;
    TGSArray <OutRegDatedContainer> outregs ;
    AloneDatedContainer_xK lastSess ;
    AloneDatedContainer_xK lastDec ;
    const bool processText =  // 10.jun.2003 - begin
      ((isCompleted == true && dlgData.wantCompleted == true) ||
        (isCompleted == false && dlgData.wantNoncompleted == true));
    const bool processHtml =
      ((isCompleted == true && dlgData.wantCompletedWeb == true) ||
        (isCompleted == false && dlgData.wantNoncompletedWeb == true));
    // 10.jun.2003 - end
    SetLastEvents (sessions, decisions, lastSess, lastDec, law) ;
    inregs_to_all_outregs (inregs, outregs) ;
    if(processText)
     {  // -- 1 --
      BankruptcyLeftColumns bLeftC (formatFNLeft, pParent, law,
                                    sessions, decisions, inregs,
                                    outregs, lastSess, lastDec,
                                    serialNo, ofLeft.MinRowsForLaw ()) ;
      BankruptcyMiddleColumns bMiddleC (formatFNMiddle, pParent, law,
                                        sessions, decisions, inregs,
                                        outregs, lastSess, lastDec,
                                        serialNo,
                                        ofMiddle.MinRowsForLaw ()) ;
      BankruptcyRightColumns bRightC (formatFNRight, pParent, law,
                                      sessions, decisions, inregs,
                                      outregs, lastSess, lastDec,
                                      serialNo, ofRight.MinRowsForLaw ()) ;
      int rows = bLeftC.getMaxRows () ;

      if (rows < bMiddleC.getMaxRows ())
        rows = bMiddleC.getMaxRows () ;
      if (rows < bRightC.getMaxRows ())
        rows = bRightC.getMaxRows () ;
      PrtOneLaw (rows, ofLeft, bLeftC) ;
      PrtOneLaw (rows, ofMiddle, bMiddleC) ;
      PrtOneLaw (rows, ofRight, bRightC) ;
     }  // -- 1 --
     if(processHtml)
     {
	BankruptcyLeftColumns bHtmlC (formatFNHtml,  // 23.apr.2003 - begin
                                    pParent, law,
                                    sessions, decisions, inregs,
                                    outregs, lastSess, lastDec,
                                    serialNo,
                                    ofLeft.MinRowsForLaw()); // 23.apr.2003-end

       {  // 23.apr.2003
			  BancruptcyHTMLWriter wrt(ofHtml);

	  wrt.WriteToTableRow(bHtmlC, OFFSET_BANKRUPTCY_LEFT,
          law.completed.Empty() ?
            TABLE_ROW_SECTION : "***_TABLE_ROW_COMPLETED");
       }
     }
   }  // -- 0 --
 }  // BankruptcyWideCheckup :: PrintOneLaw

bool BankruptcyWideCheckup ::
       DateInRange (const CDate lo, const CDate hi,
                    const CDate test)
 {
  const bool loEmpty = lo.Empty () ;
  const bool hiEmpty = hi.Empty () ;
  bool res ;

  if (loEmpty == true && hiEmpty == false)
    res = test <= hi ;
  else
    if (loEmpty == false && hiEmpty == true)
      res = lo <= test ;
    else
      if (loEmpty == false && hiEmpty == false)
        res = lo <= test && test <= hi ;
      else
        res = true ;  
  return res ;
 }  // BankruptcyWideCheckup :: DateInRange

void BankruptcyWideCheckup ::
	 PrintLaws (const char * const criteria,
			BankruptcyLMROutFile & ofLeft,
                  BankruptcyLMROutFile & ofLeftComplet,
			const char * const formatFNLeft,
                  BankruptcyLMROutFile & ofMiddle,
			BankruptcyLMROutFile & ofMiddleComplet,
                  const char * const formatFNMiddle,
                  BankruptcyLMROutFile & ofRight,
                  BankruptcyLMROutFile & ofRightComplet,
                  const char * const formatFNRight,
                  BankruptcyLMROutFile & ofHtml,  // 23.apr.2003 - begin
                  BankruptcyLMROutFile & ofHtmlComplet,
                  const char * const formatFNHtml,  // 23.apr.2003 - end
			int & serialNo,
                  int & serialNoNonComplet,
                  int & serialNoComplet,
                  const BankruptcyWideDlgData & dlgData
                 )
 {  // 14.mar.2002
  if (criteria && criteria [0] &&
      ofLeft && ofLeftComplet && formatFNLeft && formatFNLeft [0] &&
      ofMiddle && ofMiddleComplet &&
      formatFNMiddle && formatFNMiddle [0] &&
      ofRight && ofRightComplet && formatFNRight && formatFNRight [0]
     )
   {  // -- 0 --
     {  // -- 1 --
	try
	{
		TBankruptcyWideCheckupThread(criteria,
		    ofLeft, ofLeftComplet, formatFNLeft,
		    ofMiddle, ofMiddleComplet, formatFNMiddle,
		    ofRight, ofRightComplet, formatFNRight,
		    ofHtml, ofHtmlComplet, formatFNHtml,
		    serialNo, serialNoNonComplet, serialNoComplet,
		    dlgData).ExecWait();
	}
	CATCH_ANYTHING
     }  // -- 1 --
   }  // -- 0 --
  else
    error ("%s\n%s::PrintLaws",
	     WRONG_ARGS_PASSED_TO, "BankruptcyWideCheckup") ;
 }  // BankruptcyWideCheckup :: PrintLaws

void BankruptcyWideCheckup ::
	 PrintBankruptcyWideCheckup (const BankruptcyWideDlgData & dlgData)
 {  // An old variant - in VersionArchives\1232\_appname.j --> _bankrup.cpp
  char formatFNLeft [128] ;
  char formatFNMiddle [128] ;
  char formatFNRight [128] ;
  char formatFNHtml [128] ;  // 23.apr.2003

  if (MakeFormatFNames (formatFNLeft, sizeof (formatFNLeft),
				formatFNMiddle, sizeof (formatFNMiddle),
				formatFNRight, sizeof (formatFNRight),
				formatFNHtml, sizeof (formatFNHtml)))
   {  // -- 0 --
    int serialNo = 0 ;
    int serialNoNonComplet = 0 ;
    int serialNoComplet = 0 ;

	BankruptcyLMROutFile ofLeft (pParent, formatFNLeft) ;
	BankruptcyLMROutFile ofMiddle (pParent, formatFNMiddle) ;
	BankruptcyLMROutFile ofRight (pParent, formatFNRight) ;
	BankruptcyLMROutFile ofHtml(pParent, formatFNHtml);
	BankruptcyLMROutFile ofLeftComplet (pParent, formatFNLeft) ;
	BankruptcyLMROutFile ofMiddleComplet (pParent, formatFNMiddle) ;
	BankruptcyLMROutFile ofRightComplet (pParent,formatFNRight) ;
	BankruptcyLMROutFile ofHtmlComplet (pParent, formatFNHtml);

	 {  // -- 2 --
	  CharPtr criteria (1024) ;

	  if (MakeCriteria (criteria, criteria.Size (), dlgData))
	   {  // -- 3 --
	    TWaitThreadDialog wtdl;
	    PrintLaws (criteria,
			   ofLeft, ofLeftComplet, formatFNLeft,
			   ofMiddle, ofMiddleComplet, formatFNMiddle,
			   ofRight, ofRightComplet, formatFNRight,
			   ofHtml, ofHtmlComplet, formatFNHtml,
			   serialNo, serialNoNonComplet, serialNoComplet,
			   dlgData) ;

	   }  // -- 3 --
	 }  // -- 2 --

     {
	const char * const msgFormat =
	  "Липсват %s%sдела по несъстоятелност%s." ;
	const char * const nonComplStr = "неприключили " ;  // The length
	  // of this string must be at least 2 !!!

	if (serialNo == 0)
	 {
	  info (msgFormat, "", "", " със зададените характеристики") ;
	 }
	else
	 {
	  if (serialNoNonComplet == 0)
	   {
	    if (dlgData.wantNoncompleted)
		info (msgFormat, nonComplStr, "",
			", образувани през зададения период") ;
	   }
	  if (serialNoComplet == 0)
	   {
	    if (dlgData.wantCompleted)
		info (msgFormat, nonComplStr + 2,
			"през зададения период ", "") ;
	   }
	 }
     }

    if(dlgData.wantNoncompleted)
    {
	if (serialNo && serialNoNonComplet)
	{
	  show_text_stream_use_settings (NULL, ofLeft, formatFNLeft, LEFT_ALIGNMENT) ;
	  show_text_stream_use_settings (NULL, ofMiddle, formatFNMiddle, LEFT_ALIGNMENT) ;
	  show_text_stream_use_settings (NULL, ofRight, formatFNRight, LEFT_ALIGNMENT) ;
	}
    }

    if(dlgData.wantNoncompletedWeb)
    {
	if (serialNo && serialNoNonComplet)
		show_html_stream(NULL, dlgData.noncompletedWebPath, "хипертекстов", ofHtml);
	else
		remove_file(dlgData.noncompletedWebPath, "хипертекстов", -1);
    }

    if (serialNoNonComplet && serialNoComplet)
	message ("Справка за приключили дела по несъстоятелност") ;

    if(dlgData.wantCompleted)
    {
	if (serialNo && serialNoComplet)
	{
	  show_text_stream_use_settings (NULL, ofLeftComplet, formatFNLeft, LEFT_ALIGNMENT) ;
	  show_text_stream_use_settings (NULL, ofMiddleComplet, formatFNMiddle, LEFT_ALIGNMENT) ;
	  show_text_stream_use_settings (NULL, ofRightComplet, formatFNRight, LEFT_ALIGNMENT) ;
	}
    }

    if(dlgData.wantCompletedWeb)
    {
	if (serialNo && serialNoComplet)
		show_html_stream(NULL, dlgData.completedWebPath, "хипертекстов", ofHtmlComplet);
	else
		remove_file(dlgData.completedWebPath, "хипертекстов", -1);
    }
   }  // -- 0 --
 }  // BankruptcyWideCheckup :: PrintBankruptcyWideCheckup

// ---------------------------- Implementation ----------------------------
// ---------------------- BankruptcyWideCheckup - end ----------------------

void print_bankruptcy_wide_checkup(TWindow *parent, bool allowWeb)
{
	BankruptcyWideDlgData dlgData;

	if (allowWeb)
	{
		build_fn(OUT_PATH, "BANKRUPT.htm", dlgData.noncompletedWebPath);
		build_fn(OUT_PATH, "BANKRFIN.htm", dlgData.completedWebPath);

		if (TPrintBankruptcyWideWebDialog(parent, dlgData).Execute() == IDOK)
		{
			if (dlgData.wantNoncompleted || dlgData.wantCompleted || dlgData.wantNoncompletedWeb ||
				dlgData.wantCompletedWeb)
			{
				register_usage(REGIME_WEB_BANKRUPTCY_CHECKUP, 1, "", "С WEB Несъст");
				BankruptcyWideCheckup(parent).PrintBankruptcyWideCheckup(dlgData);
			}
		}
	}
	else
	{
		if (TPrintBankruptcyWideDialog (parent, dlgData).Execute () == IDOK)
		{
			if (dlgData.wantNoncompleted || dlgData.wantCompleted)
			{
				register_usage(REGIME_BANKRUPTCY_WIDE_CHECKUP, 1, "", "С Несъст");
				BankruptcyWideCheckup(parent).PrintBankruptcyWideCheckup(dlgData);
			}
		}
	}
}

#endif  // AREAL
