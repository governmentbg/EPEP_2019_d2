//{{TDialog = TSelectUCNDialog}}
class TSelectUCNDialog : public TSelectUCNBaseDialog
{
public:
	TSelectUCNDialog(TWindow* parent, TFoundGroup *ucnGroup, const char *ucnTypes, bool tAllowAdd,
		const char *involvements, bool active, ucn_action tAction, bool stat);

protected:
	bool allowAdd;
	ucn_action action;	// 2008:022 LPR: repeater support

	void UpdateButtons(bool forceSearch);	// 2008:056 LPR: new search

//{{TSelectUCNDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void GroupsLBNSelChange();
	void SearchBNClicked();
	void AddressBNClicked();
	void AddressBNEnable(TCommandEnabler &tce);
	void ResearchBNClicked();
	void NameENUpdate();		// 2004:168 LPR: artificial: any name
	void UCNENUpdate();
//{{TSelectUCNDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectUCNDialog);
};    //{{TSelectUCNDialog}}
