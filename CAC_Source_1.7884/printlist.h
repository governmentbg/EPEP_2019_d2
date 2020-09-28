void print_containers(TGArray<TRCDKeyContainer> *array, const char *stats, const char *tTitle);
void print_ucn_groups(TGroupArray *array, const char *stats, const char *name, const char *tTitle);
void print_side_links(mstr &m, TLinkWind *linkWind, bool useNewLines = false, bool ordinaryName = true,
	TGArray<TLinkWind> *linkWinds = NULL);
void print_senders(TGroupArray *senders, char ucnType);
void query_object_count(TWindow *parent);
