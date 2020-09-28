//{{TDialog = TSelectUCNMultiDialog}}
class TSelectUCNMultiDialog : public TSelectUCNBaseDialog
{
public:
	TSelectUCNMultiDialog(TWindow* parent, const char *ucnTypes, bool tAllowEmpty, const char *involvements,
		ucn_multi_action tAction);

protected:
	TFoundGroup ucnGroup;
	bool allowEmpty;
	ucn_multi_action action;

	virtual bool UCNAvailable();

//{{TSelectUCNMultiDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
	void GroupsLBNSelChange();
	void SearchBNClicked();
	void AddressBNClicked();
	void AddressBNEnable(TCommandEnabler &tce);
	void ResearchBNClicked();
	void NameENUpdate();		// 2004:168 LPR: artificial: any name
	void UCNENUpdate();
//{{TSelectUCNMultiDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectUCNMultiDialog);
};    //{{TSelectUCNMultiDialog}}
