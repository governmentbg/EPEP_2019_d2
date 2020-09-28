//{{TListWindow = TEmptyListWindow}}
class TEmptyListWindow : public TCadrListWindow
{
public:
	TEmptyListWindow(TWindow* parent, int resourceId);

	virtual void Activate();
	
protected:
	virtual TGroup *NewGroup() const;
	virtual void Refresh();
};    //{{TEmptyListWindow}}
