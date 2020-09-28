bool lawsuit_blocked_limited(const TLawsuit *lawsuit);
bool eportal_check_send(TWindow *parent, const TLawsuit *lawsuit, long eportalMode);
void eportal_send(TWindow *parent, const TSideWind *sideWind, const TLawsuit *lawsuit, char oper);
void eportal_send(TWindow *parent, const TGArray<TSideWind> *sideWinds, const TLawsuit *lawsuit, char oper);
void eportal_send(const TRequest *request, char oper);
void eportal_send(TWindow *parent, const TLawsuit *lawsuit, char oper);
void eportal_send(TWindow *parent, const TSession *session, const TLawsuit *lawsuit, long what, char oper);
void eportal_send(TWindow *parent, const TDecision *decision, const TLawsuit *lawsuit, char oper);
void eportal_send(TWindow *parent, const TInReg *inReg, TRCDKeyGroup *group, char oper, long eportalSendable);
void eportal_send(TWindow *parent, const TOutReg *outReg, TRCDKeyGroup *group, char oper);
bool eportal_side_assigned(const TRCDKey &lawsuit, char ucnType, const char *ucn, char involvement);
bool eportal_side_assigned(const TBaseWind *sideWind);
bool eportal_invoke_granted(const TRCDKey &lawsuit, char ucnType, const char *ucn, char involvement);
void eportal_send(const TSubpoena *subpoena, const TLawsuit *lawsuit, char oper, const char *receiver);
void eportal_delete(const TElectric *electric, const TLawsuit *lawsuit, const TRCDKeyContainer *container);
bool eportal_guid(const TUCNGroup *ucnGroup, char involvement);
void eportal_send(const TUCNGroup *ucnGroup, char involvement);
void eportal_send(const TEPortAssign *assign, const TSideWind *sideWind, char oper, long what);
void eportal_send(TWindow *parent, const TSurround *surround, const TLawsuit *lawsuit, char oper);
const char *not_in_progress(TRCDKeyGroup *lawsuit, char);
void eportal_send_unlock(TWindow *parent, TLawsuit *lawsuit);
void eportal_delete_lock(TWindow *parent, TLawsuit *lawsuit);
void eportal_delete_movement(const TLawsuit *lawsuit);         // 2018-10-09: LPR: +related
bool eportal_delete_limited(const TLawsuit *lawsuit);          // 2018-10-23: LPR: +related
void eportal_query(TWindow *parent);
bool eportal_delete_update(const TIndexContainer *container, long what);  // 2018-10-23: LPR: +related
bool eportal_delete_public_file(const TSession *session, long what);      // 2018-11-15: LPR: +related
void eportal_query_access(TWindow *parent, const TTransGroup *ucnGroup);  // 2018-10-24
void eportal_query_access(TWindow *parent, const TLawsuit *lawsuit);      // 2018-10-24
bool citizen_in_progress(TUCNGroup *ucnGroup);                            // 2018-11-22
void eportal_renumber_guid(TUCNGroup *source, TUCNGroup *target);         // 2018-11-23
