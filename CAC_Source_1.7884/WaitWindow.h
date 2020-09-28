// ----- TWaitWindow ---------------------------------------------------------
class TWaitDialog;

class TWaitWindow
{
public:
	TWaitWindow(TWindow* parent, const char *text);
	TWaitWindow(TWindow* parent, int resId);
	~TWaitWindow();

	void SetText(const char *text);
	void TickText(const char *text);
	void TickCount(int count, const char *suffix);
	void ShowWindow(int nCmdShow);
	void AllowCancel(bool show);
	bool Canceled();

	static int ComputeResId(const char *text);
	void ResetForeground();		// 2016:347 official +related
	TWindow *GetParent();		// 2017:198
	void DestroyWait();		// 2017:198

protected:
	void Init(TWindow *parent, const char *text, int resId);

	TWaitDialog *wait;
	DWORD last;
	DWORD fresh;
#if 20180914
	HWND active;
#else  // 20180914
	HWND prevActive;
	bool prevLocked;
#endif  // 20180914
};

// ----- generic -------------------------------------------------------------
double scale_wait_compute(const char *text);
void scale_wait_dialog(TNTDialog *dialog, TStatic *text, double factor);
