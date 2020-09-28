//{{TDialog = THtmlLoadDialog}}
class THtmlLoadDialog : public THtmlBaseDialog
{
public:
	THtmlLoadDialog(TWindow* parent, TLoadGroup *tLoadGroup, TLoadGroup *tInvisible, int resId = IDD_HTML_LOAD);
	virtual ~THtmlLoadDialog();

protected:
	TLoadGroup *loadGroup;
	TLoadGroup *invisible;
	TButton *variantBN;
	int variant;

	virtual HRESULT CallBack(const char *url);
	void SetSession(TSession &session);
	void UpdateVariantText();
	void SwitchVariant();

//{{THtmlLoadDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{THtmlLoadDialogVIRTUAL_END}}

//{{THtmlLoadDialogRSP_TBL_BEGIN}}
	void SetSessionBNClicked();
	void VariantBNClicked();
//{{THtmlLoadDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(THtmlLoadDialog);
};    //{{THtmlLoadDialog}}
