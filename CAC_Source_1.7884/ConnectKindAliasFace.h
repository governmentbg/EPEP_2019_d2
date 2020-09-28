//{{TComboBox = TConnectKindAliasFace}}
class TConnectKindAliasFace : public TCharAliasFace
{
public:
	TConnectKindAliasFace(TWindow* parent, int resourceId, const char *name, char connectType, long flags = FLAG_NULL);

	void SetConnectType(char connectType);
	void SetDefaultKind(char originType);

protected:
	char connectKinds[SIZE_OF_CONNECT_KINDS];

	void SetConnectKinds(char connectType);
};    //{{TConnectKindAliasFace}}
