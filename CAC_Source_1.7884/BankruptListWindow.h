//{{TListWindow = TBankruptListWindow}}
class TBankruptListWindow : public TGroupListWindow
{
public:
	TBankruptListWindow(TWindow* parent, int resourceId);

	void SetCreditor(const char *tCreditor);
	void Query(bool select);
	void Load();

protected:
	char creditor[SIZE_OF_UCN];

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TBankruptListWindow}}
