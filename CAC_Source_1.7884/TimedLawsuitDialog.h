//{{TDialog = TTimedLawsuitDialog}}
class TTimedLawsuitDialog : public TFloatSubjectDialog
{
public:
	TTimedLawsuitDialog(TWindow* parent, TTimedLawsuitGroup *group);

protected:
	TCharListFace *lawKinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TUserListBox *judges;
	TCharAutoListFace *compositions;
	TRadioFace *requestDate;
	TDateFace *minRequestDate;
	TDateFace *maxRequestDate;
	TRadioFace *lawsuitDate;
	TDateFace *minLawsuitDate;
	TDateFace *maxLawsuitDate;

	DEFINE_GROUP_TYPE(TTimedLawsuitGroup, TimedGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TTimedLawsuitDialogRSP_TBL_BEGIN}}
	void RequestDateBNClicked();
//{{TTimedLawsuitDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTimedLawsuitDialog);
};    //{{TTimedLawsuitDialog}}
