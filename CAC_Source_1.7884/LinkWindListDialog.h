//{{TDialog = TLinkWindListDialog}}
class TLinkWindListDialog : public TGroupListDialog
{
public:
	TLinkWindListDialog(TWindow* parent, TLinkWindArray *linkWinds, TGArray<TSideWind> *ordinary,
		TGArray<TSideWind> *proxy, TGArray<TSideWind> *redirect, int resId = IDD_LINK_WIND_LIST);

protected:
	TLinkWindListWindow *linkWinds;
	TStatic *counter;

	void UpdateCounter();

//{{TLinkWindListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLinkWindListDialogVIRTUAL_END}}

//{{TLinkWindListDialogRSP_TBL_BEGIN}}
	void AddBNClicked();
	void EditBNClicked();
	void DeleteBNClicked();
//{{TLinkWindListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLinkWindListDialog);
};    //{{TLinkWindListDialog}}
