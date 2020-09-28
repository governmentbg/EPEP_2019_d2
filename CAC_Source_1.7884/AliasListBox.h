//{{TListBox = TAliasListBox}}
class TAliasListBox : public TGroupListBox
{
public:
	TAliasListBox(TWindow* parent, int resourceId, TAliasGroup *tAlias, const char *tXCrit, long tFlags);

protected:
	TAliasGroup *alias;
	const char *xCrit;
	long flags;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *) { alias->AliasCriteria(m, xCrit, flags); }
	virtual void LoadData(const char *s);

//{{TAliasListBoxVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAliasListBoxVIRTUAL_END}}
};    //{{TAliasListBox}}
