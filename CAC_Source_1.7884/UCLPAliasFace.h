//{{TComboBox = TUCLPAliasFace}}
class TUCLPAliasFace : public TLongAliasFace
{
public:
	TUCLPAliasFace(TWindow* parent, int resourceId, const char *name, long tUCLP);

protected:
	long uclp;

	virtual void Criteria(msql &m, TGroup *group);
	virtual void Refresh();
};    //{{TUCLPAliasFace}}
