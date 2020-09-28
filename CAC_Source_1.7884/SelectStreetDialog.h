//{{TDialog = TSelectStreetDialog}}
class TSelectStreetDialog : public TSelectDialog
{
public:
	TSelectStreetDialog(TWindow* parent, TStreet *street, int resId = IDD_SELECT_STREET);

protected:
	TUCLPAliasFace *uclp;
	TStreetListBox *streets;

	DEFINE_GROUP_TYPE(TStreet, Street)
};    //{{TSelectStreetDialog}}
