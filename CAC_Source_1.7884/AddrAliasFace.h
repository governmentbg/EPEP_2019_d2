//{{TComboBox = TAddrAliasFace}}
class TAddrAliasFace : public TLongAliasFace
{
public:
	TAddrAliasFace(TWindow* parent, int resourceId, const char *name, TFindAddrAliasGroup *alias);

	void Reset(long value);

protected:
	TLongAliasGroup *LongAliasGroup() { return (TLongAliasGroup *) alias; }

	//{{TAddrAliasFaceVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAddrAliasFaceVIRTUAL_END}}
};    //{{TAddrAliasFace}}
