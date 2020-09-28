#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
#if 20170243
const char *print_protective(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit, const char *name);
#else  // 20170243
void print_protective(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit, const char *name);
#endif  // 20170243
#endif  // AREAL || REGIONAL
#if COURT_TYPE == COURT_REGIONAL
#if 20170243
const char *print_executive(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit);
#else  // 20170243
void print_executive(TWindow *parent, TDecision *decision, TSession *session, TLawsuit *lawsuit);
#endif  // 20170243
#endif  // REGIONAL
