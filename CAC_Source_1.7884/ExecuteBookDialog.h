class TExecuteBookDialog : public TFloatDialog
{
public:
	TExecuteBookDialog(TWindow* parent, TExecuteBookGroup *group, int resId = IDD_EXECUTE_BOOK);

protected:
	TCheckFace *allColumns;
	TDateFace *minDate;
	TDateFace *maxDate;
	TLongFace *serialNo;

	virtual bool IsValid();
};
