//{{TDialog = TShowHtmlDialog}}
class TShowHtmlDialog : public THtmlBaseDialog
{
public:
	TShowHtmlDialog(TWindow* parent, const char *name, bool remove, int resId = IDD_SHOW_HTML);
	TShowHtmlDialog(TWindow* parent, char *text, int resId = IDD_SHOW_HTML);

protected:
	bool printed;
	void InitControls();

//{{TShowHtmlDialogRSP_TBL_BEGIN}}
	void PrintBNClicked();
	void PrintDirectBNClicked();
	void CancelBNClicked();
	void EvClose();
//{{TShowHtmlDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TShowHtmlDialog);
};    //{{TShowHtmlDialog}}
