//{{TListWindow = TNTListWindow}}
class TNTListWindow : public TListWindow
{
public:
	TNTListWindow(TWindow* parent, int resourceId);

	void GetItem(TListWindItem &item);
	void SetItem(TListWindItem &item);
	void InsertItem(TListWindItem &item);
	void DeleteAnItem(int itemIndex);
	void InsertColumn(int colNum, TListWindColumn &colItem);
	int GetSelIndex(uint mask = LVIS_SELECTED) const;

//{{TNTListWindowVIRTUAL_BEGIN}}
public:
	virtual void SetupWindow();
//{{TNTListWindowVIRTUAL_END}}
};    //{{TNTListWindow}}
