//{{TDialog = TEditMppDialog}}
class TEditMppDialog : public TFzlDialog
{
public:
	TEditMppDialog(TWindow *parent, FZL *fzl, const char *nmrnpr, int resId = IDD_EDIT_MPP);

protected:
	MPP *mppre;
	bool hasMppre;

	TEisAliasFace *vid;
	TMoneyFace *rzm;
	TEisAliasFace *ste;
	TDateFace *dta;
	TEisAliasFace *str;

	TEisAliasFace *pre_vid;
	TMoneyFace *pre_rzm;
	TEisAliasFace *pre_ste;
	TDateFace *pre_dta;
	TEisAliasFace *pre_str;

	DEFINE_GROUP_TYPE(MPP, Mpp)
	virtual bool IsValid();
	
//{{TEditMppDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditMppDialogVIRTUAL_END}}

//{{TEditMppDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void VidCBNSelChange();
	void PreVidCBNSelChange();
//{{TEditMppDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditMppDialog);
};  //{{TEditMppDialog}}
