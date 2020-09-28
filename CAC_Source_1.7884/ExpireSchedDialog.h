//{{TDialog = TExpireSchedDialog}}
class TExpireSchedDialog : public TGroupDialog
{
public:
	TExpireSchedDialog(TWindow* parent, TExpireSchedGroup *group, int resId = IDD_EXPIRE_SCHED);

protected:
	TCharListFace *types;
	TUserListBox *judges;

	DEFINE_GROUP_TYPE(TExpireSchedGroup, ExpireSchedGroup)
	virtual bool IsValid();

//{{TExpireSchedDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TExpireSchedDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TExpireSchedDialog);
};    //{{TExpireSchedDialog}}
