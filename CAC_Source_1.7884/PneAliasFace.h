//{{TComboBox = TPneAliasFace}}
class TPneAliasFace : public TGroupComboBox
{
public:
	TPneAliasFace(TWindow* parent, int resourceId, TGArray<PNE> *array);
	virtual ~TPneAliasFace();

	long L() { return ((PNE *) Group())->sid; }
	void SetL(long l);

protected:
	virtual TGroup *NewGroup() const;	// == NULL
};    //{{TPneAliasFace}}
