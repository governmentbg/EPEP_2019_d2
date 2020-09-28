//{{TDialog = TFloatIndexDialog}}
class TFloatIndexDialog : public TGroupDialog
{
public:
	TFloatIndexDialog(TWindow* parent, TFloatIndexGroup *group, int resId = IDD_FLOAT_INDEX);

protected:
	TTypeListFace *types;
	TCharListFace *decKinds;
	TDateFace *minFromDate;
	TDateFace *maxFromDate;
	TLongFace *minEventNo;
	TLongFace *maxEventNo;

	DEFINE_GROUP_TYPE(TFloatIndexGroup, FloatIndexGroup)
	virtual bool IsValid();

//{{TFloatDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TFloatDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatIndexDialog);
};    //{{TFloatDecisionDialog}}
