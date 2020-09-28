//{{TListWindow = TXferWebListWindow}}
class TXferWebListWindow : public TGroupListWindow
{
public:
	TXferWebListWindow(TWindow* parent, int resourceId, TXferWebGroup *tWebGroup);

	TXferWebPrint *XferWeb() { return (TXferWebPrint *) Group(); }
	char XferWebType();

	void Search();
	void Query();
	void View();
	void Print();

protected:
	TXferWebGroup *webGroup;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *);
	virtual bool DeleteGroup(TGroup *group);
};  //{{TXferWebListWindow}}
