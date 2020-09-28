//{{TDialog = TEditPaymentDialog}}
class TEditPaymentDialog : public TEditDialog
{
public:
	TEditPaymentDialog(TWindow* parent, TPayment *payment, bool edit, int resId = IDD_EDIT_PAYMENT);

protected:
	TCharAliasFace *sumType;
	TCharAliasFace *accountType;
	TMoneyFace *sum;
	TCharAliasFace *type;
	TCharAliasFace *kind;
	TStringAliasFace *receiver;
	TLongFace *no;
	TLongFace *year;
	TEditPaymentListBox *obls;
	TEditPaymentListBox *ucns;
	TStringFace *text;

	bool Self();
	char type_C();
	int LawCount();
	void ProcessKey();

	DEFINE_GROUP_TYPE(TPayment, Payment)
	virtual bool IsValid();

//{{TEditPaymentDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditPaymentDialogVIRTUAL_END}}

//{{TEditPaymentDialogRSP_TBL_BEGIN}}
	void SumTypeCBNSelChange();
	void ReceiverCBNSelChange();
	void ReceiverCBNKillFocus();
	void TypeCBNSelChange();
	void OKBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
	void NoENKillFocus();
	void YearENKillFocus();
	void YearENUpdate();
	void SearchOblBNClicked();
	void SearchUCNBNClicked();
//{{TEditPaymentDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditPaymentDialog);
};    //{{TEditPaymentDialog}}
