//{{TDialog = TEditConnectDialog}}
class TEditConnectDialog : public TGroupDialog
{
public:
	TEditConnectDialog(TWindow* parent, TConnect *connect, const char *connectTypes, char tType, bool xfer, 
		int resId = IDD_EDIT_CONNECT);

protected:
	TCharAliasFace *connectType;
	TConnectNoFace *connectNo;
	TYearFace *connectYear;
	char senderTypes[2];
	TReceiverAliasFace *sender;
	TConnectKindAliasFace *connectKind;
	TCompositionAliasFace *composition;
	TCharAliasFace *decisionKind;
	TDateFace *decisionDate;
	TYearFace *outYear;
	char type;	// 2010:274 +related
	TXferKindAliasFace *xferKind;
	bool edit;	// 2012:298 +related

	void ConnectTypeChanged(bool setDefaultKind);

	DEFINE_GROUP_TYPE(TConnect, Connect)
	virtual bool IsValid();

//{{TEditConnectDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditConnectDialogVIRTUAL_END}}

//{{TEditConnectDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ConnectTypeCBNSelChange();
	void ConnectKindCBNSelChange();
	void ConnectNoKillFocus();
	void SenderCBNSelChange();
	void XferKindCBNSelChange();
//{{TEditConnectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditConnectDialog);
};    //{{TEditConnectDialog}}
