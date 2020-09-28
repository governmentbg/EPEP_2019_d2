//{{TDialog = TSelectStringDialog}}
class TSelectStringDialog : public TQuickDialog
{
public:
	TSelectStringDialog(TWindow* parent, TStringArray *tStrings, const char *tTitle, const char *tLabel, int &tChoice,
		int resId = IDD_SELECT_STRING);

protected:
	TStringArray *strings;
	const char *title;
	const char *label;
	int &choice;
	TStatic *listLabel;
	TStringListBox *list;

	void SelectEnable(TCommandEnabler &tce);

//{{TSelectStringDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectStringDialogVIRTUAL_END}}

//{{TSelectStringDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TSelectStringDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectStringDialog);
};    //{{TSelectDialog}}
