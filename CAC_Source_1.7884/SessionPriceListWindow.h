//{{TListWindow = TSessionPriceListWindow}}
class TSessionPriceListWindow : public TGroupListWindow
{
public:
	TSessionPriceListWindow(TWindow* parent, int resourceId);

	void Print();

protected:
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TSessionPriceListWindow}}
