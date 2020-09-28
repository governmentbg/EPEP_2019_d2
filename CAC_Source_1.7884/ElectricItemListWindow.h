//{{TListWindow = TElectricItemListWindow}}
class TElectricItemListWindow : public TGroupListWindow
{
public:
	TElectricItemListWindow(TWindow* parent, int resourceId, TRCDKeyContainer *tContainer, TGSArray<TElectric> *items,
		const TLawsuit *tLawsuit);

	void ViewGroup();
	void Edit() { BackEdit(); }
	TElectric *Electric() { return (TElectric *) Group(); }

protected:
	TRCDKeyContainer *container;
	const TLawsuit *lawsuit;

	virtual TGroup *NewGroup() const;

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TElectricItemListWindow}}
