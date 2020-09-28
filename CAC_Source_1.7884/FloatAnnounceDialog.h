//{{TDialog = TFloatAnnounceDialog}}
class TFloatAnnounceDialog : public TFloatDialog
{
public:
	TFloatAnnounceDialog(TWindow* parent, TFloatAnnounceGroup *group, int resId = IDD_FLOAT_ANNOUNCE);

protected:
	TCharListFace *kinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TSenderListBox *senders;
	TDateFace *minSessionDate;
	TDateFace *maxSessionDate;
	TDateFace *minSubpoenaDate;
	TDateFace *maxSubpoenaDate;
	TCharListFace *subpoenaKinds;
	TCharListFace *receivedStatuses;
	TUserListBox *deliverers;
	TDateFace *minReturnedDate;
	TDateFace *maxReturnedDate;

	DEFINE_GROUP_TYPE(TFloatAnnounceGroup, FloatAnnounceGroup)
	virtual bool IsValid();

	virtual void Action(bool execute);
};    //{{TFloatAnnounceDialog}}
