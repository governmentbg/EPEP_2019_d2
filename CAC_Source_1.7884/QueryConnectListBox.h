//{{TListBox = TQueryConnectListBox}}
class TQueryConnectListBox : public TGroupListBox
{
public:
	TQueryConnectListBox(TWindow* parent, int resourceId, TConnect *tConnect);
	virtual ~TQueryConnectListBox();

	void Print();

protected:
	TConnect *connect;

	virtual void GetGroupText(mstr &m, TGroup *group, int);
	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *);
};    //{{TQueryConnectListBox}}
