//{{TDialog = TFzlDialog}}
class TFzlDialog : public TEisDialog
{
public:
	TFzlDialog(TWindow* parent, FZL *tFZL, TXMLGroup *group, sign edit, const char *nmrnpr, int resId);

protected:
	FZL *fzl;
	TStringFace *imecyr;

//{{TFzlDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFzlDialogVIRTUAL_END}}
};  //{{TFzlDialog}}
