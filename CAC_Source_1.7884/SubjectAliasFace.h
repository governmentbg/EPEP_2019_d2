//{{TComboBox = TSubjectAliasFace}}
class TSubjectAliasFace : public TLongAliasFace
{
public:
	TSubjectAliasFace(TWindow* parent, int resourceId, const char *name, char kind, long tInclude = 0);
	// distrib; 2016:027 comout +related
	//TSubjectAliasFace(TWindow* parent, int resourceId, const char *name, char kind, TGArray<TSubject> *tSubjects);

	void SetKind(char kind);

protected:
	long include;				// 2010:271 "subject" -> "include"

	//TGArray<TSubject> *subjects;	// 2007:180 LPR: assignment limitation
	//void InitKinds(char kind);

	virtual bool QsStart(const char *text);
	virtual void Criteria(msql &m, TGroup *group);
	virtual void LoadData(const char *s);
	virtual void GetGroupText(mstr &m, TGroup *group, int column);
};    //{{TSubjectAliasFace}}
