//{{TDialog = TEditAdrDialog}}
class TEditAdrDialog : public TEisDialog
{
public:
	TEditAdrDialog(TWindow* parent, PNE *pne, const char *nmrnpr, int resId = IDD_EDIT_ADR);

protected:
	ADR *adr;
	TStringFace *pnenmr;
	TEisAliasFace *tip;
	TEisAliasFace *drj;
	TEisAreaAliasFace *area;
	TEisRegionAliasFace *region;
	TFindEisPlaceAliasFace *nsmbgr;
	TStringFace *nsmchj;
	TEisAliasFace *rjn;
	TLongFace *pstkod;
	TLongFace *krdkod;
	TStringFace *krdtxt;
	TStringFace *nmr;
	TStringFace *blk;
	TStringFace *vhd;
	TStringFace *etj;
	TStringFace *apr;
	TEisAliasFace *mstvid;
	TStringFace *msttxt;
	TLongFace *kodpdl;
	TEisAliasFace *loc;

	DEFINE_GROUP_TYPE(PNE, Pne)
	virtual bool IsValid();

//{{TEditAdrDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditAdrDialogVIRTUAL_END}}

//{{TEditAdrDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void DrjCBNSelChange();
	void AreaCBNSelChange();
	void RegionCBNSelChange();
//{{TEditAdrDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditAdrDialog);
};  //{{TEditAdrDialog}}
