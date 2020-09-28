class TClosedSessionBookDialog : public TFloatDialog
{
public:
	TClosedSessionBookDialog(TWindow* parent, TClosedSessionBookGroup *group);

protected:
	char types[SIZE_OF_TYPES];
	TCharAliasFace *type;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCharAutoListFace *compositions;
	TCheckFace *allResults;
	TCheckFace *newFormat;
	TCheckFace *sessions;
	TCheckFace *inregs;
	TCheckFace *createBook;
	TCheckFace *createReCap;
	TCheckFace *createResults;
	TRadioFace *orderByDecisions;
	TRadioFace *orderByLawsuits;
	TCheckFace *commonOutput;
	TPushButton *byReturned;

	unsigned CreateCount() const;
	void CommonOutputUpdate();
	void ByReturnedUpdate();

	DEFINE_GROUP_TYPE(TClosedSessionBookGroup, ClosedSessionBookGroup);
	virtual bool IsValid();
	virtual void Action(bool execute);

	virtual void SetupWindow();

	void CreateBookBNClicked();
	void AllResultsBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
	void TypeCBNSelChange();
	void CreateReCapBNClicked();
	void CreateResultsBNClicked();
	void ByReturnedBNClicked();	
DECLARE_RESPONSE_TABLE(TClosedSessionBookDialog);
};
