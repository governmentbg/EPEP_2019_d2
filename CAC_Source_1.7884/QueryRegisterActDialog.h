//{{TDialog = TQueryRegisterActDialog}}
class TQueryRegisterActDialog : public TFloatDialog
{
public:
	TQueryRegisterActDialog(TWindow* parent, TQueryRegisterActGroup *group, int resId);

protected:
	TTypeListFace *types;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TUserListBox *judges;
	TDateFace *minDate;
	TDateFace *maxDate;

	DEFINE_GROUP_TYPE(TQueryRegisterActGroup, QueryRegisterActGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TQueryRegisterActDialog}}
