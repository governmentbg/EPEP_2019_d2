//{{TDialog = TUserListDialog}}
class TUserListDialog : public TGroupListDialog
{
public:
	TUserListDialog(TWindow* parent, int resId = IDD_USER_LIST);

protected:
	TCharAliasFace *post;
	TUserListWindow *users;

//{{TUserListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TUserListDialogVIRTUAL_END}}

//{{TUserListDialogRSP_TBL_BEGIN}}
	void PostCBNSelChange();
	void DeleteBNEnable(TCommandEnabler &tce);
	void CopyBNClicked();
	void RightBNClicked();
	void RegionBNClicked();
	void RegionBNEnable(TCommandEnabler &tce);
	// 2016:026 LPR/LRQ: +related: block distrib
	//void AssignBNClicked();
	//void AssignBNEnable(TCommandEnabler &tce);
	void ListBNClicked();
	void QueryBNClicked();
//{{TUserListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TUserListDialog);
};    //{{TUserListDialog}}
