//{{TDialog = TEditExecListCollectDialog}}
class TEditExecListCollectDialog : public TEditDialog
{
public:
	TEditExecListCollectDialog(TWindow* parent, TPrintExecList *tExecList, TExecListCollect *collect, bool edit);

protected:
	TPrintExecList *execList;

	TDateFace *date;
	TMoneyFace *sum;
	TStringAliasFace *currency;
	TCheckFace *voluntary;
	TSelectDebtorListBox *debtor;
	TStringFace *text;

	DEFINE_GROUP_TYPE(TExecListCollect, Collect)
	virtual bool IsValid();

//{{TEditExecListCollectDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditExecListCollectDialogVIRTUAL_END}}

//{{TEditExecListCollectDialogRSP_TBL_BEGIN}}
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
//{{TEditExecListCollectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditExecListCollectDialog);
};    //{{TEditExecListCollectDialog}}
