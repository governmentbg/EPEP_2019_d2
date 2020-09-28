// ---------------------------- ToName ---------------------------------
// 2016:264
void citizenUCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m, bool fathShort = false, bool nShort = false,
	bool includingFatherN = true, bool rank = true);
void firmUCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m);
void senderUCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m);
void damnUCNAndUCNTypeToName(const char *ucn, char ucnType, mstr &m);
void ucnAndUcnTypeToName(const char *ucn, char ucnType, mstr &m);

bool citizenUCNAndUCNTypeToName (const char * ucn,
                                 const char ucnType,
					   char * destN, size_t destNSz,
                                 bool fathShort = false,
                                 bool nShort = false,
					   bool includingFatherN = true,
					   bool rank = true) ;
bool countryToName (const char * const c,
                    char * const dest, const size_t destSz) ;
bool firm_subject_short_name(const long int subj, const char type,
  char* const dest, const size_t destSz,
  const bool unknownAsCode = false);  // 14.aug.2002
bool firmUCNAndUCNTypeToName (const char * ucn, const char ucnType,
                              char * destN, size_t destNSz) ;
bool senderUCNAndUCNTypeToName (const char * ucn, const char ucnType,
                                char * dest, size_t destSz) ;
bool damnUCNAndUCNTypeToName (const char * ucn, const char ucnType,
                              char * dest, size_t destSz) ;
bool lawsuitToSenderName (const TLawsuit & lawsuit,
                          char * dest, size_t destSz,    // 06.apr.2000
                          bool courtNameOnly = false  ); // 20070427
  // see also sourceNoAndSenderToString
//bool judgeUCNToName (const char * ucn, char * dest, size_t destSz) ;
bool streetToName (long street, long uclp, char * dest, size_t destSz) ;
bool uclpToName (long uclp, char * dest, size_t destSz) ;
bool uclpToAreaName (long uclp, char * dest, size_t destSz) ;
bool uclpToMunicipalityName (long uclp, char * dest, size_t destSz) ;
bool ucnAndUcnTypeToName (const char * ucn, char ucnType, char * dest,
                          size_t destSz) ;

