//{{TDialog = TFloatExpenseDialog}}
class TFloatExpenseDialog : public TFloatDialog
{
public:
	TFloatExpenseDialog(TWindow* parent, TFloatExpenseGroup *group, int resId = IDD_FLOAT_PENALITY);

protected:
	TTypeListFace *types;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TCharListFace *kinds;
	TDateFace *minDate;
	TDateFace *maxDate;
	TAliasListBox *penalityTypes;
	TCharListFace *accountTypes;
	TCharListFace *sumStates;
	TCharListFace *compositions;	// 2003:191 LRQ: copied from float ses
	TAliasListBox *reasonTypes;
	TDateFace *minPaidDate;
	TDateFace *maxPaidDate;
	TStringFace *paidNote;

	DEFINE_GROUP_TYPE(TFloatExpenseGroup, FloatExpenseGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TFloatExpenseDialog}}
