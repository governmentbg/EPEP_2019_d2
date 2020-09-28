//{{TDialog = TEditSessionPriceDialog}}
class TEditSessionPriceDialog : public TEditDialog
{
public:
	TEditSessionPriceDialog(TWindow* parent, TSessionPrice *tSessionPrice, bool edit, int resId = IDD_EDIT_SESSION_PRICE);

protected:
	TDateFace *date;

//{{TEditSessionPriceDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditSessionPriceDialogVIRTUAL_END}}
};    //{{TEditSessionPriceDialog}}
