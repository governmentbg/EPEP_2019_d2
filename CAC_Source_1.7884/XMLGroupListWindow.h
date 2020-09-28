//{{TListWindow = TXMLGroupListWindow}}
class TXMLGroupListWindow : public TGroupListWindow
{
public:
	TXMLGroupListWindow(TWindow* parent, int resourceId, TXMLArray *array, int adjustWidth);

	int Count() { return array->Count(); }
	void Deselect() { TGroupListWindow::SetVisualIndex(-1); }

protected:
	virtual TGroup *NewGroup() const;
};    //{{TXMLGroupListWindow}}
