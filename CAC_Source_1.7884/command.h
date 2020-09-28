// ----- matchers ------------------------------------------------------------
const char *request_lawable(TRCDKeyGroup *request, char status);
const char *request_editable(TRCDKeyGroup *, char status);
bool lawsuit_settable_ll(const TLawsuit *lawsuit);  // 2018-05-08
const char *lawsuit_settable(TRCDKeyGroup *group, char);
const char *not_aborted(TRCDKeyGroup *group, char status);
const char *not_departed(TRCDKeyGroup *group, char status);
const char *not_aborted_departed(TRCDKeyGroup *group, char status);
const char *lawsuit_editable(TRCDKeyGroup *, char status);
const char *surroundable(TRCDKeyGroup *group, char status);
const char *not_aborted_present(TRCDKeyGroup *group, char status);
const char *lawsuit_archable(TRCDKeyGroup *group, char status);
const char *schedulable(TRCDKeyGroup *group, char);
const char *indoc_deletable(TRCDKeyGroup *group, char status);
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
const char *jurorable(char status);
const char *jurorable(const TLawsuit *lawsuit, char sessionKind);
const char *jurorable(TRCDKeyGroup *group, char);
#endif  // AREAL || MILITARY || REGIONAL
const char *forcable(TRCDKeyGroup *group, char status);
const char *has_eispp_nmr(TRCDKeyGroup *group, char);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
const char *eispp_nmr_deletable(TRCDKeyGroup *group, char status);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
const char *carried(TRCDKeyGroup *session, char status);
const char *event_editable(TRCDKeyGroup *, char status);
const char *inreg_editable(TRCDKeyGroup *, char status);
const char *not_appealed_act(TRCDKeyGroup *group, char);
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
const char *juror_logged(TRCDKeyGroup *group, char);
#endif  // AREAL || MILITARY || REGIONAL
const char *not_archived_killed(TRCDKeyGroup *group, char);
const char *extern_archable(TRCDKeyGroup *group, char status);
const char *resolution_deletable(TRCDKeyGroup *group, char status);

// ----- command -------------------------------------------------------------
extern char MENU_AUTOMATIC[2];
bool command_enabled(int command, const char *menus);
