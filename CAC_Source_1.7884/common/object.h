// ----- TObject -------------------------------------------------------------
class TObject
{
public:
	TObject(const char *name) : Name(name) { }

	constant Name;	// 2010:345 property-like
};

// ----- TVObject ------------------------------------------------------------
class TVObject : public TObject
{
public:
	TVObject(const char *name) : TObject(name) { }
	virtual ~TVObject() { }
};
