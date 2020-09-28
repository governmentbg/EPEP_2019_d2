//{{TDialog = TFloatSubpoenaDialog}}
class TFloatSubpoenaDialog : public TFloatDialog
{
public:
	TFloatSubpoenaDialog(TWindow* parent, TFloatSubpoenaGroup *group, int resId = IDD_FLOAT_SUBPOENA);

protected:
	TTypeListFace *types;
	TCharListFace *lawKinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TCharListFace *kinds;
	TDateFace *minDate;
	TDateFace *maxDate;
	TDateFace *minSubpoenaDate;
	TDateFace *maxSubpoenaDate;
	TCharListFace *subpoenaKinds;
	TCharListFace *receivedStatuses;
	TUserListBox *deliverers;
	TCheckFace *firmLawsuit;
	TDateFace *minReturnedDate;
	TDateFace *maxReturnedDate;
	TUserListBox *users;

	DEFINE_GROUP_TYPE(TFloatSubpoenaGroup, FloatSubpoenaGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TFloatSubpoenaDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFloatSubpoenaDialogVIRTUAL_END}}

//{{TFloatSubpoenaDialogRSP_TBL_BEGIN}}
	void FirmLawsuitBNClicked();
	void TypesSELChange();
//{{TFloatSubpoenaDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatSubpoenaDialog);
};    //{{TFloatSubpoenaDialog}}
