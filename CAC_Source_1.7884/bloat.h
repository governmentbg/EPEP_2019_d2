void adjust_law_college(char lawKind, TCharAliasFace *college);
char suggest_ses_kind(TLawsuit *lawsuit, const char *kinds);
void transfer_dec_text(TCharAliasFace *result, TStringFace *text);
bool equal_ses_person(TPersonAliasFace *persons[], constant names[], size_t count, const char *spers_what);
bool equal_ses_person(TPersonAliasFace *person, TPersonAliasFace *persons[], size_t count);
bool present_ses_person(char kind, TPersonAliasFace *judge, const char *name, const char *type);
void adjust_appeal_decisions(char type, TCharAliasFace *kind, TCharAliasFace *decisionKind);
// 2003:331
void adjust_judgement_priority(const TJudgement *judgement, TCheckFace *check);
// 2001:011; 2001:260; 2003:146
char adjust_xxx_kind(char kind, TIndexFace *index, const char *kinds);
void adjust_xxx_kind(TRCDKeyContainer *container, TIndexFace *index, const char *kinds);
// 2001:011; 2001:260
void adjust_xxx_index(char kind, TIndexFace *index, const char *kinds);
// 2001:012; 2001:260
char *adjust_xxx_kinds(char *kinds, const char *visibleKinds, const char *allKinds);
// 2001:118 Are three judges required for a closed session?
bool closed_judges_3(TLawsuit *lawsuit);
// 2001:268
const char *adjust_indoc_kinds(char kind);
// 2001:324
const char *lawsuit_accomplys(char type, char kind);
// 2001:345
const char *endoc_accomplys(char result, char kind);
// 2006:039
void kill_forward_sides(const TRCDKeyContainer *container, const TSideWind *origin);
// 2006:186
bool return_date_locked(const TLawsuit *lawsuit);
// 2006:312; 2006:320 predefined made const to signify input parameter
// 2007:218 no char *sourceKinds parameter, predefined must be non-NULL
const char *adjust_source_kinds(char type, const char *predefined);
// 2007:211; 2010:011 +colleges for DELIVERER
void adjust_user_profile(char post, TCharListFace *menus, TCharListFace *functions, TCharListFace *colleges);
// 2007:214 LPR/LRQ: new algorithmic universal super-duper connect scheme
void adjust_possible_connects(char *possible, char indocKind, char lawKind, const char *already, const char *predefined);
bool connect_types_valid(char indocKind, char lawKind, const char *entered, const char *predefined);
// 2007:220 LPR: moved here from set session; 2014:184 includes check_load_session()
bool check_set_session(TSession *session);
// 2007:221 LPR: virtual creation by predefined
void adjust_lawsuit_kinds(const char *predefined, char *kinds);
// 2008:162 check+lock or uncheck+unlock limited based on subject
void adjust_lawsuit_limited(const TLawsuit *lawsuit, TCheckFace *limited, bool clear);
// 2008:193 LPR: syncing finalizer date checks; 2017:038 lawsuit -> interval
bool lawsuit_finished_valid(const TLawInterval &interval, TDateFace *finished);
// 2008:246 LPR: set/clear session final date
void adjust_ses_final(TSession *session, const char *results, CDate &date);
// 2008:262 LRQ: select/check danger
void adjust_lawsuit_danger(TLawsuit *lawsuit, long flags, const char *what);
bool lawsuit_danger_valid(const TLawsuit *lawsuit, long flags);
// 2008:288
long four_teen_year(const char *s);
long four_teen_court(const char *s);
long four_teen_type(const char *s);
long four_teen_no(const char *s);
void update_prelawsuit_sides(TSideWind *sideWind, TLawsuit *lawsuit, const char involvement);
// 2008:303 LPR: remove highest digit for specific courts/years/types
long flag_key_no(const TRCDKey *key);
// 2009:133
void add_event_kinds(char type, mstr &kinds);
const char *adjust_surround_kinds(const TRCDKeyContainer *container);
// 2009:174 LPR: too many equivalences for adjust_xxx_kinds()
char *adjust_subpoena_kinds(char *subpoenaKinds);
// 2009:191 LRQ; 2017:118 request: get kind + lawKind
void xfer_indoc_kind(TRequest *request, long xferKind, long appealKind, char college);
// 2009:204; 2011:157 LPR: -decisionKind
void adjust_cancelaw(TSession *session);
// 2009:344 LPR: like adjust subpoena kinds
extern constant GeneralKindEquivalences[];
char *adjust_general_kinds(char *generalKinds);
// 2011:178 LPR: moved here from utility
char kind2GeneralKind(char kind);
// 2009:348 LPR: used in several dialogs
void add_postal_buttons(TDialog *dialog, TCheckBox *buttons[] = NULL);
// 2010:053; 2010:007 LRQ: support for INREG key
void set_direction(const TRCDKey &lawKey, TStringFace *direction);
const char *romanian_name(long composition);	// 2012:009 LPR
char kind2LawKind(char reqKind, char type);	// 2017:086
void kind2LawKinds(char reqKind, const char *types, mstr &lawKinds);	// 2017:087
long atobc(TPersonAliasFace *persons[], size_t count);
long suggest_lawsuit_subject(char requestKind);
bool fix_incoming_type(TRCDKeyGroup *incoming, const char *types);
char receiver_ucn_type(char ucnType);	// 2017:244 executors -> citizens
