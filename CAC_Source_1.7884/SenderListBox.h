//{{TListBox = TSenderListBox}}
class TSenderListBox : public TCloneListBox
{
public:
	TSenderListBox(TWindow* parent, int resourceId, const char *tUCNTypes);

protected:
	const char *ucnTypes;

	virtual void Criteria(msql &m, TGroup *group);
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TSenderListBox}}
