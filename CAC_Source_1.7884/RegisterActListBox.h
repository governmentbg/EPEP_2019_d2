//{{TListBox = TRegisterActListBox}}
class TRegisterActListBox : public TGroupListBox
{
public:
	TRegisterActListBox(TWindow* parent, int resourceId, TGArray<TRegisterAct> *acts);

	void View();

protected:
	virtual TGroup *NewGroup() const;
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TRegisterActListBox}}
