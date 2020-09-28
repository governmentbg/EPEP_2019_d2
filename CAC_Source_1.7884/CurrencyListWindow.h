//{{TListWindow = TCurrencyListWindow}}
class TCurrencyListWindow : public TGroupListWindow
{
public:
	TCurrencyListWindow(TWindow* parent, int resourceId);

	void Print() { print_currences(array); }

protected:
	virtual TGroup *NewGroup() const { return new TCurrency; }
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);
};    //{{TCurrencyListWindow}}
