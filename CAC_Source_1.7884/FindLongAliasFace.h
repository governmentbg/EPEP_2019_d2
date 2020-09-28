//{{TComboBox = TFindLongAliasFace}}
class TFindLongAliasFace : public TLongAliasFace
{
public:
	TFindLongAliasFace(TWindow* parent, int resourceId, const char *name, uint textLimit, TLongAliasGroup *alias, const char *xCrit, long flags);

	long LNow();

protected:
	virtual int GetVisualIndex();
};    //{{TFindLongAliasFace}}
