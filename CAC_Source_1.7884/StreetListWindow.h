//{{TListWindow = TStreetListWindow}}
class TStreetListWindow : public TCadrListWindow
{
public:
	TStreetListWindow(TWindow* parent, int resourceId, const long &newUCLP);

	virtual void Activate();
	TStreet *Street() { return (TStreet *) Group(); }

protected:
	long uclp;
	const long &newUCLP;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);
};    //{{TStreetListWindow}}
