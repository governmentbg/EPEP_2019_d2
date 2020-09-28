//{{TDialog = TEditNkzDialog}}
class TEditNkzDialog : public TEditDsmgRzmDialog
{
public:
	TEditNkzDialog(TWindow* parent, FZL *fzl, NKZ *nkz, SBE *tSbe, int resId = IDD_EDIT_NKZ);

protected:
	SBE *sbe;
	TEisAliasFace *tip;
	TEisAliasFace *ncn;
	TLongFace *iptsrkden;
	TLongFace *iptsrksdc;
	TLongFace *iptsrkmsc;
	TLongFace *iptsrkgdn;
	TDateFace *iptsrkdta;
	TEisAliasFace *rjm;
	TEisAliasFace *akt;
	TDateFace *aktdta;

	void AdjustDate(TDateFace *date);

	virtual bool IsValid();

//{{TEditNkzDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditNkzDialogVIRTUAL_END}}

//{{TEditNkzDialogRSP_TBL_BEGIN}}
	void NcnCBNSelChange();
//{{TEditNkzDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditNkzDialog);
};    //{{TEditNkzDialog}}
