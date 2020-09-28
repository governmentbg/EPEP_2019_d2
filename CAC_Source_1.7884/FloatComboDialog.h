//{{TDialog = TFloatComboDialog}}
class TFloatComboDialog : public TFloatSubjectDialog
{
public:
	TFloatComboDialog(TWindow* parent, TFloatComboGroup *group);
	virtual ~TFloatComboDialog();

protected:
	TTabControl *pages;
	TButton *ok;

	// --- Lawsuit ---
	TStatic *kindsLabel;
	TCharAutoListFace *kinds;
	TStatic *minNoLabel;
	TLongFace *minNo;
	TStatic *maxNoLabel;
	TLongFace *maxNo;
	TStatic *minYearLabel;
	TYearFace *minYear;
	TStatic *maxYearLabel;
	TYearFace *maxYear;
	TStatic *collegesLabel;
	TCharAutoListFace *colleges;
	TStatic *lawCompositionsLabel;
	TCharListFace *lawCompositions;
	TCheckFace *firstSet;
	TStatic *lawDateLabel;
	TDateFace *lawMinDate;
	TDateFace *lawMaxDate;
	TCheckFace *doublex;
	TCheckFace *subjectText;
	TStatic *lawFinishedLabel;
	TDateFace *minFinished;
	TDateFace *maxFinished;
	TStatic *lawJudgesLabel;
	TUserListBox *lawJudges;
	TCheckFace *partof;	// 2004:007 LRQ: F_JUDGE or F_REJUDGE
	TStatic *agesLabel;
	TCharListFace *ages;
	TStatic *lawForceDateLabel;
	TDateFace *lawMinForceDate;
	TDateFace *lawMaxForceDate;
	TStatic *reregisterLabel;
	TDateFace *minReregister;
	TDateFace *maxReregister;
	TStatic *grantsLabel;
	TCharListFace *grants;
	TStatic *dangersLabel;
	TDangerListBox *dangers;
	TStatic *generalKindsLabel;
	TCharListFace *generalKinds;
	TGroupBox *speedGroup;
	TRadioFace *slow;
	TRadioFace *quick;
	TRadioFace *immediate;
	TRadioFace *anySpeed;
	TStatic *lawTextLabel;
	TStringFace *lawText;
	TGroupBox *cutDownGroup;
	TCheckFace *important;		// 2003:114 yes, no, maybe
	TCheckFace *newGPK;		// 2008:080
	TCheckFace *limited;		// 2008:282
	TGroupBox *lawOrderGroup;
	TRadioFace *lawByTYN;
	TRadioFace *lawByTSYN;
	TRadioFace *lawByTKYN;
	TRadioFace *lawByJTYN;
	TStatic *eisppNmrLabel;
	TEisPartFace *eisppNmr;
	// 2014:162
	TRadioFace *cutDownAny;
	TRadioFace *cutDownCutDown;
	TRadioFace *cutDownUnspecified;
	TRadioFace *cutDownP1;
	TRadioFace *cutDownP2;
	TRadioFace *cutDownP1_P2;
	TRadioFace *cutDownNotCutDown;
	TCheckFace *lawChildren;	// 2016:105
	TCheckFace *lawsuitText;

	// --- Session ---
	TStatic *sesKindsLabel;
	TCharListFace *sesKinds;
	TStatic *sesMinDateLabel;
	TDateFace *sesMinDate;
	TStatic *sesMaxDateLabel;
	TDateFace *sesMaxDate;
	TStatic *sesMinReturnedLabel;
	TDateFace *sesMinReturned;
	TStatic *sesMaxReturnedLabel;
	TDateFace *sesMaxReturned;
	TStatic *sesEventNoLabel;
	TLongFace *sesMinEventNo;
	TLongFace *sesMaxEventNo;
	TStatic *sesJudgesLabel;
	TUserListBox *sesJudges;
	TStatic *sesCompositionsLabel;
	TCharListFace *sesCompositions;
	TStatic *sesResultsLabel;
	TCharListNameFace *sesResults;
	TGroupBox *sesJudgeGroup;
	TRadioFace *asJudge;
	TRadioFace *asAny;
	TStatic *secretarsLabel;
	TUserListBox *secretars;
	TStatic *jurorsLabel;
	TKnownPersonListBox *jurors;
	TCheckFace *sessionChanjed;
	TCheckFace *proceed;
	TCheckFace *closedd;
	TCheckFace *extended;
	TCheckFace *firstSes;
	TCheckFace *expert;
	TStatic *sesTextLabel;
	TStringFace *sesText;
	TCheckFace *sesDateAfterFinish;
	TCheckFace *sesReturnAfterFinish;
	TStatic *sesForceDateLabel;
	TDateFace *sesMinForceDate;
	TDateFace *sesMaxForceDate;
	TCheckFace *sesFinalizing;
	TCheckFace *sesAppeal;
	TStatic *sesSLCReasonsLabel;
	TSLCReasonListBox *sesSLCReasons;
	TCheckFace *sesPublTreg;

	// --- Decision ---
	TStatic *decResultsLabel;
	TCharListNameFace *decResults;
	TCheckFace *decSurrouendocs;
	TStatic *fromDateLabel;
	TDateFace *minFromDate;
	TDateFace *maxFromDate;
	TStatic *decForceDateLabel;
	TDateFace *decMinForceDate;	// 2008:038 LRQ
	TDateFace *decMaxForceDate;
	TStatic *decKindsLabel;
	TCharListFace *decKinds;
	TStatic *accomplysLabel;
	TCharListFace *accomplys;
	TStatic *returnedLabel;
	TDateFace *minReturned;
	TDateFace *maxReturned;
	TCheckFace *confess;
	TStatic *partitionLabel;
	TLongFace *minPartition;
	TLongFace *maxPartition;
	TStatic *minVolumeLabel;
	TLongFace *minVolume;
	TStatic *maxVolumeLabel;
	TLongFace *maxVolume;
	TStatic *decTextLabel;
	TStringFace *decText;
	TCheckFace *motives;
	TCheckFace *absence;
	TCheckFace *opinion;
	TStatic *debtorStatusLabel;
	TCharListFace *debtorStatus;
	TCheckFace *decAppeal;
	TCheckFace *decPublTreg;
	TCheckFace *decContract;
	TStatic *keyWordsLabel;		// 2017:109 FIX: was missing
	TKeyWordsFace *keyWords;	// 2017:082

	// --- JudgementSection ---
	TStatic *judgedsLabel;
	TCharListFace *judgeds;
	TStatic *articlesLabel;
	TAliasListBox *articles;
	TCheckFace *appendix24;
	TCheckFace *appendix25;
	TCheckFace *appendix26;
	TStatic *punishmentsLabel;
	TCharListFace *punishments;
	TStatic *intervalsLabel;
	TCharListFace *intervals;
	TStatic *sendLabel;
	TDateFace *minSend;
	TDateFace *maxSend;
	TStatic *forceDateLabel;
	TDateFace *minForceDate;
	TDateFace *maxForceDate;
	TGroupBox *judOrderGroup;
	TRadioFace *judByTYN;
	TRadioFace *judByART;
	TRadioFace *judByForce;
	TRadioFace *judBySend;
	TCheckFace *appendix23;		// 2011:019 LPR
	TCheckFace *judYoung;
	TStatic *punishmentsXLabel;
	TCharListFace *punishmentsX;
	TStatic *decSLCReasonsLabel;
	TSLCReasonListBox *decSLCReasons;
	// 2018-02-09
	TStatic *reportLabel;
	TRadioButton *reportIgnore;
	TRadioButton *reportCounted;
	TRadioButton *reportUncounted;
	TCheckFace *pt329a3;

	TViewControl *viewControls[PAGE_COUNT];

	DEFINE_GROUP_TYPE(TFloatComboGroup, FloatComboGroup);
	virtual bool IsValid();
	virtual void Action(bool execute);

	virtual void SetupWindow();

//{{TFloatComboDialogRSP_TBL_BEGIN}}
	void BasicBNClicked();
	void PagesTCNSelChange(TNotify far&);
	void SurrouendocsBNClicked();
//{{TFloatComboDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatComboDialog);
};    //{{TFloatComboDialog}}
