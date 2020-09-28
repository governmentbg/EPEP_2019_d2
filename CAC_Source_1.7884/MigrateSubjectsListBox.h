//{{TListBox = TMigrateSubjectsListBox}}
class TMigrateSubjectsListBox : public TCloneListBox
{
public:
	TMigrateSubjectsListBox(TWindow* parent, int resourceId);

	void SetKind(char tKind);
	void RemoveGroup() { TCloneListBox::RemoveGroup(); }

protected:
	char kind;
	char type;

	void InsertVisual(int index);	
	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
	void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TMigrateSubjectsListBox}}
