extern constant Q_RESTORE_CONTINUE;
bool delete_subpoena(TSubpoena *subpoena, TGArray<TUCNGroup> *affecteds, const TLawsuit *lawsuit);
void delete_winds(TRCDKeyContainer *container, TGArray<TSideWind> *sideWinds = NULL, bool unlinkOnly = false);
bool delete_incomplete(TRCDKeyContainer *container);
bool delete_surroundment(TSurroundment *surroundment, TLawsuit *lawsuit, sign bank_event);
void delete_archive(TWindow *parent, TArchive *archive, TExternContainer *lawsuit);
bool delete_decision(TDecision *decision, TLawsuit *lawsuit);
void delete_finalization(const TSession *session, const TLawsuit *lawsuit);
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void delete_distrib(const TBaseSession *session);
void delete_distrib(TWindow *parent, const TBaseSession *session);
#endif  // AREAL || MILITARY || REGIONAL
void delete_foreign(TWindow *parent, TForeignAddress *foreignAddress);
void delete_indoc(TWindow *parent, TRCDKeyContainer *container);
void delete_old_names(TWindow *parent, TNamedGroup *oldGroup);
bool delete_inreg_event(TInRegEvent *inRegEvent);
void delete_inreg_event(TWindow *parent, TInRegEvent *inRegEvent);
bool inreg_clean(TIndexContainer *container, const char *with);
bool surround_clean(TIndexContainer *container, const char *with);
void delete_lawsuit_event(TWindow *parent, TLawsuit *lawsuit, TIndexContainer *container);
void delete_mails(TOutReg *outReg);
bool delete_resolution(TRequestResolution *resolution);
void delete_resolution(TWindow *parent, TRequestResolution *resolution);
void delete_outreg_return(TWindow *parent, TOutReg *outReg);
void delete_lawsuit(TWindow *parent, TLawsuit *lawsuit);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void delete_eispp_nmr(TWindow *parent, TOriginContainer *origin);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
void delete_movement(TWindow *parent, TLawsuit *lawsuit);
void delete_movement(TWindow *parent, TMovement *movement);
void delete_outreg(TWindow *parent, TOutReg *outReg);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void delete_prove(TWindow *parent, TProve *prove);
void delete_prove_action(TWindow *parent, TProveAction *proveAction);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
void delete_nia_protocol(TWindow *parent, TNIAProtocol *protocol);
