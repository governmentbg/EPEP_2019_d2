int add_announce_subpoena(TGroupArray *array, TQuery &q, TAnnounce *announce = NULL, bool date = false);
int add_announce_announce(TGroupArray *array, TQuery &q, TAnnounce *announce = NULL, bool date = false);
void internal_subpoena_uclp_lead(msql &m);
bool serve_subpoena(TWindow *parent, TSubpoena *subpoena, TSideWind *sideWind, TRCDKeyContainer *container, char originKind,
	char secondKind, char status, int already);
bool serve_sticker(const TSubpoena *subpoena, TSubpoena *sticker);
void serve_stickers(TSubpoena *subpoena);
bool serve_subpoena(TWindow *parent, TSubpoena *subpoena, sign already);
bool serve_announce(TWindow *parent, TAnnounce *announce);
bool edit_announce(TWindow* parent, TAnnounce *announce, bool edit, TGroupArray *deliverers, const char *kinds, bool scan);
void announce_external(TWindow *parent);
void announce_internal(TWindow *parent);
