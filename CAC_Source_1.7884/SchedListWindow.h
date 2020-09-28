//{{TListWindow = TSchedListWindow}}
class TSchedListWindow : public TGroupListWindow
{
public:
	TSchedListWindow(TWindow* parent, int resourceId, TSched *tSched, const char *tScheds);

	const char *Scheds() { return scheds; }

protected:
	TSched *sched;
	const char *scheds;

	virtual TSched *NewGroup() const;

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *);
	virtual bool DeleteGroup(TGroup *group);		// 2002:112 For logging
};    //{{TSchedListWindow}}
