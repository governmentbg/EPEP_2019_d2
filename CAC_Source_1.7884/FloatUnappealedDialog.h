//{{TDialog = TFloatUnappealedDialog}}
class TFloatUnappealedDialog : public TFloatSubjectDialog
{
public:
	TFloatUnappealedDialog(TWindow* parent, TFloatUnappealedGroup *group);

protected:
	TCharAutoListFace *lawKinds;
	TCharListFace *decKinds;
	TUserListBox *judges;
	TCharListFace *compositions;
	TDateFace *minForceDate;
	TDateFace *maxForceDate;
	TDateFace *minFromDate;
	TDateFace *maxFromDate;
	TCheckFace *decFinal;

	DEFINE_GROUP_TYPE(TFloatUnappealedGroup, FloatUnappealedGroup);
	virtual bool IsValid();
	virtual void Action(bool execute);
};
