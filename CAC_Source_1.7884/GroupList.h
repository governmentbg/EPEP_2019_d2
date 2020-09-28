class TGroupList : public TQuickList
{
public:
	TGroupList(TWindow *parent, TGroupArray *tArray);
	TGroupList(TWindow *parent, TGroupArray &tArray);
	virtual ~TGroupList();

	TGroup *Group();
	TGroup *GroupAt(int index) const { return (*array)[index]; }
	TGroupArray *Array() const { return array; }
	friend void TAliasGroup::CacheInsert(TGroupList *glist, TGroup *group);

protected:
	virtual char *GetVisualText(int index, int column) = 0;
	virtual void FlushVisuals() = 0;
	virtual int GetVisualCount();

	virtual void InsertVisual(int index) = 0;
	virtual void RedrawVisual(int index);	// Remove-Insert-Visual
	virtual void RemoveVisual(int index) = 0;
	virtual void UpdateVisual() { }		// update width(s)

	TGroupArray *array;
	bool dynamic;
	bool loadData;
	bool initialRefresh;

	int sortcol;

	virtual TGroup *NewGroup() const = 0;
	virtual void Criteria(msql &m, TGroup *group);
	virtual void LoadData(const char *s);
	virtual void Refresh();

	int InsertGroup(TGroup *group);
	void InsertEmpty();
	void ResortGroup();
	void RemoveGroup();

	virtual void GetGroupText(mstr &m, TGroup *group, int column);
	virtual void GetSearchText(mstr &m, int index);
	int FindABCIndex(const char *str);
};
