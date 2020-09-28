//{{TDialog = TRegisterActListDialog}}
class TRegisterActListDialog : public TGroupListDialog
{
public:
	TRegisterActListDialog(TWindow* parent, int resId = IDD_REGISTER_ACT_LIST);

protected:
	bool initialized;
	mstr kinds;
	TCharAliasFace *kind;
	TDateFace *date;
	TRegisterActListWindow *registerActs;

	void UnattachedEnable(TCommandEnabler &tce);

//{{TRegisterActListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TRegisterActListDialogVIRTUAL_END}}

//{{TRegisterActListDialogRSP_TBL_BEGIN}}
	void KindCBNSelChange();
	void DateENUpdate();
	void ViewBNClicked();
//{{TRegisterActListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRegisterActListDialog);
};    //{{TRegisterActListDialog}}
