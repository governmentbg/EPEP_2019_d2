//{{TListWindow = TDangerListWindow}}
class TDangerListWindow : public TGroupListWindow
{
public:
	TDangerListWindow(TWindow* parent, int resourceId);

	TDanger *Danger() { return (TDanger *) Group(); }
	void Subject();
	void Print();

protected:
	virtual TGroup *NewGroup() const;
	virtual bool EditGroup(TGroup *group);
};    //{{TDangerListWindow}}
