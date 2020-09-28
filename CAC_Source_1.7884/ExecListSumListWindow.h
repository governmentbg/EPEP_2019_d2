//{{TListWindow = TExecListSumListWindow}}
class TExecListSumListWindow : public TDoubleListWindow
{
public:
	TExecListSumListWindow(TWindow* parent, int resourceId, TExecList *tExecList);

	int GetCount() { return GetVisualCount(); }

protected:
	TExecList *execList;

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *);
	virtual bool DeleteGroup(TGroup *);

	virtual TGroup *NewGroup() const;
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);

//{{TExecListSumListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TExecListSumListWindowVIRTUAL_END}}
};    //{{TExecListSumListWindow}}
