//{{TDialog = TResourceDialog}}
class TResourceDialog : public TNTDialog
{
public:
	TResourceDialog(TWindow* parent, sign scale, int resId);
	virtual ~TResourceDialog();

	const int dlgId;			// resId as value (== Attr.Name)

protected:
	long fontScale;
	DLGTEMPLATE *dlgTempl;	// delete and assign NULL to avoid scaling
	TFontScale dlgScale;
	bool dlgMsbmp;		// starndard "MS Sans Serif" bitmap font

	void SetDefault(TButton *unset, TButton *set);

//{{TResourceDialogVIRTUAL_BEGIN}}
	virtual HWND DoCreate();
	virtual int DoExecute();
	virtual void SetupWindow();
//{{TResourceDialogVIRTUAL_END}}
};    //{{TResourceDialog}}
