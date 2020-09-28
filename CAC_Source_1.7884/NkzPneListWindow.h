//{{TListWindow = TNkzPneListWindow}}
class TNkzPneListWindow : public TXMLGroupListWindow
{
public:
	TNkzPneListWindow(TWindow* parent, int resourceId, NPRFZLPNE *tNPRFZLPNE, FZL *tFZL, NPR *tNPR);

protected:
	NPRFZLPNE *nprfzlpne;
	FZL *fzl;
	NPR *npr;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *);
};    //{{TNkzPneListWindow}}
