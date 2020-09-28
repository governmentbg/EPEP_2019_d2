//{{TDialog = TJurorLoadDialog}}
class TJurorLoadDialog : public TFloatDialog
{
public:
	TJurorLoadDialog(TWindow* parent, TJurorLoadGroup *group, bool tRepeat);

protected:
	bool repeat;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckBox *explain;
	TJurorLoadListBox *jurors;

	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TJurorLoadDialog}}
