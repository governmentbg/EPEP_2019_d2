//{{TDialog = TLoadUnitListDialog}}
class TLoadUnitListDialog : public TGroupListDialog
{
public:
	TLoadUnitListDialog(TWindow* parent, TLoadGroup *loadGroup, TLoadGroup *invisible, int resId = IDD_LOAD_UNIT_LIST);

protected:
	TLoadUnitListWindow *units;
};    //{{TLoadUnitListDialog}}
