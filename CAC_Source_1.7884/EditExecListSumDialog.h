//{{TDialog = TEditExecListSumDialog}}
class TEditExecListSumDialog : public TGroupDialog
{
public:
	TEditExecListSumDialog(TWindow* parent, TExecList *execList, TExecListSum *tSum, bool tEdit, int resId);

protected:
	TExecList *execList;
	bool edit;
	char lastSumType;

	TExecListPersonListBox *persons;
	TCharAliasFace *sumType;
	TMoneyFace *sum;
	TStringFace *text;
	TAccountAliasFace *account;
	TStringFace *bankName;
	TStringFace *bankAccount;
	TStringFace *bic;
	TStringAliasFace *currency;

	DEFINE_GROUP_TYPE(TExecListSum, Sum)
	virtual bool IsValid();

//{{TEditExecListSumDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditExecListSumDialogVIRTUAL_END}}

//{{TEditExecListSumDialogRSP_TBL_BEGIN}}
	void SenderCBNSelChange();
	void AddressBNEnable(TCommandEnabler &tce);
	void AddressBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
	void SumTypeCBNSelChange();
//{{TEditExecListSumDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditExecListSumDialog);
};    //{{TEditExecListSumDialog}}
