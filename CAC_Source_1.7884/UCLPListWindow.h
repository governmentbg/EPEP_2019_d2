//{{TListWindow = TUCLPListWindow}}
class TUCLPListWindow : public TCadrListWindow
{
public:
	TUCLPListWindow(TWindow* parent, int resourceId, const long &tNewArea, const long &tNewRegion, const long &newUCLP);

	virtual void Activate();
	TUCLP *UCLP() { return (TUCLP *) Group(); }

protected:
	long area;
	long region;
	const long &newArea;
	const long &newRegion;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);
};    //{{TUCLPListWindow}}
