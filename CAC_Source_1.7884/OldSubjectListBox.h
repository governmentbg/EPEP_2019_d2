//{{TListBox = TOldSubjectListBox}}
class TOldSubjectListBox : public TCloneListBox
{
public:
	TOldSubjectListBox(TWindow* parent, int resourceId, const char *tKinds = NULL);

	void SetMark(TGArray<TOldSubject> *oldSubjects);
	const char *Kinds() { return kinds; }

protected:
	const char *kinds;

	virtual void Criteria(msql &m, TGroup *group);
	virtual void GetGroupText(mstr &m, TGroup *group, int);

	TOldSubject *OldSubjectAt(int index) { return (TOldSubject *) GroupAt(index); }
};    //{{TOldSubjectListBox}}
