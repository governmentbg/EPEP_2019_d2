// origin lawsuit: origin kind = lawsuit kind, second kind = source kind
// origin request: origin kind = request kind, second kind = target law kind
// other/missing: origin kind = KIND_V_LAWSUIT, second kind = KIND_V_REQUEST
void compute_scheds(mstr &scheds, const TRCDKeyContainer *container, char originKind, char secondKind);
void get_container_kinds(const TRCDKeyContainer *container, char &originKind, char &secondKind, const TRCDKey *tOrigin = NULL);
void compute_scheds(mstr &scheds, const TRCDKeyContainer *container, const TRCDKey *origin = NULL);
void edit_scheds(TWindow *parent, const TRCDKeyContainer *container);
