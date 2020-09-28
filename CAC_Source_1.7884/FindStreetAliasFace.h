//{{TComboBox = TFindStreetAliasFace}}
class TFindStreetAliasFace : public TFindAddrAliasFace
{
public:
	TFindStreetAliasFace(TWindow* parent, int resourceId, const char *name, TFindUCLPAliasFace *tUCLP,
		long flags = FLAG_NULL);

	void Reset(long street);
	virtual void Add();

protected:
	TFindUCLPAliasFace *uclp;
	bool numbered;

	virtual bool IsValid();
	virtual bool EditGroup(TGroup *group);

	virtual void GetGroupText(mstr &m, TGroup *group, int column);
	virtual void Criteria(msql &m, TGroup *group);

//{{TFindStreetAliasFaceRSP_TBL_BEGIN}}
	void EvSetFocus();
	void CBNDropDown();
//{{TFindStreetAliasFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFindStreetAliasFace);
};    //{{TFindStreetAliasFace}}
