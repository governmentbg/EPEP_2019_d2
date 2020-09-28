//{{TListBox = TUserLoadListBox}}
class TUserLoadListBox : public TGroupListBox
{
public:
	TUserLoadListBox(TWindow* parent, int resourceId, TGArray<TUser> *array, bool tMulti);

	virtual void Refresh() { TGroupListBox::Refresh(); }
	void Change(const char *oldJudge, const char *newJudge);
	void GetMark(TGArray<TUser> *tUsers);	// group pointers, not copies

protected:
	int multi;

	TUser *UserAt(int index) const { return (TUser *) GroupAt(index); }
	virtual TGroup *NewGroup() const;
	void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TUserLoadListBox}}
