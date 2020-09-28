//{{TListWindow = TExpenseListWindow}}
class TExpenseListWindow : public TGroupListWindow
{
public:
	TExpenseListWindow(TWindow* parent, int resourceId, TSideWind *tSideWind, TRCDKeyContainer *tContainer,
		TSession *tSession, TLawsuit *tLawsuit);

	void Penality();
	void Reward();

protected:
	TSideWind *sideWind;
	TRCDKeyContainer *container;
	TSession *session;
	TLawsuit *lawsuit;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);	// 2010:083 LPR: was TaxGroup
	virtual bool EditGroup(TGroup *group);
};    //{{TExpenseListWindow}}
