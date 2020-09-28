//{{TListWindow = TExternSessionListWindow}}
class TExternSessionListWindow : public TGroupListWindow
{
public:
	TExternSessionListWindow(TWindow* parent, int resourceId, TExternSessionListGroup &tGroup);

	TExternSession *Session() { return (TExternSession *) Group(); }
	void Search() { Refresh(); }
	void Right();
	void Assign();
	void ReAssign();
	void Query();

protected:
	TExternSessionListGroup &group;

	virtual void Criteria(msql &m, TGroup *);
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TExternSessionListWindow}}
