//{{TDialog = TQueryConnectDialog}}
class TQueryConnectDialog : public TGroupDialog
{
public:
	TQueryConnectDialog(TWindow* parent, TConnect *connect, int resId = IDD_QUERY_CONNECT);

protected:
	TCharAliasFace *connectType;
	TConnectNoFace *connectNo;
	TYearFace *connectYear;
	char senderTypes[2];
	TReceiverAliasFace *sender;
	TCharAliasFace *connectKind;
	char connectKinds[SIZE_OF_CONNECT_KINDS];

	DEFINE_GROUP_TYPE(TConnect, Connect)
	virtual bool IsValid();

//{{TQueryConnectDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TQueryConnectDialogVIRTUAL_END}}

//{{TQueryConnectDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ConnectTypeCBNSelChange();
	void ConnectKindCBNSelChange();
//{{TQueryConnectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQueryConnectDialog);
};    //{{TQueryConnectDialog}}
