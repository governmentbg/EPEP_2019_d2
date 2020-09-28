//{{TListBox = TDangerListBox}}
class TDangerListBox : public TCloneListBox
{
public:
	TDangerListBox(TWindow* parent, int resourceId);

protected:
	virtual void Criteria(msql &m, TGroup *group);
};    //{{TDangerListBox}}
