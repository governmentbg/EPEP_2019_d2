//{{TDialog = TUnfinishedLawDialog}}
class TUnfinishedLawDialog : public TFloatSubjectDialog
{
public:
	TUnfinishedLawDialog(TWindow* parent, TUnfinishedLawGroup *group, int resId = IDD_UNFINISHED_LAW);

protected:
	TCharListFace *kinds;
	TCharListFace *generalKinds;

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	TSenderListBox *senders;
	TStatic *sendersLabel;
#endif  // APPEAL || ADMIN || AREAL || MILITARY
	TDateFace *finished;
	TCharListFace *compositions;
	TUserListBox *judges;
	TCheckFace *reJudge;
	TRadioFace *byJudge;

	DEFINE_GROUP_TYPE(TUnfinishedLawGroup, UnfinishedLawGroup)
	virtual void Action(bool execute);

//{{TUnfinishedLawDialogRSP_TBL_BEGIN}}
	void ReJudgeBNClicked();
//{{TUnfinishedLawDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TUnfinishedLawDialog);
};    //{{TUnfinishedLawDialog}}
