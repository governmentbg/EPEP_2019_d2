//{{TComboBox = TFindUCLPAliasFace}}
class TFindUCLPAliasFace : public TFindAddrAliasFace
{
public:
	TFindUCLPAliasFace(TWindow* parent, int resourceId, const char *name, TAreaAliasFace *tArea, TRegionAliasFace *tRegion);

	virtual void Add();
	bool Search();

protected:
	TAreaAliasFace *area;
	TRegionAliasFace *region;

	virtual bool EditGroup(TGroup *group);
	virtual void Criteria(msql &m, TGroup *group);
};    //{{TFindUCLPAliasFace}}
