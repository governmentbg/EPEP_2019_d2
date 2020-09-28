//{{TListBox = TSelectUCLPListBox}}
class TSelectUCLPListBox : public TCloneListBox
{
public:
	TSelectUCLPListBox(TWindow* parent, int resourceId, char *tName);

protected:
	char *name;

	virtual void GetGroupText(mstr &m, TGroup *group, int column);
	virtual void Criteria(msql &m, TGroup *group);
};    //{{TSelectUCLPListBox}}
