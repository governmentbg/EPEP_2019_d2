//{{TDialog = TQueryBankruptDialog}}
class TQueryBankruptDialog : public TPeriodDialog
{
public:
	TQueryBankruptDialog(TWindow* parent, TPeriodGroup *period, TGArray<TBankrupt> *tBankrupts, int select, int resId = IDD_QUERY_BANKRUPT);

protected:
	TBankruptListBox *bankrupts;

//{{TQueryBankruptDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TQueryBankruptDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQueryBankruptDialog);
};    //{{TQueryBankruptDialog}}
