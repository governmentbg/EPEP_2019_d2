//{{TListWindow = TAnnounceListWindow}}
class TAnnounceListWindow : public TGroupListWindow
{
public:
	TAnnounceListWindow(TWindow* parent, int resourceId, TAnnounceGroup *tGroup, const char *types, TGroupArray *tDeliverers);

	virtual void Refresh();
	
	TAnnounce *Announce() { return (TAnnounce *) Group(); }
	bool Assignable() { return assignable; }
	void ServeAnnounce();
	void Assign();
	void AddRepeat(bool tCopy);				// 2010:340 +related
	void FindText(const char *text, int step);	// 2011:139 +related

protected:
	TAnnounceGroup *group;
	char connectKinds[SIZE_OF_CONNECT_KINDS];
	int assignable;
	TGroupArray *deliverers;

	virtual TGroup *NewGroup() const;
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TAnnounceListWindow}}
