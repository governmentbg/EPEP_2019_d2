bool get_date(TWindow *parent, TGroup *group, char *title = NULL, bool mandatory = true, bool future = false);
bool get_year(TWindow *parent, TGroup *group);
bool get_barcode(TWindow *parent, long *court, long *requestId, char *ces);
typedef const char *(*nia_match_func)(TNIAProtocol *protocol);
bool get_nia_protocol(TWindow *parent, TNIAProtocol *protocol, nia_match_func match);
