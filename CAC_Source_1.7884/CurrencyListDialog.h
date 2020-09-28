//{{TDialog = TCurrencyListDialog}}
class TCurrencyListDialog : public TGroupListDialog
{
public:
	TCurrencyListDialog(TWindow* parent, int resId = IDD_CURRENCY_LIST);

protected:
	TCurrencyListWindow *currences;

//{{TCurrencyListDialogRSP_TBL_BEGIN}}
	void DeleteBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
//{{TCurrencyListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCurrencyListDialog);
};    //{{TCurrencyListDialog}}
