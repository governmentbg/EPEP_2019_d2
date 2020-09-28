//{{TListWindow = TSbeListWindow}}
class TSbeListWindow : public TGroupListWindow
{
public:
	TSbeListWindow(TWindow* parent, int resourceId, NPR *tNPR, TLawsuit *tLawsuit, TDatedContainer *tContainer,
		TIntegerArray *tLset, TGArray<SBE> *sbes, TSbeAliasFace *tAlias);

	SBE *Sbe() { return (SBE *) Group(); }		
	void Edit() { BackEdit(); }

protected:
	TSbeAliasFace *alias;
	NPR *npr;
	TLawsuit *lawsuit;
	TDatedContainer *container;
	TIntegerArray *lset;

	bool PostDialog(SBE *sbe, bool edit);
	bool OvaDlo(SBE *sbe, SBE *sbe1);

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);

	virtual TGroup *NewGroup() const;
};  //{{TSbeListWindow}}
