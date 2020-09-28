//{{TListBox = TDelivererListBox}}
class TDelivererListBox : public TAliasListBox
{
public:
	TDelivererListBox(TWindow* parent, int resourceId);

	TGroupArray *Array() { return array; }

protected:
//{{TDelivererListBoxVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TDelivererListBoxVIRTUAL_END}}
};    //{{TDelivererListBox}}
