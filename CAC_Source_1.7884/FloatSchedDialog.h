//{{TDialog = TFloatSchedDialog}}
class TFloatSchedDialog : public TFloatDialog
{
public:
	TFloatSchedDialog(TWindow* parent, TFloatSchedGroup *group, int resId = IDD_FLOAT_SCHED);

protected:
	TTypeListFace *types;
	TCharListFace *scheds;
	TDateFace *minStart;
	TDateFace *maxStart;
	TDateFace *minFinal;
	TDateFace *maxFinal;
	TUserListBox *judges;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;

	DEFINE_GROUP_TYPE(TFloatSchedGroup, FloatSchedGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

//{{TFloatSchedDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFloatSchedDialogVIRTUAL_END}}
};    //{{TFloatSchedDialog}}
