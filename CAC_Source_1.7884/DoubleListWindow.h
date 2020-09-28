//{{TListWindow = TDoubleListWindow}}
class TDoubleListWindow : public TColumnListWindow, public TQuickList
{
public:
	TDoubleListWindow(TWindow* parent, int resourceId, TGroupArray *tArray, int tAdjustWidth);

	TGroup *GroupAt(int index) { return (*array)[index]; }
	TGroup *Group();
	bool ItemChanging(TLwNotify &lwn);

	void Add();
	void Edit();
	void Delete();

private:
	bool doubleLock;
	int virtualFocus;
	bool columnsAdded;

protected:
	bool surroundIcon;
	bool allowDeselect;
	TGroupArray *array;

	void Add(TGroupListColumn *column) { TColumnListWindow::Add(column); }
	void Add(TGroupListColumn *column1, TGroupListColumn *column2, ...);

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);	// Note: BackEdit()
	virtual bool AskDelete(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);

	void InsertGroup(TGroup *group);
	void RemoveGroup(int command);
	int GetFocusedIndex();
	void FlushVisuals();

	virtual void GetSearchText(mstr &m, int index);
	virtual int GetVisualCount() { return array->Count() << 1; }
	virtual int GetVisualIndex() { return TColumnListWindow::GetSelIndex(LVIS_DROPHILITED); }
	virtual void SetVisualIndex(int index);

	virtual void InsertVisual(TGroup *group, int index) { DrawGroupAt(group, index, true); }
	void DrawGroupAt(TGroup *group, int index, bool insert);
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group) = 0;
	void RedrawFocus();

	virtual TGroup *NewGroup() const;
	void Refresh(int size);		// add columns and draw items
	bool Deselect();

//{{TDoubleListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TDoubleListWindowVIRTUAL_END}}

//{{TDoubleListWindowRSP_TBL_BEGIN}}
	void EvChar(uint key, uint repeatCount, uint flags);
	void EvKeyDown(uint key, uint repeatCount, uint flags);
	void EvSetFocus(THandle hWndLostFocus);
	void EvKillFocus(THandle hWndGetFocus);
	void EvSize(uint sizeType, TSize& size);
	void EvLButtonDown(uint, TPoint& point);
	void EvPaint();
//{{TDoubleListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TDoubleListWindow);
};    //{{TDoubleListWindow}}
