//{{TDialog = TLongTextDialog}}
class TLongTextDialog : public TScaleDialog
{
public:
	TLongTextDialog(TWindow* parent, const char *tText);

protected:
	const char *text;
	TEdit *longText;

//{{TLongTextDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLongTextDialogVIRTUAL_END}}

//{{TLongTextDialogRSP_TBL_BEGIN}}
	void TextSetFocus();
//{{TLongTextDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLongTextDialog);
};    //{{TLongTextDialog}}
