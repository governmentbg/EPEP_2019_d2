//{{TListBox = TSelectUCNListBox}}
class TSelectUCNListBox : public TGroupListBox
{
public:
	TSelectUCNListBox(TWindow* parent, int resourceId, const char *tUCNTypes, bool tActive, bool tStat);

	void FlushList();
	TUCNGroup *UCNGroup() { return (TUCNGroup *) Group(); }
	TUCNGroup *UCNGroupAt(int index) { return (TUCNGroup *) GroupAt(index); }
	void Print(char uxnType);
	void Address();

	void AddUCNGroup(TNamedGroup *group, bool extra, char involvement);
	void SetUCNGroup(TNamedGroup *group, bool extra, char involvement);

protected:
	const char *ucnTypes;
	const bool active;
	const bool stat;		// 2018-01-23 EQUAL-s: exclude current court

	virtual void GetGroupText(mstr &m, TGroup *group, int column);
	virtual TGroup *NewGroup() const;

//{{TSelectUCNListBoxRSP_TBL_BEGIN}}
	void EvKeyDown(uint key, uint repeatCount, uint flags);
	void EvLButtonDown(uint, TPoint &point);
	void EvLButtonDblClk(uint modKeys, TPoint &point);
//{{TSelectUCNListBoxRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectUCNListBox);
};    //{{TSelectUCNListBox}}
