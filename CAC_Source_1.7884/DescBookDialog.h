class TDescBookDialog : public TBookInstanceDialog
{
public:
	TDescBookDialog(TWindow* parent, TDescBookGroup *group);

protected:
	TCharAutoListFace *compositions;
	TCheckFace *createBook;
	TCheckFace *reCap;
	TCheckFace *newFormat;
	TStatic *lawFormatLabel;
	TRadioFace *adminLawFormat;
	TRadioFace *citizenLawUnitedFromat;
	TRadioFace *citizenLawFromat;
	TCheckFace *eventText;	// 2012:054 +related

	virtual bool IsValid();
	virtual void Action(bool execute);
	virtual void SetupWindow();

	void TypeBNSelChange();
	void NewFormatBNClicked();
	void Instance1stBNClicked();
DECLARE_RESPONSE_TABLE(TDescBookDialog);
};
