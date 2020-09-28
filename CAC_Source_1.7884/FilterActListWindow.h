//{{TListWindow = TFilterActListWindow}}
class TFilterActListWindow : public TGroupListWindow
{
public:
	TFilterActListWindow(TWindow* parent, int resourceId, TUnfilteredActsGroup *tUnfiltered);

	void Filter();
	void Query();
	virtual void Refresh();

protected:
	TUnfilteredActsGroup *unfiltered;
	TGArray<TDecision> invisible;		// 2016:015 for rider->decision

	bool Inserted(TIndexContainer *container, long subject, char lawKind);
	bool Inserted(TDecision *decision, long subject, char lawKind);

	virtual TGroup *NewGroup() const { return NULL; }
	virtual void Criteria(msql &, TGroup *) { }
	virtual void LoadData(const char *);
};    //{{TFilterActListWindow}}
