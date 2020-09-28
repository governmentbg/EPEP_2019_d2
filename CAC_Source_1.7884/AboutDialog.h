//{{TDialog = TAboutDialog}}
class TAboutDialog : public TScaleDialog
{
public:
	TAboutDialog(TWindow* parent);

protected:
	TStatic *name;

//{{TAboutDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAboutDialogVIRTUAL_END}}

//{{TAboutDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void QueryBNClicked();
//{{TAboutDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAboutDialog);
};    //{{TAboutDialog}}
