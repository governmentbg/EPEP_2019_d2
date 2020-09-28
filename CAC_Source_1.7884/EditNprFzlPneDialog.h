//{{TDialog = TEditNprFzlPneDialog}}
class TEditNprFzlPneDialog : public TFzlDialog
{
public:
	TEditNprFzlPneDialog(TWindow* parent, FZL *fzl, NPRFZLPNE *nprfzlpne, TGArray<PNE> *pnes, sign edit, SBE *tSbe,
		TXArray<NPRFZLPNE> *tFreeLinks, int resId = IDD_EDIT_NPRFZLPNE);

protected:
	NPR *npr;
	SBE *sbe;
	TXArray<NPRFZLPNE> *freeLinks;
	NPRFZLPNE prevNprFzlPne;

	TPneAliasFace *pnesid;
	TEisAliasFace *scqvid;
	TEisAliasFace *scqstr;
	TEisAliasFace *scqrlq;
	TEisAliasFace *scqosn;
	// 2012:340 +related
	TEisAliasFace *sbcple;
	TEisAliasFace *sbcrcd;
	TEisAliasFace *sbcspj;
	TEisAliasFace *sbcobr;
	TEisAliasFace *sbctrd;
	TEisAliasFace *sbcznq;
	TEisAliasFace *sbcetn;
	TEisAliasFace *sbcrge;

	DEFINE_GROUP_TYPE(NPRFZLPNE, NprFzlPne)

//{{TEditNprFzlPneDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditNprFzlPneDialogVIRTUAL_END}}

//{{TEditNprFzlPneDialogRSP_TBL_BEGIN}}
	void PneSidCBNSelChange();
	void OKBNClicked();
//{{TEditNprFzlPneDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditNprFzlPneDialog);
};  //{{TEditNprFzlPneDialog}}
