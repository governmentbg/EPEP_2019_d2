//{{TDialog = TFloatMovementDialog}}
class TFloatMovementDialog : public TFloatDialog
{
public:
	TFloatMovementDialog(TWindow* parent, TFloatMovementGroup *group, int resId = IDD_FLOAT_MOVEMENT);

protected:
	TTypeListFace *types;
	TCharListFace *kinds;
	TDateFace *minDate;
	TDateFace *maxDate;
	TUserListBox *users;
	TStringFace *text;
	
	DEFINE_GROUP_TYPE(TFloatMovementGroup, FloatMovementGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TFloatMovementDialog}}
