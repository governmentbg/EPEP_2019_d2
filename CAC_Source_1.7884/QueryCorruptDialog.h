//{{TDialog = TQueryCorruptDialog}}
class TQueryCorruptDialog : public TMonthPeriodDialog
{
public:
	TQueryCorruptDialog(TWindow* parent, TCorruptGroup *group);

protected:
	TCheckFace *forExport;
};    //{{TQueryCorruptDialog}}
