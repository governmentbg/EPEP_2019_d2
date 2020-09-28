// -------------------------- ToString ---------------------------------
void addressToString (const TAddress & addr,
                      char * dest, size_t destSz) ;
void addressToStringForSummons (const TAddress & addr, char * dest,
                                size_t destSz,
                                const bool includingBlockNo = true) ;
void addressWithSettlStr_SB_ToString (const TAddress & addr,
                                      char * dest, size_t destSz,
                        const bool includingArea = false  // 02.feb.2001
                                     ) ;
void addressWithStr_SB_ToString (const TAddress & addr, ostream & os) ;
void addressWithStr_SB_ToString (const TAddress & addr,
                                 char * dest, const size_t destSz) ;
// returns a pointer to a static string
void CDateToString (const CDate & d, char * dest, size_t destSz) ;
void CTimeHMToString (const CTime & t, char * dest, size_t destSz) ;
void CDateForStatePaperToString(const CDate& d, char* dest, size_t destSz);
void CTimeHMForStatePaperToString(const CTime& t, char* dest,
  size_t destSz);
void collegeToString (char cC, char * dest, size_t destSz) ;
void compositionToString (long cL, char * dest, size_t destSz) ;
void compositionToStringCompl (long cL, char * dest, size_t destSz) ;
void container_sides_to_string (char * const dest, const size_t destSz,
					  const TRCDKeyContainer & container,
					  const TSideWind & pattern,
					  const int suitors = -1,  // < 0 --> all
					  const int defendants = -1,  // < 0 --> all
					  const int others = 0) ;
void grantToString (char gC, char * dest, size_t destSz) ;
bool involvementToString (char iC, char * dest, size_t destSz,
				  const bool upper = false  // 19.apr.2000
                         ) ;
bool involvementWithGenderToString(const char* ucn, const char ucnType,
  const char involvement, char* const dest, const size_t destSz,
  const bool upper = false, const bool fullArticle = false,
  const bool brackets = false);
void kind_to_alias (char kC, char * dest, size_t destSz) ;
void kindToString (char kC, char * dest, size_t destSz) ;
void connectKindToString(char kC, char * dest, size_t destSz) ;
void kindWithArticleToString (char kC, char * dest, size_t destSz,
                              const bool full) ;  // 02.feb.2001
void kindToStringAbbr (char kC, char * dest, size_t destSz) ;
// 2011:111 LPR: == monthToString
#define month_to_string(month, dest, descSz) monthToString(month, dest, destSz)
void monthToString (int month, char * dest, size_t destSz) ;
  // Pass tC as it is expected to be stored in an T_MONTH record

void period_to_string (CDate begD, CDate endD,
			     char * const dest, const size_t destSz,
                       bool mergeToMonthIfPossible = false,
                       const bool mergeToYearIfPossible = false,
                       const bool upper = false,
                       const bool shortMYTxt = true,
                       const char * const prefix = NULL,
                       const char * const suffix = NULL) ;
void preliminary_actions_to_string (char * const buff,
                                    const size_t buffSz,
                                    const TLawsuit & law,
                                 const char * const senderInstances = NULL,
                                    const bool withSender = true,
						const char * const senderPrefix = NULL,
                               const bool labelOnly = false  // 17.dec.2001
                                   ) ;  // 06.dec.2001
   void connect_to_string(const TConnect * pConnect, char * const dest,
				  const size_t destSz,
				  // const bool is1,
				  // const char sourceKind,
				  // const char psKind = PS2_KIND_FAKE,
				  const bool withSender = true,  // 06.dec.2001
				  const char * const senderPrefix = NULL,//06.dec.2001
				  const bool labelOnly = false,  // 17.dec.2001
				  const bool alias_txt = false   // 20070201
				 ) ; // 25.jul.2000



void sourceNoAndSenderToString (const TLawsuit & lawsuit,
					  char * dest, size_t destSz) ;
void region_act_to_string(const TRequest& rRequest, char* const dest,
  const size_t destSz);  // 21.dec.2004
bool resolutionToString (char rC, char * dest, size_t destSz) ;
void result_as_faked_kind_to_string (const char rC, char * const dest,
						 const size_t destSz,
						 const bool article,
						 const bool fullArticle,
						 const bool msgWhenTroubled) ;
bool resultToString (char rC, char * dest, size_t destSz) ;
void sessDateEndToString (char * s, const size_t sSz,
				  const TSession & sess) ;
void settlStreetBlockNoToString (const TAddress & a, char * const dest,
					   const size_t destSz) ;
void side_to_string (const TSideWind * const pSide, char * const dest,
			   const size_t destSz, const bool includingUCN = false,
			   const bool includingInvolvement = false) ;
void side_link_to_string (char * const buff, const size_t buffSz,
				  const TLinkWind & rLink,
				  const bool useNewLines,
			  const bool fake,
			  TGArray<TLinkWind> *links = NULL) ;  // 28.jun.2001

void streetAndAddressToString (const TStreet & street,
					 const TAddress & addr,
					 char * dest, size_t destSz) ;
void subjectToString (char type, long subject, char * dest, size_t destSz,
			    size_t skip = 0) ;
void subjectToAlias (char type, long subject, char * dest, size_t destSz,
			    size_t skip = 0) ;
void subjectAndExtraTextToString (char kind, long subject,
					    char * dest, size_t destSz,
					    const char * extraText = NULL,
					    const bool write_sL = true) ;
void surround_to_string (const TSurround & surround,
				 char * const dest, const size_t destSz) ;
void surroundment_to_string (const TSurroundment & ment,
				     char * const dest, const size_t destSz) ;
void thisCourtToString (char * dest, size_t destSz) ;
void type_to_alias (char tC, char * dest, size_t destSz) ;
void typeToString (char tC, char * dest, size_t destSz) ;
void ucn_type_to_string (char ucnT, char * dest, size_t destSz) ;
