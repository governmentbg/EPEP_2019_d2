//{{TListWindow = TExecListCollectListWindow}}
class TExecListCollectListWindow : public TGroupListWindow
{
public:
	TExecListCollectListWindow(TWindow* parent, int resourceId, TPrintExecList *tExecList);

protected:
	TPrintExecList *execList;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TExecListCollectListWindow}}
