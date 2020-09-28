class TSessionListDialog : public TFloatDialog
{
public:
	TSessionListDialog(TWindow *parent, TPrintSessionGroup *group, int resId = IDD_SESSION_LIST);

protected:
	TCharListFace *lawKinds;
	TCharAutoListFace *compositions;
	TDateFace *minDate;
	TDateFace *maxDate;
	TLongAliasFace *hall;
	TRadioFace *whole;
	TRadioFace *morning;
	TRadioFace *afternoon;
	TTimeFace *time;
	TRadioFace *withType;
	TRadioFace *withColl;
	TRadioFace *variant_1;
	TRadioFace *variant_2;
	TRadioFace *variant_3;
	TRadioFace *variant_4;
	TRadioFace *variant_41;
	TRadioFace *variant_5;
	TRadioFace *variant_6;
	TLongFace *lines;

	DEFINE_GROUP_TYPE(TPrintSessionGroup, PrintSessionGroup);
	virtual bool IsValid();
	virtual void Action(bool execute);

	virtual void SetupWindow();
	void InvertSessionList();
};    //{{TSessionListDialog}}

