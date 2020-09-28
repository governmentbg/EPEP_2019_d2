//{{TDialog = TEisDialog}}
class TEisDialog : public TEditDialog
{
public:
	TEisDialog(TWindow* parent, TXMLGroup *group, sign edit, const char *tNmrNpr, int resId);

protected:
	bool save;
	const char *nmrnpr;
	TStringFace *nprnmr;

	DEFINE_GROUP_TYPE(TXMLGroup, XMLGroup)
	virtual bool IsValid();

	void EnableEis(TWindow *window, bool enable);
	void EnableEis(TWindow *window) { EnableEis(window, true); }
	static bool WrongEis(TWindow *window, void *data);
	static void SetupEis(TWindow *window, void *data);

//{{TEisDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEisDialogVIRTUAL_END}}

//{{TEisDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEisDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEisDialog);
};    //{{TEisDialog}}
