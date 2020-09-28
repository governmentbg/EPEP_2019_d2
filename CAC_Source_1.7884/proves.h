#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
const char *origin_provable(TRCDKeyGroup *group, char status);
void new_prove(TWindow *parent, TRCDKeyContainer *container);
bool edit_prove(TWindow *parent, TProve *prove);
void new_prove_action(TWindow *parent, TProve *prove);
bool edit_prove_action(TWindow *parent, TProveAction *proveAction);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
