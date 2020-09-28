//{{TDialog = TSelectRegisterActDialog}}
class TSelectRegisterActDialog : public TGroupListDialog
{
public:
	TSelectRegisterActDialog(TWindow* parent, TElectricTrouble *tTrouble, TGArray<TRegisterAct> *tActs,
		mstr &tOthers, int resId = IDD_SELECT_REGISTER_ACT);

protected:
	TElectricTrouble *trouble;
	TSelectRegisterActListWindow *acts;
	mstr &others;

//{{TSelectRegisterActDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ViewBNEnable(TCommandEnabler &tce);
	void ViewBNClicked();
	void OtherBNEnable(TCommandEnabler &tce);
	void OtherBNClicked();
//{{TSelectRegisterActDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectRegisterActDialog);
};    //{{TSelectDialog}}
