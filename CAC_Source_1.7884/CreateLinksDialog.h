//{{TDialog = TCreateLinksDialog}}
class TCreateLinksDialog : public TScaleDialog
{
public:
	TCreateLinksDialog(TWindow* parent, TLinkWindArray *tLinkWinds, TGArray<TSideWind> *tOrdinary,
		TGArray<TSideWind> *tProxy, TGArray<TLinkWind> *tNewLinks, int resId = IDD_CREATE_LINKS);

protected:
	TLinkWindArray *linkWinds;
	TGArray<TLinkWind> *newLinks;
	TSideWindListBox *ordinary;
	TSideWindListBox *proxy;

//{{TCreateLinksDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TCreateLinksDialogVIRTUAL_END}}

//{{TCreateLinksDialogRSP_TBL_BEGIN}}
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
//{{TCreateLinksDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCreateLinksDialog);
};    //{{TCreateLinksDialog}}
