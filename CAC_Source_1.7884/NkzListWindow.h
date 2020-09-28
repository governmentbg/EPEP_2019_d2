//{{TListWindow = TNkzListWindow}}
class TNkzListWindow : public TXMLGroupListWindow
{
public:
	TNkzListWindow(TWindow* parent, int resourceId, FZL *tFZL, SBE *tSbe);

protected:
	FZL *fzl;
	SBE *sbe;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *);
};    //{{TNkzListWindow}}
