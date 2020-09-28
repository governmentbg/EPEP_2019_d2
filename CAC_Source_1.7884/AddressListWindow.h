//{{TListWindow = TAddressListWindow}}
class TAddressListWindow : public TDoubleListWindow
{
public:
	TAddressListWindow(TWindow* parent, int resourceId, TUCNGroup *tUCNGroup, int tAddrMode);
	virtual ~TAddressListWindow();

	TAnyAddress *Address() { return (TAnyAddress *) Group(); }
	TAnyAddress *AddressAt(int index) { return (TAnyAddress *) GroupAt(index); }
	TAnyAddress *OK();	// destructive, must CmOk() after call

	void Add(bool foreign);
	void Edit();
	void Delete(int command);
	int Count(bool foreign);

protected:
	TUCNGroup *ucnGroup;
	TGSArray<TAnyAddress> addresses;
	int addrMode;

	void Add(TGroupListColumn *column) { TDoubleListWindow::Add(column); }
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);

//{{TAddressListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAddressListWindowVIRTUAL_END}}
};    //{{TAddressListWindow}}
