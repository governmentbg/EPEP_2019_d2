//{{TListBox = TSelectLinkedWindsListBox}}
class TSelectLinkedWindsListBox : public TGroupListBox
{
public:
	TSelectLinkedWindsListBox(TWindow* parent, int resourceId, TGArray<TLinkWind> *array);

	void UpdateSelect(int tNotifySelect);
	virtual void Refresh() { TGroupListBox::Refresh(); }

protected:
	TLinkWind *LinkWindAt(index) { return TYPECHECK_PTR_CAST(TLinkWind, GroupAt(index)); }
	int notifySelect;

	void RestoreSelect();		// reselect the 1st if legal aid
	virtual bool IsValid();		// check for 1st if exec list / legal aid

	virtual TGroup *NewGroup() const { return new TLinkWind; }
	virtual void GetGroupText(mstr &m, TGroup *group, int);

//{{TSelectLinkedWindsListBoxRSP_TBL_BEGIN}}
	void EvLButtonDown(uint modKeys, TPoint& point);
	void EvRButtonDown(uint modKeys, TPoint& point);
	void EvKeyDown(uint key, uint repeatCount, uint flags);
//{{TSelectLinkedWindsListBoxRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectLinkedWindsListBox);
};    //{{TSelectLinkedWindsListBox}}
