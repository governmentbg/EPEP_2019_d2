//{{TListBox = TEditPaymentListBox}}
class TEditPaymentListBox : public TCloneListBox
{
public:
	TEditPaymentListBox(TWindow* parent, int resourceId);

	bool Search(const TRCDKey &tKey);
	void Search(TEditPaymentListBox *that);
	void Select(const char *ucn, char ucnType);
	void Flush() { FlushVisuals(); array->Flush(); }
	TUCNGroup *UCNGroup() { return (TUCNGroup *) Group(); }
	TUCNGroup *UCNGroupAt(int index) { return (TUCNGroup *) GroupAt(index); }

protected:
	TRCDKey key;

	virtual void Criteria(msql &m, TGroup *group);

//{{TEditPaymentListBoxRSP_TBL_BEGIN}}
	void EvSetFocus(THandle hWndLostFocus);
//{{TEditPaymentListBoxRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditPaymentListBox);
};    //{{TEditPaymentListBox}}
