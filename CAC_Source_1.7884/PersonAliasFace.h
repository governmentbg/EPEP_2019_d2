//{{TComboBox = TPersonAliasFace}}
class TPersonAliasFace : public TStringAliasFace
{
public:
	TPersonAliasFace(TWindow* parent, int resourceId, const char *name, TPersonAliasGroup *alias, const char *xCrit,
		long flags, const char *tIinclude = NULL);

	const char *S();
	void SetS(const char *s);

protected:
	const char *include;

	virtual void LoadData(const char *s);
};    //{{TPersonAliasFace}}
