const char *match_null(TRCDKeyGroup *, char);	// == NULL
void compute_kinds(const char type, const char *kindsMax, char *kinds);
extern int repeat_func;
extern TRCDKeyGroup *_primary_group;
#define PrimaryGroup(type) TYPECHECK_PTR_CAST(type, _primary_group)
void SetPrimaryGroup(TRCDKeyGroup *primary_group);
extern char PrimaryTitle[0x100];
extern int PrimaryCommand;
extern const char *wrong_status;

TRCDKeyGroup *get_group(TWindow *parent, TRCDKeyGroup *group, const char *types, const char *kinds, const char *statuses,
	match_func match, int getWhat = 0);
bool get_group(TWindow *parent, TRCDKeyGroup *group, const char *types, const char *kinds, const char *statuses);
bool get_group(TWindow *parent, TRCDKeyGroup *group, const char *types, const char *kinds);	// statuses = NULL
TRCDKeyGroup *get_group(TWindow *parent, TRCDKeyGroup *group, const char *types, const char *kinds, match_func match,
	int getWhat = 0);
TRCDKeyContainer *get_session_resolution(TWindow *parent, TSession *session, const char *resolutions, const char *label,
	bool sessionSelected);
void query_archive(TWindow *parent);
#if COURT_TYPE == COURT_ADMIN
bool get_create_request(TWindow *parent, TRequest *request);
#endif  // ADMIN
void appeal_surrouendocs(TWindow *parent);
bool get_eportal_assign(TWindow* parent, TEPortAssign *assign, long eportalMode, const TSideWind *sideWind);
bool get_move_judicial_act(TWindow* parent, TDecision &source, TSession &session);
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
TBaseSession *get_juror_distrib(TWindow* parent, sign edit, const TExternSession *suggest, bool forceExcess);
#endif  // AREAL || MILITARY || REGIONAL
TExternContainer *get_extern_container(TWindow* parent, match_func match, const TExternSession *suggest, bool forceExcess,
	int getWhat = 0);
TExternContainer *get_closed_lawsuit(TWindow* parent, CDate *sessionDate, const TExternSession *suggest, bool forceExcess);
