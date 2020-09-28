//{{TListBox = TGroupListBox}}
class TGroupListBox : public TListBox, public TGroupList
{
public:
	TGroupListBox(TWindow* parent, int resourceId, TGroupArray *array);
	TGroupListBox(TWindow* parent, int resourceId, TGroupArray &array);

#if TYPECHECK
	TGroup *Group() const;
#endif  // TYPECHECK
	void GetMark(TGroupArray *tArray);
	void CutMark();	// N.B. invalidates the list
	void SetHorizExtent() { UpdateVisual(); }	// for compatibility
	void SelectAll(bool select = true);

protected:
	bool multiSelect;
	bool multiColumn;

	virtual int GetVisualIndex();
	virtual void SetVisualIndex(int index);
	virtual char *GetVisualText(int index, int column);
	virtual void FlushVisuals();

	virtual void InsertVisual(index);
	virtual void RemoveVisual(index);
	virtual void UpdateVisual();

	virtual bool IsValid();		// marked count <= MARKEDS_MAX

	bool Inside(TPoint &point);
	void LButton(TPoint &point);
	int get_list_box_maxc_width();	// 2011:083 now method

//{{TGroupListBoxVIRTUAL_BEGIN}}
protected:
	virtual void SetupWindow();
//{{TGroupListBoxVIRTUAL_END}}

//{{TGroupListBoxRSP_TBL_BEGIN}}
	void EvChar(uint key, uint repeatCount, uint flags);
	void EvKeyDown(uint key, uint repeatCount, uint flags);
	void EvSetFocus(THandle hWndLostFocus);
	void EvKillFocus(THandle hWndGetFocus);
	void EvLButtonDown(uint, TPoint &point);
	void EvLButtonUp(uint, TPoint &point);
	void EvLButtonDblClk(uint, TPoint &point);
	void EvRButtonDown(uint modKeys, TPoint &point);
	void EvRButtonUp(uint modKeys, TPoint &point);
	void EvRButtonDblClk(uint modKeys, TPoint &point);
	long EvFaceValid(TParam1, TParam2);
//{{TGroupListBoxRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGroupListBox);
};  //{{TGroupListBox}}
