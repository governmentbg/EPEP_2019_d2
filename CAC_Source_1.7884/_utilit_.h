// --------------------------- Utils -----------------------------------
bool choose_link_wind(TWindow* parent,
  TGArray<TLinkWind>& walnLinks,  // 09.oct.2003 from TWALNLink
  int& choosenInd, const char* const prefix);

void load_non_nested_subject_ranges(const char* const types,
	TGArray<TSubjectRange>& rangesArr);  // _zdravk.cpp
void mkStrBlank (char * s, size_t sSz) ;
bool abbrev (const char * const src, char * const dest,
		 const size_t destSz) ;  // 27.mar.2001
void writeString (ostream & os, const char * s, int w,
			int align = RIGHT_ALIGN, size_t from = 0) ;
inline void writeString(ostream &os, const mstr &m, int w, int align = RIGHT_ALIGN, size_t from = 0) { writeString(os, str(m), w, align, from); }
void writeDoubleStrikedString (ostream & os, const char * s, int w,
					 int align = RIGHT_ALIGN,
					 size_t from = 0,
					 const bool upCCyr = false) ;
void writeLong (ostream & os, const long int n, int w,
		    int align = RIGHT_ALIGN) ;
void writeULong (ostream & os, const unsigned long int n, int w,
		     int align = RIGHT_ALIGN) ;
void writeShortString (ostream & os, const char * str, size_t len = 2) ;
void writeDate (ostream & os, const CDate d, int w, int align,
		    const size_t from) ;  // 28.sep.2000
// --- The printFile and viewPrintFile functions are moved to _notinus.bak
// (10.jan.2001) as unused from a very long time. Now other functions are
// written --------------------------------------------------------------
char * loadCollegeCiphers () ;
 // Returns pointer to a newly created string if success,
 // or NULL if error
char * loadCompositionCiphers () ;
 // Returns pointer to a newly created string if success,
 // or NULL if error
char * loadResultCiphers () ;  // 14.mar.2002
 // Returns pointer to a newly created string if success,
 // or NULL if error
char * loadTypeCiphers (bool followRights = true) ;
 // Returns pointer to a newly created string if success,
 // or NULL if error
inline void clearLawsuitData (TLawsuit & lS) { clearGroupData (lS) ; }
inline void clearSideData (TSideWind & s) { clearGroupData (s) ; }
inline void clearCitizenData (TCitizen & c) { clearGroupData (c) ; }
inline void clearFirmData (TFirm & f) { clearGroupData (f) ; }
inline void clearSessionData (TSession & s) { clearGroupData (s) ; }
inline void clearGroupData (TGroup & group) { group.ClearGroupData(); }
inline void clear_CDate (CDate & d) { d.Clear(); }
// critSz must be at least (70 + strlen (table) + strlen (orderBy)+
//                          10 + 1)  (see the next row)
//select * from  where f_no = 99999 and f_year = 9999 and f_type = 't' ;
// (see TRCDKeyGroup::Generate() for maximum of F_NO --> 99999)
long int sessionsForTSCount (const char * resultsString,
				     const collCompBegDEndDBegN * cCBDEDBNPtr,
                             const char * ageString = NULL,
                             const char * pGrantChar = NULL,
                             const char * pSessKindChar = NULL,
                             bool skcEq = true,
                             const char * lawKindStr = NULL,
                             const char * sourceKindStr = NULL,
                             const bool followSessDate = true,
                             const char * const decisResultsStr = NULL,
                             const bool resultsStringResultsMatch = true,
                             const bool excudeAbolishedResults = false,
                          const bool requestCurrentlyQuick = false //25.sep.2003
                            ) ;
long int sessionsForRSCount (const char * resultsString,
                             const collCompBegDEndDBegN * cCBDEDBNPtr,
                             const char * ageString = NULL,
                             const char * pGrantChar = NULL,
                             const char * pSessKindChar = NULL,
                             bool skcEq = true,
                             const char * lawKindStr = NULL,
                             const bool followSessDate = true,
                             const bool includingClosedSessions = false
                            ) ;
long int sessionsForRSCountMembership (const char * resultsString,
                             const collCompBegDEndDBegN * cCBDEDBNPtr,
                             const char * ageString = NULL,
                             const char * pGrantChar = NULL,
                             const char * pSessKindChar = NULL,
                             bool skcEq = true,
                             const char * lawKindStr = NULL,
                             const bool followSessDate = true,
                             const bool includingClosedSessions = false,
                             const bool judgeOnly = true
                                      ) ;
long int closedSessCount (const collBegDEndDBegN & cBegDEndDBegN,
                          const char * resultsString = NULL,
				  const char * decisKindsString = NULL,
# if INSTANCE
				  int instns = 0,
# endif
				  const bool followSessDate = true,
				  const char * const lawSourceKindsString = NULL,
                          const bool resStrMatch = true
                          ) ;
long int
  closedSessDecidsOrCancelsCount (const collBegDEndDBegN & cBegDEndDBegN,
# if INSTANCE
                                  int instns = 0,
# endif
                                  bool decideds = true,
                          const char * const lawSourceKindsString = NULL
                                 ) ;
long int closedSessDecidedsCount (const collBegDEndDBegN & cBegDEndDBegN,
# if INSTANCE
                                  int instns = 0,
# endif
                          const char * const lawSourceKindsString = NULL
                                 ) ;
long int closedSessCanceledsCount (const collBegDEndDBegN & cBegDEndDBegN,
# if INSTANCE
                                   int instns = 0,
# endif
                           const char * const lawSourceKindsString = NULL
                                  ) ;
# if INSTANCE
unsigned long int puniConcern2ndLawsCount (const CDate endP,
                                           const CDate begP,
                                         const bool addNotFinBeg = false,
						     const bool finisheds = false,
                                         const bool onlyNFB = false) ;
unsigned long int adm2ndLawsCount (const CDate endP,
                                   const CDate begP,
                               const long int composition,  // 27.feb.2001
                                   const bool finisheds = true,
                                   const bool nFBFromLaws = true,
                               const char * pLawTypes = NULL,  // 20070923
                               const char * pLawKinds = NULL   // 20070923
                                   ) ;
  // If finisheds is true, nFBFromLaws is ignored
# endif
unsigned long int countFinishedsWithDecJudgDefin (
                             const collCompBegDEndDBegN * cCBEDBNPtr,
                             const char * const decGJkindsStr = NULL,
                               const char * const lawKindsStr = NULL,
                             const char * const sourceKindStr = NULL,
                       const bool requestCurrentlyQuick = false  // 25.sep.2003
                             ) ;
CDate firstDayOfThisMonth (CDate d) ;
CDate firstDayOfNextMonth (CDate d) ;
CDate nextDay (const CDate &d) ;
long int differenceInMonths (const CDate &smaller,
				     const CDate &greater) ;
int difference_in_days (const CDate &date1, const CDate &date2);
CDate lastDayOfThisMonth (CDate d) ;
long int reportSidesCount (const char * types,
				   const long int composition,
				   const CDate * pHighRange,
                           const CDate * pLowRange = NULL,
                           const char * resultsString = NULL,
                           const char * judgedString = NULL,
                           const char * judgementKinds = NULL,
                           const char * oldString = NULL,
                           const char * punishmentString = NULL,
                           const char * recidiveString = NULL,
                           const char * lawKindString = NULL,
                           const char * const freedStr = NULL,
                     const char * const lawPS2Str = NULL  // 11.jan.2001
                          ) ;
unsigned long int countRecsInTable (const char * table) ;
char * intersectionOrDifference (char * set, const char * test,
                                 bool action) ;
  // if action is true :
  // deletes from set the chars, who are not included in test
  // if action is false :
  // deletes from set the chars, who are included in test
  // returns set
char * intersection (char * set, const char * test) ;
  // deletes from set the chars, who are not included in test
  // returns set
char * difference (char * set, const char * test) ;
  // deletes from set the chars, who are included in test
  // returns set
char * makeTimeBookFormatFN (const char college,
# if INSTANCE
  const bool firstInstLaws, const bool puniAdmConcLaws,
# endif
  const bool jan2005Format) ;
void writeTBSummary (const char * summFFN,
		     const collCompBegDEndDBegN * pCCBDEDBN,
			   TWindow * pParent,
			   const CodeValueArray * const pCodeValueArr = NULL,
# if INSTANCE
			   const TISArray <reportData> * const pRpDataArr = NULL,
# endif
			   const char * const whichSummary = NULL  // 14.feb.2002
			  ) ;  // The definition is in _TBSThrd.cpp
# if INSTANCE
const char * lawsuitKindSuffix (const TLawsuit & lawsuit) ;
# else
# endif

// Show dialog, possibly in a separate thread
bool show_dlg(TDialog *dialog, bool wait);

char * strDelLeadingSpaces (char * str) ;
char * strDelLastSpaces (char * str) ;
# if INSTANCE
long int minFirmSubject () ;
long int maxFirmSubject () ;
# endif

void makeJudgeCheckupFormatFN (char * const formatFN,
					 const size_t fmtFNSz,
					 const bool trueJudgeCheckup,
					const begMYEndMYTypeCIU & const bMYEMYTCIU,
# if INSTANCE
				const collCompBegDEndDBegN & const cCBDEDBN,
# endif
const unsigned int cipherIfNotTrueGudgeCheckup = OFFSET_TIME_BOOK_CIPHER,
int mainMenuItem = 0
					) ;
void outNormDblStrkStringAtCondStream (ostream & os, const char * const s,
						   const size_t len, const int algn,
						   const size_t skip) ;
size_t strConcat (char * dest, const char * appendix, size_t destSz) ;
char* strCopy (char * dest, const char * src, const size_t destSz) ;
size_t strConcatCh (char * dest, const char appendix, size_t destSz) ;
size_t strConcatLong (char * dest, const long n, size_t destSz) ;
void add_set_bit_values (arrayOfLong & arr, const int flagValue,
				 int maxFlags) ;  // 10.jan.2001
void invert_list_box_selection (TGroupListBox * const pLB) ;
bool lawsuit_finalising_decision (const TLawsuit & law, TDecision & dec,
					    TWindow * pParent,
					    const char * const forWhat,
					    const bool errMsg = true,
					    TSession* const sessPtr = NULL) ;//18.feb.2005
inline bool lawsuit_finalising_decision(const TLawsuit & law, TDecision & dec, TWindow * pParent = NULL) { return lawsuit_finalising_decision(law, dec, pParent, ""); }
void move_string_array_contents(TStringArray* pDest,
  TStringArray* pSrc);  // 27.jan.2005
// 20061228
bool connect_type_in( TRCDKeyContainer *container, const char *connectTypes );
// 20070103
TConnect *get_connect(TRCDKeyContainer *container, const char *connectType);
// 2009:275 static Summons::CharsMatch
bool summCharsMatch(const char required, const char found);
// 2010:273 LPR: set 1st element name to ALL_COMPOSITIONS
void set_zero_all_comp(TLongAliasFace *alias);
// 2010:273 LPR: select 1st available query composition
void select_de1st_comp(TLongAliasFace *alias);

// Note: show_*: NULL parent -> async

// Note: tText is delete[]-d
bool show_text_use_settings(TWindow *parent, char *tText, const char *formatFileName, int alignment = NO_ALIGNMENT,
	int safetyLines = 1);

bool show_text_stream_use_settings(TWindow *parent, ostrstream &oss, const char *formatFileName,
	int alignment = NO_ALIGNMENT, int safetyLines = 1);

// Note: -1 == depends on sync
bool show_html_stream(TWindow *parent, const char *name, const char *what, ostrstream &oss, sign remove = -1);
bool show_html_stream(TWindow *parent, ostrstream &oss);
void show_office_stream(ostrstream &oss);  // 2017:237

int longest_text_page_lines (char *tText, int safetyLines = 1);
int longest_text_line_symbols (char *tText) ;
