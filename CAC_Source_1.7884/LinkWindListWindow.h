//{{TListWindow = TLinkWindListWindow}}
class TLinkWindListWindow : public TGroupListWindow
{
public:
	TLinkWindListWindow(TWindow* parent, int resourceId, TLinkWindArray *linkWinds, TGArray<TSideWind> *tOrdinary,
		TGArray<TSideWind> *tProxy, TGArray<TSideWind> *tRedirect);

	int Count() { return array->Count(); }
	void Edit() { BackEdit(); }

protected:
	TLinkWindArray *LinkWinds() { return (TLinkWindArray *) Array(); }

	TGArray<TSideWind> *ordinary;
	TGArray<TSideWind> *proxy;
	TGArray<TSideWind> *redirect;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *);
};    //{{TLinkWindListWindow}}
