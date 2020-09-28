//{{TListWindow = TInformedListWindow}}
class TInformedListWindow : public TGroupListWindow
{
public:
	TInformedListWindow(TWindow* parent, int resourceId);

	void SetInformed(const char *tInformed);
	void Import();
	void Print();

protected:
	char informed[SIZE_OF_UCN];

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TInformedListWindow}}
