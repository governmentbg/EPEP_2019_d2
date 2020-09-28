void closed_result(TWindow *parent, TLawsuit *lawsuit, CDate *sessionDate);
bool edit_closed(TWindow *parent, TSession *session, TLawsuit *lawsuit);
bool open_result(TWindow *parent, TSession *session, TLawsuit *lawsuit);
bool reset_session(TWindow *parent, TSession *source, TRCDKeyContainer *resolution, TLawsuit *lawsuit);
bool reset_session(TWindow *parent, TSession *source, TLawsuit *lawsuit);
bool set_session(TWindow *parent, TSession *session, TLawsuit *lawsuit, TLoadGroup *loadGroup, TLoadGroup *invisible);
bool unset_session(TWindow *parent, TSession *session, TLawsuit *lawsuit);
bool edit_set_session(TWindow *parent, TSession *session, TLawsuit *lawsuit);
void edit_surrouendocs(TWindow *parent, TSession *session, TLawsuit *lawsuit);	// 2016:116
bool appeal_surrouendocs(TWindow *parent, TSession *session, TLawsuit *lawsuit);
#if COURT_TYPE == COURT_AREAL
bool edit_extern_open_session(TWindow *parent, TExternSession *session, bool edit);
bool edit_extern_closed_session(TWindow *parent, TExternSession *session, bool edit);
#endif  // AREAL
