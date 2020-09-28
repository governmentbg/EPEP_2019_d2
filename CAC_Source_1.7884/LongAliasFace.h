//{{TComboBox = TLongAliasFace}}
class TLongAliasFace : public TAliasFace
{
public:
	TLongAliasFace(TWindow* parent, int resourceId, const char *name, TLongAliasGroup *alias, const char *xCrit, long flags);

	long L();
	void SetL(long l);

protected:
	long LongAt(int index) { return ((TLongAliasGroup *) GroupAt(index))->ValueField()->L(); }
	virtual void Refresh();

	void FlushList();

//{{TLongAliasFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLongAliasFaceVIRTUAL_END}}
};    //{{TLongAliasFace}}
