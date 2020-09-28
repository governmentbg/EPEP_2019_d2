//{{TDialog = TSessionResultDialog}}
class TSessionResultDialog : public TSessionTimesDialog
{
public:
	TSessionResultDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, bool edit, int resId);

protected:
	TLawInterval interval;		// 2009:204 +related
	CDate oldFinal;
	char oldResult;
	bool chanjint;			// 2010:335 +related

	TDateFace *returned;
	TCheckFace *flagsChanjed;
	TSLCReasonAliasFace *slcReason;	// 2015:055 +related
	TStatic *slcReasonLabel;		// 2015:064 +related
	TCheckFace *flagsRecujed;
	TCharAliasFace *age;
	TDateFace *finished;
	TStringFace *text;
	TCheckFace *expert;
	TCheckFace *appeal;	// 2012:180

	bool AskExpert();
	void CommitGeneric();
	void UpdateInterval();
	void UpdateSLCReason();
	void UpdateRecujed();
	void AttachExtra(const CDate &date);
	bool PreMove();

	// 2016:042 +related
	TSurroundSelection surroundSelection;
	TPushButton *surrounds;
	char results[SIZE_OF_RESULTS];
	bool CheckSurrouendocs();

	// 2017:038 +related
	bool ReadInterval(const CDate &date, bool mandatory);
	CDate oldFirstSet;
	bool intervalRead;

	DEFINE_GROUP_TYPE(TSession, Session)
	virtual bool IsValid();
	virtual bool IsClean();

//{{TSessionResultDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSessionResultDialogVIRTUAL_END}}

//{{TSessionResultDialogRSP_TBL_BEGIN}}
	void JudgeCBNSelChange();
	void ChanjedBNClicked();
	void SurroundsBNClicked();
//{{TSessionResultDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSessionResultDialog);
};    //{{TSessionResultDialog}}
