class TCombineCorruptListDialog : public TGroupListDialog
{
public:
	TCombineCorruptListDialog(TWindow* parent, TCombineCorruptGroup *group);

protected:
	TStatic *minMonth, *minYear;
	TStatic *maxMonth, *maxYear;
	TCombineCorruptListWindow *corrupts;

	void AddBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
DECLARE_RESPONSE_TABLE(TCombineCorruptListDialog);
};

