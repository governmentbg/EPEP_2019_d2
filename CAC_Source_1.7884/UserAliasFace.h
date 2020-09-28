//{{TComboBox = TUserAliasFace}}
class TUserAliasFace : public TPersonAliasFace
{
public:
	TUserAliasFace(TWindow* parent, int resourceId, const char *name, const char *xCrit,
		long flags = FLAG_ACTIVE | ALIAS_EMPTY, const char *tExclude = NULL);

	void SetInter(bool inter);

protected:
	const char *exclude;
	
	virtual void LoadData(const char *s);
};    //{{TUserAliasFace}}
