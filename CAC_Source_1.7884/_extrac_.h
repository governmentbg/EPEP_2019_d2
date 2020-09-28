// -------------------------- Extract -----------------------------------
bool extractDecision (const TRCDKey & key,
                      const char sessKind,  // 15.jan.2001
			    const CDate & date,
                      const char kind, TDecision & decision) ;
bool extractDecision (const TRCDKey & key,
                      const char sessKind,  // 15.jan.2001
                      const CDate & date,
                      TDecision & decision, const char kind
                                           // = '\0'  // 05.sep.2001
                     ) ;
bool extract_first_decision (const TRCDKeyGroup & keyGroup,
                             TDecision & dec) ;  // 16.jun.2000
bool extractLocalJudgement (const TRCDKey & key, const char kind,
                            const CDate & date,
                            const char ucnType, const char * ucn,
                            char involvement,  // 20060413
                            TJudgement & localJudgement) ;
bool extractLocalJudgement (const TRCDKey & key, const CDate & date,
                            const char ucnType, const char * ucn,
                            char involvement,  // 20060413
                            TJudgement & localJudgement,
                            const char kind = '\0') ;
bool extractSession (const TRCDKey & key, const CDate & date,
                     TSession & session, const char kind = '\0') ;
// However, it is not recommended to pass '\0' as kind
// (because already more than 1 session of a day are allowed).
