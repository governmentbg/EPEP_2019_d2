bool select_kind(TWindow *parent, TRCDKeyContainer *container, const char *kinds);
bool get_connect(TWindow *parent, TConnect *connect);
bool select_subjects(TWindow* parent, TGArray<TSubject> *subjects, const char *kinds, bool saas = false,
	sign tHidden = false);
bool select_old_subjects(TWindow* parent, TGArray<TOldSubject> *oldSubjects, const char *kinds);
bool select_user(TWindow* parent, const char *posts, long flags, TUser *user, const char *exclude);
int select_string(TWindow *parent, TStringArray *strings, const char *title, const char *label,
	int resId = IDD_SELECT_STRING);
TRCDKeyContainer *select_event(TWindow *parent, const TRCDKeyGroup *group, char kind, const CDate *date, const CTime *time,
	const char *statuses, match_func match = match_null, const char *what = NULL);

// for _ only
#define _KIND_V_ALL_SESSIONS '\1'
#define _KIND_V_PREV_SESSIONS '\2'
bool _select_session(TWindow *parent, TSession *session, char kind, const char *statuses);
// 2009:152 LPR: brand-new, NULL container -> query
bool select_container(TWindow *parent, TRCDKeyContainer *container, TUCNGroup *ucnGroup, const char *types,
	const char *kinds, const char *statuses, match_func match);
#if COURT_TYPE == COURT_AREAL
bool p24a1tradealike(TWindow *parent, TUCNGroup *ucnGroup);
bool select_citizenwind(TWindow *parent, TSideWind *sideWind, TLawsuit *lawsuit);
#endif  // AREAL

bool select_uclp(TWindow *parent, TUCLP *uclp);
bool select_street(TWindow *parent, TStreet *street);
// 2011:019 for InRegEventListWindow
bool select_send_inreg(TWindow *parent, TInReg *inReg, TInReg *tInReg, TGArray<TInRegEvent> *already);
