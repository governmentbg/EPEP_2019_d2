//{{TListWindow = TKnownPersonListWindow}}
class TKnownPersonListWindow : public TGroupListWindow
{
public:
	TKnownPersonListWindow(TWindow* parent, int resourceId);

	void SetInvolvement(char tInvolvement);
	TKnownPerson *Person() { return (TKnownPerson *) Group(); }
	void Refresh() { TGroupListWindow::Refresh(); }

	void Registry();
	void List(long active);
	bool Active();
	void Addresses();

protected:
	char involvement;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);

//{{TKnownPersonListWindowRSP_TBL_BEGIN}}
	void EvKeyDown(uint key, uint repeatCount, uint flags);
//{{TKnownPersonListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TKnownPersonListWindow);
};    //{{TKnownPersonListWindow}}
