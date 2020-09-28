//{{TDialog = TBankruptListDialog}}
class TBankruptListDialog : public TGroupListDialog
{
public:
	TBankruptListDialog(TWindow* parent);

protected:
	TSenderAliasFace *creditor;
	TBankruptListWindow *bankrupts;

//{{TBankruptListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TBankruptListDialogVIRTUAL_END}}

//{{TBankruptListDialogRSP_TBL_BEGIN}}
	void CreditorCBNSelChange();
	void QueryBNClicked();
	void QueryBNEnable(TCommandEnabler &tce);
	void RightBNClicked();
	void LoadBNClicked();
//{{TBankruptListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBankruptListDialog);
};    //{{TBankruptListDialog}}
