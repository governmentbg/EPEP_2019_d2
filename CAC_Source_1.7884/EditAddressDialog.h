//{{TDialog = TEditAddressDialog}}
class TEditAddressDialog : public TEditDialog
{
public:
	TEditAddressDialog(TWindow *parent, TAddress *address, bool edit, int tAddrMode);

protected:
	TAreaAliasFace *area;
	TRegionAliasFace *region;
	TFindUCLPAliasFace *uclp;
	TStringFace *email;
	TPushButton *addUCLP;
	TPushButton *editUCLP;
	TFindStreetAliasFace *street;
	TPushButton *addStreet;
	TPushButton *editStreet;
	int addrMode;

	bool CanDirect(long lock);	// can_direct(area, region, lock)
	void SetLock();
	void SetData();

	DEFINE_GROUP_TYPE(TAddress, Address)
	virtual bool IsValid();

//{{TEditAddressDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditAddressDialogVIRTUAL_END}}

//{{TEditAddressDialogRSP_TBL_BEGIN}}
	void AreaCBNSelChange();
	void RegionCBNSelChange();
	void UCLPCBNSelChange();
	void AddUCLPBNClicked();
	void EditUCLPBNClicked();
	void AddStreetBNClicked();
	void EditStreetBNClicked();
	void SearchBNClicked();
//{{TEditAddressDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditAddressDialog);
};    //{{TEditAddressDialog}}
