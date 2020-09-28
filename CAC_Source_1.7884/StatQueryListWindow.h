class TStatQueryListWindow : public TGroupListWindow
{
public:
	TStatQueryListWindow(TWindow* parent, int resourceId);	

	void Launch();

protected:
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
};
