//{{TDialog = TUserDistributionDialog}}
class TUserDistributionDialog : public TSelectSubjectsDialog
{
public:
	TUserDistributionDialog(TWindow* parent, TUser *user, TGArray<TSubject> *subjects);

protected:
	// 2007:183 +related: handle all user distribution data
	TLongFace *percent;
	TLongFace *preload;
	TLongFace *fullLoad;	// 2007:256 double value -> long fullLoad

	DEFINE_GROUP_TYPE(TUser, User)
	virtual bool IsValid();

//{{TUserDistributionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TUserDistributionDialogVIRTUAL_END}}

//{{TUserDistributionDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void EquateBNClicked();
//{{TUserDistributionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TUserDistributionDialog);
};    //{{TUserDistributionDialog}}
