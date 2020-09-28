//{{TDialog = TGetRecomposeDialog}}
class TGetRecomposeDialog : public TGroupDialog
{
public:
	TGetRecomposeDialog(TWindow* parent, TLawsuit *lawsuit, int resId = IDD_GET_RECOMPOSE);

protected:
	TLongAliasFace *oldComposition;
	TLongAliasFace *composition;

	DEFINE_GROUP_TYPE(TLawsuit, Lawsuit)

//{{TGetRecomposeDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetRecomposeDialogVIRTUAL_END}}
};    //{{TGetRecomposeDialog}}
