//{{TListWindow = TXferDeleteListWindow}}
class TXferDeleteListWindow : public TGroupListWindow
{
public:
	TXferDeleteListWindow(TWindow* parent, int resourceId);

	void SetCourt(long tCount);
	TXferDocument *Document() { return (TXferDocument *) Group(); }

protected:
	long court;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
	virtual bool AskDelete(TGroup *);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TXferDeleteListWindow}}
