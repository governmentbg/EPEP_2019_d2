//{{TListBox = TLegalSideAddressListBox}}
class TLegalSideAddressListBox : public TGroupListBox
{
public:
	TLegalSideAddressListBox(TWindow* parent, int resourceId, TGArray<TLegalSide> &legalSides);

	void Address();

protected:
	virtual bool IsValid();
	
	virtual TGroup *NewGroup() const { return NULL; }
};    //{{TLegalSideAddressListBox}}
