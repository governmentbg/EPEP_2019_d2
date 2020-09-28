//{{TDialog = TLegalSideAddressDialog}}
class TLegalSideAddressDialog : public TQuickDialog
{
public:
	TLegalSideAddressDialog(TWindow* parent, TGArray<TLegalSide> &tLaids);

protected:
	TLegalSideAddressListBox *laids;

//{{TLegalSideAddressDialogRSP_TBL_BEGIN}}
	void AddressBNEnable(TCommandEnabler &tce);
	void AddressBNClicked();
//{{TLegalSideAddressDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLegalSideAddressDialog);
};    //{{TLegalSideAddressDialog}}
