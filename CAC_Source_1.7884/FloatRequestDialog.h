//{{TDialog = TFloatRequestDialog}}
class TFloatRequestDialog : public TFloatSubjectDialog
{
public:
	TFloatRequestDialog(TWindow* parent, TFloatRequestGroup *group);

protected:
	TCharListFace *generalKinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TCharAutoListFace *colleges;
	TDateFace *minDate;
	TDateFace *maxDate;
	TStringFace *text;
	TRadioFace *slow;
	TRadioFace *quick;
	TRadioFace *immediate;
	TRadioFace *anySpeed;
	TEisPartFace *eisppNmr;

	DEFINE_GROUP_TYPE(TFloatRequestGroup, FloatRequestGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TFloatRequestDialog}}
