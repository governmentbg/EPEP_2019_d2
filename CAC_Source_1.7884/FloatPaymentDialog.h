//{{TDialog = TFloatPaymentDialog}}
class TFloatPaymentDialog : public TFloatDialog
{
public:
	TFloatPaymentDialog(TWindow* parent, TFloatPaymentGroup *group, int resId = IDD_FLOAT_PAYMENT);

protected:
	int personType;
	TCharListFace *sumTypes;
	TStringFace *minTransId;
	TStringFace *maxTransId;
	TDateFace *minDate;
	TDateFace *maxDate;
	TTimeFace *minTime;
	TTimeFace *maxTime;
	TCharListFace *accountTypes;
	TLongFace *ecr;
	TCharListFace *connectKinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TSenderListBox *courts;
	TPushButton *personTypeBN;
	TEditPaymentListBox *persons;
	TStringFace *text;

	DEFINE_GROUP_TYPE(TFloatPaymentGroup, FloatPaymentGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TFloatPaymentVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFloatPaymentVIRTUAL_END}}

//{{TFloatPaymentDialogRSP_TBL_BEGIN}}
	void SearchBNClicked();
	void PersonTypeBNClicked();
//{{TFloatPaymentDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatPaymentDialog);
};    //{{TFloatPaymentDialog}}
