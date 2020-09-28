void print_rko(TWindow *parent, TExpense *expense, TIndexContainer *container, TSession *session, TLawsuit *lawsuit,
	char subpoenaKind);
void print_side_list(TWindow *parent, TRCDKeyContainer *container, TSideWindArray &sideWinds, TGArray<TLinkWind> &linkWinds,
	bool all);
void print_ucn_short(mstr &m, const char *ucn);

const char *instance_law_kinds(long flags);	// book flags
int session_outcomes(char result);			// 2013:040	
int session_outcomes(TSession *session);
