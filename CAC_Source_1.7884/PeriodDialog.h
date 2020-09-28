//{{TDialog = TPeriodDialog}}
class TPeriodDialog : public TGroupDialog
{
public:
	TPeriodDialog(TWindow* parent, TGroup *group, int resId = IDD_PERIOD);

protected:
	TDateFace *minDate;
	TDateFace *maxDate;

	virtual bool IsValid();
};    //{{TPeriodDialog}}
