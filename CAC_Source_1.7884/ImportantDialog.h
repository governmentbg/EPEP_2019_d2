//{{TDialog = TImportantDialog}}
class TImportantDialog : public TGroupDialog
{
public:
	TImportantDialog(TWindow* parent, TLawsuit *lawsuit, bool edit, int resId = IDD_IMPORTANT);

protected:
	TDangerAliasFace *danger;	// 2008:262 disable

	DEFINE_GROUP_TYPE(TLawsuit, Lawsuit)

//{{TImportantDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TImportantDialogVIRTUAL_END}}
DECLARE_RESPONSE_TABLE(TImportantDialog);
};    //{{TImportantDialog}}
