// -------------------------- Convert -----------------------------------
void lawTypeToPlural (ostream & os, const char t,
                      const bool end1 = true,
			    const char * mediumWord = NULL) ;
void types_ToPlural (ostream & os, const char * const types,
                     const bool end1 = true) ;
void kinds_to_plural (ostream & os, const char * const kinds,
                      const bool end1) ;
bool surround_kind_to_waln_state (const char surrouindKind,
                                  char & walnState) ;
// Leaves walnState unchanged when rerurns false.

bool surroundment_key_data (TSurroundment & ment,
                            const TRCDKey surroundKey,
                            const char surroundKind,
                            const TRCDKey sessionKey,
                            const CDate sessionDate,
                            const char sessionKind) ;
bool uclpCodeToTUCLP (long code, TUCLP & Uclp) ;
bool TRCDKeyToLawsuit (TRCDKey key, TLawsuit & lawsuit) ;
bool ucnAndUcnTypeToAddress (const char * ucn, char ucnType,
                             TAddress * pAddress) ;
bool ucnAndUcnTypeToFrgnAddr (const char * ucn, char ucnType,
					TForeignAddress * pAddress) ;
// 2009:154 LPR: removed
# if INSTANCE
#   if APPEAL    // APPEAL
char sourceTypeToSenderUCNType (const TLawsuit & rLawsuit,
					  const char sourceKind) ;
#   else         // AREA
char sourceTypeToSenderUCNType (const TLawsuit & rLawsuit,
					  const char lawKind) ;
#   endif
# else           // REGIONAL
char sourceTypeToSenderUCNType (const TLawsuit & rLawsuit) ;
# endif
// 20070109 <--

# if INSTANCE
bool lawsuitToDamn (TLawsuit * pLawsuit, TDamn * pDamn) ;
# endif  // of INSTANCE
void link_wind_to_link_winds(TLinkWind& link, TGArray<TLinkWind>& links);
void container_to_link_winds(TRCDKeyContainer& container,
  TGArray<TLinkWind>& links);  // 22.dec.2004
void dated_container_to_link_winds(TDatedContainer& container,
  TGArray<TLinkWind>& links);
bool inreg_to_link_winds(TInReg* pInReg, TGArray<TLinkWind>& links,
  const bool errMsg = true);
TSideWind* dated_container_to_side_wind(TDatedContainer& dContainer);
void container_to_side_winds(TRCDKeyContainer& dContainer,
  TGArray<TSideWind>& dSides);  // 22.dec.2004
void dated_container_to_side_winds(TDatedContainer& dContainer,
  TGArray<TSideWind>& dSides);
char datedContainerToSubpoenaEvStatus(TDatedContainer& dContainer);
# if INSTANCE
bool lawsuitToDamnBoss (TLawsuit * pLawsuit,
				TSideWind * pSide, TCitizen * pCitizen) ;
  // it is possible to pass NULL instead of pSide or pCitizen
  // (but only one NULL)
# endif  // of INSTANCE
bool no_year_to_outreg (const long int no, const long int year,
				TOutReg & rOutReg) ;
bool lower_regional_boza_short_name (const TSender & sender,
						 char * const dest,
						 const size_t destSz) ; // 27.mar.2001
// 20070114 in comment - bool lawsuit_to_sender (const TLawsuit & law, TSender & s) ; //27.mar.2001
bool request_to_sender (const TRequest& rRequest, TSender& s);  // 21.dec.2001
bool kinds_to_types (const char * const kinds, char * const types,
                     const size_t tSz) ;  // 27.mar.2001
bool user_ucn_at_unique_post (char * const dest, const size_t destSz,
                              const char post) ;  // 05.apr.2001
bool law_types_to_law_kinds (const char * const types,
                             char * const kinds,
			     const size_t kindsSz) ;  // 10.apr.2001
void side_to_sides (TSideWind & side, TGArray <TSideWind> & sides) ;
bool inreg_to_any_appealer (const TInReg & inReg, TSideWind & dest) ;
char inreg_to_law_type (const TInReg & inReg,
				TRCDKey * const pLawKey = NULL,  // 25.apr.2001
				TDatedContainer * pSessKeyData = NULL,//03.oct.2001
			    TRCDKeyContainer_ * pSurrKeyData = NULL,//03.oct.2001
			const bool errMsg = true,   // 30.oct.2001
			TRCDKey * pReqKey = NULL  // 11.jun.2004
			 ) ;
// Returns the associated lawsuit type. On error returns '\0'.
// If there is an associated lawsuit, the (* pLawKey) is filled.
// If there is an associated session, the (* pSessKeyData) is filled.
// If there is an associated surround, the (* pSurrKeyData) is filled.
// If there is an associated request, the (* pReqKey) is filled.
char inreg_to_law_type (TInReg & inReg,
			TRCDKey * const pLawKey = NULL,
			TDatedContainer * pSessKeyData = NULL,
			TRCDKeyContainer_ * pSurrKeyData = NULL,
			const bool errMsg = true,
			TRCDKey * pReqKey = NULL  // 11.jun.2004
			     ) ;  // 30.jul.2002
// Returns the associated lawsuit type. On error returns '\0'.
// If there is an associated lawsuit, the (* pLawKey) is filled.
// If there is an associated session, the (* pSessKeyData) is filled.
// If there is an associated surround, the (* pSurrKeyData) is filled.
// If there is an associated request, the (* pReqKey) is filled.

char outreg_to_law_type(TOutReg& outReg, TRCDKey* const pLawKey = NULL,
  TDatedContainer* pSessKeyData = NULL, TRCDKeyContainer_* pSurrKeyData = NULL,
  TRCDKey* pReqKey = NULL, const bool errMsg = true);  // 28.jul.2003
// Returns the associated lawsuit type. On error returns '\0'.
// If there is an associated lawsuit, the (*pLawKey) is filled.
// If there is an associated session, the (*pSessKeyData) is filled.
// If there is an associated surround, the (*pSurrKeyData) is filled.
// If there is an associated request, the (*pReqKey) is filled.

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
// These "# if" conditional directives are included to avoid
// linker errors (possible because the function is not used
// int the regional and appeal versions).
void inregs_to_all_outregs (LawsuitAllInRegs & inregs,
		  TGSArray <OutRegDatedContainer> & outregs) ;  // 11.jan.2001
#   endif  // of APPEAL
# endif  // of INSTANCE

bool surround_to_surroundments(TSurround& surr,
		TGArray<TSurroundment>& ments);  // 14.aug.2002
bool surround_to_brief_sessions(TSurround& surr,
		TGSArray<BriefSessionData>& briefSessions);  // 14.aug.2002
bool surroundment_to_surround (const TSurroundment & ment,
					 TSurround & surround) ;
bool surroundment_to_session (const TSurroundment & ment,
					 TSession & sess) ;
bool session_to_surrounds (TSession & sess,
				   TGArray<TSurround> & surrounds) ;
char request_doc_to_law_type (const TRequest &request);
  // Returns '\0' if unsuccessful
bool trunc_BG_string(const char* const src, char* const dest,
		const size_t destSz, const size_t maxLen,
		const size_t dotAlowedFromPosition = 2);  // 14.aug.2002

char upCyr (char a) ;
char * up_cyr_str (char * const str) ;
char loCyr (char a) ;  // 27.mar.2001
char * lo_cyr_str (char * const str) ;  // 27.mar.2001
char * mixed_case_cyr_str (char * const str) ;  // 24.jan.2004
char * mixed_case_cyr_str (char * const str, const bool up2Lo) ;  // 10.feb.2004
