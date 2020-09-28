//{{TDialog = TEditUserDialog}}
class TEditUserDialog : public TGroupDialog
{
public:
	TEditUserDialog(TWindow* parent, TUser *user, int resId = IDD_EDIT_USER);

protected:
	TStringFace *name;
	TStringFace *login;
	TStringFace *password;
	TStringFace *rePassword;

	TCharListFace *menus;
	TCharListFace *functions;
	TCharListFace *colleges;
	TCharListFace *compositions;

	TCharAliasFace *post;
	TButton *edit;
	TLongFace *failogs;
	TLongAliasFace *compos;
	TCheckFace *domain;

	DEFINE_GROUP_TYPE(TUser, User)
	virtual bool IsValid();

//{{TEditUserDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditUserDialogVIRTUAL_END}}

//{{TEditUserDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PostCBNSelChange();
	void ClearBNClicked();
	void ClearBNEnable(TCommandEnabler &tce);
	void DomainBNClicked();
//{{TEditUserDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditUserDialog);
};    //{{TEditUserDialog}}
