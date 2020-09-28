//{{TListWindow = TGroupListWindow}}
class TGroupListWindow : public TColumnListWindow, public TGroupList
{
public:
	TGroupListWindow(TWindow* parent, int resourceId, TGroupArray *array, int adjustWidth);
	TGroupListWindow(TWindow* parent, int resourceId, TGroupArray &array, int adjustWidth);

	bool ItemChanging(TLwNotify &lwn);

	void Add();
	void Edit();
	void Delete();

protected:
	TListWEmpty emptyRect;
	bool allowDeselect;

	void Init();
	void Add(TGroupListColumn *column) { TColumnListWindow::Add(column); }
	void Add(TGroupListColumn *column1, TGroupListColumn *column2, ...);
	void BackEdit();	// edit current group, backup w/ new group

	virtual int GetVisualIndex();
	virtual void SetVisualIndex(int index);
	virtual void FlushVisuals();

	virtual void InsertVisual(int index);
	virtual void RedrawVisual(int index);
	virtual void RemoveVisual(int index);
	virtual void UpdateVisual();

	virtual void GetSearchText(mstr &m, int index);
	virtual void GetGroupText(mstr &m, TGroup *group, int column);
	virtual char *GetVisualText(int index, int column);

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);

	virtual void Refresh();			// draw lock count
	void RedrawGroup();			// RedrawVisual(GetVisualIndex)
	void RemoveGroup(int command);

	void DrawItemAt(int index, bool insert);
	bool EmptyRect();
	void DrawLockCount(int count = 28) { drawLockCount = count; }

//{{TGroupListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGroupListWindowVIRTUAL_END}}

//{{TGroupListWindowRSP_TBL_BEGIN}}
	void EvPaint();
	void EvChar(uint key, uint repeatCount, uint flags);
	void EvKeyDown(uint key, uint repeatCount, uint flags);
	void EvSetFocus(THandle hWndLostFocus);
	void EvKillFocus(THandle hWndGetFocus);
	void EvLButtonDblClk(uint modKeys, TPoint& point);
//{{TGroupListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGroupListWindow);
};    //{{TGroupListWindow}}
