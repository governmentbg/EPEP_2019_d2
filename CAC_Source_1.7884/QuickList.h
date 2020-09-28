class TQuickList : virtual protected TWindow
{
public:
	TQuickList(TWindow *parent, QUICK_SEARCH_MODE tQSMode);

	void SetQuickSearch(TQuickSearch *tQS);
	void QuickSearchEdit() { QuickDestroy(); }
	void QuickSearch(int step, const char *text = NULL) { QuickSearch(step, true, text); }
	const char *ABCSkip(const char *s, bool spaces);

protected:
	TQuickSearch *qs;
	QUICK_SEARCH_MODE qsMode;
	int abcskip;

	virtual bool QsStart(const char *text);

	virtual void GetSearchText(mstr &m, int index) = 0;
	virtual int GetVisualCount() = 0;
	virtual int GetVisualIndex() = 0;
	virtual void SetVisualIndex(int index) = 0;

	void QuickRefresh();
	virtual void QuickPosition();
	bool QuickSearch(int step, bool jump, const char *text = NULL);
	void QuickDestroy(bool force = false);

	bool QuickChar(uint key);
	bool QuickKey(uint key);
	void QuickSetFocus();
	void QuickKillFocus();
};
