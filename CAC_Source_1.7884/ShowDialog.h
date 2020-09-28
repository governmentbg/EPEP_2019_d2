//{{TDialog = TShowDialog}}
class TShowDialog : public TResizeDialog
{
public:
	TShowDialog(TWindow* parent, int showMax, int resId);

	static void Cleanup();
	static int GetCount();
	static void CloseAll();
	static bool FindWindow(HANDLE hWnd);

protected:
	static TIArray<TShowDialog> Dialogs;

	bool cleanedUp;

//{{TShowDialogVIRTUAL_BEGIN}}
public:
	virtual void SetupWindow();
	virtual void CleanupWindow();
//{{TShowDialogVIRTUAL_END}}
};    //{{TShowDialog}}
