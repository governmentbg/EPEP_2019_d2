void printAnyDescBook (TWindow * pParent, const char college,
                       begNEndNYear bneny,
                       const char * formatFName,
			     unsigned & maxLawNo,
                       subjectHeapArray & sHA, kindHeapArray & kHA,
                       subjectRangeHeapArray & sRHA,
                       mergedKindsHeapArray & mKHA,
# if INSTANCE
                       orderedDamnTypeHeapArray & dTHA,
# endif
                       unsigned & lawsCnt
                      ) ;
void printDescBkSummary (TWindow * pParent, const char college,
                         const begNEndNYear & bneny,
                         const char * formatFName,
                         const unsigned maxLawNo,
                         const subjectHeapArray & sHA,
                         const kindHeapArray & kHA,
                         const subjectRangeHeapArray & sRHA,
                         const mergedKindsHeapArray & mKHA,
# if INSTANCE
                         orderedDamnTypeHeapArray & dTHA,
# endif
                         const unsigned lawsCnt) ;
void print_desk_bk_summary (TWindow * pParent, const char college,
                            const begNEndNYear & bneny,
                            const char * formatFName,
                            const unsigned maxLawNo,
                            const unsigned lawsCnt) ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
//void print_punishment_desk_bk_summary (TWindow * pParent,  // 22.oct.2001
void print_code_value_desc_bk_summary (TWindow * pParent,  // 22.oct.2001
                                       const char college,
                                       const begNEndNYear & bneny,
                                       const char * formatFName,
                                       const unsigned maxLawNo,
                                       const unsigned lawsCnt) ;
#   endif  // of APPEAL
# endif  // of INSTANCE
void printAnyAbcBook (TWindow * pParent, const char college,
                      begNLEndNLYear bnlenly,
                      const char * formatFName) ;
//void printAnySessList (TWindow * pParent, const clgDate & rClgDate,
//    const char * formatFName) ;  // 26.may.2003 (close public symbols)
void print_closed_book (TWindow * pParent) ;
void printDecisionOrJudgement (TWindow * pParent,
//                               const  // 09.jun.2000
                                 TDatedContainer * pContainer,
                               TLawsuit * pLawsuit,
                               const CDate * dateReturned = NULL) ;
void print_decision_judgement (TWindow * pParent,
//                               const  // 09.jun.2000
                               TDatedContainer * pContainer,
                               TLawsuit * pLawsuit,
                               const CDate dateReturned) ;
void print_decision_judgement (TWindow * pParent,
//                               const  // 09.jun.2000
                               TDatedContainer * pContainer,
                               TLawsuit * pLawsuit) ;
void print_judge_checkup (TWindow * parent) ;
void query_activity (TWindow * pParent) ;
void print_appealed_input_diary (TWindow * parent) ;
void print_one_inreg (TWindow * parent,
                      TInReg * const pInReg,
                      TInRegEvent * const pInRegEvent,
                      TOutReg * const pOutReg  // 02.oct.2000
                     ) ;

#if COURT_TYPE == COURT_AREAL
// 2012:317 LPR: INSTANCE && !APPEAL -> AREAL +related
void print_634_trade_book (TWindow * parent) ;
# endif

void print_undo_result (TWindow * pParent,
                        const TDatedContainer * const pContainer,
                        TLawsuit * const pLawsuit) ;


void print_end_docs_and_surroundments (TWindow * pParent, TSession * pSess,
                         const TGArray <TSurroundment> * pSurroundments) ;

#if COURT_TYPE == COURT_AREAL
void print_bankruptcy_wide_checkup (TWindow * parent, const bool allowWeb);
#endif  // AREAL

void show_distributor_history(TWindow* const pParent,
  bool withoutDialog = true);
void print_penalty_book(TWindow* const pParent);
void print_single_penalty(TWindow* const pParent,  // const  11.oct.2004
  TExpense* pExpense);
void print_mail_list(TWindow* const pParent);
