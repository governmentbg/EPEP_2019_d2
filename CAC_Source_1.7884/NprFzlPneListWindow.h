//{{TListWindow = TNprFzlPneListWindow}}
class TNprFzlPneListWindow : public TXMLGroupListWindow
{
public:
	TNprFzlPneListWindow(TWindow* parent, int resourceId, FZL *tFZL, SBE *tSbe, TNprFzlPneComboBox *tCombo);
	virtual ~TNprFzlPneListWindow();

protected:
	FZL *fzl;
	NPR *npr;
	SBE *sbe;
	TXArray<NPRFZLPNE> *allLinks;
	TNprFzlPneComboBox *combo;

	virtual void GetGroupText(mstr &m, TGroup *group, int column);

	virtual TGroup *NewGroup() const;
	virtual void LoadData(const char *);

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TNprFzlPneListWindow}}
