//{{TDialog = TExecJudgementDialog}}
class TExecJudgementDialog : public TEditDialog
{
public:
	TExecJudgementDialog(TWindow* parent, TJudgement *judgement, TDecision *tDecision);

protected:
	TDateFace *forceDate;
	TDateFace *sendDate;
	TDateFace *execDate;

	TStringFace *sendTo;
	TStringFace *execBind;
	TStringFace *execPlace;
	TStringFace *forgive;
	TStringFace *orderFrom;
	TDateFace *givenDate;
	TStringFace *sendText;
	TDateFace *countDate;

	TDecision *decision;
	TLongFace *eventNo;
	TCharAliasFace *interval;

	bool judged;
	TDateFace *notifyDate;	// 2017:303 +related

	DEFINE_GROUP_TYPE(TJudgement, Judgement)
	virtual bool IsValid();

//{{TExecJudgementDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TExecJudgementDialogVIRTUAL_END}}

//{{TExecJudgementDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PrintBNClicked();
	void EditBNClicked();
//{{TExecJudgementDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TExecJudgementDialog);
};    //{{TExecJudgementDialog}}
