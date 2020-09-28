void putCompositionToOutStream (const long int composition, ostream & os,
                                char * const s, const size_t len,
                                const size_t algn, const size_t skip) ;

// ------------------------- Outputs -----------------------------------
// All output functions below return true if dCiph is known (perceived)
bool addressOutput (const TAddress & a, ostream & os, char * s,
                    const size_t len, const int algn,
                    const size_t skip, int dCiph) ;
# if INSTANCE
# if APPEAL
# else  // of APPEAL
bool BankruptcyWideDlgData_output (const BankruptcyWideDlgData & dlgData,
					     ostream & os, char * s,
                                   const size_t len, const int algn,
                                   const size_t skip, int dCiph) ;
# endif  // of APPEAL
# endif  // of INSTANCE
bool begMYEndMYTypeCIUOutput (const begMYEndMYTypeCIU
                               & rBMYEndMYTypeCIUO,
                              ostream & os, char * s, const size_t len,
                              const int algn, const size_t, int dCiph) ;
bool begNEndNYearOutput (const begNEndNYear & bneny,
                         ostream & os, char * s,
                         const size_t len, const int algn,
                         const size_t skip, int dCiph) ;
bool begNEndNYearTypesOutput (const begNEndNYearTypes & bnenyT,
                              ostream & os, char * s,
                              const size_t len, const int algn,
                              const size_t skip, int dCiph) ;
bool begNEndNYearTypesACOutput (const begNEndNYearTypesAC & bneny,
                         ostream & os, char * s,
                         const size_t len, const int algn,
				 const size_t skip, int dCiph) ;
bool citizenOutput (const TCitizen & firm, ostream & os, char * s,
                    const size_t len, const int algn,
                    const size_t skip, int dCiph) ;
bool collCompBegDEndDBegNOutput (const collCompBegDEndDBegN & cCBEDBN,
                                 ostream & os, char * s,
                                 const size_t len, const int algn,
                                 const size_t skip, int dCiph) ;
bool collBegDEndDBegNOutput (const collBegDEndDBegN & rCollBegDEndDBegN,
                             ostream & os, char * s, const size_t len,
                             const int algn, const size_t skip,
                             int dCiph) ;
# if INSTANCE
bool damnLikeCitizenOutput (const TDamn & damn, ostream & os,
                            char * s, const size_t len, const int algn,
                            const size_t skip, int dCiph) ;
bool damnOutput (const TDamn & damn, ostream & os,
                 char * s, const size_t len, const int algn,
                 const size_t skip, int dCiph
# if 20000167  // 16.jun.2000
			, const TDecision * pDecis = NULL // 16.jun.2000
# endif  // 20000167  // 16.jun.2000
                ) ;
# endif
bool firmLikeCitizenOutput (const TFirm & firm, ostream & os,
				    char *, const size_t len, const int algn,
                            const size_t skip, int dCiph) ;
bool foreignAddrLikeAddrOutput (const TForeignAddress & a,
                                ostream & os, char * s,
                                const size_t len, const int algn,
                                const size_t skip, int dCiph) ;
bool inreg_output (const TInReg & inReg, ostream & os, char * s,
                   const size_t len, const int algn,
                   const size_t skip, int dCiph) ;
bool inreg_event_output (const TInRegEvent & inRegEvent, ostream & os,
                         char * s, const size_t len, const int algn,
                         const size_t skip, int dCiph) ;
bool lawsuitOutput (const TLawsuit & lS, ostream & os, char * s,
			  const size_t len, const int algn,
			  const size_t skip, int dCiph) ;
bool miscOutput (ostream & os, char * s,
		     const size_t len, const int algn,
		     const size_t skip, int dCiph,
		     TWindow * pParent=NULL   // 20060120
		     ) ;
bool monthOutput (ostream & os, char *, // s
			const size_t len,
			const int algn, const size_t skip, int dCiph) ;
bool regionalArealOutput (ostream & os, char * s,
				  const size_t len, const int algn,
				  const size_t skip, int dCiph,
				  int composition=0 // 20051228
				  ) ;
bool request_output (const TRequest & req, ostream & os,
			   char * s, const size_t len, const int algn,
			   const size_t skip, int dCiph) ;
bool sessionOutput (const TSession & sess, ostream & os, char * s,
			  const size_t len, const int algn,
			  const size_t skip, int dCiph) ;
bool sideOutput (const TSideWind & side, ostream & os, char * s,
		     const size_t len, const int algn,
		     const size_t skip, int dCiph) ;
bool Trade634DlgDataOutput (const Trade634DlgData & t364DD,
				    ostream & os, char * s,
				    const size_t len, const int algn,
				    const size_t skip, int dCiph) ;
bool tSCountOutput (const collCompBegDEndDBegN * cCBEDBNPtr,
			  ostream & os, const size_t len, const int algn,
			  const size_t
# if INSTANCE
				skip
# endif
				, int dCiph
				, const CodeValueArray * const pCodeValueArr = NULL
# if INSTANCE
				,
			  const TISArray <reportData> * const pRDArr = NULL
# endif
			 ) ;
