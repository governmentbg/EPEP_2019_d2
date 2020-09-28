long get_log_id(TCACDataBase *dataBase);
// 2006:011 all log()-s: bool -> void; 2009:047 resorted & cleaned up; 2013:169 TRQ/IRQ: extended log: added ucn
void log(char gop, long opc, const TRCDKey *key, char kind, const CDate *date, const char *text = NULL);
void log(char gop, long opc, const TRCDKey *key, const char *text = NULL);
void log(char gop, long opc, long no, long year, char type, char kind, const CDate *date, const char *text = NULL);
void log(char gop, long opc, const char *text = NULL);
void log(char gop, long opc, const TRCDKeyContainer *container, const char *text = NULL);
void log(char gop, long opc, const TRCDKeyContainer *container, TSideWind *sideWind);
void log(char gop, long opc, const TRCDKeyContainer *container, const TRCDKey *key);	// double
inline void log(char gop, long opc, const TSurround *surround) { log(gop, opc, surround, &surround->lawsuit); }
inline void log(char gop, long opc, const TInReg *inReg) { log(gop, opc, inReg, &inReg->lawsuit); }
void log_event(char gop, long opc, TIndexContainer *container, sign publinet = -1);
// 2015:278 limited -> lawsuit, auto text
void log_lawsuit(char gop, long opc, const TLawsuit *lawsuit);
// 2011:105 LPR: former _Usage -> log()
void register_usage(int regime, int varianth, const char* types, const char* remark);
void register_usage(int regime, const char type, int varianth, const char* remark);
void log_inout(const char *inout, const char *ucn);
void log_juror(const TBaseSession *session, char oper, const char *juror, int index, const char *text);
void log_juror_double(char gop, long opc, const TBaseSession *session, const char *text, char oper);
void log_surrouendoc(char gop, long opc, const TSurroundment *surrouendoc);
void log_subpoena(char gop, const TSubpoena *subpoena, const char *text);
void log_announce(char gop, const TAnnounce *announce, const char *suffix);
