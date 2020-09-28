//{{TDialog = TWhateverTextBaseDialog}}
class TWhateverTextBaseDialog : public TGroupDialog
{
public:
	TWhateverTextBaseDialog(TWindow* parent, TGroup *group, const char *tTitle, const char *what, int size, int resId = IDD_WHATEVER_TEXT_BASE);
	TWhateverTextBaseDialog(TWindow* parent, TText *tField, const char *tTitle, const char *what, int size, int resId = IDD_WHATEVER_TEXT_BASE);

protected:
	TStringFace *text;
	const char *title;
	TText *field;

//{{TWhateverTextBaseDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TWhateverTextBaseDialogVIRTUAL_END}}

//{{TWhateverTextBaseDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TWhateverTextBaseDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TWhateverTextBaseDialog);
};    //{{TWhateverTextBaseDialog}}
