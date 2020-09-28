//{{TDialog = TEditSLCReasonDialog}}
class TEditSLCReasonDialog : public TEditDialog
{
public:
	TEditSLCReasonDialog(TWindow* parent, TSLCReason *reason, bool edit, int resId = IDD_EDIT_SLC_REASON);

	DEFINE_GROUP_TYPE(TSLCReason, SLCReason)
};    //{{TEditSLCReasonDialog}}
