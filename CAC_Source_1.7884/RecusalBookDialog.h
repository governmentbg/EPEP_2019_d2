class TRecusalBookDialog : public TFloatDialog
{
public:
	TRecusalBookDialog(TWindow* parent, TRecusalBookGroup *group, int resId = IDD_RECUSAL_BOOK);

protected:
	TCharListFace *types;
	TCharAutoListFace *compositions;

	TDateFace *minDate;
	TDateFace *maxDate;
	TLongFace *autogen;
	TCheckFace *filtered;
	TCheckFace *recujed;

	virtual bool IsValid();
};
