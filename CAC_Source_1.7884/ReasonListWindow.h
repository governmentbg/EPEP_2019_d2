//{{TListWindow = TReasonListWindow}}
class TReasonListWindow : public TGroupListWindow
{
public:
	TReasonListWindow(TWindow* parent, int resourceId);

	void Subject();
	void Print();

protected:
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TReasonListWindow}}
