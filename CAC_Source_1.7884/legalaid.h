void subpoena_legal_name(char *name, char *tRequestId, const TRCDKey *key, char status, long requestId,
	const char *subdir = NULL);
void legalaid_rename(TSubpoena *subpoena, char oldStatus, char newStatus);
void legalaid_import(TWindow *parent);
void create_lawer_college(TFirm *firm);
const char *legal_aid_status(TSubpoena *subpoena);
