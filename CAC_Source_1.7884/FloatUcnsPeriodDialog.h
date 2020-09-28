class TFloatUCNsPeriodDialog : public TFloatDialog
{
public:
	TFloatUCNsPeriodDialog(TWindow *parent, TFloatUCNsGroup *group);

protected:
	TDateFace *date;
	TDateFace *minDate;
	TDateFace *maxDate;
	TRadioFace *nonFinLaw;
	TRadioFace *createdLaw;
	TRadioFace *finLawPeriod;
	TRadioFace *sessPeriod;

	virtual void SetupWindow();
	virtual bool IsValid();

	void OneDate();
	void TwoDates();

DECLARE_RESPONSE_TABLE(TFloatUCNsPeriodDialog);
};