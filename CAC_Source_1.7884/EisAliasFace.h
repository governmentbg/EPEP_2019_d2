//{{TComboBox = TEisAliasFace}}
class TEisAliasFace : public TLongAliasFace
{
public:
	TEisAliasFace(TWindow* parent, int resourceId, const char *name, long tNom, TXMLObject *object,
		long tFlags = FLAG_ACTIVE);	// 2014:014 IRQ/TRQ: was FLAG_NULL
	virtual ~TEisAliasFace();

	void SetL(long l);

protected:
	long nom;
	TXMLInt *attrib;
	TIntegerArray lset;

	void Init();

	virtual void Criteria(msql &m, TGroup *);
	virtual void LoadData(const char *s);
	virtual void GetGroupText(mstr &m, TGroup *group, int column);

	virtual bool IsValid();

//{{TEisAliasFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEisAliasFaceVIRTUAL_END}}
};    //{{TEisAliasFace}}
