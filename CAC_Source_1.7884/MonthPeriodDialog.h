//{{TDialog = TMonthPeriodDialog}}
class TMonthPeriodDialog : public TGroupDialog
{
public:
	TMonthPeriodDialog(TWindow* parent, TGroup *group, int resId);

protected:
	TLongAliasFace *minMonth;
	TYearFace *minYear;
	TLongAliasFace *maxMonth;
	TYearFace *maxYear;

	virtual bool IsValid();
};    //{{TMonthPeriodDialog}}
