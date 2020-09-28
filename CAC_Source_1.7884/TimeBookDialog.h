class TTimeBookDialog : public TFloatDialog
{
public:
	TTimeBookDialog(TWindow *parent, TTimeBookGroup *group);

protected:
	TCharAliasFace *type;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *cumulative;
	TStatic *beginDateLabel;
	TDateFace *beginDate;
	TCheckFace *book;
	TCheckFace *bookMno;
	TCheckFace *summary;
	TCheckFace *judgeSummary;
	TCheckFace *resultSet;
	TRadioFace *rf1;
	TRadioFace *rf2;
	TRadioFace *rf3;
	TRadioFace *rf4;
	TTimeFace *pNoon;
	TRadioFace *newFormat;

	bool IsValid();
	virtual void SetupWindow();
	virtual void Action(bool execute);

	void BClicked();
	void BChange();  
	void BCumulative();  
	void Standard();
	void BClickedBook();
	void BClickedBookMNO();
DECLARE_RESPONSE_TABLE(TTimeBookDialog);
};