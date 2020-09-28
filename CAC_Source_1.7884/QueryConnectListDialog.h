//{{TDialog = TQueryConnectListDialog}}
class TQueryConnectListDialog : public TQuickDialog
{
public:
	TQueryConnectListDialog(TWindow* parent, TConnect *connect, int resId = IDD_QUERY_CONNECT_LIST);

protected:
	TQueryConnectListBox *connects;

//{{TQueryConnectListDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
//{{TQueryConnectListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQueryConnectListDialog);
};    //{{TQueryConnectListDialog}}
