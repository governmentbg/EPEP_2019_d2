//{{TDialog = TEditTaxDialog}}
class TEditTaxDialog : public TEditDialog
{
public:
	TEditTaxDialog(TWindow* parent, TExpense *expense, bool edit, int resId = IDD_EDIT_TAX);

protected:
	TDateFace *paidDate;

	DEFINE_GROUP_TYPE(TExpense, Expense)
	virtual bool IsValid();

//{{TEditTaxDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditTaxDialogVIRTUAL_END}}
};    //{{TEditTaxDialog}}
