//{{TListWindow = TSenderListWindow}}
class TSenderListWindow : public TGroupListWindow
{
public:
	TSenderListWindow(TWindow* parent, int resourceId);

	void SetUCNType(char tUCNType);
	TSender *Sender() { return (TSender *) Group(); }
	void List();

protected:
	char ucnType;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TSenderListWindow}}
