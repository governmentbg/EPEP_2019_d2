//{{TComboBox = TGroupComboBox}}
class TGroupComboBox : public TComboBox, public TGroupList
{
public:
	TGroupComboBox(TWindow* parent, int resourceId, TGroupArray *array, int size);
	TGroupComboBox(TWindow* parent, int resourceId, TGroupArray &array, int size);
	virtual ~TGroupComboBox();

protected:
	bool autoEnable;

	virtual int GetVisualIndex();
	virtual void SetVisualIndex(int index);
	virtual char *GetVisualText(int index, int column);
	virtual void FlushVisuals();

	virtual void InsertVisual(int index);
	virtual void RemoveVisual(int index);
	virtual void QuickPosition();
	virtual void Refresh();

//{{TGroupComboBoxVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGroupComboBoxVIRTUAL_END}}

//{{TGroupComboBoxRSP_TBL_BEGIN}}
	void EvChar(uint key, uint repeatCount, uint flags);
	void EvKeyDown(uint key, uint repeatCount, uint flags);
	void EvSetFocus(THandle hWndLostFocus);
	void EvKillFocus(THandle hWndGetFocus);
	void CBNDropDown();
	void CBNCloseUp();
//{{TGroupComboBoxRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGroupComboBox);
};    //{{TGroupComboBox}}
