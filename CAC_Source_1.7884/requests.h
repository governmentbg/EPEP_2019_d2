bool edit_request(TWindow *parent, TRequest *request);
bool edit_request_resolution(TWindow *parent, TRequestResolution *resolution);
void new_request(TWindow *parent, const char *kinds, char type);
void new_request_resolution(TWindow *parent, TRequest *request);
void revert_source(TLawsuit *lawsuit);
void subvert_source(const TRequest *request, TLawsuit *lawsuit);	// 2006:069
bool renumber_request(TWindow *parent, TRequest *source);
