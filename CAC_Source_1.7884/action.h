// ----- chain ---------------------------------------------------------------
typedef void (*next_action)(TWindow *parent, TRCDKeyContainer *container);
void chain_action(TWindow *parent, TRCDKeyContainer *container, next_action action);

// ----- edit ----------------------------------------------------------------
void edit_calendar(TWindow *parent);
bool edit_lawer_data(TWindow *parent, TKnownPerson *person, bool eportal);
class TLinkWindArray;
bool edit_sidewind(TWindow *parent, TRCDKeyContainer *container, TSideWind *source, TSideWind *target,
	const char *involvements, bool unwanted, TLinkWindArray *links, char status, sign stat);

// ----- various -------------------------------------------------------------
bool ask_join(TUCNGroup *source, TUCNGroup *target);
bool ask_renumber(TUCNGroup *source, TUCNGroup *target);
bool ask_move(TDecision &source, TSession &session);
void attach_final(TSession *session, const TDecision *decision, const CDate &tReturned, const CDate &finished, bool edit);
void barcode_scan(TWindow *parent);
bool big_sas_lock(TWindow *parent);
inline void big_sas_unlock() { big_sas_lock(NULL); }
bool check_config_dates();
bool check_involvement(TWindow *parent, char originKind, const TSideWind *sideWind);	
bool check_subpoenas(const TRCDKeyContainer *container);
bool child_sidewind(TSideWind *sideWind, long flags);
bool detached_final(TSession *session, const TDecision *decision);
void move_act(TDecision &source, TSession &session);
void move_winds(TRCDKeyContainer *source, TRCDKeyContainer *target, bool override = true);
bool movement(TWindow *parent, TRCDKeyGroup *group);
void print_join_name(mstr &m, TUCNGroup *ucnGroup);
void printer_setup(TWindow *parent, TPrinterGroup *group);
void recompose_law(TLawsuit *source, TLawsuit *target);
void reload_config();
void reload_title(TWindow *parent);
void send_sms(const TRCDKey &key, const char *text, char action, int phase, const TDatedContainer *container);
inline void send_sms(const TRCDKeyGroup *group, const char *text, char action, int phase, const TDatedContainer *container)
	{ send_sms(group->key, text, action, phase, container); }
bool update_sidewind(TSideWind *source, TSideWind *target, TGArray<TLinkWind> *links);

// ----- underscore actions --------------------------------------------------
void print_abc_book(TWindow *parent);
void print_desc_book(TWindow *parent);
void print_time_book(TWindow *parent);
