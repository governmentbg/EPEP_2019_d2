//{{TDialog = TSubjectDistributionDialog}}
class TSubjectDistributionDialog : public TGroupDialog
{
public:
	TSubjectDistributionDialog(TWindow* parent, TSubject *subject, TGArray<TUserAliasGroup> *tUsers);

protected:
	TGArray<TUserAliasGroup> *array;
	TUserListBox *users;

	DEFINE_GROUP_TYPE(TSubject, Subject)

//{{TSubjectDistributionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSubjectDistributionDialogVIRTUAL_END}}

//{{TSubjectDistributionDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TSubjectDistributionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSubjectDistributionDialog);
};    //{{TSubjectDistributionDialog}}
