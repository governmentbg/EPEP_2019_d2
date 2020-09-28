//{{TDialog = TEditRegionDialog}}
class TEditRegionDialog : public TEditDialog
{
public:
	TEditRegionDialog(TWindow* parent, TRegion *tRegion, bool tEdit, int resId = IDD_EDIT_REGION);

protected:
	TAreaAliasFace *area;
	TLongFace *region;

//{{TEditRegionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditRegionDialogVIRTUAL_END}}
};    //{{TEditRegionDialog}}
