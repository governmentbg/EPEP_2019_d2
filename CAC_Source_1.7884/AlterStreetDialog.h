//{{TDialog = TAlterStreetDialog}}
class TAlterStreetDialog : public TGroupDialog
{
public:
	TAlterStreetDialog(TWindow* parent, TStreet *tSource, TStreet *tTarget, int resId);

protected:
	TStreet *source;
	TStreet *target;

	TAreaAliasFace *area;
	TRegionAliasFace *region;
	TFindUCLPAliasFace *uclp;
	TFindStreetAliasFace *sourceStreet;

	virtual bool IsValid();	// also get data

	virtual void ClearTarget() = 0;
	virtual long TargetValue() = 0;

//{{TAlterStreetDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAlterStreetDialogVIRTUAL_END}}

//{{TAlterStreetDialogRSP_TBL_BEGIN}}
	void AreaCBNSelChange();
	void RegionCBNSelChange();
	void UCLPCBNSelChange();
	// 2004:267 LPR: Super-Duper search
	void SourceStreetCBNDropdown();
	void UCLPCBNDropdown();
	void RegionCBNDropdown();
	void AreaCBNDropdown();
//{{TAlterStreetDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAlterStreetDialog);
};    //{{TAlterStreetDialog}}
