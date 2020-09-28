//{{TListBox = TBanConSideListBox}}
class TBanConSideListBox : public TGroupListBox
{
public:
	TBanConSideListBox(TWindow* parent, int resourceId, TGArray<TBankWind> *bankWinds);

	void Cert(const char *title);
	void Query();
	void Print();

protected:
	virtual TGroup *NewGroup() const;
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TBanConSideListBox}}
