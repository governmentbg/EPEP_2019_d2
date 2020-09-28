//{{TDialog = TFloatRegisterActDialog}}
class TFloatRegisterActDialog : public TQueryRegisterActDialog
{
public:
	TFloatRegisterActDialog(TWindow* parent, TFloatRegisterActGroup *group, int resId);

protected:
	char regKinds[SIZE_OF_KINDS];
	TCharListFace *kinds;

	DEFINE_GROUP_TYPE(TFloatRegisterActGroup, FloatRegisterActGroup)
	virtual void Action(bool execute);
};    //{{TFloatRegisterActDialog}}
