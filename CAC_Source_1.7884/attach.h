void query_unattached(TWindow *parent);
bool attach_write_lock(const TRCDKeyContainer *container, char *name, int type, const memblock_t &data, long mtime);
void attach_view(TWindow *parent, const char *name, int type);	// filter display if Word, otherwise electric view
#define OF_JUDICIAL_ACT "�� ������� ��� "
#define FORMAT_FROM_ACT "�� ����������� ���: %02ld/%d"
void register_acts(TWindow *parent);
void attach_right(TWindow *parent, const TRCDKey &key, TElectricTrouble &trouble, char indexKind, const CDate &date,
	TGArray<TUser> &judges);
void attach_right(TWindow *parent, TElectricTrouble &trouble, char indexKind, const CDate &date, const TSession *session);
void attach_right(TWindow *parent, const TRCDKey &key, TElectricTrouble &trouble, char indexKind, const CDate &date,
	const char *judge);
void float_register_act(TWindow *parent);
void query_register_act(TWindow *parent);
void unfiltered_acts(TWindow *parent);
