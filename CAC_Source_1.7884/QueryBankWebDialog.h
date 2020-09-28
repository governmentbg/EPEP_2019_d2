//{{TDialog = TQueryBankWebDialog}}
class TQueryBankWebDialog : public TQueryBankDialog
{
public:
	TQueryBankWebDialog(TWindow* parent, TQueryBankGroup *group);

protected:
	TCheckFace *incompleteWebQuery;
	TStringFace *incompleteWebFile;
	TCheckFace *completedWebQuery;
	TStringFace *completedWebFile;

	bool Invalid(TCheckFace *webQuery, const char *name, const char *what);
	virtual bool IsValid();

	void IncompleteDateEnable(TCommandEnabler &tce);
	void CompletedDateEnable(TCommandEnabler &tce);

//{{TQueryBankWebDialogRSP_TBL_BEGIN}}
	void IncompleteWebFileEnable(TCommandEnabler &tce);
	void CompletedWebFileEnable(TCommandEnabler &tce);
	void OKBNEnable(TCommandEnabler &tce);
//{{TQueryBankWebDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQueryBankWebDialog);
};    //{{TQueryBankWebDialog}}

