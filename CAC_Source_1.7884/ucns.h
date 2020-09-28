typedef void (*ucn_action)(TWindow *parent, TFoundGroup *ucnGroup);
bool get_ucn(TWindow *parent, TFoundGroup *ucnGroup, const char *ucnTypes, sign allowAdd, const char *involvements,
	bool active, ucn_action action, bool stat);
// use TTransGroup or exact type for allowAdd = true; 2011:088 LPR: allowAdd -1 = add without address
bool get_ucn(TWindow *parent, TFoundGroup *ucnGroup, const char *ucnTypes, sign allowAdd = false, bool stat = false);
// many ucn types, allow add
bool get_ucn(TWindow *parent, TTransGroup *ucnGroup, const char *ucnTypes, const char *involvements, bool active,
	bool stat = false, sign allowAdd = true);
// loop select only and run action
void repeat_ucn(TWindow *parent, TFoundGroup *ucnGroup, const char *ucnTypes, ucn_action action);
bool add_ucn(TWindow *parent, TUCNGroup *group, bool stat = false);
// damns are always pre-tried, so for them getit means reReg; 2010:020 LPR/LRQ: container; 2011:075 LRQ: full
bool edit_ucn(TWindow *parent, TUCNGroup *group, TRCDKeyContainer *container, bool getit = true, bool full = true,
	bool stat = false);
#if COURT_TYPE == COURT_AREAL
// 2011:109 for the new inconsistence
typedef void (*firm_action)(TWindow *parent, TGArray<TFirm> *firms);
void select_firms(TWindow *parent, firm_action action);
#endif  // AREAL
typedef void (*ucn_multi_action)(TWindow *parent, TGArray<TUCNGroup> *ucnGroups);
void select_ucns(TWindow* parent, const char *ucnTypes, bool allowEmpty, const char *involvements, ucn_multi_action action);
