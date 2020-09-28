//{{TDialog = TEditCurrencyDialog}}
class TEditCurrencyDialog : public TEditDialog
{
public:
	TEditCurrencyDialog(TWindow* parent, TCurrency *tCurrency, bool edit);

protected:
	TStringFace *currency;
	TStringFace *numeric;	

	virtual bool IsValid();

//{{TEditCurrencyDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditCurrencyDialogVIRTUAL_END}}

//{{TEditCurrencyDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditCurrencyDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditCurrencyDialog);
};    //{{TEditCurrencyDialog}}
