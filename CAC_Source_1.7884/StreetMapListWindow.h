//{{TListWindow = TStreetMapListWindow}}
class TStreetMapListWindow : public TGroupListWindow
{
public:
	TStreetMapListWindow(TWindow* parent, TUser *tUser, int resourceId);

	void SetUCLP(long tUCLP);
	void Print();
	void Copy();

protected:
	TUser *user;
	long uclp;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TStreetMapListWindow}}
