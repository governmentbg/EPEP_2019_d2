//{{TListBox = TSubjectListBox}}
class TSubjectListBox : public TCloneListBox
{
public:
	TSubjectListBox(TWindow* parent, int resourceId, const char *tKinds = NULL);

	void SetMark(TGArray<TSubject> *subjects);
	void SetKinds(const char *tKinds);
	const char *Kinds() { return kinds; }

protected:
	const char *kinds;
	bool hidden; 

	virtual void LoadData(const char *s);
	virtual void Criteria(msql &m, TGroup *group);
	virtual void GetGroupText(mstr &m, TGroup *group, int);

	TSubject *SubjectAt(int index) { return (TSubject *) GroupAt(index); }
};    //{{TSubjectListBox}}
