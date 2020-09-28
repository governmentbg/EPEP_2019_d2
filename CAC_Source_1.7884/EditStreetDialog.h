//{{TDialog = TEditStreetDialog}}
class TEditStreetDialog : public TEditDialog
{
public:
	TEditStreetDialog(TWindow* parent, TStreet *tStreet, bool edit, int resId = IDD_EDIT_STREET);

protected:
	TUCLPAliasFace *uclp;
	TLongFace *street;

	DEFINE_GROUP_TYPE(TStreet, Street)

//{{TEditStreetDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditStreetDialogVIRTUAL_END}}
};    //{{TEditStreetDialog}}
