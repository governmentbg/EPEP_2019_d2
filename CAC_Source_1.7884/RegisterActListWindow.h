//{{TListWindow = TRegisterActListWindow}}
class TRegisterActListWindow : public TGroupListWindow
{
public:
	TRegisterActListWindow(TWindow* parent, int resourceId, TDateFace *tParentDate);

	void SetCrit(char tKind, const CDate &tDate);
	bool DiffDate(const TRegisterAct *act);
	void View();

protected:
	char kind;
	CDate date;
	TDateFace *parentDate;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TRegisterActListWindow}}
