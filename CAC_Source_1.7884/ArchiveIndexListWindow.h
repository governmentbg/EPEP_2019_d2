//{{TListWindow = TArchiveIndexListWindow}}
class TArchiveIndexListWindow : public TGroupListWindow
{
public:
	TArchiveIndexListWindow(TWindow* parent, int resourceId);

	void Subject();
	void Print();
	void Empty(mstr &m);

protected:
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TArchiveIndexListWindow}}
