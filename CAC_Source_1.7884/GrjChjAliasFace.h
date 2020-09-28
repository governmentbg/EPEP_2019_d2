//{{TComboBox = TGrjChjAliasFace}}
class TGrjChjAliasFace : public TEisAliasFace
{
public:
	TGrjChjAliasFace(TWindow* parent, int resourceId, const char *name, TIntegerArray *tExclude, TXMLObject *object);

protected:
	TIntegerArray *exclude;

	virtual void LoadData(const char *s);
};    //{{TGrjChjAliasFace}}
