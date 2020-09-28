//{{TDialog = TSessionPriceListDialog}}
class TSessionPriceListDialog : public TGroupListDialog
{
public:
	TSessionPriceListDialog(TWindow* parent, int resId = IDD_SESSION_PRICE_LIST);

protected:
	TSessionPriceListWindow *prices;

//{{TSessionPriceListDialogRSP_TBL_BEGIN}}
	void PrintBNClicked();
//{{TSessionPriceListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSessionPriceListDialog);
};    //{{TSessionPriceListDialog}}
