//{{TComboBox = TDangerAliasFace}}
class TDangerAliasFace : public TLongAliasFace
{
public:
	TDangerAliasFace(TWindow* parent, int resourceId, const char *name, long tSubject, long flags);

protected:
	long subject;

	virtual void Criteria(msql &m, TGroup *group);
};    //{{TDangerAliasFace}}
