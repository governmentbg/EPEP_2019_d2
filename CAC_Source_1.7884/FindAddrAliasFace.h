//{{TComboBox = TFindAddrAliasFace}}
class TFindAddrAliasFace : public TFindLongAliasFace
{
public:
	TFindAddrAliasFace(TWindow* parent, int resourceId, const char *name, uint textLimit, TFindAddrAliasGroup *alias);

	void Reset(long value);
	virtual void Add() = 0;
	virtual void Edit();	// IsValid() then TGroupListWindow::Edit()

protected:
	virtual bool EditGroup(TGroup *group) = 0;
	TLongAliasGroup *LongAliasGroup() { return (TLongAliasGroup *) alias; }

//{{TFindAddrAliasFaceRSP_TBL_BEGIN}}
	void CBNDropDown();
//{{TFindAddrAliasFaceRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFindAddrAliasFace);
};    //{{TFindAddrAliasFace}}
