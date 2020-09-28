class TSlowBookDialog : public TFloatDialog
{
public:
	TSlowBookDialog(TWindow* parent, TSlowBookGroup *group, int resId = IDD_SLOWDOWN_BOOK);

protected:
	TCharListFace *types;
	TDateFace *minDate;
	TDateFace *maxDate;

	virtual bool IsValid();
};
