//{{TListWindow = TSbeFzlListWindow}}
class TSbeFzlListWindow : public TXMLGroupListWindow
{
public:
	TSbeFzlListWindow(TWindow* parent, int resourceId, TXArray<FZL> *fzl, TXArray<NPRFZLPNE> *tAllLinks,
		TFzlComboBox *tCombo, SBE *tSbe);

	FZL *Fzl() { return (FZL *) Group(); }
	void Edit() { BackEdit(); }

protected:
	TFzlComboBox *combo;
	NPR *npr;
	SBE *sbe;
	TXArray<NPRFZLPNE> *allLinks;

	bool PostDialog(FZL *fzl, bool edit);

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);

	virtual TGroup *NewGroup() const;
};  //{{TSbeFzlListWindow}}
