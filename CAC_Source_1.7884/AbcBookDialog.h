class TAbcBookDialog : public TFloatDialog
{
public:
	TAbcBookDialog(TWindow* parent, TAbcBookGroup *group);

protected:
	TCharAliasFace *type;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *year;
	TDateFace *minDate;
	TDateFace *maxDate;
	TStringFace *minLetter;
	TStringFace *maxLetter;
	TCheckFace *allSides;
	TCheckFace *composition;
	TCheckFace *involvement;
	TCheckFace *newFormat;
	TRadioFace *abv;
	TRadioFace *insol;
	TButton *create;

	virtual bool IsValid();
	virtual void Action(bool execute);
	virtual void SetupWindow();

	void UpdateButtons();
	void YearKillFocus();

	void TypeCBNSelChange();
	void NewFormatBNClicked();
DECLARE_RESPONSE_TABLE(TAbcBookDialog);
};
