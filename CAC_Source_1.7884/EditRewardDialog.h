//{{TDialog = TEditRewardDialog}}
class TEditRewardDialog : public TGroupDialog
{
public:
	TEditRewardDialog(TWindow* parent, TExpense *expense, TIndexContainer *tContainer, TSession *tSession,
		TLawsuit *tLawsuit, bool tEdit, bool tState, int resId = IDD_EDIT_REWARD);

protected:
	bool edit;
	bool state;				// 2003:184 LRQ: allow sumState change
	TCharAliasFace *sumState;
	TStringFace *paidNote;
	TCharAliasFace *accountType;
	TLongAliasFace *reasonType;
	TMoneyFace *sum1, *sum2;	// 2006:017 LPR: for checks
	TIndexContainer *container;	// 2012:180 LRQ: for print
	TSession *session;		// 2009:196 LPR: for print
	TLawsuit *lawsuit;		// 2009:196 LPR: for print
	TPushButton *print;

	DEFINE_GROUP_TYPE(TExpense, Expense)
	virtual bool IsValid();
	void Save(char subpoenaKind);

//{{TEditRewardDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditRewardDialogVIRTUAL_END}}

//{{TEditRewardDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PrintBNClicked();
	void RightBNClicked();
	void AccountTypeCBNSelChange();
//{{TEditRewardDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditRewardDialog);
};    //{{TEditRewardDialog}}
