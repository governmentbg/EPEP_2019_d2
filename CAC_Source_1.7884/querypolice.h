#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void query_police(TWindow *parent);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
void query_prosec(TWindow *parent);
#endif  // AREAL || REGIONAL
