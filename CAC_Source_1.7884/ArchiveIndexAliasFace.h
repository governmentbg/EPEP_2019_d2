//{{TComboBox = TArchiveIndexAliasFace}}
class TArchiveIndexAliasFace : public TStringAliasFace
{
public:
	TArchiveIndexAliasFace(TWindow* parent, int resourceId, const char *name, TExternContainer *tLawsuit, long flags);

protected:
	TExternContainer *lawsuit;

	virtual void Criteria(msql &m, TGroup *group);
};    //{{TArchiveIndexAliasFace}}
