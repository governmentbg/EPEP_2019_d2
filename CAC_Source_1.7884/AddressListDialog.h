//{{TDialog = TAddressListDialog}}
class TAddressListDialog : public TGroupDialog
{
public:
	TAddressListDialog(TWindow* parent, TUCNGroup *ucnGroup, int addrMode, int resId);

protected:
	TStatic *name;
	TAddressListWindow *addresses;

	DEFINE_GROUP_TYPE(TUCNGroup, UCNGroup)

//{{TAddressListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAddressListDialogVIRTUAL_END}}

//{{TAddressListDialogRSP_TBL_BEGIN}}
	bool AddressesLVNItemChanging(TLwNotify &lwn);
//{{TAddressListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAddressListDialog);
};    //{{TAddressListDialog}}
