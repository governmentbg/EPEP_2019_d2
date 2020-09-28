//{{TDialog = TSelectAddressDialog}}
class TSelectAddressDialog : public TAddressListDialog
{
public:
	TSelectAddressDialog(TWindow* parent, TUCNGroup *ucnGroup, TAnyAddress **tAddress, int addrMode, bool intermediate,
		int resId);

protected:
	TAnyAddress **address;
	bool foreign;
	TPushButton *buttons[3];

	void AddAddress(bool foreign);

//{{TSelectAddressDialogRSP_TBL_BEGIN}}
	void AddBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void ForeignBNClicked();
	void ForeignBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
//{{TSelectAddressDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectAddressDialog);
};    //{{TSelectAddressDialog}}
