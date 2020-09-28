//{{TDialog = TEditBanConSistDialog}}
class TEditBanConSistDialog : public TEditDialog
{
public:
	TEditBanConSistDialog(TWindow* parent, TBanConSist *tBanConSist, bool edit, bool tActive,
		int resId = IDD_EDIT_BANCONSIST);

protected:
	bool active;
	TLongFace *banConSist;
	TCheckFace *activeFlag;

//{{TEditBanConSistDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditBanConSistDialogVIRTUAL_END}}
};    //{{TEditBanConSistDialog}}
