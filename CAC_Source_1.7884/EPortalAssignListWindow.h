//{{TListWindow = TEPortalAssignListWindow}}
class TEPortalAssignListWindow : public TGroupListWindow
{
public:
	TEPortalAssignListWindow(TWindow* parent, int resourceId, const TSideWind *tSideWind, long eportalMode);

	void Active();
	int Count() { return array->Count(); }

protected:
	const TSideWind *sideWind;
	const long eportalMode;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
};    //{{TEPortalAssignListWindow}}
