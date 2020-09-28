//{{TDialog = TEditFzlDialog}}
class TEditFzlDialog : public TEisDialog
{
public:
	TEditFzlDialog(TWindow* parent, FZL *fzl, bool edit, bool tFull, const char *nmrnpr, int resId = IDD_EDIT_FZL);

protected:
	bool full;
	TEisNmrFace *nprnmr;
	TStringFace *egn;
	TStringFace *lnc;
	TEisAliasFace *pol;
	TDateFace *dtarjd;
	TDateFace *dtapnl;
	TFzlNameFace *ime;
	TFzlNameFace *prz;
	TFzlNameFace *fma;
	TFzlNameFace *imecyr;
	TFzlNameFace *imelat;
	TEisAliasFace *grjbgr;
	TEisAliasFace *grjchj;

	virtual bool IsValid();
	DEFINE_GROUP_TYPE(FZL, Fzl)

//{{TEditFzlDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditFzlDialogVIRTUAL_END}}

//{{TEditFzlDialogRSP_TBL_BEGIN}}
	void EgnKillFocus();
	void ImeCyrSetFocus();
	void ImeLatSetFocus();
	void GrjBgrSelChange();
	void OKBNClicked();
//{{TEditFzlDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditFzlDialog);
};    //{{TEditFzlDialog}}
