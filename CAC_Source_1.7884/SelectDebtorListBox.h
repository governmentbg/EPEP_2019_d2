//{{TListBox = TSelectDebtorListBox}}
class TSelectDebtorListBox : public TCloneListBox
{
public:
	TSelectDebtorListBox(TWindow* parent, int resourceId, TGArray<TExecDebtor> *debtors);

	void Select(const char *ucn, char ucnType);
	TExecDebtor *ExecDebtor() { return TYPECHECK_PTR_CAST(TExecDebtor, Group()); }

protected:
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TSelectDebtorListBox}}
