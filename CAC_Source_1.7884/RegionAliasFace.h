//{{TComboBox = TRegionAliasFace}}
class TRegionAliasFace : public TLongAliasFace
{
public:
	TRegionAliasFace(TWindow* parent, int resourceId, const char *name, TAreaAliasFace *tArea, long flags = FLAG_NULL);

	void Reset(long region);

protected:
	TAreaAliasFace *area;

	virtual void Criteria(msql &m, TGroup *group);
};    //{{TRegionAliasFace}}
