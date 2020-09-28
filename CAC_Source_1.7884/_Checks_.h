enum FinishedLawErrors
 {
  FLE_NoFinalizedSession,
  FLE_TooManyFinalizedSessions,
  FLE_FinishedBeforeTheCreating,
  FLE_NoFinalizingJudge,
  FLE_BadFinalizedOpenSessionDate,
  FLE_BadFinalizedClSessionDate,
  FLE_Firm_DoubleRegistered,  // 04.jan.2001
  FLE_Firm_TooOldRegisteredSoon,  // 04.jan.2001  // for Varna
  FLE_Firm_FakedDateFinished,  // 04.jan.2001
  FLE_Firm_LawDoubleFinished,  // 09.apr.2001
  FLE_CanceledAndGranted,  // 30.jan.2002
  FLE_FinishedAndAgeContradiction,  // 30.jan.2002
  FLE_BadFinalizedClSessionResult,  // 13.jul.2004
  FLE_NotCanceledAndNotGranted,  // 2008:282 LRQ/LPR: +related

  FLE_UndefinedError
 } ;  // enum FinishedLawErrors

enum SessionErrors
 {
  SE_WrongMembership1,
  SE_WrongMembership2,
  SE_WrongMembership3,
  SE_BadFixConcerningNextSess,
  SE_BadFixConcerningLawDate,
  SE_DifferentCompositions,

  SE_UndefinedError
 } ;  // enum SessionErrors

bool citizenIsEmpty (const TCitizen & c) ;
int compare_side_winds(const TSideWind& sw0, const TSideWind& sw1);
bool countThisSession (const CDate sessDate,
                       const CDate lawsuitFinished,
                       const bool emptyFinishedAllowed = false) ;
bool countThisSession (const TSession & sess, const TLawsuit & lawsuit,
                       const bool emptyFinishedAllowed = false) ;
bool decision_by_surround (const TSurround * pSurround,
				   const TDecision * pDec) ;
# if INSTANCE
bool enterDateNeed (const collBegDEndDBegN & cBegDEndDBegN) ;
# endif
//bool fastProcedureLaw (const TRCDKey & sourceKey) ;  // 24.sep.2003
bool inreg_by_session (const TSession * pSess, const TInReg * pInReg) ;
bool inreg_by_surround (const TSurround * pSurround,
                        const TInReg * pInReg,
                        const bool includingNoSpecification) ;
bool law_is_returned_to_up_look_into (const TLawsuit & law) ;
# if INSTANCE
bool lawIsCocern3rdInst(const char lawKind);
inline bool lawIsCocern3rdInst(const TLawsuit & rLawsuit) { return lawIsCocern3rdInst(rLawsuit.kind); }
# endif
bool lawIsCreatedByInDoc (const TLawsuit & lawsuit) ;
bool lawIsReceivedByJurisd (const TLawsuit & lawsuit) ;
bool lawIsReturnedFrom2ndInstance (const TLawsuit & lawsuit) ;

bool overXMonthsLater (const CDate & d1, const CDate & d2,
                       unsigned int differ) ;
  // returns true if d2 is over <differ> months later than d1
  // Not used yet. Definition is "commented"
bool over3MonthsLater (const CDate & d1, const CDate & d2) ;
  // returns true if d2 is over 3 months later than d1
  // Not used yet. Definition is "commented"
bool outrg_retLawToLowerInst_weakCond(const TOutReg& outReg,
	const bool checkWalns);
	// returns true if outReg fits to an indistinct criteria.
bool resMustBeTreatAsDecision (const char result) ;
	// returns true, when ... See the implementation (_checks.cpp)
bool sessionCancelsTheLaw (const TSession & sess,
                           const TLawsuit * const pLaw,  // 04.aug.2000
                           const bool accordingToSessDate,  // 26.jul.2000
                           const TDecision * const pDec = NULL,
                           const bool thereIsNotDecision = false) ;
bool sessionDecidesTheLaw (const TSession & sess,
                           const TLawsuit * const pLaw,  // 04.aug.2000
                           const bool accordingToSessDate,  // 26.jul.2000
                           const TDecision * const pDec = NULL,
                           const bool thereIsNotDecision = false) ;
bool sessResOverridden_AbolishedMove (const TSession & sess,
                                      CDate & abolishingDate,
                     CDate * const pDateReturned = NULL  // 11.sep.2000
                                     ) ;
bool sessResOverridden_AbolishProgr (const TSession & s, TQuery * pQ,
                                CDate * const pAbolishingDate = NULL,
                     CDate * const pDateReturned = NULL,  // 11.sep.2000
                     TSession * pAbolishingSess = NULL  // 13.dec.2001
                                    ) ;
bool session_by_surround (const TSurround * pSurround, TSession * pSess) ;
bool session_by_surrounds (TSession * pSess) ;
bool string_is_upper (const char * const str) ;
bool strIsBlank (const char * s) ;
inline bool strIsBlank (const mstr &m) { return strIsBlank(str(m)); }
  // returns true if s is empty string or s contains only spaces
bool strIsBlankOr160 (const char * s) ;
inline bool strIsBlankOr160 (mstr &m) { return strIsBlankOr160(str(m)); }
  // returns true if s is empty string or s contains only spaces

bool surr_kind_and_waln_state_match (const char surroundKind,
                                     const char walnState) ;
bool surr_ment_res_and_act_kind_match (const char surroundmentResult,
                                       const char actKind) ;
bool timeInRange (const CTime & noon, const WhatPartOfDay part,
			const CTime time) ;
bool ucnPresent (const char * const ucn) ;
bool ucn_distributed (const char * const ucn) ;  // 16.mar.2003
bool waln_or_link_notified (const char status, const char notifyAddress) ;
bool bankruptcy_law (const TLawsuit & law) ;
#define use_area_compositions() PRINT_COMPOSITIONS
const char * non_finished_firm_law_patch (char * const buffer,
							const size_t bufferSize,
							const char * const lawKinds,
                                          const CDate * const pEndD) ;
// returns buffer, that contains additional conditons.
//
// if (strlen (intersection (lawKinds, KIND_FIRM_LAWS)) == 0)
//   buffer [0] = '\0' ;
//
// this patch may be inserted before "ORDER BY".
//
// pEndP may be null.
// (* pEndP) may be empty.
// In these cases the period is unlimited.
//
// The (* pEndP) day is NOT included in the period.
//
bool law_is_pre_court (const TLawsuit & rLawsuit) ;
bool law_is_237_enactment (const TLawsuit & rLawsuit) ;

inline bool is_template_kind(const char *kinds, const char k) { return k ? strchr(kinds, k) : false; }
inline bool is_surround_kind (const char k) { return is_template_kind(KIND_SURROUNDS, k); }
inline bool is_session_kind (const char k) { return is_template_kind(KIND_SESSIONS, k); }
inline bool is_decision_kind (const char k) { return is_template_kind(KIND_ENDOCS, k); }
inline bool is_inreg_kind (const char k) { return is_template_kind(KIND_IN_REG_DOCS, k); }
inline bool is_outreg_kind (const char k) { return is_template_kind(KIND_OUT_REG_DOCS, k); }

bool check_finished_lawsuits(const char *const types, const CDate &begDate, const CDate &endDate, TWindow *parent,
	const char * const kinds = NULL, bool *pCheckTerminated = NULL, bool *pInconsistencyFound = NULL);

bool bad_finished_law (const TLawsuit & law,
                       const FinishedLawErrors whichError,
                       outText * pOfs,
			     const bool errMsg = true  // 04.jan.2001
			     ) ;
bool confirm_bad_finished_laws () ;
bool criteria_finished_ok (const char * const criteria,
				   const char * const comment) ;  // 18.jan.2001
bool criteria_finished_ok(const char* const criteria,
	const char* const className, const char* const methodName);//14.aug.2002
bool judges_membership_ok (const char lawKind,
				   const BriefSessionsToCheckGroup & sess,
				   const bool closedIsAlwaysOK) ;  // 29.jan.2001
bool judges_membership_ok_spk (const char lawKind,
					 const BriefSessionsToCheckGroup & sess,
					 const bool closedIsAlwaysOK,
					 outText * pOfs,
					 const bool errMsg = true
					) ;  // 29.jan.2001
bool bad_session (const BriefSessionsToCheckGroup & sess,
		   const SessionErrors whichError,
		   outText * pOfs, const bool errMsg = true) ;
bool check_sessions (const char *const types, const CDate &begDate, const CDate &endDate, TWindow *parent,
	const long composition = 0, const char *const lawKinds = NULL, const char *const sessKinds = NULL,
	const bool excludeResFixed = false, const bool closedMembershipIsAlwaysOK = false, bool *pCheckTerminated = NULL,
	bool *pInconsistencyFound = NULL);
bool reminding_notice(const TSideWind& w, CDate* const pFirstDate = NULL,
  const bool errMsg = false);
bool thereIsNextSession (const TSession & currSess, TSession & nextSess,
				 const char * sessKinds = NULL,
                         bool sessKindsMatch = false) ;
bool thisIsFirstSession (const TSession & sess) ;
bool valid_surroundment (const TSurroundment & ment,
				 TSurround * const pSurr = NULL,
                         TSession * const pSess = NULL,
                         const bool sessIsOK = false) ;

bool is_default_post_admin();

// quick -->
// without conversion
bool quick_pure_quick(const long quick);      // -
bool quick_pure_standard(const long quick);   // -
bool quick_pure_immediate(const long quick);  // - 20060324 new

// with conwersion
// from standard to ...
bool quick_formerly_standard_to_quick (const long quick);  //new 20060324
bool quick_formerly_standard_to_immediate (const long quick);  //new 20060324
// from  ... to standard
bool quick_formerly_quick_to_standard (const long quick);  //new 20060324
bool quick_formerly_immediate_to_standard (const long quick);  //new 20060324

// currently
bool quick_currently_standard(const long quick);
bool quick_currently_quick(const long quick);
bool quick_currently_immediate (const long quick);

bool quick_law_source_currently_quick(const TRCDKey source);
bool quick_law_source_currently_quick(const TRCDKey source, TRequest& tmpReq);
// quick <--

char side_wind_notified_mark(TSideWind *sideWind, char eventStatus, mstr &m);
char side_wind_notified_mark(TSideWind *sideWind, char eventStatus);
bool sess_result_appealed(DatedContainer_ sessKey,
  TIArray<DatedContainer_>* pInRegKeys);  // 31.jan.2005
bool default_split_wide_hyper_docs();  // 08.feb.2005
