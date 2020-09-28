#if COURT_TYPE == COURT_AREAL
void import_reregistered(TWindow *parent);
#endif  // AREAL
void import_known(TWindow *parent);
void import_informed(TWindow *parent, const char *ucn);
#if COURT_TYPE == COURT_AREAL
void import_bankrupts(TWindow *parent, const char *creditor, TGArray<TBankrupt> *bankrupts); 
#endif  // AREAL
long import_announces(TWindow *parent, const CDate &date, TGroupArray *deliverers);
void import_personal_no(TWindow *parent);  // 2018-01-12
bool import_subpoenas(TWindow *parent);
