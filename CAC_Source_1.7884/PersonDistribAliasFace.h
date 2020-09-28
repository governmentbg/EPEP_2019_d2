//{{TComboBox = TPersonDistribAliasFace}}
class TPersonDistribAliasFace : public TPersonAliasFace
{
public:
	TPersonDistribAliasFace(TWindow* parent, int resourceId, const char *name, TPersonAliasGroup *alias, char tLawKind);
	TPersonDistribAliasFace(TWindow* parent, int resourceId, const char *name, TPersonAliasGroup *alias, TGroupArray *tPersons, long flags);
	virtual ~TPersonDistribAliasFace();

	void SetFlags(long tFlags);

protected:
	char lawKind;
	TGroupArray *persons;

	virtual void Criteria(msql &m, TGroup *group);
	virtual void LoadData(const char *s);
};    //{{TPersonDistribAliasFace}}
