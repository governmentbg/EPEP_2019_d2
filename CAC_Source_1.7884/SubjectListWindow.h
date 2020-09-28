//{{TListWindow = TSubjectListWindow}}
class TSubjectListWindow : public TGroupListWindow
{
public:
	TSubjectListWindow(TWindow* parent, int resourceId);

	void SetKind(char tKind);
	TSubject *Subject() { return (TSubject *) Group(); }

	void Copy();
	void Print();
	void PrintOld();

protected:
	char kind;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);	// kind
	virtual void Refresh();

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
};    //{{TSubjectListWindow}}
