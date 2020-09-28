//{{TListBox = TArchiveIndexListBox}}
class TArchiveIndexListBox : public TCloneListBox
{
public:
	TArchiveIndexListBox(TWindow* parent, int resourceId, bool empty);

	void SetMark(TGArray<TArchiveIndex> *indexes);

protected:
	bool empty;

	TArchiveIndex *ArchiveIndexAt(int index) { return (TArchiveIndex *) GroupAt(index); }

	virtual void LoadData(const char *s);
	virtual void GetGroupText(mstr &m, TGroup *group, int column);
};    //{{TArchiveIndexListBox}}
