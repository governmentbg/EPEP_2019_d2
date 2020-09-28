//{{TDialog = TPaymentListDialog}}
class TPaymentListDialog : public TQuickDialog
{
public:
	TPaymentListDialog(TWindow* parent, int resId = IDD_PAYMENT_LIST);

protected:
	TDateFace *date;
	TTransIdFace *transId;
	TLongFace *ecr;
	TPaymentListWindow *payments;
	TButton *search;
	TButton *add, *edit;
	TMoneyFace *total;
	TPayment payment;

	void SetSearch();

//{{TPaymentListDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
	virtual void SetupWindow();
//{{TPaymentListDialogVIRTUAL_END}}

//{{TPaymentListDialogRSP_TBL_BEGIN}}
	bool PaymentsLVNItemChanging(TLwNotify &lwn);
	void DropCBNSelChange();
	void PrevBNClicked();
	void NextBNClicked();
	void FindBNEnable(TCommandEnabler &tce);
	void SearchBNClicked();
	void AddBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void RightBNClicked();
	long EvFocusFace(TParam1 wParam, TParam2);
	void PaymentBNClicked();
	void PaymentBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
	void PrintBNEnable(TCommandEnabler &tce);
	void QueryBNClicked();
	void QueryBNEnable(TCommandEnabler &tce);
	void CancelBNClicked();
	void EvClose();
//{{TPaymentListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPaymentListDialog);
};    //{{TPaymentListDialog}}
