//{{TThread = TWindowsPrintThread}}
class TWindowsPrintThread
{
public:
	TWindowsPrintThread(TCancelPrintDialog *tDialog);
	virtual ~TWindowsPrintThread();

	bool Init(TWindow* parent, TPrintout *tPrintout, bool tPrompt);
	const TPrintDialog::TData* GetData() const { return (Data); }
	TPrintDC *PrintDC() const { return (prnDC); }

	bool ShouldTerminate() { return !dialog->IsWindow(); }
	void Start() { Run(); }

protected:
	const TPrinterGroup *config;
	TCancelPrintDialog *dialog;

	// from OWL TPrinter
	int Error;
	TPrintDialog::TData* Data;

	bool UseBandInfo;
	uint Flags;
	bool FirstBand;
	TRect BandRect;
	TSize PageSize;
	TSize PageSizeInch;

	void CalcBandingFlags();
	bool ExecPrintDialog(TWindow* parent);

	// thread-specific
	TPrintDC *prnDC;
	TPrintout *printout;
	bool prompt;
	int selFromPage;
	int selToPage;

	virtual int Run();
};
