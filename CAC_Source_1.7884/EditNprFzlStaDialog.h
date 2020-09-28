//{{TDialog = TEditNprFzlStaDialog}}
class TEditNprFzlStaDialog : public TFzlDialog
{
public:
	TEditNprFzlStaDialog(TWindow *parent, FZL *fzl, NPR *npr, int resId = IDD_EDIT_NPRFZLSTA);

protected:
	TEisAliasFace *osn;
	TDateFace *dta;
	TEisAliasFace *kcv;
	TEisAliasFace *sts;
};    //{{TEditNprFzlStaDialog}}
