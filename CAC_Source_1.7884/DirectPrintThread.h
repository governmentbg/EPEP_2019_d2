//{{TThread = TDirectPrintThread}}
class TDirectPrintThread : public TThread
{
public:
	TDirectPrintThread(const char *tText, const TPrinterGroup *tConfig, TCancelPrintDialog *tDialog);
	virtual ~TDirectPrintThread();

	bool Init();

protected:
	const char *text;
	const TPrinterGroup *config;
	TCancelPrintDialog *dialog;
	char lpt[10];
	bool laser;
	HANDLE hPrinter;

	virtual int Run();
};
