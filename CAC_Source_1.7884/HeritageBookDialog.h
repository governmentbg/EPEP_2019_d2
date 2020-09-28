class THeritageBookDialog : public TFloatDialog
{
public:
	THeritageBookDialog(TWindow* parent, THeritageBookGroup *group, int resId = IDD_HERITAGE_BOOK);

protected:
	TRadioFace *byFinal;
	TDateFace *minFinal;
	TDateFaceUpDown *minFinalUpDn;
	TDateFace *maxFinal;
	TDateFaceUpDown *maxFinalUpDn;
	TRadioFace *byForce;
	TDateFace *minForce;
	TDateFaceUpDown *minForceUpDn;
	TDateFace *maxForce;
	TDateFaceUpDown *maxForceUpDn;
	TLongFace *autogen;

	virtual bool IsValid();
	virtual void Action(bool execute);
	void UpdateByDates();

	void ByFinalBNClicked();
	void ByForceBNClicked();
DECLARE_RESPONSE_TABLE(THeritageBookDialog);
};
