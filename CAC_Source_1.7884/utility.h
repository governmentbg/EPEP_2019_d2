// ----- convertors ----------------------------------------------------------
char accomplys2DebtorStatus(const char *accomplys);
void Composition2Group(TGroup *group, char college, long composition, bool clear, bool secretar);
TSideWind *Container2SideWind(TRCDKeyContainer *container);  // 2003:332 BRW
char Container2Sched(TRCDKeyContainer *container, char result = '\0');
const CDate &container_event_date1(const TRCDKeyContainer *container);
char container_to_law_kind(TRCDKeyContainer* container, const bool errMsg = true);
int find_person(TGroupArray *persons, const char *opt_person);  // 2018-09-25
char inReg2lawType(TInReg *inReg, TRCDKey * const pLawKey = NULL, TRCDKey * pReqKey = NULL);
int interval2Months(const CDate &start, const CDate &final);
char interval2ActAge(const CDate &start, const CDate &final, sign twoMonths = false);
int interval2Months(char lawsuitKind, const CDate &start, const CDate &first, const CDate &final);
char interval2LawAge(char lawsuitKind, const CDate &start, const CDate &first, const CDate &final);
long involvement2assignment(char involvement);
char involvement2known(char involvement);
int keykind2interval(TRCDKey *key, const char kind);
TDatedContainer *kind2DatedContainer(const char kind);
TRCDKeyContainer *kind2RCDKeyContainer(const char kind);	// 2010:005
const char *kind2SurroundmentResults(const char kind);
char kind2VisibleKind(const char kind);
void kind2involvements(const char kind, mstr &involvements);
inline char kind2type(const char kind) { return Kind->Seek(kind), Kind->type; }
char index2kind(int index, const char *kinds);
int kind2index(char kind, const char *kinds);
inline char lawsuit_kind_from(char originKind, char secondKind)
	{ return strchr(KIND_LAWSUITS, originKind) ? originKind : secondKind; }
char origin2LawsuitKind(const TRCDKey &origin, bool complain);
bool strvalue_to_lset(const char *value, TIntegerArray *lset, const char *name);		// fatal() if name != NULL
void strvalue_to_lranges(const char *value, TIntegerArray &ranges, const char *name);	// name must be non-NULL
bool try2decide(const TSession *session, TGroupArray *decisions, const char *kinds = NULL, const char *fields = NULL);
bool try2surround(const TSession *session, TGArray<TSurroundment> *surroundments, const char *fields = NULL,
	const char *kinds = NULL, bool decisionKinds = false);
bool try2surroundecide(const TSession *session, TGroupArray *decisions);
void fre2surroundecide(TGroupArray *decisions);
char connectKind2Type(char connectKind);	// 2010:005
char type2ConnectKind(char type);		// 2010:005 LPR: non-inline
TRCDKeyContainer *type2RCDKeyContainer(char type);
const char *type2ses_kinds(char type);
const char *kind2ses_kinds(char kind);	// 2017:305
inline char ucn2uxn(const char ucnType) { UCNType->Seek(ucnType); return UCNType->uxnType; }
inline char ucn2code(char ucnType) { return (char) toupper(ucn2uxn(ucnType)); }
#if COURT_TYPE == COURT_REGIONAL
char heritageState2Result(char state);
char heritageResult2State(char result);
#endif  // REGIONAL
bool fileTime2CDate(FILETIME *fileTime, CDate &date);

// ----- classifiers ---------------------------------------------------------
// 2007:201 LPR: all is_required() moved to nom
inline bool is_code(const char ucnType) { return isupper(ucnType) || strchr(UCN_LOW_CODES, ucnType); }
bool is_suitor_ll(const char kind, const char involvement);
bool is_sysadmin();
bool is_internal(const char *ucn);
bool is_real_ucn(char ucnType, const char *ucn);
inline bool is_real_ucn(const TUCNGroup *ucnGroup) { return is_real_ucn(ucnGroup->ucnType, ucnGroup->ucn); }

// ----- access --------------------------------------------------------------
bool can_func(const char func);
bool can_menu(const char menu);
bool can_admin(const char func);
bool can_query();
bool can_direct(long flag);
bool can_direct(long area, long region, long lock);	// 2010:272
// 2009:131 LPR: the right thing; only archived and killed statuses are checked
bool can_access_college(char college, bool write);	// 2017:222 LPR: separated
bool can_access_exact(const TRCDKeyContainer *container, bool write, bool archeck, mstr *m);
TRCDKeyContainer *can_access_basic(const TRCDKey &key, bool write, bool archeck, mstr *m);
inline TRCDKeyContainer *can_access_basic(TRCDKeyGroup *group, bool write, bool archeck, mstr *m)
	{ return can_access_basic(group->key, write, archeck, m); }
bool can_extra_db();		// 2015:335 LPR: public
bool can_portable();		// 2016:165
bool can_surrouendoc();		// 2016:246
bool can_back_date();		// 2017:059
bool can_extern_sess();		// 2017:072

// ----- special access ------------------------------------------------------
const char *query_colleges();
#define QUERY_COLLEGES query_colleges()
const char *query_compositions();
#define QUERY_COMPOSITIONS query_compositions()
#define can_rename() can_func(RIGHT_RENAME)

// ----- misc ----------------------------------------------------------------
const char *active_button_text(TGroup *group, long flags);
void add_side_name(mstr &m, TSideWind *sideWind, const char *sep = NULL, bool reNameInitial = true);  // NULL == ", "
bool already_restarted(const TRCDKey &key, bool oldRestart, bool newRestart, const CDate &restartDate);
bool ask_cause(mstr &cause);
bool ask_college(char kind, char college, const char *what);
inline bool assumes_decision(char result)
	{ return strchr(RESULT_ENTERABLES, result) && !strchr(RESULT_REENTERABLES, result); }
void write3x(char *s, long value, int len, int radix);
long read3x(const char *s, int len, int radix, bool crash);
void barcode_generate(mstr &m, long requestId, bool asterisk, long court = COURT_CODE);
bool barcode_scan(const char *s, long *court, long *requestId, char *ces);
const char *blocked_sidewind(TSideWind *sideWind, const char status, bool partial);
const char *blocked_sidewind(const TSideWind *sideWind, TGArray<TLinkWind> *links);
bool build_ckinds(char *kinds, const char *tKinds, bool write);
bool build_ctypes(char *types, const char *tTypes, bool write);
void calc_eispp(const char *value, char *s);
void calc_final(const char *buffer, char *ptr, const char *ucn, size_t size, char *md5sum);
void calc_md5sum(const char *buffer, size_t size, char *resblock);
void calc_pass(TUser *user, const char *password);
void camel_case(mstr &m, const char *text, constant *prefixes);
bool check_exists(const char *table, TGroup *group, const char *fields);
const char *check_exists(constant *tables, TGroup *group, const char *fields);
sign check_pshell_md5sum(TWindow *parent, const char *name);
bool conteq(const TRCDKeyContainer *container1, const TRCDKeyContainer *container2);		// key, kind, date
void copy_winds(const TRCDKeyContainer *source, TRCDKeyContainer *target, TWindData &windData, bool override = false);
inline const char *etz(const char *s) { return !s || atob(s) ? s : ""; }
void filter_prefixes(char *target, const char *source, constant *prefixes); // 2011:003 LPR: public
char find_college(const TRCDKeyContainer *container, TRCDKey *origin = NULL);
bool find_damn(const TLawsuit *lawsuit, TDamn *damn, bool complain = true);
bool find_finalizer(const TLawInterval &interval, TSession &session, const CDate &start, const CDate &final);
bool find_finalizer(const TLawsuit *lawsuit, TSession &session, const CDate &start = DATE_MIN, const CDate &final = DATE_MAX);
TDecision *find_finalizer(const TSession &session, TGroupArray &decisions, const char *priorityResults = NULL);	// 2017:044
bool find_words(const char *names, const char *uname, int words, long flags);
bool find_origin(const TRCDKeyContainer *container, TRCDKey &origin);
void full_side_list(mstr &m, TGArray<TSideWind> *sideWinds, const char *categories, char category, bool reNameInitial = true);
void full_side_list(mstr &m, TGArray<TSideWind> *sideWinds, char involvement, bool reNameInitial = true);
bool generate_max(const char *name, const char *s, long *data, long maximum, long year);	// 2004:236
void generator_limit(const char *name);	// fatal
void get_received_statuses(mstr &statuses, char receivedStatus, const TSubpoena *subpoena);	// 2018-01-20
void get_forty_fifty_stats(mstr &statuses, char type);	// 2018-03-27
bool get_receiver(const TOutReg *outReg, TSender *sender);
const char *get_sencor_name(char ucnType, long court, mstr &m);	// 2017:088
bool handle_restart(const TRCDKey &key, bool oldRestart, bool newRestart, const CDate &restartDate, bool firstSet);
char inside_letters(char first, char c);
int load_connects(const char *criteria, TGArray<TConnect> *connects);
int load_connects(const TRCDKey &key, TGArray<TConnect> *connects);
inline int load_connects(const TRCDKeyContainer *container, TGArray<TConnect> *connects)
	{ return load_connects(container->key, connects); }
void load_sidebasicv(TGArray<TSideWind> &sideWinds, const char *criteria, const char *extraUCNTypes = NULL);
void load_sidebasicv_criteria(msql &m, const TRCDKeyContainer *container);
void load_sidebasicv(const TRCDKeyContainer *container, TGArray<TSideWind> &sideWinds);
char *load_categories(TRCDKeyContainer *container, TGArray<TSideWind> &sideWinds);
char *load_sidebasic(TRCDKeyContainer *container, TGArray<TSideWind> &sideWinds);
void load_sidewinds(const TRCDKeyContainer *container, TGArray<TSideWind> *sideWinds);	// 2011:286 overdue
TGArray<TSideWind> *load_sidewinds(const TRCDKeyContainer *container);	// 2004:293; 2010:074
TConnect *main_connect(const TRCDKeyContainer *container);
void map_eispp(char *base, int low, int high);
void memzero_s(char *p, size_t size);
NET_API_STATUS net_validate_name(const char *server, const char *name, const char *account, const char *password,
	NETSETUP_NAME_TYPE NameType);
const char *obtain_deliverer(TGroupArray *deliverers, long uclp, long street, const char *adrNo, const char *blockNo);	// 2012:041
struct TFlagName
{
	long flag;
	const char *name;
};
// 2015:049 LPR: next_session (from 2015:036) -> nearest_event
void nearest_event_crit(msql &m, TRCDKeyContainer *container, bool forward, bool rejectSelf);
TRCDKeyContainer *nearest_event_find(msql &m, TRCDKeyContainer *container, bool forward);
TRCDKeyContainer *nearest_event_find(msql &m, TSideWind *sideWind, bool forward);
void print_flags(mstr &m, const TFlagName *flagNames, long flags, const char *sep, const char *lead);
void print_gps_link(mstr &m, double coordN, double coordE, const char *text);
void print_initials(mstr &m, const char *ucn, char ucnType = UCN_CITIZEN_UCN, bool userFormat = false);
void print_firm_name(mstr &m, TUCNGroup *ucnGroup, bool filtered = false);	// filter ET only
void print_sender_name(mstr &m, TUCNGroup *ucnGroup);
void print_side_name(mstr &m, TUCNGroup *ucnGroup, bool reNameInitial);
void print_side_wind(mstr &m, TUCNGroup *ucnGroup, bool filtered);
void print_missing(mstr &m, char ucnType, const char *ucn);
void print_name(mstr &m, const char *ucn, char ucnType);	// always append
bool print_postal(mstr &m, long flags, long flagsX);
inline bool print_postal(mstr &m, const TOutReg *outReg) { return print_postal(m, outReg->flags, outReg->flagsX); }
void print_received(mstr &m, char status, const CDate &received, const CTime &receivedTime, double coordN, double coordE);
const char *send_type(const TOutReg *outReg);		// 2017:051 outlined from print_send()
void print_send(mstr &m, const TOutReg *outReg);
void print_sfor(mstr &m, const TRCDKeyContainer *container);	// kind|text, date, no/year
void print_user(char *s, const TCitizenNames *names, bool rank = true, sign reFamily = false);
void print_user(char *s, const char *ucn, char ucnType = UCN_CITIZEN_UCN, bool rank = true, sign reFamily = false);
void print_user(mstr &m, const TCitizenNames *names, bool rank = true, sign reFamily = false);
void print_user(mstr &m, const char *ucn, char ucnType = UCN_CITIZEN_UCN, bool rank = true, sign reFamily = false);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
inline bool public_decision(char sessionResult, char) { return sessionResult == RESULT_PUBLIC_DECISION; }
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
inline bool public_decision(char sessionResult, char decisionKind)
	{ return sessionResult == RESULT_PUBLIC_DECISION && !strchr(KIND_EXEC_PROTS, decisionKind); }
#endif  // REGIONAL
void remove_file(const char *name, const char *what, sign remove = -1);
inline void removeFile(const char *name) { remove_file(name, NULL); }
const char *rrseek(const char type, char status);
inline const char *rrseek(const TRCDKeyGroup *group, char status) { return rrseek(group->key.type, status); }
void surrouendoc_text(mstr &m, const TSurroundment *surrouendoc);
bool say_cant_delete();
bool say_cant_update();
extern constant MISSING_DATA;
bool say_no_data(const char *name);
bool say_no_selection();
bool secondary(char lawsuitKind);
inline void sleep_sync() { Sleep(SLEEP_SYNC); }
unsigned split_words(char *names, const char *spaces = NULL);
// 2007:255 FIX: replaced buggy _ round()
double round(double d, int nDec);	// 5 -> up
inline long round(double d) { return round(d, 0); }
double uround(double d);	// 0.5 -> default
inline void subcount(mstr &m, long value) { itom(value ? value : 1, m); }

struct TContainerSched
{
	const char *originKinds;
	const char *scheduleKinds;
	char result;
	char sched;
};

extern const TContainerSched ContainerScheds[];

char get_schedule_kind(const TRCDKeyContainer *container);
void schedule(char sched, TRCDKeyContainer *container, const CDate *start, const CDate *final = NULL,
	const char *judge = NULL);
void transliterate(const char *src, char *dst, size_t maxlen);
void unschedule(char sched, TRCDKeyContainer *container);
void unschedule(char sched, TLawsuit *lawsuit);
void unschedule(char sched, TInReg *inReg);
bool write_connects(const TRCDKeyContainer *container, TGArray<TConnect> *connects, char *what);
