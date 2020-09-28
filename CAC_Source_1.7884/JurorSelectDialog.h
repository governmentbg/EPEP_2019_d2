//{{TDialog = TJurorSelectDialog}}
class TJurorSelectDialog : public TJurorMotivesDialog
{
public:
	TJurorSelectDialog(TWindow* parent, char *motives, const char *tName, char *tUCN, TGArray<TDistribJuror> *tJurors,
		int resId = IDD_JUROR_SELECT);

protected:
	TStatic *label;
	TSelectJurorListBox *jurors;
	const char *name;
	char *ucn;

//{{TJurorSelectDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TJurorSelectDialogVIRTUAL_END}}

//{{TJurorSelectDialogRSP_TBL_BEGIN}}
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
//{{TJurorSelectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TJurorSelectDialog);
};    //{{TJurorSelectDialog}}
