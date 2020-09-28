//{{TListWindow = TFzlListWindow}}
class TFzlListWindow : public TXMLGroupListWindow
{
public:
	TFzlListWindow(TWindow* parent, int resourceId, SBE *sbe, TRCDKeyContainer *tContainer);

	int Count();		// sid >= fzlmin
	FZL *Fzl() { return (FZL *) Group(); }
	void Links();
	bool IsValid();

protected:
	NPR *npr;
	SBE *sbe;				// 2013:015 LPR: for ovadlo
	TRCDKeyContainer *container;	// 2013:021 LPR: for edit

	FZL *FzlAt(int index) { return (FZL *) GroupAt(index); }
	bool HasLinks(FZL *fzl);

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TFzlListWindow}}
