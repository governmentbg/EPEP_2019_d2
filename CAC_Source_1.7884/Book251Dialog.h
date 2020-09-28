class TBook251Dialog : public TGroupDialog
{
public:
	TBook251Dialog(TWindow* parent, TInDocBookGroup *group, int resId = IDD_BOOK_251);

protected:
	TDateFace *minDate;
	TDateFace *maxDate;

	virtual bool IsValid();
};
