class TAppealedBookDialog : public TFloatDialog
{
public:
	TAppealedBookDialog(TWindow* parent, TAppealedBookGroup *group, int resId = IDD_APPEALED_BOOK);

protected:
	TCharListFace *types;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *columns;
	TCheckFace *rec;

	virtual bool IsValid();
};    //{{TAppealedBookDialog}}
