enum
{
	PRINT_SUB_ARBITARY_NAME 	= 0x01,
	PRINT_SUB_FULL_NAME 		= 0x02,
	PRINT_SUB_SHORT_NAME 		= 0x04,
	PRINT_SUB_ANY_NAME		= 0x07,
	PRINT_SUB_UCN_BULSTAT 		= 0x08,
	PRINT_SUB_ADDRESS 		= 0x10,
	PRINT_SUB_INVOLVEMENT		= 0x20,
	PRINT_SUB_ALL			= 0x3F
};

void printsub_init();
void printsub_emit_creator(TXMLFile &x, const char *tag);	// 2014:325
void print_side(mstr &m, TSideWind *sideWind, long flags);	// 2010:333 LPR: public
void print_laids(mstr &m, TGroupArray *laids, bool involvement, const char *separator);
void print_notifieds(mstr &m, TSubpoena *subpoena, TGArray<TLinkWind> *pLinks, const char *separator);
//bool print_subpoena(TWindow *parent, const char *name, TSubpoena *subpoena, TRCDKeyContainer *container,
//	TSubpoenaVariantGroup *variant, TSubpoena *sentSubpoena, TOffice &office, char *target);
bool print_subpoena(TNotifying *notifying, const char *name, TSubpoenaVariantGroup *variant, bool edit);
