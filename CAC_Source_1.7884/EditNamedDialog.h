//{{TDialog = TEditNamedDialog}}
class TEditNamedDialog : public TEditDialog
{
public:
	TEditNamedDialog(TWindow* parent, TNamedGroup *ucnGroup, bool edit, bool tFull, int resId);

protected:
	// 2011:075 +related
	bool full;		// 2008:039 +related
	TNameFace *name;

	DEFINE_GROUP_TYPE(TNamedGroup, UCNGroup)

//{{TEditNamedDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditNamedDialogVIRTUAL_END}}
};    //{{TEditNamedDialog}}
