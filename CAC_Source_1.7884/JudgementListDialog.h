//{{TDialog = TJudgementListDialog}}
class TJudgementListDialog : public TSideWindListDialog
{
public:
	TJudgementListDialog(TWindow* parent, TDecision *decision, TSession *tSession, TLawsuit *tLawsuit, long sideMode);

protected:
	TSession *session;	// 2005:075 LPR: for headers only
	TButton *extraData;

	DEFINE_SIDES_TYPE(TDecision, Decision)
	TJudgementListWindow *Judgements() { return ((TJudgementListWindow *) sideWinds); }
	const TLawsuit *Lawsuit() { return sideWinds->Lawsuit(); }
	virtual void Header2(mstr &m);
	virtual void Accept();	// 2017:303

	// 2017:311
	TStatic *noPublInet;
	TCheckFace *publInet;
	void UpdatePublInet();
	mstr publINetText;	// 2017:313
	bool alreadyPublic;	// 2017:328
	uint defaultPublic;	// 2017:328
	bool p64Appealed;		// 2017:339

//{{TJudgementListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TJudgementListDialogVIRTUAL_END}}

//{{TJudgementListDialogRSP_TBL_BEGIN}}
	void EnterBNClicked();
	void ExecBNClicked();
	void PrintBNClicked();
	void ListBNClicked();		// 2009:184 LPR: copied from decision
	void RightBNClicked();		// 2009:184 LPR: copied from decision
	void ExtraDataBNClicked();
	void MoneyBNClicked();		// 2006:030 LRQ: copied from decision
	void CancelBNClicked();
	void AddBNClicked();
	void EditBNClicked();
	void DeleteBNClicked();
//{{TJudgementListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TJudgementListDialog);
};    //{{TJudgementListDialog}}
