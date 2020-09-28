//{{TDialog = TQuickDialog}}
class TQuickDialog : public TScaleDialog
{
public:
	TQuickDialog(TWindow* parent, int resId);

	static UINT timer;

private:
	TQuickSearch qs;
	TQuickList *qs_list;
	int flashIndex;
	int flashDelay;

	void QuickSearch(int index);

protected:
//{{TQuickDialogRSP_TBL_BEGIN}}
	long EvQuickSearch(TParam1, TParam2 lParam);
	void QuickSearchPrevBNClicked();
	void QuickSearchPrevBNEnable(TCommandEnabler &tce);
	void QuickSearchEditBNClicked();
	void QuickSearchNextBNClicked();
	void QuickSearchNextBNEnable(TCommandEnabler &tce);
	void EvTimer(uint);
//{{TQuickDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQuickDialog);
};    //{{TQuickDialog}}
