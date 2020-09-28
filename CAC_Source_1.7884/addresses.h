void view_addresses(TWindow *parent, TUCNGroup *ucnGroup);
void edit_addresses(TWindow *parent, TUCNGroup *ucnGroup, int addrMode);
TAnyAddress *get_address(TWindow *parent, TUCNGroup *ucnGroup, int addrMode);			// loaded ucnGroup
TAnyAddress *get_address(TWindow *parent, const char *ucn, char ucnType, int addrMode);
bool get_address(TWindow *parent, TSubpoena &subpoena, TUCNGroup *ucnGroup, int addrMode);
bool get_address(TWindow *parent, const char *ucn, char ucnType, TAddress *address);	// DEPRECATED
bool edit_address(TWindow *parent, TUCNGroup *ucnGroup); // add/edit 1st addr
bool print_exact_address(mstr &m, TUCNGroup *ucnGroup, long address, bool foreign, long flags = 0);
void print_address(mstr &m, TUCNGroup *ucnGroup, long address = 1, long flags = 0);
void print_addresses(mstr &m, TUCNGroup *ucnGroup, const char *sep, long flags = 0);
