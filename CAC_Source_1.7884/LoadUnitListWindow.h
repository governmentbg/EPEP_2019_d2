//{{TListWindow = TLoadUnitListWindow}}
class TLoadUnitListWindow : public TGroupListWindow
{
public:
	TLoadUnitListWindow(TWindow* parent, int resourceId, TLoadGroup *tLoadGroup, TLoadGroup *invisible);

protected:
	TLoadGroup *loadGroup;
	TLoadGroup *invisible;

	virtual TGroup *NewGroup() const { return NULL; }
	virtual bool AddGroup(TGroup *);
};    //{{TLoadUnitListWindow}}
