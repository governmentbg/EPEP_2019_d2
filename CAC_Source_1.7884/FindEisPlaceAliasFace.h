//{{TComboBox = TFindEisPlaceAliasFace}}
class TFindEisPlaceAliasFace : public TEisAliasFace
{
public:
	TFindEisPlaceAliasFace(TWindow* parent, int resourceId, const char *name, TEisRegionAliasFace *tRegion,
		TXMLObject *object);

	void Reset(long value);
	long LNow();

protected:
	TEisRegionAliasFace *region;

	virtual int GetVisualIndex();
	virtual void Criteria(msql &m, TGroup *group);

//{{TFindEisPlaceAliasFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TFindEisPlaceAliasFaceVIRTUAL_END}}

//{{TFindEisPlaceAliasFaceRSP_TBL_BEGIN}}
	void CBNDropDown();
//{{TFindEisPlaceAliasFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFindEisPlaceAliasFace);
};    //{{TFindEisPlaceAliasFace}}
