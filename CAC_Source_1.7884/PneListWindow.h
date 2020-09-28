//{{TListWindow = TPneListWindow}}
class TPneListWindow : public TXMLGroupListWindow
{
public:
	TPneListWindow(TWindow* parent, int resourceId, SBE *sbe, int tPneMode, TRCDKeyContainer *tContainer);

	PNE *Pne() { return (PNE *) Group(); }
	bool IsValid();

protected:
	NPR *npr;
	long vid;				// 2013:018 LPR: for edit
	TRCDKeyContainer *container;	// 2012:022 LPR: for edit
	int pneMode;

	bool HasLinks(PNE *pne);

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TPneListWindow}}
