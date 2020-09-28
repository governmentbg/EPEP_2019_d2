//{{TListWindow = TPenalityListWindow}}
class TPenalityListWindow : public TGroupListWindow
{
public:
	TPenalityListWindow(TWindow* parent, int resourceId);

	void Subject();
	void Print();

protected:
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TPenalityListWindow}}
