//{{TListWindow = TColumnListWindow}}
class TColumnListWindow : public TNTListWindow
{
public:
	TColumnListWindow(TWindow* parent, int resourceId, int tAdjustWidth);

protected:
	int adjustWidth;
	TIArray<TGroupListColumn> columns;
	int initialWidth;
	int drawLockCount;

	void Add(TGroupListColumn *column) { columns.Add(column); }
	void Add(TGroupListColumn *column1, TGroupListColumn *column2, ...);
	void Add(TGroupListColumn *column1, TGroupListColumn *column2, va_list ap);

	void AddColumns(int targetWidth);
	virtual int GetImage(TGroup *group) { return -1; }
	void DrawGroupAt(TGroup *group, int index, bool insert);

	void FlushVisuals() { DeleteAllItems(); }
	void UpdateVisual();

	virtual void GetGroupText(mstr &m, TGroup *group, int column);
	bool UnblockFocus(int command);
	void ParentNotify(int command);

//{{TColumnListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TColumnListWindowDialogVIRTUAL_END}}

//{{TColumnListWindowRSP_TBL_BEGIN}}
	void EvRButtonDown(uint, TPoint&);
	void EvRButtonUp(uint, TPoint&);
	void EvRButtonDblClk(uint, TPoint&);
//{{TColumnListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TColumnListWindow);
};    //{{TDoubleListWindow}}
