//{{TDialog = TPostalDialog}}
class TPostalDialog : public TGroupDialog
{
public:
	TPostalDialog(TWindow* parent, TGroup *group, int resId = IDD_POSTAL);

protected:
	TCheckBox *buttons[IDC_POSTAL_INDEX_COUNT];
};    //{{TPostalDialog}}
