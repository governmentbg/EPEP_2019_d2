#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
bool mustbe_personal_data(TDecision *decision, char involvement);
sign maybe_personal_data(TDecision *decision, char involvement);
bool arrest_sidewind(TWindow *parent, TLawsuit *lawsuit, TSideWind *sideWind, bool start);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
void enter_act(TWindow *parent, TSession *session, TLawsuit *lawsuit);
bool edit_act(TWindow *parent, TDecision *decision, TLawsuit *lawsuit);
void edit_sides(TWindow *parent, TDecision *decision, TLawsuit *lawsuit, long sideMode);
void edit_sides(TWindow *parent, TSession *session, TLawsuit *lawsuit);
