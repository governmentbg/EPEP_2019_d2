//{{TListWindow = TBankruptSendListBox}}
class TBankruptSendListBox : public TGroupListBox
{
public:
	TBankruptSendListBox(TWindow* parent, int resourceId, TLawsuit *tLawsuit);

	void Export();
	void Query();

protected:
	TLawsuit *lawsuit;

	TRCDKeyContainer *ContainerAt(int index) const { return (TRCDKeyContainer *) GroupAt(index); }

	virtual TGroup *NewGroup() const { return NULL; }
	void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TBankruptSendListBox}}
