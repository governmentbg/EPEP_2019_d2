//{{TDialog = TCancelPrintDialog}}
class TCancelPrintDialog : public TScaleDialog
{
public:
	TCancelPrintDialog(TWindow* parent, const char *tTitle, int resId = IDD_CANCEL_PRINT);

	void SetPageCount(int tCount) { count = tCount; }
	void SetPageNumber(int page);
	void SetPrinterName(const char *name);

protected:
	int count;
	const char *title;

	TStatic *pages;
	TStatic *name;
	TStatic *printer;

//{{TCancelPrintDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TCancelPrintDialogVIRTUAL_END}}
};    //{{TCancelPrintDialog}}
