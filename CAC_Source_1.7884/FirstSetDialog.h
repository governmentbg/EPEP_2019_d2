class TFirstSetDialog : public TFloatSubjectDialog
{
public:
	TFirstSetDialog(TWindow* parent, TFirstSetGroup *group);

protected:
	TCharListFace *kinds;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TStringListBox *request;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *doublex;
	TCheckFace *firstSet;
	char ExchangeKinds[SIZE_OF_KINDS];

	DEFINE_GROUP_TYPE(TFirstSetGroup, FirstSetGroup);
	virtual bool IsValid();
	virtual void Action(bool execute);

	virtual void SetupWindow();
};
