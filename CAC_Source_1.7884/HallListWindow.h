//{{TListWindow = THallListWindow}}
class THallListWindow : public TGroupListWindow
{
public:
	THallListWindow(TWindow* parent, int resourceId);

	THall *Hall() { return (THall *) Group(); }

	void LoadWeek();
	void LoadDate();
	void Print();

protected:
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{THallListWindow}}
