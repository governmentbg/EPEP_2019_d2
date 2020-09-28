//{{TListBox = TExecListPersonListBox}}
class TExecListPersonListBox : public TGroupListBox
{
public:
	TExecListPersonListBox(TWindow* parent, int resourceId, TGArray<TExecListPerson> *allPersons, bool tPrivList);

	TExecListPerson *PersonAt(int i) { return (TExecListPerson *) GroupAt(i); }
	void Address();

	void SetMark(TGArray<TExecListPerson> *persons);
	void GetMark(TGArray<TExecListPerson> *persons);

protected:
	bool privList;

	virtual bool IsValid();		// 1 person once per sum

	virtual TGroup *NewGroup() const;
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TExecListPersonListBox}}
