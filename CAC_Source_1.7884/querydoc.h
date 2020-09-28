void container_events(TDatedContainer *container, TGSArray<TRCDKeyContainer> *array);
void query_events(const TRequest *request, TGSArray<TRCDKeyContainer> *array);
void surround_act(mstr &m, const TSurround *surround);
void query_event(THTMLFile &h, TRCDKeyContainer *container, sign full = false);
void query_event(TRCDKeyContainer *container);
void query_events(const TLawsuit *lawsuit, TGSArray<TRCDKeyContainer> *array);
void query_events(const TInReg *inReg, TGSArray<TRCDKeyContainer> *array);
void query_events(const TProve *prove, TGSArray<TRCDKeyContainer> *array);

void query_group(TWindow *parent, const TRCDKeyGroup *group, bool full = true);
void query_key(TWindow *parent, const TRCDKey &key);	// always full
void query_law(THTMLFile &h, const TLawsuit *lawsuit, bool full = true);
void query_surrouendoc(const TSurroundment *surroundment);
void query_surrouendocs(TWindow *parent, const TSession *session);
void query_show_hide(TWindow *parent, const TRCDKeyGroup *group);
void query_archive(TWindow *parent, TArchive &archive);
