//{{TDialog = TFzlGroupListDialog}}
class TFzlGroupListDialog : public TGroupListDialog
{
public:
	TFzlGroupListDialog(TWindow* parent, FZL *tFZL, NPR *tNPR, int resId);

protected:
	FZL *fzl;
	NPR *npr;
	TEisNmrFace *nprnmr;
	TStringFace *imecyr;

//{{TFzlGroupListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFzlGroupListDialogVIRTUAL_END}}
};
