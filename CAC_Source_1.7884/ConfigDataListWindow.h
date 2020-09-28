//{{TListWindow = TConfigDataListWindow}}
class TConfigDataListWindow : public TGroupListWindow
{
public:
	TConfigDataListWindow(TWindow* parent, int resourceId, TConfigName *tConfigName);

	void SetSection(char tSection);

protected:
	TConfigName *configName;
	char section;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TConfigDataListWindow}}
