// ----- TProcess ------------------------------------------------------------
class TProcess : public TCounterGroup
{
public:
	TProcess(TWindow *tParent, const char *what, const char *tWait);
	virtual ~TProcess();

	void Debug();
	void dprintf(const char *format, ...);

	TWaitWindow *wait;
	TWindow *parent;

	void Display();

protected:
	TTextFile tf;
	FILE *fd;
	const char *fdwhat;
	int dccnt;

	void vdprintf(const char *format, va_list ap);
};
