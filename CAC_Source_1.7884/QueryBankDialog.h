//{{TDialog = TQueryBankDialog}}
class TQueryBankDialog : public TGroupDialog
{
public:
	TQueryBankDialog(TWindow* parent, TQueryBankGroup *group, int resId = IDD_QUERY_BANK);

protected:
	TCheckFace *incompleteQuery;
	TDateFace *incompleteMinDate;
	TDateFace *incompleteMaxDate;
	TCheckFace *completedQuery;
	TDateFace *completedMinDate;
	TDateFace *completedMaxDate;
	TCheckFace *header;
	TLongFace *yourNo;

	virtual bool IsValid();

	void IncompleteDateEnable(TCommandEnabler &tce);
	void CompletedDateEnable(TCommandEnabler &tce);
	void QueryEnable(TCommandEnabler &tce);

//{{TQueryBankDialogRSP_TBL_BEGIN}}
	void YourNoEnable(TCommandEnabler &tce);
	void OKBNClicked();
//{{TQueryBankDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQueryBankDialog);
};    //{{TQueryBankDialog}}
