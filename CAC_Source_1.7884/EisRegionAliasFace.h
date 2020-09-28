//{{TComboBox = TEisRegionAliasFace}}
class TEisRegionAliasFace : public TEisAliasFace
{
public:
	TEisRegionAliasFace(TWindow* parent, int resourceId, const char *name, TEisAreaAliasFace *tArea, TXMLObject *object);

	void Reset(long region);

protected:
	TEisAreaAliasFace *area;

//{{TEisRegionAliasFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEisRegionAliasFaceVIRTUAL_END}}

	virtual void Criteria(msql &m, TGroup *group);
};    //{{TEisRegionAliasFace}}
