//{{TDialog = TPosTermWaitDialog}}
class TPosTermWaitDialog : public TScaleDialog
{
public:
	TPosTermWaitDialog(TWindow* parent, int resId = IDD_POSTERM_WAIT);

	void SetText(const char *tText);
	void AllowCancel() { allowCancel = true; }
	bool Canceled();

protected:
	TStatic *text;
	bool allowCancel;
	bool canceled;

	void PumpWaitingMessages();
};    //{{TPosTermWaitDialog}}
