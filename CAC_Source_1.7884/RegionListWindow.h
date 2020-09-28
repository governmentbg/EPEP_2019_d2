//{{TListWindow = TRegionListWindow}}
class TRegionListWindow : public TCadrListWindow
{
public:
	TRegionListWindow(TWindow* parent, int resourceId, const long &tNewArea, const long &newRegion);

	virtual void Activate();
	TRegion *Region() { return (TRegion *) Group(); }

protected:
	long area;
	const long &newArea;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TRegionListWindow}}
