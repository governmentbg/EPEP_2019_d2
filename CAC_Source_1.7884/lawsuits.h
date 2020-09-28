bool edit_lawsuit(TWindow *parent, TLawsuit *lawsuit);
void new_lawsuit(TWindow *parent, TRequest *request);
void next_new_lawsuit(TWindow *parent, TRCDKeyContainer *container);
void new_virtual(TWindow *parent, const char *predefined);
bool destroy_lawsuit(TLawsuit *lawsuit);
void enter_reregister_date(TWindow *parent, TDamn *damn, TLawsuit *lawsuit);	// 2008:039
void divert_source(TLawsuit *lawsuit, CDate *sourceDate);	// 2008:010
void finished_judge(TLawsuit *lawsuit, TSession *session);	// 2008:246
bool renumber_lawsuit(TWindow *parent, TLawsuit *source);
