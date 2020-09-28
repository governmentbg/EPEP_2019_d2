//{{TComboBox = TStringAliasFace}}
class TStringAliasFace : public TAliasFace
{
public:
	TStringAliasFace(TWindow* parent, int resourceId, const char *name, TStringAliasGroup *alias, const char *xCrit,
		long flags = FLAG_NULL, TGroupArray *array = NULL);

	const char *S();
	virtual void SetS(const char *s);
	virtual void Refresh();

protected:
	const char *StringAt(int index) { return ((TStringAliasGroup *) GroupAt(index))->ValueField()->S(); }
};    //{{TStringAliasFace}}
