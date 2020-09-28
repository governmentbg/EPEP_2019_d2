//{{TListWindow = TFzlNprPneListWindow}}
class TFzlNprPneListWindow : public TXMLGroupListWindow
{
public:
	TFzlNprPneListWindow(TWindow* parent, int resourceId, FZL *tFZL, SBE *tSbe);

	void Edit() { BackEdit(); }

	int LinkCount() const { return freeLinks.Count(); }

protected:
	FZL *fzl;
	NPR *npr;
	SBE *sbe;
	TXArray<NPRFZLPNE> freeLinks;
	TXArray<PNE> freePnes;

	bool PostDialog(NPRFZLPNE *nprfzlpne, bool edit);

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);

	virtual void GetGroupText(mstr &m, TGroup *group, int column);
	virtual TGroup *NewGroup() const;
};    //{{TFzlNprPneListWindow}}
