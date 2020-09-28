//{{TDialog = TLocalJudgementDialog}}
class TLocalJudgementDialog : public TGroupDialog
{
public:
	TLocalJudgementDialog(TWindow* parent, TJudgement *judgement, TDecision *tDecision, TLawsuit *tLawsuit, bool edit);

protected:
	TDecision *decision;
	TCharAliasFace *judged;
	TCharAliasFace *punishment;
	TCharAliasFace *recidive;
	TStringFace *text;

	TCheckFace *flagsNew;
	TCheckFace *flagsPriority;
	TCheckFace *flagsAppendix24;
	TCheckFace *flagsAppendix26;
	TCharListFace *punishmentsExtra;
	TCharAliasFace *jailMode;
	TCheckFace *flagsAppendix25;		// 2005:119
	TStatic *header;				// 2005:119
	TStatic *articleLabel;			// 2007:284 +related
	TArticleAliasFace *article;		// 2007:284 +related
	TCheckFace *flagsAppendix23;		// 2011:019

	// 2016:088
	TLawsuit *lawsuit;
	//bool Limited();
	bool limited;		// 2016:095 from Punishments()		

	DEFINE_GROUP_TYPE(TJudgement, Judgement)
	virtual bool IsValid();

//{{TLocalJudgementDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLocalJudgementDialogVIRTUAL_END}}

//{{TLocalJudgementDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ViewBNClicked();
	void JudgedCBNSelChange();
	void ExtraDataBNClicked();
//{{TLocalJudgementDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLocalJudgementDialog);
};    //{{TLocalJudgementDialog}}
