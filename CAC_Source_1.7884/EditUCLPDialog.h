//{{TDialog = TEditUCLPDialog}}
class TEditUCLPDialog : public TEditDialog
{
public:
	TEditUCLPDialog(TWindow* parent, TUCLP *tUCLP, bool full, bool edit, int resId = IDD_EDIT_UCLP);

protected:
	TAreaAliasFace *area;
	TRegionAliasFace *region;
	TLongFace *uclp;

	DEFINE_GROUP_TYPE(TUCLP, UCLP)

//{{TEditUCLPDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditUCLPDialogVIRTUAL_END}}

//{{TEditUCLPDialogRSP_TBL_BEGIN}}
	void AreaCBNSelChange();
//{{TEditUCLPDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditUCLPDialog);
};    //{{TEditUCLPDialog}}
