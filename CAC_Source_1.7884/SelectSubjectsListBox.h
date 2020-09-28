//{{TListBox = TSelectSubjectsListBox}}
class TSelectSubjectsListBox : public TSubjectListBox
{
public:
	TSelectSubjectsListBox(TWindow* parent, int resourceId, const char *kinds, bool tSaas, bool tHidden);

	void SetMark(TGArray<TSubject> *subjects);
	void Extend();

protected:
	bool saas;
	mstr extended;

	virtual void LoadData(const char *s);
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TSelectSubjectsListBox}}
