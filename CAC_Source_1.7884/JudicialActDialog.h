//{{TDialog = TJudicialActDialog}}
class TJudicialActDialog : public TTroubleDialog
{
public:
	TJudicialActDialog(TWindow* parent, TDecision *decision, TLawsuit *tLawsuit, const char *kinds, bool tGrant,
		TSession *tSession, int resId);

	// 2015:330 LPR: for TEditMotivesDialog
	void AttachRight(TWindow *parent, TElectricTrouble &trouble, char motivesKind);

protected:
	TLawsuit *lawsuit;
	TSession *session;		// 2008:074 LPR: for print_executive()

	TStatic *header;
	TDateFace *returned;
	TStatic *returnedLabel;
	TCharAliasFace *kind;
	TDateFace *fromDate;
	TLongFace *eventNo;
	TCharAliasFace *result;
	TSLCReasonAliasFace *slcReason;	// 2015:063 +related
	TCharAliasFace *grant;
	TStatic *grantLabel;
	TStringFace *text;
	TButton *print;
	CDate previous;			// 2006:188 initial returned date
	TStatic *registerLabel;
	TLongFace *volume;
	TStatic *volumeLabel;
	TLongFace *page;
	TStatic *pageLabel;
	TLongFace *partition;
	TStatic *partitionLabel;
	TMoneyFace *damages;
	TStatic *damagesLabel;
	// 2008:318 LPR/LRQ: moved here +related from standard act
	TCharAliasFace *age;
	TDateFace *finished;
	bool motiving;				// 2009:349 LPR: motivable -> motiving
	TDecResData data;				// 2011:151
	char results[SIZE_OF_RESULTS];	// 2011:166 +related
	char previousResult;			// 2012:034
	bool selectStandard;			// 2012:034
	TCheckFace *appeal;			// 2012:180
	bool forceChange;				// 2015:331
	TLawInterval interval;			// 2017:039
	bool personalData;			// 2017:300 cache/dynamic
	bool p64Connect;				// 2017:307
	bool alreadyPublic;			// 2017:320 was method

	void CreateAgeFinished();

	DEFINE_GROUP_TYPE(TDecision, Decision)
	virtual bool IsValid();
	virtual void ResultChanged();
	virtual bool DefaultPublic();
	virtual void DynamicPublic();
	virtual void KindChanged();
	virtual void SetMotiving(bool tMotiving);
	bool PublicDecision() { return public_decision(session->result, kind->C()); }
	void ProcessResults();

//{{TJudicialActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TJudicialActDialogVIRTUAL_END}}

//{{TJudicialActDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PrintBNClicked();
	void ResultCBNSelChange();
	void KindCBNSelChange();
	void ReturnedKillFocus();
	void RightBNClicked();
//{{TJudicialActDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TJudicialActDialog);
};    //{{TJudicialActDialog}}
