//{{TComboBox = TAliasFace}}
class TAliasFace : public TGroupComboBox
{
public:
	TAliasFace(TWindow* parent, int resourceId, const char *tName, TAliasGroup *tAlias, const char *tXCrit, long tFlags,
		TGroupArray *array = NULL);
	virtual ~TAliasFace();

	const char *XCrit() { return xCrit; }
	void SetXCrit(const char *tXCrit);

protected:
	const char *name;
	TAliasGroup *alias;
	const char *xCrit;
	long flags;
	bool dropDown;
	char text[0x100];

	void GetText();
	int FindVisualIndex();
	void SetIndexNotify(int index);
	void SetDefaultIndex(int index);

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *) { alias->AliasCriteria(m, xCrit, flags); }
	virtual void LoadData(const char *s);
	virtual void Refresh();

	virtual bool IsValid();
	virtual void FlushVisuals();
	bool InsertEmptyOption();

//{{TAliasFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAliasFaceVIRTUAL_END}}

//{{TAliasFaceRSP_TBL_BEGIN}}
	long EvFaceValid(TParam1, TParam2);
	long EvFaceName(TParam1, TParam2 lParam);
	void CBNDropDown();
//{{TAliasFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAliasFace);
};    //{{TAliasFace}}
