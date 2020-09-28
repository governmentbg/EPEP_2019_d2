// ---------------------- Column utils ---------------------------------
void add_abolishing_to_column (column * const pColumn,
                               const CDate abolishingDate,
			 const TSession * pAbolishingSess = NULL  // 12.dec.2001
                              ) ;  // 29.may.2001
void addAgeToColumn (const char age, column * const pColumn) ;
void addDefNameAndSettlToColumn (const TDefendant & defendant,
                                 column * const pColumn) ;
void addDateFinishedToColumn (column * pColumn,
                              char * buff, const size_t buffL,
                              const TLawsuit & lawsuit,
                              const bool addEmptyR = true) ;
void addCompositionToColumn (const long int c, column * const pColumn) ;
void addEmptyRowsIfDiffer (column * pC1, column * pC2) ;
void adjust_column_rows (columnArray & cA,
	const unsigned int additionalRows = 0) ;
void addKindToColumn (char r, column * pC) ;
void addResultToColumn (char r, column * pC) ;
void add_decision_to_column (TDecision & dec,  // 21.feb.2001
                             column * pColumn,
                             timeBookHeap * pTBH = NULL,
                             bool addKind = true,
                             bool addResult = false,
# if INSTANCE
                             const char lawKind = '\0',
                             const char sessKind = '\0',
# endif
                             const char sessResult = '\0',
				     const char sessResultToCount = '\0'
                             ) ;
// addDecision_to_column - in _notinus.bak (10.sep.2001)                           
bool addDecisionToColumn (const TRCDKey & key, const CDate & date,
                          const char sessKind,  // 15.jan.2001
                          column * pColumn, timeBookHeap * pTBH = NULL,
				  const char sessResult = '\0') ;
void cat_date_result(char *s, TDecision &dec, size_t size);	// 2010:027				  
void addDecisionKindToColumn (column * pColumn, TDecision & dec,
                              const bool addKind = true) ;  // 21.feb.2001
void add_local_surr_ments_to_column (column & decisionsColumn,
                            const TGArray <TSurroundment> & surroundments,
                                     TDecision & dec,
                                     const TLawsuit & law) ; // 05.oct.2001
bool addLocalJudgemToColumn (const TRCDKey & key, const CDate & date,
				     const char ucnType, const char * ucn,
                             const char involvement,  // 20060413
                             column * pColumn,
                             const char decisKind = '\0',
                             accusedsDistribution * pAccDistr = NULL,
                             bool * pNotMajor = NULL) ;
void defendantsAndJudgements (TSession & sess,  // 03.aug.2001 const TSession
                              TLawsuit & lawsuit,
                              timeBookHeap * pTBH,
                              accusedsDistribution & accDistr,
                              long int & notMajorD,
                              column * defendantsColumn = NULL,
                              column * decisionsJudgementsColumn = NULL,
					column * dateReturnedColumn = NULL, // 03.aug.2001
                              bool catchOtherEndingResults = false,
                              const bool wantDefSettl = false,
                              const CDate * pDateAbolish = NULL,
                           const TSession * pAbolishingSess = NULL, //13.dec.2001
                           const showJudgements = true              // 20060719
                             ) ;
void addCancLeftStopReasonToColumn (column * pColumn,
                                    const TSession & sess,
                                    TLawsuit & lawsuit,
                              bool catchOtherEndingResults = false,
                                    const TSession * pNextSess = NULL
# if INSTANCE
# else
                                    , const bool addComposit = false
# endif
                                   ) ;
void addLongToColumn (column * pColumn, const long int n,
			    const bool includingZero = false) ;
// 2016:042 LPR: +related
void addSubjCodeToColumn(column *pColumn, long subject, char lawKind);
void addNextSessionToColumn (column * pColumn,
                             char * buff, const size_t buffL,
                             const TSession & nextSess,
                             const bool addComposition = false) ;
// addDecKindToColumnIfNecessary - in _notinus.bak (10.sep.2001)
# if INSTANCE
void addRelatedLawsToColumn (column * const pColumn,
                             const TLawsuit & rLawsuit,
                             char * const buff,
				     const size_t buffSz,
                             const bool useOnPrefix = true  // 15.nov.2001
                            ) ;
void addRelated_1st_Inst_LawsToColumn (column * const pColumn,
                             const TLawsuit & rLawsuit,
                             char * const buff,
                             const size_t buffSz,
                             const bool useOnPrefix = true  // 20070214
                            ) ;
void addInDocAndRelatedLawsToColumn (column * pColumn,
                                     const TLawsuit & rLawsuit,
                                     char * const buff,
                                     const size_t buffSz) ;
void add_dec_res_damn_name_to_column (column * pColumn,
						  TLawsuit & lawsuit,
						  char * const buff,
						  const size_t buffSz,
						  const TDecision * const pDec,
						  const bool decFound,
						  const TSession & sess,
						  const bool addDesTxt
						 ) ; //23.jun.2000
# else  // INSTANCE
# endif  // INSTANCE
void add_preliminaryActions_to_column (column * pColumn,
						   char * const buff,
						   const size_t buffSz,
						   const TLawsuit & law
						  ) ;
void add_resolution_name_to_column (const char reso, column * pColumn) ;
void add_resolution_name_to_column (char * const buff,
						const size_t buffSz,
						const char * const prefix,
						const char reso, column * pColumn) ;
void add_req_resolution_to_column (const TRequestResolution & fr,
					     column * pColumn, char * const buff,
					     const size_t buffSz) ;
void add_date_to_column (column * pColumn, const CDate & d,
				 const char * const prefix = NULL,
				 const char * const suffix = NULL,
				 const bool emptyAllowed = false) ;
void many_end_docs_and_ret_dates (column * const decisionsColumn,
					    column * const retDatesColumn,
					    TSession & sess, const TLawsuit & law,
					    const int additionalRowsAfterSess = 0) ;
void sess_end_docs_to_columns (column * const decisionsColumn,
					 column * const retDatesColumn,
					 TSession & sess, const TLawsuit & law,
				 const TGArray <TSurroundment> & surroundments,
					 const char * const endDocKindsToSkip = NULL
					) ;  // 10.oct.2001

