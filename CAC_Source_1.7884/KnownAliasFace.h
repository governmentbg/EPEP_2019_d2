//{{TComboBox = TKnownAliasFace}}
class TKnownAliasFace : public TPersonAliasFace
{
public:
	TKnownAliasFace(TWindow* parent, int resourceId, const char *name, const char *xCrit, long flags, const char *include = NULL);

protected:
	virtual void GetGroupText(mstr &m, TGroup *group, int);	// 2010:267 LPR/URQ: "ï " for pedagog
};    //{{TKnownAliasFace}}
