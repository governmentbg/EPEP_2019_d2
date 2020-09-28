//{{TDialog = TShowTextDialog}}
class TShowTextDialog : public TShowDialog
{
public:
	TShowTextDialog(TWindow* parent, char *tText, TPrinterGroup *tConfig, int resId = IDD_SHOW_TEXT);
	virtual ~TShowTextDialog();	// NOTE: deletes tText and tConfig

protected:
	TEdit *edit;
	// 2007:081
	char title[77];		// for easier passing to preview/printouts
	TPrinterGroup *config;
	size_t maxLen;
	char *text;
	bool printed;

	int CountPages();
	void Print(int part);
	void Save(const char *name);

//{{TShowTextDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TShowTextDialogVIRTUAL_END}}

//{{TShowTextDialogRSP_TBL_BEGIN}}
	void PrintBNClicked();
	void LeftBNClicked();
	void RightBNClicked();
	void SaveBNClicked();
	void EditSetFocus();
	void CancelBNClicked();
	void EvClose();
//{{TShowTextDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TShowTextDialog);
};    //{{TShowTextDialog}}
