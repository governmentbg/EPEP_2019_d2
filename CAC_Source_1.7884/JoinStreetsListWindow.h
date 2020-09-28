class TJoinStreetsListWindow : public TGroupListWindow
{
public:
	TJoinStreetsListWindow(TWindow* parent, int resourceId, TGSArray<TJoinStreet> *streets);

	TJoinStreet *Street() { return (TJoinStreet *) Group(); }
	void Join(TJoinStreet *target, TJoinStreetsListWindow *targets);

	virtual TGroup *NewGroup() const { return NULL; }
};