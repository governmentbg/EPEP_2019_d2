//{{TDialog = TFloatLogDialog}}
class TFloatLogDialog : public TFloatDialog
{
public:
	TFloatLogDialog(TWindow* parent, TFloatLogGroup *group, int resId = IDD_FLOAT_LOG);

protected:
	TCharListFace *types;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TDateFace *minDate;
	TDateFace *maxDate;
	TTimeFace *minTime;
	TTimeFace *maxTime;
	TUserListBox *users;
	TCharListFace *gops;

	char extypes[SIZE_OF_EXT_TYPES];

	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TFloatLogDialog}}
