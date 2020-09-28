//{{TDialog = TTradeSyndicActDialog}}
class TTradeSyndicActDialog : public TJudicialActDialog
{
public:
	TTradeSyndicActDialog(TWindow* parent, TDecision *decision, TLawsuit *tLawsuit, const char *kinds, bool grant,
		TSession *session);

protected:
	TCharListFace *accomplys;
	TDateFace *completed;
	TCharAliasFace *debtorStatus;

	virtual void ResultChanged();
	virtual bool DefaultPublic();

//{{TTradeSyndicActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TTradeSyndicActDialogVIRTUAL_END}}

//{{TTradeSyndicActDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PrintBNClicked();
	void SyndicateBNClicked();
	void AccomplysLBNSelChange();
//{{TTradeSyndicActDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTradeSyndicActDialog);
};    //{{TTradeSyndicActDialog}}
