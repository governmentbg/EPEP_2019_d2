//{{TDialog = TRegisterActDialog}}
class TRegisterActDialog : public TTroubleDialog
{
public:
	TRegisterActDialog(TWindow* parent, TRegisterAct *act, bool edit, int resId = IDD_REGISTER_ACT);

protected:
	char types[SIZE_OF_TYPES];
	TCharAliasFace *type;
	TLongFace *no;
	TYearFace *year;
	TCharAliasFace *kind;
	TDateFace *date;
	mstr kinds;

	DEFINE_GROUP_TYPE(TRegisterAct, Act)
	virtual bool IsValid();

//{{TRegisterActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TRegisterActDialogVIRTUAL_END}}

//{{TRegisterActDialogRSP_TBL_BEGIN}}
	void TypeCBNSelChange();
	void AttachBNClicked();
	void OKBNClicked();
//{{TRegisterActDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRegisterActDialog);
};    //{{TRegisterActDialog}}
