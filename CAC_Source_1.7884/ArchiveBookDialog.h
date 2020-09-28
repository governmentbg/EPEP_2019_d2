class TArchiveBookDialog : public TBookDialog
{
public:
	TArchiveBookDialog(TWindow* parent, TArchiveBookGroup *group);

protected:
	TYearFace *minYear;
	TYearFace *maxYear;
	TCheckFace *newFormat;
	TCheckFace *sides;

	virtual bool IsValid();
	virtual void SetupWindow();
};