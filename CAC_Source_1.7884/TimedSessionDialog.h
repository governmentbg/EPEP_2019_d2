//{{TDialog = TTimedSessionDialog}}
class TTimedSessionDialog : public TFloatSubjectDialog
{
public:
	TTimedSessionDialog(TWindow* parent, TTimedSessionGroup *group, int resId = IDD_TIMED_SESSION);

protected:
	TCharListFace *lawKinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TRadioFace *sessions;
	TDateFace *minSesDate;
	TDateFace *maxSesDate;
	TRadioFace *decisions;
	TDateFace *minDecDate;
	TDateFace *maxDecDate;
	TUserListBox *judges;
	TCharAutoListFace *compositions;
	TCharListFace *sesKinds; 	
	TRadioFace *ruleNull;
	TRadioFace *ruleAct;
	TRadioFace *ruleUndo;
	TRadioFace *ruleNone;
	TRadioFace *orderDTYN;

	DEFINE_GROUP_TYPE(TTimedSessionGroup, TimedGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TTimedSessionDialogRSP_TBL_BEGIN}}
	void SessionsBNClicked();
//{{TTimedSessionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTimedSessionDialog);
};    //{{TTimedSessionDialog}}
