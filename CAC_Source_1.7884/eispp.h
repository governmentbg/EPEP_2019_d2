bool eispp_ucn_match(char ucnType, long nom);
void initialize_eispp();
long eispp_instance(long dlosprvid);
#if TESTVER
bool eispp_matches(char lawsuitKind, long dlosprvid);  // 2017:319
#endif  // TESTVER
void eispp_accept(TWindow *parent, TRequest *request);
void eispp_accept(TWindow *parent, TInReg *inReg);
void eispp_accept(TWindow *parent, TLawsuit *lawsuit, TSideWindArray *sideWinds);
void eispp_accept(TWindow *parent, TLawsuit *lawsuit, TSession *session, TDecision *decision, bool complain = false);
void eispp_delete(TRCDKeyContainer *container);
void eispp_delete(TPrimeContainer *container);
void eispp_delete(TOutReg *outReg, bool full);
void eispp_edit_lawsuit(TLawsuit *lawsuit);
void eispp_edit_fzlpne(TWindow *parent, TLawsuit *lawsuit);
void eispp_renumber(TLawsuit *source, TLawsuit *targer);
bool eispp_checkout(TOutReg *outReg);
void eispp_send(TWindow *parent, TOutReg *outReg, TInReg *inReg = NULL);
void eispp_receive(TWindow *parent, TOutReg *outReg);
bool eispp_stsakt(TWindow *parent, TOriginContainer *container);
bool eispp_print_pnes(mstr &m, const TSideWind *defendant);
#if TESTVER
long eispp_get_sbevid(long index);
long eispp_get_sbe_index(const char *name);
#endif  // TESTVER
long eispp_get_sbevid(const char *name);
