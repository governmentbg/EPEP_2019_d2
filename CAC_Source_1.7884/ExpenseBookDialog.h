class TExpenseBookDialog : public TGroupDialog
{
public:
	TExpenseBookDialog(TWindow *parent, TExpenseBookGroup *group, int resId = IDD_BOOK_EXPENSE );
	virtual bool IsValid();

protected:
	TCharListFace *types;
	TCharListFace *compositions;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *all;
};
