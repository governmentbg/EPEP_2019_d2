class TFloatSendersDialog : public TFloatDialog
{
public:
	TFloatSendersDialog(TWindow* parent, TFloatSendersGroup *group, int resId = IDD_FLOAT_SENDERS);

protected:
	TCharAutoListFace *types;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCharListFace *kinds;
	TSenderListBox *senders;
	TStringFace *text;
	
	DEFINE_GROUP_TYPE(TFloatSendersGroup, FloatSendersGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);
};
