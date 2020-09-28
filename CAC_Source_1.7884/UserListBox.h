//{{TListBox = TUserListBox}}
class TUserListBox : public TAliasListBox
{
public:
	TUserListBox(TWindow* parent, int resourceId, const char *posts, long flags = FLAG_NULL, const char *tExclude = NULL);

	void GetMark(TGroupArray *tArray);
	void SetMark(TGArray<TUserAliasGroup> *users);

protected:
	const char *exclude;
	
	TUserAliasGroup *UserAt(int index) const { return ((TUserAliasGroup *) GroupAt(index)); }
	virtual void LoadData(const char *s);	// INTER, LIMIT
};    //{{TUserListBox}}
