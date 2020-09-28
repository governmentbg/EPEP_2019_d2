//{{TDialog = TChooseRedirectDialog}}
class TChooseRedirectDialog : public TScaleDialog
{
public:
	TChooseRedirectDialog(TWindow* parent, TLinkWind *tLinkWind, TGArray<TSideWind> *tRedirect,
		int resId = IDD_CHOOSE_REDIRECT);

protected:
	TLinkWind *linkWind;
	TStatic *header;
	TSideWindListBox *redirect;

//{{TChooseRedirectDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TChooseRedirectDialogVIRTUAL_END}}

//{{TChooseRedirectDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TChooseRedirectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TChooseRedirectDialog);
};    //{{TChooseRedirectDialog}}
