//{{TDialog = TSelectUserDialog}}
class TSelectUserDialog : public TGroupDialog
{
public:
	TSelectUserDialog(TWindow* parent, const char *posts, long flags, TUser *user, const char *exclude,
		int resId = IDD_SELECT_USER);

protected:
	TUserListBox *users;

//{{TSelectUserDialogRSP_TBL_BEGIN}}
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
//{{TSelectUserDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectUserDialog);
};    //{{TSelectUserDialog}}
