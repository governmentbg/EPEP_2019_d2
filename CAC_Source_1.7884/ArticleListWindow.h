//{{TListWindow = TArticleListWindow}}
class TArticleListWindow : public TGroupListWindow
{
public:
	TArticleListWindow(TWindow* parent, int resourceId);

	void Subject();
	void Print();

protected:
	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TArticleListWindow}}
