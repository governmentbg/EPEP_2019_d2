//{{TListBox = TBankruptListBox}}
class TBankruptListBox : public TGroupListBox
{
public:
	TBankruptListBox(TWindow* parent, int resourceId, TGArray<TBankrupt> *bankrupts, int tSelect);

protected:
	int select;

	virtual int MarkedsMax() { return 10000; }	// 2013:120 MRQ
	virtual TGroup *NewGroup() const;

//{{TBankruptListBoxVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TBankruptListBoxVIRTUAL_END}}
};    //{{TBankruptListBox}}
