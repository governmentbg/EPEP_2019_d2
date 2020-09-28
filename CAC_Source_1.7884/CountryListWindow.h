//{{TListWindow = TCountryListWindow}}
class TCountryListWindow : public TGroupListWindow
{
public:
	TCountryListWindow(TWindow* parent, int resourceId);

protected:
	virtual TGroup *NewGroup() const { return new TCountry; }
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TCountryListWindow}}
