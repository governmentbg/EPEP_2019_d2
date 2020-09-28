//{{TDialog = TDecisionSideListDialog}}
class TDecisionSideListDialog : public TSideWindListDialog
{
public:
	TDecisionSideListDialog(TWindow* parent, TIndexContainer *container, TSession *tSession, TLawsuit *lawsuit,
		long sideMode, int resId = IDD_DECISION_SIDE_LIST);

protected:
	TSession *session;
	TButton *heritage;

	DEFINE_SIDES_TYPE(TIndexContainer, IndexContainer)
	virtual void Header2(mstr &m);

//{{TDecisionSideListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TDecisionSideListDialogVIRTUAL_END}}

//{{TDecisionSideListDialogRSP_TBL_BEGIN}}
	void MessagesBNClicked();
	void ListBNClicked();
	void RightBNClicked();
	void SchedBNClicked();
	void MoneyBNClicked();
	void HeritageBNEnable(TCommandEnabler &tce);
	void HeritageBNClicked();
//{{TDecisionSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TDecisionSideListDialog);
};    //{{TDecisionSideListDialog}}
