//{{TListWindow = TAccountListWindow}}
class TAccountListWindow : public TGroupListWindow
{
public:
	TAccountListWindow(TWindow* parent, int resourceId);

	void SetUCNType(char tUCNType);
	void Print() { print_accounts(array); }

protected:
	char ucnType;

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
};    //{{TAccountListWindow}}
