//{{TDialog = TSbeDialog}}
class TSbeDialog : public TEisDialog
{
public:
	TSbeDialog(TWindow* parent, SBE *sbe, TRCDKeyContainer *origin, TGArray<SBE> *sbes, TXArray<FZL> *tFzls,
		TDatedContainer *tContainer, sign edit = -1, int resId = IDD_SBE);

protected:
	TXArray<FZL> *fzls;
	TRCDKeyContainer *origin;
	TDatedContainer *container;

	TEisAliasFace *sbevid;
	TEisAliasFace *sbedkpvid;
	TStringFace *sbedkpnmr;
	TDateFace *sbedkpdta;
	TEisAliasFace *sbedkpstr;
	TEisAliasFace *sbhvid;
	TEisAliasFace *sbhstn;
	TEisAliasFace *srkvid;
	TLongFace *srkden;
	TLongFace *srksdc;
	TLongFace *srkmsc;
	TLongFace *srkgdn;
	TDateFace *srkdta;
	TEisAliasFace *dlosig;
	TStringFace *dlonmr;
	TYearFace *dlogdn;
	TEisAliasFace *dlostr;
	TEisAliasFace *dloncnone;
	TEisAliasFace *dlosprvid;
	TEisAliasFace *dlosts;
	TDateFace *dlodtasts;
	TEisAliasFace *dvjvid;
	TDateFace *dvjdta;
	TEisAliasFace *dvjprc;
	TStatic *selectLabel;
	TConnectComboBox *selectConnect;
	TFzlComboBox *selectFzl;
	TStatic *sbedkpdtaLabel;	// 2014:295 IRQ/TRQ: NSLS new spec +related

	DEFINE_GROUP_TYPE(SBE, Sbe)
	virtual bool IsValid();

//{{TSbeDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSbeDialogVIRTUAL_END}}

//{{TSbeDialogRSP_TBL_BEGIN}}
	void SelectCBNSelChange();
	void OKBNClicked();
//{{TSbeDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSbeDialog);
};    //{{TSbeDialog}}
