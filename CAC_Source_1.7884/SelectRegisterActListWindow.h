//{{TListBox = TSelectRegisterActListWindow}}
class TSelectRegisterActListWindow : public TGroupListWindow
{
public:
	TSelectRegisterActListWindow(TWindow* parent, int resourceId, TGArray<TRegisterAct> *acts);

	void View();

protected:
	virtual TGroup *NewGroup() const;
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TSelectRegisterActListWindow}}
