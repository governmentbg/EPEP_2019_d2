//{{TDialog = TEditRealityTypeDialog}}
class TEditRealityTypeDialog : public TEditDialog
{
public:
	TEditRealityTypeDialog(TWindow* parent, TRealityType *tRealityType, bool edit, int resId);

protected:
	TLongFace *realityType;

//{{TEditRealityTypeDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditRealityTypeDialogVIRTUAL_END}}
};    //{{TEditRealityTypeDialog}}
