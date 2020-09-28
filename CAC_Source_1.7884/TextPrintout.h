//{{TPrintout = TTextPrintout}}
class TTextPrintout : public TPrintout
{
public:
	TTextPrintout(const char* title, const char *tText, const TPrinterGroup *tConfig, int tPages, int part, const TWindowsPrintThread *tPrinter, TCancelPrintDialog *tDialog);
	virtual ~TTextPrintout();

protected:
	unsigned pages;
	const char **start;
	int points;
	int maxcols;
	int alignment;

	// 2007:074 make these members
	const char *text;
	TBrush brush;

	int skip;
	int push;
	int pull;

	void TextOut(TRect *band, int x, int y, const char *s, int len);
	void FillRect(TRect *band, int x1, int y1, int x2, int y2);

	const TPrinterGroup *config;
	bool left;
	bool right;
	bool split;
	bool warned;

	// 2007:115; 2007:116
	const TWindowsPrintThread *printer;
	TFont *standard;
	TFont *bold, *underline, *large;

	// 2007:123
	TDialog *dialog;
	void error(const char *format, ...);	// hide parent while error

//{{TTextPrintoutVIRTUAL_BEGIN}}
public:
	virtual void PrintPage(int page, TRect& rect, uint devmode);
	virtual bool HasPage(int pageNumber);
	virtual void GetDialogInfo(int& minPage, int& maxPage, int& selFromPage, int& selToPage);
//{{TTextPrintoutVIRTUAL_END}}
};    //{{TTextPrintout}}
