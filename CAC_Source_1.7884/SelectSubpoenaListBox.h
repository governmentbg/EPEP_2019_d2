//{{TListBox = TSelectSubpoenaListBox}}
class TSelectSubpoenaListBox : public TCloneListBox
{
public:
	TSelectSubpoenaListBox(TWindow* parent, int resourceId, TGArray<TSubpoena> *subpoenas);

	void Select(const char *ucn, char ucnType);
	TSubpoena *Subpoena() { return TYPECHECK_PTR_CAST(TSubpoena, Group()); }

protected:
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TSelectSubpoenaListBox}}
