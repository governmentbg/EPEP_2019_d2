//{{TDialog = THeritageSideListDialog}}
class THeritageSideListDialog : public TDecisionSideListDialog
{
public:
	THeritageSideListDialog(TWindow* parent, TDecision *decision, TSession *session, TLawsuit *lawsuit, long sideMode);

protected:
	TButton *heritage;
	
//{{THeritageSideListDialogRSP_TBL_BEGIN}}
	void HeritageBNEnable(TCommandEnabler &tce);
	void HeritageBNClicked();
//{{THeritageSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(THeritageSideListDialog);
};    //{{THeritageSideListDialog}}
