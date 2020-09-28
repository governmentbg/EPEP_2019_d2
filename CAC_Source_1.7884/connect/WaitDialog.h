//{{TDialog = TWaitDialog}}
class TWaitDialog : public TDialog
{
public:
	TWaitDialog(TWindow* parent, TResId resId = IDD_WAIT, TModule* module = 0);
	virtual ~TWaitDialog(void);

	TStatic *text;

	void SetText(const char *tText);
};    //{{TWaitDialog}}

// ----- TWaitWindow ---------------------------------------------------------
class TWaitWindow
{
public:
	TWaitWindow(TWindow* parent, TResId resId = IDD_WAIT, TModule* module = 0);

	void HideWindow(void) { wait.ShowWindow(false); }

protected:
	TWaitDialog wait;
};
