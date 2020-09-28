//{{TListBox = TSelectJurorListBox}}
class TSelectJurorListBox : public TCloneListBox
{
public:
	TSelectJurorListBox(TWindow* parent, int resourceId, TGArray<TDistribJuror> *jurors);

protected:	
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TSelectJurorListBox}}
