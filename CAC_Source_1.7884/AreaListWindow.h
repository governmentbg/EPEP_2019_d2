//{{TListWindow = TAreaListWindow}}
class TAreaListWindow : public TCadrListWindow
{
public:
	TAreaListWindow(TWindow* parent, int resourceId, const long &newArea);

	virtual void Activate();
	TArea *Area() { return (TArea *) Group(); }

protected:
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TAreaListWindow}}
